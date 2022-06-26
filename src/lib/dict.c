#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "emit.h"
#include "traverse.h"
#include "parse.h"
#include "gwi.h"
#include "tmpl_info.h"
#include "array.h"
#include "looper.h"
#include "dict.h"

#define HMAP_MIN_CAP 32
#define HMAP_MAX_LOAD 0.75

typedef void (clear_fn)(const HMap*, const VM_Shred, const struct HMapInfo*, const m_uint);

// TODO: arch sensible hash
static SFUN(mfun_int_h) {
  m_int x = *(m_uint*)MEM(0);
  x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
  x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
  x = x ^ (x >> 31);
  *(m_int*) RETURN = x;
}

static SFUN(mfun_float_h) {
  char c[1024];
  snprintf(c, 1024, "%f",  *(m_float*)MEM(0));
  *(m_uint*)RETURN = hash(c);
}

static SFUN(mfun_string_h) {
  *(m_int*)RETURN = hash(STRING(*(M_Object*)MEM(0)));
}

ANN static void clear_oo(const HMap *a, const VM_Shred shred, const HMapInfo *info NUSED, const m_uint idx) {
  release(*(M_Object*)(a->data + idx * SZ_INT*2), shred);
  release(*(M_Object*)((a->data + idx * SZ_INT*2) + SZ_INT), shred);
}

ANN static void clear_ss(const HMap *a, const VM_Shred shred, const HMapInfo *info, const m_uint idx) {
  struct_release(shred, info->key, a->data + idx * info->sz);
  struct_release(shred, info->val, a->data + idx * info->sz + info->key->size);
}

ANN static void clear_os(const HMap *a, const VM_Shred shred, const HMapInfo *info, const m_uint idx) {
  release(*(M_Object*)(a->data + idx * info->sz), shred);
  struct_release(shred, info->val, a->data + idx * info->sz + SZ_INT);
}

ANN static void clear_so(const HMap *a, const VM_Shred shred, const HMapInfo *info, const m_uint idx) {
  struct_release(shred, info->key, a->data + idx * info->sz);
  release(*(M_Object*)(a->data + idx * info->sz + info->key->size), shred);
}

ANN static void clear_on(const HMap *a, const VM_Shred shred, const HMapInfo *info, const m_uint idx) {
  release(*(M_Object*)(a->data + idx * info->sz), shred);
}

ANN static void clear_sn(const HMap *a, const VM_Shred shred, const HMapInfo *info, const m_uint idx) {
  struct_release(shred, info->key, a->data + idx * info->sz);
}

ANN static void clear_no(const HMap *a, const VM_Shred shred, const HMapInfo *info, const m_uint idx) {
  release(*(M_Object*)(a->data + idx * info->sz + info->key->size), shred);
}


ANN static void clear_ns(const HMap *a, const VM_Shred shred, const HMapInfo *info, const m_uint idx) {
  struct_release(shred, info->val, a->data + idx * info->sz + info->key->size);
}

static clear_fn *const n_clear[3]  = { NULL,     clear_no, clear_ns };
static clear_fn* o_clear[3]  = { clear_on, clear_oo, clear_os };
static clear_fn* s_clear[3]  = { clear_sn, clear_so, clear_ss };
static clear_fn*const* clear[3] = { n_clear, o_clear, s_clear };

ANN static void hmapinfo_init(HMapInfo *const info, const Type types[], const Type key, const Type val) {
  info->key = key;
  info->val = val;
  info->sz = key->size + val->size;
  info->keyk = isa(key, types[et_compound]) > 0 + tflag(key, tflag_struct);
  info->valk = isa(val, types[et_compound]) > 0 + tflag(val, tflag_struct);
}

static DTOR(dict_clear_dtor) {
  const HMapInfo *hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  clear_fn *fn = clear[hinfo->keyk][hinfo->valk];
  HMap *a = &*(struct HMap*)o->data;
  for(m_uint i = a->capacity; --i;) {
    const HState state = *(HState*)(a->state + (i-1) * sizeof(HState));
    if(!state.set || state.deleted) continue;
    fn(a, shred, hinfo, i-1);
  }
}

