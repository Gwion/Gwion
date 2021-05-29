#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "array.h"
#include "emit.h"
#include "operator.h"
#include "import.h"
#include "traverse.h"
#include "parse.h"
#include "gwi.h"
#include "emit.h"

static DTOR(array_dtor) {
  if(*(void**)(o->data + SZ_INT))
    xfree(*(void**)(o->data + SZ_INT));
  struct M_Vector_* a = ARRAY(o);
  free_m_vector(shred->info->mp, a);
}

static DTOR(array_dtor_obj) {
  struct M_Vector_* a = ARRAY(o);
  for(m_uint i = 0; i < ARRAY_LEN(a); ++i)
    release(*(M_Object*)(ARRAY_PTR(a) + i * SZ_INT), shred);
}

static DTOR(array_dtor_struct) {
  struct M_Vector_* a = ARRAY(o);
  for(m_uint i = 0; i < ARRAY_LEN(a); ++i)
    struct_release(shred, array_base(o->type_ref), &*(m_bit*)(ARRAY_PTR(a) + i * SZ_INT));
}

ANN M_Object new_array(MemPool p, const Type t, const m_uint length) {
  const M_Object a = new_object(p, NULL, t);
  const m_uint depth = !tflag(t, tflag_typedef) ? t->array_depth : t->info->parent->array_depth;
  const m_uint size = depth > 1 ? SZ_INT : array_base(t)->size;
  ARRAY(a) = new_m_vector(p, size, length);
  return a;
}

static MFUN(vm_vector_rem) {
  const m_int index = *(m_int*)(shred->mem + SZ_INT);
  const M_Vector v = ARRAY(o);
  if(index < 0 || (m_uint)index >= ARRAY_LEN(v))
    return;
  m_vector_rem(v, (vtype)index);
}

static MFUN(vm_vector_rem_obj) {
  const m_int index = *(m_int*)(shred->mem + SZ_INT);
  const M_Vector v = ARRAY(o);
  if(index < 0 || (m_uint)index >= ARRAY_LEN(v))
    return;
  release(*(M_Object*)(ARRAY_PTR(v) + index * ARRAY_SIZE(v)), shred);
  m_vector_rem(v, (vtype)index);
}

static MFUN(vm_vector_rem_struct) {
  const m_int index = *(m_int*)(shred->mem + SZ_INT);
  const M_Vector v = ARRAY(o);
  if(index < 0 || (m_uint)index >= ARRAY_LEN(v))
    return;
  const Type t = o->type_ref;
  struct_release(shred, array_base(t), ARRAY_PTR(v) + index * ARRAY_SIZE(v));
  m_vector_rem(v, (vtype)index);
}

static MFUN(vm_vector_insert) {
  const m_int index = *(m_int*)(shred->mem + SZ_INT);
  const M_Vector v = ARRAY(o);
  if(index < 0 || (m_uint)index > ARRAY_LEN(v))
    return;
  m_vector_insert(v, index, shred->mem + SZ_INT*2);
}

static MFUN(vm_vector_insert_obj) {
  const m_int index = *(m_int*)(shred->mem + SZ_INT);
  const M_Vector v = ARRAY(o);
  if(index < 0 || (m_uint)index > ARRAY_LEN(v))
    return;
  m_vector_insert(v, index, shred->mem + SZ_INT*2);
  ++(*(M_Object*)(shred->mem + SZ_INT*2))->ref;
}

static MFUN(vm_vector_insert_struct) {
  const m_int index = *(m_int*)(shred->mem + SZ_INT);
  const M_Vector v = ARRAY(o);
  if(index < 0 || (m_uint)index > ARRAY_LEN(v))
    return;
  m_vector_insert(v, index, shred->mem + SZ_INT*2);
  struct_addref(shred->info->vm->gwion, array_base(o->type_ref),  shred->mem + SZ_INT*2);
}

static MFUN(vm_vector_size) {
  *(m_uint*)RETURN = ARRAY_LEN(ARRAY(o));
}

static MFUN(vm_vector_depth) {
  *(m_uint*)RETURN = o->type_ref->array_depth;
}

static MFUN(vm_vector_cap) {
  *(m_uint*)RETURN = ARRAY_CAP(ARRAY(o));
}

static MFUN(vm_vector_random) {
  const M_Vector array = ARRAY(o);
  const m_uint sz = ARRAY_LEN(array);
  const m_uint idx = (m_int)(sz) * (gw_rand(shred->info->vm->rand) / (UINT32_MAX + 1.0));
  m_vector_get(array, idx, (void*)RETURN);
}

#define ARRAY_OPCK(a, b, pos)                  \
  const Type l = array_base(a->type);          \
  const Type r = array_base(b->type);          \
  if(isa(r, l) < 0)                            \
    ERR_N(pos, _("array types do not match."));

static OP_CHECK(opck_array_at) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  if(opck_const_rhs(env, data) == env->gwion->type[et_error])
    return env->gwion->type[et_error];
  if(bin->lhs->type != env->gwion->type[et_error]) {
    ARRAY_OPCK(bin->lhs, bin->rhs, exp_self(bin)->pos)
    if(bin->lhs->type->array_depth != bin->rhs->type->array_depth)
      ERR_N(exp_self(bin)->pos, _("array depths do not match."));
  }
  if(bin->rhs->exp_type == ae_exp_decl) {
    if(bin->rhs->d.exp_decl.list->self->array &&
          bin->rhs->d.exp_decl.list->self->array->exp)
      ERR_N(exp_self(bin)->pos, _("do not provide array for 'xxx => declaration'."));
  }
  exp_setvar(bin->rhs, 1);
  return bin->rhs->type;
}

