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
#include "parser.h"
#include "specialid.h"
#include "vararg.h"

#undef insert_symbol
#define insert_symbol(a) insert_symbol(emit->gwion->st, (a))

#undef ERR_B
#define ERR_B(a, b, ...) { env_err(emit->env, (a), (b), ## __VA_ARGS__); return GW_ERROR; }
#undef ERR_O
#define ERR_O(a, b, ...) { env_err(emit->env, (a), (b), ## __VA_ARGS__); return NULL; }

typedef struct Local_ {
  Type type;
  m_uint offset;
} Local;

static inline void emit_pop(const Emitter emit, const m_uint scope) { env_pop(emit->env, scope); }
static inline m_uint emit_push(const Emitter emit, const Type type, const Nspc nspc) {
  return env_push(emit->env, type, nspc);
}
static inline m_uint emit_push_type(const Emitter emit, const Type type) {
  return env_push_type(emit->env, type);
}
static inline m_uint emit_push_global(const Emitter emit) {
  return env_push_global(emit->env);
}

ANEW static Frame* new_frame(MemPool p) {
  Frame* frame = mp_calloc(p, Frame);
  vector_init(&frame->stack);
  vector_add(&frame->stack, (vtype)NULL);
  return frame;
}

ANN static void free_frame(MemPool p, Frame* a) {
  LOOP_OPTIM
  for(vtype i = vector_size(&a->stack) + 1; --i;)
    if(vector_at(&a->stack, i - 1))
      mp_free(p, Local, (Local*)vector_at(&a->stack, i - 1));
  vector_release(&a->stack);
  mp_free(p, Frame, a);
}

ANN static Local* new_local(MemPool p, const Type type) {
  Local* local  = mp_calloc(p, Local);
  local->type   = type;
  return local;
}

ANN static m_uint frame_local(MemPool p, Frame* frame, const Type t) {
  Local* local = new_local(p, t);
  local->offset = frame->curr_offset;
  frame->curr_offset += t->size;
  vector_add(&frame->stack, (vtype)local);
  return local->offset;
}

ANN static inline void frame_push(Frame* frame) {
  vector_add(&frame->stack, (vtype)NULL);
}

ANN static void struct_pop(const Emitter emit, const Type type, const m_uint offset) {
  for(m_uint i = 0; i < vector_size(&type->e->tuple->types); ++i) {
    const Type t = (Type)vector_at(&type->e->tuple->types, i);
    if(isa(t, emit->gwion->type[et_object]) > 0) {
      const Instr instr = emit_add_instr(emit, ObjectRelease);
      instr->m_val = offset + vector_at(&type->e->tuple->offset, i);
    } else if(GET_FLAG(t, struct))
      struct_pop(emit, t, offset + vector_at(&type->e->tuple->offset, i));
  }
}

ANN static m_int frame_pop(const Emitter emit) {
  Frame *frame = emit->code->frame;
  DECL_OB(const Local*, l, = (Local*)vector_pop(&frame->stack))
  frame->curr_offset -= l->type->size;
  if(GET_FLAG(l->type, struct)) {
    struct_pop(emit, l->type, l->offset);
    return frame_pop(emit);
  }
  return isa(l->type, emit->gwion->type[et_object]) > 0 ? (m_int)l->offset : frame_pop(emit);
}

ANN /*static */m_bool emit_exp(const Emitter emit, Exp exp);
ANN static m_bool emit_stmt(const Emitter emit, const Stmt stmt, const m_bool pop);
ANN static m_bool emit_stmt_list(const Emitter emit, Stmt_List list);
ANN static m_bool emit_exp_dot(const Emitter emit, const Exp_Dot* member);
ANN static m_bool emit_func_def(const Emitter emit, const Func_Def func_def);

ANEW static Code* new_code(const Emitter emit, const m_str name) {
  Code* code = mp_calloc(emit->gwion->mp, Code);
  code->name = code_name_set(emit->gwion->mp, name, emit->env->name);
  vector_init(&code->instr);
  vector_init(&code->stack_break);
  vector_init(&code->stack_cont);
  vector_init(&code->stack_return);
  code->frame = new_frame(emit->gwion->mp);
  return code;
}

ANN static void free_code(MemPool p, Code* code) {
  vector_release(&code->instr);
  vector_release(&code->stack_break);
  vector_release(&code->stack_cont);
  vector_release(&code->stack_return);
  free_frame(p, code->frame);
  free_mstr(p, code->name);
  mp_free(p, Code, code);
}

ANN static void emit_pop_scope(const Emitter emit) {
  m_int offset;
  while((offset = frame_pop(emit)) > -1) {
    Instr instr = emit_add_instr(emit, ObjectRelease);
    instr->m_val = (m_uint)offset;
  }
  vector_pop(&emit->info->pure);
}

ANN static inline void emit_push_code(const Emitter emit, const m_str name) {
  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code(emit, name);
}

ANN static inline void emit_pop_code(const Emitter emit) {
  emit->code = (Code*)vector_pop(&emit->stack);
}

ANN static inline void emit_push_scope(const Emitter emit) {
  frame_push(emit->code->frame);
  vector_add(&emit->info->pure, 0);
}

ANN static inline m_uint emit_code_size(const Emitter emit) {
  return vector_size(&emit->code->instr);
}

ANN m_uint emit_code_offset(const Emitter emit) {
  return emit->code->frame->curr_offset;
}

ANN m_uint emit_local(const Emitter emit, const Type t) {
  return frame_local(emit->gwion->mp, emit->code->frame, t);
}

ANN void emit_ext_ctor(const Emitter emit, const Type t);

ANN static inline void maybe_ctor(const Emitter emit, const Type t) {
  if(!GET_FLAG(t, nonnull) && GET_FLAG(t, ctor))
    emit_ext_ctor(emit, t);
}

ANN static void emit_pre_ctor(const Emitter emit, const Type type) {
  if(type->e->parent)
    emit_pre_ctor(emit, type->e->parent);
  maybe_ctor(emit, type);
  if(GET_FLAG(type, typedef) && type->e->parent->array_depth)
    emit_array_extend(emit, type->e->parent, type->e->def->base.ext->array->exp);
}

#define regxxx(name, instr) \
ANN static inline Instr reg##name(const Emitter emit, const m_uint sz) { \
  const Instr instr = emit_add_instr(emit, Reg##instr); \
  instr->m_val = sz; \
  return instr; \
}
regxxx(pop, Pop)
regxxx(push, Push)
regxxx(pushi, PushImm)
regxxx(seti, SetImm)

ANN static void emit_pre_constructor_array(const Emitter emit, const Type type) {
  const m_uint start_index = emit_code_size(emit);
  const Instr top = emit_add_instr(emit, ArrayTop);
  top->m_val2 = (m_uint)type;
  emit_pre_ctor(emit, type);
  const Instr bottom = emit_add_instr(emit, ArrayBottom);
  regpop(emit, SZ_INT);
  const Instr pc = emit_add_instr(emit, Goto);
  pc->m_val = start_index;
  top->m_val = emit_code_size(emit);
  bottom->m_val = start_index;
  regpop(emit, SZ_INT*3);
  emit_add_instr(emit, ArrayPost);
}

ANN2(1) static m_bool extend_indices(const Emitter emit, Exp e, const m_uint depth) {
  if(e)
    CHECK_BB(emit_exp(emit, e))
  m_uint count = 0;
  while(e) {
    ++count;
    e = e->next;
  }
  for(m_uint i = count; i < depth; ++i)
    regpushi(emit, 0);
  return GW_OK;
}

ANEW ANN static ArrayInfo* new_arrayinfo(const Emitter emit, const Type t) {
  const Type base = array_base(t);
  ArrayInfo* info = mp_calloc(emit->gwion->mp, ArrayInfo);
  vector_init(&info->type);
  for(m_uint i = 1; i < t->array_depth; ++i)
    vector_add(&info->type, (vtype)array_type(emit->env, base, i));
  vector_add(&info->type, (vtype)t);
  info->depth = (m_int)t->array_depth;
  info->base = base;
  return info;
}

ANN static inline void arrayinfo_ctor(const Emitter emit, ArrayInfo *info) {
  const Type base = info->base;
// TODO: needed for coumpund?
  if(isa(base, emit->gwion->type[et_object]) > 0 && !GET_FLAG(base, abstract)) {
    emit_pre_constructor_array(emit, base);
    info->is_obj = 1;
  }
}

ANN2(1,2) static ArrayInfo* emit_array_extend_inner(const Emitter emit, const Type t, const Exp e) {
  CHECK_BO(extend_indices(emit, e, t->array_depth))
  ArrayInfo* info = new_arrayinfo(emit, t);
  const Instr alloc = emit_add_instr(emit, ArrayAlloc);
  alloc->m_val = (m_uint)info;
  arrayinfo_ctor(emit, info);
  return info;
}

ANN void emit_ext_ctor(const Emitter emit, const Type t) {
  const Instr cpy = emit_add_instr(emit, Reg2Reg);
  cpy->m_val2 = -SZ_INT;
  if(t->nspc->pre_ctor) {
    const Instr set_code = regseti(emit, (m_uint)t->nspc->pre_ctor);
    set_code->m_val2 = SZ_INT;
  } else {
    const Instr instr = emit_add_instr(emit, SetCtor);
    instr->m_val = (m_uint)t;
  }
  const m_uint offset = emit_code_offset(emit);
  const Instr regset = regseti(emit, offset);
  regset->m_val2 = SZ_INT *2;
  const Instr push = emit_add_instr(emit, RegPush);
  push->m_val = SZ_INT *2;
  const Instr prelude = emit_add_instr(emit, SetCode);
  prelude->m_val2 = 2;
  prelude->m_val = SZ_INT;
  emit_add_instr(emit, Reg2Mem);
  const Instr next = emit_add_instr(emit, Overflow);
  next->m_val2 = offset;
}

ANN m_bool emit_array_extend(const Emitter emit, const Type t, const Exp e) {
  CHECK_OB(emit_array_extend_inner(emit, t, e))
  emit_add_instr(emit, PopArrayClass);
  emit_add_instr(emit, RegAddRef);
  return GW_OK;
}

ANN static inline void emit_notpure(const Emitter emit) {
  ++VPTR(&emit->info->pure, VLEN(&emit->info->pure) - 1);
}

ANN2(1,2) m_bool emit_instantiate_object(const Emitter emit, const Type type,
      const Array_Sub array, const m_bool is_ref) {
  emit_notpure(emit);
  if(type->array_depth) {
    ArrayInfo* info = emit_array_extend_inner(emit, type, array ? array->exp : NULL);
    if(info)
      info->is_ref = is_ref;
    return info ? GW_OK : GW_ERROR;
  } else if(!is_ref) {
    const Instr instr = emit_add_instr(emit, ObjectInstantiate);
    instr->m_val2 = (m_uint)type;
    emit_pre_ctor(emit, type);
  }
  return GW_OK;
}

static inline enum Kind kindof(const m_uint size, const uint emit_var) {
  if(emit_var)
    return KIND_ADDR;
  return size == SZ_INT ? KIND_INT : size == SZ_FLOAT ? KIND_FLOAT : KIND_OTHER;
}

