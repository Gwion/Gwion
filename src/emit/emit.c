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
#include "tuple.h"
#include "specialid.h"

#undef insert_symbol
#define insert_symbol(a) insert_symbol(emit->gwion->st, (a))

#undef ERR_B
#define ERR_B(a, b, ...) { env_err(emit->env, (a), (b), ## __VA_ARGS__); return GW_ERROR; }
#undef ERR_O
#define ERR_O(a, b, ...) { env_err(emit->env, (a), (b), ## __VA_ARGS__); return NULL; }

typedef struct Local_ {
  m_uint size;
  m_uint offset;
  m_bool is_obj;
} Local;

static inline void emit_pop_type(const Emitter emit) { nspc_pop_type(emit->gwion->mp, emit->env->curr); }
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

ANN static Local* new_local(MemPool p, const m_uint size, const m_bool is_obj) {
  Local* local  = mp_calloc(p, Local);
  local->size   = size;
  local->is_obj = is_obj;
  return local;
}

ANN static m_uint frame_local(MemPool p, Frame* frame, const m_uint size, const m_bool is_obj) {
  Local* local = new_local(p, size, is_obj);
  local->offset = frame->curr_offset;
  frame->curr_offset += size;
  vector_add(&frame->stack, (vtype)local);
  return local->offset;
}

ANN static inline void frame_push(Frame* frame) {
  vector_add(&frame->stack, (vtype)NULL);
}

ANN static m_int frame_pop(Frame* frame) {
  DECL_OB(const Local*, l, = (Local*)vector_pop(&frame->stack))
  frame->curr_offset -= l->size;
  return l->is_obj ? (m_int)l->offset : frame_pop(frame);
}

ANN static m_bool emit_exp(const Emitter emit, Exp exp, const m_bool add_ref);
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
  while((offset = frame_pop(emit->code->frame)) > -1) {
    Instr instr = emit_add_instr(emit, ObjectRelease);
    instr->m_val = (m_uint)offset;
  }
  vector_pop(&emit->info->pure);
}