INSTR(dict_ctor_alt) {
  const Type t = (Type)instr->m_val2;
  const M_Object o = new_object(shred->info->mp, t);
  const HMapInfo *hinfo = (HMapInfo*)t->nspc->class_data;
  HMap *a = &*(struct HMap*)o->data;
  a->data  = (m_bit*)mp_calloc2(shred->info->mp, hinfo->sz * instr->m_val);
  a->state = (m_bit*)mp_calloc2(shred->info->mp, sizeof(HState) * instr->m_val);
  a->capacity = instr->m_val;
  shred->reg += SZ_INT;
  *(M_Object*)REG(-SZ_INT) = o;
}

INSTR(dict_lit_ctor) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  const HMapInfo *hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  HMap *hmap = &*(struct HMap*)o->data;
  for(m_uint i = 0; i < instr->m_val; i += hinfo->sz + SZ_INT) {
    m_uint hash = *(m_uint*)REG(i + hinfo->sz) % hmap->capacity;
    while(true) {
      HState *const state = (HState*)(hmap->state + hash * sizeof(HState));
      if(!state->set) {
        m_bit *const data = hmap->data + hinfo->sz * hash;
        memcpy(data, REG(i), hinfo->sz);
        state->set = true;
        break;
      }
      if(++hash >= hmap->capacity)
        hash = 0;
    }
  }
  hmap->count = instr->m_val2;
}

static CTOR(dict_ctor) {
  const HMapInfo *hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  HMap *const a = &*(struct HMap*)o->data;
  a->data       = (m_bit*)mp_calloc2(shred->info->mp, hinfo->sz * HMAP_MIN_CAP);
  a->state      = (m_bit*)mp_calloc2(shred->info->mp, sizeof(HState) * HMAP_MIN_CAP);
  a->capacity   = HMAP_MIN_CAP;
  a->count      = 0;
}

static DTOR(dict_dtor) {
  HMap *const a = &*(struct HMap*)o->data;
  const HMapInfo *hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  mp_free2(shred->info->mp, hinfo->sz * a->capacity, a->data);
  mp_free2(shred->info->mp, sizeof(HState) * a->capacity, a->state);
}

// bound the hash
// could be put in next func
static INSTR(hmap_iter_set_ini) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT*2 - instr->m_val);
  const HMap *hmap = (HMap*)o->data;
  const size_t h =  *(m_uint*)REG(-SZ_INT - instr->m_val);
  if(h >= hmap->capacity)
    *(m_uint*)REG(-SZ_INT - instr->m_val) = h % hmap->capacity;
}

static INSTR(hmpa_set_inc) {
  (*(m_uint*)REG(-SZ_INT - instr->m_val))++;
}

static INSTR(hmap_iter_set) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT*2 - instr->m_val);
  HMap *const hmap = (HMap*)o->data;
  const HMapInfo *hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  const size_t bucket =  *(m_uint*)REG(-SZ_INT - instr->m_val);
  HState *const state = (HState*)(hmap->state + sizeof(HState) * bucket);
  m_bit *const data = hmap->data + hinfo->sz * bucket;
  if (!state->set || state->deleted) {

    if(hinfo->keyk) {
      if(hinfo->keyk == HKIND_OBJ)
        (*(M_Object*)REG(-instr->m_val))->ref++;
      else
        struct_addref(shred->info->vm->gwion, hinfo->key, REG(-instr->m_val));
    }

    state->set     = true;
    state->deleted = false;
    memcpy(data, REG(-instr->m_val), instr->m_val);
    POP_REG(shred, instr->m_val);
    *(m_bit**)REG(-SZ_INT*2) = data + hinfo->key->size;
    *(m_uint*)REG(-SZ_INT) = 1;
    hmap->count++;
  } else {
    memcpy(REG(0), data, hinfo->key->size);
    shred->reg += SZ_INT + hinfo->key->size;
    *(m_uint**)REG(-SZ_INT) = 0;
  }
}

static INSTR(hmap_iter_inc) {
  (*(m_uint*)(shred->reg - SZ_INT))++;
}