ANN /*static */Instr emit_kind(Emitter emit, const m_uint size, const uint addr, const f_instr func[]) {
  const enum Kind kind = kindof(size, addr);
  const Instr instr = emit_add_instr(emit, func[kind]);
  instr->m_val2 = size;
  return instr;
}

static const f_instr regpushimm[] = { RegPushImm, RegPushImm2, RegPushImm3, RegPushImm4 };
static const f_instr regpushmem[] = { RegPushMem, RegPushMem2, RegPushMem3, RegPushMem4 };
static const f_instr regpushbase[] = { RegPushBase, RegPushBase2, RegPushBase3, RegPushBase4 };
static const f_instr dotstatic[]  = { DotStatic, DotStatic2, DotStatic3, RegPushImm };
static const f_instr allocmember[]  = { RegPushImm, RegPushImm2, RegPushImm3, AllocMember4 };
static const f_instr allocword[]  = { AllocWord, AllocWord2, AllocWord3, RegPushMem4 };
static const f_instr structmember[]  = { StructMember, StructMemberFloat, StructMemberOther, StructMemberAddr };

ANN Exp symbol_owned_exp(const Gwion gwion, const Symbol *data);
ANN static m_bool emit_symbol_owned(const Emitter emit, const Symbol *data) {
  const Exp dot = symbol_owned_exp(emit->gwion, data);
  dot->info->nspc = prim_exp(data)->info->nspc;
  const m_bool ret = emit_exp_dot(emit, &dot->d.exp_dot);
  free_exp(emit->gwion->mp, dot);
  return ret;
}

ANN static m_bool emit_symbol_builtin(const Emitter emit, const Symbol *data) {
  const Value v = prim_self(data)->value;
  if(GET_FLAG(v, union)) {
    const m_uint size = v->type->size;
    const Instr instr = emit_kind(emit, size, exp_getvar(prim_exp(data)), dotstatic);
    instr->m_val = (m_uint)v->d.ptr;
  } else {
    const m_uint size = v->type->size;
    const Instr instr = emit_kind(emit, size, exp_getvar(prim_exp(data)), regpushimm);
    if(!exp_getvar(prim_exp(data)) && size == SZ_INT) {
      if(isa(v->type, emit->gwion->type[et_object]) > 0)
        instr->m_val = (m_uint)v->d.ptr;
      else if(v->d.ptr)
        instr->m_val = *(m_uint*)v->d.ptr;
    } else {
      assert(v->d.ptr); // instr->m_val = v->d.ptr;
      memcpy(&instr->m_val, v->d.ptr, v->type->size);
    }
    instr->m_val2 = size;
  }
  return GW_OK;
}

ANN static m_bool _emit_symbol(const Emitter emit, const Symbol *data) {
  const Value v = prim_self(data)->value;
  if(is_class(emit->gwion, v->type)) {
    regpushi(emit, (m_uint)actual_type(emit->gwion, v->type));
    return GW_OK;
  }
  if(v->from->owner_class)
    return emit_symbol_owned(emit, data);
  if(GET_FLAG(v, builtin) || GET_FLAG(v, union) || GET_FLAG(v, enum))
    return emit_symbol_builtin(emit, data);
  const m_uint size = v->type->size;
  const Instr instr = emit_kind(emit, size, exp_getvar(prim_exp(data)), !GET_FLAG(v, global) ? regpushmem : regpushbase);
  instr->m_val  = v->from->offset;
  return GW_OK;
}

ANN static m_bool emit_symbol(const Emitter emit, const Exp_Primary* prim) {
  return _emit_symbol(emit, &prim->d.var);
}

ANN static VM_Code finalyze(const Emitter emit, const f_instr exec) {
  emit_add_instr(emit, exec);
  const VM_Code code = emit->info->emit_code(emit);
  free_code(emit->gwion->mp, emit->code);
  emit->code = (Code*)vector_pop(&emit->stack);
  return code;
}

ANN static m_bool emit_prim_array(const Emitter emit, const Array_Sub *data) {
  Exp e = (*data)->exp;
  CHECK_BB(emit_exp(emit, e))
  m_uint count = 0;
  do ++count;
  while((e = e->next));
  const Type type = (*data)->type;
  regseti(emit, count);
  const Instr instr = emit_add_instr(emit, ArrayInit);
  instr->m_val = (m_uint)type;
  instr->m_val2 = type->array_depth == 1 ? array_base(type)->size : SZ_INT;
  emit_add_instr(emit, GcAdd);
  emit_notpure(emit);
  return GW_OK;
}

ANN static inline m_bool emit_exp_pop_next(const Emitter emit, Exp e);

ANN static m_bool emit_range(const Emitter emit, Range *range) {
  if(range->start)
    CHECK_BB(emit_exp_pop_next(emit, range->start))
  else
    regpushi(emit, 0);
  if(range->end)
    CHECK_BB(emit_exp_pop_next(emit, range->end))
  else
    regpushi(emit, -1);
  return GW_OK;
}

ANN static m_bool emit_prim_range(const Emitter emit, Range **data) {
  Range *range = *data;
  CHECK_BB(emit_range(emit, range))
  const Exp e = range->start ?: range->end;
  const Symbol sym = insert_symbol("@range");
  struct Op_Import opi = { .op=sym, .rhs=e->info->type,
    .pos=e->pos, .data=(uintptr_t)prim_exp(data), .op_type=op_exp };
  CHECK_OB(op_emit(emit, &opi))
  emit_add_instr(emit, GcAdd);
  return GW_OK;
}

ANN m_bool emit_array_access(const Emitter emit, struct ArrayAccessInfo *const info) {
  if(GET_FLAG(info->array.type, typedef)) {
    info->array.type = info->array.type->e->parent;
    return emit_array_access(emit, info);
  }
  // look mum no pos
  struct Op_Import opi = { .op=insert_symbol("@array"), .lhs=info->array.exp->info->type, .rhs=info->array.type,
    .data=(uintptr_t)info, .op_type=op_array };
  return op_emit(emit, &opi) != (Instr)GW_ERROR ? GW_OK : GW_ERROR;
}

ANN static m_bool emit_exp_array(const Emitter emit, const Exp_Array* array) {
  CHECK_BB(emit_exp(emit, array->base))
  const Exp e = exp_self(array);
  struct ArrayAccessInfo info = { *array->array, e->info->type, exp_getvar(e) };
  return emit_array_access(emit, &info);
}

ANN static m_bool emit_exp_slice(const Emitter emit, const Exp_Slice* range) {
  CHECK_BB(emit_exp(emit, range->base))
  CHECK_BB(emit_range(emit, range->range))
  const Symbol sym = insert_symbol("@slice");
  const Exp e = range->range->start ?: range->range->end;
  struct Op_Import opi = { .op=sym, .lhs=e->info->type, .rhs=range->base->info->type,
    .pos=e->pos, .data=(uintptr_t)exp_self(range), .op_type=op_exp };
  CHECK_OB(op_emit(emit, &opi))
  return GW_OK;
}

ANN static m_bool emit_prim_id(const Emitter emit, const Symbol *data) {
  struct SpecialId_ * spid = specialid_get(emit->gwion, *data);
  if(spid)
    return specialid_instr(emit, spid, prim_self(data)) ? GW_OK : GW_ERROR;
  return emit_symbol(emit, prim_self(data));
}

ANN static m_bool emit_prim_num(const Emitter emit, const m_uint *num) {
  regpushi(emit, *num);
  return GW_OK;
}

ANN static m_bool emit_prim_float(const Emitter emit, const m_float *fnum) {
  const Instr instr = emit_add_instr(emit, RegPushImm2);
  instr->f = *fnum;
  return GW_OK;
}

ANN static m_bool emit_prim_char(const Emitter emit, const m_str *str) {
  const char c = str2char(emit, *str, prim_pos(str));
  regpushi(emit, c);
  return GW_OK;
}

ANN static Instr emit_addref(const Emitter emit, const m_bool emit_var) {
  const f_instr exec = !emit_var ? RegAddRef : RegAddRefAddr;
  const Instr instr = emit_add_instr(emit, exec);
  instr->m_val = -SZ_INT;
  return instr;
}

ANN static m_bool emit_prim_str(const Emitter emit, const m_str *str) {
  char c[strlen(*str) + 1];
  if(strlen(*str)) {
    strcpy(c, *str);
    CHECK_BB(escape_str(emit, c, prim_pos(str)));
  } else c[0] = '\0';
  const Value v = prim_self(str)->value;
  const Symbol sym = insert_symbol(c);
  if(!v->d.ptr)
    v->d.ptr = (m_uint*)new_string2(emit->gwion, NULL, s_name(sym));
  regpushi(emit, (m_uint)v->d.ptr);
  emit_addref(emit, 0);
  return GW_OK;
}

#define emit_prim_nil     (void*)dummy_func

ANN static m_bool emit_prim_typeof(const Emitter emit, const Exp *exp) {
  const Exp e = *exp;
  if(!e->info->type->array_depth)
    regpushi(emit, (m_uint)(actual_type(emit->gwion, e->info->type)));
  else
    regpushi(emit, (m_uint)e->info->type);
  return GW_OK;
}

ANN static void interp_multi(const Emitter emit, const Exp e) {
  Var_Decl_List list = e->d.exp_decl.list;
  const int emit_var = exp_getvar(e);
  m_uint offset = 0;
  while((list = list->next))
    offset += !emit_var ? list->self->value->type->size : SZ_INT;
  if(offset)
    regpop(emit, offset);
}

ANN static void interp_size(const Emitter emit, const Type t) {
  const m_uint sz = isa(t, emit->gwion->type[et_compound]) < 0 ?
      t->size : SZ_INT;
  const Instr instr = regseti(emit, sz);
  instr->m_val2 = SZ_INT;
}

ANN static m_bool emit_interp(const Emitter emit, const Exp exp) {
  regpushi(emit, 0);
  Exp e = exp, next = NULL;
  do {
    next = e->next;
    e->next = NULL;
    if(emit_exp(emit, e) < 0) {
      e->next = next;
      return GW_ERROR;
    }
    if(e->exp_type == ae_exp_decl) // why only objects?
      interp_multi(emit, e);
    regseti(emit, (m_uint)e->info->type);
    interp_size(emit, e->info->type);
    const m_bool isobj = isa(e->info->type, emit->gwion->type[et_object]) > 0;
    if(isobj) {
      if(!GET_FLAG(e->info->type, force))
        emit_add_instr(emit, GackType);
    }
    const Instr instr = emit_add_instr(emit, Gack);
    instr->m_val = emit_code_offset(emit);
  } while((e = e->next = next));
  return GW_OK;
}

ANN static m_bool emit_prim_hack(const Emitter emit, const Exp *exp) {
  CHECK_BB(emit_interp(emit, *exp))
  if(!(emit->env->func && emit->env->func->def->base->xid == insert_symbol("@gack")))
    emit_add_instr(emit, GackEnd);
  else {
    const Instr instr = emit_add_instr(emit, Reg2Mem);
    instr->m_val = SZ_INT;
    instr->m_val2 = -SZ_INT;
  }
  return GW_OK;
}

ANN static m_bool emit_prim_interp(const Emitter emit, const Exp *exp) {
  const Exp e = *exp;
  CHECK_BB(emit_interp(emit, e))
  const Instr instr = emit_add_instr(emit, GackEnd);
  instr->m_val = 1;
  return GW_OK;
}

