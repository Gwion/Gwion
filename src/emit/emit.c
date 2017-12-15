#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "instr.h"
#include "context.h"
#include "func.h"
#include "traverse.h"

#ifdef GWCOV
#define COVERAGE(a) if(emit->coverage)coverage(emit, a->pos);
#else
#define COVERAGE(a)
#endif

#ifdef GWCOV
static void coverage(Emitter emit, m_uint pos) {
  Instr cov;

  fprintf(emit->cov_file, "%" INT_F " ini\n", pos);
  cov = emitter_add_instr(emit, InstrCoverage);
  cov->m_val = pos;
}
#endif

typedef struct {
  m_uint size;
  m_uint offset;
  m_bool is_ref;
  m_bool is_obj;
} Local;

static Frame* new_frame() {
  Frame* frame = calloc(1, sizeof(Frame));
  vector_init(&frame->stack);
  return frame;
}

static void free_frame(Frame* a) {
  vtype i;
  for(i = vector_size(&a->stack) + 1; --i;)
    free((Local*)vector_at(&a->stack, i - 1));
  vector_release(&a->stack);
  free(a);
}

static Local* frame_alloc_local(Frame* frame, m_uint size, m_uint flag) {
  Local* local = calloc(1, sizeof(Local));
  local->size = size;
  local->offset = frame->curr_offset;
  local->is_ref = (flag & 1 << 1) == 1 << 1;
  local->is_obj = (flag & 1 << 2) == 1 << 2;
  frame->curr_offset += local->size;
  vector_add(&frame->stack, (vtype)local);
  return local;
}

static void frame_push_scope(Frame* frame) {
  vector_add(&frame->stack, (vtype)NULL);
}

static void frame_pop_scope(Frame* frame, Vector v) {
  m_uint i;
  while((i = vector_size(&frame->stack) && vector_back(&frame->stack))) {
    Local* local = (Local*)vector_pop(&frame->stack);
    frame->curr_offset -= local->size;
    vector_add(v, (vtype)local);
  }
  vector_pop(&frame->stack);
}

static m_bool emit_exp(Emitter emit, Exp exp, m_bool add_ref);
static m_bool emit_stmt(Emitter emit, Stmt stmt, m_bool pop);
static m_bool emit_stmt_list(Emitter emit, Stmt_List list);
static m_bool emit_exp_dot(Emitter emit, Exp_Dot* member);
static m_bool emit_func_def(Emitter emit, Func_Def func_def);

Emitter new_emitter(Env env) {
  Emitter emit = calloc(1, sizeof(struct Emitter_));
  vector_init(&emit->stack);
  emit->env = env;
  return emit;
}

void free_emitter(Emitter a) {
  free_env(a->env);
  vector_release(&a->stack);
  free(a);
}

Instr emitter_add_instr(Emitter emit, f_instr f) {
  Instr instr = calloc(1, sizeof(struct Instr_));
  instr->execute = f;
  vector_add(&emit->code->code, (vtype)instr);
  return instr;
}

static Code* new_code() {
  Code* code = calloc(1, sizeof(Code));
  code->stack_depth = 0;
  code->need_this = 0;
  vector_init(&code->code);
  vector_init(&code->stack_break);
  vector_init(&code->stack_cont);
  vector_init(&code->stack_return);
  code->frame = new_frame();
  return code;
}

static void free_code(Code* code) {
  vector_release(&code->code);
  vector_release(&code->stack_break);
  vector_release(&code->stack_cont);
  vector_release(&code->stack_return);
  free_frame(code->frame);
  free(code->name);
  free(code->filename);
  free(code);
}

static void emit_pop_scope(Emitter emit) {
  m_uint i;
  struct Vector_ v;
  vector_init(&v);
  frame_pop_scope(emit->code->frame, &v);
  for(i = 0; i < vector_size(&v); i++) {
    Local* l = (Local*)vector_at(&v, i);
    if(l->is_obj) {
      Instr instr = emitter_add_instr(emit, Release_Object2);
      instr->m_val = l->offset;
    }
    free(l);
  }
  vector_release(&v);
}

static void emit_push_scope(Emitter emit) {
  frame_push_scope(emit->code->frame);
}

static m_uint emit_code_size(Emitter emit) {
  return vector_size(&emit->code->code);
}

static m_uint emit_code_offset(Emitter emit) {
  return emit->code->frame->curr_offset;
}

static m_int emit_alloc_local(Emitter emit, m_uint size, m_uint flag) {
  Local* l = frame_alloc_local(emit->code->frame, size, flag);
  return l ? l->offset : -1;
}

static m_bool emit_pre_ctor(Emitter emit, Type type) {
  if(type->parent)
    emit_pre_ctor(emit, type->parent);
  if(type->info->pre_ctor) {
    Instr instr = emitter_add_instr(emit, Pre_Constructor);
    instr->m_val = (m_uint)type->info->pre_ctor;
    instr->m_val2 = (m_uint)emit_code_offset(emit);
  }
  return 1;
}

static m_bool emit_pre_constructor_array(Emitter emit, Type type) {
  Instr top, bottom;
  m_uint start_index = emit_code_size(emit);
  top = emitter_add_instr(emit, Instr_Pre_Ctor_Array_Top);
  *(Type*)top->ptr = type;
  emit_pre_ctor(emit, type);
  bottom = emitter_add_instr(emit, Instr_Pre_Ctor_Array_Bottom);
  top->m_val = emit_code_size(emit);
  bottom->m_val = start_index;
  CHECK_OB(emitter_add_instr(emit, Instr_Pre_Ctor_Array_Post))
  return 1;
}

static m_bool emit_instantiate_object(Emitter emit, Type type, Array_Sub array,
    m_bool is_ref) {
  if(type->array_depth) {
    CHECK_BB(emit_exp(emit, array->exp_list, 0))
    VM_Array_Info* info = calloc(1, sizeof(VM_Array_Info));
    info->depth = type->array_depth;
    info->type = type;
    info->is_obj = isa(type->d.array_type, &t_object) > 0 ? 1 : 0;
    info->is_ref = is_ref;
    Instr alloc = emitter_add_instr(emit, Instr_Array_Alloc);
    *(VM_Array_Info**)alloc->ptr = info;
    if(!is_ref && info->is_obj)
      emit_pre_constructor_array(emit, type->d.array_type);
  } else if(isa(type, &t_object) > 0 && !is_ref) {
    Instr instr = emitter_add_instr(emit, Instantiate_Object);
    *(Type*)instr->ptr = type;
    emit_pre_ctor(emit, type);
    if(strstr(type->name, "@")){ // handle templates
      char* name = strdup(type->name);
      char* c = strsep(&name, "@");
      Type parent = nspc_lookup_type0(emit->env->curr, insert_symbol(c));
      free(c);
      if(parent)
        emit_pre_ctor(emit, parent);
    }
  }
  return 1;
}

static void prepare_this_exp(Exp base, Exp dot) {
  memset(base, 0, sizeof(struct Exp_));
  memset(dot, 0, sizeof(struct Exp_));
  base->meta = ae_meta_var;
  base->exp_type = ae_exp_primary;
  base->d.exp_primary.type = ae_primary_id;
  base->d.exp_primary.d.var = insert_symbol("this");
  base->d.exp_primary.self = base;
  dot->exp_type = ae_exp_dot;
  dot->meta = ae_meta_var;
  dot->d.exp_dot.base = base;
  dot->d.exp_dot.self = dot;
}

static m_bool emit_symbol_owned(Emitter emit, Exp_Primary* prim) {
  Value v = prim->value;
  struct Exp_ base, dot;
  prepare_this_exp(&base, &dot);
  base.type = v->owner_class;
  dot.type = v->m_type;
  dot.d.exp_dot.t_base = v->owner_class;
  dot.d.exp_dot.xid = prim->d.var;
  dot.emit_var = prim->self->emit_var;
  if(emit_exp_dot(emit, &dot.d.exp_dot) < 0)
    CHECK_BB(err_msg(EMIT_, prim->pos,
          "(emit): internal error: symbol transformation failed..."))
  return 1;
}

static m_bool emit_symbol_const(Emitter emit, Exp_Primary* prim) {
  Value v = prim->value;
    Instr instr = emitter_add_instr(emit, Reg_PushImm);
  if(v->func_ref) {
    instr->m_val = SZ_INT;
    *(Func*)instr->ptr = v->func_ref;
  } else if(isa(v->m_type, &t_float) > 0 || isa(v->m_type, &t_time) > 0 ||
      isa(v->m_type, &t_dur) > 0) {
    instr->m_val = SZ_FLOAT;
    *(m_float*)instr->ptr = *(m_float*)v->ptr;
  } else {
    instr->m_val = SZ_INT;
    *(m_uint*)instr->ptr = (prim->self->emit_var ? (m_uint)&v->ptr : (m_uint)v->ptr);
  }
  return 1;
}

static m_bool emit_symbol_addr(Emitter emit, Value v) {
  Instr instr = emitter_add_instr(emit, Reg_Push_Mem_Addr);
  instr->m_val = v->offset;
  instr->m_val2 = GET_FLAG(v, ae_flag_global);
  return 1;
}