static INSTR(hmap_iter) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT*3);
  const HMap *hmap = (HMap*)o->data;
  const HMapInfo *hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  size_t bucket =  *(m_uint*)(shred->reg - SZ_INT) % hmap->capacity;
  const HState *state = (HState*)(hmap->state + sizeof(HState) * bucket);
  if (state->set) {
    m_int *const tombstone = (m_int*)(shred->reg - SZ_INT*2);
    if (state->deleted && *tombstone == -1) {
      *tombstone = bucket++;
    }
    const m_bit *data = hmap->data + hinfo->sz * bucket;
    *(m_uint*)(shred->reg - SZ_INT) = bucket;
    memcpy(REG(0), data, hinfo->key->size);
    shred->reg += hinfo->key->size;
    return;
  }
  handle(shred, "InvalidMapAccess");
}

static INSTR(hmap_grow) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT);
  const HMap *hmap = (HMap*)o->data;
  *(m_uint*)REG(0) = (hmap->count + 1) > (hmap->capacity * HMAP_MAX_LOAD);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(hmap_grow_init) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT);
  HMap *const hmap = (HMap*)o->data;
  const HMapInfo *hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  *(m_bit**)REG(0) = hmap->data;
  *(m_bit**)REG(SZ_INT) = hmap->state;
  *(m_uint*)REG(SZ_INT*2) = hmap->capacity;
  hmap->capacity *= 2;
  hmap->count = 0;
  hmap->state = mp_calloc2(shred->info->mp, hmap->capacity * sizeof(HState));
  hmap->data = mp_calloc2(shred->info->mp, hmap->capacity * hinfo->sz);

  PUSH_REG(shred, SZ_INT*3);
}

static INSTR(hmap_grow_dec) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT*4);
  const HMapInfo *hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  const m_bit *old_data = *(m_bit**)REG(-SZ_INT*3);
  const m_bit *old_state = *(m_bit**)REG(-SZ_INT*2);
  while((*(m_uint*)REG(-SZ_INT))--) {
    const HState *state = (HState *)(old_state + (*(m_uint*)REG(-SZ_INT)) * sizeof(HState));
    if(!state->set || state->deleted)continue;
    m_bit *const data   = (m_bit*)(old_data + (*(m_uint*)REG(-SZ_INT)) * hinfo->sz);
    memcpy(shred->reg + SZ_INT, data, hinfo->key->size);
    PUSH_REG(shred, SZ_INT + hinfo->key->size);
    *(m_uint*)shred->reg = 0;
    PUSH_REG(shred, SZ_INT);
    return;
  }
  POP_REG(shred, SZ_INT*2);
  *(m_uint*)(shred->reg -SZ_INT) = 1;
}

static INSTR(hmap_find) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT*6);
  HMap *const hmap = (HMap*)o->data;
  const HMapInfo *hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  const m_bit *old_data = *(m_bit**)REG(-SZ_INT*5);
  const m_bit *old_state = *(m_bit**)REG(-SZ_INT*4);
  const m_uint h = *(m_uint*)REG(-SZ_INT);
  const m_uint idx = *(m_uint*)REG(-SZ_INT*3);
  m_uint bucket = h % hmap->capacity;
  while(true) {
    HState *const state = (HState *)(hmap->state + bucket * sizeof(HState));
    if(!state->set) {
      const HState *prev_state = (HState *)(old_state + idx * sizeof(HState));
      memcpy(state, prev_state, sizeof(HState));
      m_bit *const data = hmap->data + bucket * hinfo->sz;
      const m_bit *prev_data = old_data + idx * hinfo->sz;
      memcpy(data, prev_data, hinfo->sz);
      hmap->count++;
      break;
    }
    if(++bucket > hmap->capacity)
      bucket = 0;
  }
  if(!idx)
    POP_REG(shred, SZ_INT*4 - instr->m_val)
  else
    POP_REG(shred, SZ_INT)
  *(m_uint*)REG(-SZ_INT) = !idx;
}

static INSTR(hmap_addr) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT*2 - instr->m_val);
  const HMap *hmap = (HMap*)o->data;
  const HMapInfo *hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  POP_REG(shred, SZ_INT + instr->m_val);
  const m_uint bucket = *(m_uint*)REG(0);
  *(HState *)(hmap->state + bucket * sizeof(HState)) = (HState) { true, false };
  *(m_bit**)REG(-SZ_INT) = hmap->data + hinfo->sz * bucket + hinfo->key->size;
}