DECL_PRIM_FUNC(emit, m_bool , Emitter);
ANN static m_bool emit_prim(const Emitter emit, Exp_Primary *const prim) {
  return emit_prim_func[prim->prim_type](emit, &prim->d);
}

ANN static m_bool emit_dot_static_data(const Emitter emit, const Value v, const uint emit_var) {
  const m_uint size = v->type->size;
  const Instr instr = emit_kind(emit, size, emit_var, dotstatic);
  instr->m_val = (m_uint)(v->from->owner->info->class_data + v->from->offset);
  instr->m_val2 = size;
  return GW_OK;
}

ANN static m_bool decl_static(const Emitter emit, const Var_Decl var_decl, const uint is_ref) {
  const Value v = var_decl->value;
  Code* code = emit->code;
  emit->code = (Code*)vector_back(&emit->stack);
  CHECK_BB(emit_instantiate_object(emit, v->type, var_decl->array, is_ref))
  CHECK_BB(emit_dot_static_data(emit, v, 1))
  emit_add_instr(emit, Assign);
  (void)emit_addref(emit, 0);
  regpop(emit, SZ_INT);
  emit->code = code;
  return GW_OK;
}

ANN static inline int struct_ctor(const Value v) {
  return GET_FLAG(v->type, struct);
}

ANN static void decl_expand(const Emitter emit, const Type t) {
  const Instr instr = emit_add_instr(emit, Reg2RegDeref);
  instr->m_val = -SZ_INT;
  instr->m_val2 = t->size;
  regpush(emit, t->size - SZ_INT);
}

ANN static void emit_struct_decl_finish(const Emitter emit, const Type t, const uint emit_addr) {
  emit->code->frame->curr_offset += t->size + SZ_INT;
  emit_ext_ctor(emit, t);
  if(!emit_addr)
    decl_expand(emit, t);
  emit->code->frame->curr_offset -= t->size + SZ_INT;
}

ANN static m_bool emit_exp_decl_static(const Emitter emit, const Var_Decl var_decl, const uint is_ref, const uint emit_addr) {
  const Value v = var_decl->value;
  if(isa(v->type, emit->gwion->type[et_object]) > 0 && !is_ref)
    CHECK_BB(decl_static(emit, var_decl, 0))
  CHECK_BB(emit_dot_static_data(emit, v, !struct_ctor(v) ? emit_addr : 1))
  if(struct_ctor(v) /* && !GET_FLAG(decl->td, ref) */)
    emit_struct_decl_finish(emit, v->type, emit_addr);
  return GW_OK;
}

ANN static Instr emit_struct_decl(const Emitter emit, const Value v, const m_bool emit_addr) {
  emit_add_instr(emit, RegPushMem);
  const Instr instr = emit_kind(emit, v->type->size, emit_addr, structmember);
  if(!emit_addr)
    regpush(emit, v->type->size - SZ_INT);
  return instr;
}

ANN static m_bool emit_exp_decl_non_static(const Emitter emit, const Exp_Decl *decl, const Var_Decl var_decl,
  const uint is_ref, const uint emit_var) {
  const Value v = var_decl->value;
  const Type type = v->type;
  const Array_Sub array = var_decl->array;
  const m_bool is_array = (array && array->exp) /*|| GET_FLAG(decl->td, force)*/;
  const m_bool is_obj = isa(type, emit->gwion->type[et_object]) > 0;
  const uint emit_addr = (!is_obj || (is_ref && !is_array)) ? emit_var : 1;
  if(is_obj && (is_array || !is_ref) && !GET_FLAG(v, ref))
    CHECK_BB(emit_instantiate_object(emit, type, array, is_ref))
  f_instr *exec = (f_instr*)allocmember;
  if(!GET_FLAG(v, member)) {
    v->from->offset = emit_local(emit, type);
    exec = (f_instr*)(allocword);
    if(GET_FLAG(v, ref)) { // ref or emit_var ?
      const Instr clean = emit_add_instr(emit, MemSetImm);
      clean->m_val = v->from->offset;
    }
  }
  const Instr instr = !(SAFE_FLAG(emit->env->class_def, struct) && !emit->env->scope->depth) ?
    emit_kind(emit, v->type->size, !struct_ctor(v) ? emit_addr : 1, exec) : emit_struct_decl(emit, v, !struct_ctor(v) ? emit_addr : 1);
  instr->m_val = v->from->offset;
  if(is_obj && (is_array || !is_ref) && !GET_FLAG(v, ref)) {
    emit_add_instr(emit, Assign);
    const size_t missing_depth = type->array_depth - (array ? array->depth : 0);
    if(missing_depth && !GET_FLAG(decl->td, force)) {
      const Instr push = emit_add_instr(emit, Reg2Reg);
      push->m_val = -(missing_depth) * SZ_INT;
    }
  } else if(struct_ctor(v) /* && !GET_FLAG(decl->td, ref) */)
    emit_struct_decl_finish(emit, v->type, emit_addr);
  return GW_OK;
}

ANN static m_bool emit_exp_decl_global(const Emitter emit, const Exp_Decl *decl, const Var_Decl var_decl,
  const uint is_ref, const uint emit_var) {
  const Value v = var_decl->value;
  const Type type = v->type;
  const Array_Sub array = var_decl->array;
  const m_bool is_array = array && array->exp;
  const m_bool is_obj = isa(type, emit->gwion->type[et_object]) > 0;
  const uint emit_addr = (!is_obj || (is_ref && !is_array)) ? emit_var : 1;
  if(is_obj && (is_array || !is_ref) && !GET_FLAG(v, ref))
    CHECK_BB(emit_instantiate_object(emit, type, array, is_ref))
  const Instr instr = emit_kind(emit, v->type->size, !struct_ctor(v) ? emit_addr : 1, dotstatic);
  v->d.ptr = mp_calloc2(emit->gwion->mp, v->type->size);
  SET_FLAG(v, union);
  instr->m_val = (m_uint)v->d.ptr;
  instr->m_val2 = v->type->size;
  if(is_obj && (is_array || !is_ref) && !GET_FLAG(v, ref)) {
    const Instr assign = emit_add_instr(emit, Assign);
    const size_t missing_depth = type->array_depth - (array ? array->depth : 0);
    if(missing_depth && !GET_FLAG(decl->td, force)) {
      const Instr push = emit_add_instr(emit, Reg2Reg);
      push->m_val = -(missing_depth) * SZ_INT;
    }
    assign->m_val = emit_var;
    (void)emit_addref(emit, emit_var);
  } else if(struct_ctor(v) /* && !GET_FLAG(decl->td, ref) */)
    emit_struct_decl_finish(emit, v->type, emit_addr);
  return GW_OK;
}

ANN static m_bool emit_class_def(const Emitter, const Class_Def);
ANN static m_bool emit_cdef(const Emitter, const Class_Def);

ANN static inline m_bool ensure_emit(const Emitter emit, const Type t) {
  struct EnvSet es = { .env=emit->env, .data=emit, .func=(_exp_func)emit_cdef,
    .scope=emit->env->scope->depth, .flag=ae_flag_emit };
  return envset_run(&es, t);
}

ANN static m_bool emit_decl(const Emitter emit, const Exp_Decl* decl) {
  const m_bool global = GET_FLAG(decl->td, global);
  const uint var = exp_getvar(exp_self(decl));
  const uint ref = GET_FLAG(decl->td, ref) || type_ref(decl->type);
  Var_Decl_List list = decl->list;
  do {
    const uint r = GET_FLAG(list->self->value, ref) + ref;
    if(GET_FLAG(decl->td, static))
      CHECK_BB(emit_exp_decl_static(emit, list->self, r, var))
    else if(!global)
      CHECK_BB(emit_exp_decl_non_static(emit, decl, list->self, r, var))
    else
      CHECK_BB(emit_exp_decl_global(emit, decl, list->self, r, var))
    if(GET_FLAG(list->self->value->type, nonnull))
      emit_add_instr(emit, GWOP_EXCEPT);
  } while((list = list->next));
  return GW_OK;
}

ANN /*static */m_bool emit_exp_decl(const Emitter emit, const Exp_Decl* decl) {
  const Type t = get_type(decl->type);
  if(!GET_FLAG(t, emit) && t->e->def)
    CHECK_BB(ensure_emit(emit, t))
  const m_bool global = GET_FLAG(decl->td, global);
  const m_uint scope = !global ? emit->env->scope->depth : emit_push_global(emit);
  const m_bool ret = emit_decl(emit, decl);
  if(global)
    emit_pop(emit, scope);
  return ret;
}

ANN static m_uint vararg_size(const Exp_Call* exp_call, const Vector kinds) {
  Exp e = exp_call->args;
  Arg_List l = exp_call->m_func->def->base->args;
  m_uint size = 0;
  while(e) {
    if(!l) {
      size += e->info->type->size;
      vector_add(kinds, (vtype)e->info->type); // ->size
    } else
      l = l->next;
    e = e->next;
  }
  return size;
}

ANN static void emit_func_arg_vararg(const Emitter emit, const Exp_Call* exp_call) {
  const Instr instr = emit_add_instr(emit, VarargIni);
  const Vector kinds = new_vector(emit->gwion->mp);
  if((instr->m_val = vararg_size(exp_call, kinds)))
    instr->m_val2 = (m_uint)kinds;
  else
    free_vector(emit->gwion->mp, kinds);
}

ANN static inline m_uint exp_size(Exp e);
ANN static inline m_uint exp_totalsize(Exp e) {
  m_uint size = 0;
  do size += exp_size(e);
  while((e = e->next));
  return size;
}
ANN static /*inline */void emit_exp_addref1(const Emitter emit, Exp, const m_int size);
ANN static /*inline */void emit_exp_addref(const Emitter emit, Exp, const m_int size);

ANN static m_bool emit_func_args(const Emitter emit, const Exp_Call* exp_call) {
  if(exp_call->args) {
    CHECK_BB(emit_exp(emit, exp_call->args))
    emit_exp_addref(emit, exp_call->args, -exp_totalsize(exp_call->args));
  }
  if(exp_call->m_func && GET_FLAG(exp_call->m_func->def, variadic))
    emit_func_arg_vararg(emit, exp_call);
  return GW_OK;
}

ANN static m_bool prepare_call(const Emitter emit, const Exp_Call* exp_call) {
  CHECK_BB(emit_func_args(emit, exp_call))
  return emit_exp(emit, exp_call->func);
}

ANN static m_bool emit_exp_call(const Emitter emit, const Exp_Call* exp_call) {
  CHECK_BB(prepare_call(emit, exp_call))
  if(exp_call->m_func)
    CHECK_OB(emit_exp_call1(emit, exp_call->m_func))
  else {
    struct Op_Import opi = { .op=insert_symbol("@ctor"), .rhs=exp_call->func->info->type->e->d.base_type,
      .data=(uintptr_t)exp_call, .pos=exp_self(exp_call)->pos, .op_type=op_exp };
    CHECK_OB(op_emit(emit, &opi))
  }
  const Exp e = exp_self(exp_call);
  if(exp_getvar(e)) {
    regpop(emit, exp_self(exp_call)->info->type->size - SZ_INT);
    const Instr instr = emit_add_instr(emit, Reg2RegAddr);
    instr->m_val = -SZ_INT;
    instr->m_val2 = -SZ_INT;
  } else if(!exp_call->m_func && GET_FLAG(e->info->type, struct))
    regpop(emit, SZ_INT);
  return GW_OK;
}