static m_bool emit_symbol_actual(Emitter emit, Value v) {
  Instr instr = emitter_add_instr(emit, Reg_Push_Mem);
  instr->m_val  = v->offset;
  instr->m_val2 = v->m_type->size;
  *(m_uint*)instr->ptr = GET_FLAG(v, ae_flag_global);
  return 1;
}

static m_bool emit_symbol(Emitter emit, Exp_Primary* prim) {
  Value v = prim->value;
  if(GET_FLAG(v, ae_flag_member) || GET_FLAG(v, ae_flag_static))
    return emit_symbol_owned(emit, prim);
  if(GET_FLAG(v, ae_flag_const) && GET_FLAG(v, ae_flag_builtin))
    return emit_symbol_const(emit, prim);
  if(prim->self->emit_var)
    return emit_symbol_addr(emit, v);
  return emit_symbol_actual(emit, v);
}

VM_Code emit_code(Emitter emit) {
  Code* c = emit->code;
  VM_Code code = new_vm_code(&c->code, c->stack_depth,
      c->need_this, c->name, c->filename);
  free_code(c);
  return code;
}

static m_bool emit_exp_prim_array(Emitter emit, Array_Sub array) {
  m_uint count = 0;
  CHECK_BB(emit_exp(emit, array->exp_list, 0))
  Exp e = array->exp_list;
  while(e) {
    count++;
    e = e->next;
  }
  Type type = array->type;
  Instr instr = emitter_add_instr(emit, Instr_Array_Init);
  VM_Array_Info* info = calloc(1, sizeof(VM_Array_Info));
  info->type = type;
  info->length = count;
  *(VM_Array_Info**)instr->ptr = info;
  instr->m_val2 = type->d.array_type->size;
  return 1;
}

static m_bool emit_exp_array(Emitter emit, Exp_Array* array) {
  m_uint is_var = array->self->emit_var;
  m_uint depth = array->base->type->array_depth - array->self->type->array_depth;

  CHECK_BB(emit_exp(emit, array->base, 0))
  CHECK_BB(emit_exp(emit, array->indices->exp_list, 0))
  if(depth == 1) {
    Instr instr = emitter_add_instr(emit, Instr_Array_Access);
    instr->m_val = is_var;
    instr->m_val2 = is_var ? SZ_INT : array->self->type->size;
  } else {
    Instr instr = emitter_add_instr(emit, Instr_Array_Access_Multi);
    instr->m_val = depth;
    instr->m_val2 = (is_var || array->self->type->array_depth) ?
      SZ_INT : array->base->type->d.array_type->size;
    *(m_uint*)instr->ptr = is_var || array->self->type->array_depth;
  }
  return 1;
}

static m_bool emit_exp_prim_vec(Emitter emit, Vec* vec) {
  CHECK_BB(emit_exp(emit, vec->args, 0));
  m_int n = 3 - vec->numdims;
  while(n > 0) {
    Instr push = emitter_add_instr(emit, Reg_PushImm);
    CHECK_OB(push);
    push->m_val = SZ_FLOAT;
    n--;
  }
  return 1;
}

static m_bool emit_exp_prim_id(Emitter emit, Exp_Primary* prim) {
  if(prim->d.var == insert_symbol("this"))
    CHECK_OB(emitter_add_instr(emit, Reg_Push_This))
  else if(prim->d.var == insert_symbol("me"))
    CHECK_OB(emitter_add_instr(emit, Reg_Push_Me))
  else if(prim->d.var == insert_symbol("now"))
    CHECK_OB(emitter_add_instr(emit, Reg_Push_Now))
  else if(prim->d.var == insert_symbol("false") ||
      prim->d.var == insert_symbol("null") ||
      prim->d.var == insert_symbol("NULL")) {
    Instr push = emitter_add_instr(emit, Reg_PushImm);
    CHECK_OB(push)
    push->m_val = SZ_INT;
  }
  else if(prim->d.var == insert_symbol("true")) {
    Instr instr = emitter_add_instr(emit, Reg_PushImm);
    instr->m_val = SZ_INT;
    *(m_uint*)instr->ptr = 1;
  }  else if(prim->d.var == insert_symbol("maybe"))
    CHECK_OB(emitter_add_instr(emit, Reg_Push_Maybe))
  else
    emit_symbol(emit, prim);
  return 1;
}

static m_bool emit_exp_prim_num(Emitter emit, m_uint i) {
  Instr instr = emitter_add_instr(emit, Reg_PushImm);
  instr->m_val = SZ_INT;
  *(m_uint*)instr->ptr = i;
  return 1;
}

static m_bool emit_exp_prim_float(Emitter emit, m_float f) {
  Instr instr = emitter_add_instr(emit, Reg_PushImm);
  instr->m_val = SZ_FLOAT;
  *(m_float*)instr->ptr = f;
  return 1;
}

static m_bool emit_exp_prim_char(Emitter emit, Exp_Primary* prim) {
  Instr instr = emitter_add_instr(emit, Reg_PushImm);
  instr->m_val = SZ_INT;
  *(m_uint*)instr->ptr = str2char(prim->d.chr, prim->pos);
  return 1;
}

static m_bool emit_exp_prim_str(Emitter emit, m_str str) {
  Instr instr = emitter_add_instr(emit, Reg_Push_Str);
  instr->m_val = (m_uint)str;
  return 1;
}

static m_bool emit_exp_prim_gack(Emitter emit, Exp exp) {
  Instr instr;
  Vector types;
  Exp e;
  CHECK_BB(emit_exp(emit, exp, 0))
  types = new_vector();
  e = exp;
  while(e) {
    vector_add(types, (vtype)e->type);
    if(!(emit->env->func && GET_FLAG(emit->env->func, ae_flag_member) &&
        e->type == emit->env->class_def))
    ADD_REF(e->type);
    e = e->next;
  }
  instr = emitter_add_instr(emit, Gack);
  *(Vector*)instr->ptr = types;
  return 1;
}