static INSTR(hmap_val) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT*2);
  const HMap *hmap = (HMap*)o->data;
  const HMapInfo *hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  const m_uint bucket = *(m_uint*)REG(0);
  const m_bit *new_data = hmap->data + hinfo->sz * bucket;
  const m_int tombstone = *(m_int*)(shred->reg - SZ_INT);
  if (tombstone != -1) {
    m_bit  *const old_data = hmap->data + (hinfo->key->size + hinfo->val->size) * tombstone;
    HState *const old_state = (HState*)(hmap->state + sizeof(HState) * tombstone);
    HState *const new_state = (HState*)(hmap->state + sizeof(HState) * bucket);
    memcpy(old_state, new_state, sizeof(HState));
    memcpy(old_data, new_data, hinfo->sz);
    new_state->deleted = true;
 }

  shred->reg -= SZ_INT*2 - hinfo->val->size;
  memcpy(REG(-hinfo->val->size), new_data + hinfo->key->size, hinfo->val->size);
}

static INSTR(hmap_remove_clear) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT*2);
  const HMap *hmap = (HMap*)o->data;
  const HMapInfo *hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  const m_uint bucket = *(m_uint*)REG(0);
  clear_fn *fn = (clear_fn*)instr->m_val;
  fn(hmap, shred, hinfo, bucket);
}

static INSTR(hmap_remove) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT*2);
  HMap *const hmap = (HMap*)o->data;
  const HMapInfo *hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  const m_uint bucket = *(m_uint*)REG(0);
  m_bit *data = hmap->data + hinfo->sz * bucket;
  hmap->count--;
  HState *const state = (HState *)(hmap->state + bucket * sizeof(HState));
  state->deleted = true;
  shred->reg -= SZ_INT*3 - hinfo->val->size;
  memcpy(REG(-hinfo->val->size), data + hinfo->key->size, hinfo->val->size);
}

static OP_CHECK(opck_dict_remove_toop) {
  const Exp       e    = (Exp)data;
  const Exp_Call *call = &e->d.exp_call;
  const Exp       func  = call->func;
  const Exp       args  = call->args;
  e->exp_type         = ae_exp_binary;
  CHECK_OO(check_exp(env, e->d.exp_binary.rhs = cpy_exp(env->gwion->mp, func->d.exp_dot.base)));
  CHECK_OO(check_exp(env, e->d.exp_binary.lhs = args));
  e->d.exp_binary.op = insert_symbol("~~");
  free_exp(env->gwion->mp, func);
  const Type t = e->d.exp_binary.rhs->type;
  HMapInfo *const hinfo = (HMapInfo*)t->nspc->class_data;
  if(isa(args->type, hinfo->key) < 0 || args->next)
    ERR_N(e->pos, "dict.remove must be called with one Key argument");
  return e->type = hinfo->val;
}

ANN static m_bool emit_dict_iter(const Emitter emit, const HMapInfo *hinfo,
                          const struct Op_Import *opi, const Exp call, const Exp exp) {
  const Instr room_for_tombstone = emit_add_instr(emit, RegPushImm);
  room_for_tombstone->m_val = -1;
  CHECK_BB(emit_exp(emit, call));
  const m_uint pc = emit_code_size(emit);
  const Instr iter = emit_add_instr(emit, hmap_iter);
  iter->m_val = hinfo->key->size + SZ_INT;
  CHECK_BB(emit_exp(emit, exp));
  op_emit(emit, opi);
  const Instr ok = emit_add_instr(emit, BranchNeqInt);
  emit_add_instr(emit, hmap_iter_inc);
  const Instr top = emit_add_instr(emit, Goto);
  top->m_val = pc;
  ok->m_val = emit_code_size(emit);
  const Instr _pop = emit_add_instr(emit, RegMove);
  _pop->m_val = -SZ_INT;// - key->size;
  return GW_OK;
}