ANN static m_uint get_decl_size(Var_Decl_List a, uint emit_addr) {
  m_uint size = 0;
  do //if(GET_FLAG(a->self->value, used))
    size += !emit_addr ? a->self->value->type->size : SZ_INT;
  while((a = a->next));
  return size;
}

ANN static m_uint pop_exp_size(Exp e) {
  const uint emit_addr = exp_getvar(e);
  m_uint size = 0;
  do { // account for emit_var ?
    size += (e->exp_type == ae_exp_decl ?
        get_decl_size(e->d.exp_decl.list, emit_addr) : e->info->type->size);
  } while((e = e->next));
  return size;
}

ANN static inline void pop_exp(const Emitter emit, Exp e) {
  const m_uint size = pop_exp_size(e);
  if(size)
   regpop(emit, size);
}

ANN static inline m_bool emit_exp_pop_next(const Emitter emit, Exp e) {
  CHECK_BB(emit_exp(emit, e))
  if(e->exp_type == ae_exp_decl) {
    Var_Decl_List list = e->d.exp_decl.list->next;
    while(list) {
      regpop(emit, !exp_getvar(e) ? list->self->value->type->size : SZ_INT);
      list = list->next;
    }
  }
  if(e->next)
    pop_exp(emit, e->next);
  return GW_OK;
}

ANN static inline m_bool op_emit_bool(const Emitter emit, const struct Op_Import* opi) {
  DECL_OB(const Instr, instr, = op_emit(emit, opi))
  return GW_OK;
}

ANN static m_bool emit_exp_binary(const Emitter emit, const Exp_Binary* bin) {
  const Exp lhs = bin->lhs;
  const Exp rhs = bin->rhs;
  struct Op_Import opi = { .op=bin->op, .lhs=lhs->info->type, .rhs=rhs->info->type,
    .pos=exp_self(bin)->pos, .data=(uintptr_t)bin, .op_type=op_binary };
  CHECK_BB(emit_exp_pop_next(emit, lhs))
  CHECK_BB(emit_exp_pop_next(emit, rhs))
  const m_int size = exp_size(rhs);
  emit_exp_addref1(emit, lhs, -exp_size(lhs) - size);
  emit_exp_addref1(emit, rhs, -size);
  return op_emit_bool(emit, &opi);
}

ANN static m_bool emit_exp_cast(const Emitter emit, const Exp_Cast* cast) {
  // no pos ?
  struct Op_Import opi = { .op=insert_symbol("$"), .lhs=cast->exp->info->type, .rhs=exp_self(cast)->info->type,
    .data=(uintptr_t)cast, .op_type=op_cast};
  CHECK_BB(emit_exp(emit, cast->exp))
  (void)op_emit(emit, &opi);
  return GW_OK;
}

ANN static m_bool emit_exp_post(const Emitter emit, const Exp_Postfix* post) {
  // no pos ?
  struct Op_Import opi = { .op=post->op, .lhs=post->exp->info->type,
    .data=(uintptr_t)post, .op_type=op_postfix };
  CHECK_BB(emit_exp(emit, post->exp))
  emit_exp_addref(emit, post->exp, -exp_totalsize(post->exp));
  return op_emit_bool(emit, &opi);
}

ANN static inline m_bool is_special(const Emitter emit, const Type t) {
  return isa(t, emit->gwion->type[et_object]) < 0 &&
         isa(t, emit->gwion->type[et_class]) < 0  ?
    GW_OK : GW_ERROR;
}

ANN static inline m_bool traverse_emit_func_def(const Emitter emit, const Func_Def fdef) {
  if(!fdef->base->ret_type)
    CHECK_BB(traverse_func_def(emit->env, fdef))
  return emit_func_def(emit, fdef);
}

ANN m_bool traverse_dot_tmpl(const Emitter emit, const struct dottmpl_ *dt) {
  const m_uint scope = emit->env->scope->depth;
  struct EnvSet es = { .env=emit->env, .data=emit, .func=(_exp_func)emit_cdef,
    .scope=scope, .flag=ae_flag_emit };
  CHECK_BB(envset_push(&es, dt->owner_class, dt->owner))
  (void)emit_push(emit, dt->owner_class, dt->owner);
  const m_bool ret = traverse_emit_func_def(emit, dt->def);
  if(es.run)
    envset_pop(&es, dt->owner_class);
  emit_pop(emit, scope);
  return ret;
}

static inline m_bool push_func_code(const Emitter emit, const Func f) {
  if(!vector_size(&emit->code->instr))
    return GW_OK;
  const Instr instr = (Instr)vector_back(&emit->code->instr);
  if(instr->opcode == eDotTmplVal) {
    size_t len = strlen(f->name);
    size_t sz = len - strlen(f->value_ref->from->owner_class->name);
    char c[sz + 1];
    memcpy(c, f->name, sz);
    c[sz] = '\0';
    struct dottmpl_ *dt = mp_calloc(emit->gwion->mp, dottmpl);
    dt->name = s_name(insert_symbol(c));
    dt->vt_index = f->def->base->tmpl->base;
    dt->tl = cpy_type_list(emit->gwion->mp, f->def->base->tmpl->call);
    dt->base = f->def;
    instr->opcode = eOP_MAX;
    instr->m_val = (m_uint)dt;
    instr->m_val2 = strlen(c);
    instr->execute = DotTmpl;
    return GW_OK;
  }
  instr->opcode = eRegPushImm;
  instr->m_val = (m_uint)f->code;
  return GW_OK;
}

ANN static m_bool emit_template_code(const Emitter emit, const Func f) {
  const Value v = f->value_ref;
  const size_t scope = emit->env->scope->depth;
  struct EnvSet es = { .env=emit->env, .data=emit, .func=(_exp_func)emit_cdef,
    .scope=scope, .flag=ae_flag_emit };
  CHECK_BB(envset_push(&es, v->from->owner_class, v->from->owner))
  (void)emit_push(emit, v->from->owner_class, v->from->owner);
  const m_bool ret = emit_func_def(emit, f->def);
  if(es.run)
    envset_pop(&es, v->from->owner_class);
  emit_pop(emit, scope);
  return ret > 0 ? push_func_code(emit, f) : GW_ERROR;
}

ANN static void tmpl_prelude(const Emitter emit, const Func f) {
  struct dottmpl_ *dt = (struct dottmpl_*)mp_calloc(emit->gwion->mp, dottmpl);
  size_t len = strlen(f->name);
  size_t slen = strlen(f->value_ref->from->owner->name);
  assert(len > slen);
  size_t sz = len - slen;
  char c[sz + 1];
  memcpy(c, f->name, sz);
  c[sz] = '\0';
  dt->tl = cpy_type_list(emit->gwion->mp, f->def->base->tmpl->call);
  dt->name = s_name(insert_symbol(c));
  dt->vt_index = f->def->base->tmpl->base;
  dt->base = f->def;
  dt->owner = f->value_ref->from->owner;
  dt->owner_class = f->value_ref->from->owner_class;
  const Instr gtmpl = emit_add_instr(emit, GTmpl);
  gtmpl->m_val = (m_uint)dt;
  gtmpl->m_val2 = strlen(c);
}

ANN static Instr get_prelude(const Emitter emit, const Func f) {
  const Type t = actual_type(emit->gwion, f->value_ref->type);
  if(is_fptr(emit->gwion, t)) {
    emit_except(emit, t);
    if(f->def->base->tmpl)
      tmpl_prelude(emit, f);
  }
  const Instr instr = emit_add_instr(emit, SetCode);
  instr->m_val2 = 1;
  return instr;
}

ANN static void emit_args(const Emitter emit, const Func f) {
  const m_uint member = GET_FLAG(f, member) ? SZ_INT : 0;
  if((f->def->stack_depth - member) == SZ_INT) {
    const Instr instr = emit_add_instr(emit, Reg2Mem);
    instr->m_val = member;
  } else {
    const Instr instr = emit_add_instr(emit, Reg2Mem4);
    instr->m_val = member;
    instr->m_val2 = f->def->stack_depth - member;
  }
}

typedef struct {
  const Emitter  emit;
  const Func_Def fdef;
  struct Vector_   branch;
  const m_uint   offset;
  m_uint         arg_offset;
} MemoizeEmitter;

ANN static Instr me_push(const MemoizeEmitter *me, const m_uint sz) {
  const Instr instr = emit_kind(me->emit, sz, 0, regpushmem);
  instr->m_val  = me->arg_offset;
  return instr;
}

static m_bool me_cmp(MemoizeEmitter *me, const Arg_List arg) {
  const Emitter emit = me->emit;
  const Symbol sym = insert_symbol("==");
  struct ExpInfo_ info = { .nspc=me->fdef->base->func->value_ref->from->owner };
  struct Exp_ exp = { .info=&info };
  struct Op_Import opi = { .op=sym, .lhs=arg->type, .rhs=arg->type,
    .pos=me->fdef->pos, .data=(uintptr_t)&exp.d, .op_type=op_binary };
  CHECK_BB(op_emit_bool(emit, &opi))
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
    instr->m_val  += me->offset + SZ_INT *2;
    CHECK_BB(me_cmp(me, arg))
    me->arg_offset += arg->type->size;
  } while((arg = arg->next));
  return GW_OK;
}

ANN static Instr emit_call(const Emitter emit, const Func f) {
  const Instr prelude = get_prelude(emit, f);
  prelude->m_val = f->def->stack_depth;
  const m_uint member = GET_FLAG(f, member) ? SZ_INT : 0;
  if(member) {
    const Instr instr = emit_add_instr(emit, Reg2Mem);
    instr->m_val2 = f->def->stack_depth - SZ_INT;
    ++prelude->m_val2;
  }
  if(f->def->stack_depth - member) {
    emit_args(emit, f);
    ++prelude->m_val2;
  }
  return emit_add_instr(emit, Overflow);
}