static m_bool emit_exp_primary2(Emitter emit, Exp_Primary* prim) {
  switch(prim->type) {
    case ae_primary_str:
      return emit_exp_prim_str(emit, prim->d.str);
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

static m_bool emit_exp_primary1(Emitter emit, Exp_Primary* prim) {
  switch(prim->type) {
    case ae_primary_complex:
      return emit_exp(emit, prim->d.cmp->re, 0);
    case ae_primary_polar:
      return emit_exp(emit, prim->d.polar->mod, 0);
    case ae_primary_vec:
      return emit_exp_prim_vec(emit, prim->d.vec);
    default:
      return emit_exp_primary2(emit, prim);
  }
  return 1;
}

static m_bool emit_exp_primary(Emitter emit, Exp_Primary* prim) {
  switch(prim->type) {
    case ae_primary_id:
      return emit_exp_prim_id(emit, prim);
    case ae_primary_num:
      return emit_exp_prim_num(emit, prim->d.num);
    case ae_primary_float:
      return emit_exp_prim_float(emit, prim->d.fnum);
    case ae_primary_char:
      return emit_exp_prim_char(emit, prim);
    default:
      return emit_exp_primary1(emit, prim);
  }
  return 1;
}

static m_bool emit_dot_static_data(Emitter emit, Value v, m_bool emit_var) {
  Instr alloc, push = emitter_add_instr(emit, Reg_PushImm);
  push->m_val = SZ_INT;
  *(Type*)push->ptr = v->owner_class;
  alloc = emitter_add_instr(emit, Dot_Static_Data);
  alloc->m_val2 = emit_var ? SZ_INT : v->m_type->size;
  *(m_uint*)alloc->ptr = emit_var;
  alloc->m_val = v->offset;
  return 1;
}

static m_bool decl_static(Emitter emit, Var_Decl var_decl, m_bool is_ref) {
  Value v = var_decl->value;
  Code* code = emit->code;

  emit->code = (Code*)vector_back(&emit->stack);
  CHECK_BB(emit_instantiate_object(emit, v->m_type, var_decl->array, is_ref))
  CHECK_BB(emit_dot_static_data(emit, v, 1))
  CHECK_OB(emitter_add_instr(emit, Assign_Object))
  emit->code = code;
  return 1;
}

static m_bool emit_exp_decl_static(Emitter emit, Var_Decl var_decl, m_bool is_ref) {
  Value value = var_decl->value;

  if(isprim(value->m_type) < 0 && !is_ref)
    CHECK_BB(decl_static(emit, var_decl, 0))
  CHECK_BB(emit_dot_static_data(emit, value, 1))
  return 1;
}

static Instr emit_exp_decl_global(Emitter emit, Value v, m_uint flag) {
  m_int offset= emit_alloc_local(emit, v->m_type->size, flag);
  CHECK_BO(offset)
  v->offset   = offset;
  return emitter_add_instr(emit, Alloc_Word);
}

static m_bool emit_exp_decl_non_static(Emitter emit, Var_Decl var_decl,
  m_bool is_ref, m_bool emit_var) {
  Value value = var_decl->value;
  Type type = value->m_type;
  Instr alloc;
  Array_Sub array = var_decl->array;
  m_bool is_array = array && array->exp_list;
  m_bool is_obj = isa(type, &t_object) > 0 || var_decl->array;
  if(is_obj && (is_array || !is_ref))
    CHECK_BB(emit_instantiate_object(emit, type, array, is_ref))
  if(GET_FLAG(value, ae_flag_member))
    alloc = emitter_add_instr(emit, Alloc_Member);
  else
    alloc = emit_exp_decl_global(emit, value, (is_ref ? 1 << 1 : 0) |
        (is_obj ? 1 << 2 : 0));
  alloc->m_val2 = type->size;
  alloc->m_val = value->offset;
  *(m_uint*)alloc->ptr = ((is_ref && !array) || isprim(type) > 0)  ? emit_var : 1;
  if(is_obj) {
    if((is_array) || !is_ref) {
      Instr assign = emitter_add_instr(emit, Assign_Object);
      assign->m_val = emit_var;
    }
    if(is_array)
      ADD_REF(type);
  }
  return 1;
}
static m_bool emit_class_def(Emitter emit, Class_Def class_Def);

static m_bool emit_exp_decl_template(Emitter emit, Exp_Decl* decl) {
  CHECK_BB(template_push_types(emit->env, decl->base->types, decl->types))
  CHECK_BB(emit_class_def(emit, decl->m_type->def))
  nspc_pop_type(emit->env->curr);
  return 1;
}

static m_bool emit_exp_decl(Emitter emit, Exp_Decl* decl) {
  Var_Decl_List list = decl->list;
  m_bool ref = GET_FLAG(decl->type, ae_flag_ref);
  m_bool var = decl->self->emit_var;

  if(GET_FLAG(decl->m_type, ae_flag_template))
    CHECK_BB(emit_exp_decl_template(emit, decl))
  while(list) {
    if(GET_FLAG(decl->type, ae_flag_static))
      CHECK_BB(emit_exp_decl_static(emit, list->self, ref))
    else
      CHECK_BB(emit_exp_decl_non_static(emit, list->self, ref, var))
    list = list->next;
  }
  return 1;
}

static m_uint vararg_size(Exp_Func* exp_func, Vector kinds) {
  Exp e = exp_func->args;
  Arg_List l = exp_func->m_func->def->arg_list;
  m_uint size = 0;
  while(e) {
    if(!l) {
      size += e->type->size;
      if(e->type->size)
      vector_add(kinds, (vtype)kindof(e->type));
    } else
      l = l->next;
    e = e->next;
  }
  return size;
}

static m_bool emit_func_arg_vararg(Emitter emit, Exp_Func* exp_func) {
  Instr instr;
  Vector kinds = new_vector();

  instr = emitter_add_instr(emit, MkVararg);
  instr->m_val = vararg_size(exp_func, kinds);
  instr->m_val2 = (m_uint)kinds;
  return 1;
}

static m_bool emit_func_args(Emitter emit, Exp_Func* exp_func) {
  if(emit_exp(emit, exp_func->args, 1) < 0)
    CHECK_BB(err_msg(EMIT_, exp_func->pos,
          "(emit): internal error in emitting function call arguments..."))
  if(GET_FLAG(exp_func->m_func->def, ae_flag_variadic))
    CHECK_BB(emit_func_arg_vararg(emit, exp_func))
  return 1;
}

static m_bool emit_exp_call_helper(Emitter emit, Exp_Func* exp_func, m_bool spork) {
  if(exp_func->args && !spork && emit_func_args(emit, exp_func) < 0)
    CHECK_BB(err_msg(EMIT_, exp_func->pos,
          "internal error in evaluating function arguments..."))
  if(emit_exp(emit, exp_func->func, 0) < 0)
    CHECK_BB(err_msg(EMIT_, exp_func->pos,
          "internal error in evaluating function call..."))
  if(GET_FLAG(exp_func->m_func->def, ae_flag_variadic) && !exp_func->args) {
    // handle empty call to variadic functions
    CHECK_OB(emitter_add_instr(emit, MkVararg))
    Instr push = emitter_add_instr(emit, Reg_PushImm);
    push->m_val = SZ_INT;
  }
  return 1;
}

static m_bool emit_exp_call_template(Emitter emit, Exp_Func* exp_func, m_bool spork) {
  Func_Def def = exp_func->m_func->def;
  if(exp_func->m_func->value_ref->owner_class)
    CHECK_BB(env_push_class(emit->env, exp_func->m_func->value_ref->owner_class))
  SET_FLAG(def, ae_flag_template);
  CHECK_BB(template_push_types(emit->env, def->base, exp_func->types))
  CHECK_BB(traverse_func_def(emit->env, def))
  CHECK_BB(emit_exp_call_helper(emit, exp_func, spork))
  nspc_pop_type(emit->env->curr);
  if(exp_func->m_func->value_ref->owner_class)
    CHECK_BB(env_pop_class(emit->env))
  exp_func->m_func->flag &= ~ae_flag_checked;
  return 1;
}

static m_bool emit_exp_call(Emitter emit, Exp_Func* exp_func, m_bool spork) {
  if(exp_func->types)
    CHECK_BB(emit_exp_call_template(emit, exp_func, spork))
  else
    CHECK_BB(emit_exp_call_helper(emit, exp_func, spork))
  return emit_exp_call1(emit, exp_func->m_func, exp_func->ret_type, exp_func->pos);
}

static m_bool emit_exp_binary_ptr(Emitter emit, Exp rhs) {
  Value v = NULL;
  Instr instr = emitter_add_instr(emit, assign_func);
  if(rhs->exp_type == ae_exp_dot) {
    Type t = rhs->d.exp_dot.t_base;
    if(isa(t, &t_class) > 0)
      t = t->d.actual_type;
    v = find_value(t, rhs->d.exp_dot.xid);
    if(!GET_FLAG(v, ae_flag_member) &&
          GET_FLAG(v->m_type, ae_flag_builtin)) {
      instr->m_val = 3;
      *(Type*)instr->ptr = t;
    } else
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

static m_bool emit_binary_func(Emitter emit, Exp_Binary* binary) {
  Exp lhs = binary->lhs;
  Exp rhs = binary->rhs;
  CHECK_BB(emit_exp(emit, lhs, 1))
  if(GET_FLAG(rhs->type->d.func->def, ae_flag_variadic)) {
    Exp_Func exp;
    exp.args = lhs;
    exp.m_func = rhs->type->d.func;
    emit_func_arg_vararg(emit, &exp);
  }
  CHECK_BB(emit_exp(emit, rhs, 1))
  return emit_exp_call1(emit, binary->func, binary->func->value_ref->m_type, binary->pos);
}

static m_bool emit_array_append(Emitter emit, Type type) {
  Instr instr = emitter_add_instr(emit, Array_Append);
  instr->m_val = kindof(type);
  return 1;
}

static m_bool emit_exp_binary(Emitter emit, Exp_Binary* binary) {
  Exp lhs = binary->lhs;
  Exp rhs = binary->rhs;
  struct Op_Import opi = { binary->op, lhs->type, rhs->type, NULL, NULL, 0 };

  if(binary->op == op_chuck && isa(rhs->type, &t_function) > 0)
    return emit_binary_func(emit, binary);
  CHECK_BB(emit_exp(emit, lhs, 1))
  CHECK_BB(emit_exp(emit, rhs, 1))
  if(binary->op == op_at_chuck && isa(lhs->type, &t_function) > 0 && isa(rhs->type, &t_func_ptr) > 0)
    return emit_exp_binary_ptr(emit, rhs);
  if(binary->op == op_shift_left && (lhs->type->array_depth == rhs->type->array_depth + 1)
      && isa(lhs->type->d.array_type, rhs->type) > 0)
    return emit_array_append(emit, rhs->type);
  CHECK_OB(get_instr(emit, &opi))
  return 1;
}

static m_bool exp_exp_cast1(Emitter emit, Type to, Type from) {
  f_instr f = NULL;
  if(to->xid == from->xid || isa(from, to) > 0 ||
      (isa(from, &t_null) > 0 && isa(to, &t_object) > 0))
    return 1;
  else if(to->xid == te_int && from->xid == te_float)
    f = Cast_f2i;
  else if(to->xid == te_float && from->xid == te_int)
    f = Cast_i2f;
  CHECK_OB(emitter_add_instr(emit, f))
  return 1;
}

static m_bool exp_exp_cast(Emitter emit, Exp_Cast* cast) {
  Type to = cast->self->type;
  Type from = cast->exp->type;
  if(isa(to, &t_func_ptr) > 0 && isa(from, &t_function) > 0) {
    Instr push = emitter_add_instr(emit, Reg_PushImm);
    push->m_val = SZ_INT;
    *(Func*)push->ptr = cast->func;
    return 1;
  }
  CHECK_BB(emit_exp(emit, cast->exp, 0))
  return exp_exp_cast1(emit, to, from);
}

static m_bool emit_exp_post(Emitter emit, Exp_Postfix* post) {
  struct Op_Import opi = { post->op, NULL, NULL, NULL, 0 };
  CHECK_BB(emit_exp(emit, post->exp, 0))
  opi.lhs = post->exp->type;
  return get_instr(emit, &opi) ? 1 : -1;
}

static m_bool emit_exp_dur(Emitter emit, Exp_Dur* dur) {
  CHECK_BB(emit_exp(emit, dur->base, 0))
  if(isa(dur->base->type, &t_int) > 0)
    CHECK_OB(emitter_add_instr(emit, Cast_i2f))
  CHECK_BB(emit_exp(emit, dur->unit, 0))
  CHECK_OB(emitter_add_instr(emit, float_times))

  return 1;
}

static Func emit_get_func(Nspc nspc, Func f) {
  return isa(f->value_ref->m_type, &t_func_ptr) > 0 ||
         isa(f->value_ref->m_type, &t_class) > 0 ?
         nspc_lookup_func1(f->value_ref->owner, f->def->name) :
         nspc_lookup_func2(nspc, insert_symbol(f->name));
}

static m_bool emit_exp_call_code_template(Env env, Class_Def class_def) {
  CHECK_BB(template_push_types(env, class_def->tref, class_def->base))
  CHECK_BB(traverse_class_def(env, class_def))
  nspc_pop_type(env->curr);
  return 1;
}

static m_bool emit_exp_call1_code(Emitter emit, Func func) {
  if(!emit_get_func(emit->env->curr, func)) { //template with no list
    Instr code;

    if(func->value_ref->owner_class &&
        GET_FLAG(func->value_ref->owner_class, ae_flag_template))
      CHECK_BB(emit_exp_call_code_template(emit->env,
            func->value_ref->owner_class->def))
    else if(!GET_FLAG(func->def, ae_flag_template))
      CHECK_BB(err_msg(EMIT_, func->def->pos, "function not emitted yet"))
    if(emit_func_def(emit, func->def) < 0)
      CHECK_BB(err_msg(EMIT_, 0, "can't emit func.")) // LCOV_EXCL_LINE
    code = emitter_add_instr(emit, Reg_Push_Ptr);
    *(VM_Code*)code->ptr = func->code = func->def->d.func->code;
  } else {
    Instr code = emitter_add_instr(emit, Reg_Push_Code);
    code->m_val = func->value_ref->offset;
    code->m_val2 = func->value_ref->owner_class ? 1 : 0;
  }
  return 1;
}

static m_bool emit_exp_call1_offset(Emitter emit, m_bool is_member) {
  Instr offset = emitter_add_instr(emit, Reg_PushImm);
  offset->m_val = SZ_INT;
  *(m_uint*)offset->ptr = emit_code_offset(emit);
  return 1;
}

m_bool emit_exp_call1_builtin(Emitter emit, Func func) {
  Instr call;

  Type t = func->value_ref->m_type;
  if(isa(t, &t_class) > 0)
    t = t->d.actual_type;
  if(isa(t, &t_func_ptr) < 0)
  if(!func->code || !func->code->native_func)
    CHECK_BB(err_msg(EMIT_, func->def->pos,
          "missing native func. are you trying to spork?"))
  call = emitter_add_instr(emit, GET_FLAG(func, ae_flag_member) ?
    Instr_Exp_Func_Member : Instr_Exp_Func_Static);
  call->m_val = func->def->ret_type->size;
  return 1;
}

static m_bool emit_exp_call1_op(Emitter emit, Arg_List list) {
  Instr call    = emitter_add_instr(emit, Instr_Op_Call_Binary);
  call->m_val   = emit->code->stack_depth;
  call->m_val2  = (m_uint)list->type;
  *(Type*)call->ptr     = list->next->type;
  return 1;
}

static m_bool emit_exp_call1_usr(Emitter emit) {
  Instr call = emitter_add_instr(emit, Instr_Exp_Func);
  call->m_val = emit->code->stack_depth;
  return 1;
}

m_bool emit_exp_call1(Emitter emit, Func func, Type type, int pos) {
  m_bool is_member = GET_FLAG(func, ae_flag_member) ||
    (!GET_FLAG(func, ae_flag_member) && !GET_FLAG(func, ae_flag_builtin));
  if(!func->code) // function pointer or template
    CHECK_BB(emit_exp_call1_code(emit, func))
  else {
    Instr code = emitter_add_instr(emit, Reg_Push_Ptr);
    *(VM_Code*)code->ptr = func->code;
  }
  CHECK_BB(emit_exp_call1_offset(emit, is_member))
  if(GET_FLAG(func->def, ae_flag_builtin))
    return emit_exp_call1_builtin(emit, func);
  else if(!strcmp(s_name(func->def->name), "chuck"))
    return emit_exp_call1_op(emit, func->def->arg_list);
  return emit_exp_call1_usr(emit);
}

static m_bool emit_exp_spork_finish(Emitter emit, VM_Code code, Func f, m_uint arg_size, m_uint stack_depth) {
  Instr push_code, spork;

  push_code = emitter_add_instr(emit, Reg_PushImm);
  push_code->m_val = SZ_INT;
  *(VM_Code*)push_code->ptr = code;
  spork = emitter_add_instr(emit, Spork);
  spork->m_val = arg_size;
  spork->m_val2 = (m_uint)f;
  *(m_uint*)spork->ptr = stack_depth; // only for some sporked expressions
  return 1;
}

static m_uint emit_exp_spork_size(Emitter emit, Exp e) {
  m_uint size = 0;
  while(e) {
    size += e->cast_to ? e->cast_to->size : e->type->size;
    e = e->next;
  }
  return size;
}

static m_bool emit_exp_spork(Emitter emit, Exp_Func* exp) {
  Instr op;
  VM_Code code;
  m_uint size;

  CHECK_BB(emit_func_args(emit, exp))
  if(emit_exp(emit, exp->func, 0) < 0)
    CHECK_BB(err_msg(EMIT_, exp->pos, "(emit): internal error in evaluating function call...")) // LCOV_EXCL_LINE
  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code();
  CHECK_OB(emitter_add_instr(emit, start_gc))
  emit->code->need_this = GET_FLAG(exp->m_func, ae_flag_member);
  char c[11 + num_digit(exp->pos)];
  sprintf(c, "spork~exp:%i\n", exp->pos);
  emit->code->name = strdup(c);
  emit->code->filename = strdup(emit->filename);
  op = emitter_add_instr(emit, Mem_Push_Imm);
  CHECK_BB(emit_exp_call1(emit, exp->m_func, exp->ret_type, exp->pos))
  CHECK_OB(emitter_add_instr(emit, stop_gc))
  CHECK_OB(emitter_add_instr(emit, EOC))
  op->m_val = emit->code->stack_depth;

  code = emit_code(emit);
  exp->vm_code = code;
  emit->code = (Code*)vector_pop(&emit->stack);
  size = emit_exp_spork_size(emit, exp->args);
  CHECK_BB(emit_exp_spork_finish(emit, code, NULL, size, 0)) // last arg migth have to be 'emit_code_offset(emit)'
  return 1;
}

static m_bool emit_exp_spork1(Emitter emit, Stmt stmt) {
  Instr op;
  VM_Code code;
  ID_List list = new_id_list("sporked", stmt->pos);
  Func f = new_func("sporked", new_func_def(0, new_type_decl(list, 0, stmt->pos), "sporked", NULL, stmt, stmt->pos));

  if(emit->env->class_def)
    CHECK_OB(emitter_add_instr(emit, Reg_Push_This))
  Instr push = emitter_add_instr(emit, Reg_PushImm);
  push->m_val = SZ_INT;
  *(Func*)push->ptr = f;

  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code();
  if(emit->env->class_def) {
    SET_FLAG(f, ae_flag_member);
    emit->code->need_this = 1;
  }
  char c[12 + num_digit(stmt->pos)];
  sprintf(c, "spork~code:%i\n", stmt->pos);
  emit->code->name = strdup(c);
  emit->code->filename = strdup(emit->filename);
  op = emitter_add_instr(emit, Mem_Push_Imm);
  emit_push_scope(emit);

  CHECK_OB(emitter_add_instr(emit, start_gc))
  CHECK_BB(emit_stmt(emit, stmt, 0))
  CHECK_OB(emitter_add_instr(emit, stop_gc))
  emit_pop_scope(emit);
  CHECK_OB(emitter_add_instr(emit, EOC))
  op->m_val = emit->code->stack_depth;
  f->code = code = emit_code(emit);
  emit->code = (Code*)vector_pop(&emit->stack);
  CHECK_BB(emit_exp_spork_finish(emit, code, f, 0, emit->env->func ? emit->env->func->def->stack_depth : 0))
  return 1;
}

static m_bool emit_exp_unary(Emitter emit, Exp_Unary* unary) {
  struct Op_Import opi = { unary->op, NULL, NULL, NULL, 0 };
  if(unary->op != op_spork && emit_exp(emit, unary->exp, 0) < 0)
    return -1;
  switch(unary->op) {
    case op_spork:
      CHECK_BB((unary->code ? emit_exp_spork1(emit, unary->code) : emit_exp_spork(emit, &unary->exp->d.exp_func)))
      break;
    case op_new:
      CHECK_BB(emit_instantiate_object(emit, unary->self->type,
            unary->array, GET_FLAG(unary->type, ae_flag_ref)))
      CHECK_OB(emitter_add_instr(emit, add2gc))
      break;
    default:
      opi.rhs = unary->exp->type;
      return get_instr(emit, &opi) ? 1 : -1;
  }
  return 1;
}

static m_bool emit_implicit_cast(Emitter emit, Type from, Type to) {
  if(from->xid == te_int && to->xid == te_float)
    CHECK_OB(emitter_add_instr(emit, Cast_i2f))
  else if(from->xid == te_float && to->xid == te_int)
    CHECK_OB(emitter_add_instr(emit, Cast_f2i))
  return 1;
}

static Instr emit_flow(Emitter emit, Type type, f_instr f1, f_instr f2) {
  Instr push = emitter_add_instr(emit, Reg_PushImm);
  CHECK_OO(push)
  switch(type->xid) {
    case te_int:
      push->m_val = SZ_INT;
      return emitter_add_instr(emit, f1);
    case te_float:
    case te_dur:
    case te_time:
      push->m_val = SZ_FLOAT;
      return emitter_add_instr(emit, f2);
    default:
      break;
  }
  return NULL;
}

static m_bool emit_exp_if(Emitter emit, Exp_If* exp_if) {
  m_bool ret;
  Instr op = NULL, op2 = NULL;
  nspc_push_value(emit->env->curr);
  CHECK_BB(emit_exp(emit, exp_if->cond, 0))
  CHECK_OB((op = emit_flow(emit, exp_if->cond->type, Branch_Eq_Int, Branch_Eq_Float)))
  CHECK_BB((ret = emit_exp(emit, exp_if->if_exp, 0)))
  CHECK_OB((op2 = emitter_add_instr(emit, Goto)))
  op->m_val = emit_code_size(emit);
  ret = emit_exp(emit, exp_if->else_exp, 0);
  nspc_pop_value(emit->env->curr);
  CHECK_OB(ret)
  op2->m_val = emit_code_size(emit);
  return ret;
}

static m_bool emit_exp(Emitter emit, Exp exp, m_bool ref) {
  while(exp) {
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
    }
    if(exp->cast_to)
      CHECK_BB(emit_implicit_cast(emit, exp->type, exp->cast_to))
      if(ref && isa(exp->type, &t_object) > 0 && isa(exp->type, &t_void) < 0) {
        Instr ref = emitter_add_instr(emit, Reg_AddRef_Object3);
        ref->m_val = exp->emit_var;
      }
    exp = exp->next;
  }
  return 1;
}

static m_bool emit_stmt_if(Emitter emit, Stmt_If stmt) {
  Instr op = NULL, op2 = NULL;

  emit_push_scope(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  COVERAGE(stmt->cond)
  CHECK_OB((op = emit_flow(emit, isa(stmt->cond->type, &t_object) > 0 ? &t_int : stmt->cond->type,
                           Branch_Eq_Int, Branch_Eq_Float)))
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->if_body, 1))
  emit_pop_scope(emit);
  op2 = emitter_add_instr(emit, Goto);
  op->m_val = emit_code_size(emit);

  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->else_body, 1))
  emit_pop_scope(emit);

  op2->m_val = emit_code_size(emit);
  emit_pop_scope(emit);
  return 1;
}