ANN static Type check_array_shift(const Env env,
    const Exp a, const Exp b, const m_str str, const loc_t pos) {
/*  if(a->type == env->gwion->type[et_error] &&
      b->type->array_depth > 1)
    return a->type;*/
  ARRAY_OPCK(a, b, pos)
  if(a->type->array_depth == b->type->array_depth + 1)
    return a->type;
  else if(a->type->array_depth == b->type->array_depth)
    return a->type;
  ERR_N(pos, "array depths do not match for '%s'.", str);
}

static OP_CHECK(opck_array_sl) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  return check_array_shift(env, bin->lhs, bin->rhs, "<<", exp_self(bin)->pos);
}

static OP_CHECK(opck_array_sr) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  return check_array_shift(env, bin->rhs, bin->lhs, ">>", exp_self(bin)->pos);
}

ANN static inline m_bool emit_array_shift(const Emitter emit, const f_instr exec) {
  const Instr pop = emit_add_instr(emit, RegMove);
  pop->m_val = -SZ_INT;
  (void)emit_add_instr(emit, exec);
  return GW_OK;
}

static INSTR(ArrayAppendFront) {
  const M_Object o = *(M_Object*)(shred->reg);
  const M_Vector a = ARRAY(o);
  m_vector_add_front(a, shred->reg - ARRAY_SIZE(a));
}

static INSTR(ArrayConcatLeft) {
  const M_Object obase = *(M_Object*)(shred->reg - SZ_INT);
  const M_Object omore = *(M_Object*)(shred->reg);
  const M_Vector base = ARRAY(obase);
  const M_Vector more = ARRAY(omore);
  const m_uint len = ARRAY_LEN(base);
  const m_uint sz = ARRAY_SIZE(base);
  if((ARRAY_LEN(base) += ARRAY_LEN(more)) >= ARRAY_CAP(base)) {
    ARRAY_CAP(base) += ARRAY_CAP(more);
    m_bit *ptr = (m_bit*)xrealloc(base->ptr, ARRAY_OFFSET + ARRAY_CAP(base) * sz);
    base->ptr = ptr;
  }
  m_bit *data = more->ptr + ARRAY_OFFSET;
  memmove(ARRAY_PTR(base) + (len - 1) * sz, data, sz);
}

static INSTR(ArrayConcatRight) {
  const M_Object obase = *(M_Object*)(shred->reg);
  const M_Object omore = *(M_Object*)(shred->reg - SZ_INT);
  const M_Vector base = ARRAY(obase);
  const M_Vector more = ARRAY(omore);
  const m_uint len = ARRAY_LEN(base);
  const m_uint sz = ARRAY_SIZE(base);
  if((ARRAY_LEN(base) += ARRAY_LEN(more)) >= ARRAY_CAP(base)) {
    ARRAY_CAP(base) += ARRAY_CAP(more);
    m_bit *ptr = (m_bit*)xrealloc(base->ptr, ARRAY_OFFSET + ARRAY_CAP(base) * sz);
    base->ptr = ptr;
  }
  memmove(ARRAY_PTR(base) + (ARRAY_LEN(more) + len - 1) * sz, ARRAY_PTR(base), len * sz);
  memmove(ARRAY_PTR(base), ARRAY_PTR(more), ARRAY_LEN(more) * sz);
}

static OP_EMIT(opem_array_sr) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  if(bin->rhs->type->array_depth == bin->lhs->type->array_depth)
    return emit_array_shift(emit, ArrayConcatRight);
  const Instr pop = emit_add_instr(emit, RegMove);
  pop->m_val = -SZ_INT;
  if(isa(bin->rhs->type, emit->gwion->type[et_object]) > 0)
    emit_add_instr(emit, RegAddRef);
  (void)emit_add_instr(emit, ArrayAppendFront);
  return GW_OK;
}

static OP_EMIT(opem_array_sl) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  if(bin->lhs->type->array_depth == bin->rhs->type->array_depth)
    return emit_array_shift(emit, ArrayConcatLeft);
  const Instr pop = emit_add_instr(emit, RegMove);
  pop->m_val = -bin->rhs->type->size;
  if(isa(bin->rhs->type, emit->gwion->type[et_object]) > 0){
    const Instr ref = emit_add_instr(emit, RegAddRef);
    ref->m_val = -SZ_INT;
  }
  emit_add_instr(emit, ArrayAppend);
  return GW_OK;
}

// check me. use common ancestor maybe
static OP_CHECK(opck_array_cast) {
  const Exp_Cast* cast = (Exp_Cast*)data;
  const Type l = array_base(cast->exp->type);
  const Type r = array_base(exp_self(cast)->type);
  if(get_depth(cast->exp->type) == get_depth(exp_self(cast)->type) && isa(l->info->base_type, r->info->base_type) > 0)
    return l;
  return NULL;
}

static OP_CHECK(opck_array_slice) {
  const Exp e = (Exp)data;
  exp_setmeta(exp_self(e), 1);
  return e->d.exp_slice.base->type;
}

static inline m_bool bounds(const M_Vector v, const m_int i) {
  CHECK_BB(i);
  return (m_uint)i < ARRAY_LEN(v) ? GW_OK : GW_ERROR;
}