ANN Instr emit_exp_call1(const Emitter emit, const Func f) {
  if(!f->code || (GET_FLAG(f, ref) && !GET_FLAG(f, builtin))) {
    if(GET_FLAG(f, template) && !is_fptr(emit->gwion, f->value_ref->type)) {
      if(emit->env->func != f)
        CHECK_BO(emit_template_code(emit, f))
      else { // recursive function. (maybe should be used only for global funcs)
        const Instr back = (Instr) vector_size(&emit->code->instr) ?
            (Instr)vector_back(&emit->code->instr) : emit_add_instr(emit, RegPushImm);
        back->opcode = eOP_MAX;
        back->execute = SetRecurs;
        back->m_val = 0;
      }
    } else if(emit->env->func != f && !f->value_ref->from->owner_class && !f->code && !is_fptr(emit->gwion, f->value_ref->type)) {
      if(GET_FLAG(f->def, op)) {
        const Instr back = (Instr)vector_back(&emit->code->instr);
        back->m_val = (m_uint)f;
      } else {
// ensure env?	
        CHECK_BO(emit_func_def(emit, f->def))
        const Instr instr = emit_add_instr(emit, RegSetImm);
        instr->m_val = (m_uint)f->code;
        instr->m_val2 = -SZ_INT;
      }
    }
  } else if((f->value_ref->from->owner_class && is_special(emit, f->value_ref->from->owner_class) > 0) ||
        !f->value_ref->from->owner_class || (GET_FLAG(f, template) &&
        !is_fptr(emit->gwion, f->value_ref->type)))
    push_func_code(emit, f);
  else if(vector_size(&emit->code->instr)) {
    const Instr back = (Instr)vector_back(&emit->code->instr);
    if((f_instr)(m_uint)back->opcode == DotFunc || (f_instr)(m_uint)back->opcode == DotStaticFunc)
      back->m_val = f->vt_index;
  }
  if(vector_size(&emit->code->instr) && GET_FLAG(f, member) &&
        is_fptr(emit->gwion, f->value_ref->type)) {
    const Instr back = (Instr)vector_back(&emit->code->instr);
    m_bit exec = back->opcode;
    m_uint val = back->m_val;
    m_uint val2 = back->m_val2;
    back->opcode = eRegPushMem;
    back->m_val = back->m_val2 = 0;
    const Instr instr = emit_add_instr(emit, (f_instr)(m_uint)exec);
    instr->m_val = val;
    instr->m_val2 = val2;
  } else if(f != emit->env->func && !f->code && !is_fptr(emit->gwion, f->value_ref->type)){
    // not yet emitted static func
    if(f->value_ref->from->owner_class) {
      const Instr instr = vector_size(&emit->code->instr) ?
        (Instr)vector_back(&emit->code->instr) : emit_add_instr(emit, SetFunc);
      instr->opcode = eOP_MAX;
      instr->execute = SetFunc;
      instr->m_val = (m_uint)f;
    } else {
      const Instr instr = emit_add_instr(emit, SetFunc);
      instr->m_val = (m_uint)f;
    }
  }
  const m_uint offset = emit_code_offset(emit);
  regseti(emit, offset);
  const Instr instr = emit_call(emit, f);
  instr->m_val = f->def->base->ret_type->size;
  instr->m_val2 = offset;
  return instr;
}

ANN static void emit_exp_spork_finish(const Emitter emit, const m_uint depth) {
  regpop(emit, depth);
  const Instr spork = emit_add_instr(emit, SporkFunc);
  spork->m_val = depth + SZ_INT;
  spork->m_val2 = -SZ_INT;
}

static inline void stack_alloc(const Emitter emit) { // maybe vararg could use t_vararg instead
  emit_local(emit, emit->gwion->type[et_int]); // hiding the fact it is an object
  emit->code->stack_depth += SZ_INT;
}

static inline void stack_alloc_this(const Emitter emit) {
  SET_FLAG(emit->code, member);
  stack_alloc(emit);
}

static m_bool scoped_stmt(const Emitter emit, const Stmt stmt, const m_bool pop) {
  ++emit->env->scope->depth;
  emit_push_scope(emit);
  const Instr gc = emit_add_instr(emit, NoOp);
  const m_bool ret = emit_stmt(emit, stmt, pop);
  if(ret > 0) {
    const m_bool pure = !vector_back(&emit->info->pure);
    if(!pure) {
      gc->opcode = eGcIni;
      emit_add_instr(emit, GcEnd);
    }
  }
  emit_pop_scope(emit);
  --emit->env->scope->depth;
  return ret;
}

#define SPORK_FUNC_PREFIX "spork~func:%i"
#define FORK_FUNC_PREFIX "fork~func:%i"
#define SPORK_CODE_PREFIX "spork~code:%i"
#define FORK_CODE_PREFIX  "fork~code:%i"

static void push_spork_code(const Emitter emit, const m_str prefix, const loc_t pos) {
  char c[strlen(SPORK_FUNC_PREFIX) + num_digit(pos->first.line) + 1];
  sprintf(c, prefix, pos->first.line);
  emit_push_code(emit, c);
}

ANN static m_bool call_spork_func(const Emitter emit, const Exp_Call *exp) {
  if(GET_FLAG(exp->m_func, member))
    SET_FLAG(emit->code, member);
  return emit_exp_call1(emit, exp->m_func) ? GW_OK : GW_ERROR;
}

struct Sporker {
  const Stmt code;
  const Exp  exp;
  VM_Code vm_code;
  const m_bool emit_var;
  const m_bool is_spork;
};

ANN static m_bool spork_prepare_code(const Emitter emit, const struct Sporker *sp) {
  emit_add_instr(emit, RegPushImm);
  push_spork_code(emit, sp->is_spork ? SPORK_CODE_PREFIX : FORK_CODE_PREFIX, sp->code->pos);
  if(SAFE_FLAG(emit->env->func, member))
    stack_alloc_this(emit);
  return scoped_stmt(emit, sp->code, 0);
}

ANN static m_bool spork_prepare_func(const Emitter emit, const struct Sporker *sp) {
  push_spork_code(emit, sp->is_spork ? SPORK_FUNC_PREFIX : FORK_CODE_PREFIX, sp->exp->pos);
  return call_spork_func(emit, &sp->exp->d.exp_call);
}

ANN static VM_Code spork_prepare(const Emitter emit, const struct Sporker *sp) {
  if(!sp->code)
    CHECK_BO(prepare_call(emit, &sp->exp->d.exp_call))
  if((sp->code ? spork_prepare_code : spork_prepare_func)(emit, sp) > 0)
    return finalyze(emit, EOC);
  emit_pop_code(emit);
  return NULL;
}

ANN void spork_code(const Emitter emit, const struct Sporker *sp) {
  const Instr args = emit_add_instr(emit, SporkExp);
  args->m_val = emit->code->stack_depth;
  const Instr instr = emit_add_instr(emit, SporkEnd);
  instr->m_val = sp->emit_var;
}

ANN void spork_func(const Emitter emit, const struct Sporker *sp) {
  const Func f = sp->exp->d.exp_call.m_func;
  if(GET_FLAG(f, member) && is_fptr(emit->gwion, f->value_ref->type)) {
    const m_uint depth = f->def->stack_depth;
    regpop(emit, depth -SZ_INT);
    const Instr spork = emit_add_instr(emit, SporkMemberFptr);
    spork->m_val = depth;
  } else
    emit_exp_spork_finish(emit, f->def->stack_depth);
  (void)emit_add_instr(emit, SporkEnd);
}

ANN static Instr spork_ini(const Emitter emit, const struct Sporker *sp) {
  if(sp->is_spork) {
    const Instr instr = emit_add_instr(emit, SporkIni);
    instr->m_val = (m_uint)sp->vm_code;
    instr->m_val2 = sp->is_spork;
    return instr;
  }
  const Func f = !sp->code ? sp->exp->d.exp_call.m_func : NULL;
  const Instr instr = emit_add_instr(emit, ForkIni);
  instr->m_val = (m_uint)sp->vm_code;
  instr->m_val2 = f ? f->def->base->ret_type->size : 0;
  return instr;
}

ANN Instr emit_exp_spork(const Emitter emit, const Exp_Unary* unary) {
  struct Sporker sporker = {
    .exp=unary->exp,
    .code=unary->code,
    .is_spork=(unary->op == insert_symbol("spork")),
    .emit_var=exp_getvar(exp_self(unary))
  };
  CHECK_OO((sporker.vm_code = spork_prepare(emit, &sporker)))
  const Instr ini = spork_ini(emit, &sporker);
  (unary->code ? spork_code : spork_func)(emit, &sporker);
  return ini;
}

ANN static m_bool emit_exp_unary(const Emitter emit, const Exp_Unary* unary) {
// no pos ?
  const Type t = exp_self(unary)->info->type;
  if(t->e->def && !GET_FLAG(t, emit))
    CHECK_BB(ensure_emit(emit, t))
  struct Op_Import opi = { .op=unary->op, .data=(uintptr_t)unary, .op_type=op_unary };
  if(unary->op != insert_symbol("spork") && unary->op != insert_symbol("fork") && unary->exp) {
    CHECK_BB(emit_exp_pop_next(emit, unary->exp))
    emit_exp_addref1(emit, unary->exp, -exp_size(unary->exp));
    opi.rhs = unary->exp->info->type;
  }
  return op_emit_bool(emit, &opi);
}

ANN static m_bool emit_implicit_cast(const Emitter emit,
    const restrict Exp  from, const restrict Type to) {
  const struct Implicit imp = { from, to, from->pos };
  // no pos
  struct Op_Import opi = { .op=insert_symbol("@implicit"), .lhs=from->info->type, .rhs=to,
    .data=(m_uint)&imp, .op_type=op_implicit };
  return op_emit_bool(emit, &opi);
}

ANN static Instr _flow(const Emitter emit, const Exp e, const m_bool b) {
  CHECK_BO(emit_exp_pop_next(emit, e))
  emit_exp_addref1(emit, e, -exp_size(e));
  struct Op_Import opi = { .op=insert_symbol(b ? "@conditionnal" : "@unconditionnal"),
                           .rhs=e->info->type, .pos=e->pos, .data=(uintptr_t)e, .op_type=op_exp };
  const Instr instr = op_emit(emit, &opi);
  assert(instr != (Instr)GW_OK);
  return instr;
}
#define emit_flow(emit,b) _flow(emit, b, 1)

ANN static m_bool emit_exp_if(const Emitter emit, const Exp_If* exp_if) {
  DECL_OB(const Instr, op, = emit_flow(emit, exp_if->cond))
  CHECK_BB(emit_exp_pop_next(emit, exp_if->if_exp ?: exp_if->cond))
  const Instr op2 = emit_add_instr(emit, Goto);
  op->m_val = emit_code_size(emit);
  const m_bool ret = emit_exp_pop_next(emit, exp_if->else_exp);
  op2->m_val = emit_code_size(emit);
  return ret;
}

ANN static m_bool emit_lambda(const Emitter emit, const Exp_Lambda * lambda) {
  CHECK_BB(emit_func_def(emit, lambda->def))
  if(GET_FLAG(lambda->def, member) && !exp_getvar(exp_self(lambda)))
    emit_add_instr(emit, RegPushMem);
  regpushi(emit, (m_uint)lambda->def->base->func->code);
  return GW_OK;
}

ANN static m_bool emit_exp_lambda(const Emitter emit, const Exp_Lambda * lambda) {
  if(!lambda->def->base->func) {
    regpushi(emit, SZ_INT);
    return GW_OK;
  }
  struct EnvSet es = { .env=emit->env, .data=emit, .func=(_exp_func)emit_cdef,
    .scope=emit->env->scope->depth, .flag=ae_flag_emit };
  CHECK_BB(envset_push(&es, lambda->owner, lambda->def->base->func->value_ref->from->owner))
  const m_bool ret = emit_lambda(emit, lambda);
  if(es.run)
    envset_pop(&es, lambda->owner);
  return ret;
}

DECL_EXP_FUNC(emit, m_bool, Emitter)


ANN static void struct_addref(const Emitter emit, const Type type,
    const m_int size, const m_bool offset, const m_bool emit_var) {
  for(m_uint i = 0; i < vector_size(&type->e->tuple->types); ++i) {
    const Type t = (Type)vector_at(&type->e->tuple->types, i);
    if(isa(t, emit->gwion->type[et_object]) > 0) {
      const Instr instr = emit_addref(emit, emit_var);
      instr->m_val = size;
      instr->m_val2 = vector_at(&type->e->tuple->offset, i);
    } else if(GET_FLAG(t, struct))
      struct_addref(emit, t, size, offset + vector_at(&type->e->tuple->offset, i), emit_var);
  }
}