static m_bool emit_stmt_code(Emitter emit, Stmt_Code stmt, m_bool push) {
  Stmt_List list = stmt->stmt_list;
  while(list) {
    CHECK_BB(emit_stmt(emit, list->stmt, 1))
    list = list->next;
  }
  return 1;
}

static void emit_func_release(Emitter emit) {
  m_uint i;
  Vector v = &emit->code->frame->stack;
  for(i = vector_size(v); i; i--) {
    Local* l = (Local*)vector_at(v, i - 1);
    if(!l)
      break;
    else if(l->is_obj) {
      Instr rel = emitter_add_instr(emit, Release_Object2);
      rel->m_val = l->offset;
    }
  }
}

static m_bool emit_stmt_return(Emitter emit, Stmt_Return stmt) {
  CHECK_BB(emit_exp(emit, stmt->val, 0))
  vector_add(&emit->code->stack_return, (vtype)emitter_add_instr(emit, Goto));
  return 1;
}

static m_bool emit_stmt_continue(Emitter emit, Stmt_Continue cont) {
  vector_add(&emit->code->stack_cont, (vtype)emitter_add_instr(emit, Goto));
  return 1;
}

static m_bool emit_stmt_break(Emitter emit, Stmt_Break cont) {
  vector_add(&emit->code->stack_break, (vtype)emitter_add_instr(emit, Goto));
  return 1;
}

