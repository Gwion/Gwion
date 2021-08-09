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
#include "vararg.h"

#undef insert_symbol
#define insert_symbol(a) insert_symbol(emit->gwion->st, (a))

#undef ERR_B
#define ERR_B(a, b, ...)                                                       \
  {                                                                            \
    env_err(emit->env, (a), (b), ##__VA_ARGS__);                               \
    return GW_ERROR;                                                           \
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
  bool   skip;
  bool   is_compound;
} Local;

static inline void emit_pop(const Emitter emit, const m_uint scope) {
  env_pop(emit->env, scope);
}
static inline m_uint emit_push(const Emitter emit, const Type type,
                               const Nspc nspc) {
  return env_push(emit->env, type, nspc);
}

static inline m_uint emit_push_global(const Emitter emit) {
  return env_push_global(emit->env);
}

static inline void emit_debug(const Emitter emit, const Value v) {
  if (!emit->info->debug) return;
  const Instr instr = emit_add_instr(emit, DebugValue);
  instr->m_val      = (m_uint)v;
}

ANEW static Frame *new_frame(MemPool p) {
  Frame *frame = mp_calloc(p, Frame);
  vector_init(&frame->stack);
  vector_add(&frame->stack, (vtype)NULL);
  vector_init(&frame->defer);
  vector_add(&frame->defer, (vtype)NULL);
  return frame;
}

ANN static void free_frame(MemPool p, Frame *a) {
  LOOP_OPTIM
  for (vtype i = vector_size(&a->stack) + 1; --i;)
    if (vector_at(&a->stack, i - 1))
      mp_free(p, Local, (Local *)vector_at(&a->stack, i - 1));
  vector_release(&a->stack);
  vector_release(&a->defer);
  if (a->handlers.ptr) map_release(&a->handlers);
  mp_free(p, Frame, a);
}

ANN static Local *new_local(MemPool p, const Type type) {
  Local *local = mp_calloc(p, Local);
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

static const f_instr allocmember[] = {RegPushImm, RegPushImm2, RegPushImm3,
                                      AllocMember4};
static const f_instr allocword[]   = {AllocWord, AllocWord2, AllocWord3,
                                    RegPushMem4};

#define regmove(name, op)                                                      \
  ANN static inline Instr reg##name(const Emitter emit, const m_uint sz) {     \
    const Instr       instr = emit_add_instr(emit, RegMove);                   \
    instr->m_val            = op sz;                                           \
    return instr;                                                              \
  }
regmove(pop, -) regmove(push, );

#define regxxx(name, instr)                                                    \
  ANN static inline Instr reg##name(const Emitter emit, const m_uint sz) {     \
    const Instr instr = emit_add_instr(emit, Reg##instr);                      \
    instr->m_val      = sz;                                                    \
    return instr;                                                              \
  }
regxxx(pushi, PushImm) regxxx(seti, SetImm);

ANN static m_bool      emit_class_def(const Emitter, const Class_Def);
ANN /*static */ m_bool emit_cdef(const Emitter, const Type);

ANN static inline m_bool ensure_emit(const Emitter emit, const Type t) {
  if (tflag(t, tflag_emit) || !(tflag(t, tflag_cdef) || tflag(t, tflag_udef)))
    return GW_OK; // clean callers
  struct EnvSet es = {.env   = emit->env,
                      .data  = emit,
                      .func  = (_exp_func)emit_cdef,
                      .scope = emit->env->scope->depth,
                      .flag  = tflag_emit};
  return envset_run(&es, t);
}

ANN static inline m_uint emit_code_size(const Emitter emit) {
  return vector_size(&emit->code->instr);
}

ANN static void emit_struct_ctor(const Emitter emit, const Type type,
                                 const m_uint offset) {
  emit->code->frame->curr_offset += SZ_INT;
  const Instr instr = emit_add_instr(emit, RegPushMem4);
  instr->m_val      = offset;
  const Instr tomem = emit_add_instr(emit, Reg2Mem);
  tomem->m_val      = emit->code->frame->curr_offset;
  tomem->m_val2     = -SZ_INT;
  regpushi(emit, (m_uint)type);
  const Instr tomem2       = emit_add_instr(emit, Reg2Mem);
  tomem2->m_val            = emit->code->frame->curr_offset + SZ_INT;
  tomem2->m_val2           = -SZ_INT;
  const Instr set_code     = regseti(emit, (m_uint)type->nspc->dtor);
  set_code->m_val2         = SZ_INT;
  const m_uint code_offset = emit_code_offset(emit);
  const Instr  regset =
      regseti(emit, code_offset /*+ SZ_INT*/ /*+ sizeof(frame_t)*/);
  regset->m_val2 = SZ_INT * 2;
  regpush(emit, SZ_INT * 2);
  const Instr prelude = emit_add_instr(emit, SetCode);
  prelude->m_val      = -SZ_INT * 4;
  prelude->udata.one  = 2;
  const Instr next    = emit_add_instr(emit, Overflow);
  next->m_val2        = code_offset;
  emit->code->frame->curr_offset -= SZ_INT;
}

ANN static void struct_pop(const Emitter emit, const Type type,
                           const m_uint offset) {
  if (!type->info->tuple) return;
  if (type->nspc->dtor) emit_struct_ctor(emit, type, offset);
  for (m_uint i = 0; i < vector_size(&type->info->tuple->types); ++i) {
    const Type t = (Type)vector_at(&type->info->tuple->types, i);
    if (isa(t, emit->gwion->type[et_object]) > 0) {
      const Instr instr = emit_add_instr(emit, ObjectRelease);
      instr->m_val      = offset + vector_at(&type->info->tuple->offset, i);
    } else if (tflag(t, tflag_struct))
      struct_pop(emit, t, offset + vector_at(&type->info->tuple->offset, i));
  }
}

ANN static m_bool emit_stmt(const Emitter emit, const Stmt stmt,
                            const m_bool pop);

ANN static m_bool emit_defers(const Emitter emit) {
  if (!vector_size(&emit->code->frame->defer)) return GW_OK;
  Stmt stmt;
  while ((stmt = (Stmt)vector_pop(&emit->code->frame->defer)))
    CHECK_BB(emit_stmt(emit, stmt, 1));
  return GW_OK;
}

ANN static m_bool emit_defers2(const Emitter emit) {
  for (m_uint i = vector_size(&emit->code->frame->defer) + 1; --i;) {
    const Stmt stmt = (Stmt)vector_at(&emit->code->frame->defer, i - 1);
    if (!stmt) break;
    CHECK_BB(emit_stmt(emit, stmt, 1));
  }
  return GW_OK;
}

ANN static m_int _frame_pop(const Emitter emit) {
  Frame *frame = emit->code->frame;
  DECL_OB(const Local *, l, = (Local *)vector_pop(&frame->stack));
  frame->curr_offset -= l->type->size;
  if (l->skip) return _frame_pop(emit);
  if (!l->is_compound) return _frame_pop(emit);
  VMValue *vmval =
      (VMValue *)m_vector_addr(&emit->code->live_values, --frame->value_count);
  vmval->end = emit_code_size(emit);
  if (!tflag(l->type, tflag_struct)) return (m_int)l->offset;
  struct_pop(emit, l->type, l->offset);
  return _frame_pop(emit);
}

ANN static m_int frame_pop(const Emitter emit) {
  emit_defers(emit);
  return _frame_pop(emit);
}

ANN /*static */ m_bool emit_exp(const Emitter emit, Exp exp);
ANN static m_bool      emit_stmt(const Emitter emit, const Stmt stmt,
                                 const m_bool pop);
ANN static m_bool      emit_stmt_list(const Emitter emit, Stmt_List list);
ANN static m_bool      emit_exp_dot(const Emitter emit, const Exp_Dot *member);

ANEW static Code *new_code(const Emitter emit, const m_str name) {
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
  vector_release(&code->instr);
  vector_release(&code->stack_break);
  vector_release(&code->stack_cont);
  vector_release(&code->stack_return);
  if (code->live_values.ptr) m_vector_release(&code->live_values);
  free_frame(p, code->frame);
  free_mstr(p, code->name);
  mp_free(p, Code, code);
}

ANN static void emit_pop_scope(const Emitter emit) {
  m_int offset;
  while ((offset = frame_pop(emit)) > -1) {
    Instr instr  = emit_add_instr(emit, ObjectRelease);
    instr->m_val = (m_uint)offset;
  }
  vector_pop(&emit->info->pure);
  if (emit->info->debug) emit_add_instr(emit, DebugPop);
}

ANN static inline void emit_push_code(const Emitter emit, const m_str name) {
  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code(emit, name);
  if (emit->info->debug) emit_add_instr(emit, DebugLine);
}

ANN static inline void emit_pop_code(const Emitter emit) {
  emit->code = (Code *)vector_pop(&emit->stack);
}

ANN static inline void emit_push_scope(const Emitter emit) {
  frame_push(emit->code->frame);
  vector_add(&emit->info->pure, 0);
  if (emit->info->debug) emit_add_instr(emit, DebugPush);
}

ANN m_uint emit_code_offset(const Emitter emit) {
  return emit->code->frame->curr_offset;
}

ANN m_uint emit_local(const Emitter emit, const Type t) {
  Local *const l = frame_local(emit->gwion->mp, emit->code->frame, t);
  if (isa(t, emit->gwion->type[et_compound]) > 0) {
    l->is_compound = true;
    VMValue vmval  = {
        .t = t, .offset = l->offset, .start = emit_code_size(emit)};
    m_vector_add(&emit->code->live_values, &vmval);
    ++emit->code->frame->value_count;
  }
  return l->offset;
}

ANN m_uint emit_localn(const Emitter emit, const Type t) {
  Local *const l = frame_local(emit->gwion->mp, emit->code->frame, t);
  l->skip        = true;
  return l->offset;
}

ANN void emit_ext_ctor(const Emitter emit, const Type t);

ANN static inline void maybe_ctor(const Emitter emit, const Type t) {
  if (tflag(t, tflag_ctor)) emit_ext_ctor(emit, t);
}

ANN static void emit_pre_ctor(const Emitter emit, const Type type) {
  if (type->info->parent) {
    emit_pre_ctor(emit, type->info->parent);
    if (tflag(type, tflag_typedef) && type->info->parent->array_depth)
      emit_array_extend(emit, type, type->info->cdef->base.ext->array->exp);
  }
  maybe_ctor(emit, type);
}

ANN static void struct_expand(const Emitter emit, const Type t) {
  const Instr instr = emit_add_instr(emit, Reg2RegDeref);
  instr->m_val      = -SZ_INT;
  instr->m_val2     = t->size;
}

ANN static void emit_pre_constructor_array(const Emitter emit,
                                           const Type    type) {
  const m_uint start_index = emit_code_size(emit);
  const Instr  top         = emit_add_instr(emit, ArrayTop);
  top->m_val2              = (m_uint)type;
  if (tflag(type, tflag_struct)) {
    const Instr instr = emit_add_instr(emit, ArrayStruct);
    instr->m_val      = type->size;
  }
  emit_pre_ctor(emit, type);
  if (!tflag(type, tflag_struct))
    emit_add_instr(emit, ArrayBottom);
  else
    regpop(emit, SZ_INT);
  regpop(emit, SZ_INT);
  const Instr pc = emit_add_instr(emit, Goto);
  pc->m_val      = start_index;
  top->m_val     = emit_code_size(emit);
  regpop(emit, SZ_INT * 3);
  emit_add_instr(emit, ArrayPost);
}

ANN2(1)
static m_bool extend_indices(const Emitter emit, Exp e, const m_uint depth) {
  if (e) CHECK_BB(emit_exp(emit, e));
  m_uint count = 0;
  while (e) {
    ++count;
    e = e->next;
  }
  for (m_uint i = count; i < depth; ++i) regpushi(emit, 0);
  return GW_OK;
}

ANEW ANN static ArrayInfo *new_arrayinfo(const Emitter emit, const Type t) {
  const Type base = array_base(t);
  ArrayInfo *info = mp_calloc(emit->gwion->mp, ArrayInfo);
  vector_init(&info->type);
  info->depth = get_depth(t);
  for (long i = 1; i < info->depth; ++i)
    vector_add(&info->type, (vtype)array_type(emit->env, base, i));
  vector_add(&info->type, (vtype)t);
  info->base = base;
  return info;
}

ANN static inline void arrayinfo_ctor(const Emitter emit, ArrayInfo *info) {
  const Type base = info->base;
  if (isa(base, emit->gwion->type[et_compound]) > 0 &&
      !GET_FLAG(base, abstract)) {
    emit_pre_constructor_array(emit, base);
    info->is_obj = 1;
  }
}

ANN2(1, 2)
static ArrayInfo *emit_array_extend_inner(const Emitter emit, const Type t,
                                          const Exp e, const uint is_ref) {
  CHECK_BO(extend_indices(emit, e, get_depth(t)));
  ArrayInfo * info  = new_arrayinfo(emit, t);
  const Instr alloc = emit_add_instr(emit, ArrayAlloc);
  alloc->m_val      = (m_uint)info;
  if (!is_ref) arrayinfo_ctor(emit, info);
  return info;
}

ANN void emit_ext_ctor(const Emitter emit, const Type t) {
  const Instr cpy = emit_add_instr(emit, Reg2Reg);
  cpy->m_val2     = -SZ_INT;
  if (t->nspc->pre_ctor) {
    const Instr set_code = regseti(emit, (m_uint)t->nspc->pre_ctor);
    set_code->m_val2     = SZ_INT;
  } else {
    const Instr instr = emit_add_instr(emit, SetCtor);
    instr->m_val      = (m_uint)t;
  }
  const m_uint offset = emit_code_offset(emit);
  const Instr  regset =
      regseti(emit, offset /*+ SZ_INT */ /*+ sizeof(frame_t)*/);
  regset->m_val2 = SZ_INT * 2;
  regpush(emit, SZ_INT * 2);
  const Instr prelude = emit_add_instr(emit, SetCode);
  prelude->m_val      = -SZ_INT * 2;
  prelude->udata.one  = 2;
  emit_add_instr(emit, Reg2Mem);
  const Instr next = emit_add_instr(emit, Overflow);
  next->m_val2     = offset;
}

ANN m_bool emit_array_extend(const Emitter emit, const Type t, const Exp e) {
  CHECK_OB(emit_array_extend_inner(emit, t, e, 0));
  regpop(emit, SZ_INT);
  const Instr instr = emit_add_instr(emit, Reg2Reg);
  instr->m_val      = -SZ_INT;
  //  emit_add_instr(emit, RegAddRef);
  return GW_OK;
}

ANN static inline void emit_notpure(const Emitter emit) {
  ++VPTR(&emit->info->pure, VLEN(&emit->info->pure) - 1);
}

ANN2(1, 2)
m_bool emit_instantiate_object(const Emitter emit, const Type type,
                               const Array_Sub array, const m_bool is_ref) {
  emit_notpure(emit);
  if (type->array_depth) {
    DECL_OB(ArrayInfo *, info,
            = emit_array_extend_inner(emit, type, array ? array->exp : NULL,
                                      is_ref));
    return GW_OK;
  } else if (!is_ref) {
    const Instr instr = emit_add_instr(emit, ObjectInstantiate);
    instr->m_val2     = (m_uint)type;
    emit_pre_ctor(emit, type);
  }
  return GW_OK;
}

ANN2(1, 2)
m_bool emit_instantiate_decl(const Emitter emit, const Type type,
                             const Type_Decl *td, const Array_Sub array,
                             const m_bool is_ref) {
  Exp base = td->array ? td->array->exp : NULL, exp = base,
      next = array ? array->exp : NULL;
  const m_uint depth =
      (td->array ? td->array->depth : 0) + (array ? array->depth : 0);
  if (exp) {
    while (exp->next) exp = exp->next;
    exp->next = next;
  } else
    base = next;
  struct Array_Sub_ a   = {.exp = base, .depth = depth};
  const m_bool      ret = emit_instantiate_object(emit, type, &a, is_ref);
  if (td->array && td->array->exp) exp->next = NULL;
  return ret;
}

ANN static m_bool emit_symbol_builtin(const Emitter emit, const Symbol *data) {
  const Value v = prim_self(data)->value;
  if (vflag(v, vflag_direct)) {
    const m_uint size  = v->type->size;
    const Instr  instr = emit_dotstatic(emit, size, exp_getvar(prim_exp(data)));
    instr->m_val       = (m_uint)&v->d.ptr;
  } else {
    const m_uint size = v->type->size;
    const Instr instr = emit_regpushimm(emit, size, exp_getvar(prim_exp(data)));
    if (v->type->size == SZ_FLOAT)
      instr->f = v->d.fnum;
    else
      instr->m_val = v->d.num;
    instr->m_val2 = size;
  }
  return GW_OK;
}

ANN static m_bool _emit_symbol(const Emitter emit, const Symbol *data) {
  const Value v = prim_self(data)->value;
  if (is_class(emit->gwion, v->type)) {
    regpushi(emit, (m_uint)actual_type(emit->gwion, v->type));
    return GW_OK;
  }
  if (vflag(v, vflag_builtin) || vflag(v, vflag_direct))
    return emit_symbol_builtin(emit, data);
  if(is_func(emit->gwion, v->type) && !is_fptr(emit->gwion, v->type)) {
    const Func f = v->type->info->func;
    if(f->code)
      regpushi(emit, (m_uint)f->code);
    else {
      const Instr instr = emit_add_instr(emit, SetFunc);
      instr->m_val = (m_uint)f;
    }
    return GW_OK;
  }
  if (!strncmp(v->type->name, "Ref:[", 5)) {
    if (exp_getvar(exp_self(prim_self(data)))) {
      const Instr instr = emit_add_instr(emit, RegPushMem);
      instr->m_val      = v->from->offset;
      instr->m_val2     = SZ_INT;
    } else {
      const Instr instr = emit_add_instr(emit, RegPushMemDeref);
      instr->m_val      = v->from->offset;
      instr->m_val2     = v->type->size;
    }
    return GW_OK;
  }
  const m_uint size = v->type->size;
  const Instr  instr =
      !vflag(v, vflag_fglobal)
           ? emit_regpushmem(emit, size, exp_getvar(prim_exp(data)))
           : emit_regpushbase(emit, size, exp_getvar(prim_exp(data)));
  instr->m_val = v->from->offset;
  if (GET_FLAG(v, late) && !exp_getvar(prim_exp(data)) &&
      (isa(v->type, emit->gwion->type[et_object]) > 0 ||
       is_fptr(emit->gwion, v->type))) {
    const Instr instr = emit_add_instr(emit, GWOP_EXCEPT);
    instr->m_val      = -SZ_INT;
  }
  return GW_OK;
}

ANN static m_bool emit_symbol(const Emitter emit, const Exp_Primary *prim) {
  if (!prim->value) // assume it's an operator
    ERR_B(exp_self(prim)->pos, "missing value for operator");
  return _emit_symbol(emit, &prim->d.var);
}

ANN static VM_Code finalyze(const Emitter emit, const f_instr exec) {
  emit_add_instr(emit, exec);
  const VM_Code code = emit->info->emit_code(emit);
  free_code(emit->gwion->mp, emit->code);
  emit_pop_code(emit);
  return code;
}

ANN static inline m_uint exp_size(const Exp e) {
  if (exp_getvar(e)) return SZ_INT;
  const Type type = e->cast_to ?: e->type;
  return type->size;
}

ANN static inline m_uint exp_totalsize(Exp e) {
  m_uint size = 0;
  do size += exp_size(e);
  while ((e = e->next));
  return size;
}

ANN Instr emit_object_addref(const Emitter emit, const m_int size,
                             const bool emit_var) {
  const f_instr exec  = !emit_var ? RegAddRef : RegAddRefAddr;
  const Instr   instr = emit_add_instr(emit, exec);
  instr->m_val        = size;
  return instr;
}

ANN Instr emit_struct_addref(const Emitter emit, const Type t, const m_int size,
                             const bool emit_var) {
  const Instr instr =
      emit_add_instr(emit, !emit_var ? StructRegAddRef : StructRegAddRefAddr);
  instr->m_val  = (m_uint)t;
  instr->m_val2 = size;
  return instr;
}

ANN2(1)
static void emit_exp_addref1(const Emitter emit, const Exp exp, m_int size) {
  const Type t = exp->cast_to ?: exp->type;
  if (isa(t, emit->gwion->type[et_compound]) > 0)
    //    emit_object_addref(emit, size, exp_getvar(exp));
    //  else if(tflag(t, tflag_struct))
    emit_compound_addref(emit, exp->type, size, exp_getvar(exp));
}

ANN2(1)
static void emit_exp_addref(const Emitter emit, /* const */ Exp exp,
                            m_int size) {
  do {
    emit_exp_addref1(emit, exp, size);
    size += exp_size(exp);
  } while ((exp = exp->next));
}

ANN static m_bool emit_prim_array(const Emitter emit, const Array_Sub *data) {
  Exp e = (*data)->exp;
  CHECK_BB(emit_exp(emit, e));
  emit_exp_addref(emit, e, -exp_totalsize(e));
  m_uint count = 0;
  do ++count;
  while ((e = e->next));
  const Type type = (*data)->type;
  regseti(emit, count);
  const Instr instr = emit_add_instr(emit, ArrayInit);
  instr->m_val      = (m_uint)type;
  instr->m_val2     = array_base(type)->size;
  emit_gc(emit, -SZ_INT);
  emit_notpure(emit);
  return GW_OK;
}

ANN static inline m_bool emit_exp_pop_next(const Emitter emit, Exp e);

ANN static m_bool emit_range(const Emitter emit, Range *range) {
  if (range->start)
    CHECK_BB(emit_exp_pop_next(emit, range->start));
  else
    regpushi(emit, 0);
  if (range->end)
    CHECK_BB(emit_exp_pop_next(emit, range->end));
  else
    regpushi(emit, -1);
  return GW_OK;
}

ANN static m_bool emit_prim_range(const Emitter emit, Range **data) {
  Range *range = *data;
  CHECK_BB(emit_range(emit, range));
  const Exp    e   = range->start ?: range->end;
  const Symbol sym = insert_symbol("@range");
  assert(e);
  struct Op_Import opi = {.op   = sym,
                          .rhs  = e->type,
                          .pos  = e->pos,
                          .data = (uintptr_t)prim_exp(data)};
  CHECK_BB(op_emit(emit, &opi));
  emit_gc(emit, -SZ_INT);
  return GW_OK;
}

ANN m_bool emit_array_access(const Emitter                 emit,
                             struct ArrayAccessInfo *const info) {
  if (tflag(info->array.type, tflag_typedef)) {
    info->array.type = info->array.type->info->parent;
    return emit_array_access(emit, info);
  }
  // look mum no pos
  struct Op_Import opi = {.op   = insert_symbol("@array"),
                          .lhs  = info->array.exp->type,
                          .rhs  = info->array.type,
                          .data = (uintptr_t)info};
  if (!info->is_var &&
      (GET_FLAG(info->array.type, abstract) || type_ref(info->array.type))) {
    const Instr instr = emit_add_instr(emit, GWOP_EXCEPT);
    instr->m_val      = -SZ_INT;
  }
  return op_emit(emit, &opi);
}

ANN static m_bool emit_exp_array(const Emitter emit, const Exp_Array *array) {
  CHECK_BB(emit_exp(emit, array->base));
  const Exp              e    = exp_self(array);
  struct ArrayAccessInfo info = {*array->array, e->type, exp_getvar(e)};
  return emit_array_access(emit, &info);
}

ANN static m_bool emit_exp_slice(const Emitter emit, const Exp_Slice *range) {
  CHECK_BB(emit_exp(emit, range->base));
  CHECK_BB(emit_range(emit, range->range));
  const Symbol sym = insert_symbol("@slice");
  const Exp    e   = range->range->start ?: range->range->end;
  assert(e);
  struct Op_Import opi = {.op   = sym,
                          .lhs  = e->type,
                          .rhs  = range->base->type,
                          .pos  = e->pos,
                          .data = (uintptr_t)exp_self(range)};
  return op_emit(emit, &opi);
}

ANN static inline Instr specialid_instr(const Emitter      emit,
                                        struct SpecialId_ *spid,
                                        const Exp_Primary *prim) {
  return spid->exec ? emit_add_instr(emit, spid->exec) : spid->em(emit, prim);
}

static const f_instr upvalue[] = {UpvalueInt, UpvalueFloat, UpvalueOther,
                                  UpvalueAddr};
ANN static m_bool    emit_prim_id(const Emitter emit, const Symbol *data) {
  const Exp_Primary *prim = prim_self(data);
  if (prim->value && emit->env->func && emit->env->func->upvalues.ptr) {
    const Map map = &emit->env->func->upvalues;
    for (m_uint i = 0; i < map_size(map); ++i) {
      if (prim->value == (Value)((Exp_Primary *)VKEY(map, i))->value) {
        const Instr instr = emit_kind(emit, prim->value->type->size,
                                      exp_getvar(exp_self(prim)), upvalue);
        instr->m_val      = i ? VVAL(map, i) : 0;
        return GW_OK;
      }
    }
  }
  struct SpecialId_ *spid = specialid_get(emit->gwion, *data);
  if (spid)
    return specialid_instr(emit, spid, prim_self(data)) ? GW_OK : GW_ERROR;
  return emit_symbol(emit, prim_self(data));
}

ANN static m_bool emit_prim_perform(const Emitter emit, const Symbol *xid) {
  const Instr instr = emit_add_instr(emit, PerformEffect);
  instr->m_val      = (m_uint)s_name(*xid);
  return GW_OK;
}

ANN static m_bool emit_prim_num(const Emitter emit, const m_uint *num) {
  regpushi(emit, *num);
  return GW_OK;
}

ANN static m_bool emit_prim_float(const Emitter emit, const m_float *fnum) {
  const Instr instr = emit_add_instr(emit, RegPushImm2);
  instr->f          = *fnum;
  return GW_OK;
}

ANN static m_bool emit_prim_char(const Emitter emit, const m_str *str) {
  const char c = str2char(emit, *str, prim_pos(str));
  regpushi(emit, c);
  return GW_OK;
}

ANN static m_bool emit_prim_str(const Emitter emit, const m_str *str) {
  const Value v = prim_self(str)->value;
  if (!v->d.obj) {
    char c[strlen(*str) + 1];
    if (strlen(*str)) {
      strcpy(c, *str);
      CHECK_BB(escape_str(emit, c, prim_pos(str)));
      ;
    } else
      c[0] = '\0';
    v->d.obj = new_string2(emit->gwion, NULL, c);
  }
  regpushi(emit, (m_uint)v->d.obj);
  emit_object_addref(emit, -SZ_INT, 0);
  return GW_OK;
}

#define emit_prim_nil (void *)dummy_func

ANN static void interp_multi(const Emitter emit, const Exp e) {
  Var_Decl_List list     = e->d.exp_decl.list;
  const bool    emit_var = exp_getvar(e);
  m_uint        offset   = 0;
  while ((list = list->next))
    offset += !emit_var ? list->self->value->type->size : SZ_INT;
  if (offset) regpop(emit, offset);
}

ANN static inline void interp_size(const Emitter emit, const Type t) {
  const Instr instr = regseti(emit, t->size);
  instr->m_val2     = SZ_INT;
}

ANN /*static*/ m_bool emit_interp(const Emitter emit, const Exp exp) {
  regpushi(emit, 0);
  Exp e = exp, next = NULL;
  do {
    next    = e->next;
    e->next = NULL;
    if (emit_exp(emit, e) < 0) {
      e->next = next;
      return GW_ERROR;
    }
    if (e->exp_type == ae_exp_decl) // why only objects?
      interp_multi(emit, e);
    regseti(emit, (m_uint)e->type);
    interp_size(emit, e->type);
    const m_bool isobj = isa(e->type, emit->gwion->type[et_object]) > 0;
    if (isobj && e->exp_type != ae_exp_cast) emit_add_instr(emit, GackType);
    const Instr instr = emit_add_instr(emit, Gack);
    instr->m_val      = emit_code_offset(emit);
  } while ((e = e->next = next));
  return GW_OK;
}

ANN static m_bool emit_prim_hack(const Emitter emit, const Exp *exp) {
  CHECK_BB(emit_interp(emit, *exp));
  if (!(emit->env->func &&
        emit->env->func->def->base->xid == insert_symbol("@gack")))
    emit_add_instr(emit, GackEnd);
  else {
    const Instr instr = emit_add_instr(emit, Reg2Mem);
    instr->m_val      = SZ_INT;
    instr->m_val2     = -SZ_INT;
  }
  return GW_OK;
}

ANN static m_bool emit_prim_interp(const Emitter emit, const Exp *exp) {
  const Exp e = *exp;
  CHECK_BB(emit_interp(emit, e));
  const Instr instr = emit_add_instr(emit, GackEnd);
  instr->m_val      = 1;
  return GW_OK;
}

DECL_PRIM_FUNC(emit, m_bool, Emitter);
ANN static m_bool emit_prim(const Emitter emit, Exp_Primary *const prim) {
  return emit_prim_func[prim->prim_type](emit, &prim->d);
}

ANN static m_bool emit_dot_static_data(const Emitter emit, const Value v,
                                       const bool emit_var) {
  const m_uint size  = v->type->size;
  const Instr  instr = emit_dotstatic(emit, size, emit_var);
  instr->m_val  = (m_uint)(v->from->owner->class_data + v->from->offset);
  instr->m_val2 = size;
  return GW_OK;
}

ANN static m_bool decl_static(const Emitter emit, const Exp_Decl *decl,
                              const Var_Decl var_decl, const uint is_ref) {
  const Value v    = var_decl->value;
  Code *      code = emit->code;
  emit->code       = (Code *)vector_back(&emit->stack);
  CHECK_BB(
      emit_instantiate_decl(emit, v->type, decl->td, var_decl->array, is_ref));
  CHECK_BB(emit_dot_static_data(emit, v, 1));
  emit_add_instr(emit, Assign);
  if(get_depth(var_decl->value->type) && !is_ref)
    (void)emit_object_addref(emit, -SZ_INT, 0);
  regpop(emit, SZ_INT);
  emit->code = code;
  return GW_OK;
}

ANN static inline int struct_ctor(const Value v) {
  return tflag(v->type, tflag_struct) && v->type->nspc->pre_ctor;
}

ANN static void decl_expand(const Emitter emit, const Type t) {
  struct_expand(emit, t);
  regpush(emit, t->size - SZ_INT);
}

ANN static void emit_struct_decl_finish(const Emitter emit, const Type t,
                                        const bool emit_addr) {
  emit->code->frame->curr_offset += t->size + SZ_INT;
  emit_ext_ctor(emit, t);
  if (!emit_addr) decl_expand(emit, t);
  emit->code->frame->curr_offset -= t->size + SZ_INT;
}

ANN static m_bool emit_exp_decl_static(const Emitter emit, const Exp_Decl *decl,
                                       const Var_Decl var_decl,
                                       const bool     is_ref,
                                       const bool     emit_addr) {
  const Value v = var_decl->value;
  if (isa(v->type, emit->gwion->type[et_object]) > 0 && !is_ref)
    CHECK_BB(decl_static(emit, decl, var_decl, 0));
  CHECK_BB(emit_dot_static_data(emit, v, !struct_ctor(v) ? emit_addr : 1));
  if (struct_ctor(v)) emit_struct_decl_finish(emit, v->type, emit_addr);
  return GW_OK;
}

ANN static Instr emit_struct_decl(const Emitter emit, const Value v,
                                  const bool emit_addr) {
  emit_add_instr(emit, RegPushMem);
  const Instr instr = emit_structmember(emit, v->type->size, emit_addr);
  if (!emit_addr) {
    const m_int sz = v->type->size - SZ_INT;
    if (sz) regpush(emit, v->type->size - SZ_INT);
  }
  return instr;
}

ANN static m_bool emit_exp_decl_non_static(const Emitter   emit,
                                           const Exp_Decl *decl,
                                           const Var_Decl  var_decl,
                                           const uint      is_ref,
                                           const uint      emit_var) {
  const Value     v        = var_decl->value;
  const Type      type     = v->type;
  const Array_Sub array    = var_decl->array;
  const bool      is_array = array && array->exp;
  const bool      is_obj   = isa(type, emit->gwion->type[et_object]) > 0;
  const bool emit_addr = (!is_obj || (is_ref && !is_array)) ? emit_var : true;
  if (is_obj && (is_array || !is_ref))
    CHECK_BB(emit_instantiate_decl(emit, type, decl->td, array, is_ref));
  f_instr *exec = (f_instr *)allocmember;
  if (!emit->env->scope->depth) emit_debug(emit, v);
  if (!vflag(v, vflag_member)) {
    v->from->offset = emit_local(emit, type);
    exec            = (f_instr *)(allocword);
    if (GET_FLAG(v, late)) { // ref or emit_var ?
      const Instr clean = emit_add_instr(emit, MemSetImm);
      clean->m_val      = v->from->offset;
    }
  }
  const Instr instr =
      !(safe_tflag(emit->env->class_def, tflag_struct) &&
        !emit->env->scope->depth)
          ? emit_kind(emit, v->type->size, !struct_ctor(v) ? emit_addr : true,
                      exec)
          : emit_struct_decl(emit, v, !struct_ctor(v) ? emit_addr : 1);
  instr->m_val = v->from->offset;
  if (is_obj && (is_array || !is_ref)) {
    if (!emit_var)
      emit_add_instr(emit, Assign);
    else {
      regpop(emit, SZ_INT);
      const Instr instr = emit_add_instr(emit, Reg2Reg);
      instr->m_val      = -SZ_INT;
    }
  } else if (struct_ctor(v))
    emit_struct_decl_finish(emit, v->type, emit_addr);
  return GW_OK;
}

ANN static m_bool emit_exp_decl_global(const Emitter emit, const Exp_Decl *decl,
                                       const Var_Decl var_decl,
                                       const uint is_ref, const bool emit_var) {
  const Value     v        = var_decl->value;
  const Type      type     = v->type;
  const Array_Sub array    = var_decl->array;
  const bool      is_array = array && array->exp;
  const bool      is_obj   = isa(type, emit->gwion->type[et_object]) > 0;
  const bool emit_addr = (!is_obj || (is_ref && !is_array)) ? emit_var : true;
  if (is_obj && (is_array || !is_ref))
    CHECK_BB(emit_instantiate_decl(emit, type, decl->td, array, is_ref));
  const Instr instr =
      emit_dotstatic(emit, v->type->size, !struct_ctor(v) ? emit_addr : 1);
  if (type->size > SZ_INT) //{
    v->d.ptr = mp_calloc2(emit->gwion->mp, v->type->size);
  instr->m_val = (m_uint)&v->d.ptr;
  set_vflag(v, vflag_direct); // mpalloc
  instr->m_val2 = v->type->size;
  if (is_obj && (is_array || !is_ref || emit_addr)) {
    const Instr assign = emit_add_instr(emit, Assign);
    assign->m_val      = emit_var;
    (void)emit_object_addref(emit, -SZ_INT, emit_var);
  } else if (struct_ctor(v))
    emit_struct_decl_finish(emit, v->type, emit_addr);
  return GW_OK;
}

ANN static void set_late(const Gwion gwion, const Exp_Decl *decl,
                         const Var_Decl var) {
  const Value v         = var->value;
  const uint  array_ref = (decl->td->array && !decl->td->array->exp) ||
                         (var->array && !var->array->exp);
  if (!exp_getvar(exp_self(decl)) &&
      (GET_FLAG(array_base(v->type), abstract) || GET_FLAG(decl->td, late) ||
       is_fptr(gwion, v->type) || array_ref)) {
    SET_FLAG(v, late);
  } else
    UNSET_FLAG(v, late);
}

ANN static m_bool emit_decl(const Emitter emit, const Exp_Decl *decl) {
  const m_bool  global = GET_FLAG(decl->td, global);
  const uint    var    = exp_getvar(exp_self(decl));
  const uint    ref    = GET_FLAG(decl->td, late) || type_ref(decl->type);
  Var_Decl_List list   = decl->list;
  do {
    const Value v = list->self->value;
    const uint  r = ref || GET_FLAG(v, late);
    if (GET_FLAG(decl->td, static))
      CHECK_BB(emit_exp_decl_static(emit, decl, list->self, r, var));
    else if (!global)
      CHECK_BB(emit_exp_decl_non_static(emit, decl, list->self, r, var));
    else
      CHECK_BB(emit_exp_decl_global(emit, decl, list->self, r, var));
    if (tflag(list->self->value->type, tflag_contract) &&
        !exp_getvar(exp_self(decl))) {
      const Type       t   = list->self->value->type;
      struct Op_Import opi = {.lhs = t->info->base_type,
                              .op  = insert_symbol("@implicit"),
                              .rhs = t};
      CHECK_BB(op_emit(emit, &opi));
    }
    set_late(emit->gwion, decl, list->self);
    if (GET_FLAG(array_base(v->type), abstract) && !GET_FLAG(decl->td, late) &&
        GET_FLAG(v, late)) {
      env_warn(emit->env, decl->td->pos, _("Type '%s' is abstract, use late"),
               v->type->name);
    }
  } while ((list = list->next));
  return GW_OK;
}

ANN /*static */ m_bool emit_exp_decl(const Emitter emit, const Exp_Decl *decl) {
  const Type t = decl->type;
  CHECK_BB(ensure_emit(emit, t));
  const m_bool global = GET_FLAG(decl->td, global);
  const m_uint scope =
      !global ? emit->env->scope->depth : emit_push_global(emit);
  const m_bool ret = emit_decl(emit, decl);

  if (global) emit_pop(emit, scope);
  return ret;
}

ANN static m_uint vararg_size(const Gwion gwion, const Exp_Call *exp_call,
                              const Vector kinds) {
  Exp        e    = exp_call->args;
  const Type t    = actual_type(gwion, exp_call->func->type);
  Arg_List   l    = t->info->func->def->base->args;
  m_uint     size = 0;
  while (e) {
    if (!l) {
      size += e->type->size;
      vector_add(kinds, (vtype)e->type); // ->size
    } else
      l = l->next;
    e = e->next;
  }
  return size;
}

ANN static void emit_func_arg_vararg(const Emitter   emit,
                                     const Exp_Call *exp_call) {
  const Instr  instr = emit_add_instr(emit, VarargIni);
  struct Vector_ kinds;
  vector_init(&kinds);
  if ((instr->m_val = vararg_size(emit->gwion, exp_call, &kinds)))
    instr->m_val2 = (m_uint)kinds.ptr;
  else
    vector_release(&kinds);
}

ANN static m_bool emit_func_args(const Emitter emit, const Exp_Call *exp_call) {
  if (exp_call->args) {
    CHECK_BB(emit_exp(emit, exp_call->args));
    //    emit_exp_addref(emit, exp_call->args, -exp_totalsize(exp_call->args));
  }
  const Type t = actual_type(emit->gwion, exp_call->func->type);
  if (is_func(emit->gwion, t) &&
      fbflag(t->info->func->def->base, fbflag_variadic))
    emit_func_arg_vararg(emit, exp_call);
  return GW_OK;
}

ANN static m_bool prepare_call(const Emitter emit, const Exp_Call *exp_call) {
  CHECK_BB(emit_func_args(emit, exp_call));
  return emit_exp(emit, exp_call->func);
}

ANN static inline void emit_return_pc(const Emitter emit, const m_uint val) {
  LOOP_OPTIM
  for (m_uint i = vector_size(&emit->code->stack_return) + 1; --i;) {
    const Instr instr = (Instr)vector_at(&emit->code->stack_return, i - 1);
    instr->m_val      = val;
  }
}

ANN static inline void pop_exp(const Emitter emit, Exp e);

ANN static inline bool check_inline(const Emitter emit, const Func f) {
  const uint16_t caller_size = emit->env->func ? emit->env->func->weight
                               : emit->env->class_def
                                   ? emit->env->class_def->weight
                               : emit->env->context ? emit->env->context->weight
                                                    : 0;
  const float    threshold   = caller_size * f->inline_mult;
  return f->weight < threshold;
}

ANN static inline bool member_inlinable(const Func f, const Exp e) {
  if (fflag(f, fflag_recurs)) return false;
  const Type owner_class = f->value_ref->from->owner_class;
  if (!owner_class) return true;
  return GET_FLAG(owner_class, final) || GET_FLAG(f->def->base, final) ||
         (e->exp_type == ae_exp_dot &&
          e->d.exp_dot.base->exp_type == ae_exp_cast);
}

ANN static inline Func is_inlinable(const Emitter   emit,
                                    const Exp_Call *exp_call) {
  const Type ftype = exp_call->func->type;
  if (!is_func(emit->gwion, ftype) || is_fptr(emit->gwion, ftype) ||
      !ftype->info->func->code || ftype->info->func->code->builtin)
    return false;
  const Func f = ftype->info->func;
  return (member_inlinable(f, exp_call->func) && check_inline(emit, f)) ? f
                                                                        : NULL;
}

ANN static inline void inline_args_ini(const Emitter emit, const Func f,
                                       const Vector v) {
  const m_uint start_offset = emit_code_offset(emit);
  Arg_List     arg          = f->def->base->args;
  while (arg) {
    const Value value = arg->var_decl->value;
    vector_add(v, value->from->offset);
    value->from->offset = emit_local(emit, value->type);
    nspc_add_value(emit->env->curr, arg->var_decl->xid, value);
    arg = arg->next;
  }
  if (fbflag(f->def->base, fbflag_variadic))
    emit->vararg_offset = emit_local(emit, emit->gwion->type[et_int]) + SZ_INT;
  regpop(emit, f->code->stack_depth);
  const Instr cpy = emit_add_instr(emit, Reg2Mem4);
  cpy->m_val2     = f->code->stack_depth;
  cpy->m_val      = start_offset;
}

ANN static inline void inline_args_end(const Func f, const Vector v) {
  Arg_List arg = f->def->base->args;
  m_uint   i   = 0;
  while (arg) {
    const Value value   = arg->var_decl->value;
    value->from->offset = vector_at(v, i++);
    arg                 = arg->next;
  }
}

ANN static m_bool        scoped_stmt(const Emitter emit, const Stmt stmt,
                                     const m_bool pop);
ANN static inline m_bool inline_body(const Emitter emit, const Func f) {
  struct Vector_ v = {.ptr = emit->code->stack_return.ptr};
  vector_init(&emit->code->stack_return);
  nspc_push_value(emit->gwion->mp, emit->env->curr);
  const m_bool ret = scoped_stmt(emit, f->def->d.code, 1);
  nspc_pop_value(emit->gwion->mp, emit->env->curr);
  emit_return_pc(emit, emit_code_size(emit));
  vector_release(&emit->code->stack_return);
  emit->code->stack_return.ptr = v.ptr;
  return ret;
}

ANN static inline m_bool inline_run(const Emitter emit, const Func f) {
  struct Vector_ arg_offset;
  vector_init(&arg_offset);
  const uint16_t this_offset   = emit->this_offset;
  const uint16_t vararg_offset = emit->vararg_offset;
  inline_args_ini(emit, f, &arg_offset);
  const m_bool ret    = inline_body(emit, f);
  emit->this_offset   = this_offset;
  emit->vararg_offset = vararg_offset;
  inline_args_end(f, &arg_offset);
  vector_release(&arg_offset);
  return ret;
}

ANN static inline m_bool emit_inline(const Emitter emit, const Func f,
                                     const Exp_Call *exp_call) {
  if (!f->weight) return GW_OK;
  if (f->value_ref->from->owner_class && vflag(f->value_ref, vflag_member)) {
    CHECK_BB(emit_exp(emit, exp_call->func->d.exp_dot.base));
    emit->this_offset = emit_local(emit, emit->gwion->type[et_int]);
  }
  CHECK_BB(emit_func_args(emit, exp_call));
  return inline_run(emit, f);
}

ANN static m_bool _emit_exp_call(const Emitter emit, const Exp_Call *exp_call) {
  /*
  #ifndef GWION_NOINLINE
    const Func f = is_inlinable(emit, exp_call);
    if(f)
      return emit_inline(emit, f, exp_call);
  #endif
  */
  // skip when recursing
  const Type t = actual_type(emit->gwion, exp_call->func->type);
  const Func f = t->info->func;
  if (is_fptr(emit->gwion, t) || strstr(emit->code->name, "ork~") ||
      (f != emit->env->func || (f && f->value_ref->from->owner_class)))
    CHECK_BB(prepare_call(emit, exp_call));
  else
    CHECK_BB(emit_func_args(emit, exp_call));
  if (is_func(emit->gwion, t))
    CHECK_BB(emit_exp_call1(emit, t->info->func,
                            is_static_call(emit, exp_call->func)));
  else {
    struct Op_Import opi = {.op   = insert_symbol("@ctor"),
                            .rhs  = t,
                            .data = (uintptr_t)exp_call,
                            .pos  = exp_self(exp_call)->pos};
    CHECK_BB(op_emit(emit, &opi));
  }
  return GW_OK;
}

ANN static m_bool emit_exp_call(const Emitter emit, const Exp_Call *exp_call) {
  const Exp e = exp_self(exp_call);
  if (exp_getvar(e)) regpush(emit, SZ_INT);
  CHECK_BB(_emit_exp_call(emit, exp_call));
  if (exp_getvar(e)) {
    regpop(emit, exp_self(exp_call)->type->size);
    const Instr instr = emit_add_instr(emit, Reg2RegAddr);
    instr->m_val      = -SZ_INT;
  } else if (!is_func(emit->gwion, exp_call->func->type) &&
             tflag(e->type, tflag_struct))
    regpop(emit, SZ_INT);
  return GW_OK;
}

ANN static m_uint get_decl_size(Var_Decl_List a, bool emit_addr) {
  m_uint size = 0;
  do // if(GET_FLAG(a->self->value, used))
    size += !emit_addr ? a->self->value->type->size : SZ_INT;
  while ((a = a->next));
  return size;
}

ANN static m_uint pop_exp_size(Exp e) {
  const bool emit_addr = exp_getvar(e);
  m_uint     size      = 0;
  do {
    size += (e->exp_type == ae_exp_decl
                 ? get_decl_size(e->d.exp_decl.list, emit_addr)
             : !emit_addr ? e->type->size
                          : SZ_INT);
  } while ((e = e->next));
  return size;
}

ANN static inline void pop_exp(const Emitter emit, Exp e) {
  const m_uint size = pop_exp_size(e);
  if (size) regpop(emit, size);
}

ANN static inline m_bool emit_exp_pop_next(const Emitter emit, Exp e) {
  CHECK_BB(emit_exp(emit, e));
  if (e->exp_type == ae_exp_decl) {
    Var_Decl_List list = e->d.exp_decl.list->next;
    while (list) {
      regpop(emit, !exp_getvar(e) ? list->self->value->type->size : SZ_INT);
      list = list->next;
    }
  }
  if (e->next) pop_exp(emit, e->next);
  return GW_OK;
}

ANN static m_bool emit_exp_binary(const Emitter emit, const Exp_Binary *bin) {
  const Exp lhs = bin->lhs;
  const Exp rhs = bin->rhs;
  CHECK_BB(emit_exp_pop_next(emit, lhs));
  CHECK_BB(emit_exp_pop_next(emit, rhs));
  //  const m_int size = exp_size(rhs);
  //  emit_exp_addref1(emit, lhs, -exp_size(lhs) - size);
  //  emit_exp_addref1(emit, rhs, -size);
  struct Op_Import opi = {.op   = bin->op,
                          .lhs  = lhs->type,
                          .rhs  = rhs->type,
                          .pos  = exp_self(bin)->pos,
                          .data = (uintptr_t)bin};
  return op_emit(emit, &opi);
}

ANN static m_bool emit_exp_cast(const Emitter emit, const Exp_Cast *cast) {
  CHECK_BB(emit_exp(emit, cast->exp));
  struct Op_Import opi = {.op   = insert_symbol("$"),
                          .lhs  = cast->exp->type,
                          .rhs  = exp_self(cast)->type,
                          .data = (uintptr_t)cast}; // no pos ?
  (void)op_emit(emit, &opi);
  return GW_OK;
}

ANN static m_bool emit_exp_post(const Emitter emit, const Exp_Postfix *post) {
  CHECK_BB(emit_exp(emit, post->exp));
  struct Op_Import opi = {.op   = post->op,
                          .lhs  = post->exp->type,
                          .data = (uintptr_t)post}; // no pos ?
  return op_emit(emit, &opi);
}

ANN static inline m_bool traverse_emit_func_def(const Emitter  emit,
                                                const Func_Def fdef) {
  if (!fdef->base->ret_type) CHECK_BB(traverse_func_def(emit->env, fdef));
  return emit_func_def(emit, fdef);
}

ANN m_bool traverse_dot_tmpl(const Emitter emit, const struct dottmpl_ *dt) {
  const m_uint  scope = emit->env->scope->depth;
  struct EnvSet es    = {.env   = emit->env,
                      .data  = emit,
                      .func  = (_exp_func)emit_cdef,
                      .scope = scope,
                      .flag  = tflag_emit};
  CHECK_BB(envset_push(&es, dt->owner_class, dt->owner));
  (void)emit_push(emit, dt->owner_class, dt->owner);
  const m_bool ret = traverse_emit_func_def(emit, dt->def);
  if (es.run) envset_pop(&es, dt->owner_class);
  emit_pop(emit, scope);
  return ret;
}

static inline m_bool push_func_code(const Emitter emit, const Func f) {
  if (!vector_size(&emit->code->instr)) return GW_OK;
  const Instr instr = (Instr)vector_back(&emit->code->instr);
  if (instr->opcode == eDotTmplVal) {
    size_t len = strlen(f->name);
    assert(f->value_ref->from->owner_class);
    size_t sz = len - strlen(f->value_ref->from->owner_class->name);
    char   c[sz + 1];
    memcpy(c, f->name, sz);
    c[sz]               = '\0';
    struct dottmpl_ *dt = mp_calloc(emit->gwion->mp, dottmpl);
    dt->name            = s_name(insert_symbol(c));
    dt->tl         = cpy_type_list(emit->gwion->mp, f->def->base->tmpl->call);
    dt->base       = f->def;
    instr->opcode  = eOP_MAX;
    instr->m_val   = (m_uint)dt;
    instr->m_val2  = strlen(c);
    instr->execute = DotTmpl;
    return GW_OK;
  }
  instr->opcode = eRegPushImm;
  instr->m_val  = (m_uint)f->code;
  return GW_OK;
}

ANN static m_bool emit_template_code(const Emitter emit, const Func f) {
  const Value   v     = f->value_ref;
  const size_t  scope = emit->env->scope->depth;
  struct EnvSet es    = {.env   = emit->env,
                      .data  = emit,
                      .func  = (_exp_func)emit_cdef,
                      .scope = scope,
                      .flag  = tflag_emit};
  CHECK_BB(envset_pushv(&es, v));
  (void)emit_push(emit, v->from->owner_class, v->from->owner);
  const m_bool ret = emit_func_def(emit, f->def);
  if (es.run) envset_pop(&es, v->from->owner_class);
  emit_pop(emit, scope);
  return ret > 0 ? push_func_code(emit, f) : GW_ERROR;
}

ANN static void tmpl_prelude(const Emitter emit, const Func f) {
  struct dottmpl_ *dt  = (struct dottmpl_ *)mp_calloc(emit->gwion->mp, dottmpl);
  size_t           len = strlen(f->name);
  size_t           slen = strlen(f->value_ref->from->owner->name);
  assert(len > slen);
  size_t sz = len - slen;
  char   c[sz + 1];
  memcpy(c, f->name, sz);
  c[sz]             = '\0';
  dt->tl            = cpy_type_list(emit->gwion->mp, f->def->base->tmpl->call);
  dt->name          = s_name(insert_symbol(c));
  dt->base          = f->def;
  dt->owner         = f->value_ref->from->owner;
  dt->owner_class   = f->value_ref->from->owner_class;
  const Instr gtmpl = emit_add_instr(emit, GTmpl);
  gtmpl->m_val      = (m_uint)dt;
  gtmpl->m_val2     = strlen(c);
}

ANN static Instr get_prelude(const Emitter emit, const Func f,
                             const bool is_static) {
  const Type t  = actual_type(emit->gwion, f->value_ref->type);
  const bool fp = is_fptr(emit->gwion, t);
  if (is_fptr(emit->gwion, t)) {
    if (f->def->base->tmpl) tmpl_prelude(emit, f);
  }
  if (fp || f != emit->env->func || (!is_static && strcmp(s_name(f->def->base->xid), "new"))||
      strstr(emit->code->name, "ork~")) {
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
  if ((f->def->stack_depth - member) == SZ_INT) {
    const Instr instr = emit_add_instr(emit, Reg2Mem);
    instr->m_val      = member;
  } else {
    const Instr instr = emit_add_instr(emit, Reg2Mem4);
    instr->m_val      = member;
    instr->m_val2     = f->def->stack_depth - member;
  }
}

typedef struct {
  const Emitter  emit;
  const Func_Def fdef;
  struct Vector_ branch;
  const m_uint   offset;
  m_uint         arg_offset;
} MemoizeEmitter;

ANN static Instr me_push(const MemoizeEmitter *me, const m_uint sz) {
  const Instr instr = emit_regpushmem(me->emit, sz, false);
  instr->m_val      = me->arg_offset;
  return instr;
}

static m_bool me_cmp(MemoizeEmitter *me, const Arg_List arg) {
  const Emitter    emit = me->emit;
  const Symbol     sym  = insert_symbol("==");
  struct Exp_      exp  = {};
  struct Op_Import opi  = {.op   = sym,
                          .lhs  = arg->type,
                          .rhs  = arg->type,
                          .pos  = me->fdef->base->pos,
                          .data = (uintptr_t)&exp.d};
  CHECK_BB(op_emit(emit, &opi));
  const Instr instr = emit_add_instr(emit, BranchEqInt);
  vector_add(&me->branch, (vtype)instr);
  return GW_OK;
}

ANN static m_bool me_arg(MemoizeEmitter *me) {
  Arg_List arg = me->fdef->base->args;
  do {
    const m_uint sz = arg->type->size;
    (void)me_push(me, sz);
    const Instr instr = me_push(me, sz);
    instr->m_val += me->offset + SZ_INT * 2;
    CHECK_BB(me_cmp(me, arg));
    me->arg_offset += arg->type->size;
  } while ((arg = arg->next));
  return GW_OK;
}

ANN static Instr emit_call(const Emitter emit, const Func f,
                           const bool is_static) {
  const Instr prelude = get_prelude(emit, f, is_static);
  prelude->m_val += -f->def->stack_depth - SZ_INT;
  const m_uint member = vflag(f->value_ref, vflag_member) ? SZ_INT : 0;
  if (member) {
    const Instr instr = emit_add_instr(emit, Reg2Mem);
    instr->m_val2     = f->def->stack_depth - SZ_INT;
    ++prelude->m_val2;
  }
  if (f->def->stack_depth - member) {
    //  if(is_fptr(emit->gwion, t) || f != emit->env->func ||
    //  f->value_ref->from->owner_class || strstr(emit->code->name, "ork~"))
    {
      emit_args(emit, f);
      ++prelude->m_val2;
    }
  }
  return emit_add_instr(emit, Overflow);
}

ANN static m_bool emit_ensure_func(const Emitter emit, const Func f) {
  const struct ValueFrom_ *from = f->value_ref->from;
  if(from->owner_class)
    CHECK_BB(ensure_emit(emit, from->owner_class));
  if(f->code) return GW_OK;
  const m_uint scope = emit_push(emit, from->owner_class, from->owner);
  const m_bool ret = emit_func_def(emit, f->def);
  emit_pop(emit, scope);
  return ret;
}

ANN m_bool emit_exp_call1(const Emitter emit, const Func f,
                          const bool is_static) {
  const m_uint this_offset   = emit->this_offset;
  const m_uint vararg_offset = emit->vararg_offset;
  emit->this_offset          = 0;
  const int tmpl             = fflag(f, fflag_tmpl);
  if (!f->code ||
      (fflag(f, fflag_ftmpl) && !vflag(f->value_ref, vflag_builtin))) {
    if (tmpl && !is_fptr(emit->gwion, f->value_ref->type)) {
      if (emit->env->func != f)
        CHECK_BB(emit_template_code(emit, f));
      else { // recursive function. (maybe should be used only for global funcs)
        /*  const Instr back = (Instr) vector_size(&emit->code->instr) ?
              (Instr)vector_back(&emit->code->instr) : emit_add_instr(emit,
          RegPushImm); back->opcode = eOP_MAX; back->execute = SetRecurs;
          back->m_val = 0;*/
      }
    } else if (emit->env->func != f && !f->value_ref->from->owner_class &&
               !f->code && !is_fptr(emit->gwion, f->value_ref->type)) {
      CHECK_BB(emit_ensure_func(emit, f));
      if (fbflag(f->def->base, fbflag_op)) {
        const Instr back = (Instr)vector_back(&emit->code->instr);
        assert(back->execute == SetFunc);
        if(f->code) {
          back->opcode = eRegPushImm;
          back->m_val = (m_uint)f->code;
          //  back->m_val2     = SZ_INT;
        } else
          back->m_val      = (m_uint)f;
      } else {
        const Instr instr = emit_add_instr(emit, RegSetImm);
        instr->m_val      = (m_uint)f->code;
        instr->m_val2     = -SZ_INT;
      }
    }
  } else if ((f->value_ref->from->owner_class &&
              tflag(f->value_ref->from->owner_class, tflag_struct)) ||
             !f->value_ref->from->owner_class ||
             (tmpl && !is_fptr(emit->gwion, f->value_ref->type)))
    push_func_code(emit, f);
  else if (vector_size(&emit->code->instr)) {
    const Instr back = (Instr)vector_back(&emit->code->instr);
    if ((f_instr)(m_uint)back->opcode == DotFunc) back->m_val = f->vt_index;
  }
  if (vector_size(&emit->code->instr) && vflag(f->value_ref, vflag_member) &&
      is_fptr(emit->gwion, f->value_ref->type)) {
    const Instr back      = (Instr)vector_back(&emit->code->instr);
    const bool  is_except = back->opcode == eGWOP_EXCEPT;
    const Instr base =
        !is_except ? back
                   : (Instr)vector_at(&emit->code->instr,
                                      vector_size(&emit->code->instr) - 2);
    if (is_except) vector_pop(&emit->code->instr);
    const m_bit  exec = base->opcode;
    const m_uint val  = base->m_val;
    const m_uint val2 = base->m_val2;
    base->opcode      = eReg2Reg;
//    base->m_val       = 0;//-SZ_INT;
    base->m_val2      = -SZ_INT;
    regpush(emit, SZ_INT);
    const Instr instr = emit_add_instr(emit, (f_instr)(m_uint)exec);
    instr->m_val      = val;
    instr->m_val2     = val2;
    if (is_except) {
      vector_add(&emit->code->instr, (m_uint)back);
      back->m_val = -SZ_INT;
    }
  } else if (f != emit->env->func && !f->code &&
             !is_fptr(emit->gwion, f->value_ref->type)) {
    // not yet emitted static func
    if (f->value_ref->from->owner_class) {
      assert(GET_FLAG(f->value_ref, static));
      const Instr instr = vector_size(&emit->code->instr)
                              ? (Instr)vector_back(&emit->code->instr)
                              : emit_add_instr(emit, SetFunc);
      instr->opcode     = eOP_MAX;
      instr->execute    = SetFunc;
      instr->m_val      = (m_uint)f;
    } else {
        const Instr back = (Instr)vector_back(&emit->code->instr);
        if(back->execute != SetFunc) {
          const Instr instr = emit_add_instr(emit, SetFunc);
          instr->m_val      = (m_uint)f;
        }
    }
  }
  const m_uint offset = emit_code_offset(emit);
  if (f != emit->env->func || !is_static)
    regseti(emit, offset /*+ f->def->stack_depth + sizeof(frame_t)*/);
  const Instr instr   = emit_call(emit, f, is_static);
  instr->m_val        = f->def->base->ret_type->size;
  instr->m_val2       = offset;
  emit->this_offset   = this_offset;
  emit->vararg_offset = vararg_offset;
  return GW_OK;
}

ANN static void emit_exp_spork_finish(const Emitter emit, const m_uint depth) {
  regpop(emit, depth);
  const Instr spork = emit_add_instr(emit, SporkFunc);
  spork->m_val      = depth + SZ_INT;
  spork->m_val2     = -SZ_INT;
}

ANN static inline void
stack_alloc(const Emitter emit) { // maybe vararg could use t_vararg instead
  emit_local(emit,
             emit->gwion->type[et_int]); // hiding the fact it is an object
  emit->code->stack_depth += SZ_INT;
}

ANN static inline Instr scoped_ini(const Emitter emit) {
  ++emit->env->scope->depth;
  emit_push_scope(emit);
  return emit_add_instr(emit, NoOp);
}

ANN static inline void scoped_end(const Emitter emit, const Instr gc) {
  emit_pop_scope(emit);
  const m_bool pure =
      vector_size(&emit->info->pure) && !vector_back(&emit->info->pure);
  if (!pure) {
    gc->opcode = eGcIni;
    emit_add_instr(emit, GcEnd);
  }
  --emit->env->scope->depth;
}

ANN static m_bool scoped_stmt(const Emitter emit, const Stmt stmt,
                              const m_bool pop) {
  const Instr  gc  = scoped_ini(emit);
  const m_bool ret = emit_stmt(emit, stmt, pop);
  scoped_end(emit, gc);
  return ret;
}

#define SPORK_FUNC_PREFIX "spork~func:%u"
#define FORK_FUNC_PREFIX  "fork~func:%u"
#define SPORK_CODE_PREFIX "spork~code:%u"
#define FORK_CODE_PREFIX  "fork~code:%u"

static void push_spork_code(const Emitter emit, const m_str prefix,
                            const loc_t pos) {
  char c[strlen(SPORK_FUNC_PREFIX) + num_digit(pos.first.line) + 1];
  sprintf(c, prefix, pos.first.line);
  emit_push_code(emit, c);
}

struct Sporker {
  const Stmt code;
  const Exp  exp;
  VM_Code    vm_code;
  const Type type;
  const bool emit_var;
  const bool is_spork;
};

ANN static m_bool spork_prepare_code(const Emitter         emit,
                                     const struct Sporker *sp) {
  emit_add_instr(emit, RegPushImm);
  push_spork_code(emit, sp->is_spork ? SPORK_CODE_PREFIX : FORK_CODE_PREFIX,
                  sp->code->pos);
  if (emit->env->class_def) stack_alloc(emit);
  if (emit->env->func && vflag(emit->env->func->value_ref, vflag_member))
    stack_alloc(emit);
  return scoped_stmt(emit, sp->code, 0);
}

ANN static m_bool spork_prepare_func(const Emitter         emit,
                                     const struct Sporker *sp) {
  const Type t = actual_type(emit->gwion, sp->exp->d.exp_call.func->type);
  const Func f = t->info->func;
  if(!f->code && f != emit->env->func)
    CHECK_BB(emit_ensure_func(emit, f));
  push_spork_code(emit, sp->is_spork ? SPORK_FUNC_PREFIX : FORK_CODE_PREFIX,
                  sp->exp->pos);
  return emit_exp_call1(emit, f, false);
}

ANN static VM_Code spork_prepare(const Emitter emit, const struct Sporker *sp) {
  if (!sp->code) {
    assert(sp->exp);
    CHECK_BO(prepare_call(emit, &sp->exp->d.exp_call));
  }
  if ((sp->code ? spork_prepare_code : spork_prepare_func)(emit, sp) > 0)
    return finalyze(emit, EOC);
  emit_pop_code(emit);
  return NULL;
}

ANN void spork_code(const Emitter emit, const struct Sporker *sp) {
  const Instr args  = emit_add_instr(emit, SporkExp);
  args->m_val       = emit->code->stack_depth;
  const Instr instr = emit_add_instr(emit, SporkEnd);
  instr->m_val      = sp->emit_var;
}

ANN void spork_func(const Emitter emit, const struct Sporker *sp) {
  const Func f =
      actual_type(emit->gwion, sp->exp->d.exp_call.func->type)->info->func;
  if (vflag(f->value_ref, vflag_member) &&
      is_fptr(emit->gwion, f->value_ref->type)) {
    regpush(emit, SZ_INT * 2);
    // (re-)emit owner
    if (sp->exp->d.exp_call.func->exp_type == ae_exp_dot)
      emit_exp(emit, sp->exp->d.exp_call.func->d.exp_dot.base);
    else {
      assert(sp->exp->d.exp_call.func->exp_type == ae_exp_primary);
      emit_add_instr(emit, RegPushMem);
    }
    regpop(emit, SZ_INT * 3);
    const m_uint depth = f->def->stack_depth;
    regpop(emit, depth - SZ_INT);
    const Instr spork = emit_add_instr(emit, SporkMemberFptr);
    spork->m_val      = depth;
  } else
    emit_exp_spork_finish(emit, f->def->stack_depth);
  (void)emit_add_instr(emit, SporkEnd);
}

ANN static void spork_ini(const Emitter emit, const struct Sporker *sp) {
  if (sp->is_spork) {
    const Instr instr = emit_add_instr(emit, SporkIni);
    instr->m_val      = (m_uint)sp->vm_code;
    instr->m_val2     = sp->is_spork;
    return;
  }
  regpushi(emit, (m_uint)sp->type);
  const Instr instr = emit_add_instr(emit, ForkIni);
  instr->m_val      = (m_uint)sp->vm_code;
  instr->m_val2     = sp->type->size;
}

ANN m_bool emit_exp_spork(const Emitter emit, const Exp_Unary *unary) {
  struct Sporker sporker = {
      .exp      = unary->unary_type == unary_exp ? unary->exp : NULL,
      .code     = unary->unary_type == unary_code ? unary->code : NULL,
      .type     = exp_self(unary)->type,
      .is_spork = (unary->op == insert_symbol("spork")),
      .emit_var = exp_getvar(exp_self(unary))};
  CHECK_OB((sporker.vm_code = spork_prepare(emit, &sporker)));
  spork_ini(emit, &sporker);
  (unary->unary_type == unary_code ? spork_code : spork_func)(emit, &sporker);
  return GW_OK;
}

ANN static m_bool emit_exp_unary(const Emitter emit, const Exp_Unary *unary) {
  const Type t    = exp_self(unary)->type;
  const Type base = actual_type(emit->gwion, t);
  CHECK_BB(ensure_emit(emit, base));
  // no pos ?
  struct Op_Import opi = {.op = unary->op, .data = (uintptr_t)unary};
  if (unary->unary_type == unary_exp && unary->op != insert_symbol("spork") &&
      unary->op != insert_symbol("fork")) {
    CHECK_BB(emit_exp_pop_next(emit, unary->exp));
    opi.rhs = unary->exp->type;
  }
  return op_emit(emit, &opi);
}

ANN static m_bool emit_implicit_cast(const Emitter       emit,
                                     const restrict Exp  from,
                                     const restrict Type to) {
  const struct Implicit imp = {from, to, from->pos};
  // no pos
  struct Op_Import opi = {.op   = insert_symbol("@implicit"),
                          .lhs  = from->type,
                          .rhs  = to,
                          .data = (m_uint)&imp};
  return op_emit(emit, &opi);
}

ANN static Instr _flow(const Emitter emit, const Exp e, const m_bool b) {
  CHECK_BO(emit_exp_pop_next(emit, e));
  emit_exp_addref1(emit, e, -exp_size(e));
  struct Op_Import opi = {
      .op   = insert_symbol(b ? "@conditional" : "@unconditional"),
      .rhs  = e->type,
      .pos  = e->pos,
      .data = (uintptr_t)e};
  CHECK_BO(op_emit(emit, &opi));
  return (Instr)vector_back(&emit->code->instr);
}
#define emit_flow(emit, b) _flow(emit, b, 1)

ANN static m_bool emit_exp_if(const Emitter emit, const Exp_If *exp_if) {
  const Exp e = exp_if->if_exp ?: exp_if->cond;
  if (exp_getvar(exp_self(exp_if))) {
    exp_setvar(e, 1);
    exp_setvar(exp_if->else_exp, 1);
  }
  DECL_OB(const Instr, op, = emit_flow(emit, exp_if->cond));
  CHECK_BB(emit_exp_pop_next(emit, e));
  const Instr op2  = emit_add_instr(emit, Goto);
  op->m_val        = emit_code_size(emit);
  const m_bool ret = emit_exp_pop_next(emit, exp_if->else_exp);
  op2->m_val       = emit_code_size(emit);
  return ret;
}

ANN static inline m_bool emit_prim_novar(const Emitter      emit,
                                         const Exp_Primary *prim) {
  const Exp  e   = exp_self(prim);
  const uint var = exp_getvar(e);
  exp_setvar(e, 0);
  CHECK_BB(emit_symbol(emit, prim));
  exp_setvar(e, var);
  return GW_OK;
}

ANN static m_bool emit_upvalues(const Emitter emit, const Func func) {
  const Map map = &func->upvalues;
  for (m_uint i = 0; i < map_size(map); ++i) {
    const Exp_Primary *prim = (Exp_Primary *)VKEY(map, i);
    const Value        v    = prim->value;
    CHECK_BB(emit_prim_novar(emit, prim));
    ;
    if (isa(prim->value->type, emit->gwion->type[et_compound]) > 0) {
      if (vflag(v, vflag_fglobal) && !vflag(v, vflag_closed))
        emit_exp_addref1(emit, exp_self(prim), -v->type->size);
      map_set(&func->code->closure->m, (vtype)v->type, VVAL(map, i));
    }
    set_vflag(v, vflag_closed);
  }
  return GW_OK;
}

ANN static m_bool emit_closure(const Emitter emit, const Func func) {
  const Map    map = &func->upvalues;
  const m_uint sz =
      VVAL(map, VLEN(map) - 1) +
      ((Exp_Primary *)VKEY(map, VLEN(map) - 1))->value->type->size;
  func->code->closure = new_closure(emit->gwion->mp, sz);
  regpushi(emit, (m_uint)func->code->closure->data);
  CHECK_BB(emit_upvalues(emit, func));
  regpop(emit, sz);
  const Instr cpy = emit_add_instr(emit, Reg2RegOther);
  cpy->m_val2     = sz;
  regpop(emit, SZ_INT);
  return GW_OK;
}

ANN static m_bool emit_lambda(const Emitter emit, const Exp_Lambda *lambda) {
  CHECK_BB(emit_func_def(emit, lambda->def));
  if (lambda->def->base->func->upvalues.ptr)
    CHECK_BB(emit_closure(emit, lambda->def->base->func));
  if (vflag(lambda->def->base->func->value_ref, vflag_member) &&
      !exp_getvar(exp_self(lambda)))
    emit_add_instr(emit, RegPushMem);
  regpushi(emit, (m_uint)lambda->def->base->func->code);
  return GW_OK;
}

ANN static m_bool emit_exp_lambda(const Emitter     emit,
                                  const Exp_Lambda *lambda) {
  if (!lambda->def->base->func) {
    regpushi(emit, SZ_INT);
    return GW_OK;
  }
  struct EnvSet es = {.env   = emit->env,
                      .data  = emit,
                      .func  = (_exp_func)emit_cdef,
                      .scope = emit->env->scope->depth,
                      .flag  = tflag_emit};
  CHECK_BB(envset_pushv(&es, lambda->def->base->func->value_ref));
  const m_bool ret = emit_lambda(emit, lambda);
  if (es.run) envset_pop(&es, lambda->owner);
  return ret;
}

ANN static m_bool emit_exp_td(const Emitter emit, Type_Decl *td) {
  const Type base = exp_self(td)->type;
  regpushi(emit, (m_uint)base);
  return GW_OK;
}

DECL_EXP_FUNC(emit, m_bool, Emitter)

ANN2(1) /*static */ m_bool emit_exp(const Emitter emit, /* const */ Exp e) {
  Exp exp = e;
  do {
    if (emit->info->debug && emit->info->line < e->pos.first.line) {
      const Instr instr = emit_add_instr(emit, DebugLine);
      instr->m_val = emit->info->line = e->pos.first.line;
    }
    CHECK_BB(emit_exp_func[exp->exp_type](emit, &exp->d));
    if (exp->cast_to) CHECK_BB(emit_implicit_cast(emit, exp, exp->cast_to));
    if (isa(e->type, emit->gwion->type[et_object]) > 0 &&
        (e->cast_to ? isa(e->cast_to, emit->gwion->type[et_object]) > 0 : 1) &&
        e->exp_type == ae_exp_decl && GET_FLAG(e->d.exp_decl.td, late) &&
        exp_getuse(e) && !exp_getvar(e) &&
        GET_FLAG(e->d.exp_decl.list->self->value, late)) {
      //         e->exp_type == ae_exp_decl && !exp_getvar(e)) {
      const Instr instr = emit_add_instr(emit, GWOP_EXCEPT);
      instr->m_val      = -SZ_INT;
    }
  } while ((exp = exp->next));
  return GW_OK;
}

ANN static m_bool emit_if_const(const Emitter emit, const Stmt_If stmt) {
  if (stmt->cond->d.prim.d.num) return emit_stmt(emit, stmt->if_body, 1);
  return stmt->else_body ? emit_stmt(emit, stmt->else_body, 1) : GW_OK;
}

ANN static m_bool emit_if(const Emitter emit, const Stmt_If stmt) {
  if (stmt->cond->exp_type == ae_exp_primary &&
      stmt->cond->d.prim.prim_type == ae_prim_num)
    return emit_if_const(emit, stmt);
  DECL_OB(const Instr, op, = emit_flow(emit, stmt->cond));
  CHECK_BB(scoped_stmt(emit, stmt->if_body, 1));
  const Instr op2 = emit_add_instr(emit, Goto);
  op->m_val       = emit_code_size(emit);
  if (stmt->else_body) CHECK_BB(scoped_stmt(emit, stmt->else_body, 1));
  op2->m_val = emit_code_size(emit);
  return GW_OK;
}

ANN static m_bool emit_stmt_if(const Emitter emit, const Stmt_If stmt) {
  emit_push_scope(emit);
  const m_bool ret = emit_if(emit, stmt);
  emit_pop_scope(emit);
  return ret;
}

ANN static m_bool emit_stmt_code(const Emitter emit, const Stmt_Code stmt) {
  emit_push_scope(emit);
  ++emit->env->scope->depth;
  const m_bool ret =
      stmt->stmt_list ? emit_stmt_list(emit, stmt->stmt_list) : 1;
  emit_pop_scope(emit);
  --emit->env->scope->depth;
  return ret;
}

ANN static m_bool optimize_tail_call(const Emitter emit, const Exp_Call *e) {
  if (e->args) {
    emit_func_args(emit, e);
    const Func f = e->func->type->info->func;
    regpop(emit, f->def->stack_depth);
    emit_args(emit, f);
  }
  emit_add_instr(emit, Goto);
  return GW_OK;
}

ANN static m_bool emit_stmt_return(const Emitter emit, const Stmt_Exp stmt) {
  CHECK_BB(emit_defers2(emit));
  if (stmt->val) {
    if (stmt->val->exp_type == ae_exp_call) {
      const Func f = stmt->val->d.exp_call.func->type->info->func;
      if (stmt->val->exp_type == ae_exp_call && emit->env->func == f)
        return optimize_tail_call(emit, &stmt->val->d.exp_call);
    }
    CHECK_BB(emit_exp_pop_next(emit, stmt->val));
  }
  vector_add(&emit->code->stack_return, (vtype)emit_add_instr(emit, Goto));
  return GW_OK;
}

ANN static inline m_bool emit_jump_index(const Emitter emit, const Vector v,
                                         const m_int n) {
  vector_add(v, 0); // make room
  const m_uint sz  = vector_size(v);
  m_int        idx = 1;
  for (m_uint i = sz; --i > 1;) {
    if (!vector_at(v, i) && ++idx == n) {
      m_uint *data = v->ptr + OFFSET + i;
      memmove(data + 1, data, (sz - i) * SZ_INT);
      const Instr instr = emit_add_instr(emit, Goto);
      VPTR(v, i - 1)    = (m_uint)instr;
      return GW_OK;
    }
  }
  return GW_ERROR;
}

ANN static inline m_bool emit_stmt_continue(const Emitter    emit,
                                            const Stmt_Index stmt) {
  CHECK_BB(emit_defers2(emit));
  if (stmt->idx == -1 || stmt->idx == 1)
    vector_add(&emit->code->stack_cont, (vtype)emit_add_instr(emit, Goto));
  else if (stmt->idx) {
    if (emit_jump_index(emit, &emit->code->stack_cont, stmt->idx) < 0)
      ERR_B(stmt_self(stmt)->pos, _("to many jumps required."))
  }
  return GW_OK;
}

ANN static inline m_bool emit_stmt_break(const Emitter         emit,
                                         const Stmt_Index stmt NUSED) {
  CHECK_BB(emit_defers2(emit));
  if (stmt->idx == -1 || stmt->idx == 1)
    vector_add(&emit->code->stack_break, (vtype)emit_add_instr(emit, Goto));
  else if (stmt->idx) {
    if (emit_jump_index(emit, &emit->code->stack_break, stmt->idx) < 0)
      ERR_B(stmt_self(stmt)->pos, _("to many jumps required."))
  }
  return GW_OK;
}

ANN static inline void emit_push_stack(const Emitter emit) {
  emit_push_scope(emit);
  vector_add(&emit->code->stack_cont, (vtype)NULL);
  vector_add(&emit->code->stack_break, (vtype)NULL);
}

ANN static void pop_vector(Vector v, const m_uint pc) {
  Instr instr;
  while ((instr = (Instr)vector_pop(v))) instr->m_val = pc;
}

ANN static void emit_pop_stack(const Emitter emit, const m_uint index) {
  pop_vector(&emit->code->stack_cont, index);
  pop_vector(&emit->code->stack_break, emit_code_size(emit));
  emit_pop_scope(emit);
}

ANN static m_bool _emit_stmt_flow(const Emitter emit, const Stmt_Flow stmt,
                                  const m_uint index) {
  Instr           op       = NULL;
  const ae_stmt_t is_while = stmt_self(stmt)->stmt_type == ae_stmt_while;
  const uint      is_const = stmt->cond->exp_type == ae_exp_primary &&
                        stmt->cond->d.prim.prim_type == ae_prim_num;
  if (!stmt->is_do) {
    if (!is_const)
      op = _flow(emit, stmt->cond, is_while);
    else if ((!is_while && stmt->cond->d.prim.d.num) ||
             (is_while && !stmt->cond->d.prim.d.num))
      return GW_OK;
  }
  CHECK_BB(scoped_stmt(emit, stmt->body, 1));
  if (stmt->is_do) {
    if (!is_const) {
      CHECK_OB((op = _flow(emit, stmt->cond, !is_while)));
      op->m_val = index;
    } else if ((is_while && stmt->cond->d.prim.d.num) ||
               (!is_while && !stmt->cond->d.prim.d.num)) {
      const Instr goto_ = emit_add_instr(emit, Goto);
      goto_->m_val      = index;
    }
  } else {
    const Instr goto_ = emit_add_instr(emit, Goto);
    goto_->m_val      = index;
    if (op) op->m_val = emit_code_size(emit);
  }
  return GW_OK;
}

ANN static m_bool emit_stmt_flow(const Emitter emit, const Stmt_Flow stmt) {
  const m_uint index = emit_code_size(emit);
  emit_push_stack(emit);
  const m_bool ret = _emit_stmt_flow(emit, stmt, index);
  emit_pop_stack(emit, index);
  return ret;
}

ANN static m_bool variadic_state(const Emitter emit, const Stmt_VarLoop stmt,
                                 const m_uint status) {
  regpushi(emit, status);
  CHECK_BB(emit_exp(emit, stmt->exp));
  const Instr member = emit_add_instr(emit, DotMember4);
  member->m_val      = SZ_INT;
  emit_add_instr(emit, int_r_assign);
  regpop(emit, SZ_INT);
  return GW_OK;
}

ANN static m_bool emit_stmt_varloop(const Emitter      emit,
                                    const Stmt_VarLoop stmt) {
  CHECK_BB(variadic_state(emit, stmt, 1));
  CHECK_BB(emit_exp(emit, stmt->exp));
  const Instr s       = emit_add_instr(emit, DotMember);
  s->m_val            = SZ_INT * 5;
  const Instr nonnull = emit_add_instr(emit, Goto);
  regpop(emit, SZ_INT);
  const Instr check  = emit_add_instr(emit, Goto);
  const Instr member = emit_add_instr(emit, DotMember4);
  member->m_val      = SZ_INT * 2;
  nonnull->m_val     = emit_code_size(emit);
  const Instr  instr = emit_add_instr(emit, BranchEqInt);
  const m_uint pc    = emit_code_size(emit);
  emit_stmt(emit, stmt->body, 1);
  CHECK_BB(emit_exp(emit, stmt->exp));
  emit_vararg_end(emit, pc);
  CHECK_BB(variadic_state(emit, stmt, 0));
  check->m_val = instr->m_val = emit_code_size(emit);
  return GW_OK;
}

ANN static m_bool _emit_stmt_for(const Emitter emit, const Stmt_For stmt,
                                 m_uint *action_index) {
  CHECK_BB(emit_stmt(emit, stmt->c1, 1));
  const m_uint index = emit_code_size(emit);
  DECL_OB(const Instr, op, = emit_flow(emit, stmt->c2->d.stmt_exp.val));
  CHECK_BB(scoped_stmt(emit, stmt->body, 1));
  *action_index = emit_code_size(emit);
  if (stmt->c3) {
    CHECK_BB(emit_exp(emit, stmt->c3));
    pop_exp(emit, stmt->c3);
  }
  const Instr _goto = emit_add_instr(emit, Goto);
  _goto->m_val      = index;
  op->m_val         = emit_code_size(emit);
  return GW_OK;
}

ANN static m_bool emit_stmt_for(const Emitter emit, const Stmt_For stmt) {
  emit_push_stack(emit);
  m_uint       action_index = 0;
  const m_bool ret          = _emit_stmt_for(emit, stmt, &action_index);
  emit_pop_stack(emit, action_index);
  return ret;
}

struct Looper;
typedef Instr (*f_looper_init)(const Emitter, const struct Looper *);
typedef void (*f_looper)(const Emitter, const struct Looper *);
struct Looper {
  const Stmt          stmt;
  /*const */ m_uint   offset;
  const m_uint        n;
  const f_looper_init roll;
  const f_looper      unroll;
};

ANN static inline m_bool roll(const Emitter emit, const struct Looper *loop) {
  const Instr instr = loop->roll(emit, loop);
  CHECK_BB(scoped_stmt(emit, loop->stmt, 1));
  instr->m_val = emit_code_size(emit) + 1; // pass after goto
  return GW_OK;
}

ANN static Instr stmt_each_roll(const Emitter emit, const struct Looper *loop) {
  const Instr instr = emit_add_instr(emit, AutoLoop);
  instr->m_val2     = loop->offset + SZ_INT;
  return instr;
}

ANN static inline void unroll_init(const Emitter emit, const m_uint n) {
  emit->info->unroll = 0;
  const Instr instr  = emit_add_instr(emit, MemSetImm);
  instr->m_val       = emit_local(emit, emit->gwion->type[et_int]);
  instr->m_val2      = n;
}

ANN static inline m_bool unroll_run(const Emitter        emit,
                                    const struct Looper *loop) {
  if (loop->unroll) loop->unroll(emit, loop);
  return emit_stmt(emit, loop->stmt, 1);
}

ANN static m_bool unroll(const Emitter emit, const struct Looper *loop) {
  const Instr gc     = scoped_ini(emit);
  const Instr unroll = emit_add_instr(emit, Unroll);
  unroll->m_val      = loop->offset;
  const m_uint start = emit_code_size(emit);
  CHECK_BB(unroll_run(emit, loop));
  const m_uint end = emit_code_size(emit);
  for (m_uint i = 1; i < loop->n; ++i) CHECK_BB(unroll_run(emit, loop));
  unroll->m_val2      = end - start;
  const Instr unroll2 = emit_add_instr(emit, Unroll2);
  unroll2->m_val      = (m_uint)unroll;
  scoped_end(emit, gc);
  return GW_OK;
}

ANN static void stmt_each_unroll(const Emitter        emit,
                                 const struct Looper *loop) {
  const Instr instr = emit_add_instr(emit, AutoLoop);
  instr->m_val      = loop->offset + SZ_INT * 2;
}

ANN static inline m_bool looper_run(const Emitter        emit,
                                    const struct Looper *loop) {
  return (!loop->n ? roll : unroll)(emit, loop);
}

ANN static m_bool _emit_stmt_each(const Emitter emit, const Stmt_Each stmt,
                                  m_uint *end_pc) {
  const uint n = emit->info->unroll;
  if (n) {
    unroll_init(emit, n);
    emit_local(emit, emit->gwion->type[et_int]);
  }

  CHECK_BB(emit_exp(emit, stmt->exp)); // add ref?
  regpop(emit, SZ_INT);
  const m_uint offset = emit_local(emit, emit->gwion->type[et_int]); // array?
  emit_local(emit, emit->gwion->type[et_int]);
  emit_local(emit, emit->gwion->type[et_int]);
  const Instr tomem     = emit_add_instr(emit, Reg2Mem);
  tomem->m_val          = offset;
  const Instr loop_idx  = emit_add_instr(emit, MemSetImm);
  loop_idx->m_val       = offset + SZ_INT;
  loop_idx->m_val2      = -1;
  stmt->v->from->offset = offset + SZ_INT * 2;
  emit_debug(emit, stmt->v);
  if (stmt->idx) stmt->idx->v->from->offset = offset + SZ_INT;
  if (n) {
    const Instr instr = emit_add_instr(emit, AutoUnrollInit);
    instr->m_val      = offset - SZ_INT;
  }
  const m_uint  ini_pc = emit_code_size(emit);
  struct Looper loop   = {.stmt   = stmt->body,
                        .offset = offset,
                        .n      = n,
                        .roll   = stmt_each_roll,
                        .unroll = stmt_each_unroll};
  if (n) loop.offset -= SZ_INT;
  CHECK_BB(looper_run(emit, &loop));
  *end_pc         = emit_code_size(emit);
  const Instr tgt = emit_add_instr(emit, Goto);
  tgt->m_val      = ini_pc;
  return GW_OK;
}

ANN static m_bool emit_stmt_each(const Emitter emit, const Stmt_Each stmt) {
  emit_push_stack(emit);
  m_uint       end_pc = 0;
  const m_bool ret    = _emit_stmt_each(emit, stmt, &end_pc);
  emit_pop_stack(emit, end_pc);
  return ret;
}

ANN static Instr stmt_loop_roll(const Emitter emit, const struct Looper *loop) {
  const Instr eq = emit_add_instr(emit, Repeat);
  eq->m_val2     = loop->offset;
  return eq;
}

ANN static Instr stmt_loop_roll_idx(const Emitter        emit,
                                    const struct Looper *loop) {
  const Instr instr = emit_add_instr(emit, RepeatIdx);
  instr->m_val2     = loop->offset;
  return instr;
}

ANN static m_bool _emit_stmt_loop(const Emitter emit, const Stmt_Loop stmt,
                                  m_uint *index) {
  const uint n = emit->info->unroll;
  if (n) unroll_init(emit, n);
  const m_uint offset = emit_local(emit, emit->gwion->type[et_int]);
  if (stmt->idx) {
    const Instr instr          = emit_add_instr(emit, MemSetImm);
    instr->m_val               = emit_local(emit, emit->gwion->type[et_int]);
    stmt->idx->v->from->offset = offset;
    const Instr idx            = emit_add_instr(emit, MemSetImm);
    idx->m_val                 = offset;
    idx->m_val2                = -1;
  }
  CHECK_BB(emit_exp_pop_next(emit, stmt->cond));
  regpop(emit, SZ_INT);
  const Instr tomem  = emit_add_instr(emit, Reg2Mem);
  tomem->m_val       = offset + (!stmt->idx ? 0 : SZ_INT);
  *index             = emit_code_size(emit);
  struct Looper loop = {.stmt   = stmt->body,
                        .offset = offset,
                        .n      = n,
                        .roll =
                            !stmt->idx ? stmt_loop_roll : stmt_loop_roll_idx};
  CHECK_BB(looper_run(emit, &loop));
  const Instr _goto = emit_add_instr(emit, Goto);
  _goto->m_val      = *index;
  return GW_OK;
}

ANN static m_bool emit_stmt_loop(const Emitter emit, const Stmt_Loop stmt) {
  emit_push_stack(emit);
  m_uint       index = 0;
  const m_bool ret   = _emit_stmt_loop(emit, stmt, &index);
  emit_pop_stack(emit, index);
  return ret;
}

ANN static m_bool emit_type_def(const Emitter emit, const Type_Def tdef) {
  if (tdef->when) CHECK_BB(emit_func_def(emit, tdef->when_def));
  return (!is_fptr(emit->gwion, tdef->type) && tdef->type->info->cdef)
             ? emit_class_def(emit, tdef->type->info->cdef)
             : GW_OK;
}

ANN static m_bool emit_enum_def(const Emitter emit NUSED, const Enum_Def edef) {
  LOOP_OPTIM
  for (m_uint i = 0; i < vector_size(&edef->values); ++i) {
    const Value v = (Value)vector_at(&edef->values, i);
    *(m_uint *)(v->from->owner->class_data + v->from->offset) = i;
  }
  set_tflag(edef->t, tflag_emit);
  return GW_OK;
}

ANN static m_bool emit_union_def(const Emitter emit NUSED,
                                 const Union_Def    udef) {
  if (tmpl_base(udef->tmpl)) return GW_OK;
  set_tflag(udef->type, tflag_emit);
  return GW_OK;
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

ANN static inline m_bool emit_handler_list(const restrict Emitter emit,
                                           const Handler_List     handler,
                                           const Vector           v) {
  const Instr instr = emit_add_instr(emit, HandleEffect);
  instr->m_val2     = (m_uint)handler->xid;
  CHECK_BB(scoped_stmt(emit, handler->stmt, 1));
  if (handler->next) CHECK_BB(emit_handler_list(emit, handler->next, v));
  emit_try_goto(emit, v);
  instr->m_val = emit_code_size(emit);
  return GW_OK;
}

ANN static inline m_bool emit_stmt_try(const restrict Emitter emit,
                                       const Stmt_Try         stmt) {
  const m_uint top           = emit->code->frame->try_top;
  emit->code->frame->try_top = emit_code_size(emit);
  (void)emit_add_instr(emit, TryIni);
  struct Vector_ v; // store Gotos to the happy path
  vector_init(&v);
  CHECK_BB(scoped_stmt(emit, stmt->stmt, 1));
  emit_try_goto(emit, &v);
  if (!emit->code->frame->handlers.ptr) map_init(&emit->code->frame->handlers);
  CHECK_BB(emit_handler_list(emit, stmt->handler, &v));
  try_goto_indexes(&v, emit_code_size(emit));
  vector_release(&v);
  emit->code->frame->try_top = top;
  (void)emit_add_instr(emit, TryEnd);
  return GW_OK;
}

ANN static m_bool emit_stmt_exp(const Emitter           emit,
                                const struct Stmt_Exp_ *exp) {
  return exp->val ? emit_exp(emit, exp->val) : GW_OK;
}

ANN static inline m_bool emit_exp1(const Emitter emit, const Exp e) {
  const Exp next   = e->next;
  e->next          = NULL;
  const m_bool ret = emit_exp(emit, e);
  e->next          = next;
  return ret;
}

ANN static m_bool emit_case_head(const Emitter emit, const Exp base,
                                 const Exp e, const Symbol op, const Vector v) {
  CHECK_BB(emit_exp1(emit, base));
  emit_exp_addref1(emit, base, -exp_size(base));
  CHECK_BB(emit_exp1(emit, e));
  emit_exp_addref1(emit, e, -exp_size(e));
  const Exp_Binary bin  = {.lhs = base, .rhs = e, .op = op};
  struct Exp_      ebin = {
      .d = {.exp_binary = bin},
  };
  struct Op_Import opi = {.op   = op,
                          .lhs  = base->type,
                          .rhs  = e->type,
                          .data = (uintptr_t)&ebin.d.exp_binary,
                          .pos  = e->pos};
  CHECK_BB(op_emit(emit, &opi));
  const Instr instr = emit_add_instr(emit, BranchEqInt);
  vector_add(v, (vtype)instr);
  return GW_OK;
}

ANN static m_bool emit_case_body(const Emitter             emit,
                                 const struct Stmt_Match_ *stmt) {
  const Instr when = stmt->when ? emit_flow(emit, stmt->when) : NULL;
  if (stmt->when) CHECK_OB(when);
  CHECK_BB(emit_stmt_list(emit, stmt->list));
  const Instr instr = emit_add_instr(emit, Goto);
  vector_add(&emit->env->scope->match->vec, (vtype)instr);
  if (when) when->m_val = emit_code_size(emit);
  return GW_OK;
}

ANN static m_bool case_value(const Emitter emit, const Exp base, const Exp e) {
  const Value v   = e->d.prim.value;
  v->from->offset = emit_local(emit, base->type);
  emit_debug(emit, v);
  const Instr instr = emit_add_instr(emit, Reg2Mem4);
  instr->m_val      = v->from->offset;
  instr->m_val2     = base->type->size;
  return GW_OK;
}

#define CASE_PASS (Symbol)1
ANN static Symbol case_op(const Emitter emit, const Exp base, const Exp e,
                          const Vector vec, const uint n) {
  if (e->exp_type == ae_exp_primary) {
    if (e->d.prim.prim_type == ae_prim_id) {
      if (e->d.prim.d.var == insert_symbol("_")) return CASE_PASS;
      if (!nspc_lookup_value1(emit->env->curr, e->d.prim.d.var)) {
        if (!n) {
          CHECK_BO(emit_exp(emit, base));
          emit_exp_addref(emit, base, -exp_totalsize(base));
          regpop(emit, base->type->size);
        }
        CHECK_BO(case_value(emit, base, e));
        return CASE_PASS;
      }
    }
  } else if (isa(base->type, emit->gwion->type[et_union]) > 0 &&
             e->exp_type == ae_exp_call) {
    const Exp func = e->d.exp_call.func;
    if (func->d.prim.prim_type == ae_prim_id) {
      const Map map = &base->type->nspc->info->value->map;
      for (m_uint i = 0; i < map_size(map); ++i) {
        if (VKEY(map, i) == (m_uint)func->d.prim.d.var) {
          const Value v = (Value)VVAL(map, i);
          if (v) {
            if (!n)
              CHECK_BO(emit_exp(emit, base));
            else
              regpush(emit, SZ_INT);
            const Instr check = emit_add_instr(emit, UnionCheck);
            check->m_val2     = i;
            vector_add(vec, (m_uint)check);
            const Instr instr =
                emit_unionmember(emit, v->type->size, false /*emit_addr*/);
            instr->m_val = i;
            regpop(emit, v->type->size);
            case_op(emit, e->d.exp_call.args, e->d.exp_call.args, vec, i + 1);
            return CASE_PASS;
          }
        }
      }
    }
  }
  if (!n) return insert_symbol("==");
  regpush(emit, SZ_INT);
  CHECK_BO(emit_exp(emit, e));
  const Exp_Binary bin  = {.lhs = base, .rhs = e, .op = insert_symbol("==")};
  struct Exp_      ebin = {
      .d = {.exp_binary = bin},
  };
  struct Op_Import opi = {.op   = insert_symbol("=="),
                          .lhs  = base->type,
                          .rhs  = e->type,
                          .data = (uintptr_t)&ebin.d.exp_binary,
                          .pos  = e->pos};
  CHECK_BO(op_emit(emit, &opi));
  const Instr instr = emit_add_instr(emit, BranchEqInt);
  vector_add(vec, (vtype)instr);
  return CASE_PASS;
}

ANN static m_bool _emit_stmt_match_case(const Emitter             emit,
                                        const struct Stmt_Match_ *stmt,
                                        const Vector              v) {
  Exp          e    = stmt->cond;
  const Vector cond = &emit->env->scope->match->cond;
  for (m_uint i = 0; i < vector_size(cond) && e; e = e->next, ++i) {
    const Exp    base = (Exp)vector_at(cond, i);
    const Symbol op   = case_op(emit, base, e, v, 0);
    if (op != CASE_PASS) CHECK_BB(emit_case_head(emit, base, e, op, v));
  }
  CHECK_BB(emit_case_body(emit, stmt));
  return GW_OK;
}

ANN static m_bool emit_stmt_match_case(const Emitter             emit,
                                       const struct Stmt_Match_ *stmt) {
  emit_push_scope(emit);
  struct Vector_ v;
  vector_init(&v);
  const m_bool ret = _emit_stmt_match_case(emit, stmt, &v);
  emit_pop_scope(emit);
  for (m_uint i = 0; i < vector_size(&v); ++i) {
    const Instr instr = (Instr)vector_at(&v, i);
    instr->m_val      = emit_code_size(emit);
  }
  vector_release(&v);
  return ret;
}

ANN static inline void match_unvec(struct Match_ *const match,
                                   const m_uint         pc) {
  const Vector vec = &match->vec;
  for (m_uint i = 0; i < vector_size(vec); ++i) {
    const Instr instr = (Instr)VPTR(vec, i);
    instr->m_val      = pc;
  }
  vector_release(vec);
}

ANN static m_bool emit_stmt_cases(const Emitter emit, Stmt_List list) {
  do CHECK_BB(emit_stmt_match_case(emit, &list->stmt->d.stmt_match));
  while ((list = list->next));
  return GW_OK;
}

ANN static m_bool emit_match(const Emitter             emit,
                             const struct Stmt_Match_ *stmt) {
  if (stmt->where) CHECK_BB(emit_stmt(emit, stmt->where, 1));
  MATCH_INI(emit->env->scope)
  vector_init(&m.vec);
  const m_bool ret = emit_stmt_cases(emit, stmt->list);
  match_unvec(&m, emit_code_size(emit));
  MATCH_END(emit->env->scope)
  return ret;
}

ANN static m_bool emit_stmt_match(const Emitter             emit,
                                  const struct Stmt_Match_ *stmt) {
  emit_push_scope(emit);
  const m_bool ret = emit_match(emit, stmt);
  emit_pop_scope(emit);
  return ret;
}

ANN static m_bool emit_stmt_pp(const Emitter          emit,
                               const struct Stmt_PP_ *stmt) {
  if (stmt->pp_type == ae_pp_pragma) {
    if (!strncmp(stmt->data, "unroll", strlen("unroll")))
      emit->info->unroll = strtol(stmt->data + 6, NULL, 10);
  } else if (stmt->pp_type == ae_pp_include)
    emit->env->name = stmt->data;
  return GW_OK;
}

ANN static m_bool emit_stmt_defer(const Emitter             emit,
                                  const struct Stmt_Defer_ *stmt) {
  vector_add(&emit->code->frame->defer, (m_uint)stmt->stmt);
  return GW_OK;
}

ANN static m_bool emit_stmt_retry(const Emitter                  emit,
                                  const struct Stmt_Index_ *stmt NUSED) {
  const Instr instr = emit_add_instr(emit, Goto);
  instr->m_val      = emit->code->frame->try_top + 1;
  return GW_OK;
}

#define emit_stmt_while emit_stmt_flow
#define emit_stmt_until emit_stmt_flow
DECL_STMT_FUNC(emit, m_bool, Emitter)

ANN static m_bool emit_stmt(const Emitter emit, const Stmt stmt,
                            const m_bool pop) {
  CHECK_BB(emit_stmt_func[stmt->stmt_type](emit, &stmt->d));
  if (pop && stmt->stmt_type == ae_stmt_exp && stmt->d.stmt_exp.val) {
    pop_exp(emit, stmt->d.stmt_exp.val);
  }
  return GW_OK;
}

ANN static m_bool emit_stmt_list(const Emitter emit, Stmt_List l) {
  do CHECK_BB(emit_stmt(emit, l->stmt, 1));
  while ((l = l->next));
  return GW_OK;
}

ANN static m_bool emit_exp_dot(const Emitter emit, const Exp_Dot *member) {
  struct Op_Import opi = {.op   = insert_symbol("@dot"),
                          .lhs  = member->base->type,
                          .rhs  = exp_self(member)->type,
                          .data = (uintptr_t)member,
                          .pos  = exp_self(member)->pos};
  return op_emit(emit, &opi);
}

ANN static inline void emit_func_def_fglobal(const Emitter emit,
                                             const Value   value) {
  const Instr set_mem = emit_add_instr(emit, MemSetImm);
  set_mem->m_val      = value->from->offset;
  set_mem->m_val2     = (m_uint)value->d.func_ref->code;
}

ANN static inline void emit_func_def_init(const Emitter emit, const Func func) {
  emit_push_code(emit, func->name);
}

ANN static void emit_func_def_args(const Emitter emit, Arg_List a) {
  do {
    const Type type = a->var_decl->value->type;
    emit->code->stack_depth += type->size;
    a->var_decl->value->from->offset = emit_localn(emit, type);
    emit_debug(emit, a->var_decl->value);
  } while ((a = a->next));
}

ANN static m_bool emit_func_def_return(const Emitter emit) {
  const m_uint val = emit_code_size(emit);
  CHECK_BB(emit_defers(emit));
  emit_return_pc(emit, val);
  vector_clear(&emit->code->stack_return);
  const Func f = emit->env->func;
  if (f->memoize && fflag(f, fflag_pure)) {
    const Instr instr = emit_add_instr(emit, MemoizeStore);
    instr->m_val      = f->def->stack_depth;
  }
  return GW_OK;
}

ANN static VM_Code emit_internal(const Emitter emit, const Func f) {
  if (f->def->base->xid == insert_symbol("@dtor")) {
    emit->env->class_def->nspc->dtor = f->code = finalyze(emit, DTOR_EOC);
    vmcode_addref(f->code);
    return f->code;
  } else if (f->def->base->xid == insert_symbol("@gack")) {
    regpop(emit, SZ_INT + f->value_ref->from->owner_class->size);
    const Instr instr                       = emit_add_instr(emit, RegPushMem);
    instr->m_val                            = SZ_INT;
    f->code                                 = finalyze(emit, FuncReturn);
    return emit->env->class_def->info->gack = f->code;
  }
  return finalyze(emit, FuncReturn);
}

ANN static inline VM_Code _emit_func_def_code(const Emitter emit,
                                              const Func    func) {
  if(!strcmp(s_name(func->def->base->xid), "new"))
    emit_add_instr(emit, RegPushMem);
  return !fbflag(func->def->base, fbflag_internal) ? finalyze(emit, FuncReturn)
                                                   : emit_internal(emit, func);
}

ANN static VM_Code emit_func_def_code(const Emitter emit, const Func func) {
  const VM_Code code = _emit_func_def_code(emit, func);
  if (func->memoize && fflag(func, fflag_pure)) code->is_memoize = true;
  code->ret_type = func->def->base->ret_type;
  return code;
}

ANN static m_bool emit_func_def_body(const Emitter emit, const Func_Def fdef) {
  if (fdef->base->args) emit_func_def_args(emit, fdef->base->args);
  if (fbflag(fdef->base, fbflag_variadic)) stack_alloc(emit);
  if (fdef->d.code) CHECK_BB(scoped_stmt(emit, fdef->d.code, 1));
  return GW_OK;
}

ANN static Instr me_top(MemoizeEmitter *me) {
  const Instr idx = emit_add_instr(me->emit, MemSetImm);
  idx->m_val      = me->offset;
  const Instr pc  = emit_add_instr(me->emit, MemSetImm);
  pc->m_val       = me->offset + SZ_INT;
  return pc;
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
  const Instr idx = emit_add_instr(me->emit, RegPushMem4);
  idx->m_val      = me->offset;
  emit_add_instr(me->emit, int_pre_inc);
  regpop(me->emit, SZ_INT);
  const Instr loop = emit_add_instr(me->emit, Goto);
  loop->m_val      = pc;
}

ANN static void me_ret(MemoizeEmitter *me) {
  const Instr instr =
      emit_regpushmem(me->emit, me->fdef->base->ret_type->size, false);
  instr->m_val = (me->offset + SZ_INT) * 2;
  emit_add_instr(me->emit, FuncReturn);
}

ANN static m_bool me_run(MemoizeEmitter *me, const m_uint pc) {
  me_ini(me);
  if (me->fdef->base->args) CHECK_BB(me_arg(me));
  me_ret(me);
  me_end(me, emit_code_size(me->emit));
  me_bottom(me, pc);
  return GW_OK;
}

ANN static m_bool emit_memoize(const Emitter emit, const Func_Def fdef) {
  MemoizeEmitter me = {.emit = emit, .fdef = fdef, .offset = fdef->stack_depth};
  const Instr    pc = me_top(&me);
  const m_uint   top = emit_code_size(emit);
  vector_init(&me.branch);
  const m_bool ret = me_run(&me, top);
  vector_release(&me.branch);
  pc->m_val2 = emit_code_size(emit);
  return ret;
}

ANN static m_bool emit_fdef(const Emitter emit, const Func_Def fdef) {
  const Func f = fdef->base->func;
  if (f->memoize && fflag(f, fflag_pure))
    CHECK_BB(emit_memoize(emit, fdef));
  CHECK_BB(emit_func_def_body(emit, fdef));
  emit_func_def_return(emit);
  return GW_OK;
}

ANN static void emit_fdef_finish(const Emitter emit, const Func_Def fdef) {
  const Func func = fdef->base->func;
  func->code      = emit_func_def_code(emit, func);
  if (func->memoize && fflag(func, fflag_pure))
    func->code->memoize = memoize_ini(emit, func);
}

ANN m_bool _emit_func_def(const Emitter emit, const Func_Def f) {
  if (tmpl_base(f->base->tmpl) && fbflag(f->base, fbflag_op)) return GW_OK;
  const Func     func   = f->base->func;
  const Func_Def fdef   = func->def;
  const Func     former = emit->env->func;
  if (func->code || tmpl_base(fdef->base->tmpl) || fflag(func, fflag_emit))
    return GW_OK;
  set_fflag(func, fflag_emit);
  if ((vflag(func->value_ref, vflag_builtin) &&
      safe_tflag(emit->env->class_def, tflag_tmpl)) || (fdef->base->tmpl && !strcmp(s_name(f->base->xid), "new"))) {
    const Func base =
        nspc_lookup_func1(func->value_ref->from->owner, f->base->xid);
    builtin_func(emit->gwion->mp, func, (f_xfun)base->code->native_func);
    return GW_OK;
  }
  const uint   global = GET_FLAG(f->base, global);
  const m_uint scope =
      !global ? emit->env->scope->depth : env_push_global(emit->env);
  emit_func_def_init(emit, func);
  if (vflag(func->value_ref, vflag_member)) stack_alloc(emit);
  emit->env->func = func;
  emit_push_scope(emit);
  if (!strcmp(s_name(fdef->base->xid), "@gack")) {
    emit_local(emit, emit->gwion->type[et_int]);
    const Instr instr = emit_add_instr(emit, MemSetImm);
    instr->m_val      = SZ_INT;
  }
  const m_bool ret = scanx_fdef(emit->env, emit, fdef, (_exp_func)emit_fdef);
  emit_pop_scope(emit);
  emit->env->func = former;
  if (ret > 0)
    emit_fdef_finish(emit, fdef);
  else
    emit_pop_code(emit);
  if (global) env_pop(emit->env, scope);
  return ret;
}

ANN m_bool emit_func_def(const Emitter emit, const Func_Def fdef) {
  const uint16_t depth = emit->env->scope->depth;
  emit->env->scope->depth = 0;
  const m_bool ret = _emit_func_def(emit, fdef);
  emit->env->scope->depth = depth;
  return ret;
}

ANN static m_bool emit_extend_def(const Emitter emit, const Extend_Def xdef);
#define emit_fptr_def  dummy_func
#define emit_trait_def dummy_func
HANDLE_SECTION_FUNC(emit, m_bool, Emitter)

ANN static inline m_bool emit_ast_inner(const Emitter emit, Ast ast) {
  do CHECK_BB(emit_section(emit, ast->section));
  while ((ast = ast->next));
  return emit_defers(emit);
}

ANN static m_bool emit_extend_def(const Emitter emit, const Extend_Def xdef) {
  CHECK_BB(ensure_emit(emit, xdef->t));
  CHECK_BB(extend_push(emit->env, xdef->t));
  const m_bool ret = emit_ast_inner(emit, xdef->body);
  extend_pop(emit->env, xdef->t);
  return ret;
}

ANN Code *emit_class_code(const Emitter emit, const m_str name) {
  const m_uint len = strlen(name) + 7;
  char         c[len];
  snprintf(c, len, "class %s", name);
  emit_push_code(emit, c);
  stack_alloc(emit);
  return emit->code;
}

ANN static m_bool emit_parent(const Emitter emit, const Class_Def cdef) {
  const Type parent = cdef->base.type->info->parent;
  return ensure_emit(emit, parent);
}

ANN /*static */ inline m_bool emit_cdef(const Emitter emit, const Type t) {
  return scanx_cdef(emit->env, emit, t, (_exp_func)emit_class_def,
                    (_exp_func)emit_union_def);
}

ANN static m_bool cdef_parent(const Emitter emit, const Class_Def cdef) {
  const bool tmpl = !!cdef->base.tmpl;
  if (tmpl) CHECK_BB(template_push_types(emit->env, cdef->base.tmpl));
  const m_bool ret = emit_parent(emit, cdef);
  if (tmpl) nspc_pop_type(emit->gwion->mp, emit->env->curr);
  return ret;
}

ANN static m_bool emit_class_def(const Emitter emit, const Class_Def cdef) {
  if (tmpl_base(cdef->base.tmpl)) return GW_OK;
  const Type      t = cdef->base.type;
  if (tflag(t, tflag_emit)) return GW_OK;
  set_tflag(t, tflag_emit);
  const Class_Def c = t->info->cdef;
  const Type owner = t->info->value->from->owner_class;
  if (owner) CHECK_BB(ensure_emit(emit, owner));
  if (c->base.ext && t->info->parent->info->cdef &&
      !tflag(t->info->parent, tflag_emit)) // ?????
    CHECK_BB(cdef_parent(emit, c));
  if (c->body) {
    emit_class_code(emit, t->name);
    if (scanx_body(emit->env, c, (_exp_func)emit_section, emit) > 0)
      t->nspc->pre_ctor = finalyze(emit, FuncReturn);
    else {
      free_code(emit->gwion->mp, emit->code);
      emit_pop_code(emit);
      return GW_ERROR;
    }
  }
  return GW_OK;
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
  vector_clear(&emit->info->pure);
  emit_free_code(emit, emit->code);
  return NULL;
}

ANN static inline void emit_clear(const Emitter emit) {
  emit->info->unroll  = 0;
  emit->info->line    = 0;
  emit->this_offset   = 0;
  emit->vararg_offset = 0;
}

ANN m_bool emit_ast(const Env env, Ast ast) {
  const Emitter emit  = env->gwion->emit;
  emit_clear(emit);
  emit->code          = new_code(emit, emit->env->name);
  emit_push_scope(emit);
  const m_bool ret = emit_ast_inner(emit, ast);
  emit_pop_scope(emit);
  if (ret > 0)
    emit->info->code = finalyze(emit, EOC);
  else
    emit_free_stack(emit);
  emit_clear(emit);
  return ret;
}