ANN static inline m_uint exp_size(const Exp e) {
  if(exp_getvar(e))
    return SZ_INT;
  const Type type = e->info->cast_to ?: e->info->type;
  return type->size;
}

ANN2(1) static void emit_exp_addref1(const Emitter emit, /* const */Exp exp, m_int size) {
  if(isa(exp->info->type, emit->gwion->type[et_object]) > 0 &&
    (exp->info->cast_to ? isa(exp->info->cast_to, emit->gwion->type[et_object]) > 0 : 1)) {
    const Instr instr = emit_addref(emit, exp_getvar(exp));
    instr->m_val = size;
  } else if(GET_FLAG(exp->info->type, struct)) // check cast_to ?
    struct_addref(emit, exp->info->type, size, 0, exp_getvar(exp));
}

ANN2(1) static void emit_exp_addref(const Emitter emit, /* const */Exp exp, m_int size) {
  do {
    emit_exp_addref1(emit, exp, size);
    size += exp_size(exp);
  } while((exp = exp->next));
}

ANN2(1) /*static */m_bool emit_exp(const Emitter emit, /* const */Exp e) {
  Exp exp = e;
  do {
    CHECK_BB(emit_exp_func[exp->exp_type](emit, &exp->d))
    if(exp_getnonnull(exp))
      emit_except(emit, exp->info->type);
    if(exp->info->cast_to)
      CHECK_BB(emit_implicit_cast(emit, exp, exp->info->cast_to))
  } while((exp = exp->next));
  return GW_OK;
}

ANN static m_bool emit_if(const Emitter emit, const Stmt_If stmt) {
  DECL_OB(const Instr, op, = emit_flow(emit, stmt->cond))
  CHECK_BB(scoped_stmt(emit, stmt->if_body, 1))
  const Instr op2 = emit_add_instr(emit, Goto);
  op->m_val = emit_code_size(emit);
  if(stmt->else_body)
    CHECK_BB(scoped_stmt(emit, stmt->else_body, 1))
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
  ++emit->env->scope->depth;
  const m_bool ret = stmt->stmt_list ? emit_stmt_list(emit, stmt->stmt_list) : 1;
  --emit->env->scope->depth;
  return ret;
}

ANN static m_bool optimize_taill_call(const Emitter emit, const Exp_Call* e) {
  if(e->args) {
    emit_func_args(emit, e);
    regpop(emit, e->m_func->def->stack_depth);
    emit_args(emit, e->m_func);
  }
  emit_add_instr(emit, Goto);
  return GW_OK;
}

ANN static m_bool emit_stmt_return(const Emitter emit, const Stmt_Exp stmt) {
  if(stmt->val) {
    if(stmt->val->exp_type == ae_exp_call && emit->env->func == stmt->val->d.exp_call.m_func)
      return optimize_taill_call(emit, &stmt->val->d.exp_call);
    CHECK_BB(emit_exp_pop_next(emit, stmt->val))
  }
  vector_add(&emit->code->stack_return, (vtype)emit_add_instr(emit, Goto));
  return GW_OK;
}

ANN static inline m_bool emit_stmt_continue(const Emitter emit, const Stmt stmt NUSED) {
  vector_add(&emit->code->stack_cont, (vtype)emit_add_instr(emit, Goto));
  return GW_OK;
}

ANN static inline m_bool emit_stmt_break(const Emitter emit, const Stmt stmt NUSED) {
  vector_add(&emit->code->stack_break, (vtype)emit_add_instr(emit, Goto));
  return GW_OK;
}

ANN static inline void emit_push_stack(const Emitter emit) {
  emit_push_scope(emit);
  vector_add(&emit->code->stack_cont, (vtype)NULL);
  vector_add(&emit->code->stack_break, (vtype)NULL);
}

ANN static void pop_vector(Vector v, const  m_uint pc) {
  Instr instr;
  while((instr = (Instr)vector_pop(v)))
    instr->m_val = pc;
}

ANN static void emit_pop_stack(const Emitter emit, const m_uint index) {
  pop_vector(&emit->code->stack_cont, index);
  pop_vector(&emit->code->stack_break, emit_code_size(emit));
  emit_pop_scope(emit);
}

