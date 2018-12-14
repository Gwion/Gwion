#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "nspc.h"
#include "value.h"
#include "instr.h"
#include "emit.h"
#include "object.h"
#include "array.h"
#include "func.h"
#include "traverse.h"
#include "template.h"
#include "escape.h"
#include "parse.h"
#include "memoize.h"
#include "operator.h"
#include "import.h"

typedef struct Local_ {
  m_uint size;
  m_uint offset;
  m_bool is_obj;
} Local;

static inline void emit_pop_type(const Emitter emit) { nspc_pop_type(emit->env->curr); }
static inline void emit_pop(const Emitter emit, const m_uint scope) { env_pop(emit->env, scope); }
static inline void emit_push(const Emitter emit, const Type type, const Nspc nspc, m_uint *scope) {
  env_push(emit->env, type, nspc, scope);
}

ANEW static Frame* new_frame() {
  Frame* frame = mp_alloc(Frame);
  vector_init(&frame->stack);
  vector_add(&frame->stack, (vtype)NULL);
  return frame;
}

ANN static void free_frame(Frame* a) {
  LOOP_OPTIM
  for(vtype i = vector_size(&a->stack) + 1; --i;)
    if(vector_at(&a->stack, i - 1))
      mp_free(Local, (Local*)vector_at(&a->stack, i - 1));
  vector_release(&a->stack);
  mp_free(Frame, a);
}

ANN static Local* new_local(const m_uint size, const m_bool is_obj) {
  Local* local  = mp_alloc(Local);
  local->size   = size;
  local->is_obj = is_obj;
  return local;
}

ANN static m_uint frame_local(Frame* frame, const m_uint size, const m_bool is_obj) {
  Local* local = new_local(size, is_obj);
  local->offset = frame->curr_offset;
  frame->curr_offset += size;
  vector_add(&frame->stack, (vtype)local);
  return local->offset;
}

ANN static inline void frame_push(Frame* frame) {
  vector_add(&frame->stack, (vtype)NULL);
}

ANN static m_int frame_pop(Frame* frame) {
  const Local* l = (Local*)vector_pop(&frame->stack);
  CHECK_OB(l)
  frame->curr_offset -= l->size;
  return l->is_obj ? (m_int)l->offset : frame_pop(frame);
}

ANN static m_bool emit_exp(const Emitter emit, Exp exp, const m_bool add_ref);
ANN static m_bool emit_stmt(const Emitter emit, const Stmt stmt, const m_bool pop);
ANN static m_bool emit_stmt_list(const Emitter emit, Stmt_List list);
ANN static m_bool emit_exp_dot(const Emitter emit, const Exp_Dot* member);
ANN static m_bool emit_func_def(const Emitter emit, const Func_Def func_def);

ANEW Emitter new_emitter(void) {
  Emitter emit = (Emitter)xcalloc(1, sizeof(struct Emitter_));
  vector_init(&emit->stack);
  return emit;
}

ANN void free_emitter(Emitter a) {
  vector_release(&a->stack);
  xfree(a);
}

__attribute__((returns_nonnull))
ANN2(1) Instr emit_add_instr(const Emitter emit, const f_instr f) {
  const Instr instr = mp_alloc(Instr);
  instr->execute = f;
  vector_add(&emit->code->instr, (vtype)instr);
  return instr;
}

ANEW static Code* new_code(const Emitter emit, const m_str name) {
  Code* code = mp_alloc(Code);
  code->name = code_name_set(name, emit->env->name);
  code->stack_depth = 0;
  vector_init(&code->instr);
  vector_init(&code->stack_break);
  vector_init(&code->stack_cont);
  vector_init(&code->stack_return);
  code->frame = new_frame();
  return code;
}

ANN static void free_code(Code* code) {
  vector_release(&code->instr);
  vector_release(&code->stack_break);
  vector_release(&code->stack_cont);
  vector_release(&code->stack_return);
  free_frame(code->frame);
  free(code->name);
  mp_free(Code, code);
}

ANN static void emit_pop_scope(const Emitter emit) { GWDEBUG_EXE
  m_int offset;
  while((offset = frame_pop(emit->code->frame)) > -1) {
    Instr instr = emit_add_instr(emit, ObjectRelease);
    instr->m_val = (m_uint)offset;
  }
}

ANN static inline void emit_push_code(const Emitter emit, const m_str name) {
  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code(emit, name);
}

ANN static inline void emit_pop_code(const Emitter emit)   {
  emit->code = (Code*)vector_pop(&emit->stack);
}

ANN static inline void emit_push_scope(const Emitter emit) {
  frame_push(emit->code->frame);
}

ANN static inline m_uint emit_code_size(const Emitter emit) {
  return vector_size(&emit->code->instr);
}

ANN static inline m_uint emit_code_offset(const Emitter emit) {
  return emit->code->frame->curr_offset;
}

ANN static inline m_uint emit_local(const Emitter emit, const m_uint size, const m_bool is_obj) { GWDEBUG_EXE
  return frame_local(emit->code->frame, size, is_obj);
}

ANN static void emit_pre_ctor_inner(const Emitter emit, const Type type) { GWDEBUG_EXE
  const Instr instr = emit_add_instr(emit, PreCtor);
  instr->m_val = (m_uint)type->nspc->pre_ctor;
  instr->m_val2 = (m_uint)emit_code_offset(emit);
}

ANN static void emit_pre_ctor(const Emitter emit, const Type type) { GWDEBUG_EXE
  if(type->parent)
    emit_pre_ctor(emit, type->parent);
  if(type->nspc->pre_ctor)
    emit_pre_ctor_inner(emit, type);
  if(GET_FLAG(type, template) && GET_FLAG(type, builtin)) {
    const Type t = template_parent(type);
    if(t->nspc->pre_ctor)
      emit_pre_ctor_inner(emit, t);
  }
}

ANN static void emit_pre_constructor_array(const Emitter emit, const Type type) {
  const m_uint start_index = emit_code_size(emit);
  const Instr top = emit_add_instr(emit, ArrayTop);
  top->m_val2 = (m_uint)type;
  emit_pre_ctor(emit, type);
  const Instr bottom = emit_add_instr(emit, ArrayBottom);
  top->m_val = emit_code_size(emit);
  bottom->m_val = start_index;
  emit_add_instr(emit, ArrayPost);
}

ANN ArrayInfo* emit_array_extend_inner(const Emitter emit, const Type t, const Exp e) { GWDEBUG_EXE
  CHECK_BO(emit_exp(emit, e, 0))
  const Type base = array_base(t);
  ArrayInfo* info = mp_alloc(ArrayInfo);
  vector_init(&info->type);
  for(m_uint i = 1; i < t->array_depth; ++i)
    vector_add(&info->type, (vtype)array_type(base, i));
  vector_add(&info->type, (vtype)t);
  info->depth = (m_int)t->array_depth;
  info->base = base;
  const Instr alloc = emit_add_instr(emit, ArrayAlloc);
  *(ArrayInfo**)alloc->ptr = info;
  if(isa(base, t_object) > 0) {
    emit_pre_constructor_array(emit, base);
    info->is_obj = 1;
  }
  return info;
}

ANN void emit_ext_ctor(const Emitter emit, const VM_Code code) { GWDEBUG_EXE
  emit_add_instr(emit, RegDup);
  const Instr push_f = emit_add_instr(emit, RegPushImm);
  *(VM_Code*)push_f->ptr = code;
  const Instr offset = emit_add_instr(emit, RegPushImm);
  *(m_uint*)offset->ptr = emit_code_offset(emit);
  emit_add_instr(emit, FuncMember);
}

ANN m_bool emit_array_extend(const Emitter emit, const Type t, const Exp e) { GWDEBUG_EXE
  CHECK_OB(emit_array_extend_inner(emit, t, e))
  return !!emit_add_instr(emit, PopArrayClass);
}

