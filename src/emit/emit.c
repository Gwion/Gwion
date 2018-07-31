#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "value.h"
#include "instr.h"
#include "context.h"
#include "func.h"
#include "traverse.h"
#include "mpool.h"
#include "escape.h"

#define INSTR_RECURS (f_instr)1
#define INSTR_INLINE (f_instr)2

typedef struct Local_ {
  m_uint size;
  m_uint offset;
  m_bool is_obj;
} Local;
POOL_HANDLE(Local, 128)
POOL_HANDLE(Frame, 8)
POOL_HANDLE(Code,  8)
POOL_HANDLE(Instr, 2048)
POOL_HANDLE(VM_Array_Info, 64)

#ifdef GWCOV
#define COVERAGE(a) if(emit->coverage)coverage(emit, a->pos);
ANN static void coverage(const Emitter emit, const m_uint pos) {
  fprintf(emit->cov_file, "%" INT_F " ini\n", pos);
  const Instr cov = emitter_add_instr(emit, InstrCoverage);
  cov->m_val = pos;
}
#else
#define COVERAGE(a)
#endif

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

ANEW ANN static Local* frame_alloc_local(Frame* frame, const m_uint size, const m_bool is_obj) {
  Local* local = mp_alloc(Local);
  local->size = size;
  local->offset = frame->curr_offset;
  local->is_obj = is_obj;
  frame->curr_offset += local->size;
  vector_add(&frame->stack, (vtype)local);
  return local;
}

ANN static void frame_push_scope(Frame* frame) {
  vector_add(&frame->stack, (vtype)NULL);
}

ANN static void frame_pop_scope(Frame* frame, Vector v) {
  while(vector_back(&frame->stack)) {
    const Local* local = (Local*)vector_pop(&frame->stack);
    frame->curr_offset -= local->size;
    vector_add(v, (vtype)local);
  }
  vector_pop(&frame->stack);
}

ANN static m_bool emit_exp(const Emitter emit, Exp exp, const m_bool add_ref);
ANN static m_bool emit_stmt(const Emitter emit, const Stmt stmt, const m_bool pop);
ANN static m_bool emit_stmt_list(const Emitter emit, Stmt_List list);
ANN static m_bool emit_exp_dot(const Emitter emit, const Exp_Dot* member);
ANN static m_bool emit_func_def(const Emitter emit, const Func_Def func_def);

ANEW ANN Emitter new_emitter(const Env env) {
  Emitter emit = (Emitter)xcalloc(1, sizeof(struct Emitter_));
  vector_init(&emit->stack);
  vector_init(&emit->codes);
  emit->env = env;
  return emit;
}

ANN void free_emitter(Emitter a) {
  free_env(a->env);
  vector_release(&a->stack);
  vector_release(&a->codes);
  free(a);
}

ANEW Instr new_instr() { return mp_alloc(Instr); }
ANN void free_instr(Instr instr) { mp_free(Instr, instr); }

__attribute__((returns_nonnull))
ANN2(1) Instr emitter_add_instr(const Emitter emit, const f_instr f) {
  const Instr instr = mp_alloc(Instr);
  instr->execute = f;
  vector_add(&emit->code->instr, (vtype)instr);
  return instr;
}

ANEW static Code* new_code() {
  Code* code = mp_alloc(Code);
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
  struct Vector_ v;
  vector_init(&v);
  frame_pop_scope(emit->code->frame, &v);
  for(m_uint i = 0; i < vector_size(&v); ++i) {
    Local* l = (Local*)vector_at(&v, i);
    if(l->is_obj) {
      Instr instr = emitter_add_instr(emit, Release_Object);
      instr->m_val = l->offset;
    }
    mp_free(Local, l);
  }
  vector_release(&v);
}

ANN static inline void emit_push_code(const Emitter emit) { GWDEBUG_EXE
  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code();
}

ANN static inline Code* emit_pop_code(const Emitter emit) { GWDEBUG_EXE
  return (Code*)vector_pop(&emit->stack);
}

ANN static inline void emit_push_scope(const Emitter emit) { GWDEBUG_EXE
  frame_push_scope(emit->code->frame);
}

ANN static inline m_uint emit_code_size(const Emitter emit) { GWDEBUG_EXE
  return vector_size(&emit->code->instr);
}

ANN static inline m_uint emit_code_offset(const Emitter emit) { GWDEBUG_EXE
  return emit->code->frame->curr_offset;
}

ANN static inline m_uint emit_alloc_local(const Emitter emit, const m_uint size, const m_bool is_obj) { GWDEBUG_EXE
  const Local* l = frame_alloc_local(emit->code->frame, size, is_obj);
  return l->offset;
}

ANN static void emit_pre_ctor_inner(const Emitter emit, const Type type) { GWDEBUG_EXE
  const Instr instr = emitter_add_instr(emit, Pre_Constructor);
  instr->m_val = (m_uint)type->nspc->pre_ctor;
  instr->m_val2 = (m_uint)emit_code_offset(emit);
}

ANN static void emit_pre_ctor(const Emitter emit, const Type type) { GWDEBUG_EXE
  if(type->parent)
    emit_pre_ctor(emit, type->parent);
  if(type->nspc->pre_ctor)
    emit_pre_ctor_inner(emit, type);
  if(GET_FLAG(type, ae_flag_template) && GET_FLAG(type, ae_flag_builtin)) {
    const m_str name = get_type_name(type->name, 0);
    const Type t = nspc_lookup_type1(type->nspc->parent, insert_symbol(name));
    if(t->nspc->pre_ctor)
      emit_pre_ctor_inner(emit, t);
  }
}

ANN static void emit_pre_constructor_array(const Emitter emit, const Type type) {
  const m_uint start_index = emit_code_size(emit);
  const Instr top = emitter_add_instr(emit, Instr_Pre_Ctor_Array_Top);
  *(Type*)top->ptr = type;
  emit_pre_ctor(emit, type);
  const Instr bottom = emitter_add_instr(emit, Instr_Pre_Ctor_Array_Bottom);
  top->m_val = emit_code_size(emit);
  bottom->m_val = start_index;
  emitter_add_instr(emit, Instr_Pre_Ctor_Array_Post);
}

ANN void free_array_info(VM_Array_Info* info) { mp_free(VM_Array_Info, info); }
ANN VM_Array_Info* emit_array_extend_inner(const Emitter emit, const Type t, const Exp e) { GWDEBUG_EXE
  const Type base = array_base(t);
  CHECK_BO(emit_exp(emit, e, 0))
  VM_Array_Info* info = mp_alloc(VM_Array_Info);
  info->depth = t->array_depth;
  info->type = t;
  info->base = base;
  const Instr alloc = emitter_add_instr(emit, Instr_Array_Alloc);
  *(VM_Array_Info**)alloc->ptr = info;
  if(isa(base, t_object) > 0) {
    emit_pre_constructor_array(emit, base);
    info->is_obj = 1;
  }
  return info;
}

ANN void emit_ext_ctor(const Emitter emit, const VM_Code code) { GWDEBUG_EXE
  emitter_add_instr(emit, Reg_Dup_Last);
  const Instr push_f = emitter_add_instr(emit, Reg_Push_Imm);
  const Instr offset = emitter_add_instr(emit, Reg_Push_Imm);
  push_f->m_val = SZ_INT;
  *(VM_Code*)push_f->ptr = code;
  offset->m_val = SZ_INT;
  *(m_uint*)offset->ptr = emit_code_offset(emit);
  emitter_add_instr(emit, Func_Member);
}

ANN m_bool emit_array_extend(const Emitter emit, const Type t, const Exp e) { GWDEBUG_EXE
  CHECK_OB(emit_array_extend_inner(emit, t, e))
  emitter_add_instr(emit, pop_array_class);
  return 1;
}

ANN2(1,2) m_bool emit_instantiate_object(const Emitter emit, const Type type,
    const Array_Sub array, const m_bool is_ref) {
  if(type->array_depth) {
    VM_Array_Info* info = emit_array_extend_inner(emit, type, array->exp);
    CHECK_OB(info)
    info->is_ref = is_ref;
  } else if(!is_ref) {
    const Instr instr = emitter_add_instr(emit, Instantiate_Object);
    *(Type*)instr->ptr = type;
    emit_pre_ctor(emit, type);
  }
  return 1;
}

ANN static void prepare_this_exp(Exp base, Exp dot) {
  memset(base, 0, sizeof(struct Exp_));
  memset(dot, 0, sizeof(struct Exp_));
  base->meta = ae_meta_var;
  base->exp_type = ae_exp_primary;
  base->d.exp_primary.primary_type = ae_primary_id;
  base->d.exp_primary.d.var = insert_symbol("this");
  base->d.exp_primary.self = base;
  dot->exp_type = ae_exp_dot;
  dot->meta = ae_meta_var;
  dot->d.exp_dot.base = base;
  dot->d.exp_dot.self = dot;
}

ANN static m_bool emit_symbol_owned(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  const Value v = prim->value;
  struct Exp_ base, dot;
  prepare_this_exp(&base, &dot);
  base.type = v->owner_class;
  dot.type = v->type;
  dot.d.exp_dot.t_base = v->owner_class;
  dot.d.exp_dot.xid = prim->d.var;
  dot.emit_var = prim->self->emit_var;
  return emit_exp_dot(emit, &dot.d.exp_dot);
}

ANN static m_bool emit_symbol_builtin(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  const Value v = prim->value;
  const Instr instr = emitter_add_instr(emit, Reg_Push_Imm);
  if(GET_FLAG(v, ae_flag_func)) {
    instr->m_val = SZ_INT;
    *(Func*)instr->ptr = v->d.func_ref;
  } else if(!prim->self->emit_var && isa(v->type, t_object) < 0 && !GET_FLAG(v,ae_flag_enum)) { GWDEBUG_EXE
    instr->m_val = v->type->size;
    if(v->d.ptr)
      memcpy(instr->ptr, v->d.ptr, v->type->size);
  } else {
    instr->m_val = v->type->size;
    *(m_uint*)instr->ptr = (prim->self->emit_var ? (m_uint)&v->d.ptr : (m_uint)v->d.ptr);
  }
  return 1;
}