ANN static inline void emit_push_code(const Emitter emit, const m_str name) {
  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code(emit, name);
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

ANN m_uint emit_local(const Emitter emit, const m_uint size, const m_bool is_obj) {
  return frame_local(emit->gwion->mp, emit->code->frame, size, is_obj);
}

ANN static void emit_pre_ctor(const Emitter emit, const Type type) {
  if(type->e->parent)
    emit_pre_ctor(emit, type->e->parent);
  if(type->nspc && type->nspc->pre_ctor && !GET_FLAG(type, nonnull))
    emit_ext_ctor(emit, type->nspc->pre_ctor);
/*
  if(type->nspc && !GET_FLAG(type, nonnull)) {
    if(type->nspc->pre_ctor)
      emit_ext_ctor(emit, type->nspc->pre_ctor);
    if(type->nspc->ctor)
      emit_ext_ctor(emit, type->nspc->ctor);
  } 
*/
  if(GET_FLAG(type, typedef) && type->e->parent->array_depth)
    emit_array_extend(emit, type->e->parent, type->e->def->base.ext->array->exp);
  if(GET_FLAG(type, template) && GET_FLAG(type, builtin)) {
    const Type t = template_parent(emit->env, type);
    if(t->nspc->pre_ctor)
      emit_ext_ctor(emit, t->nspc->pre_ctor);
  }
}

#define regxxx(name, instr) \
ANN static inline Instr reg##name(const Emitter emit, const m_uint sz) { \
  const Instr instr = emit_add_instr(emit, Reg##instr); \
  instr->m_val = sz; \
  return instr; \
}
regxxx(pop, Pop)
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
    CHECK_BB(emit_exp(emit, e, 0))
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

ANN void emit_ext_ctor(const Emitter emit, const VM_Code code) {
  const Instr cpy = emit_add_instr(emit, Reg2Reg);
  cpy->m_val2 = -SZ_INT;
  const Instr set_code = regseti(emit, (m_uint)code);
  set_code->m_val2 = SZ_INT;
  const m_uint offset = emit_code_offset(emit);
  const Instr regset = regseti(emit, offset);
  regset->m_val2 = SZ_INT *2;
  const Instr push = emit_add_instr(emit, RegPush);
  push->m_val = SZ_INT *2;
  const Instr prelude = emit_add_instr(emit, !GET_FLAG(code, builtin) ? FuncUsr : FuncMember);
  prelude->m_val2 = 2;
  prelude->m_val = SZ_INT;
  emit_add_instr(emit, Reg2Mem);
  const Instr next = emit_add_instr(emit, Overflow);
  next->m_val2 = offset;
}

ANN m_bool emit_array_extend(const Emitter emit, const Type t, const Exp e) {
  CHECK_OB(emit_array_extend_inner(emit, t, e))
  emit_add_instr(emit, PopArrayClass);
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

ANN static Instr emit_kind(Emitter emit, const m_uint size, const uint addr, const f_instr func[]) {
  const enum Kind kind = kindof(size, addr);
  const Instr instr = emit_add_instr(emit, func[kind]);
  instr->m_val2 = size;
  return instr;
}

static const f_instr regpushimm[] = { RegPushImm, RegPushImm2, RegPushImm3, RegPushImm4 };
static const f_instr regpushmem[] = { RegPushMem, RegPushMem2, RegPushMem3, RegPushMem4 };
static const f_instr regpushbase[] = { RegPushBase, RegPushBase2, RegPushBase3, RegPushBase4 };
static const f_instr dotstatic[]  = { DotStatic, DotStatic2, DotStatic3, RegPushImm };
static const f_instr dotmember[]  = { DotMember, DotMember2, DotMember3, DotMember4 };
static const f_instr allocmember[]  = { RegPushImm, RegPushImm2, RegPushImm3, AllocMember4 };
static const f_instr allocword[]  = { AllocWord, AllocWord2, AllocWord3, RegPushMem4 };

ANN static inline Exp this_exp(const Emitter emit, const Type t, const loc_t pos) {
  const Exp exp = new_prim_id(emit->gwion->mp, insert_symbol("this"),
    loc_cpy(emit->gwion->mp, pos));
  exp->type = t;
  return exp;
}

ANN static inline Exp dot_this_exp(const Emitter emit, const Symbol *data, const Type t) {
  const Exp exp = this_exp(emit, t, prim_pos(data));
  const Exp dot = new_exp_dot(emit->gwion->mp, exp, *data);
  dot->d.exp_dot.t_base = t;
  return dot;
}

ANN static inline Exp dot_static_exp(const Emitter emit, const Symbol *data, const Type t) {
  const Symbol s = insert_symbol(t->name);
  const Exp    e = new_prim_id(emit->gwion->mp, s,
    loc_cpy(emit->gwion->mp, prim_pos(data)));
  const Value  val = nspc_lookup_value1(t->nspc->parent, s);
  const Exp dot = new_exp_dot(emit->gwion->mp, e, *data);
  dot->d.exp_dot.t_base = val->type;
  return dot;
}

ANN static m_bool emit_symbol_owned(const Emitter emit, const Symbol *data) {
  const Value v = prim_self(data)->value;
  const Exp dot = (!GET_FLAG(v, static) ? dot_this_exp : dot_static_exp)(emit, data, v->from->owner_class);
  dot->type = prim_exp(data)->type;
  dot->emit_var = prim_exp(data)->emit_var;
  const m_bool ret = emit_exp_dot(emit, &dot->d.exp_dot);
  free_exp(emit->gwion->mp, dot);
  return ret;
}

ANN static m_bool emit_symbol_builtin(const Emitter emit, const Symbol *data) {
  const Value v = prim_self(data)->value;
  if(GET_FLAG(v, union)) {
    const m_uint size = v->type->size;
    const Instr instr = emit_kind(emit, size, prim_exp(data)->emit_var, dotstatic);
    instr->m_val = (m_uint)v->d.ptr;
  } else {
    const m_uint size = v->type->size;
    const Instr instr = emit_kind(emit, size, prim_exp(data)->emit_var, regpushimm);
    if(!exp_self(data)->emit_var && size == SZ_INT) {
      if(isa(v->type, emit->gwion->type[et_object]) > 0) {
        instr->opcode = eRegPushImm;
        instr->m_val = (m_uint)v->d.ptr;
      } else if(v->d.ptr)
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
  const Instr instr = emit_kind(emit, size, prim_exp(data)->emit_var, !GET_FLAG(v, global) ? regpushmem : regpushbase);
  instr->m_val  = v->from->offset;
  if(isa(v->type, emit->gwion->type[et_function]) > 0 && !is_fptr(emit->gwion, v->type))
    instr->m_val = prim_exp(data)->type->e->d.func->value_ref->from->offset;
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
  CHECK_BB(emit_exp(emit, e, 0))
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

ANN static void array_loop(const Emitter emit, const m_uint depth) {
  regpop(emit, depth * SZ_INT);
  emit_add_instr(emit, GWOP_EXCEPT);
  for(m_uint i = 0; i < depth - 1; ++i) {
    const Instr access = emit_add_instr(emit, ArrayAccess);
    access->m_val = i;
    const Instr get = emit_add_instr(emit, ArrayGet);
    get->m_val = i;
    get->m_val2 = -SZ_INT;
    emit_add_instr(emit, GWOP_EXCEPT);
  }
  regpop(emit, SZ_INT);
  const Instr access = emit_add_instr(emit, ArrayAccess);
  access->m_val = depth;
}

ANN static void array_finish(const Emitter emit, const m_uint depth,
		const m_uint size, const m_bool is_var) {
  const Instr get = emit_add_instr(emit, is_var ? ArrayAddr : ArrayGet);
  get->m_val = depth;
  const Instr push = emit_add_instr(emit, ArrayValid);
  push->m_val = is_var ? SZ_INT : size;
}

ANN static inline m_bool array_do(const  Emitter emit, const Array_Sub array, const m_bool is_var) {
  emit_add_instr(emit, GcAdd);
  CHECK_BB(emit_exp(emit, array->exp, 0))
  array_loop(emit, array->depth);
  array_finish(emit, array->depth, array->type->size, is_var);
  return GW_OK;
}

ANN static inline void tuple_access(const  Emitter emit, const m_uint idx,
        const m_bool is_var) {
  const Instr instr = emit_add_instr(emit, TupleMember);
  instr->m_val = idx;
  instr->m_val2 = is_var;
  emit_add_instr(emit, DotMember); // just a place holder.
}

struct ArrayAccessInfo {
  struct Array_Sub_ array;
  const Type type;
  const m_bool is_var;
};

ANN static inline m_bool _emit_indexes(const Emitter emit, struct ArrayAccessInfo *const info);

ANN void emit_except(const Emitter emit, const Type t) {
  if(!GET_FLAG(t, nonnull))
    emit_add_instr(emit, GWOP_EXCEPT);
}

ANN static inline m_bool tuple_index(const Emitter emit, struct ArrayAccessInfo *const info) {
  assert(isa(info->array.type, emit->gwion->type[et_tuple]) > 0);
  const m_uint idx = info->array.exp->d.prim.d.num;
  emit_except(emit, info->array.type);
  tuple_access(emit, info->array.exp->d.prim.d.num, (info->array.depth -1)? 0 : info->is_var);
  if(!info->array.exp->next)
    return GW_OK;
  const Type type = (Type)vector_at(&info->array.type->e->tuple->types, idx);
  struct Array_Sub_ next = { info->array.exp->next, type, info->array.depth - 1 };
  info->array = next;
  return _emit_indexes(emit, info);
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

ANN static inline m_bool emit_partial_indexes(const Emitter emit, struct ArrayAccessInfo *const info) {
  struct Array_Sub_ partial = { info->array.exp, info->array.type, info->array.type->array_depth };
  struct Array_Sub_ next = { info->array.exp, array_base(info->array.type), info->array.depth - info->array.type->array_depth };
  info->array = partial;
  DECL_OB(const Exp, exp, = emit_n_exp(emit, info))
  next.exp = exp;
  info->array = next;
  return _emit_indexes(emit, info);
}

ANN static inline m_bool _emit_indexes(const Emitter emit, struct ArrayAccessInfo *const info) {
  if(GET_FLAG(info->array.type, typedef)) {
    info->array.type = info->array.type->e->parent;
    return _emit_indexes(emit, info);
  }
  if(info->array.type->array_depth >= info->array.depth) {
    struct Array_Sub_ next = { info->array.exp, info->type, info->array.depth };
    return array_do(emit, &next, info->is_var);
  }
  return (info->array.type->array_depth ? emit_partial_indexes : tuple_index)(emit, info);
}

ANN static m_bool emit_exp_array(const Emitter emit, const Exp_Array* array) {
  CHECK_BB(emit_exp(emit, array->base, 0))
  const Exp e = exp_self(array);
  struct ArrayAccessInfo info = { *array->array, e->type, e->emit_var };
  return _emit_indexes(emit, &info);
}

ANN static m_bool emit_prim_vec(const Emitter emit, const Vec *vec) {
  const ae_prim_t t = prim_self(vec)->prim_type;
  CHECK_BB(emit_exp(emit, vec->exp, 0));
  m_int n = (m_int)((t == ae_prim_vec ? 3 : 2) - vec->dim + 1);
  while(--n > 0)
    emit_add_instr(emit, RegPushImm2);
  if(prim_exp(vec)->emit_var) {
    emit_local(emit, prim_exp(vec)->type->size, 0);
    const m_uint offset = emit_local(emit, SZ_INT, 0);
    const Instr cpy = emit_add_instr(emit, VecCpy);
    cpy->m_val = offset;
    cpy->m_val2 = prim_exp(vec)->type->size;
    const Instr instr = emit_add_instr(emit, RegPushMem);
    instr->m_val = offset;
  }
  return GW_OK;
}

ANN static m_bool emit_prim_id(const Emitter emit, const Symbol *data) {
  struct SpecialId_ * spid = specialid_get(emit->gwion, *data);
  if(spid)
    return specialid_instr(emit, spid, prim_self(data)) ? GW_OK : GW_ERROR;
  return emit_symbol(emit, prim_self(data));
}

ANN static m_bool emit_prim_tuple(const Emitter emit, const Tuple *tuple) {
  CHECK_BB(emit_exp(emit, tuple->exp, 1))
  const Instr instr = emit_add_instr(emit, TupleCtor);
  instr->m_val = (m_uint)prim_exp(tuple)->type;
  return GW_OK;
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
  DECL_BB(const m_int, c, = str2char(emit, *str, prim_pos(str)))
  regpushi(emit, c);
  return GW_OK;
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
  emit_add_instr(emit, RegAddRef);
  return GW_OK;
}

ANN static m_bool emit_prim_hack(const Emitter emit, const Exp *exp) {
  Exp e = *exp, next = NULL;
  do {
    next = e->next;
    e->next = NULL;
    if(emit_exp(emit, e, 0) < 0) {
      e->next = next;
      return GW_ERROR;
    }
    const Instr instr = emit_add_instr(emit, Gack);
    instr->m_val = (m_uint)e->type;
    instr->m_val2 = emit_code_offset(emit);
  } while((e = e->next = next));
  if(!(emit->env->func && emit->env->func->def->base->xid == insert_symbol("@gack")))
    emit_add_instr(emit, Gack3);
  return GW_OK;
}

ANN static m_bool emit_prim_unpack(const Emitter emit NUSED, const Tuple *tuple) {
  if(prim_exp(tuple)->meta == ae_meta_var)
    return GW_OK;
  ERR_B(prim_pos(tuple), _("unused Tuple unpack"))
}

#define emit_prim_complex emit_prim_vec
#define emit_prim_polar   emit_prim_vec
#define emit_prim_nil     (void*)dummy_func

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
  emit_add_instr(emit, RegAddRef);
  regpop(emit, SZ_INT);
  emit->code = code;
  return GW_OK;
}

ANN static m_bool emit_exp_decl_static(const Emitter emit, const Var_Decl var_decl, const uint is_ref, const uint emit_addr) {
  const Value value = var_decl->value;
  if(isa(value->type, emit->gwion->type[et_object]) > 0 && !is_ref)
    CHECK_BB(decl_static(emit, var_decl, 0))
  return emit_dot_static_data(emit, value, emit_addr);
}

ANN static m_bool emit_exp_decl_non_static(const Emitter emit, const Exp_Decl *decl, const Var_Decl var_decl,
  const uint is_ref, const uint emit_var) {
  const Value v = var_decl->value;
  const Type type = v->type;
  const Array_Sub array = var_decl->array;
  const m_bool is_array = (array && array->exp) || GET_FLAG(decl->td, force);
  const m_bool is_obj = isa(type, emit->gwion->type[et_object]) > 0;
  const uint emit_addr = ((is_ref && !array) || isa(type, emit->gwion->type[et_object]) < 0) ?
    emit_var : 1;
  if(is_obj && (is_array || !is_ref))
    CHECK_BB(emit_instantiate_object(emit, type, array, is_ref))
  f_instr *exec = (f_instr*)allocmember;
  if(!GET_FLAG(v, member)) {
    v->from->offset = emit_local(emit, v->type->size, is_obj);
    exec = (f_instr*)(allocword);
    if(GET_FLAG(var_decl->value, ref)) {
      const Instr clean = emit_add_instr(emit, MemSetImm);
      clean->m_val = v->from->offset;
    }
  }
  const Instr instr = emit_kind(emit, v->type->size, emit_addr, exec);
  instr->m_val = v->from->offset;
  instr->m_val2 = v->type->size;
  if(is_obj && (is_array || !is_ref)) {
    emit_add_instr(emit, Assign);
    const size_t missing_depth = type->array_depth - (array ? array->depth : 0);
    if(missing_depth && !GET_FLAG(decl->td, force)) {
      const Instr push = emit_add_instr(emit, Reg2Reg);
      push->m_val = -(missing_depth) * SZ_INT;
    }
  }
  return GW_OK;
}

ANN static m_bool emit_exp_decl_global(const Emitter emit, const Var_Decl var_decl,
  const uint is_ref, const uint emit_var) {
  const Value v = var_decl->value;
  const Type type = v->type;
  const Array_Sub array = var_decl->array;
  const m_bool is_array = array && array->exp;
  const m_bool is_obj = isa(type, emit->gwion->type[et_object]) > 0;
  const uint emit_addr = ((is_ref && !array) || isa(type, emit->gwion->type[et_object]) < 0) ?
    emit_var : 1;
  if(is_obj && (is_array || !is_ref))
    CHECK_BB(emit_instantiate_object(emit, type, array, is_ref))
  const Instr instr = emit_kind(emit, v->type->size, emit_addr, dotstatic);
  v->d.ptr = mp_calloc2(emit->gwion->mp, v->type->size);
  SET_FLAG(v, union);
  instr->m_val = (m_uint)v->d.ptr;
  instr->m_val2 = v->type->size;
  if(is_obj && (is_array || !is_ref)) {
    const Instr assign = emit_add_instr(emit, Assign);
    assign->m_val = emit_var;
    const Instr instr = emit_add_instr(emit, RegAddRef);
    instr->m_val = emit_var;
  }
  return GW_OK;
}

ANN static m_bool emit_class_def(const Emitter, const Class_Def);
ANN static m_bool emit_cdef(const Emitter, const Class_Def);

ANN static inline m_bool emit_exp_decl_template(const Emitter emit, const Exp_Decl* decl) {
  const Type t = get_type(decl->type);
  return !GET_FLAG(t, emit) ? emit_cdef(emit, t->e->def) : GW_OK;
}

ANN static m_bool emit_decl(const Emitter emit, const Exp_Decl* decl) {
  const m_bool global = GET_FLAG(decl->td, global);
  const uint var = exp_self(decl)->emit_var;
  const uint ref = GET_FLAG(decl->td, ref) || type_ref(decl->type);
  Var_Decl_List list = decl->list;
  do {
    const uint r = (list->self->array && list->self->array->exp && ref) ? 0 : (uint)(GET_FLAG(list->self->value, ref) + ref);
    if(GET_FLAG(decl->td, static))
      CHECK_BB(emit_exp_decl_static(emit, list->self, r, var))
    else if(!global)
      CHECK_BB(emit_exp_decl_non_static(emit, decl, list->self, r, var))
    else
      CHECK_BB(emit_exp_decl_global(emit, list->self, r, var))
    if(GET_FLAG(list->self->value->type, nonnull))
      emit_add_instr(emit, GWOP_EXCEPT);
  } while((list = list->next));
  return GW_OK;
}

ANN /*static */m_bool emit_exp_decl(const Emitter emit, const Exp_Decl* decl) {
  if(GET_FLAG(decl->type, template))
    CHECK_BB(emit_exp_decl_template(emit, decl))
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
      size += e->type->size;
      vector_add(kinds, e->type->size);
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
  else {
    instr->opcode = eRegPushImm;
    free_vector(emit->gwion->mp, kinds);
  }
}

ANN static m_bool emit_func_args(const Emitter emit, const Exp_Call* exp_call) {
  if(exp_call->args)
    CHECK_BB(emit_exp(emit, exp_call->args, 1))
  if(GET_FLAG(exp_call->m_func->def, variadic))
    emit_func_arg_vararg(emit, exp_call);
  return GW_OK;
}

ANN static m_bool prepare_call(const Emitter emit, const Exp_Call* exp_call) {
  CHECK_BB(emit_func_args(emit, exp_call))
  return emit_exp(emit, exp_call->func, 0);
}

ANN static m_bool emit_exp_call(const Emitter emit, const Exp_Call* exp_call) {
  CHECK_BB(prepare_call(emit, exp_call))
  return emit_exp_call1(emit, exp_call->m_func) ? GW_OK : GW_ERROR;
}

ANN static m_uint get_decl_size(Var_Decl_List a) {
  m_uint size = 0;
  do //if(GET_FLAG(a->self->value, used))
    size += a->self->value->type->size;
  while((a = a->next));
  return size;
}

ANN static m_uint pop_exp_size(const Emitter emit, Exp e) {
  m_uint size = 0;
  do {
    if(e->exp_type == ae_exp_primary &&
        e->d.prim.prim_type == ae_prim_hack) {
      size += pop_exp_size(emit, e->d.prim.d.exp);
      continue;
    }
    size += (e->exp_type == ae_exp_decl ?
        get_decl_size(e->d.exp_decl.list) : e->type->size);
  } while((e = e->next));
  return size;
}

ANN static inline void pop_exp(const Emitter emit, Exp e) {
  const m_uint size = pop_exp_size(emit, e);
  if(size)
   regpop(emit, size);
}

ANN static inline m_bool emit_exp_pop_next(const Emitter emit, Exp e, const m_bool addref) {
  CHECK_BB(emit_exp(emit, e, addref))
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
  struct Op_Import opi = { .op=bin->op, .lhs=lhs->type, .rhs=rhs->type, .pos=exp_self(bin)->pos, .data = (uintptr_t)bin };
  CHECK_BB(emit_exp_pop_next(emit, lhs, 1))
  CHECK_BB(emit_exp_pop_next(emit, rhs, 1))
  return op_emit_bool(emit, &opi);
}

ANN static m_bool emit_exp_cast(const Emitter emit, const Exp_Cast* cast) {
  struct Op_Import opi = { .op=insert_symbol("$"), .lhs=cast->exp->type, .rhs=exp_self(cast)->type, .data=(uintptr_t)cast};
  CHECK_BB(emit_exp(emit, cast->exp, 0))
  (void)op_emit(emit, &opi);
  return GW_OK;
}

ANN static m_bool emit_exp_post(const Emitter emit, const Exp_Postfix* post) {
  struct Op_Import opi = { .op=post->op, .lhs=post->exp->type, .data=(uintptr_t)post };
  CHECK_BB(emit_exp(emit, post->exp, 1))
  return op_emit_bool(emit, &opi);
}

ANN static m_bool is_special(const Emitter emit, const Type t) {
  if(isa(t, emit->gwion->type[et_complex]) > 0 || isa(t, emit->gwion->type[et_polar]) > 0 ||
     isa(t, emit->gwion->type[et_vec3])    > 0 || isa(t, emit->gwion->type[et_vec4])  > 0 ||
     isa(t, emit->gwion->type[et_vararg])  > 0)
    return GW_OK;
  return GW_ERROR;
}

ANN static inline m_bool traverse_emit_func_def(const Emitter emit, const Func_Def fdef) {
  if(!fdef->base->ret_type)
    CHECK_BB(traverse_func_def(emit->env, fdef))
  return emit_func_def(emit, fdef);
}

ANN m_bool traverse_dot_tmpl(const Emitter emit, const struct dottmpl_ *dt) {
  const m_uint scope = emit_push(emit, dt->owner_class, dt->owner);
  const m_bool ret = traverse_emit_func_def(emit, dt->def);
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
  size_t scope = emit_push(emit, v->from->owner_class, v->from->owner);
  const m_bool ret = emit_func_def(emit, f->def);
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
  Instr instr;
  const Type t = actual_type(emit->gwion, f->value_ref->type);
  if(!is_fptr(emit->gwion, t))
    instr = emit_add_instr(emit, !GET_FLAG(f, builtin) ? FuncUsr : SetCode);
  else {
    emit_except(emit, t);
    if(f->def->base->tmpl)
      tmpl_prelude(emit, f);
    instr = emit_add_instr(emit, FuncPtr);
  }
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
  struct Exp_ exp = { .nspc=me->fdef->base->func->value_ref->from->owner };
  struct Op_Import opi = { .op=sym, .lhs=arg->type, .rhs=arg->type, .pos=me->fdef->pos, .data=(uintptr_t)&exp.d };
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
      else {
        const Instr back = (Instr) vector_size(&emit->code->instr) ?
            (Instr)vector_back(&emit->code->instr) : emit_add_instr(emit, RegPushImm);
        back->opcode = ePushStaticCode;
        back->m_val = 0;
      }
    } else if(emit->env->func != f && !f->value_ref->from->owner_class && !f->code && !is_fptr(emit->gwion, f->value_ref->type)) {
      const Instr back = !GET_FLAG(f->def, op) ? emit_add_instr(emit, PushStaticCode) : (Instr)vector_back(&emit->code->instr);
      back->m_val = (m_uint)f;
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

static inline void stack_alloc(const Emitter emit) {
  emit_local(emit, SZ_INT, 0);
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
  if(emit_stmt(emit, stmt, pop) > 0) {
    const m_bool pure = !vector_back(&emit->info->pure);
    if(!pure) {
      gc->opcode = eGcIni;
      emit_add_instr(emit, GcEnd);
    }
  }
  emit_pop_scope(emit);
  --emit->env->scope->depth;
  return GW_OK;
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

ANN static m_bool spork_func(const Emitter emit, const Exp_Call* exp) {
  if(GET_FLAG(exp->m_func, member))
    SET_FLAG(emit->code, member);
  return emit_exp_call1(emit, exp->m_func) ? GW_OK : GW_ERROR;
}

ANN Instr emit_exp_spork(const Emitter emit, const Exp_Unary* unary) {
  const m_bool is_spork = unary->op == insert_symbol("spork");
  const Func f = !unary->code ? unary->exp->d.exp_call.m_func : NULL;
  if(unary->code) {
    emit_add_instr(emit, RegPushImm);
    push_spork_code(emit, is_spork ? SPORK_CODE_PREFIX : FORK_CODE_PREFIX, unary->code->pos);
    if(!SAFE_FLAG(emit->env->func, member))
      stack_alloc_this(emit);
    CHECK_BO(scoped_stmt(emit, unary->code, 0))
  } else {
    CHECK_BO(prepare_call(emit, &unary->exp->d.exp_call))
    push_spork_code(emit, is_spork ? SPORK_FUNC_PREFIX : FORK_CODE_PREFIX, unary->exp->pos);
    CHECK_BO(spork_func(emit, &unary->exp->d.exp_call))
  }
  const VM_Code code = finalyze(emit, EOC);
  const Instr ini = emit_add_instr(emit, unary->op == insert_symbol("spork") ? SporkIni : ForkIni);
  ini->m_val = (m_uint)code;
  ini->m_val2 = is_spork;
  if(!f) {
    if(is_spork) {
      const Instr spork = emit_add_instr(emit, SporkExp);
      spork->m_val = emit->code->stack_depth;
    } else {
      const Instr spork = emit_add_instr(emit, ForkEnd);
      spork->m_val = exp_self(unary)->emit_var;
    }
  } else {
    if(GET_FLAG(f, member) && is_fptr(emit->gwion, f->value_ref->type)) {
      const m_uint depth = f->def->stack_depth;
      regpop(emit, depth -SZ_INT);
      const Instr spork = emit_add_instr(emit, SporkMemberFptr);
      spork->m_val = depth + SZ_INT;
      spork->m_val2 = -SZ_INT*2;
    } else
      emit_exp_spork_finish(emit, f->def->stack_depth);
    const Instr end = emit_add_instr(emit, is_spork ? SporkEnd : ForkEnd);
    end->m_val2 = f->def->base->ret_type->size;
  }
  return ini;
}

ANN static m_bool emit_exp_unary(const Emitter emit, const Exp_Unary* unary) {
  struct Op_Import opi = { .op=unary->op, .data=(uintptr_t)unary };
  if(unary->op != insert_symbol("spork") && unary->op != insert_symbol("fork") && unary->exp) {
    CHECK_BB(emit_exp_pop_next(emit, unary->exp, 1))
    opi.rhs = unary->exp->type;
  }
  return op_emit_bool(emit, &opi);
}

ANN static m_bool emit_implicit_cast(const Emitter emit,
    const restrict Exp  from, const restrict Type to) {
  const struct Implicit imp = { from, to, from->pos };
  struct Op_Import opi = { .op=insert_symbol("@implicit"), .lhs=from->type, .rhs=to, .data=(m_uint)&imp };
  return op_emit_bool(emit, &opi);
}

ANN static Instr _flow(const Emitter emit, const Exp e, const m_bool b) {
  CHECK_BO(emit_exp_pop_next(emit, e, 1))
  struct Op_Import opi = { .op=insert_symbol(b ? "@conditionnal" : "@unconditionnal"),
                           .rhs=e->type, .pos=e->pos, .data=(uintptr_t)e};
  const Instr instr = op_emit(emit, &opi);
  assert(instr != (Instr)GW_OK);
  return instr;
}
#define emit_flow(emit,b) _flow(emit, b, 1)

ANN static m_bool emit_exp_if(const Emitter emit, const Exp_If* exp_if) {
  const Instr op = emit_flow(emit, exp_if->cond);
  CHECK_BB(emit_exp(emit, exp_if->if_exp ?: exp_if->cond, 0))
  const Instr op2 = emit_add_instr(emit, Goto);
  op->m_val = emit_code_size(emit);
  const m_bool ret = emit_exp(emit, exp_if->else_exp, 0);
  op2->m_val = emit_code_size(emit);
  return ret;
}

ANN static m_bool emit_lambda(const Emitter emit, const Exp_Lambda * lambda) {
  CHECK_BB(emit_func_def(emit, lambda->def))
  if(GET_FLAG(lambda->def, member) && !exp_self(lambda)->emit_var)
    emit_add_instr(emit, RegPushMem);
  regpushi(emit, (m_uint)lambda->def->base->func->code);
  return GW_OK;
}

ANN static m_bool emit_exp_lambda(const Emitter emit, const Exp_Lambda * lambda) {
  if(!lambda->def->base->func) {
    regpushi(emit, SZ_INT);
    return GW_OK;
  }
  const m_uint scope = !lambda->owner ?
    emit->env->scope->depth : emit_push_type(emit, lambda->owner);
  const m_bool ret = emit_lambda(emit, lambda);
  if(lambda->owner)
    emit_pop(emit, scope);
  return ret;
}

ANN static m_bool emit_exp_typeof(const Emitter emit, const Exp_Typeof *exp) {
  if(!exp->exp->type->array_depth)
    regpushi(emit, (m_uint)(actual_type(emit->gwion, exp->exp->type)));
  else
    regpushi(emit, (m_uint)exp->exp->type);
  return GW_OK;
}

DECL_EXP_FUNC(emit, m_bool, Emitter)

ANN2(1) static m_bool emit_exp(const Emitter emit, Exp exp, const m_bool ref) {
  do {
    CHECK_BB(emit_exp_func[exp->exp_type](emit, &exp->d))
   if(ref && isa(exp->type, emit->gwion->type[et_object]) > 0) {
      const Instr instr = emit_add_instr(emit, RegAddRef);
      instr->m_val = exp->emit_var;
    }
    if(exp->cast_to)
      CHECK_BB(emit_implicit_cast(emit, exp, exp->cast_to))
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
    CHECK_BB(emit_exp(emit, e->args, 0))
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
    CHECK_BB(emit_exp_pop_next(emit, stmt->val, 0))
    if(isa(stmt->val->type, emit->gwion->type[et_object]) > 0)
      emit_add_instr(emit, RegAddRef);
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

// scope push problem
ANN static m_bool emit_stmt_flow(const Emitter emit, const Stmt_Flow stmt) {
  const m_uint index = emit_code_size(emit);
  Instr op = NULL;
  emit_push_stack(emit);
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
  emit_pop_stack(emit, index);
  return GW_OK;
}

// scope push problem
ANN static m_bool emit_stmt_for(const Emitter emit, const Stmt_For stmt) {
  emit_push_stack(emit);
  CHECK_BB(emit_stmt(emit, stmt->c1, 1))
  const m_uint index = emit_code_size(emit);
  const Instr op = emit_flow(emit, stmt->c2->d.stmt_exp.val);
  CHECK_BB(scoped_stmt(emit, stmt->body, 1))
  const m_uint action_index = emit_code_size(emit);
  if(stmt->c3) {
    CHECK_BB(emit_exp(emit, stmt->c3, 0))
    pop_exp(emit, stmt->c3);
  }
  const Instr _goto = emit_add_instr(emit, Goto);
  _goto->m_val = index;
  op->m_val = emit_code_size(emit);
  emit_pop_stack(emit, action_index);
  return GW_OK;
}

ANN static Instr emit_stmt_autoptr_init(const Emitter emit, const Type type) {
  const Instr new_obj = emit_add_instr(emit, ObjectInstantiate);
  new_obj->m_val2 = (m_uint)type;
  emit_add_instr(emit, RegAddRef);
  regpop(emit, SZ_INT);
  return emit_add_instr(emit, Reg2Mem);
}

// scope push problem
ANN static m_bool emit_stmt_auto(const Emitter emit, const Stmt_Auto stmt) {
  CHECK_BB(emit_exp(emit, stmt->exp, 0))
  const Instr s1 = emit_add_instr(emit, MemSetImm);
  emit_push_stack(emit);
  Instr cpy = stmt->is_ptr ? emit_stmt_autoptr_init(emit, stmt->v->type) : NULL;
  const m_uint ini_pc  = emit_code_size(emit);
  emit_except(emit, stmt->exp->type);
  const Instr loop = emit_add_instr(emit, stmt->is_ptr ? AutoLoopPtr : AutoLoop);
  const Instr end = emit_add_instr(emit, BranchEqInt);
  const m_uint offset = emit_local(emit, SZ_INT + stmt->v->type->size, 0);
  stmt->v->from->offset = offset + SZ_INT;
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  const m_uint end_pc = emit_code_size(emit);
  if(stmt->is_ptr) {
    loop->m_val2 = (m_uint)stmt->v->type;
    cpy->m_val = stmt->v->from->offset;
  }
  const Instr tgt = emit_add_instr(emit, Goto);
  end->m_val = emit_code_size(emit);
  tgt->m_val = ini_pc;
  s1->m_val = loop->m_val = offset;
  regpop(emit, SZ_INT);
  emit_pop_stack(emit, end_pc);
  return GW_OK;
}

// scope push problem
ANN static m_bool emit_stmt_loop(const Emitter emit, const Stmt_Loop stmt) {
  emit_push_stack(emit);
  CHECK_BB(emit_exp_pop_next(emit, stmt->cond, 0))
  const m_uint index = emit_code_size(emit);
  const Instr cpy = emit_add_instr(emit, Reg2RegAddr);
  cpy->m_val2 = -SZ_INT;
  emit_add_instr(emit, int_post_dec);
  const Instr op = emit_add_instr(emit, BranchEqInt);
  CHECK_BB(scoped_stmt(emit, stmt->body, 1))
  const Instr _goto = emit_add_instr(emit, Goto);
  _goto->m_val = index;
  op->m_val = emit_code_size(emit);
  emit_pop_stack(emit, index);
  regpop(emit, SZ_INT);
  return GW_OK;
}

ANN static m_bool emit_stmt_jump(const Emitter emit, const Stmt_Jump stmt) {
  if(!stmt->is_label)
    stmt->data.instr = emit_add_instr(emit, Goto);
  else {
    assert(stmt->data.v.ptr);
    const m_uint size = vector_size(&stmt->data.v);
    if(!size)
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
      v->from->offset = emit_local(emit, SZ_INT, 0);
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
        new_id_list(emit->gwion->mp, udef->xid, loc_cpy(emit->gwion->mp, udef->pos)));
    type_decl->flag = udef->flag;
    const Var_Decl var_decl = new_var_decl(emit->gwion->mp, udef->xid, NULL, loc_cpy(emit->gwion->mp, udef->pos));
    const Var_Decl_List var_decl_list = new_var_decl_list(emit->gwion->mp, var_decl, NULL);
    const Exp exp = new_exp_decl(emit->gwion->mp, type_decl, var_decl_list);
    exp->d.exp_decl.type = udef->value->type;
    SET_FLAG(udef->value->type, emit);
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
    SET_FLAG(udef->type, emit);
    scope = emit_push_type(emit, udef->type);
  } else if(global) {
    void* ptr = (void*)xcalloc(1, udef->s);
    l = udef->l;
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
  return GW_OK;
}

ANN static m_bool emit_stmt_exp(const Emitter emit, const struct Stmt_Exp_* exp) {
  return exp->val ? emit_exp(emit, exp->val, 0) : 1;
}

ANN static m_bool emit_case_head(const Emitter emit, const Exp base,
    const Exp e, const Symbol op, const Vector v) {
  CHECK_BB(emit_exp(emit, base, 1))
  const Exp next = e->next;
  e->next = NULL;
  const m_bool ret = emit_exp(emit, e, 1);
  e->next = next;
  CHECK_BB(ret)
  const Exp_Binary bin = { .lhs=base, .rhs=e, .op=op };
  struct Exp_ ebin = { .d={.exp_binary=bin}, .nspc=e->nspc};
  struct Op_Import opi = { .op=op, .lhs=base->type, .rhs=e->type, .data=(uintptr_t)&ebin.d.exp_binary, .pos=e->pos };
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
  v->from->offset = emit_local(emit, base->type->size, isa(base->type, emit->gwion->type[et_object]) > 0);
  CHECK_BB(emit_exp(emit, base, 1))
  regpop(emit, base->type->size);
  const Instr instr = emit_add_instr(emit, Reg2Mem4);
  instr->m_val = v->from->offset;
  instr->m_val2 = base->type->size;
  return GW_OK;
}


#define CASE_PASS (Symbol)1
ANN static Symbol case_op(const Emitter emit, const Exp base, const Exp e) {
  if(e->exp_type == ae_exp_primary) {
    if(e->d.prim.prim_type == ae_prim_unpack)
      return insert_symbol("@=>");
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
  do CHECK_BB(emit_stmt_match_case(emit, &list->stmt->d.stmt_match)) // beware push
  while((list = list->next));
  return GW_OK;
}

ANN static m_bool emit_match(const Emitter emit, const struct Stmt_Match_* stmt) {
  if(stmt->where)
    CHECK_BB(emit_stmt(emit, stmt->where, 1)) // beware, we have push scope
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

ANN static m_bool emit_dot_static_import_data(const Emitter emit, const Value v, const uint emit_addr) {
  if(v->d.ptr && GET_FLAG(v, builtin) && GET_FLAG(v, const)) {
    const m_uint size = v->type->size;
    const Instr instr = emit_kind(emit, size, emit_addr, regpushimm);
    instr->m_val = (m_uint)v->d.ptr;
    instr->m_val2 = size;
    return GW_OK;
  }
  return emit_dot_static_data(emit, v, emit_addr);
}

ANN static m_bool emit_complex_member(const Emitter emit, const Exp_Dot* member) {
  const Exp base = member->base;
  base->emit_var = 1;
  CHECK_BB(emit_exp(emit, base, 0))
  const m_bool is_complex = !strcmp((isa(base->type, emit->gwion->type[et_complex]) > 0  ? "re" : "phase") ,
        s_name(member->xid));
  if(is_complex && exp_self(member)->emit_var)
    return GW_OK;
  const Instr instr = emit_add_instr(emit, is_complex ? ComplexReal : ComplexImag);
  instr->m_val = exp_self(member)->emit_var;
  return GW_OK;
}

ANN static m_bool emit_VecMember(const Emitter emit, const Exp_Dot* member) {
  member->base->emit_var = 1;
  CHECK_BB(emit_exp(emit, member->base, 0))
  const Value v = find_value(member->base->type, member->xid);
  if(GET_FLAG(v, func)) {
    regpushi(emit, (m_uint)v->d.func_ref->code);
    return GW_OK;
  }
  if(!v->from->offset && exp_self(member)->emit_var)
    return GW_OK;
  const Instr instr = emit_add_instr(emit, VecMember);
  instr->m_val2 = v->from->offset;
  instr->m_val = exp_self(member)->emit_var;
  return GW_OK;
}

ANN static m_bool emit_vararg_start(const Emitter emit, const m_uint offset) {
  const Instr instr = emit_add_instr(emit, VarargTop);
  instr->m_val = offset;
  instr->m_val2 = emit_code_size(emit);
  vector_set(&emit->info->variadic, vector_size(&emit->info->variadic) -1, (vtype)instr);
  return GW_OK;
}
ANN static inline Instr get_variadic(const Emitter emit) {
  return (Instr)vector_back(&emit->info->variadic);
}

ANN static void emit_vararg_end(const Emitter emit, const m_uint offset) {
  const Instr instr = emit_add_instr(emit, VarargEnd),
    variadic = get_variadic(emit);
  instr->m_val = offset;
  instr->m_val2 = variadic->m_val2;
  variadic->m_val2 = emit_code_size(emit);
  SET_FLAG(emit->env->func, empty);// mark vararg func as complete
}

ANN static m_bool emit_vararg(const Emitter emit, const Exp_Dot* member) {
  m_uint offset = emit->env->class_def ? SZ_INT : 0;
  Arg_List l = emit->env->func->def->base->args;
  const m_str str = s_name(member->xid);
  while(l) {
    offset += l->type->size;
    l = l->next;
  }
  if(!strcmp(str, "start")) {
    if(get_variadic(emit))
      ERR_B(exp_self(member)->pos, _("vararg.start already used"))
    emit_vararg_start(emit, offset);
    return GW_OK;
  }
  if(!strcmp(str, "end")) {
    if(!get_variadic(emit))
      ERR_B(exp_self(member)->pos, _("vararg.start not used before vararg.end"))
    emit_vararg_end(emit, offset);
    return GW_OK;
  }
  if(!get_variadic(emit))
      ERR_B(exp_self(member)->pos, _("vararg.%s used before vararg.start"), s_name(member->xid))
  if(GET_FLAG(emit->env->func, empty))
    ERR_B(exp_self(member)->pos, _("vararg.%s used after vararg.end"), s_name(member->xid))
  const Instr instr = emit_add_instr(emit, VarargMember);
  instr->m_val = offset;
  instr->m_val2 = exp_self(member)->type->size;
  return GW_OK;
}

ANN static m_bool emit_exp_dot_special(const Emitter emit, const Exp_Dot* member) {
  const Type t = member->t_base;
  if(isa(t, emit->gwion->type[et_complex]) > 0 || isa(t, emit->gwion->type[et_polar]) > 0)
    return emit_complex_member(emit, member);
  else if(isa(t, emit->gwion->type[et_vec3]) > 0 || isa(t, emit->gwion->type[et_vec4]) > 0)
    return emit_VecMember(emit, member);
  return emit_vararg(emit, member);
}

ANN static m_bool emit_member_func(const Emitter emit, const Exp_Dot* member) {
  const Func f = exp_self(member)->type->e->d.func;
  if(is_class(emit->gwion, member->t_base) || GET_FLAG(member->base->type, force)) {
    const Instr func_i = emit_add_instr(emit, f->code ? RegPushImm : PushStaticCode);
    func_i->m_val = (m_uint)f->code;
    return GW_OK;
  }
  if(f->def->base->tmpl)
    emit_add_instr(emit, DotTmplVal);
  else {
    const Instr instr = emit_add_instr(emit, GET_FLAG(f, member) ? DotFunc : DotStaticFunc);
    instr->m_val = f->vt_index;
  }
  return GW_OK;
}

ANN static inline m_bool emit_member(const Emitter emit, const Value v, const uint emit_addr) {
  const m_uint size = v->type->size;
  const Instr instr = emit_kind(emit, size, emit_addr, dotmember);
  instr->m_val = v->from->offset;
  instr->m_val2 = size;
  return GW_OK;
}

ANN static m_bool emit_exp_dot(const Emitter emit, const Exp_Dot* member) {
  if(is_special(emit, member->t_base) > 0)
    return emit_exp_dot_special(emit, member);
  const Value value = find_value(actual_type(emit->gwion, member->t_base), member->xid);
  if(!is_class(emit->gwion, member->t_base) && (GET_FLAG(value, member) ||
(isa(exp_self(member)->type, emit->gwion->type[et_function]) > 0 && !is_fptr(emit->gwion, exp_self(member)->type)))
) {
    CHECK_BB(emit_exp(emit, member->base, 0))
    emit_add_instr(emit, GWOP_EXCEPT);

//    emit_except(emit, member->t_base);
  }
  if(isa(exp_self(member)->type, emit->gwion->type[et_function]) > 0 && !is_fptr(emit->gwion, exp_self(member)->type))
    return emit_member_func(emit, member);
  return (GET_FLAG(value, member) ? emit_member : emit_dot_static_import_data) (emit, value, exp_self(member)->emit_var);
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
    const Value  value = a->var_decl->value;
    const m_uint size = value->type->size;
    const m_bool obj = isa(value->type, emit->gwion->type[et_object]) > 0;
    emit->code->stack_depth += size;
    value->from->offset = emit_local(emit, size, obj);
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

ANN static m_bool _fdef_body(const Emitter emit, const Func_Def fdef) {
  if(fdef->base->args)
    emit_func_def_args(emit, fdef->base->args);
  if(GET_FLAG(fdef, variadic))
    stack_alloc(emit);
  if(fdef->d.code)
    CHECK_BB(emit_stmt_code(emit, &fdef->d.code->d.stmt_code))
  emit_func_def_ensure(emit, fdef);
  return GW_OK;
}

ANN static m_bool emit_func_def_body(const Emitter emit, const Func_Def fdef) {
  vector_add(&emit->info->variadic, 0);
  CHECK_BB(_fdef_body(emit, fdef))
  if(GET_FLAG(fdef, variadic)) {
    if(!get_variadic(emit))
      ERR_B(fdef->pos, _("invalid variadic use"))
    if(!GET_FLAG(fdef->base->func, empty))
      ERR_B(fdef->pos, _("invalid variadic use"))
  }
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

ANN static m_bool emit_func_def(const Emitter emit, const Func_Def fdef) {
  const Func func = fdef->base->func;
  const Func former = emit->env->func;
  if(tmpl_base(fdef->base->tmpl))
    return GW_OK;
  if(SAFE_FLAG(emit->env->class_def, builtin) && GET_FLAG(emit->env->class_def, template))
    return GW_OK;
  if(!emit->env->class_def && !GET_FLAG(fdef, global) && !fdef->base->tmpl && !emit->env->scope->depth)
    func->value_ref->from->offset = emit_local(emit, SZ_INT, 0);
  emit_func_def_init(emit, func);
  if(GET_FLAG(func, member))
    stack_alloc_this(emit);
  emit->env->func = func;
  emit_push_scope(emit);
  const m_bool ret = scanx_fdef(emit->env, emit, fdef, (_exp_func)emit_fdef);
  emit_pop_scope(emit);
  emit->env->func = former;
  if(ret > 0)
    emit_fdef_finish(emit, fdef);
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
if(GET_FLAG(cdef->base.type, emit))return GW_OK;
  const Type type = cdef->base.type;
  const Nspc nspc = type->nspc;
  if(cdef->base.ext && cdef->base.type->e->parent->e->def && !GET_FLAG(cdef->base.type->e->parent, emit))
    CHECK_BB(cdef_parent(emit, cdef))
  SET_FLAG(type, emit);
  nspc_allocdata(emit->gwion->mp, nspc);
  emit_class_code(emit, type->name);
  if(cdef->body)
    CHECK_BB(scanx_body(emit->env, cdef, (_exp_func)emit_section, emit))
  emit_class_finish(emit, nspc);
  SET_FLAG(type, emit);
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