static void emit_push_stack(Emitter emit) { // should be m_bool as vector_add should return m_bool
  vector_add(&emit->code->stack_cont, (vtype)NULL);
  vector_add(&emit->code->stack_break, (vtype)NULL);
}

static void emit_pop_stack(Emitter emit, m_uint index) { // should be m_bool as vector_add should return m_bool
  while(vector_size(&emit->code->stack_cont) && vector_back(&emit->code->stack_cont)) {
    Instr instr = (Instr)vector_pop(&emit->code->stack_cont);
    instr->m_val = index;
  }
  while(vector_size(&emit->code->stack_break) && vector_back(&emit->code->stack_break)) {
    Instr instr = (Instr)vector_pop(&emit->code->stack_break);
    instr->m_val = emit_code_size(emit);
  }
  emit_pop_scope(emit);
  vector_pop(&emit->code->stack_cont);
  vector_pop(&emit->code->stack_break);
}

static m_bool emit_stmt_while(Emitter emit, Stmt_While stmt) {
  m_bool ret = 1;
  m_uint index = emit_code_size(emit);
  Instr op, goto_;

  emit_push_scope(emit);
  emit_push_stack(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  COVERAGE(stmt->cond)
  CHECK_OB((op = emit_flow(emit, stmt->cond->type, Branch_Eq_Int, Branch_Eq_Float)))

  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1)) // was '0' , then 'stmt->body->type == ae_stmt_code ? 0 : 1'
  emit_pop_scope(emit);

  goto_ = emitter_add_instr(emit, Goto);
  goto_->m_val = index;
  op->m_val = emit_code_size(emit);
  emit_pop_stack(emit, index);
  return ret;
}

static m_bool emit_stmt_do_while(Emitter emit, Stmt_While stmt) {
  m_bool ret = 1;
  Instr op;
  m_uint index = emit_code_size(emit);

  emit_push_scope(emit);
  emit_push_stack(emit);
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  CHECK_OB((op = emit_flow(emit, stmt->cond->type,
                           Branch_Neq_Int, Branch_Neq_Float)))
  op->m_val = index;
  emit_pop_stack(emit, index);
  return ret;
}

static m_bool emit_stmt_until(Emitter emit, Stmt_Until stmt) {
  Instr op;
  m_uint index;

  emit_push_scope(emit);
  index = emit_code_size(emit);
  emit_push_stack(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  CHECK_OB((op = emit_flow(emit, stmt->cond->type,
                           Branch_Neq_Int, Branch_Neq_Float)))
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);

  Instr _goto = emitter_add_instr(emit, Goto);
  _goto->m_val = index;
  op->m_val = emit_code_size(emit);
  emit_pop_stack(emit, index);
  return 1;
}

static m_bool emit_stmt_do_until(Emitter emit, Stmt_Until stmt) {
  Instr op;
  m_uint index;

  emit_push_scope(emit);
  index = emit_code_size(emit);
  emit_push_stack(emit);
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  CHECK_OB((op = emit_flow(emit, stmt->cond->type,
                           Branch_Eq_Int, Branch_Eq_Float)))
  op->m_val = index;
  emit_pop_stack(emit, index);
  return 1;
}

static m_bool emit_stmt_for(Emitter emit, Stmt_For stmt) {
  Instr  op = NULL;
  m_uint index;

  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->c1, 1))
  index = emit_code_size(emit);
  emit_push_stack(emit);
  CHECK_BB(emit_stmt(emit, stmt->c2, 0))
  CHECK_OB((op = emit_flow(emit, stmt->c2->d.stmt_exp.val->type,
                           Branch_Eq_Int, Branch_Eq_Float)))
  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);
  m_uint action_index = emit_code_size(emit);
  if(stmt->c3) {
    CHECK_BB(emit_exp(emit, stmt->c3, 0))
    Exp e = stmt->c3;
    m_uint num_words = 0;
    while(e) {
      num_words += e->type->size;
      e = e->next;
    }
    if(num_words) {
      Instr pop = emitter_add_instr(emit, Reg_Pop_Word4);
      pop->m_val = num_words;
    }
  }
  Instr _goto = emitter_add_instr(emit, Goto);
  _goto->m_val = index;
  op->m_val = emit_code_size(emit);
  emit_pop_stack(emit, action_index);
  return 1;
}

static m_bool emit_stmt_loop(Emitter emit, Stmt_Loop stmt) {
  Instr init, op, deref, dec, _goto, push;
  m_int* counter;
  m_uint index;

  emit_push_scope(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  counter = calloc(1, sizeof(m_int));
  init = emitter_add_instr(emit, Init_Loop_Counter);
  init->m_val = (m_uint)counter;
  index = emit_code_size(emit);
  emit_push_stack(emit);
  deref = emitter_add_instr(emit, Reg_Push_Deref);
  deref->m_val = (m_uint)counter;

  CHECK_OB((push = emitter_add_instr(emit, Reg_PushImm)))
  push->m_val = SZ_INT;
  op = emitter_add_instr(emit, Branch_Eq_Int);
  dec = emitter_add_instr(emit, Dec_int_Addr);
  dec->m_val = (m_uint)counter;

  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);

  _goto = emitter_add_instr(emit, Goto);
  _goto->m_val = index;
  op->m_val = emit_code_size(emit);
  emit_pop_stack(emit, index);
  return 1;
}

