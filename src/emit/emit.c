#include <limits.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "emit.h"
#include "object.h"
#include "array.h"
#include "traverse.h"
#include "template.h"
#include "escape.h"
#include "parse.h"
#include "memoize.h"
#include "operator.h"
#include "import.h"
#include "match.h"
#include "specialid.h"
#include "looper.h"

#undef insert_symbol
#define insert_symbol(a) insert_symbol(emit->gwion->st, (a))

#undef ERR_B
#define ERR_B(a, b, ...)                                                       \
  {                                                                            \
    env_err(emit->env, (a), (b), ##__VA_ARGS__);                               \
    return false;                                                              \
  }
#undef ERR_O
#define ERR_O(a, b, ...)                                                       \
  {                                                                            \
    env_err(emit->env, (a), (b), ##__VA_ARGS__);                               \
    return NULL;                                                               \
  }

typedef struct Local_ {
  Type   type;
  m_uint offset;
  Instr instr;
  bool   skip;
  bool   is_compound;
} Local;

static inline void emit_pop(const Emitter emit, const m_uint scope) {
  env_pop(emit->env, scope);
}
static inline m_uint emit_pushv(const Emitter emit, const Value value) {
  return env_pushv(emit->env, value);
}

static inline m_uint emit_push_global(const Emitter emit) {
  return env_push_global(emit->env);
}


ANEW static Frame *new_frame(MemPool p) {
  Frame *frame = mp_calloc(p, Frame);
  vector_init(&frame->stack);
  vector_add(&frame->stack, (vtype)NULL);
  vector_init(&frame->defer);
  vector_add(&frame->defer, (vtype)NULL);
  return frame;
}

ANN static void release_maybe_stack(const MemPool mp, MaybeValList * ms) {
  for (vtype i = ms->len + 1; --i;) {
    const MaybeVal mv = maybevallist_at(ms, i - 1);
    struct M_Vector_ v = { .ptr = mv.ptr };
    m_vector_release(&v);
  }
  free_maybevallist(mp, ms);
}

ANN static void free_frame(MemPool p, Frame *a) {
  LOOP_OPTIM
  for (vtype i = vector_size(&a->stack) + 1; --i;)
    if (vector_at(&a->stack, i - 1))
      mp_free(p, Local, (Local *)vector_at(&a->stack, i - 1));
  vector_release(&a->stack);
  if(a->maybe_stack)
    release_maybe_stack(p, a->maybe_stack);
  vector_release(&a->defer);
  if (a->handlers.ptr) map_release(&a->handlers);
  mp_free(p, Frame, a);
}

ANN static inline Local *new_local(MemPool p, const Type type) {
  Local *const local = mp_calloc(p, Local);
  local->type  = type;
  return local;
}

ANN static Local *frame_local(MemPool p, Frame *frame, const Type t) {
  Local *const l = new_local(p, t);
  l->offset      = frame->curr_offset;
  frame->curr_offset += t->size;
  vector_add(&frame->stack, (vtype)l);
  return l;
}

ANN static inline void frame_push(Frame *frame) {
  vector_add(&frame->stack, (vtype)NULL);
  vector_add(&frame->defer, (vtype)NULL);
}

static const f_instr allocmember[4] = {RegPushImm, RegPushImm2, _staticmemset_,
                                      DotMemberMem4};
static const f_instr allocword[4]   = {AllocWord, AllocWord2, AllocWord3,
                                    RegPushMem4};

ANN static bool emit_class_def(const Emitter, const Class_Def);
ANN static bool emit_cdef(const Emitter, const Type);

ANN static bool ensure_emit(const Emitter emit, const Type t) {
  if (tflag(t, tflag_emit) || !(tflag(t, tflag_cdef) || tflag(t, tflag_udef)))
    return true;
  if(!tflag(t, tflag_tmpl)) return true;
  struct EnvSet es = {.env   = emit->env,
                      .data  = emit,
                      .func  = (_envset_func)emit_cdef,
                      .scope = emit->env->scope->depth,
                      .flag  = tflag_emit};
  return envset_run(&es, t);
}

ANN void emit_object_release(const Emitter emit, const m_uint offset) {
  const Instr instr = emit_add_instr(emit, ObjectRelease);
  instr->m_val = offset;
}

ANN void emit_compound_release(const Emitter emit, const Type t, const m_uint offset) {
  if(tflag(t, tflag_compound))
    return emit_object_release(emit, offset);
  emit_struct_release(emit, t, offset);
}

ANN void emit_struct_release(const Emitter emit, const Type type,
                             const m_uint offset) {
  if(!tflag(type, tflag_release)) return;
  f_instr exec = tflag(type, tflag_union)
    ? UnionReleaseMem
    : StructReleaseMem;
  const Instr instr = emit_add_instr(emit, exec);
  instr->m_val = offset;
  instr->m_val2 = (m_uint)type;
}

ANN static bool emit_stmt(const Emitter emit, Stmt* stmt);

ANN static bool emit_defers(const Emitter emit) {
  const Vector v = &emit->code->frame->defer;
  if (!vector_size(v)) return true;
  m_uint i;
  for(i = vector_size(v) + 1; --i;) {
    Stmt* s = (Stmt*)vector_at(v, i - 1);
    if(s) CHECK_B(emit_stmt(emit, s));
  }
  VLEN(v) = i;
  return true;
}

ANN static bool emit_defers2(const Emitter emit) {
  const Vector v = &emit->code->frame->defer;
  for (m_uint i = vector_size(v) + 1; --i;) {
    Stmt* s = (Stmt*)vector_at(v, i - 1);
    if (!s) break;
    CHECK_B(emit_stmt(emit, s));
  }
  return true;
}

ANN static m_int _frame_pop(const Emitter emit) {
  Frame *frame = emit->code->frame;
  const Local *l = (Local *)vector_pop(&frame->stack);
  if(!l)return -1;
  frame->curr_offset -= l->type->size;
  if (l->skip) return _frame_pop(emit);
  if (!l->is_compound) return _frame_pop(emit);
  VMValue *vmval =
      (VMValue *)m_vector_addr(&emit->code->live_values, --frame->value_count);
  vmval->end = emit_code_size(emit);
  if (!tflag(l->type, tflag_struct)) return (m_int)l->offset;
  emit_struct_release(emit, l->type, l->offset);
  return _frame_pop(emit);
}

ANN static void emit_maybe_release(const Emitter emit, MaybeValList *const ms) {
  for(m_uint i = 0; i < ms->len; i++) {
    const MaybeVal mv = maybevallist_at(ms, i);
    struct M_Vector_ vals = { .ptr = mv.ptr };
    for(m_uint j = 0; j < m_vector_size(&vals); j++) {
      const VMValue val = *(VMValue*)(vals.ptr + ARRAY_OFFSET + j * sizeof(VMValue));
      emit_compound_release(emit, val.t, val.offset);
    }
  }
}

ANN static m_int frame_pop(const Emitter emit) {
  emit_defers(emit);
  if(emit->code->frame->maybe_stack)
    emit_maybe_release(emit, emit->code->frame->maybe_stack);
  return _frame_pop(emit);
}

ANN /*static */ bool emit_exp(const Emitter emit, Exp* exp);
ANN static bool      emit_stmt_list(const Emitter emit, StmtList *list);
ANN static bool      emit_exp_dot(const Emitter emit, const Exp_Dot *member);

ANEW Code *new_code(const Emitter emit, const m_str name) {
  Code *code = mp_calloc(emit->gwion->mp, Code);
  code->name = code_name_set(emit->gwion->mp, name, emit->env->name);
  vector_init(&code->instr);
  vector_init(&code->stack_break);
  vector_init(&code->stack_cont);
  vector_init(&code->stack_return);
  m_vector_init(&code->live_values, sizeof(VMValue), 0);
  code->frame = new_frame(emit->gwion->mp);
  return code;
}

ANN static void free_code(MemPool p, Code *code) {
// we should use instr destructors
  if(code->instr.ptr) vector_release(&code->instr);
  vector_release(&code->stack_break);
  vector_release(&code->stack_cont);
  vector_release(&code->stack_return);
  if (code->live_values.ptr) m_vector_release(&code->live_values);
  free_frame(p, code->frame);
  free_mstr(p, code->name);
  mp_free(p, Code, code);
}

ANN void emit_pop_scope(const Emitter emit) {
  m_int offset;
  struct Vector_ v;
  vector_init(&v);
  while ((offset = frame_pop(emit)) > -1)
    vector_add(&v, offset);
  if(!vector_size(&v))
    vector_release(&v);
  else if(vector_size(&v) == 1) {
    emit_object_release(emit, vector_front(&v));
    vector_release(&v);
  } else {
    Instr instr  = emit_add_instr(emit, ObjectRelease2);
    instr->m_val = (m_uint)v.ptr;
  }
  if (emit->info->debug) emit_add_instr(emit, DebugPop);
}

ANN void emit_push_scope(const Emitter emit) {
  frame_push(emit->code->frame);
  if (emit->info->debug) emit_add_instr(emit, DebugPush);
}

ANN m_uint emit_code_offset(const Emitter emit) {
  return emit->code->frame->curr_offset;
}

ANN static Local * add_local(const Emitter emit, const Type t, const bool is_compound) {
  Local *const l = frame_local(emit->gwion->mp, emit->code->frame, t);
  if (is_compound) {
    l->is_compound = true;
    VMValue vmval  = {
        .t = t,
        .offset = l->offset,
        .start = emit_code_size(emit)
    };
    m_vector_add(&emit->code->live_values, &vmval);
    ++emit->code->frame->value_count;
  }
  return l;
}

ANN m_uint emit_local(const Emitter emit, const Type t) {
  const bool is_compound = tflag(t, tflag_compound);
  Local *const l = add_local(emit, t, is_compound);
  return l->offset;
}

ANN void* emit_localx(const Emitter emit, const Type t) {
  Local *const l = add_local(emit, t, 1);
  l->instr = emit_regtomem(emit, l->offset, -SZ_INT);
  return l;
}

ANN m_uint emit_local_exp(const Emitter emit, Exp* e) {
  Local *const l = emit_localx(emit, e->type);
  if(e->ref) e->ref->data = l;
  return l->offset;
}

ANN m_uint emit_local_exp2(const Emitter emit, Exp* e) {
  Local *const l = emit_localx(emit, e->type);
  e->data = l;
  return l->offset;
}

ANN m_uint emit_localn(const Emitter emit, const Type t) {
  Local *const l = frame_local(emit->gwion->mp, emit->code->frame, t);
  l->skip        = true;
  return l->offset;
}

ANN void emit_ext_ctor(const Emitter emit, const Type t);

ANN static inline void maybe_ctor(const Emitter emit, const Type t) {
  if (tflag(t, tflag_ctor))emit_ext_ctor(emit, t);
}

ANN2(1, 2)
static ArrayInfo *emit_array_extend(const Emitter emit, const Type t,
                                          Exp* e, const uint is_ref);
ANN static bool emit_pre_ctor(const Emitter emit, const Type type) {
  if (type->info->parent && !tflag(type, tflag_cxx)) {
    CHECK_B(emit_pre_ctor(emit, type->info->parent));
    if (tflag(type, tflag_typedef) && type->info->parent->array_depth)
      CHECK_B(emit_array_extend(emit, type, type->info->cdef->base.ext->array->exp, false));
  }
  maybe_ctor(emit, type);
  return true;
}

ANN static void struct_expand(const Emitter emit, const Type t) {
  const Instr instr = emit_add_instr(emit, Reg2RegDeref);
  instr->m_val      = -SZ_INT;
  instr->m_val2     = t->size;
}

ANN static bool emit_pre_constructor_array(const Emitter emit,
                                           const Type    type) {
  const m_uint start_index = emit_code_size(emit);
  const Instr  top         = emit_add_instr(emit, ArrayTop);
  top->m_val2              = (m_uint)type;
  if (tflag(type, tflag_struct)) {
    const Instr instr = emit_add_instr(emit, ArrayStruct);
    instr->m_val      = type->size;
  }
  CHECK_B(emit_pre_ctor(emit, type));
  if (!tflag(type, tflag_struct))
    emit_add_instr(emit, ArrayBottom);
  else
    emit_regmove(emit, -SZ_INT);
  emit_regmove(emit, -SZ_INT);
  const Instr pc = emit_add_instr(emit, Goto);
  pc->m_val      = start_index;
  top->m_val     = emit_code_size(emit);
  emit_regmove(emit, -SZ_INT*3);
  emit_add_instr(emit, ArrayPost);
  return true;
}

ANN2(1)
static bool extend_indices(const Emitter emit, Exp* e, const m_uint depth) {
  if (e) CHECK_B(emit_exp(emit, e));
  m_uint count = 0;
  while (e) {
    ++count;
    e = e->next;
  }
  for (m_uint i = count; i < depth; ++i) emit_pushimm(emit, 0);
  return true;
}

ANEW ANN static ArrayInfo *new_arrayinfo(const Emitter emit, const Type t, const loc_t loc) {
  const Type base = array_base(t);
  ArrayInfo *info = mp_calloc(emit->gwion->mp, ArrayInfo);
  vector_init(&info->type);
  info->depth = get_depth(t);
  for (long i = 1; i < info->depth; ++i)
    vector_add(&info->type, (vtype)array_type(emit->env, base, i, loc));
  vector_add(&info->type, (vtype)t);
  info->base = base;
  return info;
}

ANN static inline bool arrayinfo_ctor(const Emitter emit, ArrayInfo *info) {
  const Type base = info->base;
  if (tflag(base, tflag_compound) &&
      !GET_FLAG(base, abstract)) {
    CHECK_B(emit_pre_constructor_array(emit, base));
    info->is_obj = 1;
  }
  return true;
}

ANN2(1, 2)
static ArrayInfo *emit_array_extend(const Emitter emit, const Type t,
                                          Exp* e, const uint is_ref) {
  CHECK_O(extend_indices(emit, e, get_depth(t)));
  ArrayInfo * info  = new_arrayinfo(emit, t, t->info->cdef->base.tag.loc);
  const Instr alloc = emit_add_instr(emit, ArrayAlloc);
  alloc->m_val      = (m_uint)info;
  if (!is_ref) CHECK_O(arrayinfo_ctor(emit, info));
  return info;
}

ANN static void call_finish(const Emitter emit, const Func f, const m_uint size, const bool);
ANN void emit_ext_ctor(const Emitter emit, const Type t) {
  const Instr cpy = emit_add_instr(emit, Reg2Reg);
  cpy->m_val2     = -SZ_INT;
  emit_regmove(emit, SZ_INT);
  const Func f = (Func)vector_front(&t->nspc->vtable);
  emit_pushfunc(emit, f);
  const m_uint size = !tflag(t, tflag_union) ? 0 : t->size;
  call_finish(emit, f, size, false);
}

ANN2(1, 2)
bool emit_instantiate_object(const Emitter emit, const Type type,
                               const Array_Sub array, const bool is_ref) {
  if (type->array_depth) {
    DECL_B(ArrayInfo *, info,
            = emit_array_extend(emit, type, array ? array->exp : NULL,
                                      is_ref));
    return true;
  } else if (!is_ref) {
    if(!tflag(type, tflag_typedef) || isa(type, emit->gwion->type[et_closure])) {
      const Instr instr = emit_add_instr(emit, ObjectInstantiate);
      instr->m_val2     = (m_uint)type;
    } // maybe we should instantiate the first actual type
    CHECK_B(emit_pre_ctor(emit, type));
  }
  return true;
}

ANN2(1, 2)
bool emit_instantiate_decl(const Emitter emit, const Type type,
                             const Type_Decl *td, const bool is_ref) {
  return emit_instantiate_object(emit, type, td->array, is_ref);
}

ANN static bool emit_symbol_builtin(const Emitter emit, const Symbol *data) {
  const Value v = prim_self(data)->value;
  const bool emit_addr = exp_getvar(prim_exp(data));
  if (vflag(v, vflag_direct)) {
    const m_uint size  = v->type->size;
    emit_dotstatic(emit, (m_uint)&v->d.ptr, size, emit_addr);
    // prevent invalid access to global variables
    if(!emit_addr && is_object(emit->gwion, v->type))
      emit_fast_except(emit, v->from, prim_pos(data));
  } else {
    const m_uint size = v->type->size;
    const Instr instr = emit_regpushimm(emit, v->d.num, size, emit_addr);
    if (v->type->size == SZ_FLOAT)
      instr->f = v->d.fnum;
  }
  return true;
}

ANN static bool emit_symbol(const Emitter emit, const Symbol *data) {
  const Value v = prim_self(data)->value;
  if (is_class(emit->gwion, v->type)) {
    emit_pushimm(emit, (m_uint)actual_type(emit->gwion, v->type));
    return true;
  }
  if (vflag(v, vflag_builtin) || vflag(v, vflag_direct))
    return emit_symbol_builtin(emit, data);
  const Type t = prim_exp(data)->type;
  if(is_func(emit->gwion, v->type)) {
    const Func f = t->info->func;
    if(f->code)
      emit_pushimm(emit, (m_uint)f->code);
    else if(!f->def->base->tmpl) {
      const Instr instr = emit_add_instr(emit, SetFunc);
      instr->m_val = (m_uint)f;
    } else emit_pushimm(emit, (m_uint)f);
    return true;
  }
  if (tflag(v->type, tflag_ref) && !safe_tflag(prim_exp(data)->cast_to, tflag_ref)) {
    if (exp_getvar(exp_self(prim_self(data)))) {
      const Instr instr = emit_add_instr(emit, RegPushMem);
      instr->m_val      = v->from->offset;
      instr->m_val2     = SZ_INT;
    } else {
      const Instr instr = emit_add_instr(emit, RegPushMemDeref);
      instr->m_val      = v->from->offset;
      const Type t = (Type)vector_front(&v->type->info->tuple->contains);
      instr->m_val2     = t->size;
    }
    return true;
  }
  const m_uint size = v->type->size;
  if(!vflag(v, vflag_fglobal))
    emit_regpushmem(emit, v->from->offset, size, exp_getvar(prim_exp(data)));
  else
    emit_regpushbase(emit, v->from->offset, size, exp_getvar(prim_exp(data)));
  if (GET_FLAG(v, late) && !exp_getvar(prim_exp(data)) &&
      is_object(emit->gwion, v->type))
    emit_fast_except(emit, v->from, prim_pos(data));
  return true;
}

ANN VM_Code finalyze(const Emitter emit, const f_instr exec) {
  emit_add_instr(emit, exec);
  const VM_Code code = emit->info->emit_code(emit);
  free_code(emit->gwion->mp, emit->code);
  emit_pop_code(emit);
  return code;
}

ANN static inline m_uint exp_size(Exp* e) {
  if (exp_getvar(e)) return SZ_INT;
  const Type type = e->cast_to ?: e->type;
  return type->size;
}

ANN void emit_object_addref(const Emitter emit, const m_uint size,
                             const bool emit_var) {
  const f_instr exec  = !emit_var ? RegAddRef : RegAddRefAddr;
  const Instr   instr = emit_add_instr(emit, exec);
  instr->m_val        = size;
}

ANN void emit_struct_addref(const Emitter emit, const Type t, const m_uint size,
                             const bool emit_var) {
  if(!tflag(t, tflag_release)) return;
  const Instr instr =
      emit_add_instr(emit, !emit_var ? StructRegAddRef : StructRegAddRefAddr);
  instr->m_val2  = (m_uint)t;
  instr->m_val = !emit_var ? (m_int)size : (m_int)-SZ_INT;
}

ANN2(1)
static void emit_exp_addref1(const Emitter emit, Exp* exp, m_uint size) {
  const Type t = exp->cast_to ?: exp->type;
  if (tflag(t, tflag_compound))
    emit_compound_addref(emit, exp->type, size, exp_getvar(exp));
}

/*ANN2(1)
static void emit_exp_addref(const Emitter emit, Exp* exp,
                            m_uint size) {
  do {
    emit_exp_addref1(emit, exp, size);
    size += exp_size(exp);
  } while ((exp = exp->next));
}*/

ANN static inline bool emit_exp1(const Emitter emit, Exp* e) {
  Exp* next   = e->next;
  e->next          = NULL;
  const bool ret = emit_exp(emit, e);
  e->next          = next;
  return ret;
}

ANN static bool emit_prim_array_exp(const Emitter emit, const Type t, Exp* e) {
  do {
    CHECK_B(emit_exp1(emit, e));
    emit_exp_addref1(emit, e, -t->size);
    emit_regmove(emit, - t->size + t->actual_size);
  } while((e = e->next));
  return true;
}

ANN static bool emit_prim_array(const Emitter emit, const Array_Sub *data) {
  Exp* e = (*data)->exp;
  const Type type = (*data)->type;
  const Type base = array_base_simple(type);
  if(!base->actual_size) CHECK_B(emit_exp(emit, e));
  else CHECK_B(emit_prim_array_exp(emit, base, e));
  m_uint count = 0;
  do ++count;
  while ((e = e->next));
  emit_regsetimm(emit, count, 0);
  const Instr instr = emit_add_instr(emit, ArrayInit);
  instr->m_val      = (m_uint)type;
  const m_uint sz = base->actual_size ?: base->size;
  instr->m_val2     = type->array_depth == 1 ? sz : SZ_INT;
  emit_local_exp(emit, prim_exp(data));
  return true;
}

ANN static inline bool emit_exp_pop_next(const Emitter emit, Exp* e);

ANN static bool emit_range(const Emitter emit, Range *range) {
  if (range->start)
    CHECK_B(emit_exp(emit, range->start));
  else emit_pushimm(emit, 0);
  if (range->end)
    CHECK_B(emit_exp(emit, range->end));
  else emit_pushimm(emit, -1);
  return true;
}

ANN static bool emit_prim_range(const Emitter emit, Range **data) {
  Range *range = *data;
  CHECK_B(emit_range(emit, range));
  Exp*    e   = range->start ?: range->end;
  const Symbol sym = insert_symbol("[:]");
  assert(e);
  struct Op_Import opi = {.op   = sym,
                          .lhs  = e->type,
                          .loc  = e->loc,
                          .data = (uintptr_t)prim_exp(data)};
  CHECK_B(op_emit(emit, &opi));
  emit_local_exp(emit, prim_exp(data));
  return true;
}

static inline m_uint int2pow2(const m_uint x) {
	return x == 1 ? 2 : 1<<((CHAR_BIT * sizeof(m_uint))-__builtin_clzl(x));
}

ANN static bool emit_prim_dict(const Emitter emit, Exp* *data) {
  Exp* e = *data;
  const Type  key = e->type;
  const Type  val = e->next->type;
  const Type t = dict_type(emit->gwion, key, val, e->loc);
  const Instr init = emit_add_instr(emit, dict_ctor_alt);
  Exp* next = e->next;
  e->next = NULL;
  Exp func = { .exp_type = ae_exp_primary, .d = { .prim = { .prim_type = ae_prim_id, .d = { .var = insert_symbol("hash") }} }};
  Exp call = { .exp_type = ae_exp_call, .d = { .exp_call = { .func = &func, .args = e}}};
  CHECK_B(traverse_exp(emit->env, &call));
  e->next = next;
  m_uint count = 0;
  do {
    Exp* next = e->next;
    Exp* nnext = next->next;
    next->next = NULL;
    e->next = NULL;
    CHECK_B(emit_exp(emit, e));
    e->next = next;
    CHECK_B(emit_exp(emit, next));
    next->next = nnext;
    if(key->size == SZ_INT) {
      const Instr instr = emit_add_instr(emit, Reg2Reg);
      instr->m_val2 = -SZ_INT - val->size;
      emit_regmove(emit, SZ_INT);
    } else {
      const Instr instr = emit_add_instr(emit, Reg2RegOther);
      instr->m_val  = -key->size;
      instr->m_val2 = key->size;
      emit_regmove(emit, key->size);
    }
    e->next = next;
    CHECK_B(emit_exp(emit, &func));
    CHECK_B(emit_exp_call1(emit, func.type->info->func,
          func.type->info->func->def->base->ret_type->size, true));
    count++;
  } while((e = e->next->next));
  init->m_val = int2pow2(count);
  init->m_val2 = (m_uint)t;
  const m_uint sz = (key->size + val->size + SZ_INT) * count;
  emit_regmove(emit, -sz);
  const Instr ctor = emit_add_instr(emit, dict_lit_ctor);
  ctor->m_val = sz;
  ctor->m_val2 = count;
  return true;
}

ANN bool emit_array_access(const Emitter                 emit,
                             struct ArrayAccessInfo *const info) {
  if (tflag(info->array.type, tflag_typedef))
    info->array.type = typedef_base(info->array.type);
  // look mum no pos
  struct Op_Import opi = {.op   = insert_symbol("[]"),
                          .lhs  = info->array.exp->type,
                          .rhs  = info->array.type,
                          .data = (uintptr_t)info};
  return op_emit(emit, &opi);
}

ANN static bool emit_exp_array(const Emitter emit, const Exp_Array *array) {
  Exp* e    = exp_self(array);
  exp_setvar(array->base, get_emit_var(emit, array->base->type, exp_getvar(e)));
  CHECK_B(emit_exp(emit, array->base));
  struct ArrayAccessInfo info = {
    .array = *array->array,
    .type = e->type,
    .is_var = exp_getvar(e)};
  return emit_array_access(emit, &info);
}

ANN static bool emit_exp_slice(const Emitter emit, const Exp_Slice *range) {
  CHECK_B(emit_exp(emit, range->base));
  CHECK_B(emit_range(emit, range->range));
  const Symbol sym = insert_symbol("[:]");
  Exp*    e   = range->range->start ?: range->range->end;
  assert(e);
  struct Op_Import opi = {.op   = sym,
                          .lhs  = e->type,
                          .rhs  = range->base->type,
                          .loc  = e->loc,
                          .data = (uintptr_t)exp_self(range)};
  CHECK_B(op_emit(emit, &opi));
  emit_local_exp(emit, exp_self(range));
  return true;
}

ANN static bool specialid_instr(const Emitter      emit,
                                        struct SpecialId_ *spid,
                                        const Exp_Primary *prim) {
  if(spid->exec) emit_add_instr(emit, spid->exec);
  else if (spid->em) return spid->em(emit, prim);
  return true;
}

ANN static bool emit_prim_id(const Emitter emit, const Symbol *data) {
  struct SpecialId_ *spid = specialid_get(emit->gwion, *data);
  if (unlikely(spid))
    return specialid_instr(emit, spid, prim_self(data));
  return emit_symbol(emit, data);
}

ANN static bool emit_prim_perform(const Emitter emit, const Symbol *xid) {
  const Instr instr = emit_add_instr(emit, PerformEffect);
  if(*xid) instr->m_val      = (m_uint)s_name(*xid);
  instr->m_val2      = emit->status.effect;
  return true;
}

ANN static bool emit_prim_num(const Emitter emit, const m_uint *num) {
  emit_pushimm(emit, *num);
  return true;
}

ANN static bool emit_prim_float(const Emitter emit, const m_float *fnum) {
  const Instr instr = emit_add_instr(emit, RegPushImm2);
  instr->f          = *fnum;
  return true;
}

ANN static bool emit_prim_char(const Emitter emit, const m_str *str) {
  char c;
  CHECK_B(str2char(emit, *str, &c, prim_pos(str)));
  emit_pushimm(emit, c);
  return true;
}

ANN static bool emit_prim_str(const Emitter emit, const struct AstString *str) {
  const Value v = prim_self(str)->value;
  bool has_obj = v->d.obj;
  if (!has_obj) {
    const size_t sz = strlen(str->data);
    char c[sz + 1];
    if (sz) {
      strcpy(c, str->data);
      CHECK_B(escape_str(emit, c, prim_pos(str)));
    } else
      c[0] = '\0';
    v->d.obj = new_string(emit->gwion, c);
  }
  emit_pushimm(emit, (m_uint)v->d.obj);
  emit_object_addref(emit, -SZ_INT, 0);
  return true;
}

#define emit_prim_nil (void *)dummy_func

ANN static inline void interp_size(const Emitter emit, Exp* e) {
  const Type t = !tflag(e->type, tflag_ref) || safe_tflag(e->cast_to, tflag_ref) ?
    e->type : (Type)vector_front(&e->type->info->tuple->contains);
  emit_regsetimm(emit, t->size, SZ_INT);
}

ANN static void emit_gack_type(const Emitter emit, Exp* e) {
  if (e->exp_type == ae_exp_cast ||
      (e->exp_type == ae_exp_primary && e->d.prim.prim_type == ae_prim_str))
     return;
  const bool isobj = is_object(emit->gwion, e->type);
  if (isobj  && (tflag(e->type, tflag_ref) || !GET_FLAG(e->type, final)))
    emit_add_instr(emit, GackType);
}

ANN /*static*/ bool emit_interp(const Emitter emit, Exp* exp) {
  emit_pushimm(emit, 0);
  //emit_local(emit, emit->gwion->type[et_int]);
  Exp* e = exp;
  Exp* next = NULL;
  do {
    next    = e->next;
    e->next = NULL;
    if (!emit_exp(emit, e)) {
      e->next = next;
      return false;
    }
    if(tflag(e->type, tflag_ref) && !safe_tflag(e->cast_to, tflag_ref)) {
      const Type t = (Type)vector_front(&e->type->info->tuple->contains);
      emit_regsetimm(emit, (m_uint)t, 0);
    } else
      emit_regsetimm(emit, (m_uint)e->type, 0);
    interp_size(emit, e);
    emit_gack_type(emit, e);
    const Instr instr = emit_add_instr(emit, Gack);
    instr->m_val      = emit_code_offset(emit);
  } while ((e = e->next = next));
  return true;
}

ANN static bool emit_prim_hack(const Emitter emit, Exp* *exp) {
  CHECK_B(emit_interp(emit, *exp));
  if (!(emit->env->func &&
        emit->env->func->def->base->tag.sym == insert_symbol("@gack")))
    emit_add_instr(emit, GackEnd);
  else emit_regtomem(emit, SZ_INT, -SZ_INT);
  return true;
}

ANN static bool emit_prim_interp(const Emitter emit, Exp* *exp) {
  Exp* e = *exp;
  CHECK_B(emit_interp(emit, e));
  const Instr instr = emit_add_instr(emit, GackEnd);
  instr->m_val      = 1;
  emit_localx(emit, emit->gwion->type[et_string]);
  return true;
}

ANN bool emit_ensure_func(const Emitter emit, const Func f) {
  const ValueFrom *from = f->value_ref->from;
  if(from->owner_class)
    CHECK_B(ensure_emit(emit, from->owner_class));
  if(f->code) return true;
  const m_uint scope = emit_pushv(emit, f->value_ref);
  const bool ret = emit_func_def(emit, f->def);
  emit_pop(emit, scope);
  return ret;
}

ANN static bool emit_prim_locale(const Emitter emit, const Symbol *id) {
  if(emit->locale->def->d.code) {
    const Stmt stmt = stmtlist_at((emit->locale->def->d.code), 0);
    const Func f = stmt.d.stmt_exp.val->d.exp_call.func->type->info->func;
    CHECK_B(emit_ensure_func(emit, f));
  }
  CHECK_B(emit_ensure_func(emit, emit->locale));
  comptime_ini(emit, "locale");
  const M_Object string = new_string(emit->gwion, s_name(*id));
  emit_pushimm(emit, (m_uint)string);
  emit_pushimm(emit, (m_uint)emit->locale->code);
  CHECK_B(emit_exp_call1(emit, emit->locale, SZ_FLOAT, true));
  m_float ret;
  comptime_end(emit, SZ_FLOAT, &ret);
  if(ret == -1.0)
    ERR_B(prim_pos(id), "error in locale");
  const Instr instr = emit_add_instr(emit, RegPushImm2);
  instr->f = ret;
  return true;
}

DECL_PRIM_FUNC(emit, bool, Emitter,);
ANN static bool emit_prim(const Emitter emit, Exp_Primary *const prim) {
  return emit_prim_func[prim->prim_type](emit, &prim->d);
}

ANN static bool emit_dot_static_data(const Emitter emit, const Value v,
                                       const bool emit_var) {
  const m_uint size  = v->type->size;
  emit_dotstatic(emit, (m_uint)(v->from->owner->class_data + v->from->offset), size, emit_var);
  return true;
}

ANN static bool _decl_static(const Emitter emit, const Exp_Decl *decl,
                              const Var_Decl *var_decl, const uint is_ref) {
  const Value v    = var_decl->value;
  if(!decl->args) CHECK_B(emit_instantiate_decl(emit, v->type, decl->var.td, is_ref));
  else CHECK_B(emit_exp(emit, decl->args));
  CHECK_B(emit_dot_static_data(emit, v, 1));
  emit_add_instr(emit, Assign);
//  if(get_depth(var_decl->value->type) && !is_ref)
//    (void)emit_object_addref(emit, -SZ_INT, 0);
  emit_regmove(emit, -SZ_INT);
  return true;
}

ANN static bool decl_static(const Emitter emit, const Exp_Decl *decl,
                              const Var_Decl *var_decl, const uint is_ref) {
  Code *const code = emit->code;
  emit->code       = (Code *)vector_back(&emit->stack);
  const bool ret = _decl_static(emit, decl, var_decl, is_ref);
  emit->code = code;
  return ret;
}

ANN static inline int struct_ctor(const Value v) {
  return tflag(v->type, tflag_struct) /*&& tflag(v->type, tflag_ctor)*/;
}

ANN static void decl_expand(const Emitter emit, const Type t) {
  struct_expand(emit, t);
  emit_regmove(emit, t->size - SZ_INT);
}

ANN static m_uint decl_non_static_offset(const Emitter emit, const Exp_Decl *decl, const Type t) {
  if(!exp_self(decl)->data)
    return emit_local(emit, t);
  const Local *l = exp_self(decl)->data;
  exp_self(decl)->data = (void*)-1;
  return l->offset;
}

ANN static bool struct_finish(const Emitter emit, const Exp_Decl *decl) {
  const Type t = decl->type;
  const bool emit_addr = exp_getvar(exp_self(decl));
  if(GET_FLAG(decl->var.vd.value, late) || GET_FLAG(decl->var.td, late)) {
    if(!emit_addr) decl_expand(emit, t);
    return true;
  }
  if(!decl->args && !tflag(t, tflag_ctor) && emit_addr)
    return true;
  if (decl->args) {
    if (emit_addr) {
      const Instr back = (Instr)vector_back(&emit->code->instr);
      const Instr instr = emit_add_instr(emit, NoOp);
      memcpy(instr, back, sizeof(struct Instr_));
    }
    CHECK_B(emit_exp(emit, decl->args));
    if (emit_addr) emit_regmove(emit, -t->size);
    return true;
  }
  if(tflag(t, tflag_ctor)) emit_ext_ctor(emit, t);
  if (!emit_addr) decl_expand(emit, t);
  return true;
}

ANN static bool emit_exp_decl_static(const Emitter emit, const Exp_Decl *decl,
                                       const Var_Decl *var_decl,
                                       const bool     is_ref,
                                       const bool     emit_addr) {
  const Value v = var_decl->value;
  bool isobj = is_object(emit->gwion, v->type);
  if (isobj && !is_ref)
    CHECK_B(decl_static(emit, decl, var_decl, 0));
  CHECK_B(emit_dot_static_data(emit, v, !struct_ctor(v) ? emit_addr : 1));
  if (tflag(v->type, tflag_struct)) CHECK_B(struct_finish(emit, decl));
  if (isobj && !is_ref) {
    if(safe_tflag(emit->env->class_def, tflag_struct) && GET_FLAG(emit->env->class_def, global))
      emit_object_addref(emit, 0, emit_addr);
  }
  return true;
}

ANN static Instr emit_struct_decl(const Emitter emit, const Value v,
                                  const bool emit_addr) {
  emit_add_instr(emit, RegPushMem);
  const Instr instr = emit_structmember(emit, v->from->offset, v->type->size, emit_addr);
  if (!emit_addr) {
    const m_int sz = v->type->size - SZ_INT;
    if (sz) emit_regmove(emit, sz);
  }
  return instr;
}

ANN void unset_local(const Emitter emit, Local *const l) {
  l->instr->opcode = eNoOp;
  for(m_uint i = m_vector_size(&emit->code->live_values) + 1; --i;) {
    VMValue vmval = *(VMValue*)(ARRAY_PTR((&emit->code->live_values)) + (i-1) * sizeof(VMValue));
    if(vmval.offset != l->offset) continue;
    m_vector_rem(&emit->code->live_values, i-1);
    vector_rem2(&emit->code->frame->stack, (m_uint)l);
    vector_rem2(&emit->code->instr, (m_uint)l->instr);
    free_instr(emit->gwion, l->instr);
    emit->code->frame->curr_offset -= l->type->size;
    --emit->code->frame->value_count;
    break;
  }
}

static INSTR(UsedBy) {
  const FuncList *v =(FuncList*)instr->m_val;
  for(uint32_t i = 0; i < v->len; i++) {
    const Func f = funclist_at(v, i);
    for(m_uint j = 0; i < vector_size(&f->code->instr); j++) {
      const Instr instr = (Instr)vector_at(&f->code->instr, j);
      if(instr->execute == FuncWait) {
        instr->m_val2++;
        break;
      }
    }
  }
}

ANN static void used_by(const Emitter emit, const Value v) {
  FuncList *vec = new_funclist(emit->gwion->mp, 0);
  for(uint32_t i = 0; i < v->used_by->len; i++) {
    const Func f = funclist_at(v->used_by, i);
    if(f->_wait) funclist_add(emit->gwion->mp, &vec, f);
  }
  free_funclist(emit->gwion->mp, v->used_by);
  v->used_by = vec;
  if(vec->len) {
    const Instr instr = emit_add_instr(emit, UsedBy);
    instr->m_val = (m_uint)vec;
  }
}
ANN static bool emit_exp_decl_non_static(const Emitter   emit,
                                           const Exp_Decl *decl,
                                           const Var_Decl *var_decl,
                                           const uint      is_ref,
                                           const uint      emit_var) {
  const Value     v        = var_decl->value;
  const Type      type     = v->type;
  const bool      is_obj   = is_object(emit->gwion, type);
  const bool emit_addr = (!is_obj || is_ref) ? emit_var : true;
  if (is_obj && !is_ref && !exp_self(decl)->ref) {
    if(!decl->args) CHECK_B(emit_instantiate_decl(emit, type, decl->var.td, is_ref));
    else CHECK_B(emit_exp(emit, decl->args));
  }
  f_instr *exec = (f_instr *)allocmember;
  if (!vflag(v, vflag_member)) {
    if(v->used_by) used_by(emit, v);
    v->from->offset = decl_non_static_offset(emit, decl, type);
    exec            = (f_instr *)(allocword);
    if (GET_FLAG(v, late)) // ref or emit_var ?
      emit_memsetimm(emit, v->from->offset, 0);
  }
  if(!(safe_tflag(emit->env->class_def, tflag_struct) && !emit->env->scope->depth)) {
    const Instr instr =  emit_kind(emit, v->from->offset, v->type->size, !struct_ctor(v) ? emit_addr : true,
                      exec);
    // we want to push a zero
    if(vflag(v, vflag_member) && !emit_addr)
      instr->m_val = 0;
  }
  else emit_struct_decl(emit, v, !struct_ctor(v) ? emit_addr : 1);
  if (is_obj && !is_ref && !exp_self(decl)->ref) {
    if (!emit_var)
      emit_add_instr(emit, Assign);
    else {
      emit_regmove(emit, -SZ_INT);
      const Instr instr = emit_add_instr(emit, Reg2Reg);
      instr->m_val      = -SZ_INT;
    }
//    if(safe_tflag(emit->env->class_def, tflag_struct) && GET_FLAG(emit->env->class_def, global))
//      emit_object_addref(emit, 0, emit_addr);
  } else if (tflag(v->type, tflag_struct))
    CHECK_B(struct_finish(emit, decl));
  return true;
}

ANN static bool emit_exp_decl_global(const Emitter emit, const Exp_Decl *decl,
                                       const Var_Decl *var_decl,
                                       const uint is_ref, const bool emit_var) {
  const Value     v        = var_decl->value;
  const Type      type     = v->type;
  const bool      is_obj   = is_object(emit->gwion, type);
  const bool emit_addr = (!is_obj || is_ref) ? emit_var : true;
  if (is_obj && !is_ref) {
    if(!decl->args) CHECK_B(emit_instantiate_decl(emit, type, decl->var.td, is_ref));
    else CHECK_B(emit_exp(emit, decl->args));
  }
  if (type->size > SZ_INT)
    v->d.ptr = mp_calloc2(emit->gwion->mp, v->type->size);
  emit_dotstatic(emit, (m_uint)&v->d.ptr, v->type->size, !struct_ctor(v) ? emit_addr : 1);
//  set_vflag(v, vflag_direct); // mpalloc // set in check.c
  if(v->used_by) used_by(emit, v);
  if (is_obj && !is_ref) {
    const Instr assign = emit_add_instr(emit, Assign);
    assign->m_val      = emit_var;
    (void)emit_object_addref(emit, -SZ_INT, emit_var);
  } else if (tflag(v->type, tflag_struct)) {
//    (void)emit_struct_addref(emit, v->type, -v->type->size, emit_var);
    struct_finish(emit, decl);
  }
  return true;
}

ANN static bool emit_decl(const Emitter emit, Exp_Decl *const decl) {
  const bool  global = GET_FLAG(decl->var.td, global);
  const uint    var    = exp_getvar(exp_self(decl));
  const uint    ref    = GET_FLAG(decl->var.td, late) || type_ref(decl->type);
  Var_Decl *vd = &decl->var.vd;
  const Value v = vd->value;
  const uint  r = ref || GET_FLAG(v, late);
  if (GET_FLAG(decl->var.td, static))
    CHECK_B(emit_exp_decl_static(emit, decl, vd, r, var));
  else if (!global)
    CHECK_B(emit_exp_decl_non_static(emit, decl, vd, r, var));
  else
    CHECK_B(emit_exp_decl_global(emit, decl, vd, r, var));
  if (tflag(v->type, tflag_contract) &&
      !exp_getvar(exp_self(decl))) {
    const Type       t   = v->type;
    struct Op_Import opi = {.lhs = t->info->base_type,
                            .op  = insert_symbol("@implicit"),
                            .rhs = t};
    CHECK_B(op_emit(emit, &opi));
  } 
  if(is_object(emit->gwion, v->type) /*&& GET_FLAG(v, late) */&& exp_getuse(exp_self(decl)) && !exp_getvar(exp_self(decl)))
    emit_add_instr(emit, GWOP_EXCEPT);
  return true;
}

ANN /*static */ bool emit_exp_decl(const Emitter emit, Exp_Decl *const decl) {
  const Type t = decl->type;
  if(decl->args && !strncmp(decl->args->type->name, "partial:", 8))
    ERR_B(decl->args->loc, "unresolved partial");
  CHECK_B(ensure_emit(emit, t));
  const bool global = GET_FLAG(decl->var.td, global);
  const m_uint scope =
      !global ? emit->env->scope->depth : emit_push_global(emit);
  const bool ret = emit_decl(emit, decl);
  if (global) emit_pop(emit, scope);
  if(emit->status.in_return && GET_FLAG(decl->var.vd.value, late) && is_object(emit->gwion, t))
    emit_add_instr(emit, GWOP_EXCEPT);
  return ret;
}


// see take exp
Exp* nth_exp(Exp* e, uint32_t n) {
  for(uint32_t i = 0; i < n; i++)
    e = e->next;
  return e;
}

ANN static bool emit_func_args(const Emitter emit, const Exp_Call *exp_call) {
  if (exp_call->args) CHECK_B(emit_exp(emit, exp_call->args));
  return true;
}

ANN static bool prepare_call(const Emitter emit, const Exp_Call *exp_call) {
  CHECK_B(emit_func_args(emit, exp_call));
  return emit_exp(emit, exp_call->func);
}

ANN static inline void emit_return_pc(const Emitter emit, const m_uint val) {
  LOOP_OPTIM
  for (m_uint i = vector_size(&emit->code->stack_return) + 1; --i;) {
    const Instr instr = (Instr)vector_at(&emit->code->stack_return, i - 1);
    instr->m_val      = val;
  }
}

ANN static inline void pop_exp(const Emitter emit, Exp* e);

ANN static inline void scoped_ini(const Emitter emit) {
  ++emit->env->scope->depth;
  emit_push_scope(emit);
}

ANN static inline void scoped_end(const Emitter emit) {
  emit_pop_scope(emit);
  --emit->env->scope->depth;
}

ANN static bool scoped_stmt(const Emitter emit, Stmt* stmt) {
  scoped_ini(emit);
  const bool ret = emit_stmt(emit, stmt);
  scoped_end(emit);
  return ret;
}

#ifdef GWION_INLINE
ANN static inline bool check_inline(const Emitter emit, const Func f) {
  const uint16_t caller_size = emit->env->func ? emit->env->func->weight
                               : emit->env->class_def
                                   ? emit->env->class_def->weight
                               : emit->env->context ? emit->env->context->weight
                                                    : 0;
  const float    threshold   = caller_size * f->inline_mult;
  return f->weight < threshold;
}

ANN static inline bool member_inlinable(const Emitter emit, const Func f, Exp* e) {
  if (f!= emit->env->func)return false;
  const Type owner_class = f->value_ref->from->owner_class;
  if (!owner_class) return true;
  return GET_FLAG(owner_class, final) || GET_FLAG(f->def->base, final) ||
         (e->exp_type == ae_exp_dot &&
          e->d.exp_dot.base->exp_type == ae_exp_cast);
}

ANN static inline Func is_inlinable(const Emitter   emit,
                                    const Exp_Call *exp_call) {
  const Type ftype = exp_call->func->type;
  if (!is_func(emit->gwion, ftype) ||
      !ftype->info->func->code || ftype->info->func->code->builtin)
    return false;
  const Func f = ftype->info->func;
  return (member_inlinable(emit, f, exp_call->func) && check_inline(emit, f)) ? f
                                                                        : NULL;
}

ANN static inline void inline_args_ini(const Emitter emit, const Func f,
                                       const Vector v) {
  const bool member =  f->value_ref->from->owner_class && vflag(f->value_ref, vflag_member);
  if(member)
    emit->status.this_offset = emit_local(emit, emit->gwion->type[et_int]);
  const m_uint start_offset = emit_code_offset(emit) - (member ? SZ_INT : 0);
  Arg_List args          = f->def->base->args;
  for(uint32_t i = 0; i < args->len; i++) {
    const Arg arg = arglist_at(args, i);
    const Val val = arg.var_decl.value;
    vector_add(v, val->from->offset);
    val->from->offset = emit_local(emit, val->type);
    nspc_add_value(emit->env->curr, arg.var_decl.xid, val);
  }
  emit_regmove(emit, -f->code->stack_depth);
  emit_regtomem4(emit, f->code->stack_depth, start_offset);
}

ANN static inline void inline_args_end(const Func f, const Vector v) {
  Arg_List args = f->def->base->args;
  for(uint32_t i = 0; i < args->len; i++) {
    const Arg   arg   = arglist_at(args, i);
    const Value val   = arg.var_decl.value;
    val->from->offset = vector_at(v, i++);
  }
}

ANN static inline bool inline_body(const Emitter emit, const Func f) {
  struct Vector_ v = {.ptr = emit->code->stack_return.ptr};
  vector_init(&emit->code->stack_return);
  nspc_push_value(emit->gwion->mp, emit->env->curr);
  scoped_ini(emit);
  const bool ret = emit_stmt_list(emit, f->def->d.code);
  scoped_end(emit);
  emit_return_pc(emit, emit_code_size(emit));
  nspc_pop_value(emit->gwion->mp, emit->env->curr);
  vector_release(&emit->code->stack_return);
  emit->code->stack_return.ptr = v.ptr;
  return ret;
}

ANN static inline bool inline_run(const Emitter emit, const Func f) {
  struct Vector_ arg_offset;
  vector_init(&arg_offset);
  inline_args_ini(emit, f, &arg_offset);
  const bool ret    = inline_body(emit, f);
  inline_args_end(f, &arg_offset);
  vector_release(&arg_offset);
  return ret;
}

ANN static inline bool _emit_inline(const Emitter emit, const Func f,
                                     const Exp_Call *exp_call) {
  if (!f->weight) return true;
  if (f->value_ref->from->owner_class && vflag(f->value_ref, vflag_member))
    CHECK_B(emit_exp(emit, exp_call->func->d.exp_dot.base));
  CHECK_B(emit_func_args(emit, exp_call));
  return inline_run(emit, f);
}

ANN static inline bool emit_inline(const Emitter emit, const Func f,
                                     const Exp_Call *exp_call) {
  const EmitterStatus status = emit->status;
  nspc_push_value(emit->gwion->mp, emit->env->curr);
  const bool ret = _emit_inline(emit, f, exp_call);
  nspc_pop_value(emit->gwion->mp, emit->env->curr);
  emit->status = status;
  return ret;
}
#endif


ANN static inline bool is_new_struct(const Func f, const Type t) {
    return is_new(f->def) && tflag(t, tflag_struct);
}

ANN static bool emit_new_struct(const Emitter emit,const Exp_Call *call)  {
  Exp* self = exp_self(call);
  const Type t =  self->type;
  const m_int offset = self->ref ? emit->code->frame->curr_offset - t->size: emit_local(emit, t);
  const Instr back = self->ref ? (Instr)vector_pop(&emit->code->instr) : NULL;
  CHECK_B(emit_func_args(emit, call));
  if(back) vector_add(&emit->code->instr, (m_uint)back);
  else emit_regpushmem(emit, offset, t->size, true);
  if(tflag(t, tflag_ctor)) emit_ext_ctor(emit, t);
  emit_add_instr(emit, NoOp);
  CHECK_B(emit_exp_call1(emit, call->func->type->info->func, t->size, is_static_call(emit->gwion, call->func))); // is a ctor, is_static is true
  return true;
}

ANN static bool _emit_exp_call(const Emitter emit, const Exp_Call *call) {
  #ifdef GWION_INLINE
    const Func _f = is_inlinable(emit, call);
    if(_f) {
      const Func base = emit->env->func;
      emit->env->func = _f;
      const m_uint scope = emit_pushv(emit, _f->value_ref);
      const bool ret = emit_inline(emit, _f, call);
      emit_pop(emit, scope);
      emit->env->func = base;
      return ret;
    }
  #endif

  const Type t = call->func->type;
  if(unlikely(!is_func(emit->gwion, t))) {
    const Type t = actual_type(emit->gwion, call->func->type);
    struct Op_Import opi = {.op   = insert_symbol("call_type"),
                            .rhs  = t,
                            .data = (uintptr_t)call,
                            .loc  = exp_self(call)->loc};
    CHECK_B(op_emit(emit, &opi));
  }
  const Func f = t->info->func;
  if(unlikely(is_new_struct(f, exp_self(call)->type)))
    emit_new_struct(emit, call);
  else {
    if (f != emit->env->func || (f && f->value_ref->from->owner_class))
      CHECK_B(prepare_call(emit, call));
    else CHECK_B(emit_func_args(emit, call));
    CHECK_B(emit_exp_call1(emit, f, exp_self(call)->type->size, is_static_call(emit->gwion, call->func)));
  }
  return true;
}

ANN static bool emit_exp_call(const Emitter emit, const Exp_Call *exp_call) {
  CHECK_B(_emit_exp_call(emit, exp_call));
  Exp* e = exp_self(exp_call);
  if (exp_getvar(e)) {
    const m_uint size = e->type->size;
    emit_regmove(emit, -size);
    const Local *l = e->ref ? e->ref->data : NULL;
    const m_uint offset  = l ? l->offset : emit_local_exp(emit, e);
    emit_regtomem4(emit, offset, size);
    emit_regpushmem4(emit, offset, 0);
  }
  return true;
}

ANN static m_uint get_type_size(Exp* e) {
  if(tflag(e->type, tflag_ref)&& !safe_tflag(e->cast_to, tflag_ref)) {
    const Type base =(Type)vector_front(&e->type->info->tuple->contains);
    return base->size;
  }
  return e->type->size;
}

ANN static m_uint pop_exp_size(Exp* e) {
  const bool emit_addr = exp_getvar(e);
  m_uint     size      = 0;
  do size += !emit_addr ? get_type_size(e) : SZ_INT;
  while ((e = e->next));
  return size;
}

ANN static inline void pop_exp(const Emitter emit, Exp* e) {
  const m_uint size = pop_exp_size(e);
  if (size) emit_regmove(emit, -size);
}

ANN static inline bool emit_exp_pop_next(const Emitter emit, Exp* e) {
  CHECK_B(emit_exp(emit, e));
  if (e->next) pop_exp(emit, e->next);
  return true;
}

ANN static bool emit_exp_binary(const Emitter emit, const Exp_Binary *bin) {
  Exp* lhs = bin->lhs;
  Exp* rhs = bin->rhs;
  CHECK_B(emit_exp_pop_next(emit, lhs));
  CHECK_B(emit_exp_pop_next(emit, rhs));
  struct Op_Import opi = {.op   = bin->op,
                          .lhs  = lhs->type,
                          .rhs  = rhs->type,
                          .loc  = exp_self(bin)->loc,
                          .data = (uintptr_t)bin};
  return op_emit(emit, &opi);
}

ANN static bool emit_exp_cast(const Emitter emit, const Exp_Cast *cast) {
  CHECK_B(emit_exp(emit, cast->exp));
  struct Op_Import opi = {.op   = insert_symbol("$"),
                          .lhs  = cast->exp->type,
                          .rhs  = exp_self(cast)->type,
                          .data = (uintptr_t)cast}; // no pos ?
  (void)op_emit(emit, &opi);
  return true;
}

ANN static bool emit_exp_post(const Emitter emit, const Exp_Postfix *post) {
  CHECK_B(emit_exp(emit, post->exp));
  struct Op_Import opi = {.op   = post->op,
                          .lhs  = post->exp->type,
                          .data = (uintptr_t)post}; // no pos ?
  return op_emit(emit, &opi);
}

ANN static inline bool traverse_emit_func_def(const Emitter  emit,
                                                const Func_Def fdef) {
  if (!fdef->base->ret_type) CHECK_B(traverse_func_def(emit->env, fdef));
  return emit_func_def(emit, fdef);
}

ANN bool traverse_dot_tmpl(const Emitter emit, const Func_Def fdef, const Value v) {
  const m_uint  scope = emit->env->scope->depth;
  const bool    shadowing = emit->env->scope->shadowing;
  emit->env->scope->shadowing = true;
  struct EnvSet es    = {.env   = emit->env,
                      .data  = emit,
                      .func  = (_envset_func)emit_cdef,
                      .scope = scope,
                      .flag  = tflag_emit};
  CHECK_B(envset_pushf(&es, v));
  const bool ret = traverse_emit_func_def(emit, fdef);
  envset_popf(&es, v);
  emit->env->scope->shadowing = shadowing;
  return ret;
}

static INSTR(fptr_call) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  *(VM_Code*)REG(-SZ_INT) = *(VM_Code*)(o->data + instr->m_val);
  const m_bit *caps = *(m_bit**)(o->data + SZ_INT);
  if(caps) {
    const Func_Def fdef = *(Func_Def*) caps;
    const Capture cap = capturelist_at(fdef->captures, fdef->captures->len - 1);
    const uint32_t sz = cap.offset + cap.temp->type->size - fdef->stack_depth;
    memcpy(REG(SZ_INT), caps + SZ_INT, sz);
  }
}

static inline bool push_func_code(const Emitter emit, const Func f) {
  if (!vector_size(&emit->code->instr)) {
    if(fflag(f, fflag_tmpl)) {
      // we are sporking a template
      // assume static call for now
      emit_regsetimm(emit, (m_uint)f->code, -SZ_INT);
    }
    return true;
  }
  const Instr instr = (Instr)vector_back(&emit->code->instr);
  if(f->code) {
    instr->opcode = eRegPushImm;
    instr->m_val  = (m_uint)f->code;
  }
  return true;
}

ANN static bool emit_template_code(const Emitter emit, const Func f) {
  const Value   v     = f->value_ref;
  const size_t  scope = emit->env->scope->depth;
  struct EnvSet es    = {.env   = emit->env,
                      .data  = emit,
                      .func  = (_envset_func)emit_cdef,
                      .scope = scope,
                      .flag  = tflag_emit};
  CHECK_B(envset_pushf(&es, v));
  const bool ret = emit_func_def(emit, f->def);
  envset_popf(&es, v);
  return ret ? push_func_code(emit, f) : false;
}

ANN static void tmpl_prelude(const Emitter emit, const Func f) {
  const Instr gtmpl = emit_add_instr(emit, GTmpl);
  gtmpl->m_val      = (m_uint)f->def;
  gtmpl->m_val2     = (m_uint)tl2str(emit->gwion, f->def->base->tmpl->call, f->def->base->tag.loc);
}

ANN static Instr get_prelude(const Emitter emit, const Func f,
                             const bool is_static) {
  if (f != emit->env->func || !is_static) {
    const Instr instr = emit_add_instr(emit, SetCode);
    instr->udata.one  = 1;
    return instr;
  }
  const Instr instr = emit_add_instr(emit, Recurs);
  instr->m_val      = SZ_INT;
  instr->udata.one  = 1;
  instr->udata.two  = emit_code_offset(emit) + sizeof(frame_t);
  return instr;
}

ANN static void emit_args(const Emitter emit, const Func f) {
  const m_uint member = vflag(f->value_ref, vflag_member) ? SZ_INT : 0;
  if ((f->def->stack_depth - member) == SZ_INT)
    emit_regtomem(emit, member, 0);
  else
    emit_regtomem4(emit, member, f->def->stack_depth - member);
}

typedef struct {
  const Emitter  emit;
  const Func_Def fdef;
  struct Vector_ branch;
  const m_uint   offset;
  m_uint         arg_offset;
} MemoizeEmitter;

static bool me_cmp(MemoizeEmitter *me, const Arg *arg) {
  const Emitter    emit = me->emit;
  const Symbol     sym  = insert_symbol("?=");
  Exp      lhs  = {
    .exp_type = ae_exp_primary,
    .type = arg->type,
    .loc = arg->var.td->tag.loc,
    .d = {
      .prim = { .prim_type = ae_prim_id }
    }
  };
  Exp      rhs  = {
    .exp_type = ae_exp_primary,
    .type     = me->emit->gwion->type[et_bool],
    .loc = arg->var.td->tag.loc,
    .d = {
      .prim = { .prim_type = ae_prim_id }
    }
  };
  Exp      bin  = {
    .exp_type = ae_exp_binary,
    .type = arg->type,
    .loc = arg->var.td->tag.loc,
    .d = {
      .exp_binary = {
        .lhs = &lhs,
        .op = sym,
        .rhs = &rhs
      }
    }
  };
  struct Op_Import opi  = {.op   = sym,
                          .lhs  = arg->type,
                          .rhs  = arg->type,
                          .loc  = me->fdef->base->tag.loc,
                          .data = (uintptr_t)&bin.d};
  CHECK_B(op_emit(emit, &opi));
  const Instr instr = emit_add_instr(emit, BranchEqInt);
  vector_add(&me->branch, (vtype)instr);
  return true;
}

ANN static bool me_arg(MemoizeEmitter *me) {
  ArgList *args = me->fdef->base->args;
  for(uint32_t i = 0; i < args->len; i++) {
    Arg *arg = arglist_ptr_at(args, i);
    const m_uint sz = arg->type->size;
    emit_regpushmem(me->emit, me->arg_offset, sz, false);
    emit_regpushmem(me->emit, me->offset + SZ_INT * 2, sz, false);
    CHECK_B(me_cmp(me, arg));
    me->arg_offset += sz;
  }
  return true;
}

ANN static Instr emit_call(const Emitter emit, const Func f,
                           const bool is_static) {
  const Instr prelude = get_prelude(emit, f, is_static);
  prelude->m_val += -f->def->stack_depth - SZ_INT;
  const m_uint member = vflag(f->value_ref, vflag_member) ? SZ_INT : 0;
  if (member) {
    emit_regtomem(emit, 0, f->def->stack_depth - SZ_INT);
    ++prelude->m_val2;
  }
  if (f->def->stack_depth - member) {
    //  f != emit->env->func ||
    //  f->value_ref->from->owner_class || strstr(emit->code->name, "ork~"))
    {
      emit_args(emit, f);
      ++prelude->m_val2;
    }
  }
  return emit_add_instr(emit, Overflow);
}

ANN static void emit_fptr_call(const Emitter emit, const Func f) {
  const Instr call = emit_add_instr(emit, fptr_call);
  if(f->def->base->tmpl) {
    // maybe merge those
    call->m_val = SZ_INT*2;
    tmpl_prelude(emit, f);
  }
}

ANN static void call_finish(const Emitter emit, const Func f,
                          const m_uint size, const bool is_static) {
  const m_uint offset = emit_code_offset(emit);
  if (f != emit->env->func || !is_static || !is_new(f->def))
    emit_regsetimm(emit, offset, 0);
  const Instr instr   = emit_call(emit, f, is_static);
  instr->m_val        = size;
  instr->m_val2       = offset;
}

ANN bool emit_exp_call1(const Emitter emit, const Func f,
                          const m_uint size, const bool is_static) {
  const EmitterStatus status = emit->status;
  emit->status =  (EmitterStatus){};
  if(unlikely(fflag(f, fflag_fptr))) emit_fptr_call(emit, f);
  else if (unlikely(!f->code && emit->env->func != f)) {
    if (fflag(f, fflag_tmpl)) CHECK_B(emit_template_code(emit, f));
    else //if(is_new(f->def))//if(tflag(f->value_ref->type, tflag_ftmpl))
{
const Type t = f->value_ref->from->owner_class;
if(t && (!emit->env->curr || !isa(t, emit->env->class_def)))

      //!is_new(f->def) || f->value_ref->from->owner_class->array_depth)

      //if(f,->value_ref->from->owner_class->array_depth)
CHECK_B(emit_ensure_func(emit, f));
}
  } else if(is_static)
       push_func_code(emit, f);
  call_finish(emit, f, size, is_static);
  emit->status = status;
  return true;
}

ANN static void emit_exp_spork_finish(const Emitter emit, const m_uint depth) {
  emit_regmove(emit, -depth);
  const Instr spork = emit_add_instr(emit, SporkFunc);
  spork->m_val      = depth + SZ_INT;
  spork->m_val2     = -SZ_INT;
}

ANN static inline void stack_alloc(const Emitter emit) {
  emit_local(emit,
             emit->gwion->type[et_int]); // hiding the fact it is an object
  emit->code->stack_depth += SZ_INT;
}

#define SPORK_FUNC_PREFIX "spork~func:%u"
#define FORK_FUNC_PREFIX  "fork~func:%u"
#define SPORK_CODE_PREFIX "spork~code:%u"
#define FORK_CODE_PREFIX  "fork~code:%u"

static void push_spork_code(const Emitter emit, const m_str prefix,
                            const loc_t loc) {
  char c[strlen(SPORK_FUNC_PREFIX) + num_digit(loc.first.line) + 1];
  sprintf(c, prefix, loc.first.line);
  emit_push_code(emit, c);
}

struct Sporker {
  StmtList *code;
  Exp*  exp;
  VM_Code    vm_code;
  const Type type;
  const CaptureList *captures;
  const loc_t loc;
  const bool emit_var;
  const bool is_spork;
};

ANN static bool spork_prepare_code(const Emitter         emit,
                                     const struct Sporker *sp) {
  emit_pushimm(emit, 0);
  push_spork_code(emit, sp->is_spork ? SPORK_CODE_PREFIX : FORK_CODE_PREFIX,
                  sp->loc);
  if (emit->env->class_def) stack_alloc(emit);
  if (emit->env->func && vflag(emit->env->func->value_ref, vflag_member))
    stack_alloc(emit);
  if(sp->captures) emit->code->frame->curr_offset += captures_sz(sp->captures);
//  return scoped_stmt(emit, sp->code);
  return emit_stmt_list(emit, sp->code);
}

ANN static bool spork_prepare_func(const Emitter         emit,
                                     const struct Sporker *sp) {
  const Type t = actual_type(emit->gwion, sp->exp->d.exp_call.func->type);
  const Func f = t->info->func;
  if(!f->code && f != emit->env->func)
    CHECK_B(emit_ensure_func(emit, f));
  push_spork_code(emit, sp->is_spork ? SPORK_FUNC_PREFIX : FORK_CODE_PREFIX,
                  sp->exp->loc);
  return emit_exp_call1(emit, f, f->def->base->ret_type->size, false);
}

ANN static VM_Code spork_prepare(const Emitter emit, const struct Sporker *sp) {
  if (!sp->code) {
    assert(sp->exp);
    CHECK_O(prepare_call(emit, &sp->exp->d.exp_call));
  }
  if ((sp->code ? spork_prepare_code : spork_prepare_func)(emit, sp) > 0)
    return finalyze(emit, EOC);
  emit_pop_code(emit);
  return NULL;
}

ANN void spork_code(const Emitter emit, const struct Sporker *sp) {
  const Instr args  = emit_add_instr(emit, SporkExp);
  args->m_val       = emit->code->stack_depth;
  const Instr instr = emit_add_instr(emit, sp->is_spork ? SporkEnd : ForkEnd);
  instr->m_val      = sp->emit_var;
}

ANN void spork_func(const Emitter emit, const struct Sporker *sp) {
  const Func f =
      actual_type(emit->gwion, sp->exp->d.exp_call.func->type)->info->func;
  emit_exp_spork_finish(emit, f->def->stack_depth);
  (void)emit_add_instr(emit, SporkEnd);
}

ANN static void spork_ini(const Emitter emit, const struct Sporker *sp) {
  if (sp->is_spork) {
    const Instr instr = emit_add_instr(emit, SporkIni);
    instr->m_val      = (m_uint)sp->vm_code;
    return;
  }
  const Instr instr = emit_add_instr(emit, ForkIni);
  instr->m_val      = (m_uint)sp->vm_code;
  instr->m_val2 = (m_uint)sp->type;
}

ANN bool emit_exp_spork(const Emitter emit, const Exp_Unary *unary) {
  struct Sporker sporker = {
      .exp      = unary->unary_type == unary_exp ? unary->exp : NULL,
      .code     = unary->unary_type == unary_code ? unary->code : NULL,
      .type     = exp_self(unary)->type,
      .captures = unary->captures,
      .loc = exp_self(unary)->loc,
      .is_spork = (unary->op == insert_symbol("spork")),
      .emit_var = exp_getvar(exp_self(unary))};
  CHECK_B((sporker.vm_code = spork_prepare(emit, &sporker)));
  if(!sporker.is_spork)
    emit_local_exp(emit, exp_self(unary));
  spork_ini(emit, &sporker);
  // add this if needed
//  uint32_t offset = 0;
  m_uint offset = 0;
  if(emit->env->class_def && sporker.code) {
    Exp exp = {
      .d = { .prim = {
        .d = { .var = insert_symbol("this") },
          .prim_type = ae_prim_id
        }},
      .type = emit->env->class_def,
      .exp_type = ae_exp_primary
    };
    CHECK_B(emit_exp(emit, &exp));
    offset += SZ_INT;
  }
  if(sporker.captures) {
    const CaptureList *caps = sporker.captures;
    for (uint32_t i = 0; i < caps->len; i++) {
      const Capture cap = capturelist_at(caps, i);
      const Value v = cap.var.value;
      Exp exp = {
        .d = { .prim = {
          .d = { .var = cap.var.tag.sym },
          .value = v,
          .prim_type = ae_prim_id
        }},
        .type = v->type,
        .exp_type = ae_exp_primary
      };
      if(cap.is_ref) exp_setvar(&exp, true);
      offset += exp_size(&exp);
      CHECK_B(emit_exp(emit, &exp));
//      emit_exp_addref(emit, &exp, -exp_size(&exp));
    }
  }
  if(offset) {
    emit_regmove(emit, -offset);
    const Instr args  = emit_add_instr(emit, SporkCode);
    args->m_val       = offset;
  }
  (unary->unary_type == unary_code ? spork_code : spork_func)(emit, &sporker);
  return true;
}

ANN static bool emit_exp_unary(const Emitter emit, const Exp_Unary *unary) {
  const Type t    = exp_self(unary)->type;
  const Type base = actual_type(emit->gwion, t);
  CHECK_B(ensure_emit(emit, base));
  // no pos ?
  struct Op_Import opi = {.op = unary->op, .data = (uintptr_t)unary};
  if (unary->unary_type == unary_exp && unary->op != insert_symbol("spork") &&
      unary->op != insert_symbol("fork")) {
    CHECK_B(emit_exp_pop_next(emit, unary->exp));
    opi.rhs = unary->exp->type;
  }
  return op_emit(emit, &opi);
}

ANN static bool emit_implicit_cast(const Emitter emit,
                                     Exp*  from, const Type to) {
  const struct Implicit imp = { .e=from, .t=to, . loc = from->loc};
  // no pos
  struct Op_Import opi = {.op   = insert_symbol("@implicit"),
                          .lhs  = from->type,
                          .rhs  = to,
                          .data = (m_uint)&imp,
                          .loc  = from->loc
                         };
  return op_emit(emit, &opi);
}

ANN2(1,2) static Instr _flow(const Emitter emit, Exp* e, Instr *const instr, /*const */bool b) {
  CHECK_O(emit_exp_pop_next(emit, e));
  {
    const Instr ex = (Instr)vector_back(&emit->code->instr);
    if(ex->opcode == eOP_MAX && ex->execute == fast_except) {
      vector_rem(&emit->code->instr, vector_size(&emit->code->instr) - 1);
      free_instr(emit->gwion, ex);
    }
  }

  if(instr)
    *instr = emit_add_instr(emit, NoOp);
  struct Op_Import opi = {
      .op   = insert_symbol(b ? "@conditional" : "@unconditional"),
      .rhs  = e->type,
      .loc  = e->loc,
      .data = (uintptr_t)e};
  CHECK_B(op_emit(emit, &opi));
  return (Instr)vector_back(&emit->code->instr);
}
#define emit_flow(emit, b) _flow(emit, b, NULL, true)

static void emit_maybe_stack(const Emitter emit, const Instr instr, const MaybeVal *mv) {
  instr->opcode = eReg2Mem;
  instr->m_val = -SZ_INT;
  instr->m_val2 = mv->reg;
  if(!emit->code->frame->maybe_stack)
    emit->code->frame->maybe_stack = new_maybevallist(emit->gwion->mp, 0);
  maybevallist_add(emit->gwion->mp, &emit->code->frame->maybe_stack, *mv);
}

ANN static bool emit_exp_if(const Emitter emit, const Exp_If *exp_if) {
  Exp* e = exp_if->if_exp ?: exp_if->cond;
  Instr instr;
  struct M_Vector_ v = {};
  const m_uint reg = emit_local(emit, emit->gwion->type[et_bool]);
  DECL_B(const Instr, op, = _flow(emit, exp_if->cond, &instr, true));
  if (exp_getvar(exp_self(exp_if))) {
    exp_setvar(e, 1);
    exp_setvar(exp_if->else_exp, 1);
  }
  const m_uint nloc = vector_size(&emit->code->frame->stack);
  const m_uint nval = m_vector_size(&emit->code->live_values);
  const uint16_t offset = emit->code->frame->curr_offset;
  const uint16_t vcount = emit->code->frame->value_count;
  CHECK_B(emit_exp(emit, e));
  const m_uint nval_if = m_vector_size(&emit->code->live_values);
  if(nval < nval_if) {
    const m_uint diff = nval_if - nval;
    m_vector_init(&v, sizeof(VMValue), diff);
    memcpy(v.ptr + ARRAY_OFFSET, emit->code->live_values.ptr + ARRAY_OFFSET + nval * sizeof(VMValue), diff * sizeof(VMValue));
  }
  emit->code->frame->curr_offset = offset;
  emit->code->frame->value_count = vcount;
  VLEN(&emit->code->live_values) = nval;
  VLEN(&emit->code->frame->stack) = nloc;

  const Instr op2  = emit_add_instr(emit, Goto);
  op->m_val        = emit_code_size(emit);
  const bool ret = emit_exp(emit, exp_if->else_exp);
  const m_uint nval_else = m_vector_size(&emit->code->live_values);
  if(nval < nval_else) {
    const m_uint diff = nval_else - nval;
    if(!v.ptr) {
      m_vector_init(&v, sizeof(VMValue), diff);
      memcpy(v.ptr + ARRAY_OFFSET, emit->code->live_values.ptr + ARRAY_OFFSET + nval * sizeof(VMValue), diff * sizeof(VMValue));
    } else {
      for(m_uint i = 0; i < diff; i++) {
        m_vector_add(&v, m_vector_addr(&emit->code->live_values, i + nval));
      }
    }
  }
  emit->code->frame->curr_offset = offset;
  emit->code->frame->value_count = vcount;
  VLEN(&emit->code->live_values) = nval;
  VLEN(&emit->code->frame->stack) = nloc;

  if(v.ptr) {
    MaybeVal mv = (MaybeVal) {
      .ptr = v.ptr,
      .reg = reg,
      .limit = nval_if - nval,
    };
    emit_maybe_stack(emit, instr, &mv);
  }
  op2->m_val       = emit_code_size(emit);
  return ret;
}

ANN static bool emit_lambda(const Emitter emit, const Exp_Lambda *lambda) {
  const EmitterStatus status = emit->status;
  emit->status = (EmitterStatus){};
  CHECK_B(emit_func_def(emit, lambda->def));
  if (vflag(lambda->def->base->func->value_ref, vflag_member) &&
      !exp_getvar(exp_self(lambda)))
    emit_add_instr(emit, RegPushMem);
  emit_pushimm(emit, (m_uint)lambda->def->base->func->code);
  emit->status = status;
  return true;
}

ANN static bool emit_exp_lambda(const Emitter     emit,
                                  const Exp_Lambda *lambda) {
  if (!lambda->def->base->func) {
    exp_self(lambda)->type = emit->gwion->type[et_void];
    return true;
  }
  struct EnvSet es = {.env   = emit->env,
                      .data  = emit,
                      .func  = (_envset_func)emit_cdef,
                      .scope = emit->env->scope->depth,
                      .flag  = tflag_emit};
  CHECK_B(envset_pushv(&es, lambda->def->base->func->value_ref));
  const bool ret = emit_lambda(emit, lambda);
  envset_pop(&es, lambda->owner);
  return ret;
}

ANN static bool emit_exp_td(const Emitter emit, Type_Decl *td) {
  const Type base = exp_self(td)->type;
  if(!is_func(emit->gwion, base))
    emit_pushimm(emit, (m_uint)base);
  else {
    const Instr instr = emit_add_instr(emit, SetFunc);
    instr->m_val = (m_uint)base->info->func;
  }
  return true;
}

ANN static bool emit_exp_named(const Emitter emit, Exp_Named *exp) {
//  gw_out("these should be changed in verify pass");
  return emit_exp(emit, exp->exp);
}
DECL_EXP_FUNC(emit, bool, Emitter,)

ANN2(1) bool emit_exp(const Emitter emit, /* const */ Exp* e) {
  Exp* exp = e;
  do {
    if (emit->info->debug){
      if(emit->status.line < e->loc.first.line) {
        const Instr instr = emit_add_instr(emit, DebugLine);
        instr->m_val = emit->status.line = e->loc.first.line;
      }
      emit->status.line = e->loc.first.line;
    }
    CHECK_B(emit_exp_func[exp->exp_type](emit, &exp->d));
    if (exp->cast_to) CHECK_B(emit_implicit_cast(emit, exp, exp->cast_to));
    if (is_object(emit->gwion, exp->type) &&
        (e->cast_to ? is_object(emit->gwion, e->cast_to) : true) &&
        e->exp_type == ae_exp_decl && GET_FLAG(e->d.exp_decl.var.td, late) &&
        exp_getuse(e) && !exp_getvar(e) &&
        GET_FLAG(e->d.exp_decl.var.vd.value, late))
    emit_fast_except(emit, e->d.exp_decl.var.vd.value->from, e->loc);
  } while ((exp = exp->next));
  return true;
}

ANN static bool emit_if_const(const Emitter emit, const Stmt_If stmt) {
  if (stmt->cond->d.prim.d.gwint.num) return emit_stmt(emit, stmt->if_body);
  return stmt->else_body ? emit_stmt(emit, stmt->else_body) : true;
}

ANN static bool emit_if(const Emitter emit, const Stmt_If stmt) {
  if (stmt->cond->exp_type == ae_exp_primary &&
      stmt->cond->d.prim.prim_type == ae_prim_num)
    return emit_if_const(emit, stmt);
  DECL_B(const Instr, op, = emit_flow(emit, stmt->cond));
  CHECK_B(scoped_stmt(emit, stmt->if_body));
  const Instr op2 = emit_add_instr(emit, Goto);
  op->m_val       = emit_code_size(emit);
  if (stmt->else_body) CHECK_B(scoped_stmt(emit, stmt->else_body));
  op2->m_val = emit_code_size(emit);
  return true;
}

ANN static bool emit_stmt_if(const Emitter emit, const Stmt_If stmt) {
  emit_push_scope(emit);
  const bool ret = emit_if(emit, stmt);
  emit_pop_scope(emit);
  return ret;
}

ANN static bool emit_stmt_code(const Emitter emit, const Stmt_Code stmt) {
  if(!stmt->stmt_list) return true;
  emit_push_scope(emit);
  ++emit->env->scope->depth;
  const bool ret =  emit_stmt_list(emit, stmt->stmt_list);
  emit_pop_scope(emit);
  --emit->env->scope->depth;
  return ret;
}

ANN static bool optimize_tail_call(const Emitter emit, const Exp_Call *e) {
  if (e->args) {
    CHECK_B(emit_func_args(emit, e));
    const Func f = e->func->type->info->func;
    emit_regmove(emit, -f->def->stack_depth);
    emit_args(emit, f);
  }
  emit_add_instr(emit, Goto);
  return true;
}

ANN static bool emit_stmt_return(const Emitter emit, const Stmt_Exp stmt) {
  CHECK_B(emit_defers2(emit));
  if (stmt->val) {
    if (stmt->val->exp_type == ae_exp_call) {
      const Func f = stmt->val->d.exp_call.func->type->info->func;
      if (stmt->val->exp_type == ae_exp_call && emit->env->func == f)
        return optimize_tail_call(emit, &stmt->val->d.exp_call);
    }
//    if(!stmt->val->ref && tflag(stmt->val->type, tflag_compound))
//      emit_local(emit, stmt->val->type);
    emit->status.in_return = true;
    CHECK_B(emit_exp(emit, stmt->val));
    emit->status.in_return = false;
  }
  vector_add(&emit->code->stack_return, (vtype)emit_add_instr(emit, Goto));
  return true;
}

ANN static inline bool emit_jump_index(const Emitter emit, const Vector v,
                                         const m_uint n) {
  vector_add(v, 0); // make room
  const m_uint sz  = vector_size(v);
  m_uint        idx = 1;
  for (m_uint i = sz; --i > 1;) {
    if (!vector_at(v, i) && ++idx == n) {
      m_uint *data = v->ptr + OFFSET + i;
      memmove(data + 1, data, (sz - i) * SZ_INT);
      const Instr instr = emit_add_instr(emit, Goto);
      VPTR(v, i - 1)    = (m_uint)instr;
      return true;
    }
  }
  return false;
}

ANN static inline bool emit_jump(const Emitter    emit, const Stmt_Index stmt,
  const Vector v) {
  CHECK_B(emit_defers2(emit));
  if (stmt->idx == -1 || stmt->idx == 1)
    vector_add(v, (vtype)emit_add_instr(emit, Goto));
  else if (stmt->idx) {
    if (!emit_jump_index(emit, v, stmt->idx))
      ERR_B(stmt_self(stmt)->loc, _("too many jumps required."))
  }
  return true;
}

ANN static inline bool emit_stmt_continue(const Emitter    emit,
                                            const Stmt_Index stmt) {
  return emit_jump(emit, stmt, &emit->code->stack_cont);
}
ANN static inline bool emit_stmt_break(const Emitter         emit,
                                         const Stmt_Index stmt NUSED) {
  return emit_jump(emit, stmt, &emit->code->stack_break);
}

ANN static inline void emit_push_stack(const Emitter emit) {
  emit_push_scope(emit);
  vector_add(&emit->code->stack_cont, (vtype)NULL);
  vector_add(&emit->code->stack_break, (vtype)NULL);
}

ANN static void set_pcs(const Vector v, const m_uint pc) {
  m_uint i;
  for(i = vector_size(v) + 1; --i;) {
    Instr instr = (Instr)vector_at(v, i - 1);
    if(!instr) break;
    instr->m_val = pc;
  }
  VLEN(v) = i - 1;
}

ANN static void emit_pop_stack(const Emitter emit, const m_uint index) {
  set_pcs(&emit->code->stack_cont, index);
  set_pcs(&emit->code->stack_break, emit_code_size(emit));
  emit_pop_scope(emit);
}

static INSTR(run_always) {
  shreduler_remove(shred->tick->shreduler, shred, 0);
}

ANN static bool _emit_stmt_flow(const Emitter emit, const Stmt_Flow stmt,
                                  const m_uint index) {
  Instr           op       = NULL;
  const bool is_while = stmt_self(stmt)->stmt_type == ae_stmt_while;
  const bool is_const = stmt->cond->exp_type == ae_exp_primary &&
                        stmt->cond->d.prim.prim_type == ae_prim_num;
  if (!stmt->is_do) {
    if (!is_const) {
      if(is_while && !stmt->body->d.stmt_code.stmt_list &&
          stmt->cond->d.prim.prim_type == ae_prim_id &&
          !strcmp("true", s_name(stmt->cond->d.prim.d.var))) {
        (void)emit_add_instr(emit, run_always);
      }
      op = _flow(emit, stmt->cond, NULL, is_while);
    } else if ((!is_while && stmt->cond->d.prim.d.gwint.num) ||
             (is_while && !stmt->cond->d.prim.d.gwint.num))
      return true;
  }
  CHECK_B(scoped_stmt(emit, stmt->body));
  if (stmt->is_do) {
    if (!is_const) {
      CHECK_B((op = _flow(emit, stmt->cond, NULL, !is_while)));
      op->m_val = index;
    } else if ((is_while && stmt->cond->d.prim.d.gwint.num) ||
               (!is_while && !stmt->cond->d.prim.d.gwint.num)) {
      const Instr goto_ = emit_add_instr(emit, Goto);
      goto_->m_val      = index;
    }
  } else {
    const Instr goto_ = emit_add_instr(emit, Goto);
    goto_->m_val      = index;
    if (op) op->m_val = emit_code_size(emit);
  }
  return true;
}

ANN static bool emit_stmt_flow(const Emitter emit, const Stmt_Flow stmt) {
  const m_uint index = emit_code_size(emit);
  emit_push_stack(emit);
  const bool ret = _emit_stmt_flow(emit, stmt, index);
  emit_pop_stack(emit, index);
  return ret;
}

ANN static bool _emit_stmt_for(const Emitter emit, const Stmt_For stmt,
                                 m_uint *action_index) {
  CHECK_B(emit_stmt(emit, stmt->c1));
  const m_uint index = emit_code_size(emit);
  DECL_B(const Instr, op, = emit_flow(emit, stmt->c2->d.stmt_exp.val));
  CHECK_B(scoped_stmt(emit, stmt->body));
  *action_index = emit_code_size(emit);
  if (stmt->c3) {
    CHECK_B(emit_exp(emit, stmt->c3));
    pop_exp(emit, stmt->c3);
  }
  const Instr _goto = emit_add_instr(emit, Goto);
  _goto->m_val      = index;
  op->m_val         = emit_code_size(emit);
  return true;
}

ANN static bool emit_stmt_for(const Emitter emit, const Stmt_For stmt) {
  emit_push_stack(emit);
  m_uint       action_index = 0;
  const bool ret          = _emit_stmt_for(emit, stmt, &action_index);
  emit_pop_stack(emit, action_index);
  return ret;
}

ANN static Instr each_op(const Emitter emit, const Looper *loop) {
  struct Op_Import opi = {
    .lhs = loop->exp->type,
    .op = insert_symbol("@each"),
    .data = (m_uint)loop
  };
  CHECK_B(op_emit(emit, &opi));
  return loop->instr;
}

ANN static inline bool roll(const Emitter emit, Looper*const loop) {
  const Instr instr = loop->roll(emit, loop);
//  DECL_B(const Instr, instr, = each_op(emit, loop)); // maybe check in check.c
  CHECK_B(scoped_stmt(emit, loop->stmt));
  instr->m_val = emit_code_size(emit) + 1; // pass after goto
  return true;
}

ANN static inline void unroll_init(const Emitter emit, const m_uint n) {
  emit_memsetimm(emit, emit_local(emit, emit->gwion->type[et_int]), n);
}

ANN static inline bool unroll_run(const Emitter        emit,
                                    struct Looper *loop) {
  const Instr instr = each_op(emit, loop);
  CHECK_B(scoped_stmt(emit, loop->stmt));
  if(instr)
    vector_add(&loop->unroll_v, (m_uint)instr);
  return true;
}

ANN static bool _unroll(const Emitter emit, Looper *loop) {
  scoped_ini(emit);
  const Instr unroll = emit_add_instr(emit, Unroll);
  unroll->m_val      = loop->offset;
  const m_uint start = emit_code_size(emit);
  CHECK_B(unroll_run(emit, loop));
  const m_uint end = emit_code_size(emit);
  for (m_uint i = 1; i < loop->n; ++i) CHECK_B(unroll_run(emit, loop));
  unroll->m_val2      = end - start;
  const Instr unroll2 = emit_add_instr(emit, VM_IN);
  unroll2->m_val      = (m_uint)unroll;
  scoped_end(emit);
  return true;
}

ANN static bool unroll(const Emitter emit, Looper *loop) {
  if(loop->unroll)
    vector_init(&loop->unroll_v);
  const bool ret = _unroll(emit, loop);
  if(loop->unroll) {
    for(m_uint i = 0; i < vector_size(&loop->unroll_v); i++) {
      const Instr instr = (Instr)vector_at(&loop->unroll_v, i);
      instr->m_val = emit_code_size(emit) + 1; // + 2 for arrays
    }
    vector_release(&loop->unroll_v);
  }
  return ret;
}

ANN static inline bool looper_run(const Emitter        emit,
                                    Looper *const loop) {
  return (!loop->n ? roll : unroll)(emit, loop);
}

ANN static bool _emit_stmt_each(const Emitter emit, const Stmt_Each stmt,
                                  m_uint *end_pc) {
  const uint n = emit->status.unroll;
  const m_uint arr_offset = emit_local(emit, emit->gwion->type[et_int]); // array?
  const m_uint key_offset = /*!stmt->idx
     ? */emit_local(emit, emit->gwion->type[et_int])
     /*: emit_local(emit, stmt->idx->v->type)*/;

  const m_uint val_offset = emit_localn(emit, stmt->var.value->type); // localn ?
  emit_regtomem(emit, arr_offset, 0);
  emit_memsetimm(emit, key_offset, -1);
  stmt->var.value->from->offset = val_offset;
//value_addref(stmt->v);
  nspc_add_value(emit->env->curr, stmt->var.tag.sym, stmt->var.value);
  if (stmt->idx.tag.sym) {
    stmt->idx.value->from->offset = key_offset;
  nspc_add_value(emit->env->curr, stmt->idx.tag.sym, stmt->var.value);
//value_addref(stmt->idx->v);
  }
  struct Looper loop   = {.exp  = stmt->exp,
                        .stmt   = stmt->body,
                        .offset = arr_offset,
                        .n      = n,
                        .roll   = each_op,
                        .unroll = each_op,
                        .idx    = stmt->idx.tag.sym ? &stmt->idx : NULL,
                        .init = false
                        };
  if (n) {
    loop.offset -= SZ_INT;
    struct Op_Import opi = {
      .lhs = loop.exp->type,
      .op = insert_symbol("@each_init"),
      .data = (m_uint)&loop
    };
    CHECK_B(op_emit(emit, &opi));
  }
  const m_uint  ini_pc = emit_code_size(emit);
  CHECK_B(looper_run(emit, &loop));
  *end_pc         = emit_code_size(emit);
  const Instr tgt = emit_add_instr(emit, Goto);
  tgt->m_val      = ini_pc;
  return true;
}

ANN static bool emit_stmt_each(const Emitter emit, const Stmt_Each stmt) {
  const uint n = emit->status.unroll;
  nspc_push_value(emit->gwion->mp, emit->env->curr);

  CHECK_B(emit_exp(emit, stmt->exp)); // add ref?
  emit_regmove(emit, -SZ_INT);

  if (n) {
    unroll_init(emit, n);
    emit_local(emit, emit->gwion->type[et_int]);
  }

  emit_push_stack(emit);
  m_uint       end_pc = 0;
  const bool ret    = _emit_stmt_each(emit, stmt, &end_pc);
  emit_pop_stack(emit, end_pc);
  nspc_pop_value(emit->gwion->mp, emit->env->curr);
  emit->status.unroll = 0;
  return ret;
}

ANN static Instr stmt_loop_roll(const Emitter emit, const Looper *loop) {
  const Instr eq = emit_add_instr(emit, Repeat);
  eq->m_val2     = loop->offset;
  return eq;
}

ANN static Instr stmt_loop_roll_idx(const Emitter        emit,
                                    const Looper *loop) {
  const Instr instr = emit_add_instr(emit, RepeatIdx);
  instr->m_val2     = loop->offset;
  return instr;
}

ANN static bool _emit_stmt_loop(const Emitter emit, const Stmt_Loop stmt,
                                  m_uint *index) {
  const uint n = emit->status.unroll;
  if (n) {
    unroll_init(emit, n);
    emit->status.unroll = 0;
  }
  const m_uint offset = emit_local(emit, emit->gwion->type[et_int]);
  if (stmt->idx.tag.sym) {
    emit_memsetimm(emit, emit_local(emit, emit->gwion->type[et_int]), 0);
    emit_memsetimm(emit, offset, -1);
    stmt->idx.value->from->offset = offset;
  }
  CHECK_B(emit_exp(emit, stmt->cond));
  emit_regmove(emit, -SZ_INT);
  emit_regtomem(emit, offset + !!stmt->idx.tag.sym * SZ_INT, 0);
  *index             = emit_code_size(emit);
  struct Looper loop = {.exp    = stmt->cond,
                        .stmt   = stmt->body,
                        .offset = offset,
                        .n      = n,
                        .roll =
                            !stmt->idx.tag.sym ? stmt_loop_roll : stmt_loop_roll_idx};
  CHECK_B(looper_run(emit, &loop));
  const Instr _goto = emit_add_instr(emit, Goto);
  _goto->m_val      = *index;
  return true;
}

ANN static bool emit_stmt_loop(const Emitter emit, const Stmt_Loop stmt) {
  emit_push_stack(emit);
  m_uint       index = 0;
  const bool ret   = _emit_stmt_loop(emit, stmt, &index);
  emit_pop_stack(emit, index);
  return ret;
}

ANN static bool emit_type_def(const Emitter emit, const Type_Def tdef) {
  if (tdef->when_def) CHECK_B(emit_func_def(emit, tdef->when_def));
  if (tflag(tdef->type, tflag_cdef))
    return emit_class_def(emit, tdef->type->info->cdef);
  return true;
}

ANN static bool emit_enum_def(const Emitter emit NUSED, const Enum_Def edef) {
  set_tflag(edef->type, tflag_emit);
  return true;
}

ANN static bool emit_union_def(const Emitter emit NUSED,
                                 const Union_Def    udef) {
  if (tmpl_base(udef->tmpl)) return true;
  set_tflag(udef->type, tflag_emit);
  return true;
}

// add a Goto. later the index is set to the ont of the happy path
// maybe this and the function above can use the same machinery as returns or
// breaks
ANN static inline void emit_try_goto(const restrict Emitter emit,
                                     const Vector           v) {
  const Instr instr = emit_add_instr(emit, Goto);
  vector_add(v, (m_uint)instr);
}

// set Goto indexes the one of the happy path
ANN static inline void try_goto_indexes(const Vector v, const m_uint pc) {
  for (m_uint i = 0; i < vector_size(v); i++) {
    const Instr instr = (Instr)vector_at(v, i);
    instr->m_val      = pc;
  }
}

ANN static inline bool emit_handler_list(const restrict Emitter emit,
                                           const HandlerList   *handlers,
                                           const Vector         v) {
  emit_push_scope(emit);
  const m_uint offset = emit_local(emit, emit->gwion->type[et_int]);
  for(uint32_t i = 0; i < handlers->len; i++) {
    const Handler handler = handlerlist_at(handlers, i);
    const Instr instr = emit_add_instr(emit, HandleEffect);
    instr->m_val      = emit->status.effect = offset;
    instr->m_val2     = (m_uint)handler.tag.sym;
    CHECK_B(scoped_stmt(emit, handler.stmt));
    emit_try_goto(emit, v);
    instr->m_val = emit_code_size(emit);
  }
//  emit->status.effect = 0;
  emit_pop_scope(emit);
  return true;
}

ANN static inline bool emit_stmt_try(const restrict Emitter emit,
                                       const Stmt_Try         stmt) {
  const m_uint top           = emit->code->frame->try_top;
  emit->code->frame->try_top = emit_code_size(emit);
  (void)emit_add_instr(emit, TryIni);
  struct Vector_ v; // store Gotos to the happy path
  vector_init(&v);
  CHECK_B(scoped_stmt(emit, stmt->stmt));
  emit_try_goto(emit, &v);
  if (!emit->code->frame->handlers.ptr) map_init(&emit->code->frame->handlers);
  CHECK_B(emit_handler_list(emit, stmt->handler, &v));
  try_goto_indexes(&v, emit_code_size(emit));
  vector_release(&v);
  emit->code->frame->try_top = top;
  (void)emit_add_instr(emit, TryEnd);
  return true;
}

ANN static bool emit_stmt_exp(const Emitter           emit,
                                const struct Stmt_Exp_ *exp) {
  return exp->val ? emit_exp(emit, exp->val) : true;
}

ANN static bool emit_case_head(const Emitter emit, Exp* base,
                                 Exp* e, const Symbol op, const Vector v) {
  CHECK_B(emit_exp1(emit, base));
  CHECK_B(emit_exp1(emit, e));
  const Exp_Binary bin  = {.lhs = base, .rhs = e, .op = op};
  Exp      ebin = { .d = {.exp_binary = bin}, .exp_type = ae_exp_binary, .loc = e->loc };
  struct Op_Import opi = {.op   = op,
                          .lhs  = base->type,
                          .rhs  = e->type,
                          .data = (uintptr_t)&ebin.d.exp_binary,
                          .loc  = e->loc};
  CHECK_B(op_emit(emit, &opi));
  const Instr instr = emit_add_instr(emit, BranchEqInt);
  vector_add(v, (vtype)instr);
  return true;
}

ANN static bool emit_case_body(const Emitter             emit,
                                 const struct Match *stmt) {
  const Instr when = stmt->when ? emit_flow(emit, stmt->when) : NULL;
  if (stmt->when) CHECK_B(when);
  CHECK_B(emit_stmt_list(emit, stmt->list));
  const Instr instr = emit_add_instr(emit, Goto);
  vector_add(&emit->env->scope->match->vec, (vtype)instr);
  if (when) when->m_val = emit_code_size(emit);
  return true;
}

ANN static bool case_value(const Emitter emit, Exp* base, Exp* e) {
  const Value v   = e->d.prim.value;
  v->from->offset = emit_local(emit, base->type);
  emit_regtomem4(emit, v->from->offset, base->type->size);
  return true;
}

ANN Symbol case_basic_op(const Env env, const Type base, Exp* e);
#define CASE_PASS (Symbol)1
ANN static Symbol case_op(const Emitter emit, Exp* base, Exp* e,
                          const Vector vec, const uint n) {
  if (e->exp_type == ae_exp_primary) {
    if (e->d.prim.prim_type == ae_prim_id) {
      if (e->d.prim.d.var == insert_symbol("_")) return CASE_PASS;
      if (!nspc_lookup_value1(emit->env->curr, e->d.prim.d.var)) {
        if (!n) {
          CHECK_O(emit_exp(emit, base));
          emit_regmove(emit, -base->type->size);
        }
        CHECK_O(case_value(emit, base, e));
        return CASE_PASS;
      }
    }
  } else if (tflag(base->type, tflag_union) &&
             e->exp_type == ae_exp_call) {
    Exp* func = e->d.exp_call.func;
    if (func->d.prim.prim_type == ae_prim_id) {
      const Map map = &base->type->nspc->info->value->map;
      for (m_uint i = 0; i < map_size(map); ++i) {
        if (VKEY(map, i) == (m_uint)func->d.prim.d.var) {
          const Value v = (Value)VVAL(map, i);
          if (v) {
            if (!n)
              CHECK_O(emit_exp(emit, base));
            else
              emit_regmove(emit, SZ_INT);
            const Instr check = emit_add_instr(emit, UnionCheck);
            check->m_val2     = i;
            vector_add(vec, (m_uint)check);
            emit_unionmember(emit, i, v->type->size, false);
            emit_regmove(emit, -v->type->size);
            case_op(emit, e->d.exp_call.args, e->d.exp_call.args, vec, i + 1);
            return CASE_PASS;
          }
        }
      }
    }
  }
  if (!n) return case_basic_op(emit->env, base->type, e);
  emit_regmove(emit, SZ_INT);
  CHECK_O(emit_exp(emit, e));
  const Exp_Binary bin  = {.lhs = base, .rhs = e, .op = insert_symbol("?=")};
  Exp      ebin = {.d = {.exp_binary = bin}, .loc = e->loc };
  struct Op_Import opi = {.op   = insert_symbol("?="),
                          .lhs  = base->type,
                          .rhs  = e->type,
                          .data = (uintptr_t)&ebin.d.exp_binary,
                          .loc  = e->loc};
  CHECK_B(op_emit(emit, &opi));
  const Instr instr = emit_add_instr(emit, BranchEqInt);
  vector_add(vec, (vtype)instr);
  return CASE_PASS;
}

ANN static bool _emit_stmt_match_case(const Emitter             emit,
                                        const struct Match *stmt,
                                        const Vector              v) {
  Exp*          e    = stmt->cond;
  const Vector cond = &emit->env->scope->match->cond;
  for (m_uint i = 0; i < vector_size(cond) && e; e = e->next, ++i) {
    Exp*    base = (Exp*)vector_at(cond, i);
    const Symbol op   = case_op(emit, base, e, v, 0);
    if (op != CASE_PASS) CHECK_B(emit_case_head(emit, base, e, op, v));
  }
  CHECK_B(emit_case_body(emit, stmt));
  return true;
}

ANN static bool emit_stmt_match_case(const Emitter             emit,
                                       const struct Match *stmt) {
  emit_push_scope(emit);
  struct Vector_ v;
  vector_init(&v);
  const bool ret = _emit_stmt_match_case(emit, stmt, &v);
  emit_pop_scope(emit);
  for (m_uint i = 0; i < vector_size(&v); ++i) {
    const Instr instr = (Instr)vector_at(&v, i);
    instr->m_val      = emit_code_size(emit);
  }
  vector_release(&v);
  return ret;
}

ANN static inline void match_unvec(struct Matcher *const match,
                                   const m_uint         pc) {
  const Vector vec = &match->vec;
  for (m_uint i = 0; i < vector_size(vec); ++i) {
    const Instr instr = (Instr)VPTR(vec, i);
    instr->m_val      = pc;
  }
  vector_release(vec);
}

ANN static bool emit_stmt_cases(const Emitter emit, StmtList *list) {
  for(m_uint i = 0; i < list->len; i++) {
    const Stmt* stmt = stmtlist_ptr_at(list, i);
    CHECK_B(emit_stmt_match_case(emit, &stmt->d.stmt_match));
  }
  return true;
}

ANN static bool emit_match(const Emitter             emit,
                             const struct Match *stmt) {
  if (stmt->where) CHECK_B(emit_stmt(emit, stmt->where));
  MATCH_INI(emit->env->scope)
  vector_init(&m.vec);
  const bool ret = emit_stmt_cases(emit, stmt->list);
  match_unvec(&m, emit_code_size(emit));
  MATCH_END(emit->env->scope)
  return ret;
}

ANN static bool emit_stmt_match(const Emitter             emit,
                                  const struct Match *stmt) {
  emit_push_scope(emit);
  const bool ret = emit_match(emit, stmt);
  emit_pop_scope(emit);
  return ret;
}

ANN static bool emit_stmt_pp(const Emitter          emit,
                               const struct Stmt_PP_ *stmt) {
  if (stmt->pp_type == ae_pp_include)
    emit->env->name = stmt->data;
  else if (stmt->pp_type == ae_pp_locale)
    emit->locale = stmt->exp->d.exp_lambda.def->base->func;
  else if (stmt->pp_type == ae_pp_pragma) {
    if (!strncmp(stmt->data, "unroll", strlen("unroll")))
      emit->status.unroll = strtol(stmt->data + 6, NULL, 10);
  }
  return true;
}

ANN static bool emit_stmt_defer(const Emitter             emit,
                                  const struct Stmt_Defer_ *stmt) {
  vector_add(&emit->code->frame->defer, (m_uint)stmt->stmt);
  return true;
}

ANN static bool emit_stmt_retry(const Emitter                  emit,
                                  const struct Stmt_Index_ *stmt NUSED) {
  const Instr instr = emit_add_instr(emit, Goto);
  instr->m_val      = emit->code->frame->try_top + 1;
  return true;
}

#define emit_stmt_while emit_stmt_flow
#define emit_stmt_until emit_stmt_flow
#define emit_stmt_spread dummy_func
#define emit_stmt_using dummy_func
#define emit_stmt_import dummy_func

DECL_STMT_FUNC(emit, bool, Emitter,);

ANN static bool emit_stmt(const Emitter emit, Stmt* stmt) {
  CHECK_B(emit_stmt_func[stmt->stmt_type](emit, &stmt->d));
  if (stmt->stmt_type == ae_stmt_exp && stmt->d.stmt_exp.val)
    pop_exp(emit, stmt->d.stmt_exp.val);
  return true;
}

ANN static bool emit_stmt_list(const Emitter emit, StmtList *l) {
  for(m_uint i = 0; i < l->len; i++) {
    Stmt* stmt = stmtlist_ptr_at(l, i);
    CHECK_B(emit_stmt(emit, stmt));
  }
  return true;
}

ANN static bool emit_exp_dot(const Emitter emit, const Exp_Dot *member) {
  struct Op_Import opi = {.op   = insert_symbol("."),
                          .lhs  = member->base->type,
                          .rhs  = exp_self(member)->type,
                          .data = (uintptr_t)member,
                          .loc  = exp_self(member)->loc};
  return op_emit(emit, &opi);
}

ANN static inline void emit_func_def_init(const Emitter emit, const Func func) {
  emit_push_code(emit, func->name);
  if(valuelist_len(func->_wait)) {
    const Instr instr = emit_add_instr(emit, FuncWait);
    instr->m_val = (m_uint) func;
  }
}

ANN static void emit_func_def_args(const Emitter emit, ArgList *args) {
  for(uint32_t i = 0; i < args->len; i++) {
    const Arg arg = arglist_at(args, i);
    const Type type = arg.var.vd.value->type;
    emit->code->stack_depth += type->size;
    arg.var.vd.value->from->offset = emit_localn(emit, type);
    nspc_add_value(emit->env->curr, insert_symbol(arg.var.vd.value->name), arg.var.vd.value);
  }
}

ANN static bool emit_func_def_return(const Emitter emit) {
  const m_uint val = emit_code_size(emit);
  CHECK_B(emit_defers(emit));
  emit_return_pc(emit, val);
  vector_clear(&emit->code->stack_return);
  const Func f = emit->env->func;
  if (f->memoize && fflag(f, fflag_pure)) {
    const Instr instr = emit_add_instr(emit, MemoizeStore);
    instr->m_val      = f->def->stack_depth;
  }
  return true;
}

ANN static VM_Code emit_internal(const Emitter emit, const Func f) {
  if (f->def->base->tag.sym == insert_symbol("@dtor"))
    return emit->env->class_def->nspc->dtor = finalyze(emit, DTOR_EOC);
  else if (f->def->base->tag.sym == insert_symbol("@gack")) {
    emit_regmove(emit, -SZ_INT - f->value_ref->from->owner_class->size);
    const Instr instr                       = emit_add_instr(emit, RegPushMem);
    instr->m_val                            = SZ_INT;
    f->code                                 = finalyze(emit, FuncReturn);
    return emit->env->class_def->info->gack = f->code;
  }
  return finalyze(emit, FuncReturn);
}

ANN static inline VM_Code _emit_func_def_code(const Emitter emit,
                                              const Func    func) {
  if(is_new(func->def)) {
    const Type t = func->value_ref->from->owner_class;
    if(!tflag(t, tflag_struct))
      emit_add_instr(emit, RegPushMem);
    else  {
      const Instr instr = emit_add_instr(emit, RegPushMemDeref);
      instr->m_val2 = t->size;
    }
  }
  const VM_Code code = !fbflag(func->def->base, fbflag_internal) ? finalyze(emit, FuncReturn)
                                                   : emit_internal(emit, func);
  return code;
}

ANN static VM_Code emit_func_def_code(const Emitter emit, const Func func) {
  const VM_Code code = _emit_func_def_code(emit, func);
  if (func->memoize && fflag(func, fflag_pure)) code->is_memoize = true;
  code->ret_type = func->def->base->ret_type;
  return code;
}

ANN static bool emit_func_def_body(const Emitter emit, const Func_Def fdef) {
  if (fdef->base->tag.sym == insert_symbol("@dtor")) emit_local(emit, emit->gwion->type[et_int]);
  if (fdef->base->args) emit_func_def_args(emit, fdef->base->args);
  if (fdef->d.code) {
    if(!fdef->builtin) {
      const bool ctor = is_ctor(fdef);
      if(!ctor)
        scoped_ini(emit);
      const bool ret = emit_stmt_list(emit, fdef->d.code);
      if(!ctor)
        scoped_end(emit);
      CHECK_B(ret);
    }
    else fdef->base->func->code = (VM_Code)vector_at(&fdef->base->func->value_ref->from->owner_class->nspc->vtable, fdef->vt_index);
  }
  emit_func_def_return(emit);
  return true;
}

ANN static Instr me_top(MemoizeEmitter *me) {
  emit_memsetimm(me->emit, me->offset, 0); // idx 
  return emit_memsetimm(me->emit, me->offset + SZ_INT, 0); // pc
}

ANN static void me_ini(MemoizeEmitter *me) {
  const Instr ini = emit_add_instr(me->emit, MemoizeIni);
  ini->m_val      = me->offset;
  ini->m_val2     = me->fdef->stack_depth + me->fdef->base->ret_type->size;
}

ANN static void me_end(MemoizeEmitter *me, const m_uint pc) {
  for (m_uint i = 0; i < vector_size(&me->branch); ++i)
    ((Instr)vector_at(&me->branch, i))->m_val = pc;
}

ANN static void me_bottom(MemoizeEmitter *me, const m_uint pc) {
  emit_regpushmem4(me->emit, me->offset, 0);
  emit_add_instr(me->emit, int_pre_inc);
  emit_regmove(me->emit, -SZ_INT);
  const Instr loop = emit_add_instr(me->emit, Goto);
  loop->m_val      = pc;
}

ANN static void me_ret(MemoizeEmitter *me) {
  emit_regpushmem(me->emit, (me->offset + SZ_INT) * 2, me->fdef->base->ret_type->size, false);
  vector_add(&me->emit->code->stack_return, (vtype)emit_add_instr(me->emit, Goto));
}

ANN static bool me_run(MemoizeEmitter *me, const m_uint pc) {
  me_ini(me);
  if (me->fdef->base->args) CHECK_B(me_arg(me));
  me_ret(me);
  me_end(me, emit_code_size(me->emit));
  me_bottom(me, pc);
  return true;
}

ANN static bool emit_memoize(const Emitter emit, const Func_Def fdef) {
  MemoizeEmitter me = {.emit = emit, .fdef = fdef, .offset = fdef->stack_depth};
  const Instr    pc = me_top(&me);
  const m_uint   top = emit_code_size(emit);
  vector_init(&me.branch);
  const bool ret = me_run(&me, top);
  vector_release(&me.branch);
  pc->m_val2 = emit_code_size(emit);
  return ret;
}

ANN static bool emit_fdef(const Emitter emit, const Func_Def fdef) {
  const Func f = fdef->base->func;
  if (f->memoize && fflag(f, fflag_pure))
    CHECK_B(emit_memoize(emit, fdef));
  nspc_push_value(emit->gwion->mp, emit->env->curr); // handle
  const bool ret = emit_func_def_body(emit, fdef);
  nspc_pop_value(emit->gwion->mp, emit->env->curr); // handle
  return ret;
}

ANN static void emit_fdef_finish(const Emitter emit, const Func_Def fdef) {
  const Func func = fdef->base->func;
  func->code      = emit_func_def_code(emit, func);
  if (func->memoize && fflag(func, fflag_pure))
    func->code->memoize = memoize_ini(emit, func);
}

ANN static inline Capture* get_capture(const Emitter emit, CaptureList *captures, const uint32_t i) {
  Capture *const cap = capturelist_ptr_at(captures, i);
  emit_localn(emit, cap->temp->type);
  return cap;
}

ANN static void emit_lambda_capture(const Emitter emit, const Func_Def fdef) {
  for(uint32_t i = 0; i < fdef->captures->len - 1; i++)
    (void)get_capture(emit, fdef->captures, i);
  const Capture *cap = get_capture(emit, fdef->captures, fdef->captures->len - 1);
  const m_uint offset = cap->temp->from->offset + cap->temp->type->size - fdef->stack_depth;
  emit_regtomem4(emit, fdef->stack_depth, offset);
}

ANN static bool emit_fdef_b(const Emitter emit, const Func_Def fdef) {
  return emit_fdef(emit, fdef) > 0;
}

ANN static bool _emit_func_def(const Emitter emit, const Func_Def f) {
  if (tmpl_base(f->base->tmpl) && fbflag(f->base, fbflag_op)) return true;
  const Func     func   = f->base->func;
  const Func_Def fdef   = func->def;
  const Func     former = emit->env->func;
  if (func->code || tmpl_base(fdef->base->tmpl) || fflag(func, fflag_emit))
    return true;
  set_fflag(func, fflag_emit);
  if(fdef->builtin)  {
    fdef->base->func->code = new_vmcode(emit->gwion->mp, NULL, NULL, func->name, fdef->stack_depth, true, false);
    fdef->base->func->code->native_func = (m_uint)fdef->d.dl_func_ptr;
    return true;
  }
  if ((vflag(func->value_ref, vflag_builtin) &&
      safe_tflag(emit->env->class_def, tflag_tmpl)) || (fdef->base->tmpl && is_new(f))) {
    const Func base =
        nspc_lookup_func1(func->value_ref->from->owner, f->base->tag.sym);
    builtin_func(emit->gwion, func, (f_xfun)base->code->native_func);
    return true;
  }
  const uint   global = GET_FLAG(f->base, global);
  const m_uint scope =
      !global ? emit->env->scope->depth : env_push_global(emit->env);
  if(fdef->base->tmpl) {
    if(tmplarg_ntypes(fdef->base->tmpl->call) != fdef->base->tmpl->call->len) {
      emit_push_code(emit, "function const generic");
      uint32_t size = 0;
      for(uint32_t i = 0; i < fdef->base->tmpl->call->len; i++) {
        const TmplArg targ = tmplarglist_at(fdef->base->tmpl->call, i);
        if(targ.type == tmplarg_td)continue;
        // spec could be null cause of spread ops
        // TODO: check that is safe
        Specialized *spec = specializedlist_ptr_at(fdef->base->tmpl->list, i);
        if(!spec) break;
        CHECK_B(emit_exp(emit, targ.d.exp));
        size += targ.d.exp->type->size;
      }
      fdef->base->func->value_ref->type->nspc->class_data_size = size;
      fdef->base->func->value_ref->type->nspc->class_data = _mp_malloc(emit->gwion->mp, size);
      comptime_end(emit, size, fdef->base->func->value_ref->type->nspc->class_data);
    }
  }
  emit_func_def_init(emit, func);
  if (vflag(func->value_ref, vflag_member)) stack_alloc(emit);
  emit->env->func = func;
  emit_push_scope(emit);
  if (!strcmp(s_name(fdef->base->tag.sym), "@gack")) {
    emit_local(emit, emit->gwion->type[et_int]);
    emit_memsetimm(emit, SZ_INT, 0);
  } else if(fdef->captures) emit_lambda_capture(emit, fdef);
  const bool ret = scanx_fdef(emit->env, emit, fdef, (_envset_func)emit_fdef_b);
  emit_pop_scope(emit);
  emit->env->func = former;
  if (ret)
    emit_fdef_finish(emit, fdef);
  else
    emit_pop_code(emit);
  if (global) env_pop(emit->env, scope);
  return ret;
}

ANN bool emit_func_def(const Emitter emit, const Func_Def fdef) {
  const uint16_t depth = emit->env->scope->depth;
  emit->env->scope->depth = 0;
  const Func locale = emit->locale;
  const bool ret = _emit_func_def(emit, fdef);
  emit->locale = locale;
  emit->env->scope->depth = depth;
  return ret;
}

#define emit_fptr_def  dummy_func
#define emit_trait_def dummy_func
#define emit_extend_def dummy_func
#define emit_prim_def dummy_func

HANDLE_SECTION_FUNC(emit, bool, Emitter,);

ANN static inline bool emit_ast_inner(const Emitter emit, Ast ast) {
  for(m_uint i = 0; i < ast->len; i++) {
    Section section = sectionlist_at(ast, i);
    CHECK_B(emit_section(emit, &section));
  }
  return emit_defers(emit);
}

ANN Code *emit_class_code(const Emitter emit, const m_str name) {
  const m_uint len = strlen(name) + 7;
  char         c[len];
  snprintf(c, len, "class %s", name);
  emit_push_code(emit, c);
  stack_alloc(emit);
  return emit->code;
}

ANN static bool emit_parent(const Emitter emit, const Class_Def cdef) {
  const Type parent = cdef->base.type->info->parent;
  return ensure_emit(emit, parent);
}

ANN static bool emit_class_def_b(const Emitter emit, Class_Def cdef) {
  return emit_class_def(emit, cdef) > 0;
}
ANN static bool emit_union_def_b(const Emitter emit, Union_Def udef) {
  return emit_union_def(emit, udef) > 0;
}
ANN static inline bool emit_cdef(const Emitter emit, const Type t) {
  return scanx_cdef(emit->env, emit, t, (_envset_func)emit_class_def_b,
                    (_envset_func)emit_union_def_b);
}

ANN static bool cdef_parent(const Emitter emit, const Class_Def cdef) {
  const bool tmpl = !!cdef->base.tmpl;
  if (tmpl) CHECK_B(template_push_types(emit->env, cdef->base.tmpl));
  const bool ret = emit_parent(emit, cdef);
  if (tmpl) nspc_pop_type(emit->gwion->mp, emit->env->curr);
  return ret;
}

static INSTR(set) {
  Value v = *(Value*)(shred->reg + -SZ_INT);
  shred->reg -= instr->m_val;
  memcpy(&v->d.ptr, shred->reg, instr->m_val2);
}

ANN static bool emit_class_tmpl(const Emitter emit, const Tmpl *tmpl, const Nspc nspc) {
  if(tmplarg_ntypes(tmpl->call) != tmpl->list->len) {
    comptime_ini(emit, "class tmpl");
    for(uint32_t i = 0; i < tmpl->list->len; i++) {
      const TmplArg targ = tmplarglist_at(tmpl->call, i);
      if(likely(targ.type == tmplarg_td)) continue;
      CHECK_B(emit_exp(emit, targ.d.exp));
      const Specialized spec = specializedlist_at(tmpl->list, i);
      const Value v = nspc_lookup_value1(nspc, spec.tag.sym);
      emit_pushimm(emit, (m_uint)v);
      const Instr instr = emit_add_instr(emit, set);
      instr->m_val2 = targ.d.exp->type->size;
      instr->m_val = instr->m_val2 + SZ_INT;
    }
    comptime_end(emit, 0, NULL);
  }
  return true;
}

ANN static bool _emit_class_def(const Emitter emit, const Class_Def cdef) {
  const Type      t = cdef->base.type;
  set_tflag(t, tflag_emit);
  const Class_Def c = t->info->cdef;
  if (c->base.ext && t->info->parent->info->cdef &&
      !tflag(t->info->parent, tflag_emit)) // ?????
    CHECK_B(cdef_parent(emit, c));
  if (c->base.tmpl) CHECK_B(emit_class_tmpl(emit, c->base.tmpl, c->base.type->nspc));
  if (c->body)
    return scanx_body(emit->env, c, (_envset_func)emit_section, emit);
  return true;
}

ANN static bool emit_class_def(const Emitter emit, const Class_Def cdef) {
  if (tmpl_base(cdef->base.tmpl)) return true;
  if (tflag(cdef->base.type, tflag_emit)) return true;
  const Func locale = emit->locale;
  const bool ret = _emit_class_def(emit, cdef);
  emit->locale = locale;
  return ret;
}

ANN static inline void emit_free_code(const Emitter emit, Code *code) {
  if (vector_size(&code->instr)) free_code_instr(&code->instr, emit->gwion);
  free_code(emit->gwion->mp, code);
}

ANN static VM_Code emit_free_stack(const Emitter emit) {
  LOOP_OPTIM
  for (m_uint i = vector_size(&emit->stack) + 1; --i;)
    emit_free_code(emit, (Code *)vector_at(&emit->stack, i - 1));
  vector_clear(&emit->stack);
  emit_free_code(emit, emit->code);
  return NULL;
}

ANN bool emit_ast(const Env env, Ast *ast) {
  const Emitter emit  = env->gwion->emit;
  const Func locale = emit->locale;
  emit->code          = new_code(emit, emit->env->name);
  emit_push_scope(emit);
  const bool ret = emit_ast_inner(emit, *ast);
  emit_pop_scope(emit);
  if (ret > 0)
    emit->info->code = finalyze(emit, EOC);
  else
   emit_free_stack(emit);
  emit->status = (EmitterStatus){};
  emit->locale = locale;
  return ret;
}