static INSTR(ArraySlice) {
  shred->reg -= SZ_INT *2;
  const M_Object array = *(M_Object*)REG(-SZ_INT);
  const M_Vector in = ARRAY(array);
  const m_int start = *(m_uint*)REG(0);
  m_int end   = *(m_uint*)REG(SZ_INT);
  if(end < 0)
    end = ARRAY_LEN(in) + end;
  const m_int op    = start < end ? 1 : -1;
  const m_uint sz    = op > 0 ? end - start : start - end;
  if(bounds(in, start) < 0 || bounds(in, end) < 0) {
    handle(shred, "OutOfBoundsArraySliceException");
    return;
  }
  const M_Object out = new_array(shred->info->mp, array->type_ref, sz);
  for(m_int i = start, j = 0; i != end; i += op, ++j) {
    m_bit buf[ARRAY_SIZE(in)];
    m_vector_get(in, i, &buf);
    m_vector_set(ARRAY(out), j, buf);
  }
  *(M_Object*)REG(-SZ_INT) = out;
}

static OP_EMIT(opem_array_slice) {
  emit_add_instr(emit, ArraySlice);
  emit_gc(emit, -SZ_INT);
  return GW_OK;
}

static FREEARG(freearg_array) {
  ArrayInfo* info = (ArrayInfo*)instr->m_val;
  vector_release(&info->type);
  mp_free(((Gwion)gwion)->mp, ArrayInfo, info);
}

ANN Type check_array_access(const Env env, const Array_Sub array);

static OP_CHECK(opck_array) {
  const Array_Sub array = (Array_Sub)data;
  const Type t_int = env->gwion->type[et_int];
  Exp e = array->exp;
  do CHECK_BN(check_implicit(env, e, t_int));
  while((e = e->next));
  const Type t = array->type->array_depth ? array->type : typedef_base(array->type);
  if(t->array_depth >= array->depth)
    return array_type(env, array_base(t), t->array_depth - array->depth);
  const Exp curr = take_exp(array->exp, t->array_depth);
  struct Array_Sub_ next = { curr->next, array_base(t), array->depth - t->array_depth };
  return check_array_access(env, &next) ?: env->gwion->type[et_error];
}

ANN static void array_loop(const Emitter emit, const m_uint depth) {
  const Instr pre_pop = emit_add_instr(emit, RegMove);
  pre_pop->m_val = -depth * SZ_INT;
  for(m_uint i = 0; i < depth - 1; ++i) {
    const Instr access = emit_add_instr(emit, ArrayAccess);
    access->m_val = i * SZ_INT;
    access->m_val2 = !i ? SZ_INT : 0;
    const Instr get = emit_add_instr(emit, ArrayGet);
    get->m_val = i * SZ_INT;
    get->m_val2 = -SZ_INT;
  }
  const Instr post_pop = emit_add_instr(emit, RegMove);
  post_pop->m_val = -SZ_INT;
  const Instr access = emit_add_instr(emit, ArrayAccess);
  access->m_val = depth * SZ_INT;
}

ANN static void array_finish(const Emitter emit, const m_uint depth,
		const m_uint size, const m_bool is_var) {
  const Instr get = emit_add_instr(emit, is_var ? ArrayAddr : ArrayGet);
  get->m_val = depth * SZ_INT;
  emit_add_instr(emit, ArrayValid);
  const Instr push = emit_add_instr(emit, RegMove);
  push->m_val = is_var ? SZ_INT : size;
}

ANN static inline m_bool array_do(const  Emitter emit, const Array_Sub array, const m_bool is_var) {
  emit_gc(emit, -SZ_INT);
  CHECK_BB(emit_exp(emit, array->exp));
  array_loop(emit, array->depth);
  array_finish(emit, array->depth, array->type->size, is_var);
  return GW_OK;
}

ANN static inline Exp emit_n_exp(const Emitter emit,  struct ArrayAccessInfo *const info) {
  const Exp e = take_exp(info->array.exp, info->array.depth);
  const Exp next = e->next;
  e->next = NULL;
  struct Array_Sub_ partial = { info->array.exp, info->array.type, info->array.depth };
  const m_bool ret = array_do(emit, &partial, 0);
  e->next = next;
  return ret > 0 ? next : NULL;
}

static OP_EMIT(opem_array_access) {
  struct ArrayAccessInfo *const info = (struct ArrayAccessInfo*)data;
  if(info->array.type->array_depth >= info->array.depth) {
    struct Array_Sub_ next = { .exp=info->array.exp, .type=info->type, .depth=info->array.depth };
    return array_do(emit, &next, info->is_var);
  }
  struct Array_Sub_ partial = { info->array.exp, info->array.type, info->array.type->array_depth };
  struct Array_Sub_ next = { info->array.exp, array_base(info->array.type), info->array.depth - info->array.type->array_depth };
  info->array = partial;
  const Exp exp = emit_n_exp(emit, info);
  next.exp = exp;
  info->array = next;
  return exp ? emit_array_access(emit, info) : GW_ERROR;
}

static m_bit map_byte[BYTECODE_SZ*4];
static const struct VM_Code_ map_run_code = {
  .name        = "map_run_code",
  .bytecode = map_byte
};

static m_bit compactmap_byte[BYTECODE_SZ*4];
static const struct VM_Code_ compactmap_run_code = {
  .name        = "compactmap_run_code",
  .bytecode = compactmap_byte
};

static m_bit filter_byte[BYTECODE_SZ*4];
static const struct VM_Code_ filter_run_code = {
  .name        = "filter_run_code",
  .bytecode = filter_byte
};

static m_bit count_byte[BYTECODE_SZ*4];
static const struct VM_Code_ count_run_code = {
  .name        = "count_run_code",
  .bytecode = count_byte
};