static m_bool emit_stmt_gotolabel(Emitter emit, Stmt_Goto_Label stmt) {
  m_uint i, size;
  Stmt_Goto_Label label;
  if(!stmt->is_label)
    stmt->data.instr = emitter_add_instr(emit, Goto);
  else {
    if(emit->cases && !strcmp(s_name(stmt->name), "default")) {
      if(emit->default_case_index != -1)
        CHECK_BB(err_msg(EMIT_, stmt->pos, "default case already defined"))
        emit->default_case_index = emit_code_size(emit);
      return 1;
    }
    if(!stmt->data.v.ptr) {
      emit->cases = NULL; // check me (mem leak?)
      CHECK_BB(err_msg(EMIT_, stmt->pos, "illegal case"))
    }
    size = vector_size(&stmt->data.v);
    if(!size) {
      vector_release(&stmt->data.v);
      CHECK_BB(err_msg(EMIT_, stmt->pos, "label '%s' defined but not used.", s_name(stmt->name)))
    }
    for(i = size + 1; --i;) {
      label = (Stmt_Goto_Label)vector_at(&stmt->data.v, i - 1);
      label->data.instr->m_val = emit_code_size(emit);
    }
    vector_release(&stmt->data.v);
  }
  return 1;
}

static m_bool emit_stmt_switch(Emitter emit, Stmt_Switch stmt) {
  Instr instr, _break;
  vector_add(&emit->code->stack_break, (vtype)NULL);
  CHECK_BB(emit_exp(emit, stmt->val, 0))
  if(emit->cases)
    CHECK_BB(err_msg(EMIT_, stmt->pos, "swith inside an other switch. this is not allowed for now"))
    emit->default_case_index = -1;
  CHECK_OB(emitter_add_instr(emit, start_gc))
  instr = emitter_add_instr(emit, Branch_Switch);
  emit->cases = new_map();
  *(Map*)instr->ptr = emit->cases;

  emit_push_scope(emit);
  CHECK_BB(emit_stmt(emit, stmt->stmt, 1))
  emit_pop_scope(emit);
  instr->m_val = emit->default_case_index > -1 ? emit->default_case_index : emit_code_size(emit);
  emit->default_case_index = -1;
  while(vector_size(&emit->code->stack_break) && vector_back(&emit->code->stack_break)) {
    _break = (Instr)vector_pop(&emit->code->stack_break);
    _break->m_val = emit_code_size(emit);
  }
  vector_pop(&emit->code->stack_break);
  CHECK_OB(emitter_add_instr(emit, stop_gc))
  emit->cases = NULL;
  return 1;
}

static m_bool primary_case(Exp_Primary* prim, m_int* value) {
  if(prim->type == ae_primary_num)
    *value = prim->d.num;
  else if(prim->d.var == insert_symbol("true"))
    *value = 1;
  else if(prim->d.var == insert_symbol("false"))
    *value = 0;
  else if(prim->d.var == insert_symbol("maybe"))
    CHECK_BB(err_msg(EMIT_, prim->pos, "'maybe' is not constant."))
  else {
    if(!GET_FLAG(prim->value, ae_flag_const))
      CHECK_BB(err_msg(EMIT_, prim->pos,
            "value is not const. this is not allowed for now"))
    *value = (m_uint)prim->value->ptr; // assume enum.
  }
  return 1;
}

static m_int get_case_value(Stmt_Case stmt, m_int* value) {
  if(stmt->val->exp_type == ae_exp_primary)
    CHECK_BB(primary_case(&stmt->val->d.exp_primary, value))
  else {
    Type t = isa(stmt->val->d.exp_dot.t_base, &t_class) > 0 ?
        stmt->val->d.exp_dot.t_base->d.actual_type :
        stmt->val->d.exp_dot.t_base;
    Value v = find_value(t, stmt->val->d.exp_dot.xid);
    *value = GET_FLAG(v, ae_flag_enum) ? !GET_FLAG(v, ae_flag_builtin) ?
      t->info->class_data[v->offset] : (m_uint)v->ptr : *(m_uint*)v->ptr;
  }
  return 1;
}

static m_bool emit_stmt_case(Emitter emit, Stmt_Case stmt) {
  m_int value = 0;
  if(!emit->cases)
    CHECK_BB(err_msg(EMIT_, stmt->pos, "case found outside switch statement. this is not allowed for now"))
  CHECK_BB(get_case_value(stmt, &value))
  if(map_get(emit->cases, (vtype)value))
    CHECK_BB(err_msg(EMIT_, stmt->pos, "duplicated cases value %i", value))
  map_set(emit->cases, (vtype)value, (vtype)emit_code_size(emit));
  return 1;
}

static m_bool emit_stmt_fptr(Emitter emit, Stmt_Ptr ptr) {
  if(GET_FLAG(ptr, ae_flag_static))
    ADD_REF(ptr->func)
    return 1;
}

static m_bool emit_stmt_enum(Emitter emit, Stmt_Enum stmt) {
  m_uint i;
  for(i = 0; i < vector_size(&stmt->values); i++) {
    Value v = (Value)vector_at(&stmt->values, i);
    if(!emit->env->class_def) {
      m_int offset = emit_alloc_local(emit, sizeof(m_uint), 0);
      CHECK_BB(offset)
      v->offset = offset;
      v->ptr = (m_uint*)i;
    } else
      emit->env->class_def->info->class_data[v->offset] = i;
  }
  return 1;
}

static m_bool emit_stmt_union(Emitter emit, Stmt_Union stmt) {
  Decl_List l = stmt->l;

  if(stmt->xid) {
    if(!stmt->value->m_type->info->class_data)
      stmt->value->m_type->info->class_data =
        calloc(1, stmt->value->m_type->info->class_data_size);
    Type_Decl *type_decl = new_type_decl(new_id_list(s_name(stmt->xid), stmt->pos),
        0, emit->env->class_def ? ae_flag_member : 0);
    type_decl->flag = stmt->flag;
    Var_Decl var_decl = new_var_decl(s_name(stmt->xid), NULL, 0);
    Var_Decl_List var_decl_list = new_var_decl_list(var_decl, NULL, 0);
    Exp exp = new_exp_decl(type_decl, var_decl_list, 0);
    exp->d.exp_decl.m_type = stmt->value->m_type;
    var_decl->value = stmt->value;
    CHECK_BB(emit_exp_decl(emit, &exp->d.exp_decl))
    if(!emit->env->class_def)
      ADD_REF(stmt->value);
    free_exp(exp);
    env_push_class(emit->env, stmt->value->m_type);
  }
  else if(!GET_FLAG(l->self->d.exp_decl.list->self->value, ae_flag_member)) {
    m_int offset = emit_alloc_local(emit, stmt->s, 1 << 1);
    CHECK_BB(offset)
    stmt->o = offset;
  }

  while(l) {
    Var_Decl_List var_list = l->self->d.exp_decl.list;
    while(var_list) {
      var_list->self->value->offset = stmt->o;
      var_list = var_list->next;
    }
    l = l->next;
  }
  if(stmt->xid) {
    Instr instr = emitter_add_instr(emit, Reg_Pop_Word4);
    instr->m_val = SZ_INT;
    env_pop_class(emit->env);
  }
  return 1;
}

static m_bool emit_stmt_exp(Emitter emit, struct Stmt_Exp_* exp, m_bool pop) {
  int ret;
  if(!exp->val)
    return 1;
  ret = emit_exp(emit, exp->val, 0);
  if(ret > 0 && pop && exp->val->type && exp->val->type->size > 0) {
    Exp e = exp->val;
    if(e->exp_type == ae_exp_primary && e->d.exp_primary.type == ae_primary_hack)
      e = e->d.exp_primary.d.exp;
    while(e) {
      Instr instr = emitter_add_instr(emit, Reg_Pop_Word4);
      instr->m_val = (e->exp_type == ae_exp_decl ?
        e->d.exp_decl.num_decl * e->type->size : e->type->size);
      e = e->next;
    }
  }
 return ret;
}

static m_bool emit_stmt(Emitter emit, Stmt stmt, m_bool pop) {
  if(!stmt)
    return 1;
  if(stmt->type != ae_stmt_if || stmt->type != ae_stmt_while)
    COVERAGE(stmt)
  switch(stmt->type) {
    case ae_stmt_exp:
      return emit_stmt_exp(emit, &stmt->d.stmt_exp, pop);
    case ae_stmt_code:
      return emit_stmt_code(emit, &stmt->d.stmt_code, 1);
    case ae_stmt_if:
      return emit_stmt_if(emit, &stmt->d.stmt_if);
    case ae_stmt_return:
      return emit_stmt_return(emit, &stmt->d.stmt_return);
    case ae_stmt_break:
      return emit_stmt_break(emit, &stmt->d.stmt_break);
    case ae_stmt_continue:
      return emit_stmt_continue(emit, &stmt->d.stmt_continue);
    case ae_stmt_while:
      return stmt->d.stmt_while.is_do ? emit_stmt_do_while(emit, &stmt->d.stmt_while) :
            emit_stmt_while(emit, &stmt->d.stmt_while);
    case ae_stmt_until:
      return stmt->d.stmt_until.is_do ? emit_stmt_do_until(emit, &stmt->d.stmt_until) :
            emit_stmt_until(emit, &stmt->d.stmt_until);
    case ae_stmt_for:
      return emit_stmt_for(emit, &stmt->d.stmt_for);
    case ae_stmt_loop:
      return emit_stmt_loop(emit, &stmt->d.stmt_loop);
    case ae_stmt_gotolabel:
      return emit_stmt_gotolabel(emit, &stmt->d.stmt_gotolabel);
    case ae_stmt_case:
      return emit_stmt_case(emit, &stmt->d.stmt_case);
    case ae_stmt_enum:
      return emit_stmt_enum(emit, &stmt->d.stmt_enum);
    case ae_stmt_switch:
      return emit_stmt_switch(emit, &stmt->d.stmt_switch);
    case ae_stmt_funcptr:
      return emit_stmt_fptr(emit, &stmt->d.stmt_ptr);
    case ae_stmt_typedef:
      return 1;
    case ae_stmt_union:
      return emit_stmt_union(emit, &stmt->d.stmt_union);
  }
  return 1;
}