ANN static m_bool emit_symbol(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  const Value v = prim->value;
  if(v->owner_class)
    return emit_symbol_owned(emit, prim);
  if(GET_FLAG(v, ae_flag_builtin) || GET_FLAG(v, ae_flag_enum))
    return emit_symbol_builtin(emit, prim);
  const Instr instr = emitter_add_instr(emit, prim->self->emit_var ?
      Reg_Push_Mem_Addr : Reg_Push_Mem);
  instr->m_val  = v->offset;
  instr->m_val2 = v->type->size;
  *(m_uint*)instr->ptr = GET_FLAG(v, ae_flag_global);
  return 1;
}

ANEW ANN VM_Code emit_code(const Emitter emit) { GWDEBUG_EXE
  Code* c = emit->code;
  const VM_Code code = new_vm_code(&c->instr, c->stack_depth,
      GET_FLAG(c, ae_flag_member), c->name);
  if(GET_FLAG(c, ae_flag_recurs))
    SET_FLAG(code, ae_flag_recurs);
  vector_add(&emit->codes, (vtype)code);
  free_code(c);
  return code;
}

ANN static m_bool emit_exp_prim_array(const Emitter emit, const Array_Sub array) { GWDEBUG_EXE
  m_uint count = 0;
  Exp e = array->exp;
  CHECK_BB(emit_exp(emit, e, 0))
  do ++count;
  while((e = e->next));
  const Type type = array->type;
  const Type base = array_base(type);
  const Instr instr = emitter_add_instr(emit, Instr_Array_Init);
  instr->m_val2 = base->size;
  VM_Array_Info* info = mp_alloc(VM_Array_Info);
  info->type = type;
  info->base = base;
  info->length = count;
  *(VM_Array_Info**)instr->ptr = info;
  return 1;
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
    const Instr instr = emitter_add_instr(emit, Instr_Array_Access);
    instr->m_val = is_var;
    instr->m_val2 = is_var ? SZ_INT : array->self->type->size;
  } else {
    const Instr instr = emitter_add_instr(emit, Instr_Array_Access_Multi);
    instr->m_val = depth;
    instr->m_val2 = (is_var || array->self->type->array_depth) ?
      SZ_INT : array_base(array->base->type)->size;
    *(m_uint*)instr->ptr = is_var || array->self->type->array_depth;
  }
  return 1;
}

ANN static m_bool emit_exp_prim_vec(const Emitter emit, const Vec* vec, const ae_Exp_Primary_Type t) { GWDEBUG_EXE
  CHECK_BB(emit_exp(emit, vec->exp, 0));
  m_int n = (t == ae_primary_vec ? 3 : 2) - vec->dim + 1;
  while(--n > 0) {
    const Instr push = emitter_add_instr(emit, Reg_Push_Imm);
    push->m_val = SZ_FLOAT;
  }
  return 1;
}

ANN static inline void push_this(const Emitter emit) {
  const Instr instr = emitter_add_instr(emit, Reg_Push_Mem);
  instr->m_val2 = SZ_INT;
}

ANN static m_bool emit_exp_prim_id(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  if(prim->d.var == insert_symbol("this"))
    push_this(emit);
  else if(prim->d.var == insert_symbol("me"))
    emitter_add_instr(emit, Reg_Push_Me);
  else if(prim->d.var == insert_symbol("now"))
    emitter_add_instr(emit, Reg_Push_Now);
  else if(prim->d.var == insert_symbol("false") ||
      prim->d.var == insert_symbol("null") ||
      prim->d.var == insert_symbol("NULL")) {
    const Instr push = emitter_add_instr(emit, Reg_Push_Imm);
    push->m_val = SZ_INT;
  }
  else if(prim->d.var == insert_symbol("true")) {
    const Instr instr = emitter_add_instr(emit, Reg_Push_Imm);
    instr->m_val = SZ_INT;
    *(m_uint*)instr->ptr = 1;
  }  else if(prim->d.var == insert_symbol("maybe"))
    emitter_add_instr(emit, Reg_Push_Maybe);
  else
    emit_symbol(emit, prim);
  return 1;
}

ANN static m_bool emit_exp_prim_num(const Emitter emit, const m_uint i) { GWDEBUG_EXE
  const Instr instr = emitter_add_instr(emit, Reg_Push_Imm);
  instr->m_val = SZ_INT;
  *(m_uint*)instr->ptr = i;
  return 1;
}

ANN static m_bool emit_exp_prim_float(const Emitter emit, const m_float f) { GWDEBUG_EXE
  const Instr instr = emitter_add_instr(emit, Reg_Push_Imm);
  instr->m_val = SZ_FLOAT;
  *(m_float*)instr->ptr = f;
  return 1;
}

ANN static m_bool emit_exp_prim_char(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  const m_int c = str2char(prim->d.chr, prim->self->pos);
  CHECK_BB(c);
  const Instr instr = emitter_add_instr(emit, Reg_Push_Imm);
  instr->m_val = SZ_INT;
  *(m_uint*)instr->ptr = c;
  return 1;
}

ANN static m_bool emit_exp_prim_str(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  char c[strlen(prim->d.str)];
  strcpy(c, prim->d.str);
  CHECK_BB(escape_str(c, prim->self->pos));
  const Instr instr = emitter_add_instr(emit, Reg_Push_Str);
  instr->m_val = (m_uint)s_name(insert_symbol(c));
  return 1;
}

ANN static m_bool emit_exp_prim_gack(const Emitter emit, const Exp exp) { GWDEBUG_EXE
  const Vector v = new_vector();
  m_uint offset = 0;
  Exp e = exp;
  do {
    vector_add(v, (vtype)e->type);
    offset += e->type->size;
    if(e->type != emit->env->class_def)
      ADD_REF(e->type);
  } while((e = e->next));
  if(emit_exp(emit, exp, 0) < 0) { GWDEBUG_EXE
    free_vector(v);
    CHECK_BB(err_msg(EMIT_, exp->pos, "\t... in 'gack' expression."))
  }
  const Instr instr = emitter_add_instr(emit, Gack);
  *(Vector*)instr->ptr = v;
  instr->m_val = offset;
  return 1;
}

ANN static m_bool emit_exp_primary2(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  switch(prim->primary_type) {
    case ae_primary_str:
      return emit_exp_prim_str(emit, prim);
    case ae_primary_array:
      return emit_exp_prim_array(emit, prim->d.array);
    case ae_primary_hack:
      return emit_exp_prim_gack(emit, prim->d.exp);
    case ae_primary_nil:
    default:
      break;
  }
  return 1;
}

ANN static m_bool emit_exp_primary1(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  switch(prim->primary_type) {
    case ae_primary_complex:
    case ae_primary_polar:
    case ae_primary_vec:
      return emit_exp_prim_vec(emit, &prim->d.vec, prim->primary_type);
    default:
      return emit_exp_primary2(emit, prim);
  }
  return 1;
}

#ifdef OPTIMIZE
ANN static m_bool emit_exp_prim_constprop(Emitter emit, const Exp_Primary* prim) {
  const Instr instr = emitter_add_instr(emit, ConstPropSet);
  instr->m_val = prim->value->offset;
  instr->m_val2 = prim->d.num;
  *(m_bool*)instr->ptr = prim->self->emit_var;
  return 1;
}
#endif

ANN static m_bool emit_exp_primary(const Emitter emit, const Exp_Primary* prim) { GWDEBUG_EXE
  switch(prim->primary_type) {
    case ae_primary_id:
      return emit_exp_prim_id(emit, prim);
    case ae_primary_num:
      return emit_exp_prim_num(emit, prim->d.num);
    case ae_primary_float:
      return emit_exp_prim_float(emit, prim->d.fnum);
    case ae_primary_char:
      return emit_exp_prim_char(emit, prim);
#ifdef OPTIMIZE
    case ae_primary_constprop:
      return emit_exp_prim_constprop(emit, prim);
#endif

    default:
      return emit_exp_primary1(emit, prim);
  }
  return 1;
}

ANN static m_bool emit_dot_static_data(const Emitter emit, const Value v, const m_bool emit_var) { GWDEBUG_EXE
  const Instr push = emitter_add_instr(emit, Reg_Push_Imm);
  push->m_val = SZ_INT;
  *(Type*)push->ptr = v->owner_class;
  const Instr alloc = emitter_add_instr(emit, Dot_Static_Data);
  alloc->m_val2 = emit_var ? SZ_INT : v->type->size;
  *(m_uint*)alloc->ptr = emit_var;
  alloc->m_val = v->offset;
  return 1;
}

ANN static m_bool decl_static(const Emitter emit, const Var_Decl var_decl, const m_bool is_ref) {
  const Value v = var_decl->value;
  Code* code = emit->code;

  emit->code = (Code*)vector_back(&emit->stack);
  CHECK_BB(emit_instantiate_object(emit, v->type, var_decl->array, is_ref))
  CHECK_BB(emit_dot_static_data(emit, v, 1))
  emitter_add_instr(emit, Assign_Object);
  emit->code = code;
  return 1;
}

ANN static m_bool emit_exp_decl_static(const Emitter emit, const Var_Decl var_decl, const m_bool is_ref) { GWDEBUG_EXE
  const Value value = var_decl->value;

  if(isa(value->type, t_object) > 0 && !is_ref)
    CHECK_BB(decl_static(emit, var_decl, 0))
  CHECK_BB(emit_dot_static_data(emit, value, 1))
  return 1;
}

ANN static Instr emit_exp_decl_global(const Emitter emit, const Value v, const m_bool is_obj) { GWDEBUG_EXE
  v->offset = emit_alloc_local(emit, v->type->size, is_obj);
  return emitter_add_instr(emit, Alloc_Word);
}