static OP_EMIT(_opem_dict_access) {
  struct ArrayAccessInfo *const info = (struct ArrayAccessInfo *const)data;
  Array_Sub array = &info->array;
  const Env env = emit->env;
  const HMapInfo *hinfo = (HMapInfo*)array->type->nspc->class_data;
  const Type key = *(Type*)array->type->nspc->class_data;

  struct Exp_ func = { .exp_type = ae_exp_primary, .d = { .prim = { .prim_type = ae_prim_id, .d = { .var = insert_symbol("hash") }} }};

  struct Exp_ call = {
   .exp_type = ae_exp_call,
   .d = {
     .exp_call = {
       .func = &func,
       .args = array->exp // beware next
     }
   }
};
struct Exp_ lhs = { .exp_type = ae_exp_primary, .type = key, .d = { .prim = { .prim_type = ae_prim_id } }};
struct Exp_ rhs = { .exp_type = ae_exp_primary, .type = key, .d = { .prim = { .prim_type = ae_prim_id } }};
struct Exp_ bin = { .exp_type = ae_exp_binary, .d = { .exp_binary = { .lhs = &lhs, .rhs = &rhs, .op = insert_symbol("?=") } }};
struct Op_Import opi = {
  .lhs = key,
  .rhs = key,
  .op  = bin.d.exp_binary.op,
  .data = (m_uint)&bin
};


CHECK_BB(traverse_exp(env, &call));
if(info->is_var) {

  const Instr instr = emit_add_instr(emit, hmap_grow);
  instr->m_val = key->size;
  const Instr nogrow = emit_add_instr(emit, BranchEqInt);
  emit_add_instr(emit, hmap_grow_init);
  const m_uint grow_pc = emit_code_size(emit);
  emit_add_instr(emit, hmap_grow_dec);
  const Instr endgrow = emit_add_instr(emit, BranchNeqInt);
  emit_exp(emit, call.d.exp_call.func);
  emit_exp_call1(emit, call.d.exp_call.func->type->info->func, true);
  emit_add_instr(emit, hmap_find);
  const Instr regrow = emit_add_instr(emit, BranchEqInt);
  regrow->m_val = grow_pc;
  nogrow->m_val = emit_code_size(emit);
  endgrow->m_val = emit_code_size(emit);
  CHECK_BB(emit_exp(emit, &call));
  CHECK_BB(emit_exp(emit, array->exp));
  const m_uint top_pc = emit_code_size(emit);
  const Instr idx = emit_add_instr(emit, hmap_iter_set_ini);
  idx->m_val = key->size;


  const Instr iter = emit_add_instr(emit, hmap_iter_set);
  iter->m_val = key->size;
  const Instr fast = emit_add_instr(emit, BranchNeqInt);
  CHECK_BB(emit_exp(emit, array->exp));
  op_emit(emit, &opi);

  const Instr ok = emit_add_instr(emit, BranchNeqInt);
  const Instr inc = emit_add_instr(emit, hmpa_set_inc);
  inc->m_val = key->size;
  const Instr not_ok = emit_add_instr(emit, Goto);
  not_ok->m_val = top_pc;
  ok->m_val = emit_code_size(emit);

  const Instr iseq = emit_add_instr(emit, hmap_addr);
  iseq->m_val = key->size;
  fast->m_val = emit_code_size(emit);
  return GW_OK;
}
  CHECK_BB(emit_dict_iter(emit, hinfo, &opi, &call, array->exp));
  const Instr pushval = emit_add_instr(emit, hmap_val);
  pushval->m_val2 = key->size;
  return GW_OK;
}