static m_bool emit_stmt_list(Emitter emit, Stmt_List list) {
  Stmt_List tmp = list;
  while(tmp) {
    CHECK_BB(emit_stmt(emit, tmp->stmt, 1))
    tmp = tmp->next;
  }
  return 1;
}

static m_bool is_special(Type t) {
  m_uint xid = t->xid;
  if(xid == te_complex || xid == te_polar ||
     xid == te_vec3    || xid == te_vec4  ||
     xid == te_vararg)
    return 1;
  return -1;
}

static m_bool emit_dot_static_import_data(Emitter emit, Value v, m_bool emit_addr) {
  Instr func_i;

  if(v->ptr && GET_FLAG(v, ae_flag_builtin)) { // from C
    if(GET_FLAG(v, ae_flag_enum)) {
      func_i = emitter_add_instr(emit, Reg_PushImm);
      *(m_uint*)func_i->ptr = (m_uint)v->ptr;
      func_i->m_val = SZ_INT;
    } else {
      func_i = emitter_add_instr(emit, Dot_Static_Import_Data);
      func_i->m_val = (m_uint)v->ptr;
      func_i->m_val2 = emit_addr ? SZ_INT : v->m_type->size;
      *(m_uint*)func_i->ptr = emit_addr;
    }
  } else { // from code
    Instr push_i = emitter_add_instr(emit, Reg_PushImm);
    func_i = emitter_add_instr(emit, Dot_Static_Data);
    push_i->m_val = SZ_INT;
    *(Type*)push_i->ptr = v->owner_class;
    func_i->m_val = (m_uint)v->offset;
    func_i->m_val2 = emit_addr ? SZ_INT : v->m_type->size;
    *(m_uint*)func_i->ptr = emit_addr;
  }
  return 1;
}

static m_bool emit_complex_member(Emitter emit, Exp_Dot* member) {
  Instr instr;
  Exp base = member->base;

  base->emit_var = 1;
  CHECK_BB(emit_exp(emit, base, 0))
  if(!strcmp((isa(base->type, &t_complex) > 0  ? "re" : "phase") ,
        s_name(member->xid)))
    instr = emitter_add_instr(emit, complex_real);
  else
    instr = emitter_add_instr(emit, complex_imag);
  instr->m_val = member->self->emit_var;
  return 1;
}

static m_bool emit_vec_func(Emitter emit, Value v) {
  Instr instr;
  CHECK_OB(emitter_add_instr(emit, Reg_Dup_Last))
  instr = emitter_add_instr(emit, member_function);
  *(Vector*)instr->ptr = &v->owner_class->info->obj_v_table;
  instr->m_val = v->func_ref->vt_index;
  return 1;
}

static m_bool emit_vec_member(Emitter emit, Exp_Dot* member) {
  Instr instr;
  Value v;

  member->base->emit_var = 1;
  CHECK_BB(emit_exp(emit, member->base, 0))
  v = find_value(member->base->type, member->xid);
  if(v->func_ref)
    return emit_vec_func(emit, v);
  instr = emitter_add_instr(emit, vec_member);
  instr->m_val2 = v->offset;
  instr->m_val = member->self->emit_var;
  return 1;
}

static m_bool emit_vararg_start(Emitter emit , m_uint offset) {
  if(emit->env->func->variadic)
    CHECK_BB(err_msg(EMIT_, 0, "vararg.start already used. this is an error"))
  emit->env->func->variadic = emitter_add_instr(emit, Vararg_start);
  emit->env->func->variadic->m_val = offset;
  emit->env->func->variadic->m_val2 = emit_code_size(emit);
  return 1;
}

static m_bool emit_vararg_end(Emitter emit, m_uint offset) {
  if(!emit->env->func->variadic)
    CHECK_BB(err_msg(EMIT_, 0, "vararg.start not used before vararg.end. this is an error"))
  Instr instr = emitter_add_instr(emit, Vararg_end);
  instr->m_val = offset;
  instr->m_val2 = emit->env->func->variadic->m_val2;
  emit->env->func->variadic->m_val2 = emit_code_size(emit);
  *(m_uint*)emit->env->func->variadic->ptr = 1;
  return 1;
}

static m_bool emit_vararg(Emitter emit, Exp_Dot* member) {
  Instr instr;
  m_uint offset = 0;
  Arg_List l = emit->env->func->def->arg_list;
  m_str str = s_name(member->xid);
  while(l) {
    offset += l->type->size;
    l = l->next;
  }
  if(!strcmp(str, "start"))
    return emit_vararg_start(emit, offset);
  if(!strcmp(str, "end"))
    return emit_vararg_end(emit, offset);
  instr = emitter_add_instr(emit, Vararg_Member);
  instr->m_val = offset;
  instr->m_val2 = member->self->type->size;
  return 1;
}

static m_bool emit_exp_dot_special(Emitter emit, Exp_Dot* member) {
  Type t = member->t_base;

  if(t->xid == te_complex || t->xid == te_polar)
    return emit_complex_member(emit, member);
  else if(t->xid == te_vec3 || t->xid == te_vec4)
    return emit_vec_member(emit, member);
  return emit_vararg(emit, member);
}

static m_bool emit_dot_static_func(Emitter emit, Type type, Func func) {
  Instr push_i = emitter_add_instr(emit, Reg_PushImm);
  Instr func_i = emitter_add_instr(emit, Dot_Static_Func);
  push_i->m_val = SZ_INT;
  *(Type*)push_i->ptr = type;
  func_i->m_val = (m_uint)func;
  return 1;
}

static m_bool emit_member_func(Emitter emit, Exp_Dot* member, Func func) {
  if(GET_FLAG(func, ae_flag_member)) { // member
    Instr func_i;
    if(emit_exp(emit, member->base, 0) < 0)
      CHECK_BB(err_msg(EMIT_, member->pos, "... in member function")) // LCOV_EXCL_LINE
    CHECK_OB(emitter_add_instr(emit, Reg_Dup_Last))
    func_i = emitter_add_instr(emit, Exp_Dot_Func);
    func_i->m_val = func->vt_index;
  }
  return 1;
}

static m_bool emit_member(Emitter emit, Value v, m_bool emit_addr) {
  Instr func_i = emitter_add_instr(emit, Exp_Dot_Data);
  func_i->m_val = v->offset;
  func_i->m_val2 = v->m_type->size;
  *(m_uint*)func_i->ptr = emit_addr;
  return 1;
}