static m_bit foldl_byte[BYTECODE_SZ*4];
static const struct VM_Code_ foldl_run_code = {
  .name        = "foldl_run_code",
  .bytecode = foldl_byte
};

static m_bit foldr_byte[BYTECODE_SZ*4];
static const struct VM_Code_ foldr_run_code = {
  .name        = "foldr_run_code",
  .bytecode = foldr_byte
};

typedef struct FunctionalFrame {
  m_uint pc;
  VM_Code code;
  m_uint offset;
  m_uint index;
} FunctionalFrame;

ANN static inline void _init(const VM_Shred shred,
    const struct VM_Code_ *code, const m_uint offset, const m_uint start) {
  FunctionalFrame *frame = &*(FunctionalFrame*)MEM(SZ_INT*2 + start);
  frame->pc = shred->pc;
  frame->code = shred->code;
  frame->offset = offset;
  frame->index = 0;
  *(m_uint*)REG(SZ_INT) = offset;
  shred->code = (VM_Code)code;
  shred->pc = 0;
  shredule(shred->tick->shreduler, shred, 0);
}

ANN static inline void _next(const VM_Shred shred, const m_uint offset) {
  shred->pc = 0;
  *(m_uint*)REG(0) = offset;
  POP_REG(shred, SZ_INT);
}

ANN static inline void _return(const VM_Shred shred, const FunctionalFrame* frame) {
  shred->pc = frame->pc;
  shred->code = frame->code;
}

ANN static inline void _finish(const VM_Shred shred, const FunctionalFrame* frame) {
  POP_MEM(shred, frame->offset);
  shredule(shred->tick->shreduler, shred, 0);
}

#define MAP_CODE_OFFSET (sizeof(FunctionalFrame) + SZ_INT*4)
static INSTR(map_run_ini) {
  const m_uint offset = *(m_uint*)REG(SZ_INT);
  if(offset)
    PUSH_MEM(shred, offset);
  PUSH_REG(shred, SZ_INT);
  const M_Object self = *(M_Object*)MEM(0);
  const M_Vector array = ARRAY(self);
  FunctionalFrame *frame = &*(FunctionalFrame*)MEM(SZ_INT*3);
  shred->pc++;
  shred->mem += MAP_CODE_OFFSET + SZ_INT; // work in a safe memory space
  *(m_uint*)(shred->mem-SZ_INT) = 0;
  *(m_uint*)(shred->mem-SZ_INT*2) = 0;
  m_vector_get(array, frame->index, &*(m_bit**)(shred->mem + SZ_INT*6));
}

static INSTR(map_run_end) {
  shred->mem -= MAP_CODE_OFFSET + SZ_INT;
  const M_Object ret_obj = *(M_Object*)MEM(SZ_INT*2);
  const M_Vector array = ARRAY(*(M_Object*)MEM(0));
  POP_REG(shred, ARRAY_SIZE(array));
  FunctionalFrame *const frame = &*(FunctionalFrame*)MEM(SZ_INT*3);
  m_vector_set(ARRAY(ret_obj), frame->index, shred->reg);
  if(++frame->index == ARRAY_LEN(array)) {
    _return(shred, frame);
    *(M_Object*)(REG(-SZ_INT)) = ret_obj;
  } else
    _next(shred, frame->offset);
  _finish(shred, frame);
}

static INSTR(compactmap_run_end) {
  shred->mem -= MAP_CODE_OFFSET + SZ_INT;
  const M_Object self = *(M_Object*)MEM(0);
  const M_Vector self_array = ARRAY(self);
  const M_Object ret_obj = *(M_Object*)MEM(SZ_INT*2);
  const M_Vector ret_array = ARRAY(ret_obj);
  POP_REG(shred, ARRAY_SIZE(ret_array));
  FunctionalFrame *const frame = &*(FunctionalFrame*)MEM(SZ_INT*3);
  const m_uint size = m_vector_size(self_array);
  const M_Object obj = *(M_Object*)REG(0);
  if(*(m_uint*)obj->data)
    m_vector_add(ret_array, &*(m_bit*)(obj->data + SZ_INT));
  if(++frame->index == size) {
    _return(shred, frame);
    *(M_Object*)(REG(-SZ_INT)) = ret_obj;
  } else
    _next(shred, frame->offset);
  _finish(shred, frame);
}

static INSTR(filter_run_end) {
  shred->mem -= MAP_CODE_OFFSET + SZ_INT;
  POP_REG(shred, SZ_INT);
  const M_Object self = *(M_Object*)MEM(0);
  const M_Object ret_obj = *(M_Object*)MEM(SZ_INT*2);
  const M_Vector array = ARRAY(ret_obj);
  FunctionalFrame *const frame = &*(FunctionalFrame*)MEM(SZ_INT*3);
  if(*(m_uint*)(shred->reg))
    m_vector_add(array, ARRAY_PTR(ARRAY(self)) + frame->index * ARRAY_SIZE(array));
  if(++frame->index == ARRAY_LEN(ARRAY(self))) {
    _return(shred, frame);
    *(M_Object*)(REG(-SZ_INT)) = ret_obj;
  } else
    _next(shred, frame->offset);
  _finish(shred, frame);
}

static INSTR(count_run_end) {
  shred->mem -= MAP_CODE_OFFSET + SZ_INT;
  const M_Object self= *(M_Object*)MEM(0);
  POP_REG(shred, SZ_INT);
  FunctionalFrame *const frame = &*(FunctionalFrame*)MEM(SZ_INT*3);
  if(*(m_uint*)(shred->reg))
    (*(m_uint*)MEM(SZ_INT*2))++;
  if(++frame->index == ARRAY_LEN(ARRAY(self))) {
    _return(shred, frame);
    *(m_uint*)(REG(-SZ_INT)) = *(m_uint*)MEM(SZ_INT*2);
  } else
    _next(shred, frame->offset);
  _finish(shred, frame);
}