ANN2(1,2) m_bool emit_instantiate_object(const Emitter emit, const Type type,
    const Array_Sub array, const uint is_ref) {
  if(type->array_depth) {
    assert(array && array->exp);
    ArrayInfo* info = emit_array_extend_inner(emit, type, array->exp);
    CHECK_OB(info)
    info->is_ref = !!is_ref;
  } else if(!is_ref) {
    const Instr instr = emit_add_instr(emit, ObjectInstantiate);
    instr->m_val = (m_uint)type;
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

static const f_instr regpushimm[] = { RegPushImm, RegPushImm2, RegPushImm3, RegPushDeref }; // caution last
static const f_instr regpushderef[] = { RegPushDeref, RegPushDeref2, RegPushDeref3,  RegPushDeref }; // caution last
static const f_instr regpushmem[] = { RegPushMem, RegPushMem2, RegPushMem3, RegPushMem4 };
static const f_instr dotstatic[]  = { DotStatic, DotStatic2, DotStatic3, DotStatic4 };
static const f_instr dotimport[]  = { DotImport, DotImport2, DotImport3, DotImport4 };
static const f_instr dotmember[]  = { DotMember, DotMember2, DotMember3, DotMember4 };
static const f_instr allocmember[]  = { PushNull, PushNull2, PushNull3, AllocMember4 };
static const f_instr allocword[]  = { AllocWord, AllocWord2, AllocWord3, AllocWord4 };

ANN static m_bool emit_symbol_owned(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  const Value v = prim->value;
  const Exp exp = new_exp_prim_id(insert_symbol("this"), prim->self->pos);
  const Exp dot = new_exp_dot(exp, prim->d.var);
  exp->type = v->owner_class;
  dot->d.exp_dot.t_base = v->owner_class;
  dot->type = v->type;
  dot->emit_var = prim->self->emit_var;
  const m_bool ret = emit_exp_dot(emit, &dot->d.exp_dot);
  free_exp(exp);
  return ret;
}

ANN static m_bool emit_symbol_builtin(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  const Value v = prim->value;
  if(GET_FLAG(v, func)) {
    const Instr instr = emit_add_instr(emit, RegPushImm);
    *(Func*)instr->ptr = v->d.func_ref;
    return GW_OK;
  }
  if(GET_FLAG(v, union)) {
    const m_uint size = v->type->size;
    const Instr instr = emit_kind(emit, size, prim->self->emit_var, regpushderef);
    *(m_uint*)instr->ptr = (m_uint)v->d.ptr;
  } else {
    const m_uint size = v->type->size;
    const Instr instr = emit_kind(emit, size, prim->self->emit_var, regpushimm);
    if(isa(v->type, t_object) < 0) {
      if(v->d.ptr)
        memcpy(instr->ptr, v->d.ptr, v->type->size);
    } else
      *(m_uint**)instr->ptr = v->d.ptr;
  }
  return GW_OK;
}

ANN static m_bool emit_symbol(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  const Value v = prim->value;
  if(v->owner_class)
    return emit_symbol_owned(emit, prim);
  if(GET_FLAG(v, builtin) || GET_FLAG(v, enum) ||
      GET_FLAG(v, union))
    return emit_symbol_builtin(emit, prim);
  const m_uint size = v->type->size;
  const Instr instr = emit_kind(emit, size, prim->self->emit_var, regpushmem);
  instr->m_val  = v->offset;
  *(m_uint*)instr->ptr = GET_FLAG(v, global);
  return GW_OK;
}

ANEW ANN VM_Code emit_code(const Emitter emit) { GWDEBUG_EXE
  Code* c = emit->code;
  const VM_Code code = new_vm_code(&c->instr, c->stack_depth,
      GET_FLAG(c, member), c->name);
  free_code(c);
  return code;
}

ANN static VM_Code finalyze(const Emitter emit) {
  emit_add_instr(emit, EOC);
  const VM_Code code = emit_code(emit);
  emit_pop_code(emit);
  return code;
}

ANN static m_bool prim_array(const Emitter emit, const Exp_Primary * primary) {
  const Array_Sub array = primary->d.array;
  Exp e = array->exp;
  CHECK_BB(emit_exp(emit, e, 0))
  m_uint count = 0;
  do ++count;
  while((e = e->next));
  const Type type = array->type;
  const Type base = array_base(type);
  const Instr instr = emit_add_instr(emit, ArrayInit);
  instr->m_val = count;
  instr->m_val2 = base->size;
  *(Type*)instr->ptr = type;
  return GW_OK;
}

ANN static m_uint get_depth(Type t) {
  m_uint depth = 0;
  do depth += t->array_depth;
  while((t = t->parent));
  return depth;
}

ANN static m_bool emit_exp_array(const Emitter emit, const Exp_Array* array) { GWDEBUG_EXE
  const m_uint is_var = array->self->emit_var;
  const m_uint depth = get_depth(array->base->type) - array->self->type->array_depth;
  CHECK_BB(emit_exp(emit, array->base, 0))
  CHECK_BB(emit_exp(emit, array->array->exp, 0))
  if(depth == 1) {
    const Instr instr = emit_add_instr(emit, ArrayAccess);
    instr->m_val = is_var;
    instr->m_val2 = is_var ? SZ_INT : array->self->type->size;
  } else {
    const Instr instr = emit_add_instr(emit, ArrayAccessMulti);
    instr->m_val = is_var || array->self->type->array_depth;
    instr->m_val2 = (is_var || array->self->type->array_depth) ?
      SZ_INT : array_base(array->base->type)->size;
    *(m_uint*)instr->ptr = depth;
  }
  return GW_OK;
}

ANN static m_bool prim_vec(const Emitter emit, const Exp_Primary * primary) { GWDEBUG_EXE
  const Vec * vec = &primary->d.vec;
  const ae_prim_t t = primary->primary_type;
  CHECK_BB(emit_exp(emit, vec->exp, 0));
  m_int n = (m_int)((t == ae_primary_vec ? 3 : 2) - vec->dim + 1);
  while(--n > 0)
    emit_add_instr(emit, PushNull2);
  return GW_OK;
}

ANN static m_bool prim_id(const Emitter emit, const Exp_Primary* prim) {
  if(prim->d.var == insert_symbol("this"))
    emit_add_instr(emit, RegPushMem);
  else if(prim->d.var == insert_symbol("me"))
    emit_add_instr(emit, RegPushMe);
  else if(prim->d.var == insert_symbol("now"))
    emit_add_instr(emit, RegPushNow);
  else if(prim->d.var == insert_symbol("maybe"))
    emit_add_instr(emit, RegPushMaybe);
  else if(prim->d.var == insert_symbol("__func__")) {
    const Instr instr = emit_add_instr(emit, RegPushStr);
    instr->m_val = (m_uint)s_name(insert_symbol(emit->env->func ?
      emit->env->func->name : emit->env->class_def ?
      emit->env->class_def->name : emit->env->name));
  } else
    emit_symbol(emit, prim);
  return GW_OK;
}

ANN static m_bool prim_num(const Emitter emit, const Exp_Primary * primary) {
  const Instr instr = emit_add_instr(emit, RegPushImm);
  *(m_uint*)instr->ptr = primary->d.num;
  return GW_OK;
}

ANN static m_bool prim_float(const Emitter emit, const Exp_Primary* primary) {
  const Instr instr = emit_add_instr(emit, RegPushImm2);
  *(m_float*)instr->ptr = primary->d.fnum;
  return GW_OK;
}

ANN static m_bool prim_char(const Emitter emit, const Exp_Primary* prim) {
  const m_int c = str2char(prim->d.chr, prim->self->pos);
  CHECK_BB(c);
  const Instr instr = emit_add_instr(emit, RegPushImm);
  *(m_int*)instr->ptr = c;
  return GW_OK;
}

ANN static m_bool prim_str(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  char c[strlen(prim->d.str)];
  strcpy(c, prim->d.str);
  CHECK_BB(escape_str(c, prim->self->pos));
  const Instr instr = emit_add_instr(emit, RegPushStr);
  instr->m_val = (m_uint)s_name(insert_symbol(c));
  return GW_OK;
}

ANN static m_bool prim_gack(const Emitter emit, const Exp_Primary* primary) {
  const Exp exp = primary->d.exp;
  const Vector v = new_vector();
  m_uint offset = 0;
  Exp e = exp;
  do {
    vector_add(v, (vtype)e->type);
    offset += e->type->size;
    if(e->type != emit->env->class_def)
      ADD_REF(e->type);
  } while((e = e->next));
  if(emit_exp(emit, exp, 0) < 0) {
    free_vector(v);
    ERR_B(exp->pos, "\t... in 'gack' expression.")
  }
  const Instr instr = emit_add_instr(emit, Gack);
  *(Vector*)instr->ptr = v;
  instr->m_val = offset;
  return GW_OK;
}

#ifdef OPTIMIZE
ANN static m_bool prim_constprop(Emitter emit, const Exp_Primary* prim) {
  const Instr instr = emit_add_instr(emit, ConstPropSet);
  instr->m_val = prim->value->offset;
  instr->m_val2 = prim->d.num;
  *(m_bool*)instr->ptr = prim->self->emit_var;
  return GW_OK;
}
#endif

static const _exp_func prim_func[] = {
  (_exp_func)prim_id, (_exp_func)prim_num, (_exp_func)prim_float, (_exp_func)prim_str,
  (_exp_func)prim_array, (_exp_func)prim_gack, (_exp_func)prim_vec, (_exp_func)prim_vec,
  (_exp_func)prim_vec, (_exp_func)prim_char, (_exp_func)dummy_func,
#ifdef OPTIMIZE
  (_exp_func)prim_constprop
#endif
};

ANN static m_bool emit_exp_primary(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  return prim_func[prim->primary_type](emit, prim);
}

ANN static m_bool emit_dot_static_data(const Emitter emit, const Value v, const uint  emit_var) { GWDEBUG_EXE
  const Instr push = emit_add_instr(emit, RegPushImm);
  *(Type*)push->ptr = v->owner_class;
  const m_uint size = v->type->size;
  const Instr instr = emit_kind(emit, size, emit_var, dotstatic);
  instr->m_val = v->offset;
  return GW_OK;
}

ANN static m_bool decl_static(const Emitter emit, const Var_Decl var_decl, const uint is_ref) {
  const Value v = var_decl->value;
  Code* code = emit->code;
  emit->code = (Code*)vector_back(&emit->stack);
  CHECK_BB(emit_instantiate_object(emit, v->type, var_decl->array, is_ref))
  CHECK_BB(emit_dot_static_data(emit, v, 1))
  emit_add_instr(emit, ObjectAssign);
  emit->code = code;
  return GW_OK;
}

ANN static m_bool emit_exp_decl_static(const Emitter emit, const Var_Decl var_decl, const uint is_ref) { GWDEBUG_EXE
  const Value value = var_decl->value;
  if(isa(value->type, t_object) > 0 && !is_ref)
    CHECK_BB(decl_static(emit, var_decl, 0))
  return emit_dot_static_data(emit, value, 1);
}

ANN static m_bool emit_exp_decl_non_static(const Emitter emit, const Var_Decl var_decl,
  const uint is_ref, const uint emit_var) { GWDEBUG_EXE
  const Value v = var_decl->value;
  const Type type = v->type;
  const Array_Sub array = var_decl->array;
  const m_bool is_array = array && array->exp;
  const m_bool is_obj = isa(type, t_object) > 0;
  const uint emit_addr = ((is_ref && !array) || isa(type, t_object) < 0) ?
    emit_var : 1;
  if(is_obj && (is_array || !is_ref))
    CHECK_BB(emit_instantiate_object(emit, type, array, is_ref))
  f_instr *exec = (f_instr*)allocmember;
  if(!GET_FLAG(v, member)) {
    v->offset = emit_local(emit, v->type->size, is_obj);
    exec = (f_instr*)allocword;
  }
  const Instr instr = emit_kind(emit, v->type->size, emit_addr, exec);
  instr->m_val = v->offset;
  if(is_obj && (is_array || !is_ref)) {
    const Instr assign = emit_add_instr(emit, ObjectAssign);
    assign->m_val = (m_uint)emit_var;
    if(is_array && !emit->env->scope)
      ADD_REF(type)
  }
  return GW_OK;
}

ANN static m_bool emit_class_def(const Emitter, const Class_Def);

ANN static m_bool emit_exp_decl_template(const Emitter emit, const Exp_Decl* decl) { GWDEBUG_EXE
  const Type t = typedef_base(decl->type);
  if(!GET_FLAG(t, emit)) {
    CHECK_BB(template_push_types(emit->env, t->def->tmpl->list.list, t->def->tmpl->base))
    CHECK_BB(emit_class_def(emit, t->def))
    emit_pop_type(emit);
  }
  return GW_OK;
}

ANN static m_bool emit_exp_decl(const Emitter emit, const Exp_Decl* decl) { GWDEBUG_EXE
  Var_Decl_List list = decl->list;
  const uint  ref = GET_FLAG(decl->td, ref) || type_ref(decl->type);
  const uint var = decl->self->emit_var;

  if(GET_FLAG(decl->type, template))
    CHECK_BB(emit_exp_decl_template(emit, decl))
  m_uint scope;
  const m_bool global = GET_FLAG(decl->td, global);
  if(global)
    emit_push(emit, NULL, emit->env->global_nspc, &scope);
  do {
    const uint r = (uint)(GET_FLAG(list->self->value, ref) + ref);
    if(!GET_FLAG(list->self->value, used))
      continue;
    if(GET_FLAG(decl->td, static))
      CHECK_BB(emit_exp_decl_static(emit, list->self, r))
    else
      CHECK_BB(emit_exp_decl_non_static(emit, list->self, r, var))
  } while((list = list->next));
  if(global)
    emit_pop(emit, scope);
  return GW_OK;
}

ANN static m_uint vararg_size(const Exp_Call* exp_call, const Vector kinds) {
  Exp e = exp_call->args;
  Arg_List l = exp_call->m_func->def->arg_list;
  m_uint size = 0;
  while(e) {
    if(!l) {
      size += e->type->size;
      if(e->type->size)
        vector_add(kinds, e->type->size);
    } else
      l = l->next;
    e = e->next;
  }
  return size;
}

ANN static m_bool emit_func_arg_vararg(const Emitter emit, const Exp_Call* exp_call) { GWDEBUG_EXE
  const Vector kinds = new_vector();
  const Instr instr = emit_add_instr(emit, VarargIni);
  instr->m_val = vararg_size(exp_call, kinds);
  instr->m_val2 = (m_uint)kinds;
  return GW_OK;
}

ANN static m_bool emit_func_args(const Emitter emit, const Exp_Call* exp_call) { GWDEBUG_EXE
  if(exp_call->args)
    CHECK_BB(emit_exp(emit, exp_call->args, 1))
  if(GET_FLAG(exp_call->m_func->def, variadic))
    CHECK_BB(emit_func_arg_vararg(emit, exp_call))
  return GW_OK;
}

ANN static m_bool prepare_call(const Emitter emit, const Exp_Call* exp_call) { GWDEBUG_EXE
  if(exp_call->args)
    CHECK_BB(emit_func_args(emit, exp_call))
  CHECK_BB(emit_exp(emit, exp_call->func, 0))
  if(GET_FLAG(exp_call->m_func->def, variadic) && !exp_call->args) {
    // handle empty call to variadic functions
    const Instr mk = emit_add_instr(emit, VarargIni);
    *(m_uint*)mk->ptr = 1;
    emit_add_instr(emit, PushNull);
  }
  return GW_OK;
}

ANN static inline m_int push_tmpl_func(const Emitter emit, const Func f,
    const Type_List types) {
  const Value v = f->value_ref;
  m_uint scope;
  emit_push(emit, v->owner_class, v->owner, &scope);
  CHECK_BB(traverse_func_template(emit->env, f->def, types))
  return (m_int)scope;
}

ANN static m_bool emit_exp_call_template(const Emitter emit, const Exp_Call* exp_call) {
  if(emit->env->func && emit->env->func == exp_call->m_func)
    return prepare_call(emit, exp_call);
  m_int scope = push_tmpl_func(emit, exp_call->m_func, exp_call->tmpl->types);
  CHECK_BB(scope);
  CHECK_BB(prepare_call(emit, exp_call))
  emit_pop_type(emit);
  emit_pop(emit, (m_uint)scope);
  UNSET_FLAG(exp_call->m_func, checked);
  return GW_OK;
}

ANN static m_bool emit_exp_call(const Emitter emit, const Exp_Call* exp_call) { GWDEBUG_EXE
  if(!exp_call->tmpl)
    CHECK_BB(prepare_call(emit, exp_call))
  else
    CHECK_BB(emit_exp_call_template(emit, exp_call))
  return emit_exp_call1(emit, exp_call->m_func);
}

ANN static m_bool emit_binary_func(const Emitter emit, const Exp_Binary* bin) { GWDEBUG_EXE
  const Exp_Call exp = { .func=bin->rhs, .args=bin->lhs, .m_func=bin->func };
  return emit_exp_call(emit, &exp);
}

ANN static m_bool emit_exp_binary(const Emitter emit, const Exp_Binary* bin) { GWDEBUG_EXE
  const Exp lhs = bin->lhs;
  const Exp rhs = bin->rhs;
  struct Op_Import opi = { .op=bin->op, .lhs=lhs->type, .rhs=rhs->type, .data = (uintptr_t)bin };
  if(bin->op == op_chuck && isa(rhs->type, t_function) > 0)
    return emit_binary_func(emit, bin);
  CHECK_BB(emit_exp(emit, lhs, 1))
  CHECK_BB(emit_exp(emit, rhs, 1))
  return op_emit(emit, &opi);
}

ANN static m_bool emit_exp_cast(const Emitter emit, const Exp_Cast* cast) {
  struct Op_Import opi = { .op=op_cast, .lhs=cast->exp->type, .rhs=cast->self->type, .data=(uintptr_t)cast};
  CHECK_BB(emit_exp(emit, cast->exp, 0))
  (void)op_emit(emit, &opi);
  return GW_OK;
}

ANN static m_bool emit_exp_post(const Emitter emit, const Exp_Postfix* post) { GWDEBUG_EXE
  struct Op_Import opi = { .op=post->op, .lhs=post->exp->type, .data=(uintptr_t)post };
  CHECK_BB(emit_exp(emit, post->exp, 1))
  return op_emit(emit, &opi);
}

ANN static m_bool emit_exp_dur(const Emitter emit, const Exp_Dur* dur) { GWDEBUG_EXE
  CHECK_BB(emit_exp(emit, dur->base, 0))
  CHECK_BB(emit_exp(emit, dur->unit, 0))
  emit_add_instr(emit, FloatTimes);
  return GW_OK;
}

static inline m_bool push_func_code(const Emitter emit, const Func f) {
  const Instr instr = emit_add_instr(emit, RegPushPtr);
  return !!(instr->m_val = (m_uint)f->code);
}

static m_bool emit_template_code(const Emitter emit, const Func f) {
  if(GET_FLAG(f, ref))
    CHECK_BB(traverse_template(emit->env, f->value_ref->owner_class->def))
  CHECK_BB(emit_func_def(emit, f->def))
  return push_func_code(emit, f);
}

ANN static m_bool emit_exp_call1_code(const Emitter emit, const Func f) {
  if(GET_FLAG(f, template) && emit->env->func != f)
    return emit_template_code(emit, f);
  return !!emit_add_instr(emit, RegPushCode);
}

ANN static Instr emit_call(const Emitter emit, const Func f) {
  if(GET_FLAG(f, pure))
    emit_add_instr(emit, MemoizeCall);
  const Type t = actual_type(f->value_ref->type);
  const f_instr exec = isa(t, t_fptr) < 0 ? GET_FLAG(f->def, builtin) ?
     GET_FLAG(f, member) ? FuncMember : FuncStatic : FuncUsr : FuncPtr;
  return emit_add_instr(emit, exec);
}

ANN m_bool emit_exp_call1(const Emitter emit, const Func func) { GWDEBUG_EXE
  if(!func->code || (GET_FLAG(func, ref) && !GET_FLAG(func, builtin)))
    CHECK_BB(emit_exp_call1_code(emit, func))
  else
    push_func_code(emit, func);
  const Instr offset = emit_add_instr(emit, RegPushImm);
  *(m_uint*)offset->ptr = emit_code_offset(emit);
  const Instr instr = emit_call(emit, func);
  const m_uint size = instr->m_val = func->def->ret_type->size;
  return (m_bool)(instr->m_val2 = kindof(size, !size));
}

ANN2(1,2) static m_bool emit_exp_spork_finish(const Emitter emit, const VM_Code code, const Func f,
    const m_uint arg_size, const m_uint depth) {
  const Instr push_code = emit_add_instr(emit, RegPushImm);
  *(VM_Code*)push_code->ptr = code;
  const Instr spork = emit_add_instr(emit, f ? SporkExp : SporkFunc);
  spork->m_val = f ? emit_code_offset(emit) : arg_size;
  spork->m_val2 = (m_uint)code;
  return (m_bool)(*(m_uint*)spork->ptr = depth);
}

ANN static m_bool spork_exp(const Emitter emit, const Exp_Call* exp) { GWDEBUG_EXE
  CHECK_BB(prepare_call(emit, exp))
  char c[11 + num_digit(exp->self->pos)];
  sprintf(c, "spork~exp:%i", exp->self->pos);
  emit_push_code(emit, c);
  if(GET_FLAG(exp->m_func, member))
    SET_FLAG(emit->code, member);
  const Instr op = emit_add_instr(emit, MemPushImm);
  op->m_val = emit->code->stack_depth;
  CHECK_BB(emit_exp_call1(emit, exp->m_func))
  const VM_Code code = finalyze(emit);
  const m_uint size = exp->m_func->def->stack_depth - (GET_FLAG(exp->m_func,
    member) ? SZ_INT : 0);
  return emit_exp_spork_finish(emit, code, NULL, size, 0);
}

static m_bool scoped_stmt(const Emitter emit, const Stmt stmt, const m_bool pop) {
  ++emit->env->scope;
  emit_push_scope(emit);
  emit_add_instr(emit, GcIni);
  CHECK_BB(emit_stmt(emit, stmt, pop))
  emit_add_instr(emit, GcEnd);
  emit_pop_scope(emit);
  --emit->env->scope;
  return GW_OK;
}

static inline void stack_alloc(const Emitter emit) {
  emit_local(emit, SZ_INT, 0);
  emit->code->stack_depth += SZ_INT;
}

static inline void stack_alloc_this(const Emitter emit) {
  SET_FLAG(emit->code, member);
  stack_alloc(emit);
}

ANN static m_bool spork_func(const Emitter emit, const Stmt stmt) { GWDEBUG_EXE
  emit_add_instr(emit, RegPushImm); // could be reg_push_ptr
  emit_push_code(emit, "sporked");
  if(SAFE_FLAG(emit->env->func, member))
    stack_alloc_this(emit);
  CHECK_BB(scoped_stmt(emit, stmt, 0))
  const VM_Code code = finalyze(emit);
  return emit_exp_spork_finish(emit, code, (Func)code, 0, emit->code->stack_depth);
}

ANN m_bool emit_exp_spork(const Emitter emit, const Exp_Unary* unary) {
  return unary->code ? spork_func(emit, unary->code) :
    spork_exp(emit, &unary->exp->d.exp_call);
}

ANN static m_bool emit_exp_unary(const Emitter emit, const Exp_Unary* unary) { GWDEBUG_EXE
  struct Op_Import opi = { .op=unary->op, .data=(uintptr_t)unary };
  if(unary->op != op_spork && unary->exp) {
    CHECK_BB(emit_exp(emit, unary->exp, 1))
    opi.rhs = unary->exp->type;
  }
  return op_emit(emit, &opi);
}

ANN static m_bool emit_implicit_cast(const Emitter emit,
    const restrict Type from, const restrict Type to) { GWDEBUG_EXE
  struct Op_Import opi = { .op=op_impl, .lhs=from, .rhs=to, .data=(m_uint)from };
  return op_emit(emit, &opi);
}

ANN static Instr emit_flow(const Emitter emit, const Type type,
    const f_instr f1, const f_instr f2) { GWDEBUG_EXE
  if(isa(type, t_float) > 0) {
    emit_add_instr(emit, PushNull2);
    return emit_add_instr(emit, f2);
  }
  emit_add_instr(emit, PushNull);
  return emit_add_instr(emit, f1);
}

ANN static m_bool emit_exp_if(const Emitter emit, const Exp_If* exp_if) { GWDEBUG_EXE
  CHECK_BB(emit_exp(emit, exp_if->cond, 0))
  const Instr op = emit_flow(emit, exp_if->cond->type, BranchEqInt, BranchEqFloat);
  CHECK_BB(emit_exp(emit, exp_if->if_exp, 0))
  const Instr op2 = emit_add_instr(emit, Goto);
  op->m_val = emit_code_size(emit);
  const m_bool ret = emit_exp(emit, exp_if->else_exp, 0);
  op2->m_val = emit_code_size(emit);
  return ret;
}

#ifdef OPTIMIZE
ANN static void emit_exp_constprop(const Emitter emit, const Exp e) {
  if(!e->emit_var) {
    if(e->exp_type == ae_exp_constprop) {
      const Instr instr = emit_add_instr(emit, RegPushMem);
      instr->m_val = e->d.exp_primary.value->offset;
      *(m_uint*)instr->ptr = GET_FLAG(e->d.exp_primary.value, global);
      return;
    }
    const Instr instr = emit_add_instr(emit, ConstPropGet);
    instr->m_val2 = e->d.exp_primary.value->offset;
    instr->m_val = e->d.exp_primary.d.num;
    *(m_uint*)instr->ptr = 1;
  } else {
    const Instr instr = emit_add_instr(emit, ConstPropSet);
    instr->m_val = e->d.exp_primary.value->offset;
    *(m_bool*)instr->ptr = 1;
    instr->m_val2 = e->d.exp_primary.d.num;
  }
}
#endif

DECL_EXP_FUNC(emit)

ANN2(1) static m_bool emit_exp(const Emitter emit, Exp exp, const m_bool ref) { GWDEBUG_EXE
  do {
    CHECK_BB(exp_func[exp->exp_type](emit, &exp->d))
    if(exp->cast_to)
      CHECK_BB(emit_implicit_cast(emit, exp->type, exp->cast_to))
    if(ref && isa(exp->type, t_object) > 0) {
      const Instr instr = emit_add_instr(emit, RegAddRef);
      instr->m_val = exp->emit_var;
    }
    if(emit->env->func && isa(exp->type, t_function) > 0 &&
        !GET_FLAG(exp->type->d.func->value_ref->d.func_ref, pure))
      UNSET_FLAG(emit->env->func, pure);
  } while((exp = exp->next));
  return GW_OK;
}

ANN static m_bool emit_stmt_if(const Emitter emit, const Stmt_If stmt) { GWDEBUG_EXE
  emit_push_scope(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  const Instr op = emit_flow(emit, isa(stmt->cond->type, t_object) > 0 ?
      t_int : stmt->cond->type, BranchEqInt, BranchEqFloat);
  CHECK_OB(op)
  CHECK_BB(scoped_stmt(emit, stmt->if_body, 1))
  const Instr op2 = emit_add_instr(emit, Goto);
  op->m_val = emit_code_size(emit);
  if(stmt->else_body)
    CHECK_BB(scoped_stmt(emit, stmt->else_body, 1))
  op2->m_val = emit_code_size(emit);
  emit_pop_scope(emit);
  return GW_OK;
}

ANN static m_bool emit_stmt_code(const Emitter emit, const Stmt_Code stmt) { GWDEBUG_EXE
  ++emit->env->scope;
  const m_bool ret = stmt->stmt_list ? emit_stmt_list(emit, stmt->stmt_list) : 1;
  --emit->env->scope;
  return ret;
}

#ifdef OPTIMIZE
ANN static m_bool optimize_taill_call(const Emitter emit, const Exp_Call* e) {
  Exp arg = e->args;
  if(arg)
    CHECK_BB(emit_exp(emit, e->args, 0))
  const Instr instr = emit_add_instr(emit, PutArgsInMem);
  while(arg) {
    instr->m_val += arg->type->size;
    arg = arg->next;
  }
  emit_add_instr(emit, Goto);
  return GW_OK;
}
#define OPTIMIZE_TCO\
  if(stmt->val->exp_type == ae_exp_call && emit->env->func == stmt->val->d.exp_call.m_func)\
    return optimize_taill_call(emit, &stmt->val->d.exp_call);
#else
#define OPTIMIZE_TCO
#endif


ANN static m_bool emit_stmt_return(const Emitter emit, const Stmt_Exp stmt) { GWDEBUG_EXE
  if(stmt->val) {
    OPTIMIZE_TCO
    CHECK_BB(emit_exp(emit, stmt->val, 0))
  }
  vector_add(&emit->code->stack_return, (vtype)emit_add_instr(emit, Goto));
  return GW_OK;
}

ANN static inline m_bool emit_stmt_continue(const Emitter emit, const Stmt stmt __attribute__((unused))) { GWDEBUG_EXE
  vector_add(&emit->code->stack_cont, (vtype)emit_add_instr(emit, Goto));
  return GW_OK;
}

ANN static inline m_bool emit_stmt_break(const Emitter emit, const Stmt stmt __attribute__((unused))) { GWDEBUG_EXE
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

ANN static Instr _flow(const Emitter emit, const Exp e, const m_bool b) {
  CHECK_BO(emit_exp(emit, e, 0))
  const f_instr instr_i = b ? BranchEqInt : BranchNeqInt;
  const f_instr instr_f = b ? BranchEqFloat : BranchNeqFloat;
  return emit_flow(emit, e->type, instr_i, instr_f);
}

ANN static m_bool emit_stmt_flow(const Emitter emit, const Stmt_Flow stmt) { GWDEBUG_EXE
  const m_uint index = emit_code_size(emit);
  Instr op = NULL;
  emit_push_stack(emit);
  if(!stmt->is_do)
    op = _flow(emit, stmt->cond, stmt->self->stmt_type == ae_stmt_while);
  CHECK_BB(scoped_stmt(emit, stmt->body, 1))
  if(stmt->is_do) {
    CHECK_OB((op = _flow(emit, stmt->cond, stmt->self->stmt_type != ae_stmt_while)))
    op->m_val = index;
  } else {
    const Instr goto_ = emit_add_instr(emit, Goto);
    goto_->m_val = index;
    op->m_val = emit_code_size(emit);
  }
  emit_pop_stack(emit, index);
  return GW_OK;
}

ANN static m_uint get_decl_size(Var_Decl_List a) {
  m_uint size = 0;
  do if(GET_FLAG(a->self->value, used))
    size += a->self->value->type->size;
  while((a = a->next));
  return size;
}

ANN static m_uint pop_exp_size(const Emitter emit, Exp e) {
  m_uint size = 0;
  do {
    if(e->exp_type == ae_exp_primary &&
        e->d.exp_primary.primary_type == ae_primary_hack) {
      size += pop_exp_size(emit, e->d.exp_primary.d.exp);
      continue;
    }
    size += (e->exp_type == ae_exp_decl ?
        get_decl_size(e->d.exp_decl.list) : e->type->size);
  } while((e = e->next));
  return size;
}

ANN static void pop_exp(const Emitter emit, Exp e) {
  const m_uint size = pop_exp_size(emit, e);
  if(size) {
    const Instr instr = emit_add_instr(emit, RegPop);
    instr->m_val = size;
  }
}

ANN static m_bool emit_stmt_for(const Emitter emit, const Stmt_For stmt) { GWDEBUG_EXE
  emit_push_stack(emit);
  CHECK_BB(emit_stmt(emit, stmt->c1, 1))
  const m_uint index = emit_code_size(emit);
  CHECK_BB(emit_stmt(emit, stmt->c2, 0))
  const Instr op = emit_flow(emit, stmt->c2->d.stmt_exp.val->type,
    BranchEqInt, BranchEqFloat);
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

ANN static m_bool emit_stmt_auto(const Emitter emit, const Stmt_Auto stmt) { GWDEBUG_EXE
  CHECK_BB(emit_exp(emit, stmt->exp, 0))
  const Instr s1 = emit_add_instr(emit, MemSetImm);
  const Instr s2 = emit_add_instr(emit, MemSetImm);
  const m_uint start  = emit_code_size(emit);
  emit_push_stack(emit);
  const Instr loop = emit_add_instr(emit, AutoLoopStart);
  const m_uint offset = emit_local(emit, 2*SZ_INT, 0);
  stmt->v->offset = offset + SZ_INT;
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  const m_uint index = emit_code_size(emit);
  const Instr end = emit_add_instr(emit, AutoLoopEnd);
  const Instr tgt = emit_add_instr(emit, Goto);
  *(m_uint*)end->ptr = emit_code_size(emit);
  tgt->m_val = start;
  loop->m_val = offset;
  end->m_val = offset;
  s1->m_val = offset;
  s2->m_val = offset + SZ_INT;
  if(stmt->is_ptr)
    end->m_val2 = loop->m_val2 = (m_uint)stmt->v->type;
  emit_pop_stack(emit, index);
  return GW_OK;
}

ANN static m_bool emit_stmt_loop(const Emitter emit, const Stmt_Loop stmt) { GWDEBUG_EXE
  emit_push_stack(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  m_int* counter = (m_int*)xcalloc(1, SZ_INT);
  const Instr init = emit_add_instr(emit, InitLoopCounter);
  init->m_val = (m_uint)counter;
  const m_uint index = emit_code_size(emit);
  const Instr deref = emit_add_instr(emit, RegPushDeref);
  deref->m_val2 = SZ_INT;
  *(m_int**)deref->ptr = counter;
  emit_add_instr(emit, PushNull);
  const Instr op = emit_add_instr(emit, BranchEqInt);
  const Instr dec = emit_add_instr(emit, DecIntAddr);
  dec->m_val = (m_uint)counter;
  CHECK_BB(scoped_stmt(emit, stmt->body, 1))
  const Instr _goto = emit_add_instr(emit, Goto);
  _goto->m_val = index;
  op->m_val = emit_code_size(emit);
  emit_pop_stack(emit, index);
  return GW_OK;
}

ANN static m_bool emit_stmt_jump(const Emitter emit, const Stmt_Jump stmt) { GWDEBUG_EXE
  if(!stmt->is_label)
    stmt->data.instr = emit_add_instr(emit, Goto);
  else {
    if(emit->env->sw->cases && !strcmp(s_name(stmt->name), "default")) {
      if(!strcmp(s_name(stmt->name), "default"))
        vector_release(&stmt->data.v);
      if(emit->env->sw->default_case_index)
        ERR_B(stmt->self->pos, "default case already defined")
      emit->env->sw->default_case_index = emit_code_size(emit);
      return GW_OK;
    }
    if(!stmt->data.v.ptr)
      ERR_B(stmt->self->pos, "illegal case")
    const m_uint size = vector_size(&stmt->data.v);
    if(!size) {
      vector_release(&stmt->data.v);
      ERR_B(stmt->self->pos, "label '%s' defined but not used.", s_name(stmt->name))
    }
    LOOP_OPTIM
    for(m_uint i = size + 1; --i;) {
      const Stmt_Jump label = (Stmt_Jump)vector_at(&stmt->data.v, i - 1);
      if(!label->data.instr) {
        vector_release(&stmt->data.v);
        ERR_B(label->self->pos, "you are trying to use a upper label.")
      }
      label->data.instr->m_val = emit_code_size(emit);
    }
    vector_release(&stmt->data.v);
  }
  return GW_OK;
}

ANN static m_bool emit_stmt_switch(const Emitter emit, const Stmt_Switch stmt) { GWDEBUG_EXE
  const m_uint dyn = vector_size(&emit->env->sw->exp);
  Instr push;
  if(dyn) {
    for(m_uint i = 0; i < dyn; ++i)
      CHECK_BB(emit_exp(emit, (Exp)vector_at(&emit->env->sw->exp, i), 0))
    push = emit_add_instr(emit, SwitchIni);
    emit->env->sw->vec = new_vector();
    push->m_val = (m_uint)emit->env->sw->vec;
    push->m_val2 = (m_uint)new_map();
  }
  vector_add(&emit->code->stack_break, (vtype)NULL);
  CHECK_BB(emit_exp(emit, stmt->val, 0))
  if(emit->env->sw->cases)
    ERR_B(stmt->self->pos, "swith inside an other switch. this is not allowed for now")
  emit->env->sw->default_case_index = 0;
  emit->env->sw->cases = NULL;
  const Instr instr = emit_add_instr(emit, BranchSwitch);
  emit->env->sw->cases = new_map();
  instr->m_val2 = (m_uint)emit->env->sw->cases;
  CHECK_BB(scoped_stmt(emit, stmt->stmt, 1))
  instr->m_val = emit->env->sw->default_case_index ?: emit_code_size(emit);
  if(dyn) {
    const Map m = (Map)push->m_val2, map = emit->env->sw->cases;
    for(m_uint i = map_size(map) + 1; --i;)
      map_set(m, VKEY(map, i), VVAL(map, i -1));
  }
  *(m_uint*)instr->ptr = dyn ? SZ_INT : 0;
  pop_vector(&emit->code->stack_break, emit_code_size(emit));
  emit->env->sw->cases = NULL;
  return GW_OK;
}

ANN m_bool value_value(const Value v, m_int *value) {
  if((!GET_FLAG(v, builtin) && !GET_FLAG(v, enum)) || GET_FLAG(v, member))
     return 0;
  *value = v->d.ptr ? *(m_int*)v->d.ptr : v->owner->class_data[v->offset];
  return GW_OK;
}

ANN Value case_value(const Exp exp) {
  if(exp->exp_type == ae_exp_primary) {
    const Exp_Primary* prim = &exp->d.exp_primary;
    if(prim->primary_type == ae_primary_num)
      return NULL;
    return prim->value;
  }
  const Exp_Dot* dot = &exp->d.exp_dot;
  return find_value(actual_type(dot->t_base), dot->xid);
}

ANN static m_bool prim_value(const Exp exp, m_int *value) {
  *value = (m_int)exp->d.exp_primary.d.num;
  return GW_OK;
}
ANN static m_bool emit_stmt_case(const Emitter emit, const Stmt_Exp stmt) { GWDEBUG_EXE
  if(!emit->env->sw->cases)
    ERR_B(stmt->self->pos, "case found outside switch statement.")
  m_int value = 0;
  const Value v = case_value(stmt->val);
  if((!v && prim_value(stmt->val, &value)) || value_value(v, &value)) {
    if(map_get(emit->env->sw->cases, (vtype)value))
      ERR_B(stmt->val->pos, "duplicated cases value %i", value)
    map_set(emit->env->sw->cases, (vtype)value, emit_code_size(emit));
  } else
    vector_add(emit->env->sw->vec, emit_code_size(emit));
  return GW_OK;
}

ANN static m_bool emit_stmt_type(const Emitter emit, const Stmt_Type stmt) { GWDEBUG_EXE
  return stmt->type->def ? emit_class_def(emit, stmt->type->def) : 1;
}

ANN static m_bool emit_stmt_enum(const Emitter emit, const Stmt_Enum stmt) { GWDEBUG_EXE
  LOOP_OPTIM
  for(m_uint i = 0; i < vector_size(&stmt->values); ++i) {
    const Value v = (Value)vector_at(&stmt->values, i);
    if(!emit->env->class_def) {
      ALLOC_PTR(addr, m_uint, i);
      v->offset = emit_local(emit, SZ_INT, 0);
      v->d.ptr = addr;
    } else
      *(m_uint*)(emit->env->class_def->nspc->class_data + v->offset) = i;
  }
  return GW_OK;
}

ANN void emit_union_offset(Decl_List l, const m_uint o) { GWDEBUG_EXE
  do {
    Var_Decl_List v = l->self->d.exp_decl.list;
    do v->self->value->offset = o;
    while((v = v->next));
  } while((l = l->next));
}

ANN static m_bool emit_stmt_union(const Emitter emit, const Stmt_Union stmt) { GWDEBUG_EXE
  Decl_List l = stmt->l;
  m_uint scope = emit->env->scope;
  const m_bool global = GET_FLAG(stmt, global);
  if(stmt->xid) {
    if(stmt->value->type->nspc->class_data_size && !stmt->value->type->nspc->class_data)
      stmt->value->type->nspc->class_data =
        (m_bit*)xcalloc(1, stmt->value->type->nspc->class_data_size);
    stmt->value->type->nspc->offset = stmt->s;
    if(!stmt->value->type->p)
      stmt->value->type->p = mp_ini((uint32_t)stmt->value->type->size);
    Type_Decl *type_decl = new_type_decl(new_id_list(stmt->xid, stmt->self->pos),
        emit->env->class_def ? ae_flag_member : 0); // or zero ?
    type_decl->flag = stmt->flag;
    const Var_Decl var_decl = new_var_decl(stmt->xid, NULL, 0);
    const Var_Decl_List var_decl_list = new_var_decl_list(var_decl, NULL);
    const Exp exp = new_exp_decl(type_decl, var_decl_list);
    exp->d.exp_decl.type = stmt->value->type;
    var_decl->value = stmt->value;
    CHECK_BB(emit_exp_decl(emit, &exp->d.exp_decl))
//    if(!emit->env->class_def)
//      ADD_REF(stmt->value->type);
    free_exp(exp);
    if(global) {
      const M_Object o = new_object(NULL, stmt->value->type);
      stmt->value->d.ptr = (m_uint*)o;
      SET_FLAG(stmt->value, builtin);
      SET_FLAG(stmt->value, global);
    }
    emit_push(emit, stmt->value->type, stmt->value->type->nspc, &scope);
  } else if(stmt->type_xid) {
    if(stmt->type->nspc->class_data_size && !stmt->type->nspc->class_data)
      stmt->type->nspc->class_data =
        (m_bit*)xcalloc(1, stmt->type->nspc->class_data_size);
    stmt->type->nspc->offset = stmt->s;
    if(!stmt->type->p)
      stmt->type->p = mp_ini((uint32_t)stmt->type->size);
    emit_push(emit, stmt->type, stmt->type->nspc, &scope);
  } else if(emit->env->class_def) {
    if(!GET_FLAG(l->self->d.exp_decl.list->self->value, member))
      stmt->o = emit_local(emit, stmt->s, 0);
  } else if(global) {
    void* ptr = (void*)xcalloc(1, stmt->s);
    l = stmt->l;
    do {
      Var_Decl_List list = l->self->d.exp_decl.list;
      do {
        list->self->value->d.ptr = ptr;
        SET_FLAG(list->self->value, union);
      } while((list = list->next));
    } while((l = l->next));
    SET_FLAG(stmt->l->self->d.exp_decl.list->self->value, enum);
  }
  emit_union_offset(stmt->l, stmt->o);
  if(stmt->xid) {
    const Instr instr = emit_add_instr(emit, RegPop);
    instr->m_val = SZ_INT;
  }
  if(stmt->xid || stmt->type_xid || global)
    emit_pop(emit, scope);
  return GW_OK;
}

ANN static m_bool emit_stmt_exp(const Emitter emit, const struct Stmt_Exp_* exp) {
  return exp->val ? emit_exp(emit, exp->val, 0) : 1;
}

static const _exp_func stmt_func[] = {
  (_exp_func)emit_stmt_exp,   (_exp_func)emit_stmt_flow,     (_exp_func)emit_stmt_flow,
  (_exp_func)emit_stmt_for,   (_exp_func)emit_stmt_auto,     (_exp_func)emit_stmt_loop,
  (_exp_func)emit_stmt_if,    (_exp_func)emit_stmt_code,     (_exp_func)emit_stmt_switch,
  (_exp_func)emit_stmt_break, (_exp_func)emit_stmt_continue, (_exp_func)emit_stmt_return,
  (_exp_func)emit_stmt_case,  (_exp_func)emit_stmt_jump,     (_exp_func)emit_stmt_enum,
  (_exp_func)dummy_func,      (_exp_func)emit_stmt_type,     (_exp_func)emit_stmt_union,
};

ANN static m_bool emit_stmt(const Emitter emit, const Stmt stmt, const m_bool pop) { GWDEBUG_EXE
  CHECK_BB(stmt_func[stmt->stmt_type](emit, &stmt->d))
  if(pop && stmt->stmt_type == ae_stmt_exp && stmt->d.stmt_exp.val)
    pop_exp(emit, stmt->d.stmt_exp.val);
  return GW_OK;
}

ANN static m_bool emit_stmt_list(const Emitter emit, Stmt_List l) { GWDEBUG_EXE
  do CHECK_BB(emit_stmt(emit, l->stmt, 1))
  while((l = l->next));
  return GW_OK;
}

ANN static m_bool is_special(const Type t) {
  if(isa(t, t_complex) > 0 || isa(t, t_polar) > 0 ||
     isa(t, t_vec3)    > 0 || isa(t, t_vec4)  > 0  ||
     isa(t, t_vararg)  > 0)
    return GW_OK;
  return GW_ERROR;
}

ANN static m_bool emit_dot_static_import_data(const Emitter emit, const Value v, const uint emit_addr) { GWDEBUG_EXE
  if(v->d.ptr && GET_FLAG(v, builtin)) { // from C
    if(GET_FLAG(v, enum)) {
      const Instr func_i = emit_add_instr(emit, RegPushImm);
      *(m_uint*)func_i->ptr = (m_uint)v->d.ptr;
    } else {
      const m_uint size = v->type->size;
      const Instr instr = emit_kind(emit, size, emit_addr, dotimport);
      instr->m_val = (isa(v->type, t_object) > 0 ?
        (m_uint)&v->d.ptr : (m_uint)v->d.ptr);
    }
  } else { // from code
    const Instr push_i = emit_add_instr(emit, RegPushImm);
    *(Type*)push_i->ptr = v->owner_class;
    const m_uint size = v->type->size;
    const Instr instr = emit_kind(emit, size, emit_addr, dotstatic);
    instr->m_val =  v->offset;
  }
  return GW_OK;
}

ANN static m_bool emit_complex_member(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  const Exp base = member->base;
  base->emit_var = 1;
  CHECK_BB(emit_exp(emit, base, 0))
  const m_bool is_complex = !strcmp((isa(base->type, t_complex) > 0  ? "re" : "phase") ,
        s_name(member->xid));
  if(is_complex && member->self->emit_var) // skip
    return GW_OK;
  const Instr instr = is_complex ? emit_add_instr(emit, ComplexReal) :
      emit_add_instr(emit, ComplexImag);
  instr->m_val = member->self->emit_var;
  return GW_OK;
}

ANN static inline void emit_vec_func(const Emitter emit, const Value v) {
  const Instr instr = emit_add_instr(emit, RegPushImm);
  *(m_uint*)instr->ptr = (m_uint)((Func)vector_at(&v->owner_class->nspc->vtable, v->d.func_ref->vt_index))->code;
}

ANN static m_bool emit_VecMember(const Emitter emit, const Exp_Dot* member) {
  member->base->emit_var = 1;
  CHECK_BB(emit_exp(emit, member->base, 0))
  const Value v = find_value(member->base->type, member->xid);
  if(GET_FLAG(v, func)) {
    emit_vec_func(emit, v);
    return GW_OK;
  }
  const Instr instr = emit_add_instr(emit, VecMember);
  instr->m_val2 = v->offset;
  instr->m_val = member->self->emit_var;
  return GW_OK;
}

ANN static m_bool emit_vararg_start(const Emitter emit, const m_uint offset) { GWDEBUG_EXE
  if(emit->env->func->variadic)
    ERR_B(0, "vararg.start already used. this is an error")
  emit->env->func->variadic = emit_add_instr(emit, VarargTop);
  emit->env->func->variadic->m_val = offset;
  emit->env->func->variadic->m_val2 = emit_code_size(emit);
  return GW_OK;
}

ANN static m_bool emit_vararg_end(const Emitter emit, const m_uint offset) { GWDEBUG_EXE
  if(!emit->env->func->variadic)
    ERR_B(0, "vararg.start not used before vararg.end. this is an error")
  const Instr instr = emit_add_instr(emit, VarargEnd);
  instr->m_val = offset;
  instr->m_val2 = emit->env->func->variadic->m_val2;
  emit->env->func->variadic->m_val2 = emit_code_size(emit);
  *(m_uint*)emit->env->func->variadic->ptr = 1;
  return GW_OK;
}

ANN static m_bool emit_vararg(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  m_uint offset = emit->env->class_def ? SZ_INT : 0;
  Arg_List l = emit->env->func->def->arg_list;
  const m_str str = s_name(member->xid);
  while(l) {
    offset += l->type->size;
    l = l->next;
  }
  if(!strcmp(str, "start"))
    return emit_vararg_start(emit, offset);
  if(!strcmp(str, "end"))
    return emit_vararg_end(emit, offset);
  const Instr instr = emit_add_instr(emit, VarargMember);
  instr->m_val = offset;
  instr->m_val2 = member->self->type->size;
  return GW_OK;
}

ANN static m_bool emit_exp_dot_special(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  const Type t = member->t_base;
  if(isa(t, t_complex) > 0 || isa(t, t_polar) > 0)
    return emit_complex_member(emit, member);
  else if(isa(t, t_vec3) > 0 || isa(t, t_vec4) > 0)
    return emit_VecMember(emit, member);
  return emit_vararg(emit, member);
}

ANN static m_bool emit_dot_static_func(const Emitter emit, const Func func) { GWDEBUG_EXE
  const Instr func_i = emit_add_instr(emit, RegPushImm);
  *(Func*)func_i->ptr = func;
  return GW_OK;
}

ANN static m_bool emit_member_func(const Emitter emit, const Exp_Dot* member, const Func func) { GWDEBUG_EXE
  if(GET_FLAG(func, member)) {
    if(emit_exp(emit, member->base, 0) < 0)
      ERR_B(member->self->pos, "... in member function") // LCOV_EXCL_LINE
    emit_add_instr(emit, RegDup);
    const Instr func_i = emit_add_instr(emit, DotFunc);
    func_i->m_val = func->vt_index;
  }
  return GW_OK;
}

ANN static inline void emit_member(const Emitter emit, const Value v, const uint emit_addr) {
  const m_uint size = v->type->size;
  const Instr instr = emit_kind(emit, size, emit_addr, dotmember);
  instr->m_val = v->offset;
}

ANN static m_bool emit_exp_dot_instance(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  const Type t_base = member->t_base;
  const Value value = find_value(t_base, member->xid);
  const uint emit_addr = member->self->emit_var;
  if(isa(member->self->type, t_fptr) > 0) { // function pointer
    if(GET_FLAG(value, member)) { // member
      if(emit_exp(emit, member->base, 0) < 0)
        ERR_B(member->self->pos, "... in member function") // LCOV_EXCL_LINE
      if(!GET_FLAG(value->type->d.func, global))
        emit_add_instr(emit, RegDup);
      emit_member(emit, value, emit_addr);
      return GW_OK;
    } else
      return emit_dot_static_data(emit, value, emit_addr);
  } else if(isa(member->self->type, t_function) > 0) { // function
    const Func func = value->d.func_ref;
    if(GET_FLAG(func, member)) {
      return emit_member_func(emit, member, func);
    } else
      return emit_dot_static_func(emit, func);
  } else { // variable
    if(GET_FLAG(value, member)) { // member
      CHECK_BB(emit_exp(emit, member->base, 0))
      emit_member(emit, value, emit_addr);
      return GW_OK;
    } else // static
      CHECK_BB(emit_dot_static_import_data(emit, value, emit_addr))
  }
  return GW_OK;
}

ANN static m_bool emit_exp_dot_static(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  const Type t_base = member->t_base->d.base_type;
  const Value value = find_value(t_base, member->xid);
  if(isa(member->self->type, t_function) > 0 && isa(member->self->type, t_fptr) < 0)
    return emit_dot_static_func(emit, value->d.func_ref);
  return emit_dot_static_import_data(emit, value, member->self->emit_var);
}

ANN static m_bool emit_exp_dot(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  if(is_special(member->t_base) > 0)
    return emit_exp_dot_special(emit, member);
  if(isa(member->t_base, t_class) > 0)
    return emit_exp_dot_static(emit, member);
  return emit_exp_dot_instance(emit, member);
}

ANN static inline void emit_func_def_global(const Emitter emit, const Value value) { GWDEBUG_EXE
  const Instr set_mem = emit_add_instr(emit, MemSetImm);
  set_mem->m_val = value->offset = emit_local(emit, value->type->size, 0);
  set_mem->m_val2 = (m_uint)value->d.func_ref;
}

ANN static void emit_func_def_init(const Emitter emit, const Func func) { GWDEBUG_EXE
  const Type t = emit->env->class_def;
  char c[(t ? strlen(t->name) + 1 : 0) + strlen(func->name) + 6];
  sprintf(c, "%s%s%s(...)", t ? t->name : "", t ? "." : "", func->name);
  emit_push_code(emit, c);
}

ANN static void emit_func_def_args(const Emitter emit, Arg_List a) { GWDEBUG_EXE
  do {
    const Value  value = a->var_decl->value;
    const m_uint size = value->type->size;
    const m_bool obj = isa(value->type, t_object) > 0;
    emit->code->stack_depth += size;
    value->offset = emit_local(emit, size, obj);
  } while((a = a->next));
}

ANN static void emit_func_def_ensure(const Emitter emit, const Func_Def func_def) { GWDEBUG_EXE
  const m_uint size = func_def->ret_type->size;
  if(size)
    emit_kind(emit, size, 0, regpushimm);
  vector_add(&emit->code->stack_return, (vtype)emit_add_instr(emit, Goto));
}

ANN static void emit_func_def_return(const Emitter emit) { GWDEBUG_EXE
  const m_uint val = emit_code_size(emit);
  LOOP_OPTIM
  for(m_uint i = vector_size(&emit->code->stack_return) + 1; --i; ) {
    const Instr instr = (Instr)vector_at(&emit->code->stack_return, i-1);
    instr->m_val = val;
  }
  vector_clear(&emit->code->stack_return);
  emit_pop_scope(emit);
  if(GET_FLAG(emit->env->func, pure))
    emit_add_instr(emit, MemoizeStore);
  emit_add_instr(emit, FuncReturn);
}

ANN static void emit_func_def_code(const Emitter emit, const Func func) { GWDEBUG_EXE
  func->code = emit_code(emit);
  if(GET_FLAG(func->def, dtor)) {
    emit->env->class_def->nspc->dtor = func->code;
    Instr instr = (Instr)vector_back(func->code->instr);
    instr->execute = DTOR_EOC;
    ADD_REF(func->code)
  }
}

ANN static m_bool emit_func_def_body(const Emitter emit, const Func_Def func_def) { GWDEBUG_EXE
  if(func_def->arg_list)
    emit_func_def_args(emit, func_def->arg_list);
  if(GET_FLAG(func_def, variadic))
    stack_alloc(emit);
  if(func_def->d.code->d.stmt_code.stmt_list)
    CHECK_BB(emit_stmt_code(emit, &func_def->d.code->d.stmt_code))
  emit_func_def_ensure(emit, func_def);
  return GW_OK;
}

ANN static m_bool emit_func_def(const Emitter emit, const Func_Def func_def) { GWDEBUG_EXE
  const Func func = get_func(emit->env, func_def);
  if(func->code)return GW_OK;
  if(tmpl_list_base(func_def->tmpl)) { // don't check template definition
    UNSET_FLAG(func_def, template);;
    return GW_OK;
  }
  if(!emit->env->class_def)
    emit_func_def_global(emit, func->value_ref);
  else if(GET_FLAG(emit->env->class_def, builtin) && GET_FLAG(emit->env->class_def, template))
    return GW_OK;
  emit_func_def_init(emit, func);
  if(GET_FLAG(func, member))
    stack_alloc_this(emit);
  emit_push_scope(emit);
  const Func former = emit->env->func;
  emit->env->func = func;
  CHECK_BB(emit_func_def_body(emit, func_def))
  if(GET_FLAG(func_def, variadic) && (!emit->env->func->variadic ||
      !*(m_uint*)emit->env->func->variadic->ptr))
    ERR_B(func_def->td->xid->pos, "invalid variadic use")
  emit_func_def_return(emit);
  emit_func_def_code(emit, func);
  emit->env->func = former;
  emit_pop_code(emit);
  if(GET_FLAG(func, pure))
    func->code->memoize = memoize_ini(func, kindof(func->def->ret_type->size, !func->def->ret_type->size));
  return GW_OK;
}

DECL_SECTION_FUNC(emit)

ANN Code* emit_class_code(const Emitter emit, const m_str name) { GWDEBUG_EXE
  const m_uint len = strlen(name) + 7;
  char c[len];
  snprintf(c, len, "class %s", name);
  emit_push_code(emit, c);
  stack_alloc_this(emit);
  return emit->code;
}

ANN inline void emit_class_finish(const Emitter emit, const Nspc nspc) { GWDEBUG_EXE
  emit_add_instr(emit, FuncReturn);
  nspc->pre_ctor = emit_code(emit);
}

ANN static inline void emit_class_push(const Emitter emit, const Type type) { GWDEBUG_EXE
  vector_add(&emit->env->class_stack, (vtype)emit->env->class_def);
  emit->env->class_def = type;
}

ANN static inline void emit_class_pop(const Emitter emit) { GWDEBUG_EXE
  emit->env->class_def = (Type)vector_pop(&emit->env->class_stack);
  emit_pop_code(emit);
}

ANN static m_bool emit_class_def(const Emitter emit, const Class_Def class_def) { GWDEBUG_EXE
  const Type type = class_def->type;
  const Nspc nspc = type->nspc;
  if(tmpl_class_base(class_def->tmpl))
    return GW_OK;
  if(class_def->ext && ((/*!GET_FLAG(type->parent, emit) &&*/
      GET_FLAG(class_def->ext, typedef)) || class_def->ext->types)) {
    const Type base = class_def->ext->array ?
             array_base(type->parent) : type->parent;
    if(!base->nspc->pre_ctor)
      CHECK_BB(emit_class_def(emit, base->def))
  }
  if(nspc->class_data_size)
    nspc->class_data = (m_bit*)xcalloc(1, nspc->class_data_size);
  emit_class_push(emit, type);
  emit_class_code(emit, type->name);

  if(class_def->ext && class_def->ext->array)
      CHECK_BB(emit_array_extend(emit, type->parent,
            class_def->ext->array->exp))
  if(class_def->body) {
    Class_Body body = class_def->body;
    do CHECK_BB(emit_section(emit, body->section))
    while((body = body->next));
  }
  emit_class_finish(emit, nspc);
  emit_class_pop(emit);
  SET_FLAG(type, emit);
  return GW_OK;
}

ANN static void emit_free_code(Code* code) {
  LOOP_OPTIM
  for(m_uint j = vector_size(&code->instr) + 1; --j;)
    mp_free(Instr, (Instr)vector_at(&code->instr, j - 1));
  free_code(code);
}

ANN static void emit_free_stack(const Emitter emit) { GWDEBUG_EXE
  LOOP_OPTIM
  for(m_uint i = vector_size(&emit->stack) + 1; --i;)
    emit_free_code((Code*)vector_at(&emit->stack, i - 1));
  emit_free_code(emit->code);
  vector_clear(&emit->stack);
}

ANN static inline m_bool emit_ast_inner(const Emitter emit, Ast ast) { GWDEBUG_EXE
  do CHECK_BB(emit_section(emit, ast->section))
  while((ast = ast->next));
  return GW_OK;
}

ANN static void* emitter_reset(const Emitter emit) {
  emit_free_stack(emit);
  if(emit->env->sw->cases)
    free_map(emit->env->sw->cases);
  if(emit->env->sw->exp.ptr)
    vector_clear(&emit->env->sw->exp);
  return emit->env->sw->cases = NULL;
}

ANN VM_Code emit_ast(const Emitter emit, Ast ast) { GWDEBUG_EXE
  emit->code = new_code(emit, emit->env->name);
  emit_push_scope(emit);
  const m_bool ret = emit_ast_inner(emit, ast);
  emit_pop_scope(emit);
  return ret > 0 ? finalyze(emit) : emitter_reset(emit);
}