static OP_EMIT(opem_dict_remove) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Env env = emit->env;
  struct Exp_ func = { .exp_type = ae_exp_primary, .d = { .prim = { .prim_type = ae_prim_id, .d = { .var = insert_symbol("hash") }} }};

  struct Exp_ call = {
    .exp_type = ae_exp_call,
    .d = {
      .exp_call = {
        .func = &func,
        .args = bin->lhs // beware next
      }
    }
  };
  const Type t = bin->rhs->type;
  HMapInfo *const hinfo = (HMapInfo*)t->nspc->class_data;

  struct Exp_ lhs = { .exp_type = ae_exp_primary, .type = hinfo->key, .d = { .prim = { .prim_type = ae_prim_id } }};
  struct Exp_ rhs = { .exp_type = ae_exp_primary, .type = hinfo->key, .d = { .prim = { .prim_type = ae_prim_id } }};
  struct Exp_ _bin = { .exp_type = ae_exp_binary, .d = { .exp_binary = { .lhs = &lhs, .rhs = &rhs, .op = insert_symbol("?=") } }};
  struct Op_Import opi = {
    .lhs = hinfo->key,
    .rhs = hinfo->key,
    .op  = _bin.d.exp_binary.op,
    .data = (m_uint)&_bin
  };

  CHECK_BB(traverse_exp(env, &call));
  CHECK_BB(emit_dict_iter(emit, hinfo, &opi, &call, bin->lhs));
  if(hinfo->keyk || hinfo->valk) {
    clear_fn *const fn = clear[hinfo->keyk][hinfo->valk];
    const Instr instr = emit_add_instr(emit, hmap_remove_clear);
    instr->m_val = (m_uint)fn;
  }

  const Instr pushval = emit_add_instr(emit, hmap_remove);
  pushval->m_val2 = hinfo->key->size;
  return GW_OK;
}

ANN static m_bool emit_next_access(const Emitter emit, struct ArrayAccessInfo *const info) {
  const struct Array_Sub_ array = info->array;
  HMapInfo *const hinfo = (HMapInfo*)info->array.type->nspc->class_data;
  info->array = (struct Array_Sub_){
    .exp   = array.exp->next,
    .type  = hinfo->val,
    .depth = array.depth - 1
  };
  return emit_array_access(emit, info);
}

static OP_EMIT(opem_dict_access) {
  struct ArrayAccessInfo *const info = (struct ArrayAccessInfo *const)data;
  const Array_Sub array = &info->array;
  const Exp enext = array->exp->next;
  array->exp->next = NULL;
  _opem_dict_access(emit, data);
  array->exp->next = enext;
  return !enext ? GW_OK : emit_next_access(emit, info);
}

static OP_CHECK(opck_dict_access) {
  const Array_Sub array = (Array_Sub)data;
  HMapInfo *const hinfo = (HMapInfo*)array->type->nspc->class_data;

  struct Exp_ func = { .exp_type = ae_exp_primary, .d = { .prim = { .prim_type = ae_prim_id, .d = { .var = insert_symbol("hash") }} }};

  struct Exp_ call = {
   .exp_type = ae_exp_call,
   .d = {
     .exp_call = {
       .func = &func,
       .args = array->exp // beware next
     }
   }
};
struct Exp_ lhs = { .exp_type = ae_exp_primary, .type = hinfo->key, .d = { .prim = { .prim_type = ae_prim_id } }};
struct Exp_ rhs = { .exp_type = ae_exp_primary, .type = hinfo->key, .d = { .prim = { .prim_type = ae_prim_id } }};
struct Exp_ bin = { .exp_type = ae_exp_binary, .d = { .exp_binary = { .lhs = &lhs, .rhs = &rhs, .op = insert_symbol("?=") } }};
struct Op_Import opi = {
  .lhs = hinfo->key,
  .rhs = hinfo->key,
  .op  = bin.d.exp_binary.op,
  .data = (m_uint)&bin
};


  CHECK_BN(traverse_exp(env, &call));
  CHECK_ON(op_check(env, &opi));

  if(!array->exp->next) return hinfo->val;
  struct Array_Sub_ next = { array->exp->next, hinfo->val,
                            array->depth - 1};
  return check_array_access(env, &next) ?: env->gwion->type[et_error];
}

static INSTR(DictEach) {
  const M_Object o = *(M_Object *)(shred->mem + instr->m_val2);
  HMapInfo *const hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  const HMap *hmap = (HMap*)o->data;
  m_uint bucket = ++*(m_uint *)(shred->mem + instr->m_val2 + SZ_INT);
  while(bucket < hmap->capacity) {
    const HState *state = (HState *)(hmap->state + bucket * sizeof(HState));
    if (state->set && !state->deleted)
      break;
    bucket++;
  }
  *(m_uint *)(shred->mem + instr->m_val2 + SZ_INT) = bucket;
  memcpy(shred->mem + instr->m_val2 + SZ_INT*2, hmap->data + (bucket * hinfo->sz) + hinfo->key->size, hinfo->val->size);
  *(m_uint*)shred->reg = (bucket == hmap->capacity);
  PUSH_REG(shred, SZ_INT);
}