static MFUN(vm_vector_map) {
  const m_uint offset = *(m_uint*)REG(SZ_INT*3);
  const M_Object ret = new_array(shred->info->mp, o->type_ref, ARRAY_LEN(ARRAY(o)));
  vector_add(&shred->gc, (m_uint)ret);
  if(ARRAY_LEN(ARRAY(o))) {
    _init(shred, &map_run_code, offset, SZ_INT);
    *(M_Object*)MEM(SZ_INT*2) = ret;
  } else
    *(M_Object*)RETURN = ret;
}

static MFUN(vm_vector_compactmap) {
  const VM_Code code = *(VM_Code*)REG(SZ_INT*2);
  const m_uint offset = *(m_uint*)REG(SZ_INT*3);
  const M_Object ret = new_array(shred->info->mp,
    code->ret_type, 0);
  vector_add(&shred->gc, (m_uint)ret);
  if(ARRAY_LEN(ARRAY(o))) {
    _init(shred, &compactmap_run_code, offset, SZ_INT);
    *(M_Object*)MEM(SZ_INT*2) = ret;
  } else
    *(M_Object*)RETURN = ret;
}

static MFUN(vm_vector_filter) {
  const m_uint offset = *(m_uint*)REG(SZ_INT*3);
  const M_Object ret = new_array(shred->info->mp, o->type_ref, 0);
  vector_add(&shred->gc, (m_uint)ret);
  if(ARRAY_LEN(ARRAY(o))) {
    _init(shred, &filter_run_code, offset, SZ_INT);
    *(M_Object*)MEM(SZ_INT*2) = ret;
  } else
    *(M_Object*)RETURN = ret;
}

static MFUN(vm_vector_count) {
  const m_uint offset = *(m_uint*)REG(SZ_INT*3);
  if(ARRAY_LEN(ARRAY(o))) {
    _init(shred, &count_run_code, offset, SZ_INT);
    *(m_uint*)MEM(SZ_INT*2) = 0;
  } else
    *(m_uint*)RETURN = 0;
}

static INSTR(foldl_run_ini) {
  const m_uint offset = *(m_uint*)REG(SZ_INT);
  if(offset)
    PUSH_MEM(shred, offset);
  const M_Object self = *(M_Object*)MEM(0);
  *(m_uint*)(shred->reg + SZ_INT) = 0;
  PUSH_REG(shred, SZ_INT);
  shred->pc++;
  const FunctionalFrame *frame = &*(FunctionalFrame*)MEM(SZ_INT*3);
  shred->mem += MAP_CODE_OFFSET + SZ_INT; // work in a safe memory space
  *(m_uint*)(shred->mem-SZ_INT) = 0;
  *(m_uint*)(shred->mem-SZ_INT*2) = 0;
  m_vector_get(ARRAY(self), frame->index, &*(m_bit**)(shred->mem + SZ_INT*4));
}

static INSTR(foldr_run_ini) {
  const m_uint offset = *(m_uint*)REG(SZ_INT);
  if(offset)
    PUSH_MEM(shred, offset);
  const M_Object self = *(M_Object*)MEM(0);
  *(m_uint*)(shred->reg + SZ_INT) = 0;
  PUSH_REG(shred, SZ_INT);
  shred->pc++;
  const FunctionalFrame *frame = &*(FunctionalFrame*)MEM(SZ_INT*3);
  shred->mem += MAP_CODE_OFFSET + SZ_INT; // work in a safe memory space
  *(m_uint*)(shred->mem-SZ_INT) = 0;
  *(m_uint*)(shred->mem-SZ_INT*2) = 0;
  const M_Vector array = ARRAY(self);
  m_vector_get(array, ARRAY_LEN(array) - frame->index - 1, &*(m_bit**)(shred->mem + SZ_INT*4));
}

static INSTR(fold_run_end) {
  shred->mem -= MAP_CODE_OFFSET + SZ_INT;
  FunctionalFrame *const frame = &*(FunctionalFrame*)MEM(SZ_INT*3);
  const M_Object self = *(M_Object*)MEM(0);
  const VM_Code code = *(VM_Code*)MEM(SZ_INT);
  const m_uint sz = code->stack_depth - ARRAY_SIZE(ARRAY(self));
  const m_uint base_sz = code->stack_depth - sz;
  POP_REG(shred, base_sz);
  if(++frame->index == ARRAY_LEN(ARRAY(self))) {
    POP_REG(shred, SZ_INT - base_sz);
    shred->pc = frame->pc;
    shred->code = frame->code;
    memcpy(REG(-sz), REG(0), base_sz);
  } else {
    memcpy(shred->mem + MAP_CODE_OFFSET + SZ_INT*2 + SZ_INT*3 + sz, shred->reg, base_sz);
    _next(shred, frame->offset);
  }
  _finish(shred, frame);
}

static MFUN(vm_vector_foldl) {
  const m_bit *byte   = shred->code->bytecode + (shred->pc-1) * BYTECODE_SZ;
  const m_uint acc_sz = *(m_uint*)(byte + SZ_INT);
  const m_uint offset = *(m_uint*)REG(SZ_INT*3 + acc_sz);
  if(ARRAY_LEN(ARRAY(o))) {
    _init(shred, &foldl_run_code, offset, SZ_INT);
    memcpy(shred->mem + MAP_CODE_OFFSET + SZ_INT*5 + acc_sz, MEM(SZ_INT*2), acc_sz);
  } else
    memcpy((m_bit*)RETURN, MEM(SZ_INT*2), acc_sz);
}