ANN static m_bool emit_exp_decl_non_static(const Emitter emit, const Var_Decl var_decl,
  const m_bool is_ref, const m_bool emit_var) { GWDEBUG_EXE
  const Value value = var_decl->value;
  const Type type = value->type;
  const Array_Sub array = var_decl->array;
  const m_bool is_array = array && array->exp;
  const m_bool is_obj = isa(type, t_object) > 0 || var_decl->array;
  if(is_obj && (is_array || !is_ref))
    CHECK_BB(emit_instantiate_object(emit, type, array, is_ref))

  const Instr alloc = GET_FLAG(value, ae_flag_member) ?
    emitter_add_instr(emit, Alloc_Member) :
    emit_exp_decl_global(emit, value, is_obj);
  alloc->m_val2 = emit_var ? SZ_INT : type->size;
  alloc->m_val = value->offset;
  *(m_uint*)alloc->ptr = ((is_ref && !array) || isa(type, t_object) < 0)  ? emit_var : 1;
  if(is_obj) {
    if(GET_FLAG(type, ae_flag_typedef | ae_flag_ref)) {
      if(!(type->def && type->def->ext &&
          GET_FLAG(type->def->ext, ae_flag_typedef)))
        return 1;
    }
    if((is_array) || !is_ref) {
      const Instr assign = emitter_add_instr(emit, Assign_Object);
      assign->m_val = emit_var;
    }
  }
  return 1;
}

ANN static m_bool emit_class_def(const Emitter emit, const Class_Def class_Def);

ANN static m_bool emit_exp_decl_template(const Emitter emit, const Exp_Decl* decl) { GWDEBUG_EXE
  CHECK_BB(template_push_types(emit->env, decl->base->tmpl->list.list, decl->td->types))
  CHECK_BB(emit_class_def(emit, decl->type->def))
  nspc_pop_type(emit->env->curr);
  return 1;
}

ANN static m_bool emit_exp_decl(const Emitter emit, const Exp_Decl* decl) { GWDEBUG_EXE
  Var_Decl_List list = decl->list;
  const m_bool ref = GET_FLAG(decl->td, ae_flag_ref) || type_ref(decl->type);
  const m_bool var = decl->self->emit_var;

  if(GET_FLAG(decl->type, ae_flag_template))
    CHECK_BB(emit_exp_decl_template(emit, decl))
  do {
    const m_bool r = GET_FLAG(list->self->value, ae_flag_ref) + ref;

    if(isa(list->self->value->type, t_func_ptr) > 0)
      REM_REF(list->self->value->type)

    if(GET_FLAG(decl->td, ae_flag_static))
      CHECK_BB(emit_exp_decl_static(emit, list->self, r))
    else
      CHECK_BB(emit_exp_decl_non_static(emit, list->self, r, var))
  } while((list = list->next));
  return 1;
}