static INSTR(DictEachIdx) {
  const M_Object o = *(M_Object *)(shred->mem + instr->m_val2);
  HMapInfo *const hinfo = (HMapInfo*)o->type_ref->nspc->class_data;
  const HMap *hmap = (HMap*)o->data;
  DictEach(shred, instr);
  const m_int bucket = *(m_uint *)(shred->mem + instr->m_val2 + SZ_INT);
  memcpy(shred->mem + instr->m_val, hmap->data + (bucket * hinfo->sz), hinfo->key->size);
}

static OP_EMIT(opem_dict_each) {
  Looper *loop = (Looper *)data;
  HMapInfo *const hinfo = (HMapInfo*)loop->exp->type->nspc->class_data;
  if(loop->idx && !loop->init) loop->idx->v->from->offset = emit_localn(emit, hinfo->key);
  const Instr instr = emit_add_instr(emit, !loop->idx ? DictEach : DictEachIdx);
  instr->m_val2 = loop->offset;
  if(loop->idx) instr->m_val = loop->idx->v->from->offset;
  if(loop->n)instr->m_val2 += SZ_INT;
  const Instr go = emit_add_instr(emit, BranchNeqInt);
  if(!loop->n) loop->instr = go;
  else vector_add(&loop->unroll_v, (m_uint)go);
  loop->init = true;
  return GW_OK;
}

static INSTR(DictEachInit) {
  const M_Object o = *(M_Object *)(shred->mem + instr->m_val + SZ_INT);
  *(m_uint *)(shred->mem + instr->m_val) = ((HMap*)o->data)->capacity;
}

static OP_EMIT(opem_dict_each_init) {
  const Looper *loop = (Looper *)data;
  const Instr instr = emit_add_instr(emit, DictEachInit);
  instr->m_val = loop->offset;
  return GW_OK;
}

static OP_CHECK(opck_dict_each_key) {
  const Exp exp = (const Exp)data;
  HMapInfo *const hinfo = (HMapInfo*)exp->type->nspc->class_data;
  return hinfo->key;
}

static OP_CHECK(opck_dict_each_val) {
  const Exp exp = (const Exp)data;
  HMapInfo *const hinfo = (HMapInfo*)exp->type->nspc->class_data;
  return hinfo->val;
}

static OP_CHECK(opck_dict_scan) {
  struct TemplateScan *ts   = (struct TemplateScan *)data;
  struct tmpl_info     info = {
      .base = ts->t, .td = ts->td, .list = ts->t->info->cdef->base.tmpl->list};
  const Type  exists = tmpl_exists(env, &info);
  if (exists) return exists != env->gwion->type[et_error] ? exists : NULL;
  if(!ts->td->types || ts->td->types->len != 2) return env->gwion->type[et_error];
  DECL_ON(const Type, key, = known_type(env, *mp_vector_at(ts->td->types, Type_Decl*, 0)));
  DECL_ON(const Type, val, = known_type(env, *mp_vector_at(ts->td->types, Type_Decl*, 1)));
  if(tflag(key, tflag_ref) || tflag(val, tflag_ref))
    ERR_N(ts->td->pos, "can't use Ref:[] in dicts");
  const Class_Def cdef  = cpy_class_def(env->gwion->mp, env->gwion->type[et_dict]->info->cdef);
  cdef->base.ext        = type2td(env->gwion, env->gwion->type[et_dict], (loc_t) {});
  cdef->base.xid        = info.name;
  cdef->base.tmpl->call = cpy_type_list(env->gwion->mp, info.td->types);

  (void)scan0_class_def(env, cdef);
  const Type   t   = cdef->base.type;
  t->nspc->class_data_size = sizeof(struct HMapInfo);
  const m_bool ret = traverse_cdef(env, t);
  HMapInfo *const hinfo = (HMapInfo*)t->nspc->class_data;
  hmapinfo_init(hinfo, env->gwion->type, key, val);
  if(hinfo->keyk + hinfo->valk) {
    t->nspc->dtor = new_vmcode(env->gwion->mp, NULL, NULL, "@dtor", SZ_INT, true, false);
    t->nspc->dtor->native_func = (m_uint)dict_clear_dtor;
    set_tflag(t, tflag_dtor);
  }
  struct Op_Func opfunc = { .ck = opck_dict_access, .em = opem_dict_access };
  struct Op_Import opi = { .lhs = key, .rhs = t, .ret = val, .op = insert_symbol("@array"), .func = &opfunc };
  add_op(env->gwion, &opi);
  opi.op = insert_symbol("~~");
  opfunc.em = opem_dict_remove;
  add_op(env->gwion, &opi);

  {
  const Func             f      = (Func)vector_front(&t->nspc->vtable);
  const struct Op_Func   opfunc = {.ck = opck_dict_remove_toop};
  const struct Op_Import opi    = {
      .rhs  = f->value_ref->type,
      .func = &opfunc,
      .data = (uintptr_t)f,
      .op   = insert_symbol("@func_check")};
  CHECK_BN(add_op(env->gwion, &opi));

  }

  return ret > 0 ? t : NULL;
}