static MFUN(vm_vector_foldr) {
  const m_bit *byte   = shred->code->bytecode + (shred->pc-1) * BYTECODE_SZ;
  const m_uint acc_sz = *(m_uint*)(byte + SZ_INT);
  const m_uint offset = *(m_uint*)REG(SZ_INT*3 + acc_sz);
  if(ARRAY_LEN(ARRAY(o))) {
    _init(shred, &foldr_run_code, offset, SZ_INT);
    memcpy(shred->mem + MAP_CODE_OFFSET + SZ_INT*5 + acc_sz, MEM(SZ_INT*2), acc_sz);
  } else
    memcpy((m_bit*)RETURN, MEM(SZ_INT*2), acc_sz);
}

ANN /*static */Symbol array_sym(const Env env, const Type src, const m_uint depth);
#include "template.h"
static OP_CHECK(opck_array_scan) {
  struct TemplateScan *ts = (struct TemplateScan*)data;
  const Type t_array = env->gwion->type[et_array];
  const Class_Def c = t_array->info->cdef;
  const Type base = ts->t != t_array ?
    ts->t : known_type(env, ts->td->types->td);
  const Symbol sym = array_sym(env, array_base(base), base->array_depth + 1);
  const Type type = nspc_lookup_type1(base->info->value->from->owner, sym);
  if(type)
    return type;
  const Class_Def cdef = cpy_class_def(env->gwion->mp, c);
  cdef->base.ext = type2td(env->gwion, t_array, (loc_t){});
  cdef->base.xid = sym;
  cdef->base.tmpl->base = 1; // could store depth here?
  cdef->base.tmpl->call = new_type_list(env->gwion->mp, type2td(env->gwion, base, (loc_t){}), NULL);
  const Context ctx = env->context;
  env->context = base->info->value->from->ctx;
  const m_uint scope = env_push(env, base->info->value->from->owner_class, base->info->value->from->owner);
  (void)scan0_class_def(env, cdef);
  const Type t = cdef->base.type;
  (void)traverse_cdef(env, t);
  env_pop(env, scope);
  env->context = ctx;
  if(GET_FLAG(base, abstract))
    SET_FLAG(t, abstract);
  else
    UNSET_FLAG(t, abstract);
  set_tflag(t, tflag_emit);
  t->array_depth = base->array_depth + 1;
  t->info->base_type = array_base(base);
  set_tflag(t, tflag_cdef | tflag_tmpl);
  void* rem = isa(base, env->gwion->type[et_compound]) > 0 ?
    !tflag(base, tflag_struct) ? vm_vector_rem_obj : vm_vector_rem_struct : vm_vector_rem;
  builtin_func(env->gwion->mp, (Func)vector_at(&t->nspc->vtable, 0), rem);
  void* insert = isa(base, env->gwion->type[et_compound]) > 0 ?
    !tflag(base, tflag_struct) ? vm_vector_insert_obj : vm_vector_insert_struct : vm_vector_insert;
  builtin_func(env->gwion->mp, (Func)vector_at(&t->nspc->vtable, 1), insert);
  builtin_func(env->gwion->mp, (Func)vector_at(&t->nspc->vtable, 2), vm_vector_size);
  builtin_func(env->gwion->mp, (Func)vector_at(&t->nspc->vtable, 3), vm_vector_depth);
  builtin_func(env->gwion->mp, (Func)vector_at(&t->nspc->vtable, 4), vm_vector_cap);
  builtin_func(env->gwion->mp, (Func)vector_at(&t->nspc->vtable, 5), vm_vector_random);
  builtin_func(env->gwion->mp, (Func)vector_at(&t->nspc->vtable, 6), vm_vector_map);
  builtin_func(env->gwion->mp, (Func)vector_at(&t->nspc->vtable, 7), vm_vector_compactmap);
  builtin_func(env->gwion->mp, (Func)vector_at(&t->nspc->vtable, 8), vm_vector_filter);
  builtin_func(env->gwion->mp, (Func)vector_at(&t->nspc->vtable, 9), vm_vector_count);
  builtin_func(env->gwion->mp, (Func)vector_at(&t->nspc->vtable, 10), vm_vector_foldl);
  builtin_func(env->gwion->mp, (Func)vector_at(&t->nspc->vtable, 11), vm_vector_foldr);
  if(isa(base, env->gwion->type[et_compound]) > 0) {
    t->nspc->dtor = new_vmcode(env->gwion->mp, NULL, SZ_INT, 1, "array component dtor");
    set_tflag(t, tflag_dtor);
    t->nspc->dtor->native_func = (m_uint) (!tflag(base, tflag_struct) ?
        array_dtor_obj : array_dtor_struct);
  }
  return t;
}

static OP_CHECK(opck_array_implicit) {
  const struct Implicit* imp = (struct Implicit*)data;
  if(imp->t->array_depth != imp->e->type->array_depth)
    return env->gwion->type[et_error];
  if(isa(array_base(imp->e->type), array_base(imp->t)) < 0)
    return env->gwion->type[et_error];
  return imp->t;
}