ANN static m_uint vararg_size(const Exp_Func* exp_func, const Vector kinds) {
  Exp e = exp_func->args;
  Arg_List l = exp_func->m_func->def->arg_list;
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

ANN static m_bool emit_func_arg_vararg(const Emitter emit, const Exp_Func* exp_func) { GWDEBUG_EXE
  const Vector kinds = new_vector();
  const Instr instr = emitter_add_instr(emit, MkVararg);
  instr->m_val = vararg_size(exp_func, kinds);
  instr->m_val2 = (m_uint)kinds;
  return 1;
}

ANN static m_bool emit_func_args(const Emitter emit, const Exp_Func* exp_func) { GWDEBUG_EXE
  if(exp_func->args)
    CHECK_BB(emit_exp(emit, exp_func->args, 1))
  if(GET_FLAG(exp_func->m_func->def, ae_flag_variadic))
    CHECK_BB(emit_func_arg_vararg(emit, exp_func))
  return 1;
}

ANN static m_bool emit_exp_call_helper(const Emitter emit, const Exp_Func* exp_func, const m_bool spork) { GWDEBUG_EXE
  if(exp_func->args && !spork)
    CHECK_BB(emit_func_args(emit, exp_func))
  CHECK_BB(emit_exp(emit, exp_func->func, 0))
  if(GET_FLAG(exp_func->m_func->def, ae_flag_variadic) && !exp_func->args) {
    // handle empty call to variadic functions
    const Instr mk = emitter_add_instr(emit, MkVararg);
    *(m_uint*)mk->ptr = 1;
    const Instr push = emitter_add_instr(emit, Reg_Push_Imm);
    push->m_val = SZ_INT;
  }
  return 1;
}

ANN static m_bool emit_exp_call_template(const Emitter emit, const Exp_Func* exp_func, const m_bool spork) { GWDEBUG_EXE
  const Func_Def def = exp_func->m_func->def;
  if(exp_func->m_func->value_ref->owner_class)
    env_push_class(emit->env, exp_func->m_func->value_ref->owner_class);
  SET_FLAG(def, ae_flag_template);
  CHECK_BB(template_push_types(emit->env, def->tmpl->list, exp_func->tmpl->types))
  CHECK_BB(traverse_func_def(emit->env, def))
  CHECK_BB(emit_exp_call_helper(emit, exp_func, spork))
  nspc_pop_type(emit->env->curr);
  if(exp_func->m_func->value_ref->owner_class)
    env_pop_class(emit->env);
  UNSET_FLAG(exp_func->m_func, ae_flag_checked);
  return 1;
}

ANN static m_bool emit_exp_call(const Emitter emit, const Exp_Func* exp_func, const m_bool spork) { GWDEBUG_EXE
  if(!exp_func->tmpl)
    CHECK_BB(emit_exp_call_helper(emit, exp_func, spork))
  else
    CHECK_BB(emit_exp_call_template(emit, exp_func, spork))
  return emit_exp_call1(emit, exp_func->m_func);
}

ANN m_bool emit_exp_binary_ptr(const Emitter emit, const Exp rhs) { GWDEBUG_EXE
  Value v = NULL;
  const Instr instr = emitter_add_instr(emit, assign_func);
  if(rhs->exp_type == ae_exp_dot) {
    Type t = rhs->d.exp_dot.t_base;
    if(isa(t, t_class) > 0)
      t = t->d.base_type;
    v = find_value(t, rhs->d.exp_dot.xid);
    if(GET_FLAG(v, ae_flag_member))
        instr->m_val = 1;
  } else if(rhs->exp_type == ae_exp_primary) {
    if(GET_FLAG(rhs->d.exp_primary.value, ae_flag_member)) {
      v = rhs->d.exp_primary.value;
      instr->m_val = 1;
    }
  } else if(rhs->exp_type == ae_exp_decl)
      v = rhs->d.exp_decl.list->self->value;
  else return -1;
  instr->m_val2 = v ? v->offset : 0;
  return 1;
}

ANN static m_bool emit_binary_func(const Emitter emit, const Exp_Binary* bin) { GWDEBUG_EXE
  const Exp lhs = bin->lhs;
  const Exp rhs = bin->rhs;
  CHECK_BB(emit_exp(emit, lhs, 1))
  if(GET_FLAG(rhs->type->d.func->def, ae_flag_variadic)) {
    Exp_Func exp;
    exp.args = lhs;
    exp.m_func = rhs->type->d.func;
    emit_func_arg_vararg(emit, &exp);
  }
  CHECK_BB(emit_exp(emit, rhs, 1))
  return emit_exp_call1(emit, bin->func);
}

ANN static m_bool emit_exp_binary(const Emitter emit, const Exp_Binary* bin) { GWDEBUG_EXE
  const Exp lhs = bin->lhs;
  const Exp rhs = bin->rhs;
  struct Op_Import opi = { bin->op, lhs->type, rhs->type, NULL,
    NULL, NULL, (uintptr_t)bin };

  if(bin->op == op_chuck && isa(rhs->type, t_function) > 0)
    return emit_binary_func(emit, bin);
  CHECK_BB(emit_exp(emit, lhs, 1))
  CHECK_BB(emit_exp(emit, rhs, 1))
  CHECK_BB(op_emit(emit, &opi))
  return 1;
}

ANN static m_bool exp_exp_cast(const Emitter emit, const Exp_Cast* cast) {
  struct Op_Import opi = { op_dollar, cast->exp->type, cast->self->type, NULL,
    NULL, NULL, (uintptr_t)cast};
  CHECK_BB(emit_exp(emit, cast->exp, 0))
  (void)op_emit(emit, &opi);
  return 1;
}

ANN static m_bool emit_exp_post(const Emitter emit, const Exp_Postfix* post) { GWDEBUG_EXE
  struct Op_Import opi;
  memset(&opi, 0, sizeof(struct Op_Import));
  opi.op = post->op;
  opi.lhs = post->exp->type;
  opi.data = (uintptr_t)post;
  CHECK_BB(emit_exp(emit, post->exp, 1))
  return op_emit(emit, &opi);
}

ANN static m_bool emit_exp_dur(const Emitter emit, const Exp_Dur* dur) { GWDEBUG_EXE
  CHECK_BB(emit_exp(emit, dur->base, 0))
  if(isa(dur->base->type, t_int) > 0)
    emitter_add_instr(emit, Cast_i2f);
  CHECK_BB(emit_exp(emit, dur->unit, 0))
  emitter_add_instr(emit, float_times);
  return 1;
}

ANN static Func emit_get_func(const Nspc nspc, const Func f) { GWDEBUG_EXE
  return isa(f->value_ref->type, t_func_ptr) > 0 ||
         isa(f->value_ref->type, t_class) > 0 ?
         nspc_lookup_func1(f->value_ref->owner, f->def->name) :
         nspc_lookup_func2(nspc, insert_symbol(f->name));
}

ANN static m_bool emit_exp_call1_code(const Emitter emit, const Func func) { GWDEBUG_EXE
  if(!emit_get_func(emit->env->curr, func)) { GWDEBUG_EXE //template with no list
    if(GET_FLAG(func, ae_flag_ref))
      CHECK_BB(traverse_template(emit->env,
            func->value_ref->owner_class->def))
    else if(!GET_FLAG(func->def, ae_flag_template)) {
      const Instr code = emitter_add_instr(emit, INSTR_RECURS);
      code->m_val = (m_uint)func;
      SET_FLAG(emit->code, ae_flag_recurs);
      return 1;
    }
    if(emit_func_def(emit, func->def) < 0)
      CHECK_BB(err_msg(EMIT_, 0, "can't emit func.")) // LCOV_EXCL_LINE
    const Instr code = emitter_add_instr(emit, Reg_Push_Ptr);
    *(VM_Code*)code->ptr = func->code = func->def->func->code;
  } else {
    const Instr code = emitter_add_instr(emit, Reg_Push_Code);
    code->m_val = func->value_ref->offset;
    code->m_val2 = func->value_ref->owner_class ? 1 : 0;
  }
  return 1;
}

ANN static m_bool emit_exp_call1_offset(const Emitter emit) { GWDEBUG_EXE
  const Instr offset = emitter_add_instr(emit, Reg_Push_Imm);
  offset->m_val = SZ_INT;
  *(m_uint*)offset->ptr = emit_code_offset(emit);
  return 1;
}

ANN m_bool emit_exp_call1_builtin(const Emitter emit, const Func func) { GWDEBUG_EXE
  Type t = func->value_ref->type;
  if(isa(t, t_class) > 0)
    t = t->d.base_type;
  if(isa(t, t_func_ptr) < 0)
  if(!func->code || !func->code->native_func)
    CHECK_BB(err_msg(EMIT_, func->def->td->pos,
          "missing native func. are you trying to spork?"))
  const Instr call = emitter_add_instr(emit, GET_FLAG(func, ae_flag_member) ?
    Func_Member : Func_Static);
  call->m_val = func->def->ret_type->size;
  return 1;
}

ANN static m_bool emit_exp_call1_op(const Emitter emit, const Arg_List list) { GWDEBUG_EXE
  const Instr call    = emitter_add_instr(emit, Call_Binary);
  call->m_val   = emit->code->stack_depth;
  call->m_val2  = (m_uint)list->type;
  *(Type*)call->ptr     = list->next ? list->next->type : NULL;
  return 1;
}

ANN static m_bool emit_exp_call1_usr(const Emitter emit) { GWDEBUG_EXE
  const Instr call = emitter_add_instr(emit, Instr_Exp_Func);
  call->m_val = emit->code->stack_depth;
  return 1;
}

ANN m_bool emit_exp_call1(const Emitter emit, const Func func) { GWDEBUG_EXE
#ifdef OPTIMIZE
  if(GET_FLAG(func, ae_flag_inline)) {
    SET_FLAG(func, ae_flag_recurs);
    SET_FLAG(emit->code, ae_flag_recurs);
    const Instr instr = emitter_add_instr(emit, INSTR_INLINE);
    *(Func*)instr->ptr = func;
    instr->m_val = emit_code_offset(emit);
    return 1;
  }
#endif
  if(!func->code ||
      (GET_FLAG(func, ae_flag_ref) && !GET_FLAG(func, ae_flag_builtin)))
    CHECK_BB(emit_exp_call1_code(emit, func))
  else {
    const Instr code = emitter_add_instr(emit, Reg_Push_Ptr);
    *(VM_Code*)code->ptr = func->code;
  }
  CHECK_BB(emit_exp_call1_offset(emit))
  if(GET_FLAG(func->def, ae_flag_builtin))
    CHECK_BB(emit_exp_call1_builtin(emit, func))
  else if(GET_FLAG(func->def, ae_flag_op))
    CHECK_BB(emit_exp_call1_op(emit, func->def->arg_list))
  else
    CHECK_BB(emit_exp_call1_usr(emit))
  return 1;
}

ANN2(1,2) static m_bool emit_exp_spork_finish(const Emitter emit, const VM_Code code, const Func f,
    const m_uint arg_size, const m_uint stack_depth) {
  const Instr push_code = emitter_add_instr(emit, Reg_Push_Imm);
  push_code->m_val = SZ_INT;
  *(VM_Code*)push_code->ptr = code;
  const Instr spork = emitter_add_instr(emit, Spork);
  spork->m_val = arg_size;
  spork->m_val2 = (m_uint)f;
  *(m_uint*)spork->ptr = stack_depth; // only for some sporked expressions
  return 1;
}

ANN static m_uint emit_exp_spork_size(Exp e) { GWDEBUG_EXE
  m_uint size = 0;
  do size += e->cast_to ? e->cast_to->size : e->type->size;
  while((e = e->next));
  return size;
}

ANN m_bool emit_exp_spork(const Emitter emit, const Exp_Func* exp) { GWDEBUG_EXE
  CHECK_BB(emit_func_args(emit, exp))
  CHECK_BB(emit_exp(emit, exp->func, 0))
  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code();
  emitter_add_instr(emit, start_gc);
  if(GET_FLAG(exp->m_func, ae_flag_member))
    SET_FLAG(emit->code, ae_flag_member);
#ifdef OPTIMIZE
  if(GET_FLAG(exp->m_func, ae_flag_inline))
    SET_FLAG(emit->code, ae_flag_recurs);
#endif
  char c[11 + num_digit(exp->self->pos)];
  sprintf(c, "spork~exp:%i", exp->self->pos);
  emit->code->name = code_name_set(c, emit->filename);
  const Instr op = emitter_add_instr(emit, Mem_Push_Imm);
  CHECK_BB(emit_exp_call1(emit, exp->m_func))
  emitter_add_instr(emit, stop_gc);
  emitter_add_instr(emit, EOC);
  op->m_val = emit->code->stack_depth;
  const VM_Code code = emit_code(emit);
  emit->code = (Code*)vector_pop(&emit->stack);
  const m_uint size = exp->args ? emit_exp_spork_size(exp->args) : 0;
  CHECK_BB(emit_exp_spork_finish(emit, code, NULL, size, 0)) // last arg migth have to be 'emit_code_offset(emit)'
  return 1;
}

ANN m_bool emit_exp_spork1(const Emitter emit, const Stmt stmt) { GWDEBUG_EXE
  const Symbol sporked = insert_symbol("sporked");
  const ID_List list = new_id_list(sporked, stmt->pos);
  const Func f = new_func("sporked", new_func_def(0, new_type_decl(list, 0, stmt->pos), sporked, NULL, stmt));

  if(emit->env->class_def)
    push_this(emit);
  const Instr push = emitter_add_instr(emit, Reg_Push_Imm);
  push->m_val = SZ_INT;
  *(Func*)push->ptr = f;

  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code();
  if(emit->env->class_def) {
    SET_FLAG(f, ae_flag_member);
    SET_FLAG(emit->code, ae_flag_member);
  }
  char c[12 + num_digit(stmt->pos)];
  sprintf(c, "spork~code:%i", stmt->pos);
  emit->code->name = code_name_set(c, emit->filename);
  const Instr op = emitter_add_instr(emit, Mem_Push_Imm);
  emit_push_scope(emit);
  emitter_add_instr(emit, start_gc);
  CHECK_BB(emit_stmt(emit, stmt, 0))
  emitter_add_instr(emit, stop_gc);
  emit_pop_scope(emit);
  emitter_add_instr(emit, EOC);
  op->m_val = emit->code->stack_depth;
  f->code = emit_code(emit);
  emit->code = (Code*)vector_pop(&emit->stack);
  CHECK_BB(emit_exp_spork_finish(emit, f->code, f, 0, emit->env->func ?
      emit->env->func->def->stack_depth : 0))
  return 1;
}

ANN static m_bool emit_exp_unary(const Emitter emit, const Exp_Unary* unary) { GWDEBUG_EXE
  struct Op_Import opi;
  memset(&opi, 0, sizeof(struct Op_Import));
  opi.op = unary->op;
  opi.data = (uintptr_t)unary;
  if(unary->op != op_spork && unary->exp) {
    CHECK_BB(emit_exp(emit, unary->exp, 1))
    opi.rhs = unary->exp->type;
  }
  return op_emit(emit, &opi);
}

ANN static m_bool emit_implicit_cast(const Emitter emit, const Type from, const Type to) { GWDEBUG_EXE
  struct Op_Import opi = { op_implicit, from, to, NULL,
         NULL, NULL, (m_uint)from };
  op_emit(emit, &opi);
  return 1;
}

ANN static Instr emit_flow(const Emitter emit, const Type type, const f_instr f1, const f_instr f2) { GWDEBUG_EXE
  const Instr push = emitter_add_instr(emit, Reg_Push_Imm);
  if(isa(type, t_float) > 0) {
    push->m_val = SZ_FLOAT;
    return emitter_add_instr(emit, f2);
  }
  push->m_val = SZ_INT;
  return emitter_add_instr(emit, f1);
}

ANN static m_bool emit_exp_if(const Emitter emit, const Exp_If* exp_if) { GWDEBUG_EXE
  nspc_push_value(emit->env->curr);
  CHECK_BB(emit_exp(emit, exp_if->cond, 0))
  const Instr op = emit_flow(emit, exp_if->cond->type, Branch_Eq_Int, Branch_Eq_Float);
  CHECK_BB(emit_exp(emit, exp_if->if_exp, 0))
  const Instr op2 = emitter_add_instr(emit, Goto);
  op->m_val = emit_code_size(emit);
  const m_bool ret = emit_exp(emit, exp_if->else_exp, 0);
  nspc_pop_value(emit->env->curr);
  op2->m_val = emit_code_size(emit);
  return ret;
}

#ifdef OPTIMIZE
ANN static void emit_exp_constprop(const Emitter emit, const Exp e) {
  if(!e->emit_var) {
    if(e->exp_type == ae_exp_constprop) {
      const Instr instr = emitter_add_instr(emit, Reg_Push_Mem);
      instr->m_val = e->d.exp_primary.value->offset;
      instr->m_val2 = SZ_INT;
      *(m_uint*)instr->ptr = GET_FLAG(e->d.exp_primary.value, ae_flag_global);
      return;
    }
    const Instr instr = emitter_add_instr(emit, ConstPropGet);
    instr->m_val2 = e->d.exp_primary.value->offset;
    instr->m_val = e->d.exp_primary.d.num;
    *(m_uint*)instr->ptr = 1;
  } else {
    const Instr instr = emitter_add_instr(emit, ConstPropSet);
    instr->m_val = e->d.exp_primary.value->offset;
    *(m_bool*)instr->ptr = 1;
    instr->m_val2 = e->d.exp_primary.d.num;
  }
}
#endif

ANN2(1) static m_bool emit_exp(const Emitter emit, Exp exp, const m_bool ref) { GWDEBUG_EXE
  do {
    switch(exp->exp_type) {
      case ae_exp_decl:
        CHECK_BB(emit_exp_decl(emit, &exp->d.exp_decl))       break;
      case ae_exp_primary:
        CHECK_BB(emit_exp_primary(emit, &exp->d.exp_primary)) break;
      case ae_exp_unary:
        CHECK_BB(emit_exp_unary(emit, &exp->d.exp_unary))     break;
      case ae_exp_binary:
        CHECK_BB(emit_exp_binary(emit, &exp->d.exp_binary))   break;
      case ae_exp_post:
        CHECK_BB(emit_exp_post(emit, &exp->d.exp_post)) break;
      case ae_exp_cast:
        CHECK_BB(exp_exp_cast(emit, &exp->d.exp_cast))        break;
      case ae_exp_dot:
        CHECK_BB(emit_exp_dot(emit, &exp->d.exp_dot))         break;
      case ae_exp_call:
        CHECK_BB(emit_exp_call(emit, &exp->d.exp_func, 0))    break;
      case ae_exp_array:
        CHECK_BB(emit_exp_array(emit, &exp->d.exp_array))     break;
      case ae_exp_if:
        CHECK_BB(emit_exp_if(emit, &exp->d.exp_if))           break;
      case ae_exp_dur:
        CHECK_BB(emit_exp_dur(emit, &exp->d.exp_dur))         break;
#ifdef OPTIMIZE
      case ae_exp_constprop2:
      case ae_exp_constprop:
        emit_exp_constprop(emit, exp);
        break;
#endif
    }
    if(exp->cast_to)
      CHECK_BB(emit_implicit_cast(emit, exp->type, exp->cast_to))
      if(ref && isa(exp->type, t_object) > 0 && isa(exp->type, t_void) < 0) {
        const Instr ref = emitter_add_instr(emit, Reg_Add_Ref);
        ref->m_val = exp->emit_var;
      }
  } while((exp = exp->next));
  return 1;
}

ANN static m_bool emit_stmt_if(const Emitter emit, const Stmt_If stmt) { GWDEBUG_EXE
  emit_push_scope(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  COVERAGE(stmt->cond)
  const Instr op = emit_flow(emit, isa(stmt->cond->type, t_object) > 0 ?
      t_int : stmt->cond->type, Branch_Eq_Int, Branch_Eq_Float);
  CHECK_OB(op)
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->if_body, 1))
  emit_pop_scope(emit);
  const Instr op2 = emitter_add_instr(emit, Goto);
  op->m_val = emit_code_size(emit);

  if(stmt->else_body) {
    emit_push_scope(emit);
    CHECK_BB(emit_stmt(emit, stmt->else_body, 1))
    emit_pop_scope(emit);
  }
  op2->m_val = emit_code_size(emit);
  emit_pop_scope(emit);
  return 1;
}