GWION_IMPORT(dict) {
//  gwidoc(gwi, "Ref: take a reference from a variable.");
//  gwinote(gwi, "used just as the variable it reference.");
//  gwinote(gwi, "can only be used as argument.");
//  gwinote(gwi, "and cannot be returned.");


  DECL_OB(const Type, t_dict, = gwi_class_ini(gwi, "Dict:[Key,Val]", "Object"));
  gwi_class_xtor(gwi, dict_ctor, dict_dtor);
  t_dict->nspc->offset += sizeof(struct HMap);
  gwi->gwion->type[et_dict] = t_dict;

  GWI_BB(gwi_func_ini(gwi, "bool",   "remove"));
  GWI_BB(gwi_func_arg(gwi, "Key",    "key"));
  GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_none));

  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, "Dict", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_dict_scan))
  GWI_BB(gwi_oper_end(gwi, "@scan", NULL))

  GWI_BB(gwi_oper_ini(gwi, "Dict", NULL, "int"))
  GWI_BB(gwi_oper_emi(gwi, opem_dict_each))
  GWI_BB(gwi_oper_end(gwi, "@each", NULL))

  GWI_BB(gwi_oper_ini(gwi, "Dict", NULL, "void"))
  GWI_BB(gwi_oper_emi(gwi, opem_dict_each_init))
  GWI_BB(gwi_oper_end(gwi, "@each_init", NULL))

  GWI_BB(gwi_oper_ini(gwi, "Dict", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_dict_each_val))
  GWI_BB(gwi_oper_end(gwi, "@each_val", NULL))

  GWI_BB(gwi_oper_ini(gwi, "Dict", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_dict_each_key))
  GWI_BB(gwi_oper_end(gwi, "@each_idx", NULL))

  GWI_BB(gwi_func_ini(gwi, "int",    "hash"));
  GWI_BB(gwi_func_arg(gwi, "int",    "key"));
  GWI_BB(gwi_func_end(gwi, mfun_int_h, ae_flag_none));

  GWI_BB(gwi_func_ini(gwi, "int",    "hash"));
  GWI_BB(gwi_func_arg(gwi, "Object",    "key"));
  GWI_BB(gwi_func_end(gwi, mfun_int_h, ae_flag_none));

  GWI_BB(gwi_func_ini(gwi, "int",    "hash"));
  GWI_BB(gwi_func_arg(gwi, "float",    "key"));
  GWI_BB(gwi_func_end(gwi, mfun_float_h, ae_flag_none));

  GWI_BB(gwi_func_ini(gwi, "int",    "hash"));
  GWI_BB(gwi_func_arg(gwi, "time",    "key"));
  GWI_BB(gwi_func_end(gwi, mfun_float_h, ae_flag_none));

  GWI_BB(gwi_func_ini(gwi, "int",    "hash"));
  GWI_BB(gwi_func_arg(gwi, "dur",    "key"));
  GWI_BB(gwi_func_end(gwi, mfun_float_h, ae_flag_none));

  GWI_BB(gwi_func_ini(gwi, "int",    "hash"));
  GWI_BB(gwi_func_arg(gwi, "string",    "key"));
  GWI_BB(gwi_func_end(gwi, mfun_string_h, ae_flag_none));

  return GW_OK;
}