ANN static void prepare_run(m_bit *const byte, const f_instr ini, const f_instr end) {
  *(unsigned*)byte = eOP_MAX;
  *(f_instr*)(byte + SZ_INT*2) = ini;
  *(unsigned*)(byte+ BYTECODE_SZ) = eSetCode;
  *(m_uint*)(byte + BYTECODE_SZ + SZ_INT*2) = 3;
  *(unsigned*)(byte+ BYTECODE_SZ*2) = eOverflow;
  *(unsigned*)(byte+ BYTECODE_SZ*3) = eOP_MAX;
  *(f_instr*)(byte + BYTECODE_SZ*3 + SZ_INT*2) = end;
}

ANN static void prepare_map_run(m_bit *const byte, const f_instr end) {
  prepare_run(byte, map_run_ini, end);
}

ANN static void prepare_fold_run(m_bit *const byte, const f_instr ini) {
  prepare_run(byte, ini, fold_run_end);
}

GWION_IMPORT(array) {
  prepare_map_run(map_byte, map_run_end);
  prepare_map_run(compactmap_byte, compactmap_run_end);
  prepare_map_run(filter_byte, filter_run_end);
  prepare_map_run(count_byte, count_run_end);
  prepare_fold_run(foldl_byte, foldl_run_ini);
  prepare_fold_run(foldr_byte, foldr_run_ini);
  const Type t_array  = gwi_class_ini(gwi, "Array:[T]", "Object");
  gwi->gwion->type[et_array] = t_array;
  gwi_class_xtor(gwi, NULL, array_dtor);
  GWI_BB(gwi_item_ini(gwi, "@internal", "@array"))
  GWI_BB(gwi_item_end(gwi, 0, num, 0))
  GWI_BB(gwi_item_ini(gwi, "@internal", "@ctor_data"))
  GWI_BB(gwi_item_end(gwi, 0, num, 0))

  GWI_BB(gwi_fptr_ini(gwi, "A", "map_t:[A]"))
  GWI_BB(gwi_func_arg(gwi, "T", "elem"))
  GWI_BB(gwi_fptr_end(gwi, ae_flag_global))

  GWI_BB(gwi_fptr_ini(gwi, "Option:[A]", "compactmap_t:[A]"))
  GWI_BB(gwi_func_arg(gwi, "T", "elem"))
  GWI_BB(gwi_fptr_end(gwi, ae_flag_global))

  GWI_BB(gwi_fptr_ini(gwi, "A", "fold_t:[A]"))
  GWI_BB(gwi_func_arg(gwi, "T", "elem"))
  GWI_BB(gwi_func_arg(gwi, "A", "acc"))
  GWI_BB(gwi_fptr_end(gwi, ae_flag_global))

  GWI_BB(gwi_fptr_ini(gwi, "bool", "filter_t"))
  GWI_BB(gwi_func_arg(gwi, "T", "elem"))
  GWI_BB(gwi_fptr_end(gwi, ae_flag_global))

  // put functions using T first
  GWI_BB(gwi_func_ini(gwi, "bool", "remove"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_end(gwi, vm_vector_rem, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "bool", "insert"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_arg(gwi, "T", "data"))
  GWI_BB(gwi_func_end(gwi, vm_vector_insert, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "size"))
  GWI_BB(gwi_func_end(gwi, vm_vector_size, ae_flag_none))
  GWI_BB(gwi_func_ini(gwi, "int", "depth"))
  GWI_BB(gwi_func_end(gwi, vm_vector_depth, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "cap"))
  GWI_BB(gwi_func_end(gwi, vm_vector_cap, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "T", "random"))
  GWI_BB(gwi_func_end(gwi, vm_vector_random, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "A[]", "map:[A]"))
  GWI_BB(gwi_func_arg(gwi, "map_t:[A]", "data"))
  GWI_BB(gwi_func_end(gwi, vm_vector_map, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "A[]", "compactMap:[A]"))
  GWI_BB(gwi_func_arg(gwi, "compactmap_t:[A]", "data"))
  GWI_BB(gwi_func_end(gwi, vm_vector_compactmap, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "T[]", "filter"))
  GWI_BB(gwi_func_arg(gwi, "filter_t", "data"))
  GWI_BB(gwi_func_end(gwi, vm_vector_filter, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "count"))
  GWI_BB(gwi_func_arg(gwi, "filter_t", "data"))
  GWI_BB(gwi_func_end(gwi, vm_vector_count, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "A", "foldl:[A]"))
  GWI_BB(gwi_func_arg(gwi, "fold_t", "data"))
  GWI_BB(gwi_func_arg(gwi, "A", "initial"))
  GWI_BB(gwi_func_end(gwi, vm_vector_foldl, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "A", "foldr:[A]"))
  GWI_BB(gwi_func_arg(gwi, "fold_t", "data"))
  GWI_BB(gwi_func_arg(gwi, "A", "initial"))
  GWI_BB(gwi_func_end(gwi, vm_vector_foldr, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, "Array", "Array", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_array_at))
  GWI_BB(gwi_oper_end(gwi, "=>", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_array_implicit))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  GWI_BB(gwi_oper_ini(gwi, "Array", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_array_sl))
  GWI_BB(gwi_oper_emi(gwi, opem_array_sl))
  GWI_BB(gwi_oper_end(gwi, "<<", NULL))
  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "Array", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_array_sr))
  GWI_BB(gwi_oper_emi(gwi, opem_array_sr))
  GWI_BB(gwi_oper_end(gwi, ">>", NULL))
  GWI_BB(gwi_oper_ini(gwi, "Array", "Array", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_array_cast))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_ini(gwi, "int", "Array", "int"))
  GWI_BB(gwi_oper_add(gwi, opck_array_slice))
  GWI_BB(gwi_oper_emi(gwi, opem_array_slice))
  GWI_BB(gwi_oper_end(gwi, "@slice", NULL))
  GWI_BB(gwi_oper_ini(gwi, "int", "Array", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_array))
  GWI_BB(gwi_oper_emi(gwi, opem_array_access))
  GWI_BB(gwi_oper_end(gwi, "@array", NULL))
  GWI_BB(gwi_oper_ini(gwi, "Array", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_array_scan))
  GWI_BB(gwi_oper_end(gwi, "@scan", NULL))
  gwi_register_freearg(gwi, ArrayAlloc, freearg_array);
  return GW_OK;
}