ANN static m_bool emit_stmt_code(const Emitter emit, const Stmt_Code stmt) { GWDEBUG_EXE
  return stmt->stmt_list ? emit_stmt_list(emit, stmt->stmt_list) : 1;
}

#ifdef OPTIMIZE
ANN static m_bool optimize_taill_call(const Emitter emit, const Exp_Func* e) {
  Exp arg = e->args;
  CHECK_BB(emit_exp(emit, e->args, 0))
  const Instr instr = emitter_add_instr(emit, PutArgsInMem);
  while(arg) {
    instr->m_val += arg->type->size;
    arg = arg->next;
  }
  emitter_add_instr(emit, Goto);
  return 1;
}
#define OPTIMIZE_TCO\
  if(stmt->val->exp_type == ae_exp_call && emit->env->func == stmt->val->d.exp_func.m_func)\
    return optimize_taill_call(emit, &stmt->val->d.exp_func);
#else
#define OPTIMIZE_TCO
#endif


ANN static m_bool emit_stmt_return(const Emitter emit, const Stmt_Exp stmt) { GWDEBUG_EXE
  if(stmt->val) {
    OPTIMIZE_TCO
    CHECK_BB(emit_exp(emit, stmt->val, 0))
  }
  vector_add(&emit->code->stack_return, (vtype)emitter_add_instr(emit, Goto));
  return 1;
}

ANN static m_bool emit_stmt_continue(const Emitter emit, const Stmt stmt __attribute__((unused))) { GWDEBUG_EXE
  vector_add(&emit->code->stack_cont, (vtype)emitter_add_instr(emit, Goto));
  return 1;
}

ANN static m_bool emit_stmt_break(const Emitter emit, const Stmt stmt __attribute__((unused))) { GWDEBUG_EXE
  vector_add(&emit->code->stack_break, (vtype)emitter_add_instr(emit, Goto));
  return 1;
}

ANN static void emit_push_stack(const Emitter emit) { GWDEBUG_EXE
  vector_add(&emit->code->stack_cont, (vtype)NULL);
  vector_add(&emit->code->stack_break, (vtype)NULL);
}

ANN static void emit_pop_stack(const Emitter emit, const m_uint index) { GWDEBUG_EXE
  while(vector_back(&emit->code->stack_cont)) {
    const Instr instr = (Instr)vector_pop(&emit->code->stack_cont);
    instr->m_val = index;
  }
  while(vector_back(&emit->code->stack_break)) {
    const Instr instr = (Instr)vector_pop(&emit->code->stack_break);
    instr->m_val = emit_code_size(emit);
  }
  emit_pop_scope(emit);
  vector_pop(&emit->code->stack_cont);
  vector_pop(&emit->code->stack_break);
}

ANN Instr _flow(const Emitter emit, const Exp e, const m_bool b) {
  CHECK_BO(emit_exp(emit, e, 0))
  COVERAGE(e)
  const f_instr instr_i = b ? Branch_Eq_Int : Branch_Neq_Int;
  const f_instr instr_f = b ? Branch_Eq_Float : Branch_Neq_Float;
  return emit_flow(emit, e->type, instr_i, instr_f);
}

  ANN static m_bool emit_stmt_flow(const Emitter emit, const Stmt_Flow stmt) { GWDEBUG_EXE
  const m_uint index = emit_code_size(emit);
  Instr op = NULL;
  emit_push_scope(emit);
  emit_push_stack(emit);
  if(!stmt->is_do)
    op = _flow(emit, stmt->cond, stmt->self->stmt_type == ae_stmt_while);
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);
  if(stmt->is_do) {
    op = _flow(emit, stmt->cond, stmt->self->stmt_type != ae_stmt_while);
    op->m_val = index;
  } else {
    const Instr goto_ = emitter_add_instr(emit, Goto);
    goto_->m_val = index;
    op->m_val = emit_code_size(emit);
  }
  emit_pop_stack(emit, index);
  return 1;
}

ANN static m_bool emit_stmt_for(const Emitter emit, const Stmt_For stmt) { GWDEBUG_EXE
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->c1, 1))
  const m_uint index = emit_code_size(emit);
  emit_push_stack(emit);
  CHECK_BB(emit_stmt(emit, stmt->c2, 0))
  const Instr op = emit_flow(emit, stmt->c2->d.stmt_exp.val->type,
                           Branch_Eq_Int, Branch_Eq_Float);
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);
  const m_uint action_index = emit_code_size(emit);
  if(stmt->c3) {
    CHECK_BB(emit_exp(emit, stmt->c3, 0))
    Exp e = stmt->c3;
    m_uint num_words = 0;
    do num_words += e->type->size;
    while((e = e->next));
    if(num_words) {
      const Instr pop = emitter_add_instr(emit, Reg_Pop_Word);
      pop->m_val = num_words;
    }
  }
  const Instr _goto = emitter_add_instr(emit, Goto);
  _goto->m_val = index;
  op->m_val = emit_code_size(emit);
  emit_pop_stack(emit, action_index);
  return 1;
}

ANN static m_bool emit_stmt_auto(const Emitter emit, const Stmt_Auto stmt) { GWDEBUG_EXE
  CHECK_BB(emit_exp(emit, stmt->exp, 0))
  const Instr s1 = emitter_add_instr(emit, Mem_Set_Imm);
  const Instr s2 = emitter_add_instr(emit, Mem_Set_Imm);
  const m_uint start  = emit_code_size(emit);
  emit_push_stack(emit);
  const Instr loop = emitter_add_instr(emit, AutoLoopStart);
  const m_uint offset = emit_alloc_local(emit, 2*SZ_INT, 0);
  stmt->v->offset = offset + SZ_INT;
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);
  const m_uint index = emit_code_size(emit);
  const Instr end = emitter_add_instr(emit, AutoLoopEnd);
  const Instr tgt = emitter_add_instr(emit, Goto);
  end->m_val2 = emit_code_size(emit);
  tgt->m_val = start;
  loop->m_val = offset;
  end->m_val = offset;
  s1->m_val = offset;
  s2->m_val = offset + SZ_INT;
  if(stmt->is_ptr) {
    *(Type*)loop->ptr = stmt->v->type;
    *(Type*)end->ptr = stmt->v->type;
  }
  emit_pop_stack(emit, index);
  return 1;
}