ANN static m_bool _emit_stmt_flow(const Emitter emit, const Stmt_Flow stmt, const m_uint index) {
  Instr op = NULL;
  if(!stmt->is_do)
    op = _flow(emit, stmt->cond, stmt_self(stmt)->stmt_type == ae_stmt_while);
  CHECK_BB(scoped_stmt(emit, stmt->body, 1))
  if(stmt->is_do) {
    CHECK_OB((op = _flow(emit, stmt->cond, stmt_self(stmt)->stmt_type != ae_stmt_while)))
    op->m_val = index;
  } else {
    const Instr goto_ = emit_add_instr(emit, Goto);
    goto_->m_val = index;
    op->m_val = emit_code_size(emit);
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

ANN static m_bool variadic_state(const Emitter emit, const Stmt_VarLoop stmt, const m_uint status) {
  regpushi(emit, status);
  CHECK_BB(emit_exp(emit, stmt->exp))
  emit_add_instr(emit, SetObj);
  const Instr member = emit_add_instr(emit, DotMember4);
  member->m_val = SZ_INT;
  emit_add_instr(emit, int_r_assign);
  regpop(emit, SZ_INT);
  return GW_OK;
}

ANN static m_bool emit_stmt_varloop(const Emitter emit, const Stmt_VarLoop stmt) {
  CHECK_BB(variadic_state(emit, stmt, 1))
  CHECK_BB(emit_exp(emit, stmt->exp))
  const Instr member = emit_add_instr(emit, DotMember4);
  member->m_val = SZ_INT*2;
  const Instr instr = emit_add_instr(emit, BranchEqInt);
  const m_uint pc = emit_code_size(emit);
  emit_stmt(emit, stmt->body, 1);
  CHECK_BB(emit_exp(emit, stmt->exp))
  emit_vararg_end(emit, pc);
  instr->m_val = emit_code_size(emit);
  CHECK_BB(variadic_state(emit, stmt, 0))
  instr->m_val = emit_code_size(emit);
  return GW_OK;
}

ANN static m_bool _emit_stmt_for(const Emitter emit, const Stmt_For stmt, m_uint *action_index) {
  CHECK_BB(emit_stmt(emit, stmt->c1, 1))
  const m_uint index = emit_code_size(emit);
  DECL_OB(const Instr, op, = emit_flow(emit, stmt->c2->d.stmt_exp.val))
  CHECK_BB(scoped_stmt(emit, stmt->body, 1))
  *action_index = emit_code_size(emit);
  if(stmt->c3) {
    CHECK_BB(emit_exp(emit, stmt->c3))
    pop_exp(emit, stmt->c3);
  }
  const Instr _goto = emit_add_instr(emit, Goto);
  _goto->m_val = index;
  op->m_val = emit_code_size(emit);
  return GW_OK;
}

ANN static m_bool emit_stmt_for(const Emitter emit, const Stmt_For stmt) {
  emit_push_stack(emit);
  m_uint action_index = 0;
  const m_bool ret = _emit_stmt_for(emit, stmt, &action_index);
  emit_pop_stack(emit, action_index);
  return ret;
}

ANN static Instr emit_stmt_autoptr_init(const Emitter emit, const Type type) {
  const Instr new_obj = emit_add_instr(emit, ObjectInstantiate);
  new_obj->m_val2 = (m_uint)type;
  (void)emit_addref(emit, 0);
  regpop(emit, SZ_INT);
  return emit_add_instr(emit, Reg2Mem);
}

ANN static m_bool _emit_stmt_auto(const Emitter emit, const Stmt_Auto stmt, m_uint *end_pc) {
  emit_push_scope(emit);
  const Instr s1 = emit_add_instr(emit, MemSetImm);
  Instr cpy = stmt->is_ptr ? emit_stmt_autoptr_init(emit, stmt->v->type) : NULL;
  emit_local(emit, emit->gwion->type[et_int]);
  const m_uint offset = emit_local(emit, emit->gwion->type[et_int]);
  emit_local(emit, emit->gwion->type[et_int]);
  stmt->v->from->offset = offset + SZ_INT;
  const m_uint ini_pc  = emit_code_size(emit);
  emit_except(emit, stmt->exp->info->type);
  const Instr loop = emit_add_instr(emit, stmt->is_ptr ? AutoLoopPtr : AutoLoop);
  const Instr end = emit_add_instr(emit, BranchEqInt);
  const m_bool ret = scoped_stmt(emit, stmt->body, 1);
  *end_pc = emit_code_size(emit);
  if(stmt->is_ptr) {
    loop->m_val2 = (m_uint)stmt->v->type;
    cpy->m_val = stmt->v->from->offset;
  }
  const Instr tgt = emit_add_instr(emit, Goto);
  end->m_val = emit_code_size(emit);
  tgt->m_val = ini_pc;
  s1->m_val = loop->m_val = offset;
  regpop(emit, SZ_INT);
  emit_pop_scope(emit);
  return ret;
}

ANN static m_bool emit_stmt_auto(const Emitter emit, const Stmt_Auto stmt) {
  CHECK_BB(emit_exp(emit, stmt->exp))
  emit_push_stack(emit);
  m_uint end_pc = 0;
  const m_bool ret = _emit_stmt_auto(emit, stmt, &end_pc);
  emit_pop_stack(emit, end_pc);
  return ret;
}

ANN static m_bool _emit_stmt_loop(const Emitter emit, const Stmt_Loop stmt, m_uint *index) {
  CHECK_BB(emit_exp_pop_next(emit, stmt->cond))
  const m_uint offset = emit_local(emit, emit->gwion->type[et_int]);
  const Instr tomem = emit_add_instr(emit, Reg2Mem);
  tomem->m_val = offset;
  tomem->m_val2 = -SZ_INT;
  regpop(emit, SZ_INT);
  *index = emit_code_size(emit);
  const Instr cpy = emit_add_instr(emit, RegPushMem4);
  cpy->m_val = offset;
  emit_add_instr(emit, int_post_dec);
  const Instr op = emit_add_instr(emit, BranchEqInt);
  CHECK_BB(scoped_stmt(emit, stmt->body, 1))
  const Instr _goto = emit_add_instr(emit, Goto);
  _goto->m_val = *index;
  op->m_val = emit_code_size(emit);
  return GW_OK;
}

ANN static m_bool emit_stmt_loop(const Emitter emit, const Stmt_Loop stmt) {
  emit_push_stack(emit);
  m_uint index = 0;
  const m_bool ret = _emit_stmt_loop(emit, stmt, &index);
  emit_pop_stack(emit, index);
  return ret;
}

ANN static m_bool emit_stmt_jump(const Emitter emit, const Stmt_Jump stmt) {
  if(!stmt->is_label)
    stmt->data.instr = emit_add_instr(emit, Goto);
  else {
    assert(stmt->data.v.ptr);
    const m_uint size = vector_size(&stmt->data.v);
    if(!size)
//return GW_OK;
      ERR_B(stmt_self(stmt)->pos, _("label '%s' defined but not used."), s_name(stmt->name))
    LOOP_OPTIM
    for(m_uint i = size + 1; --i;) {
      const Stmt_Jump label = (Stmt_Jump)vector_at(&stmt->data.v, i - 1);
      if(!label->data.instr)
        ERR_B(stmt_self(label)->pos, _("you are trying to use a upper label."))
      label->data.instr->m_val = emit_code_size(emit);
    }
  }
  return GW_OK;
}

ANN static m_bool emit_type_def(const Emitter emit, const Type_Def tdef) {
  return tdef->type->e->def ? emit_class_def(emit, tdef->type->e->def) : GW_OK;
}

ANN static m_bool emit_enum_def(const Emitter emit, const Enum_Def edef) {
  LOOP_OPTIM
  for(m_uint i = 0; i < vector_size(&edef->values); ++i) {
    const Value v = (Value)vector_at(&edef->values, i);
    if(!emit->env->class_def) {
      ALLOC_PTR(emit->gwion->mp, addr, m_uint, i);
      v->from->offset = emit_local(emit, emit->gwion->type[et_int]);
      v->d.ptr = addr;
    } else
      *(m_bit*)(emit->env->class_def->nspc->info->class_data + v->from->offset) = i;
  }
  return GW_OK;
}

ANN void emit_union_offset(Decl_List l, const m_uint o) {
  do {
    Var_Decl_List v = l->self->d.exp_decl.list;
    do v->self->value->from->offset = o;
    while((v = v->next));
  } while((l = l->next));
}

ANN static inline void union_allocdata(MemPool mp, const Union_Def udef) {
  const Nspc nspc = (udef->xid ? udef->value->type : udef->type)->nspc;
  nspc_allocdata(mp, nspc);
  nspc->info->offset = udef->s;
}

ANN static m_bool emit_union_def(const Emitter emit, const Union_Def udef) {
  if(tmpl_base(udef->tmpl))
    return GW_OK;
  Decl_List l = udef->l;
  m_uint scope = emit->env->scope->depth;
  const m_bool global = GET_FLAG(udef, global);
  if(udef->xid) {
    union_allocdata(emit->gwion->mp, udef);
    Type_Decl *type_decl = new_type_decl(emit->gwion->mp,
        udef->xid, loc_cpy(emit->gwion->mp, udef->pos));
    type_decl->flag = udef->flag;
    const Var_Decl var_decl = new_var_decl(emit->gwion->mp, udef->xid, NULL, loc_cpy(emit->gwion->mp, udef->pos));
    const Var_Decl_List var_decl_list = new_var_decl_list(emit->gwion->mp, var_decl, NULL);
    const Exp exp = new_exp_decl(emit->gwion->mp, type_decl, var_decl_list);
    exp->d.exp_decl.type = udef->value->type;
    var_decl->value = udef->value;
    const m_bool ret = emit_exp_decl(emit, &exp->d.exp_decl);
    free_exp(emit->gwion->mp, exp);
    CHECK_BB(ret)
    if(global) {
      const M_Object o = new_object(emit->gwion->mp, NULL, udef->value->type);
      udef->value->d.ptr = (m_uint*)o;
      SET_FLAG(udef->value, builtin);
      UNSET_FLAG(udef->value, union);
    }
    scope = emit_push_type(emit, udef->value->type);
  } else if(udef->type_xid) {
    union_allocdata(emit->gwion->mp, udef);
    scope = emit_push_type(emit, udef->type);
  } else if(global) {
    // TODO: use mpool allocation
    void* ptr = (void*)xcalloc(1, udef->s);
    l = udef->l;
    udef->value->d.ptr = ptr;
    SET_FLAG(udef->value, enum);
    SET_FLAG(udef->value, dtor);
    do {
      Var_Decl_List list = l->self->d.exp_decl.list;
      list->self->value->d.ptr = ptr;
      SET_FLAG(list->self->value, union);
    } while((l = l->next));
    SET_FLAG(udef->l->self->d.exp_decl.list->self->value, enum);
    SET_FLAG(udef->l->self->d.exp_decl.list->self->value, dtor);
  }
  if(udef->xid)
    regpop(emit, SZ_INT);
  emit_union_offset(udef->l, udef->o);
  if(udef->xid || udef->type_xid || global)
    emit_pop(emit, scope);
  union_flag(udef, ae_flag_emit);
  return GW_OK;
}

ANN static m_bool emit_stmt_exp(const Emitter emit, const struct Stmt_Exp_* exp) {
  return exp->val ? emit_exp(emit, exp->val) : GW_OK;
}

ANN static m_bool emit_case_head(const Emitter emit, const Exp base,
    const Exp e, const Symbol op, const Vector v) {
  CHECK_BB(emit_exp(emit, base))
  CHECK_BB(emit_exp_pop_next(emit, e))
  const m_int size = -exp_size(e);
  emit_exp_addref(emit, base, -exp_totalsize(base) - size);
  emit_exp_addref1(emit, e, -size);
  const Exp_Binary bin = { .lhs=base, .rhs=e, .op=op };
  struct ExpInfo_ info = { .nspc=e->info->nspc };
  struct Exp_ ebin = { .d={.exp_binary=bin}, .info=&info };
  struct Op_Import opi = { .op=op, .lhs=base->info->type, .rhs=e->info->type,
    .data=(uintptr_t)&ebin.d.exp_binary, .pos=e->pos, .op_type=op_binary };
  CHECK_BB(op_emit_bool(emit, &opi))
  const Instr instr = emit_add_instr(emit, BranchEqInt);
  vector_add(v, (vtype)instr);
  return GW_OK;
}

ANN static m_bool emit_case_body(const Emitter emit, const struct Stmt_Match_* stmt) {
  const Instr when = stmt->when ? emit_flow(emit, stmt->when) : NULL;
  if(stmt->when)
    CHECK_OB(when)
  CHECK_BB(emit_stmt_list(emit, stmt->list))
  const Instr instr = emit_add_instr(emit, Goto);
  vector_add(&emit->env->scope->match->vec, (vtype)instr);
  if(when)
    when->m_val = emit_code_size(emit);
  return GW_OK;
}

ANN static m_bool case_value(const Emitter emit, const Exp base, const Exp e) {
  const Value v = e->d.prim.value;
  v->from->offset = emit_local(emit, base->info->type);
  CHECK_BB(emit_exp(emit, base))
  emit_exp_addref(emit, base, -exp_totalsize(base));
  regpop(emit, base->info->type->size);
  const Instr instr = emit_add_instr(emit, Reg2Mem4);
  instr->m_val = v->from->offset;
  instr->m_val2 = base->info->type->size;
  return GW_OK;
}


#define CASE_PASS (Symbol)1
ANN static Symbol case_op(const Emitter emit, const Exp base, const Exp e) {
  if(e->exp_type == ae_exp_primary) {
    if(e->d.prim.prim_type == ae_prim_id) {
      if(e->d.prim.d.var == insert_symbol("_"))
        return CASE_PASS;
      if(!nspc_lookup_value1(emit->env->curr, e->d.prim.d.var)) {
        CHECK_BO(case_value(emit, base, e))
        return CASE_PASS;
      }
    }
  }
  return insert_symbol("==");
}

ANN static m_bool _emit_stmt_match_case(const Emitter emit, const struct Stmt_Match_* stmt,
    const Vector v) {
  Exp e = stmt->cond;
  const Map map = &emit->env->scope->match->map;
  for(m_uint i = 0; i < map_size(map) && e; e = e->next, ++i) {
    const Exp base = (Exp)VKEY(map, i);
    const Symbol op = case_op(emit, base, e);
    if(op != CASE_PASS)
      CHECK_BB(emit_case_head(emit, base, e, op, v))
  }
  CHECK_BB(emit_case_body(emit, stmt))
  return GW_OK;
}

ANN static m_bool emit_stmt_match_case(const Emitter emit, const struct Stmt_Match_* stmt) {
  emit_push_scope(emit);
  struct Vector_ v;
  vector_init(&v);
  const m_bool ret = _emit_stmt_match_case(emit, stmt, &v);
  emit_pop_scope(emit);
  for(m_uint i = 0; i < vector_size(&v); ++i) {
    const Instr instr = (Instr)vector_at(&v, i);
    instr->m_val = emit_code_size(emit);
  }
  vector_release(&v);
  return ret;
}

ANN static inline void match_unvec(struct Match_ *const match , const m_uint pc) {
  const Vector vec = &match->vec;
  for(m_uint i = 0; i < vector_size(vec); ++i) {
    const Instr instr = (Instr)VPTR(vec, i);
    instr->m_val = pc;
  }
  vector_release(vec);
}

ANN static m_bool emit_stmt_cases(const Emitter emit, Stmt_List list) {
  do CHECK_BB(emit_stmt_match_case(emit, &list->stmt->d.stmt_match))
  while((list = list->next));
  return GW_OK;
}

ANN static m_bool emit_match(const Emitter emit, const struct Stmt_Match_* stmt) {
  if(stmt->where)
    CHECK_BB(emit_stmt(emit, stmt->where, 1))
  MATCH_INI(emit->env->scope)
  vector_init(&m.vec);
  const m_bool ret = emit_stmt_cases(emit, stmt->list);
  match_unvec(&m, emit_code_size(emit));
  MATCH_END(emit->env->scope)
  return ret;
}

ANN static m_bool emit_stmt_match(const Emitter emit, const struct Stmt_Match_* stmt) {
  emit_push_scope(emit);
  const m_bool ret = emit_match(emit, stmt);
  emit_pop_scope(emit);
  return ret;
}

ANN static m_bool emit_stmt_pp(const Emitter emit, const struct Stmt_PP_* stmt) {
  if(stmt->pp_type == ae_pp_pragma) {
    if(!strncmp(stmt->data, "memoize", strlen("memoize")))
      emit->info->memoize = strtol(stmt->data + 7, NULL, 10);
  }
  return GW_OK;
}

#define emit_stmt_while emit_stmt_flow
#define emit_stmt_until emit_stmt_flow
DECL_STMT_FUNC(emit, m_bool , Emitter)

ANN static m_bool emit_stmt(const Emitter emit, const Stmt stmt, const m_bool pop) {
  CHECK_BB(emit_stmt_func[stmt->stmt_type](emit, &stmt->d))
  if(pop && stmt->stmt_type == ae_stmt_exp && stmt->d.stmt_exp.val)
    pop_exp(emit, stmt->d.stmt_exp.val);
  return GW_OK;
}

ANN static m_bool emit_stmt_list(const Emitter emit, Stmt_List l) {
  do CHECK_BB(emit_stmt(emit, l->stmt, 1))
  while((l = l->next));
  return GW_OK;
}

ANN static m_bool emit_exp_dot(const Emitter emit, const Exp_Dot* member) {
  struct Op_Import opi = { .op=insert_symbol("@dot"), .lhs=member->t_base,
    .rhs=exp_self(member)->info->type, .data=(uintptr_t)member, .pos=exp_self(member)->pos, .op_type=op_dot };
  return op_emit_bool(emit, &opi);
}

ANN static inline void emit_func_def_global(const Emitter emit, const Value value) {
  const Instr set_mem = emit_add_instr(emit, MemSetImm);
  set_mem->m_val = value->from->offset;
  set_mem->m_val2 = (m_uint)value->d.func_ref->code;
}

ANN static void emit_func_def_init(const Emitter emit, const Func func) {
  const Type t = emit->env->class_def;
  char c[(t ? strlen(t->name) + 1 : 0) + strlen(func->name) + 6];
  sprintf(c, "%s%s%s(...)", t ? t->name : "", t ? "." : "", func->name);
  emit_push_code(emit, c);
}

ANN static void emit_func_def_args(const Emitter emit, Arg_List a) {
  do {
    const Type type = a->var_decl->value->type;
    emit->code->stack_depth += type->size;
    a->var_decl->value->from->offset = emit_local(emit, type);
  } while((a = a->next));
}

ANN static void emit_func_def_ensure(const Emitter emit, const Func_Def fdef) {
  const m_uint size = fdef->base->ret_type->size;
  if(size) {
    const Instr instr = emit_kind(emit, size, 0, regpushimm);
    instr->m_val2 = size;
  }
  vector_add(&emit->code->stack_return, (vtype)emit_add_instr(emit, Goto));
}

ANN static void emit_func_def_return(const Emitter emit) {
  const m_uint val = emit_code_size(emit);
  LOOP_OPTIM
  for(m_uint i = vector_size(&emit->code->stack_return) + 1; --i; ) {
    const Instr instr = (Instr)vector_at(&emit->code->stack_return, i-1);
    instr->m_val = val;
  }
  vector_clear(&emit->code->stack_return);
  if(emit->info->memoize && GET_FLAG(emit->env->func, pure)) {
    const Instr instr = emit_add_instr(emit, MemoizeStore);
    instr->m_val = emit->env->func->def->stack_depth;
  }
}

ANN static VM_Code emit_internal(const Emitter emit, const Func f) {
  if(f->def->base->xid == insert_symbol("@dtor")) {
    emit->env->class_def->nspc->dtor = f->code = finalyze(emit, DTOR_EOC);
    ADD_REF(f->code)
    return f->code;
  } else if(f->def->base->xid == insert_symbol("@gack")) {
    regpop(emit, SZ_INT*2);
    const Instr instr = emit_add_instr(emit, RegPushMem);
    instr->m_val = SZ_INT;
    f->code = finalyze(emit, FuncReturn);
    return emit->env->class_def->e->gack = f->code;
  }
  return finalyze(emit, FuncReturn);
}

ANN static VM_Code emit_func_def_code(const Emitter emit, const Func func) {
  if(GET_FLAG(func->def, typedef))
    return emit_internal(emit, func);
  else
    return finalyze(emit, FuncReturn);
}

ANN static m_bool emit_func_def_body(const Emitter emit, const Func_Def fdef) {
  if(fdef->base->args)
    emit_func_def_args(emit, fdef->base->args);
  if(GET_FLAG(fdef, variadic))
    stack_alloc(emit);
  if(fdef->d.code)
    CHECK_BB(scoped_stmt(emit, fdef->d.code, 1))
  emit_func_def_ensure(emit, fdef);
  return GW_OK;
}

ANN static Instr me_top(MemoizeEmitter *me) {
  const Instr idx = emit_add_instr(me->emit, MemSetImm);
  idx->m_val = me->offset;
  const Instr pc = emit_add_instr(me->emit, MemSetImm);
  pc->m_val = me->offset + SZ_INT;
  return pc;
}

ANN static void me_ini(MemoizeEmitter *me) {
  const Instr ini = emit_add_instr(me->emit, MemoizeIni);
  ini->m_val = me->offset;
  ini->m_val2 = me->fdef->stack_depth + me->fdef->base->ret_type->size;
}

ANN static void me_end(MemoizeEmitter *me, const m_uint pc) {
  for(m_uint i = 0; i < vector_size(&me->branch); ++i)
	  ((Instr)vector_at(&me->branch, i))->m_val = pc;
}

ANN static void me_bottom(MemoizeEmitter *me, const m_uint pc) {
  const Instr idx = emit_add_instr(me->emit, RegPushMem4);
  idx->m_val = me->offset;
  emit_add_instr(me->emit, int_pre_inc);
  regpop(me->emit, SZ_INT);
  const Instr loop = emit_add_instr(me->emit, Goto);
  loop->m_val = pc;
}

ANN static void me_ret(MemoizeEmitter *me) {
  const Instr instr = emit_kind(me->emit, me->fdef->base->ret_type->size, 0, regpushmem);
  instr->m_val = (me->offset + SZ_INT)*2;
  emit_add_instr(me->emit, FuncReturn);
}

ANN static m_bool me_run(MemoizeEmitter *me, const m_uint pc) {
  me_ini(me);
  if(me->fdef->base->args)
    CHECK_BB(me_arg(me))
  me_ret(me);
  me_end(me, emit_code_size(me->emit));
  me_bottom(me, pc);
  return GW_OK;
}

ANN static m_bool emit_memoize(const Emitter emit, const Func_Def fdef) {
  MemoizeEmitter me = { .emit=emit, .fdef=fdef, .offset=fdef->stack_depth };
  const Instr pc = me_top(&me);
  const m_uint top = emit_code_size(emit);
  vector_init(&me.branch);
  const m_bool ret = me_run(&me, top);
  vector_release(&me.branch);
  pc->m_val2 = emit_code_size(emit);
  return ret;
}

ANN static m_bool emit_fdef(const Emitter emit, const Func_Def fdef) {
  if(emit->info->memoize && GET_FLAG(fdef->base->func, pure))
    CHECK_BB(emit_memoize(emit, fdef))
  CHECK_BB(emit_func_def_body(emit, fdef))
  emit_func_def_return(emit);
  return GW_OK;
}

ANN static void emit_fdef_finish(const Emitter emit, const Func_Def fdef) {
  const Func func = fdef->base->func;
  func->code = emit_func_def_code(emit, func);
  if(!emit->env->class_def && !GET_FLAG(fdef, global) && !fdef->base->tmpl)
    emit_func_def_global(emit, func->value_ref);
  if(emit->info->memoize && GET_FLAG(func, pure))
    func->code->memoize = memoize_ini(emit, func);
}

ANN static m_bool emit_func_def(const Emitter emit, const Func_Def f) {
  const Func func = f->base->func;
  const Func_Def fdef = func->def;
  const Func former = emit->env->func;
  if(func->code || tmpl_base(fdef->base->tmpl))
    return GW_OK;
  if(SAFE_FLAG(emit->env->class_def, builtin) && GET_FLAG(emit->env->class_def, template))
    return GW_OK;
  // TODO: we might not need lambdas here
  if(!emit->env->class_def && !GET_FLAG(fdef, global) && !fdef->base->tmpl && !emit->env->scope->depth)
    func->value_ref->from->offset = emit_local(emit, emit->gwion->type[et_int]);
  emit_func_def_init(emit, func);
  if(GET_FLAG(func, member))
    stack_alloc_this(emit);
  emit->env->func = func;
  emit_push_scope(emit);
  if(!strcmp(s_name(fdef->base->xid), "@gack")) {
    emit_local(emit, emit->gwion->type[et_int]);
    const Instr instr = emit_add_instr(emit, MemSetImm);
    instr->m_val = SZ_INT;
  }
  const m_bool ret = scanx_fdef(emit->env, emit, fdef, (_exp_func)emit_fdef);
  emit_pop_scope(emit);
  emit->env->func = former;
  if(ret > 0)
    emit_fdef_finish(emit, fdef);
  else
    emit_pop_code(emit);
  return ret;
}

#define emit_fptr_def dummy_func
HANDLE_SECTION_FUNC(emit, m_bool, Emitter)

ANN Code* emit_class_code(const Emitter emit, const m_str name) {
  const m_uint len = strlen(name) + 7;
  char c[len];
  snprintf(c, len, "class %s", name);
  emit_push_code(emit, c);
  stack_alloc_this(emit);
  return emit->code;
}

ANN static m_bool emit_parent(const Emitter emit, const Class_Def cdef) {
  const Type parent = cdef->base.type->e->parent;
  return !GET_FLAG(parent, emit) ? scanx_parent(parent, emit_cdef, emit) : GW_OK;
}

ANN static inline m_bool emit_cdef(const Emitter emit, const Class_Def cdef) {
  return scanx_cdef(emit->env, emit, cdef,
      (_exp_func)emit_class_def, (_exp_func)emit_union_def);
}

ANN void emit_class_finish(const Emitter emit, const Nspc nspc) {
  nspc->pre_ctor = finalyze(emit, FuncReturn);
  SET_FLAG(nspc->pre_ctor, ctor);
}

ANN static m_bool cdef_parent(const Emitter emit, const Class_Def cdef) {
  if(cdef->base.tmpl && cdef->base.tmpl->list)
    CHECK_BB(template_push_types(emit->env, cdef->base.tmpl))
  const m_bool ret = scanx_parent(cdef->base.type, emit_parent, emit);
  if(cdef->base.tmpl && cdef->base.tmpl->list)
    nspc_pop_type(emit->gwion->mp, emit->env->curr);
  return ret;
}

ANN static m_bool emit_class_def(const Emitter emit, const Class_Def cdef) {
  if(tmpl_base(cdef->base.tmpl))
    return GW_OK;
  const Type t = cdef->base.type;
  if(GET_FLAG(t, emit))
    return GW_OK;
  SET_FLAG(t, emit);
  if(cdef->base.ext && t->e->parent->e->def && !GET_FLAG(t->e->parent, emit))
    CHECK_BB(cdef_parent(emit, cdef))
  nspc_allocdata(emit->gwion->mp, t->nspc);
  if(cdef->body) {
    emit_class_code(emit, t->name);
    if(scanx_body(emit->env, cdef, (_exp_func)emit_section, emit) > 0)
      emit_class_finish(emit, t->nspc);
    else {
      free_code(emit->gwion->mp, emit->code);
      emit_pop_code(emit);
      return GW_ERROR;
    }
  }
  return GW_OK;
}

ANN static inline void emit_free_code(const Emitter emit, Code* code) {
  if(vector_size(&code->instr))
    free_code_instr(&code->instr, emit->gwion);
  free_code(emit->gwion->mp, code);
}

ANN static VM_Code emit_free_stack(const Emitter emit) {
  LOOP_OPTIM
  for(m_uint i = vector_size(&emit->stack) + 1; --i;)
    emit_free_code(emit, (Code*)vector_at(&emit->stack, i - 1));
  vector_clear(&emit->stack);
  vector_clear(&emit->info->pure);
  emit_free_code(emit, emit->code);
  return NULL;
}

ANN static inline m_bool emit_ast_inner(const Emitter emit, Ast ast) {
  do CHECK_BB(emit_section(emit, ast->section))
  while((ast = ast->next));
  return GW_OK;
}

ANN m_bool emit_ast(const Env env, Ast ast) {
  const Emitter emit = env->gwion->emit;
  emit->info->memoize = 0;
  emit->code = new_code(emit, emit->env->name);
  emit_push_scope(emit);
  const m_bool ret = emit_ast_inner(emit, ast);
  emit_pop_scope(emit);
  if(ret > 0)
    emit->info->code = finalyze(emit, emit->info->finalyzer);
  else
    emit_free_stack(emit);
  return ret;
}