INSTR(ArrayStruct) {
  const M_Object ref = *(M_Object*)(REG(-SZ_INT * 5));
  const m_int idx = (*(m_int*)((shred->reg -SZ_INT * 3)))++;
  *(m_bit**)(shred->reg) = m_vector_addr(ARRAY(ref), idx);
  shred->reg += SZ_INT; // regpush
}

INSTR(ArrayBottom) {
  *(M_Object*)(*(m_uint**)REG(-SZ_INT * 4))[(*(m_int*)REG(-SZ_INT * 3))++] = *(M_Object*)REG(-SZ_INT);
}

INSTR(ArrayPost) {
  xfree(*(m_uint**)REG(0));
  const M_Object o = *(M_Object*)(REG(-SZ_INT));
  *(m_uint*)(o->data + SZ_INT) = 0;
}

INSTR(ArrayInit) {// for litteral array
  const Type t = (Type)instr->m_val;
  const m_uint sz = *(m_uint*)REG(0);
  const m_uint off = instr->m_val2 * sz;
  POP_REG(shred, off - SZ_INT);
  const M_Object obj = new_array(shred->info->mp, t, sz);
  memcpy(ARRAY(obj)->ptr + ARRAY_OFFSET, REG(-SZ_INT), off);
  *(M_Object*)REG(-SZ_INT) = obj;
}

#define TOP -1

ANN static inline M_Object do_alloc_array_object(MemPool p, const ArrayInfo* info, const m_int cap) {
  struct Vector_ v = info->type;
  const Type t = (Type)vector_at(&v, (vtype)(-info->depth - 1));
  return new_array(p, t, (m_uint)cap);
}

ANN static inline M_Object do_alloc_array_init(ArrayInfo* info, const m_uint cap,
    const M_Object base) {
  for(m_uint i = 0; i < cap; ++i)
    info->data[(*info->d.idx)++] = (M_Object)m_vector_addr(ARRAY(base), i);
  return base;
}

ANN static M_Object do_alloc_array(const VM_Shred shred, ArrayInfo* info);
ANN static M_Object do_alloc_array_loop(const VM_Shred shred, ArrayInfo* info,
    const m_uint cap, const M_Object base) {
  for(m_uint i = 0; i < cap; ++i) {
    struct ArrayInfo_ aai = { info->depth + 1, info->type,
      info->base, info->data, { info->d.idx } , info->is_obj };
    const M_Object next = do_alloc_array(shred, &aai);
    if(!next) {
      _release(base, shred);
      return NULL;
    }
    m_vector_set(ARRAY(base), i, &next);
  }
  return base;
}

ANN static M_Object do_alloc_array(const VM_Shred shred, ArrayInfo* info) {
  const m_int cap = *(m_int*)REG(info->depth * SZ_INT);
  if(cap < 0) {
    gw_err("[gwion](VM): NegativeArraySize: while allocating arrays...\n");
    return NULL;
  }
  const M_Object base = do_alloc_array_object(shred->info->mp, info, cap);
  return info->depth < TOP ? do_alloc_array_loop(shred, info, (m_uint)cap, base) :
    info->data ? do_alloc_array_init(info, (m_uint)cap, base) : base;
}

ANN static M_Object* init_array(const VM_Shred shred, const ArrayInfo* info, m_uint* num_obj) {
  m_int curr = -info->depth;
  while(curr <= TOP) {
    *num_obj *= *(m_uint*)REG(SZ_INT * curr);
    ++curr;
  }
  return *num_obj > 0 ? (M_Object*)xcalloc(*num_obj, info->base->size) : NULL;
}

INSTR(ArrayAlloc) {
  const ArrayInfo* info = (ArrayInfo*)instr->m_val;
  m_uint num_obj = 1;
  m_int idx = 0;
  struct ArrayInfo_ aai = { -info->depth, info->type, .base=info->base,
         NULL, { &idx }, info->is_obj};
  if(info->is_obj)
    aai.data = init_array(shred, info, &num_obj);
  const M_Object ref = do_alloc_array(shred, &aai);
  if(!ref) {
    gw_err("[Gwion](VM): (note: in shred[id=%" UINT_F ":%s])\n", shred->tick->xid, shred->code->name);
    vm_shred_exit(shred);
    return; // TODO make exception vararg
  }
  *(void**)(ref->data + SZ_INT) = aai.data;
  vector_add(&shred->gc, (m_uint)ref);
  if(!info->is_obj) {
    POP_REG(shred, SZ_INT * (info->depth - 1));
    *(M_Object*)REG(-SZ_INT) = ref;
  } else {
    POP_REG(shred, SZ_INT * (info->depth - 4));
    *(M_Object*)REG(-SZ_INT*4) = ref;
    *(M_Object**)REG(-SZ_INT*3) = aai.data;
    *(m_uint*) REG(-SZ_INT*2) = 0;
    *(m_uint*) REG(-SZ_INT) = num_obj;
  }
}