ANN static m_bool emit_stmt_loop(const Emitter emit, const Stmt_Loop stmt) { GWDEBUG_EXE
  emit_push_scope(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  const m_int* counter = (m_int*)xcalloc(1, SZ_INT);
  const Instr init = emitter_add_instr(emit, Init_Loop_Counter);
  init->m_val = (m_uint)counter;
  const m_uint index = emit_code_size(emit);
  emit_push_stack(emit);
  const Instr deref = emitter_add_instr(emit, Reg_Push_Deref);
  deref->m_val = (m_uint)counter;
  const Instr push = emitter_add_instr(emit, Reg_Push_Imm);
  push->m_val = SZ_INT;
  const Instr op = emitter_add_instr(emit, Branch_Eq_Int);
  const Instr dec = emitter_add_instr(emit, Dec_int_Addr);
  dec->m_val = (m_uint)counter;

  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);

  const Instr _goto = emitter_add_instr(emit, Goto);
  _goto->m_val = index;
  op->m_val = emit_code_size(emit);
  emit_pop_stack(emit, index);
  return 1;
}

ANN static m_bool emit_stmt_gotolabel(const Emitter emit, const Stmt_Goto_Label stmt) { GWDEBUG_EXE
  if(!stmt->is_label)
    stmt->data.instr = emitter_add_instr(emit, Goto);
  else {
    if(emit->cases && !strcmp(s_name(stmt->name), "default")) {
      if(emit->default_case_index != -1)
        CHECK_BB(err_msg(EMIT_, stmt->self->pos, "default case already defined"))
        emit->default_case_index = emit_code_size(emit);
      return 1;
    }
    if(!stmt->data.v.ptr) {
      emit->cases = NULL; // check me (mem leak?)
      CHECK_BB(err_msg(EMIT_, stmt->self->pos, "illegal case"))
    }
    const m_uint size = vector_size(&stmt->data.v);
    if(!size) {
      vector_release(&stmt->data.v);
      CHECK_BB(err_msg(EMIT_, stmt->self->pos, "label '%s' defined but not used.", s_name(stmt->name)))
    }
    LOOP_OPTIM
    for(m_uint i = size + 1; --i;) {
      const Stmt_Goto_Label label = (Stmt_Goto_Label)vector_at(&stmt->data.v, i - 1);
      if(!label->data.instr) {
        vector_release(&stmt->data.v);
        CHECK_BB(err_msg(EMIT_, label->self->pos, "you are trying to use a upper label."))
      }
      label->data.instr->m_val = emit_code_size(emit);
    }
    vector_release(&stmt->data.v);
  }
  return 1;
}

ANN static m_bool emit_stmt_switch(const Emitter emit, const Stmt_Switch stmt) { GWDEBUG_EXE
  vector_add(&emit->code->stack_break, (vtype)NULL);
  CHECK_BB(emit_exp(emit, stmt->val, 0))
  if(emit->cases)
    CHECK_BB(err_msg(EMIT_, stmt->self->pos, "swith inside an other switch. this is not allowed for now"))
    emit->default_case_index = -1;
  emitter_add_instr(emit, start_gc);
  const Instr instr = emitter_add_instr(emit, Branch_Switch);
  emit->cases = new_map();
  *(Map*)instr->ptr = emit->cases;

  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->stmt, 1))
  emit_pop_scope(emit);
  instr->m_val = emit->default_case_index > -1 ? (m_uint)emit->default_case_index : emit_code_size(emit);
  emit->default_case_index = -1;
  while(vector_back(&emit->code->stack_break)) {
    const Instr _break = (Instr)vector_pop(&emit->code->stack_break);
    _break->m_val = emit_code_size(emit);
  }
  vector_pop(&emit->code->stack_break);
  emitter_add_instr(emit, stop_gc);
  emit->cases = NULL;
  return 1;
}

ANN static m_bool primary_case(const Exp_Primary* prim, m_int* value) {
  if(prim->primary_type == ae_primary_num)
    *value = prim->d.num;
  else if(prim->d.var == insert_symbol("true"))
    *value = 1;
  else if(prim->d.var == insert_symbol("false"))
    *value = 0;
  else if(prim->d.var == insert_symbol("maybe"))
    CHECK_BB(err_msg(EMIT_, prim->self->pos, "'maybe' is not constant."))
  else {
    if(!GET_FLAG(prim->value, ae_flag_const))
      CHECK_BB(err_msg(EMIT_, prim->self->pos,
            "value is not constant."))
    *value = (m_uint)prim->value->d.ptr; // assume enum.
  }
  return 1;
}

ANN static m_int get_case_value(const Stmt_Exp stmt, m_int* value) {
  if(stmt->val->exp_type == ae_exp_primary)
    CHECK_BB(primary_case(&stmt->val->d.exp_primary, value))
  else {
    const Type t = isa(stmt->val->d.exp_dot.t_base, t_class) > 0 ?
        stmt->val->d.exp_dot.t_base->d.base_type :
        stmt->val->d.exp_dot.t_base;
    const Value v = find_value(t, stmt->val->d.exp_dot.xid);
    *value = GET_FLAG(v, ae_flag_enum) ? !GET_FLAG(v, ae_flag_builtin) ?
      (m_uint)t->nspc->class_data[v->offset] : (m_uint)v->d.ptr : *(m_uint*)v->d.ptr;
  }
  return 1;
}

ANN static m_bool emit_stmt_case(const Emitter emit, const Stmt_Exp stmt) { GWDEBUG_EXE
  m_int value = 0;
  if(!emit->cases)
    CHECK_BB(err_msg(EMIT_, stmt->self->pos, "case found outside switch statement. this is not allowed for now"))
  CHECK_BB(get_case_value(stmt, &value))
  if(map_get(emit->cases, (vtype)value))
    CHECK_BB(err_msg(EMIT_, stmt->self->pos, "duplicated cases value %i", value))
  map_set(emit->cases, (vtype)value, (vtype)emit_code_size(emit));
  return 1;
}

ANN static m_bool emit_stmt_fptr(const Emitter emit __attribute__((unused)), const Stmt_Ptr ptr) { GWDEBUG_EXE
  if(GET_FLAG(ptr, ae_flag_static))
    ADD_REF(ptr->func)
  return 1;
}

ANN static m_bool emit_stmt_type(const Emitter emit, const Stmt_Typedef stmt) { GWDEBUG_EXE
  return stmt->type->def ? emit_class_def(emit, stmt->type->def) : 1;
}

ANN static m_bool emit_stmt_enum(const Emitter emit, const Stmt_Enum stmt) { GWDEBUG_EXE
  LOOP_OPTIM
  for(m_uint i = 0; i < vector_size(&stmt->values); ++i) {
    const Value v = (Value)vector_at(&stmt->values, i);
    if(!emit->env->class_def) {
      const m_uint offset = emit_alloc_local(emit, SZ_INT, 0);
      v->offset = offset;
      v->d.ptr = (m_uint*)i;
    } else
      emit->env->class_def->nspc->class_data[v->offset] = i;
  }
  return 1;
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

  if(stmt->xid) {
    if(!stmt->value->type->nspc->class_data)
      stmt->value->type->nspc->class_data =
        (m_bit*)xcalloc(1, stmt->value->type->nspc->class_data_size);
    Type_Decl *type_decl = new_type_decl(new_id_list(stmt->xid, stmt->self->pos),
        0, emit->env->class_def ? ae_flag_member : 0);
    type_decl->flag = stmt->flag;
    const Var_Decl var_decl = new_var_decl(stmt->xid, NULL, 0);
    const Var_Decl_List var_decl_list = new_var_decl_list(var_decl, NULL);
    const Exp exp = new_exp_decl(type_decl, var_decl_list, 0);
    exp->d.exp_decl.type = stmt->value->type;
    var_decl->value = stmt->value;
    CHECK_BB(emit_exp_decl(emit, &exp->d.exp_decl))
    if(!emit->env->class_def)
      ADD_REF(stmt->value);
    free_exp(exp);
    env_push_class(emit->env, stmt->value->type);
  } else if(!GET_FLAG(l->self->d.exp_decl.list->self->value, ae_flag_member))
    stmt->o = emit_alloc_local(emit, stmt->s, 0);
  emit_union_offset(stmt->l, stmt->o);
  if(stmt->xid) {
    const Instr instr = emitter_add_instr(emit, Reg_Pop_Word);
    instr->m_val = SZ_INT;
    env_pop_class(emit->env);
  }
  return 1;
}

ANN static m_uint get_decl_size(Var_Decl_List a) {
  m_uint size = 0;
  do size += a->self->value->type->size;
  while((a = a->next));
  return size;
}

ANN static void pop_exp(const Emitter emit, Exp e) {
  do {
    if(e->type->size) {
      const Instr instr = emitter_add_instr(emit, Reg_Pop_Word);
      instr->m_val = (e->exp_type == ae_exp_decl ?
        get_decl_size(e->d.exp_decl.list) : e->type->size);
    }
  } while((e = e->next));
}

ANN static m_bool emit_stmt_exp(const Emitter emit, const struct Stmt_Exp_* exp, const m_bool pop) { GWDEBUG_EXE
  CHECK_BB(emit_exp(emit, exp->val, 0))
  if(pop && exp->val->type->size) {
    Exp e = exp->val;
    do {
      if(e->exp_type == ae_exp_primary &&
          e->d.exp_primary.primary_type == ae_primary_hack) {
        pop_exp(emit, e->d.exp_primary.d.exp);
        continue;
      }
      pop_exp(emit, e);
    } while((e = e->next));
  }
 return 1;
}