static m_bool emit_exp_dot_instance(Emitter emit, Exp_Dot* member) {
  Type t_base = member->t_base;
  Value value = find_value(t_base, member->xid);
  m_bool emit_addr = member->self->emit_var;
  if(isa(member->self->type, &t_func_ptr) > 0) { // function pointer
    if(GET_FLAG(value, ae_flag_member)) { // member
      if(emit_exp(emit, member->base, 0) < 0)
        CHECK_BB(err_msg(EMIT_, member->pos, "... in member function")) // LCOV_EXCL_LINE
      CHECK_OB(emitter_add_instr(emit, Reg_Dup_Last))
      return emit_member(emit, value, emit_addr);
    } else
      return emit_dot_static_data(emit, value, emit_addr);
  } else if(isa(member->self->type, &t_function) > 0) { // function
    Func func = value->func_ref;
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

static m_bool emit_exp_dot_static(Emitter emit, Exp_Dot* member) {
  Type t_base = member->t_base->d.actual_type;
  Value value = find_value(t_base, member->xid);

  if(isa(member->self->type, &t_func_ptr) > 0)
    return emit_dot_static_import_data(emit, value, member->self->emit_var);
  if(isa(member->self->type, &t_function) > 0)
    return emit_dot_static_func(emit, t_base, value->func_ref);
  return emit_dot_static_import_data(emit, value, member->self->emit_var);
}

static m_bool emit_exp_dot(Emitter emit, Exp_Dot* member) {
  if(is_special(member->t_base) > 0)
    return emit_exp_dot_special(emit, member);
  if(member->t_base->xid != te_class)
    return emit_exp_dot_instance(emit, member);
  return emit_exp_dot_static(emit, member);
}

static m_bool emit_func_def_global(Emitter emit, Value value) {
  m_int offset = emit_alloc_local(emit, value->m_type->size, 1 << 1);
  Instr set_mem = emitter_add_instr(emit, Mem_Set_Imm);
  CHECK_BB(offset)
  set_mem->m_val = value->offset = offset;
  *(Func*)set_mem->ptr = value->func_ref;
  return 1;
}

static m_bool emit_func_def_init(Emitter emit, Func func) {
  Type t = emit->env->class_def;
  char c[(t ? strlen(t->name) + 1 : 0) + strlen(func->name) + 6];
  emit->env->func = func;
  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code();
  sprintf(c, "%s%s%s(...)", t ? t->name : "", t ? "." : "", func->name);
  emit->code->name = strdup(c);
  emit->code->need_this = GET_FLAG(func, ae_flag_member);
  emit->code->filename = strdup(emit->filename);
  return 1;
}

static m_bool emit_func_def_flag(Emitter emit, Func func) {
  if(GET_FLAG(func, ae_flag_member)) {
    emit->code->stack_depth += SZ_INT;
    if(emit_alloc_local(emit, SZ_INT, 1 << 1) < 0)
      CHECK_BB(err_msg(EMIT_, func->def->pos, "(emit): internal error: cannot allocate local 'this'...")) // LCOV_EXCL_LINE
  }

  return 1;
}

static m_bool emit_func_def_args(Emitter emit, Arg_List a) {
  while(a) {
    Value value = a->var_decl->value;
    m_int offset, size = value->m_type->size;
    m_bool obj = isa(value->m_type, &t_object) > 0 ? 1 << 2 : 0;
    m_bool ref = GET_FLAG(a->type_decl, ae_flag_ref) ? 1 << 1 : 0;
    emit->code->stack_depth += size;
    if((offset = emit_alloc_local(emit, size, ref | obj)) < 0)
      CHECK_BB(err_msg(EMIT_, a->pos,
        "(emit): internal error: cannot allocate local '%s'...", value->name))
    value->offset = offset;
    a = a->next;
  }
  return 1;
}

static m_bool emit_func_def_ensure(Emitter emit, m_uint size) {
  Instr instr;
  if(size) {
    instr = emitter_add_instr(emit, Reg_PushImm);
    instr->m_val = size;
  }
  vector_add(&emit->code->stack_return, (vtype)emitter_add_instr(emit, Goto));
  return 1;
}

static m_bool emit_func_def_return(Emitter emit) {
  m_uint i;
  for(i = 0; i < vector_size(&emit->code->stack_return); i++) {
    Instr instr = (Instr)vector_at(&emit->code->stack_return, i);
    instr->m_val = emit_code_size(emit);
  }
  vector_clear(&emit->code->stack_return);
  emit_func_release(emit);
  CHECK_OB(emitter_add_instr(emit, Func_Return))
  return 1;
}

static m_bool emit_func_def_code(Emitter emit, Func func) {
  Arg_List a = func->def->arg_list;
  func->code = emit_code(emit);
  if(GET_FLAG(func->def, ae_flag_dtor))
    emit->env->class_def->info->dtor = func->code;
  else if(GET_FLAG(func->def, ae_flag_op))
    operator_set_func(emit->env, func, a->type, a->next->type);
  return 1;
}

static m_bool emit_func_def_body(Emitter emit, Func_Def func_def) {
  CHECK_BB(emit_func_def_args(emit, func_def->arg_list))
  if(GET_FLAG(func_def, ae_flag_variadic)) {
    if(emit_alloc_local(emit, SZ_INT, 1 << 1) < 0)
      CHECK_BB(err_msg(EMIT_, func_def->pos, "(emit): internal error: cannot allocate local 'vararg'...")) // LCOV_EXCL_LINE
      emit->code->stack_depth += SZ_INT;
  }
  CHECK_BB(emit_stmt(emit, func_def->code, 0))
  CHECK_BB(emit_func_def_ensure(emit, func_def->ret_type->size))
  return 1;
}

static m_bool emit_func_def(Emitter emit, Func_Def func_def) {
  Func func = func_def->d.func;
  if(func_def->types) { // don't check template definition
    func_def->flag &= ~ae_flag_template;
    return 1;
  }
  if(!emit->env->class_def)
    CHECK_BB(emit_func_def_global(emit, func->value_ref))
  CHECK_BB(emit_func_def_init(emit, func))
  CHECK_BB(emit_func_def_flag(emit, func))
  emit_push_scope(emit);
  CHECK_BB(emit_func_def_body(emit, func_def))
  if(GET_FLAG(func_def, ae_flag_variadic) && (!emit->env->func->variadic ||
      !*(m_uint*)emit->env->func->variadic->ptr))
    CHECK_BB(err_msg(EMIT_, func_def->pos, "invalid variadic use"))
  CHECK_BB(emit_func_def_return(emit))
  emit_pop_scope(emit);
  CHECK_BB(emit_func_def_code(emit, func))
  emit->env->func = NULL;
  emit->code = (Code*)vector_pop(&emit->stack);
  return 1;
}

static m_bool emit_section(Emitter emit, Section* section) {
  ae_Section_Type t = section->type;
  if(t == ae_section_stmt)
    CHECK_BB(emit_stmt_list(emit, section->d.stmt_list))
  else if(t == ae_section_func)
    CHECK_BB(emit_func_def(emit, section->d.func_def))
  else if(t == ae_section_class)
    CHECK_BB(emit_class_def(emit, section->d.class_def))
  return 1;
}

static m_bool init_class_data(Nspc nspc) {
  if(nspc->class_data_size) {
    nspc->class_data = calloc(nspc->class_data_size, sizeof(char));
    if(!nspc->class_data)
      CHECK_BB(err_msg(EMIT_, 0, "OutOfMemory: while allocating static data '%s'\n", nspc->name))
  }
  memset(nspc->class_data, 0, nspc->class_data_size);
  return 1;
}

static Code* emit_class_code(Emitter emit, m_str name) {
  char c[strlen(name) + 7];
  Code* code = new_code();
  CHECK_OO(code);
  sprintf(c, "class %s", name);
  code->name = strdup(c);
  code->need_this = 1;
  code->filename = strdup(emit->filename);
  code->stack_depth += SZ_INT;
  return code;

}

static m_bool emit_class_def_body(Emitter emit, Class_Body body) {
  if(emit_alloc_local(emit, SZ_INT, 1 << 1 | 1 << 2) < 0)
    CHECK_BB(err_msg(EMIT_, body->pos,
          "internal error: cannot allocate local 'this'..."))
  while(body) {
    CHECK_BB(emit_section(emit, body->section))
    body = body->next;
  }
  return 1;
}

static m_bool emit_class_finish(Emitter emit, Nspc nspc, m_bool ret) {
  if(ret > 0) {
    CHECK_OB(emitter_add_instr(emit, Func_Return))
    free_vm_code(nspc->pre_ctor);
    nspc->pre_ctor = emit_code(emit);
  } else if(nspc->class_data_size)
    free(nspc->class_data);
  return 1;
}

static m_bool emit_class_push(Emitter emit, Type type) {
  vector_add(&emit->env->class_stack, (vtype)emit->env->class_def);
  emit->env->class_def = type;
  vector_add(&emit->stack, (vtype)emit->code);
  return 1;
}

static m_bool emit_class_pop(Emitter emit) {
  emit->env->class_def = (Type)vector_pop(&emit->env->class_stack);
  emit->code = (Code*)vector_pop(&emit->stack);
  return 1;
}

static m_bool emit_class_def(Emitter emit, Class_Def class_def) {
  Type type = class_def->type;
  m_bool ret = 1;

  if(class_def->types)
    return 1;
  CHECK_BB(init_class_data(type->info))
  CHECK_BB(emit_class_push(emit, type))
  CHECK_OB((emit->code = emit_class_code(emit, type->name)))
  ret = emit_class_def_body(emit, class_def->body);
  CHECK_BB(emit_class_finish(emit, type->info, ret))
  CHECK_BB(emit_class_pop(emit))
  return ret;
}

static void emit_free_stack(Emitter emit) {
  m_uint i, j;
  for(i = 0;  i < vector_size(&emit->stack); i++) {
    Code* code = (Code*)vector_at(&emit->stack, i);
    for(j = 0; j < vector_size(&code->code); j++)
      free((Instr)vector_at(&code->code, j));
     free_code(code);
  }
  for(i = 0; i < emit_code_size(emit); i++)
    free((Instr)vector_at(&emit->code->code, i));
  free_code(emit->code);
}

static m_bool emit_ast_inner(Emitter emit, Ast ast) {
  while(ast) {
    CHECK_BB(emit_section(emit, ast->section))
    ast = ast->next;
   }
  return 1;
}

m_bool emit_ast(Emitter emit, Ast ast, m_str filename) {
  int ret;
  emit->filename = filename;
#ifdef GWCOV
  if(emit->coverage) {
    char cov_filename[strlen(filename) + 3];
    sprintf(cov_filename, "%sda", filename);
    emit->cov_file = fopen(cov_filename, "a");
  }
#endif
  emit->code = new_code();
  vector_clear(&emit->stack);
  emit_push_scope(emit);
  CHECK_OB(emitter_add_instr(emit, start_gc))
  ret = emit_ast_inner(emit, ast);
  CHECK_OB(emitter_add_instr(emit, stop_gc))
  if(emit->cases) {
    free_map(emit->cases);
    emit->cases = NULL;
  }
  emit_pop_scope(emit);
  if(ret < 0) { // should free all stack.
    emit_free_stack(emit);
    free(filename);
    free_ast(ast);
  }
#ifdef GWCOV
  if(emit->coverage)
    fclose(emit->cov_file);
#endif
  return ret;
}