ANN static m_bool emit_stmt(const Emitter emit, const Stmt stmt, const m_bool pop) { GWDEBUG_EXE
  if(stmt->stmt_type != ae_stmt_if || stmt->stmt_type != ae_stmt_while)
    COVERAGE(stmt)
  switch(stmt->stmt_type) {
    case ae_stmt_exp:
      return stmt->d.stmt_exp.val ?
          emit_stmt_exp(emit, &stmt->d.stmt_exp, pop) : 1;
    case ae_stmt_code:
      return emit_stmt_code(emit, &stmt->d.stmt_code);
    case ae_stmt_if:
      return emit_stmt_if(emit, &stmt->d.stmt_if);
    case ae_stmt_return:
      return emit_stmt_return(emit, &stmt->d.stmt_exp);
    case ae_stmt_break:
      return emit_stmt_break(emit, stmt);
    case ae_stmt_continue:
      return emit_stmt_continue(emit, stmt);
    case ae_stmt_while:
    case ae_stmt_until:
      return emit_stmt_flow(emit, &stmt->d.stmt_flow);
    case ae_stmt_for:
      return emit_stmt_for(emit, &stmt->d.stmt_for);
    case ae_stmt_auto:
      return emit_stmt_auto(emit, &stmt->d.stmt_auto);
    case ae_stmt_loop:
      return emit_stmt_loop(emit, &stmt->d.stmt_loop);
    case ae_stmt_gotolabel:
      return emit_stmt_gotolabel(emit, &stmt->d.stmt_gotolabel);
    case ae_stmt_case:
      return emit_stmt_case(emit, &stmt->d.stmt_exp);
    case ae_stmt_enum:
      return emit_stmt_enum(emit, &stmt->d.stmt_enum);
    case ae_stmt_switch:
      return emit_stmt_switch(emit, &stmt->d.stmt_switch);
    case ae_stmt_funcptr:
      return emit_stmt_fptr(emit, &stmt->d.stmt_ptr);
    case ae_stmt_typedef:
      return emit_stmt_type(emit, &stmt->d.stmt_type);
    case ae_stmt_union:
      return emit_stmt_union(emit, &stmt->d.stmt_union);
  }
  return 1;
}

ANN static m_bool emit_stmt_list(const Emitter emit, Stmt_List l) { GWDEBUG_EXE
  do CHECK_BB(emit_stmt(emit, l->stmt, 1))
  while((l = l->next));
  return 1;
}

ANN static m_bool is_special(const Type t) {
  if(isa(t, t_complex) > 0 || isa(t, t_polar) > 0 ||
     isa(t, t_vec3)    > 0 || isa(t, t_vec4)  > 0  ||
     isa(t, t_vararg)  > 0)
    return 1;
  return -1;
}

ANN static m_bool emit_dot_static_import_data(const Emitter emit, const Value v, const m_bool emit_addr) { GWDEBUG_EXE
  if(v->d.ptr && GET_FLAG(v, ae_flag_builtin)) { // from C
    if(GET_FLAG(v, ae_flag_enum)) {
      const Instr func_i = emitter_add_instr(emit, Reg_Push_Imm);
      *(m_uint*)func_i->ptr = (m_uint)v->d.ptr;
      func_i->m_val = SZ_INT;
    } else {
      const Instr func_i = emitter_add_instr(emit, Dot_Static_Import_Data);
      func_i->m_val = (isa(v->type, t_object) > 0 ?
        (m_uint)&v->d.ptr : (m_uint)v->d.ptr);
      func_i->m_val2 = emit_addr ? SZ_INT : v->type->size;
      *(m_uint*)func_i->ptr = emit_addr;
    }
  } else { // from code
    const Instr push_i = emitter_add_instr(emit, Reg_Push_Imm);
    const Instr func_i = emitter_add_instr(emit, Dot_Static_Data);
    push_i->m_val = SZ_INT;
    *(Type*)push_i->ptr = v->owner_class;
    func_i->m_val = (m_uint)v->offset;
    func_i->m_val2 = emit_addr ? SZ_INT : v->type->size;
    *(m_uint*)func_i->ptr = emit_addr;
  }
  return 1;
}

ANN static m_bool emit_complex_member(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  const Exp base = member->base;
  base->emit_var = 1;
  CHECK_BB(emit_exp(emit, base, 0))
  const m_bool is_complex = !strcmp((isa(base->type, t_complex) > 0  ? "re" : "phase") ,
        s_name(member->xid));
  const Instr instr = is_complex ? emitter_add_instr(emit, complex_real) :
      emitter_add_instr(emit, complex_imag);
  instr->m_val = member->self->emit_var;
  return 1;
}

ANN static m_bool emit_vec_func(const Emitter emit, const Value v) { GWDEBUG_EXE
  emitter_add_instr(emit, Reg_Dup_Last);
  const Instr instr = emitter_add_instr(emit, member_function);
  *(Vector*)instr->ptr = &v->owner_class->nspc->vtable;
  instr->m_val = v->d.func_ref->vt_index;
  return 1;
}

ANN static m_bool emit_vec_member(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  member->base->emit_var = 1;
  CHECK_BB(emit_exp(emit, member->base, 0))
  const Value v = find_value(member->base->type, member->xid);
  if(GET_FLAG(v, ae_flag_func))
    return emit_vec_func(emit, v);
  const Instr instr = emitter_add_instr(emit, vec_member);
  instr->m_val2 = v->offset;
  instr->m_val = member->self->emit_var;
  return 1;
}

ANN static m_bool emit_vararg_start(const Emitter emit, const m_uint offset) { GWDEBUG_EXE
  if(emit->env->func->variadic)
    CHECK_BB(err_msg(EMIT_, 0, "vararg.start already used. this is an error"))
  emit->env->func->variadic = emitter_add_instr(emit, Vararg_start);
  emit->env->func->variadic->m_val = offset;
  emit->env->func->variadic->m_val2 = emit_code_size(emit);
  return 1;
}

ANN static m_bool emit_vararg_end(const Emitter emit, const m_uint offset) { GWDEBUG_EXE
  if(!emit->env->func->variadic)
    CHECK_BB(err_msg(EMIT_, 0, "vararg.start not used before vararg.end. this is an error"))
  const Instr instr = emitter_add_instr(emit, Vararg_end);
  instr->m_val = offset;
  instr->m_val2 = emit->env->func->variadic->m_val2;
  emit->env->func->variadic->m_val2 = emit_code_size(emit);
  *(m_uint*)emit->env->func->variadic->ptr = 1;
  return 1;
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
  const Instr instr = emitter_add_instr(emit, Vararg_Member);
  instr->m_val = offset;
  instr->m_val2 = member->self->type->size;
  return 1;
}

ANN static m_bool emit_exp_dot_special(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  const Type t = member->t_base;

  if(isa(t, t_complex) > 0 || isa(t, t_polar) > 0)
    return emit_complex_member(emit, member);
  else if(isa(t, t_vec3) > 0 || isa(t, t_vec4) > 0)
    return emit_vec_member(emit, member);
  return emit_vararg(emit, member);
}

ANN static m_bool emit_dot_static_func(const Emitter emit, const Type type, const Func func) { GWDEBUG_EXE
  const Instr push_i = emitter_add_instr(emit, Reg_Push_Imm);
  const Instr func_i = emitter_add_instr(emit, Reg_Push_Ptr);
  push_i->m_val = SZ_INT;
  *(Type*)push_i->ptr = type;
  *(Func*)func_i->ptr = func;
  return 1;
}

ANN static m_bool emit_member_func(const Emitter emit, const Exp_Dot* member, const Func func) { GWDEBUG_EXE
  if(GET_FLAG(func, ae_flag_member)) {
    if(emit_exp(emit, member->base, 0) < 0)
      CHECK_BB(err_msg(EMIT_, member->self->pos, "... in member function")) // LCOV_EXCL_LINE
    emitter_add_instr(emit, Reg_Dup_Last);
    const Instr func_i = emitter_add_instr(emit, Exp_Dot_Func);
    func_i->m_val = func->vt_index;
  }
  return 1;
}

ANN static m_bool emit_member(const Emitter emit, const Value v, const m_bool emit_addr) { GWDEBUG_EXE
  const Instr func_i = emitter_add_instr(emit, Exp_Dot_Data);
  func_i->m_val = v->offset;
  func_i->m_val2 = v->type->size;
  *(m_uint*)func_i->ptr = emit_addr;
  return 1;
}

ANN static m_bool emit_exp_dot_instance(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  const Type t_base = member->t_base;
  const Value value = find_value(t_base, member->xid);
  const m_bool emit_addr = member->self->emit_var;
  if(isa(member->self->type, t_func_ptr) > 0) { // function pointer
    if(GET_FLAG(value, ae_flag_member)) { // member
      if(emit_exp(emit, member->base, 0) < 0)
        CHECK_BB(err_msg(EMIT_, member->self->pos, "... in member function")) // LCOV_EXCL_LINE
      emitter_add_instr(emit, Reg_Dup_Last);
      return emit_member(emit, value, emit_addr);
    } else
      return emit_dot_static_data(emit, value, emit_addr);
  } else if(isa(member->self->type, t_function) > 0) { // function
    const Func func = value->d.func_ref;
    if(GET_FLAG(func, ae_flag_member))
      return emit_member_func(emit, member, func);
    else
      return emit_dot_static_func(emit, t_base, func);
  } else { // variable
    if(GET_FLAG(value, ae_flag_member)) { // member
      CHECK_BB(emit_exp(emit, member->base, 0))
      return emit_member(emit, value, emit_addr);
    } else // static
      CHECK_BB(emit_dot_static_import_data(emit, value, emit_addr))
  }
  return 1;
}

ANN static m_bool emit_exp_dot_static(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  const Type t_base = member->t_base->d.base_type;
  const Value value = find_value(t_base, member->xid);

  if(isa(member->self->type, t_func_ptr) > 0)
    return emit_dot_static_import_data(emit, value, member->self->emit_var);
  if(isa(member->self->type, t_function) > 0)
    return emit_dot_static_func(emit, t_base, value->d.func_ref);
  return emit_dot_static_import_data(emit, value, member->self->emit_var);
}

ANN static m_bool emit_exp_dot(const Emitter emit, const Exp_Dot* member) { GWDEBUG_EXE
  if(is_special(member->t_base) > 0)
    return emit_exp_dot_special(emit, member);
  if(isa(member->t_base, t_class) > 0)
    return emit_exp_dot_static(emit, member);
  return emit_exp_dot_instance(emit, member);
}

ANN static void emit_func_def_global(const Emitter emit, const Value value) { GWDEBUG_EXE
  const Instr set_mem = emitter_add_instr(emit, Mem_Set_Imm);
  set_mem->m_val = value->offset = emit_alloc_local(emit, value->type->size, 0);
  *(Func*)set_mem->ptr = value->d.func_ref;
}

ANN static void emit_func_def_init(const Emitter emit, const Func func) { GWDEBUG_EXE
  const Type t = emit->env->class_def;
  char c[(t ? strlen(t->name) + 1 : 0) + strlen(func->name) + 6];
  emit->env->func = func;
  emit_push_code(emit);
//  emit->code = new_code();
  sprintf(c, "%s%s%s(...)", t ? t->name : "", t ? "." : "", func->name);
  emit->code->name = code_name_set(c, emit->filename);
  if(GET_FLAG(func, ae_flag_member))
    SET_FLAG(emit->code, ae_flag_member);
}

ANN static void emit_func_def_args(const Emitter emit, Arg_List a) { GWDEBUG_EXE
  do {
    const Value  value = a->var_decl->value;
    const m_uint size = value->type->size;
    const m_bool obj = isa(value->type, t_object) > 0 ? 1 : 0;
    emit->code->stack_depth += size;
    const m_uint offset = emit_alloc_local(emit, size, obj);
    value->offset = offset;
  } while((a = a->next));
}

ANN static void emit_func_def_ensure(const Emitter emit, const m_uint size) { GWDEBUG_EXE
  if(size) {
    const Instr instr = emitter_add_instr(emit, Reg_Push_Imm);
    instr->m_val = size;
  }
  vector_add(&emit->code->stack_return, (vtype)emitter_add_instr(emit, Goto));
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
  emitter_add_instr(emit, Func_Return);
}

ANN static void emit_func_def_code(const Emitter emit, const Func func) { GWDEBUG_EXE
  func->code = emit_code(emit);
  if(GET_FLAG(func->def, ae_flag_dtor)) {
    emit->env->class_def->nspc->dtor = func->code;
    ADD_REF(func->code)
  } else if(GET_FLAG(func->def, ae_flag_op)) {
    const Arg_List a = func->def->arg_list;
    const m_bool is_unary = GET_FLAG(func->def, ae_flag_unary);
    const Type l = is_unary ? NULL : a->type;
    const Type r = is_unary ? a->type : a->next ? a->next->type : NULL;
    struct Op_Import opi = { name2op(s_name(func->def->name)), l, r, NULL,
      NULL, NULL, 0 };
    opi.data = (m_uint)func;
    operator_set_func(&opi);
  }
}

ANN static m_bool emit_func_def_body(const Emitter emit, const Func_Def func_def) { GWDEBUG_EXE
  if(func_def->arg_list)
    emit_func_def_args(emit, func_def->arg_list);
  if(GET_FLAG(func_def, ae_flag_variadic)) {
    emit_alloc_local(emit, SZ_INT, 0);
    emit->code->stack_depth += SZ_INT;
  }
  if(func_def->d.code->d.stmt_code.stmt_list)
    CHECK_BB(emit_stmt_code(emit, &func_def->d.code->d.stmt_code))
if(GET_FLAG(func_def->func, ae_flag_ref)) {
  Type t = type_decl_resolve(emit->env, func_def->td);
  emit_func_def_ensure(emit, t->size);
} else
  emit_func_def_ensure(emit, func_def->ret_type->size);
  return 1;
}

ANN static m_bool emit_func_def(const Emitter emit, const Func_Def func_def) { GWDEBUG_EXE
  const Func func = get_func(emit->env, func_def);
  if(func->code)return 1;
  if(tmpl_list_base(func_def->tmpl)) { // don't check template definition
    func_def->flag &= ~ae_flag_template;
    return 1;
  }
  if(!emit->env->class_def)
    emit_func_def_global(emit, func->value_ref);
  else if(GET_FLAG(emit->env->class_def, ae_flag_builtin) && GET_FLAG(emit->env->class_def, ae_flag_template))
    return 1;
  emit_func_def_init(emit, func);
  if(GET_FLAG(func, ae_flag_member)) {
    emit->code->stack_depth += SZ_INT;
    emit_alloc_local(emit, SZ_INT, 0);
  }
  emit_push_scope(emit);
  CHECK_BB(emit_func_def_body(emit, func_def))
  if(GET_FLAG(func_def, ae_flag_variadic) && (!emit->env->func->variadic ||
      !*(m_uint*)emit->env->func->variadic->ptr))
    CHECK_BB(err_msg(EMIT_, func_def->td->pos, "invalid variadic use"))
  emit_func_def_return(emit);
//  emit_pop_scope(emit);
  emit_func_def_code(emit, func);
  emit->env->func = NULL;
  emit->code = emit_pop_code(emit);
  return 1;
}

ANN static m_bool emit_section(const Emitter emit, const Section* section) { GWDEBUG_EXE
  const ae_Section_Type t = section->section_type;
  if(t == ae_section_stmt)
    CHECK_BB(emit_stmt_list(emit, section->d.stmt_list))
  else if(t == ae_section_func)
    CHECK_BB(emit_func_def(emit, section->d.func_def))
  else if(t == ae_section_class)
    CHECK_BB(emit_class_def(emit, section->d.class_def))
  return 1;
}

ANN Code* emit_class_code(const Emitter emit, const m_str name) { GWDEBUG_EXE
  const m_uint len = strlen(name) + 7;
  char c[len];
  emit_push_code(emit);
  Code* code = emit->code;
  snprintf(c, len, "class %s", name);
  code->name = code_name_set(c, emit->filename);
  SET_FLAG(code, ae_flag_member);
  code->stack_depth += SZ_INT;
  return emit->code;
}

ANN void emit_class_finish(const Emitter emit, const Nspc nspc) { GWDEBUG_EXE
  emitter_add_instr(emit, Func_Return);
  nspc->pre_ctor = emit_code(emit);
}

ANN static void emit_class_push(const Emitter emit, const Type type) { GWDEBUG_EXE
  vector_add(&emit->env->class_stack, (vtype)emit->env->class_def);
  emit->env->class_def = type;
}

ANN static void emit_class_pop(const Emitter emit) { GWDEBUG_EXE
  emit->env->class_def = (Type)vector_pop(&emit->env->class_stack);
  emit->code = emit_pop_code(emit);
}

ANN static m_bool emit_class_def(const Emitter emit, const Class_Def class_def) { GWDEBUG_EXE
  const Type type = class_def->type;
  const Nspc nspc = type->nspc;
  if(tmpl_class_base(class_def->tmpl))
    return 1;
  if(class_def->ext && ((/*!GET_FLAG(type->parent, ae_flag_emit) &&*/
      GET_FLAG(class_def->ext, ae_flag_typedef)) || class_def->ext->types)) {
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
  emit_alloc_local(emit, SZ_INT, 1);
  if(class_def->body) {
    Class_Body body = class_def->body;
    do CHECK_BB(emit_section(emit, body->section))
    while((body = body->next));
  }
  emit_class_finish(emit, nspc);
  emit_class_pop(emit);
  SET_FLAG(type, ae_flag_emit);
  return 1;
}

ANN static void emit_free_code(Code* code) {
  LOOP_OPTIM
  for(m_uint j = 0; j < vector_size(&code->instr); ++j)
    free_instr((Instr)vector_at(&code->instr, j));
  free_code(code);
}

ANN static void emit_free_stack(const Emitter emit) { GWDEBUG_EXE
  LOOP_OPTIM
  for(m_uint i = 0;  i < vector_size(&emit->stack); ++i)
    emit_free_code((Code*)vector_at(&emit->stack, i));
  emit_free_code(emit->code);
}

ANN static m_bool emit_ast_inner(const Emitter emit, Ast ast) { GWDEBUG_EXE
  do CHECK_BB(emit_section(emit, ast->section))
  while((ast = ast->next));
  return 1;
}

ANN static void handle_code(const VM_Code c) {
  LOOP_OPTIM
  for(m_uint i = 0; i < vector_size(c->instr); ++i) {
    const Instr instr = (Instr)vector_at(c->instr, i);
    if(instr->execute == INSTR_RECURS) {
      instr->execute = Reg_Push_Ptr;
      const Func func = (Func)instr->m_val;
      *(VM_Code*)instr->ptr = func->code;
    }
  }
}

ANN static void code_optim(const Emitter emit) {
  for(m_uint i = vector_size(&emit->codes) + 1; --i;) {
    const VM_Code c = (VM_Code)vector_at(&emit->codes, i - 1);
    if(GET_FLAG(c, ae_flag_recurs))
      handle_code(c);
  }
  vector_clear(&emit->codes);
}

ANN m_bool emit_ast(const Emitter emit, Ast ast, const m_str filename) { GWDEBUG_EXE
#ifdef GWCOV
  if(emit->coverage) {
    char cov_filename[strlen(filename) + 3];
    sprintf(cov_filename, "%sda", filename);
    emit->cov_file = fopen(cov_filename, "a");
  }
#endif
  emit->filename = filename;
  emit->code = new_code();
  vector_clear(&emit->stack);
  emit_push_scope(emit);
  const m_bool ret = emit_ast_inner(emit, ast);
  emit_pop_scope(emit);
  if(ret > 0)
    code_optim(emit);
  else { // should free all stack.
    gw_err("in file '%s'\n", filename);
    emit_free_stack(emit);
    free(filename);
    free_ast(ast);
  }
  if(emit->cases) {
    free_map(emit->cases);
    emit->cases = NULL;
  }
#ifdef GWCOV
  if(emit->coverage)
    fclose(emit->cov_file);
#endif
  return ret;
}
