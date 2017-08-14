#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "instr.h"
#include "vm.h"
#include "context.h"
#include "func.h"
#include "emit.h"

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

static Local* frame_alloc_local(Frame* frame, m_uint size, m_bool is_ref, m_bool is_obj) {
  Local* local = calloc(1, sizeof(Local));
  local->size = size;
  local->offset = frame->curr_offset;
  local->is_ref = is_ref;
  local->is_obj = is_obj;
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

static void sadd_instr(Emitter emit, f_instr f) {
  Instr instr = calloc(1, sizeof(struct Instr_));
  instr->execute = f;
  vector_add(&emit->code->code, (vtype)instr);
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
      Instr instr = add_instr(emit, Release_Object2);
      instr->m_val = l->offset;
    }
    free(l);
  }
  vector_release(&v);
}

m_bool emit_pre_ctor(Emitter emit, Type type) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "pre ctor");
#endif
  if(type->parent)
    emit_pre_ctor(emit, type->parent);
  if(GET_FLAG(type, ae_flag_ctor)) {
    Instr instr = add_instr(emit, Pre_Constructor);
    instr->m_val = (m_uint)type->info->pre_ctor;
    instr->m_val2 = (m_uint)emit->code->frame->curr_offset;
  }
  return 1;
}

static m_bool emit_pre_constructor_array(Emitter emit, Type type) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "pre ctor array");
#endif
  Instr top, bottom;
  m_uint start_index = vector_size(&emit->code->code);
  top = add_instr(emit, Instr_Pre_Ctor_Array_Top);
  *(Type*)top->ptr = type;
  emit_pre_ctor(emit, type);
  bottom = add_instr(emit, Instr_Pre_Ctor_Array_Bottom);
  top->m_val = vector_size(&emit->code->code);
  bottom->m_val = start_index;
  sadd_instr(emit, Instr_Pre_Ctor_Array_Post);
  return 1;
}

static m_bool emit_instantiate_object(Emitter emit, Type type, Array_Sub array, m_bool is_ref) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "instantiate object");
#endif
  if(type->array_depth) {
    CHECK_BB(emit_exp(emit, array->exp_list, 0))
    VM_Array_Info* info = calloc(1, sizeof(VM_Array_Info));
    info->depth = type->array_depth;
    info->type = type;
    info->is_obj = isa(type->d.array_type, &t_object) > 0 ? 1 : 0;
    info->is_ref = is_ref;
    Instr alloc = add_instr(emit, Instr_Array_Alloc);
    *(VM_Array_Info**)alloc->ptr = info;
    if(!is_ref && info->is_obj)
      emit_pre_constructor_array(emit, type->d.array_type);
  } else if(isa(type, &t_object) > 0 && !is_ref) {
    Instr instr = add_instr(emit, Instantiate_Object);
    *(Type*)instr->ptr = type;
    emit_pre_ctor(emit, type);
  }
  return 1;
}

static m_bool emit_symbol_owned(Emitter emit, S_Symbol symbol, Value v, m_bool emit_var, m_uint pos) {
  m_bool ret;
  Exp base = new_exp_prim_ID("this", pos);
  Exp dot = new_exp_dot(base, s_name(symbol), pos);
  base->type = v->owner_class;
  dot->type = v->m_type;
  dot->d.exp_dot.t_base = v->owner_class;
  dot->emit_var = emit_var;
  ret = emit_exp_dot(emit, &dot->d.exp_dot);
  free_expression(dot);
  if(ret < 0)
    CHECK_BB(err_msg(EMIT_, pos, "(emit): internal error: symbol transformation failed...")) // LCOV_EXCL_LINE
  return ret;
}

static m_bool emit_symbol_const(Emitter emit, Value v, m_bool emit_var) {
  if(v->func_ref) {
    Instr instr = add_instr(emit, Reg_Push_Imm);
    instr->m_val = (m_uint)v->func_ref;
  } else if(isa(v->m_type, &t_float) > 0 || isa(v->m_type, &t_dur) > 0 ||
      isa(v->m_type, &t_dur) > 0) {
    Instr instr = add_instr(emit, Reg_Push_Imm2);
    *(m_float*)instr->ptr = *(m_float*)v->ptr;
  } else {
    Instr instr = add_instr(emit, Reg_Push_Imm);
    instr->m_val = (emit_var ? (m_uint)&v->ptr : (m_uint)v->ptr);
  }
  return 1;
}

static m_bool emit_symbol_addr(Emitter emit, Value v) {
  Instr instr = add_instr(emit, Reg_Push_Mem_Addr);
  instr->m_val = v->offset;
  instr->m_val2 = GET_FLAG(v, ae_flag_global);
  return 1;
}

static f_instr emit_symbol_actual_instr(Kindof kind) {
  switch(kind) {
    case Kindof_Int:
      return Reg_Push_Mem;
    case Kindof_Float:
      return Reg_Push_Mem2;
    case Kindof_Complex:
      return Reg_Push_Mem_Complex;
    case Kindof_Vec3:
      return Reg_Push_Mem_Vec3;
    case Kindof_Vec4:
      return Reg_Push_Mem_Vec4;
    case Kindof_Void:
      break; // unreachable
  }
  return NULL;
}

static m_bool emit_symbol_actual(Emitter emit, Value v) {
  Instr instr;
  Kindof kind = kindof(v->m_type);
  f_instr f = emit_symbol_actual_instr(kind);

  instr         = add_instr(emit, f);
  instr->m_val  = v->offset;
  instr->m_val2 = GET_FLAG(v, ae_flag_global);
  return 1;
}

static m_bool emit_symbol(Emitter emit, S_Symbol symbol, Value v, int emit_var, int pos) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "symbol %s (const:%i) %i %p", s_name(symbol), GET_FLAG(v, ae_flag_const) + GET_FLAG(v, ae_flag_enum), GET_FLAG(v, ae_flag_static), v->owner_class);
#endif
  if(GET_FLAG(v, ae_flag_member) || GET_FLAG(v, ae_flag_static))
    return emit_symbol_owned(emit, symbol, v, emit_var, pos);
  if(GET_FLAG(v, ae_flag_const))
    return emit_symbol_const(emit, v, emit_var);
  if(emit_var)
    return emit_symbol_addr(emit, v);
  return emit_symbol_actual(emit, v);
}

VM_Code emit_code(Emitter emit) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "emit to code");
#endif
  Code* c = emit->code;
  VM_Code code = new_vm_code(&c->code, c->stack_depth, c->need_this, c->name, c->filename);
  free_code(c);
  return code;
}

static m_bool emit_exp_prim_array(Emitter emit, Array_Sub array) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "array lit");
#endif
  m_uint count = 0;
  CHECK_BB(emit_exp(emit, array->exp_list, 0))
  Exp e = array->exp_list;
  while(e) {
    count++;
    e = e->next;
  }
  Type type = array->type;
  Instr instr = add_instr(emit, Instr_Array_Init);
  VM_Array_Info* info = calloc(1, sizeof(VM_Array_Info));
  info->type = type;
  info->length = count;
  *(VM_Array_Info**)instr->ptr = info;
  instr->m_val2 = kindof(type->d.array_type);
  return 1;
}

static m_bool emit_exp_array(Emitter emit, Exp_Array* array) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "array (access?)");
#endif
  Type type, base_type;
  Instr instr;
  m_uint depth = 0;
  Array_Sub sub;
  Exp exp;
  m_uint is_var = 0;

  type = array->self->type;
  base_type = array->base->type;
  is_var = array->self->emit_var;
  depth = base_type->array_depth - type->array_depth;
  sub = array->indices;
  exp = sub->exp_list;
  CHECK_BB(emit_exp(emit, array->base, 0))
  CHECK_BB(emit_exp(emit, exp, 0))
  if(depth == 1) {
    instr = add_instr(emit, Instr_Array_Access);
    instr->m_val = is_var;
    instr->m_val2 = kindof(type);
  } else {
    instr = add_instr(emit, Instr_Array_Access_Multi);
    instr->m_val = depth;
    instr->m_val2 = kindof(array->base->type->d.array_type);
    *(m_uint*)instr->ptr = is_var || type->array_depth;
  }
  return 1;
}

static m_bool emit_exp_prim_vec(Emitter emit, Vec val) {
  CHECK_BB(emit_exp(emit, val->args, 0));
  m_int n = 3 - val->numdims;
  while(n > 0) {
    sadd_instr(emit, Reg_Push_Imm2);
    n--;
  }
  return 1;
}

static m_bool emit_exp_prim_id(Emitter emit, Exp_Primary* prim) {
  if(prim->d.var == insert_symbol("this"))
    sadd_instr(emit, Reg_Push_This);
  else if(prim->d.var == insert_symbol("me"))
    sadd_instr(emit, Reg_Push_Me);
  else if(prim->d.var == insert_symbol("now"))
    sadd_instr(emit, Reg_Push_Now);
  else if(prim->d.var == insert_symbol("false") ||
      prim->d.var == insert_symbol("null") ||
      prim->d.var == insert_symbol("NULL"))
    sadd_instr(emit, Reg_Push_Imm);
  else if(prim->d.var == insert_symbol("true")) {
    Instr instr = add_instr(emit, Reg_Push_Imm);
    instr->m_val = 1;
  } else if(prim->d.var == insert_symbol("maybe"))
    sadd_instr(emit, Reg_Push_Maybe);
  else
    emit_symbol(emit, prim->d.var, prim->value, prim->self->emit_var, prim->pos);
  return 1;
}

static m_bool emit_exp_prim_num(Emitter emit, m_uint i) {
  Instr instr = add_instr(emit, Reg_Push_Imm);
  instr->m_val = i;
  return 1;
}

static m_bool emit_exp_prim_float(Emitter emit, m_float f) {
  Instr instr = add_instr(emit, Reg_Push_Imm2);
  *(m_float*)instr->ptr = f;
  return 1;
}

static m_bool emit_exp_prim_char(Emitter emit, Exp_Primary* prim) {
  Instr instr = add_instr(emit, Reg_Push_Imm);
  instr->m_val = str2char(prim->d.chr, prim->pos);
  return 1;
}

static m_bool emit_exp_prim_str(Emitter emit, m_str str) {
  Instr instr = add_instr(emit, Reg_Push_Str);
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
    ADD_REF(e->type);
    e = e->next;
  }
  instr = add_instr(emit, Gack);
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

static m_bool emit_dot_static_data(Emitter emit, Value v, Kindof kind, m_bool emit_var) {
  Instr alloc, push = add_instr(emit, Reg_Push_Imm);
  push->m_val = (m_uint)v->owner_class;
  alloc = add_instr(emit, Dot_Static_Data);
  alloc->m_val2 = kind;
  *(m_uint*)alloc->ptr = emit_var;
  alloc->m_val = v->offset;
  return 1;
}

static Instr decl_member(Emitter emit, Kindof kind) {
  switch(kind)  {
    case Kindof_Int:
      return add_instr(emit, Alloc_Member_Word);
    case Kindof_Float:
      return add_instr(emit, Alloc_Member_Word_Float);
    case Kindof_Complex:
      return add_instr(emit, Alloc_Member_Word_Complex);
    case Kindof_Vec3:
      return add_instr(emit, Alloc_Member_Word_Vec3);
    case Kindof_Vec4:
      return add_instr(emit, Alloc_Member_Word_Vec4);
    case Kindof_Void:
      break;
  }
  return NULL;
}

static Instr decl_global(Emitter emit, Kindof kind) {
  switch(kind)  {
    case Kindof_Int:
      return add_instr(emit,  Alloc_Word);
    case Kindof_Float:
      return add_instr(emit,  Alloc_Word_Float);
    case Kindof_Complex:
      return add_instr(emit,  Alloc_Word_Complex);
    case Kindof_Vec3:
      return add_instr(emit,  Alloc_Word_Vec3);
    case Kindof_Vec4:
      return add_instr(emit,  Alloc_Word_Vec4);
    case Kindof_Void:
      break;
  }
  return NULL;
}

static m_bool decl_static(Emitter emit, Value v, Array_Sub array,
  Kindof kind, m_bool is_ref) {
  Code* code = emit->code;
  emit->code = (Code*)vector_back(&emit->stack);
  CHECK_BB(emit_instantiate_object(emit, v->m_type, array, is_ref))
  CHECK_BB(emit_dot_static_data(emit, v, kind, 1))
  Instr assign  = add_instr(emit, Assign_Object);
  assign->m_val = 0;
  emit->code = code;
  return 1;
}

static m_bool emit_exp_decl(Emitter emit, Exp_Decl* decl) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "decl");
#endif
  Exp_Decl* exp = decl;
  Var_Decl_List list = exp->list;
  Instr alloc;

  while(list) {
    Var_Decl var_decl = list->self;
    Value value = var_decl->value;
    Type type = value->m_type;
    Array_Sub array = list->self->array;
    m_bool is_obj = isa(type, &t_object) > 0 || list->self->array;
    m_bool is_ref = decl->type->ref;
    Kindof kind = kindof(type);

    if(is_obj && ((array && array->exp_list) || !is_ref) && !decl->is_static)
      CHECK_BB(emit_instantiate_object(emit, type, array, is_ref))
    if(GET_FLAG(value, ae_flag_member)) {
      alloc = decl_member(emit, kind);
    } else {
      if(!emit->env->class_def || !decl->is_static) {
        Local* local = frame_alloc_local(emit->code->frame, type->size, is_ref, is_obj);
        CHECK_OB(local)
        value->offset   = local->offset;
        alloc = decl_global(emit, kind);
        alloc->m_val2 = GET_FLAG(value, ae_flag_global);
      } else { // static
        if(is_obj && !is_ref)
          CHECK_BB(decl_static(emit, value, array, kind, is_ref))
        CHECK_BB(emit_dot_static_data(emit, value, kind, 1))
        list = list->next;
        continue;
      }
    }
    alloc->m_val = value->offset;
    *(m_uint*)alloc->ptr = ((is_ref && !array) || isprim(type) > 0)  ? decl->self->emit_var : 1;
    if(is_obj) {
      if((array && array->exp_list) || !is_ref) {
        Instr assign = add_instr(emit, Assign_Object);
        assign->m_val = decl->self->emit_var;
      }
      if(array && array->exp_list)
        ADD_REF(type);
    }
    list = list->next;
  }
  return 1;
}

static m_bool emit_func_args(Emitter emit, Exp_Func* exp_func) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "func args");
#endif
  if(emit_exp(emit, exp_func->args, 1) < 0)
    CHECK_BB(err_msg(EMIT_, exp_func->pos, "(emit): internal error in emitting function call arguments...")) // LCOV_EXCL_LINE
  if(GET_FLAG(exp_func->m_func->def, ae_flag_variadic)) {
    m_uint offset = 0, size = 0;
    Instr instr;
    Exp e = exp_func->args;
    Arg_List l = exp_func->m_func->def->arg_list;
    Vector kinds = new_vector();
    while(e) {
      if(!l) {
        size += e->type->size;
        vector_add(kinds, (vtype)kindof(e->type));
      } else {
        l = l->next;
        offset += e->type->size;
      }
      e = e->next;
    }
    instr = add_instr(emit, MkVararg);
    instr->m_val = size;
    instr->m_val2 = (m_uint)kinds;
  }
  return 1;
}

static m_bool emit_exp_call_helper(Emitter emit, Exp_Func* exp_func, m_bool spork) {
  if(exp_func->args && !spork && emit_func_args(emit, exp_func) < 0)
    CHECK_BB(err_msg(EMIT_, exp_func->pos, "internal error in evaluating function arguments...")) // LCOV_EXCL_LINE
  if(emit_exp(emit, exp_func->func, 0) < 0)
    CHECK_BB(err_msg(EMIT_, exp_func->pos, "internal error in evaluating function call...")) // LCOV_EXCL_LINE
  if(GET_FLAG(exp_func->m_func->def, ae_flag_variadic) && !exp_func->args) { // handle empty call to variadic functions
    sadd_instr(emit, MkVararg);
    sadd_instr(emit, Reg_Push_Imm);
  }
  return 1;
}

static m_bool emit_exp_call(Emitter emit, Exp_Func* exp_func, m_bool spork) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "func call");
#endif
  if(exp_func->types) {
    Func_Def def = exp_func->m_func->def;
    ID_List base_t = def->base;
    Type_List list = exp_func->types;
    if(exp_func->m_func->value_ref->owner_class)
      CHECK_BB(env_push_class(emit->env, exp_func->m_func->value_ref->owner_class))
    nspc_push_type(emit->env->curr);
    while(base_t) {
      nspc_add_type(emit->env->curr, base_t->xid, find_type(emit->env, list->list));
      base_t = base_t->next;
      list = list->next;
    }
    SET_FLAG(def, ae_flag_template);

    CHECK_BB(scan1_func_def(emit->env, def))
    CHECK_BB(scan2_func_def(emit->env, def))
    CHECK_BB(check_func_def(emit->env, def))

    CHECK_BB(emit_exp_call_helper(emit, exp_func, spork))
    nspc_pop_type(emit->env->curr);
    if(exp_func->m_func->value_ref->owner_class)
      CHECK_BB(env_pop_class(emit->env))
  } else
    CHECK_BB(emit_exp_call_helper(emit, exp_func, spork))
  return emit_exp_call1(emit, exp_func->m_func, exp_func->ret_type, exp_func->pos);
}

static m_bool emit_exp_binary(Emitter emit, Exp_Binary* binary) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "binary");
#endif
  Exp lhs = binary->lhs;
  Exp rhs = binary->rhs;

  CHECK_BB(emit_exp(emit, lhs, 1))
  CHECK_BB(emit_exp(emit, rhs, 1))
  // arrays
  if(binary->op == op_shift_left && (lhs->type->array_depth == rhs->type->array_depth + 1)
      && isa(lhs->type->d.array_type, rhs->type) > 0) {
    Instr instr = add_instr(emit, Array_Append);
    instr->m_val = kindof(rhs->type);
    return 1;
  }

  if(lhs->type->array_depth && rhs->type->array_depth) {
    if(binary->op == op_at_chuck && lhs->type->array_depth == rhs->type->array_depth)
      sadd_instr(emit, Assign_Object);
    return 1;
  }
  // function pointer assignement
  if(binary->op == op_at_chuck && isa(lhs->type, &t_function) > 0 && isa(rhs->type, &t_func_ptr) > 0) {
    Value v = NULL;
    Instr instr = add_instr(emit, assign_func);
    switch(rhs->exp_type) {
      case ae_exp_dot:
        v = find_value(rhs->d.exp_dot.t_base, rhs->d.exp_dot.xid);
        instr->m_val = 1;
        break;
      case ae_exp_primary:
        if(GET_FLAG(rhs->d.exp_primary.value, ae_flag_member)) {
          v = rhs->d.exp_primary.value;
          instr->m_val = 1;
        }
        break;
      case ae_exp_decl:
        v = rhs->d.exp_decl.list->self->value;
        break;
      default:
        return -1;
    }
    instr->m_val2 = v ? v->offset : 0;
    return 1;
  }

  if(binary->op == op_chuck && isa(rhs->type, &t_function) > 0)
    return emit_exp_call1(emit, binary->func, binary->func->value_ref->m_type, binary->pos);

  if(binary->op == op_at_chuck && isa(rhs->type, &t_object) > 0 &&
      (isa(lhs->type, &t_null) > 0 || isa(lhs->type, &t_object) > 0)) {
    Instr instr = add_instr(emit, Assign_Object);
    instr->m_val2 = 1;
    return 1;
  }
  CHECK_OB(get_instr(emit, binary->op, lhs->type, rhs->type))
  return 1;
}

static m_bool exp_exp_cast1(Emitter emit, Type to, Type from) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "cast %s to %s", from->name, to->name);
#endif
  f_instr f = NULL;
  if(to->xid == from->xid || isa(from, to) > 0 ||
      (isa(from, &t_null) > 0 && isa(to, &t_object) > 0))
    return 1;
  else if(to->xid == te_int && from->xid == te_float)
    f = Cast_f2i;
  else if(to->xid == te_float && from->xid == te_int)
    f = Cast_i2f;
  sadd_instr(emit, f);
  return 1;
}

static m_bool exp_exp_cast(Emitter emit, Exp_Cast* cast) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "cast");
#endif
  Type to = cast->self->type;
  Type from = cast->exp->type;
  if(isa(to, &t_func_ptr) > 0 && isa(from, &t_function) > 0) {
    Instr push = add_instr(emit, Reg_Push_Imm);
    push->m_val = (m_uint)cast->func;
    return 1;
  }
  CHECK_BB(emit_exp(emit, cast->exp, 0))
  return exp_exp_cast1(emit, to, from);
}

static m_bool emit_exp_postfix(Emitter emit, Exp_Postfix* postfix) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "postfix");
#endif
  CHECK_BB(emit_exp(emit, postfix->exp, 0))
  return get_instr(emit, postfix->op, postfix->exp->type, NULL) ? 1 : -1;
}

static m_bool emit_exp_dur(Emitter emit, Exp_Dur* dur) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "dur");
#endif
  CHECK_BB(emit_exp(emit, dur->base, 0))
  if(isa(dur->base->type, &t_int) > 0)
    sadd_instr(emit, Cast_i2f);
  CHECK_BB(emit_exp(emit, dur->unit, 0))
  sadd_instr(emit, float_times);

  return 1;
}

/* not static as it is called in operator.c*/
m_bool emit_exp_call1(Emitter emit, Func func, Type type, int pos) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "func call1. '%s' offset: %i", func->name, emit->code->frame->curr_offset);
#endif
  Instr code, offset, call;
  if(!func->code) { // function pointer or template
    Func f = isa(func->value_ref->m_type, &t_func_ptr) > 0 ||
             isa(func->value_ref->m_type, &t_class) > 0 ?
             nspc_lookup_func(func->value_ref->owner, func->def->name, 1) :
             nspc_lookup_func(emit->env->curr, insert_symbol(func->name), -1);

    if(!f) { //template with no list
      if(!GET_FLAG(func->def, ae_flag_template))
        CHECK_BB(err_msg(EMIT_, func->def->pos, "function not emitted yet"))
        if(emit_func_def(emit, func->def) < 0)
          CHECK_BB(err_msg(EMIT_, 0, "can't emit func.")) // LCOV_EXCL_LINE
          func->code = func->def->d.func->code;
      code = add_instr(emit, Reg_Push_Ptr);
      *(VM_Code*)code->ptr = func->code;
    } else {
      code = add_instr(emit, Reg_Push_Code);
      code->m_val = func->value_ref->offset;
      code->m_val2 = func->value_ref->owner_class ? 1 : 0;
    }
  } else {
    code = add_instr(emit, Reg_Push_Ptr);
    *(VM_Code*)code->ptr = func->code;
  }

  if(!emit->code->stack_depth && !emit->code->frame->curr_offset)
    if(!GET_FLAG(func, ae_flag_member)) // 25/07/17
    sadd_instr(emit, Mem_Push_Imm);

  offset = add_instr(emit, Reg_Push_Imm);
  offset->m_val = emit->code->frame->curr_offset;
  call = add_instr(emit, Instr_Exp_Func);
//  call->m_val = GET_FLAG(func->def, ae_flag_builtin) ? kindof(func->def->ret_type) : emit->code->stack_depth;
  call->m_val = GET_FLAG(func->def, ae_flag_builtin) ? func->def->ret_type->size : emit->code->stack_depth;
  if(GET_FLAG(func->def, ae_flag_builtin)) {
    if(!func->code || !func->code->native_func)
      CHECK_BB(err_msg(EMIT_, func->def->pos, "missing native func. are you trying to spork?"))
      if(GET_FLAG(func, ae_flag_member))
        call->execute = Instr_Exp_Func_Member;
      else
        call->execute = Instr_Exp_Func_Static;
  } else if(!strcmp(s_name(func->def->name), "chuck")) { // should also handle other ops
    call->execute = Instr_Op_Call_Binary;
    call->m_val2  = (m_uint)func->def->arg_list->type;
    *(Type*)call->ptr     = func->def->arg_list->next->type;
  }
  return 1;
}

static m_bool emit_exp_spork_finish(Emitter emit, VM_Code code, Func f, m_uint arg_size, m_uint stack_depth) {
  Instr push_code, spork;

  push_code = add_instr(emit, Reg_Push_Imm);
  push_code->m_val = (m_uint)code;
  spork = add_instr(emit, Spork);
  spork->m_val = arg_size;
  spork->m_val2 = (m_uint)f;
  *(m_uint*)spork->ptr = stack_depth; // only for some sporked expressions
  return 1;
}

static m_bool emit_exp_spork(Emitter emit, Exp_Func* exp) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "spork");
#endif
  Instr op;
  VM_Code code;

  CHECK_BB(emit_func_args(emit, exp))
  if(emit_exp(emit, exp->func, 0) < 0)
    CHECK_BB(err_msg(EMIT_, exp->pos, "(emit): internal error in evaluating function call...")) // LCOV_EXCL_LINE
    vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code();
  sadd_instr(emit, start_gc);
  emit->code->need_this = GET_FLAG(exp->m_func, ae_flag_member);
  emit->code->name = strdup("spork~exp");
  emit->code->filename = strdup(emit->filename);
  op = add_instr(emit, Mem_Push_Imm);
  CHECK_BB(emit_exp_call1(emit, exp->m_func, exp->ret_type, exp->pos))
  sadd_instr(emit, stop_gc);
  sadd_instr(emit, EOC);
  op->m_val = emit->code->stack_depth;

  code = emit_code(emit);
  exp->vm_code = code;
  emit->code = (Code*)vector_pop(&emit->stack);

  if(exp->m_func->value_ref->owner_class) // just a workaround: leads to a leak
    ADD_REF(exp->m_func->code)

    Exp e = exp->args;
  m_uint size = 0;
  while(e) {
    size += e->cast_to ? e->cast_to->size : e->type->size;
    e = e->next;
  }
  CHECK_BB(emit_exp_spork_finish(emit, code, NULL, size, 0)) // last arg migth have to be 'emit->code->frame->curr_offset'
  return 1;
}

static m_bool emit_exp_spork1(Emitter emit, Stmt stmt) {
  Instr op;
  VM_Code code;
  ID_List list = new_id_list("sporked", stmt->pos);
  Func f = new_func("sporked", new_func_def(0, new_type_decl(list, 0, stmt->pos), "sporked", NULL, stmt, stmt->pos));

  if(emit->env->class_def)
    sadd_instr(emit, Reg_Push_This);
  Instr push = add_instr(emit, Reg_Push_Imm);
  push->m_val = (m_uint)f;

  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code();
  if(emit->env->class_def) {
    SET_FLAG(f, ae_flag_member);
    emit->code->need_this = 1;
  }
  emit->code->name = strdup("spork~code");
  emit->code->filename = strdup(emit->filename);
  op = add_instr(emit, Mem_Push_Imm);
  frame_push_scope(emit->code->frame);

  sadd_instr(emit, start_gc);
  CHECK_BB(emit_stmt(emit, stmt, 0))
  sadd_instr(emit, stop_gc);
  emit_pop_scope(emit);
  sadd_instr(emit, EOC);
  op->m_val = emit->code->stack_depth;
  f->code = code = emit_code(emit);
  emit->code = (Code*)vector_pop(&emit->stack);
  CHECK_BB(emit_exp_spork_finish(emit, code, f, 0, emit->env->func ? emit->env->func->def->stack_depth : 0))
  return 1;
}

static m_bool emit_exp_unary(Emitter emit, Exp_Unary* unary) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "exp_unary");
#endif
  if(unary->op != op_spork && emit_exp(emit, unary->exp, 0) < 0)
    return -1;
  switch(unary->op) {
    case op_spork:
      CHECK_BB((unary->code ? emit_exp_spork1(emit, unary->code) : emit_exp_spork(emit, &unary->exp->d.exp_func)))
      break;
    case op_new:
      CHECK_BB(emit_instantiate_object(emit, unary->self->type, unary->array, unary->type->ref))
      break;
    default:
      return get_instr(emit, unary->op, NULL, unary->exp->type) ? 1 : -1;
  }
  return 1;
}

static m_bool emit_implicit_cast(Emitter emit, Type from, Type to) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "implicit cast '%s' to '%s'", from->name, to->name);
#endif
  if(from->xid == te_int && to->xid == te_float)
    sadd_instr(emit, Cast_i2f);
  else if(from->xid == te_float && to->xid == te_int)
    sadd_instr(emit, Cast_f2i);
  return 1;
}

static Instr emit_flow(Emitter emit, Type type, f_instr f1, f_instr f2) {
  switch(type->xid) {
    case te_int:
      sadd_instr(emit, Reg_Push_Imm);
      return add_instr(emit, f1);
    case te_float:
    case te_dur:
    case te_time:
      sadd_instr(emit, Reg_Push_Imm2);
      return add_instr(emit, f2);
    default:
      break;
  }
  return NULL;
}

static m_bool emit_exp_if(Emitter emit, Exp_If* exp_if) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "expression if");
#endif
  m_bool ret;
  Instr op = NULL, op2 = NULL;
  nspc_push_value(emit->env->curr);
  CHECK_BB(emit_exp(emit, exp_if->cond, 0))
  CHECK_OB((op = emit_flow(emit, exp_if->cond->type, Branch_Eq_Int, Branch_Eq_Float)))
  CHECK_BB((ret = emit_exp(emit, exp_if->if_exp, 0)))
  op2 = add_instr(emit, Goto);
  op->m_val = vector_size(&emit->code->code);
  ret = emit_exp(emit, exp_if->else_exp, 0);
  nspc_pop_value(emit->env->curr);
  CHECK_OB(ret)
  op2->m_val = vector_size(&emit->code->code);
  return ret;
}

static m_bool emit_exp(Emitter emit, Exp exp, m_bool ref) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "expression");
#endif
  Exp tmp = exp;
  while(tmp) {
    switch(tmp->exp_type) {
      case ae_exp_decl:
        CHECK_BB(emit_exp_decl(emit, &tmp->d.exp_decl))       break;
      case ae_exp_primary:
        CHECK_BB(emit_exp_primary(emit, &tmp->d.exp_primary)) break;
      case ae_exp_unary:
        CHECK_BB(emit_exp_unary(emit, &tmp->d.exp_unary))     break;
      case ae_exp_binary:
        CHECK_BB(emit_exp_binary(emit, &tmp->d.exp_binary))   break;
      case ae_exp_postfix:
        CHECK_BB(emit_exp_postfix(emit, &tmp->d.exp_postfix)) break;
      case ae_exp_cast:
        CHECK_BB(exp_exp_cast(emit, &tmp->d.exp_cast))        break;
      case ae_exp_dot:
        CHECK_BB(emit_exp_dot(emit, &tmp->d.exp_dot))         break;
      case ae_exp_call:
        CHECK_BB(emit_exp_call(emit, &tmp->d.exp_func, 0))    break;
      case ae_exp_array:
        CHECK_BB(emit_exp_array(emit, &tmp->d.exp_array))     break;
      case ae_exp_if:
        CHECK_BB(emit_exp_if(emit, &tmp->d.exp_if))           break;
      case ae_exp_dur:
        CHECK_BB(emit_exp_dur(emit, &tmp->d.exp_dur))         break;
    }
    if(tmp->cast_to)
      CHECK_BB(emit_implicit_cast(emit, tmp->type, tmp->cast_to))
      if(ref && isprim(tmp->type) < 0 && isa(tmp->type, &t_void) < 0) {
        Instr ref = add_instr(emit, Reg_AddRef_Object3);
        ref->m_val = tmp->emit_var;
      }
    tmp = tmp->next;
  }
#ifdef DEBUG_EMIT
  debug_msg("emit", "exp end");
#endif
  return 1;
}

static m_bool emit_stmt_if(Emitter emit, Stmt_If stmt) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "emit if");
#endif
  Instr op = NULL, op2 = NULL;

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  CHECK_OB((op = emit_flow(emit, isa(stmt->cond->type, &t_object) > 0 ? &t_int : stmt->cond->type,
                           Branch_Eq_Int, Branch_Eq_Float)))
  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_stmt(emit, stmt->if_body, 1))
  emit_pop_scope(emit);
  op2 = add_instr(emit, Goto);
  op->m_val = vector_size(&emit->code->code);

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_stmt(emit, stmt->else_body, 1))
  emit_pop_scope(emit);

  op2->m_val = vector_size(&emit->code->code);
  emit_pop_scope(emit);
  return 1;
}

static m_bool emit_stmt_code(Emitter emit, Stmt_Code stmt, m_bool push) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "emit stmt code");
#endif
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
  for(i = vector_size(v) - 1; i; i--) {
    Local* l = (Local*)vector_at(v, i);
    if(!l)
      break;
    else if(l->is_obj) {
      Instr rel = add_instr(emit, Release_Object2);
      rel->m_val = l->offset;
    }
  }
}

static m_bool emit_stmt_return(Emitter emit, Stmt_Return stmt) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "return");
#endif
  CHECK_BB(emit_exp(emit, stmt->val, 0))
  emit_func_release(emit); // /04/04/2017
  //  if(stmt->val && isa(stmt->val->type, &t_object) > 0) // void doesn't have ->val
  //	add_instr(emit, Reg_AddRef_Object3);
  vector_add(&emit->code->stack_return, (vtype)add_instr(emit, Goto));
  return 1;
}

static m_bool emit_stmt_continue(Emitter emit, Stmt_Continue cont) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "continue");
#endif
  vector_add(&emit->code->stack_cont, (vtype)add_instr(emit, Goto));
  return 1;
}

static m_bool emit_stmt_break(Emitter emit, Stmt_Break cont) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "break");
#endif
  vector_add(&emit->code->stack_break, (vtype)add_instr(emit, Goto));
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
    instr->m_val = vector_size(&emit->code->code);
  }
  emit_pop_scope(emit);
  vector_pop(&emit->code->stack_cont);
  vector_pop(&emit->code->stack_break);
}

static m_bool emit_stmt_while(Emitter emit, Stmt_While stmt) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "func while");
#endif
  m_bool ret = 1;
  m_uint index = vector_size(&emit->code->code);
  Instr op, goto_;

  frame_push_scope(emit->code->frame);
  emit_push_stack(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  CHECK_OB((op = emit_flow(emit, stmt->cond->type, Branch_Eq_Int, Branch_Eq_Float)))

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_stmt(emit, stmt->body, 1)) // was '0' , then 'stmt->body->type == ae_stmt_code ? 0 : 1'
  emit_pop_scope(emit);

  goto_ = add_instr(emit, Goto);
  goto_->m_val = index;
  op->m_val = vector_size(&emit->code->code);
  emit_pop_stack(emit, index);
  return ret;
}

static m_bool emit_stmt_do_while(Emitter emit, Stmt_While stmt) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "do while");
#endif
  m_bool ret = 1;
  Instr op;
  m_uint index = vector_size(&emit->code->code);

  frame_push_scope(emit->code->frame);
  emit_push_stack(emit);
  frame_push_scope(emit->code->frame);
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
#ifdef DEBUG_EMIT
  debug_msg("emit", "until");
#endif
  Instr op;
  m_uint index;

  frame_push_scope(emit->code->frame);
  index = vector_size(&emit->code->code);
  emit_push_stack(emit);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  CHECK_OB((op = emit_flow(emit, stmt->cond->type,
                           Branch_Neq_Int, Branch_Neq_Float)))
  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);

  Instr _goto = add_instr(emit, Goto);
  _goto->m_val = index;
  op->m_val = vector_size(&emit->code->code);
  emit_pop_stack(emit, index);
  return 1;
}

static m_bool emit_stmt_do_until(Emitter emit, Stmt_Until stmt) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "do until");
#endif
  Instr op;
  m_uint index;

  frame_push_scope(emit->code->frame);
  index = vector_size(&emit->code->code);
  emit_push_stack(emit);
  frame_push_scope(emit->code->frame);
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
#ifdef DEBUG_EMIT
  debug_msg("emit", "for");
#endif
  Instr  op = NULL;
  m_uint index;

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_stmt(emit, stmt->c1, 1))
  index = vector_size(&emit->code->code);
  emit_push_stack(emit);
  CHECK_BB(emit_stmt(emit, stmt->c2, 0))
  CHECK_OB((op = emit_flow(emit, stmt->c2->d.stmt_exp.val->type,
                           Branch_Eq_Int, Branch_Eq_Float)))
  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);
  m_uint action_index = vector_size(&emit->code->code);
  if(stmt->c3) {
    CHECK_BB(emit_exp(emit, stmt->c3, 0))
    Exp e = stmt->c3;
    m_uint num_words = 0;
    while(e) {
      num_words += e->type->size;
      e = e->next;
    }
    if(num_words) {
      Instr pop = add_instr(emit, Reg_Pop_Word4);
      pop->m_val = num_words;
    }
  }
  Instr _goto = add_instr(emit, Goto);
  _goto->m_val = index;

//  if(stmt->c2) // coverity told me to do so
  op->m_val = vector_size(&emit->code->code);
  emit_pop_stack(emit, action_index);
  return 1;
}

static m_bool emit_stmt_loop(Emitter emit, Stmt_Loop stmt) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "loop");
#endif
  Instr init, op, deref, dec, _goto;
  m_int* counter;
  m_uint index;

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_exp(emit, stmt->cond, 0))
  counter = calloc(1, sizeof(m_int));
  init = add_instr(emit, Init_Loop_Counter);
  init->m_val = (m_uint)counter;
  index = vector_size(&emit->code->code);
  emit_push_stack(emit);
  deref = add_instr(emit, Reg_Push_Deref);
  deref->m_val = (m_uint)counter;

  sadd_instr(emit, Reg_Push_Imm);
  op = add_instr(emit, Branch_Eq_Int);
  dec = add_instr(emit, Dec_int_Addr);
  dec->m_val = (m_uint)counter;

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);

  _goto = add_instr(emit, Goto);
  _goto->m_val = index;
  op->m_val = vector_size(&emit->code->code);
  emit_pop_stack(emit, index);
  return 1;
}

static m_bool emit_stmt_gotolabel(Emitter emit, Stmt_Goto_Label stmt) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "%s '%s'", stmt->is_label ? "label" : "goto", s_name(stmt->name));
#endif
  m_uint i, size;
  Stmt_Goto_Label label;
  if(!stmt->is_label)
    stmt->data.instr = add_instr(emit, Goto);
  else {
    if(emit->cases && !strcmp(s_name(stmt->name), "default")) {
      if(emit->default_case_index != -1)
        CHECK_BB(err_msg(EMIT_, stmt->pos, "default case already defined"))
        emit->default_case_index = vector_size(&emit->code->code);
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
      label->data.instr->m_val = vector_size(&emit->code->code);
    }
    vector_release(&stmt->data.v);
  }
  return 1;
}

static m_bool emit_stmt_switch(Emitter emit, Stmt_Switch stmt) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "emit switch");
#endif
  Instr instr, _break;
  vector_add(&emit->code->stack_break, (vtype)NULL);
  CHECK_BB(emit_exp(emit, stmt->val, 0))
  if(emit->cases)
    CHECK_BB(err_msg(EMIT_, stmt->pos, "swith inside an other switch. this is not allowed for now"))
    emit->default_case_index = -1;
  sadd_instr(emit, start_gc);
  instr = add_instr(emit, Branch_Switch);
  emit->cases = new_map();
  *(Map*)instr->ptr = emit->cases;

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_stmt(emit, stmt->stmt, 1))
  emit_pop_scope(emit);
  instr->m_val = emit->default_case_index > -1 ? emit->default_case_index : vector_size(&emit->code->code);
  emit->default_case_index = -1;
  while(vector_size(&emit->code->stack_break) && vector_back(&emit->code->stack_break)) {
    _break = (Instr)vector_pop(&emit->code->stack_break);
    _break->m_val = vector_size(&emit->code->code);
  }
  vector_pop(&emit->code->stack_break);
  sadd_instr(emit, stop_gc);
  emit->cases = NULL;
  return 1;
}

static m_bool emit_stmt_case(Emitter emit, Stmt_Case stmt) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "case");
#endif
  m_uint value = 0;
  Value v;
  Type t;
  if(!emit->cases)
    CHECK_BB(err_msg(EMIT_, stmt->pos, "case found outside switch statement. this is not allowed for now"))
    if(stmt->val->exp_type == ae_exp_primary) {
      if(stmt->val->d.exp_primary.type == ae_primary_num)
        value = stmt->val->d.exp_primary.d.num;
      else {
        if(stmt->val->d.exp_primary.d.var == insert_symbol("true"))
          value = 1;
        else if(stmt->val->d.exp_primary.d.var == insert_symbol("false"))
          value = 0;
        else if(stmt->val->d.exp_primary.d.var == insert_symbol("maybe")) {
          CHECK_BB(err_msg(EMIT_, stmt->val->d.exp_primary.pos, "'maybe' is not constant."))
        } else  {
          if(!GET_FLAG(stmt->val->d.exp_primary.value, ae_flag_const))
            CHECK_BB(err_msg(EMIT_, stmt->pos, "value is not const. this is not allowed for now"))
            value = (m_uint)stmt->val->d.exp_primary.value->ptr; // assume enum.
        }
      }
    } else if(stmt->val->exp_type == ae_exp_dot) {
      t = isa(stmt->val->d.exp_dot.t_base, &t_class) > 0 ?
          stmt->val->d.exp_dot.t_base->d.actual_type : stmt->val->d.exp_dot.t_base;
      v = find_value(t, stmt->val->d.exp_dot.xid);
      value = GET_FLAG(v, ae_flag_enum) ? t->info->class_data[v->offset] : *(m_uint*)v->ptr;
    } else
      CHECK_BB(err_msg(EMIT_, stmt->pos, "unhandled expression type '%i'", stmt->val->exp_type))
  if(map_get(emit->cases, (vtype)value))
    CHECK_BB(err_msg(EMIT_, stmt->pos, "duplicated cases value %i", value))
  map_set(emit->cases, (vtype)value, (vtype)vector_size(&emit->code->code));
  return 1;
}

static m_bool emit_stmt_typedef(Emitter emit, Stmt_Ptr ptr) {
  if(GET_FLAG(ptr, ae_flag_static))
    ADD_REF(ptr->func)
    return 1;
}

static m_bool emit_stmt_enum(Emitter emit, Stmt_Enum stmt) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "enum");
#endif
  m_uint i;
  for(i = 0; i < vector_size(&stmt->values); i++) {
    Value v = (Value)vector_at(&stmt->values, i);
    Local* local;
    if(!emit->env->class_def) {
      CHECK_OB((local = frame_alloc_local(emit->code->frame, sizeof(m_uint), 0, 0)))
      v->offset = local->offset;
      v->ptr = (m_uint*)i;
    } else
      emit->env->class_def->info->class_data[v->offset] = i;
  }
  return 1;
}

static m_bool emit_stmt_union(Emitter emit, Stmt_Union stmt) {
  Decl_List l = stmt->l;

  if(!GET_FLAG(l->self->d.exp_decl.list->self->value, ae_flag_member)) {
    Local* local = frame_alloc_local(emit->code->frame, stmt->s, 1, 0);
    stmt->o = local->offset;
  }

  while(l) {
    Var_Decl_List var_list = l->self->d.exp_decl.list;
    while(var_list) {
      var_list->self->value->offset = stmt->o;
      var_list = var_list->next;
    }
    l = l->next;
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
      Instr instr = add_instr(emit, Reg_Pop_Word4);
      instr->m_val = (e->exp_type == ae_exp_decl ?
        e->d.exp_decl.num_decl * e->type->size : e->type->size);
      e = e->next;
    }
  }
 return 1;
}

static m_bool emit_stmt(Emitter emit, Stmt stmt, m_bool pop) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "Stmt (pop: %s)", pop ? "yes" : "no");
#endif
  switch(stmt ? stmt->type : -1) {
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
      return emit_stmt_typedef(emit, &stmt->d.stmt_ptr);
    case ae_stmt_union:
      return emit_stmt_union(emit, &stmt->d.stmt_union);
  }
  return 1;
}

static m_bool emit_stmt_list(Emitter emit, Stmt_List list) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "Stmt list");
#endif
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

static m_bool emit_dot_static_import_data(Emitter emit, Value v, Type type, m_bool emit_addr) {
  Instr func_i;

  if(v->ptr && GET_FLAG(v, ae_flag_builtin)) { // from C
    func_i = add_instr(emit, Dot_Static_Import_Data);
    func_i->m_val = (m_uint)v->ptr;
    func_i->m_val2 = kindof(v->m_type);
    *(m_uint*)func_i->ptr = emit_addr;
  } else { // from code
    Instr push_i = add_instr(emit, Reg_Push_Imm);
    func_i = add_instr(emit, Dot_Static_Data);
    push_i->m_val = (m_uint)type;
    func_i->m_val = (m_uint)v->offset;
    func_i->m_val2 = (m_uint)kindof(v->m_type);
    *(m_uint*)func_i->ptr = emit_addr;
  }
  return 1;

}

static m_bool emit_complex_member(Emitter emit, Exp exp, Value v, m_str c, m_bool emit_addr) {
  Instr instr;

  if(exp->meta == ae_meta_var)
    exp->emit_var = 1;
  CHECK_BB(emit_exp(emit, exp, 0))
  if(!strcmp(v->name, c))
    instr = add_instr(emit, complex_real);
  else
    instr = add_instr(emit, complex_imag);
  instr->m_val = emit_addr;
  return 1;
}

static m_bool emit_vec_member(Emitter emit, Exp exp, Value v, m_bool emit_addr) {
  Instr instr;

  if(exp->meta == ae_meta_var)
    exp->emit_var = 1;
  CHECK_BB(emit_exp(emit, exp, 0))
  if(v->func_ref) {
    sadd_instr(emit, Reg_Dup_Last_Vec4);
    instr = add_instr(emit, member_function);
    *(Vector*)instr->ptr = &v->owner_class->info->obj_v_table;
    instr->m_val = v->func_ref->vt_index;
    return 1;
  }
  instr = add_instr(emit, vec_member);
  switch(v->name[0]) {
    case 'x':
      instr->m_val2 = 0;
      break;
    case 'y':
      instr->m_val2 = 1;
      break;
    case 'z':
      instr->m_val2 = 2;
      break;
    case 'w':
      instr->m_val2 = 3;
      break;
  }
  instr->m_val = emit_addr;
  return 1;
}

static m_bool emit_vararg(Emitter emit, Exp_Dot* member) {
  m_uint offset = 0;
  Arg_List l = emit->env->func->def->arg_list;
  while(l) {
    offset += l->type->size;
    l = l->next;
  }
  if(!strcmp(s_name(member->xid), "start")) {
    if(emit->env->func->variadic) {
      /*free(emit->env->func->variadic);*/
      CHECK_BB(err_msg(EMIT_, 0, "vararg.start already used. this is an error"))
    }
    emit->env->func->variadic = add_instr(emit, Vararg_start);
    emit->env->func->variadic->m_val = offset;
    emit->env->func->variadic->m_val2 = vector_size(&emit->code->code);
  }
  if(!strcmp(s_name(member->xid), "end")) {
    if(!emit->env->func->variadic)
      CHECK_BB(err_msg(EMIT_, 0, "vararg.start not used before vararg.end. this is an error"))
    Instr instr = add_instr(emit, Vararg_end);
    instr->m_val = offset;
    instr->m_val2 = emit->env->func->variadic->m_val2;
    emit->env->func->variadic->m_val2 = vector_size(&emit->code->code);
    *(m_uint*)emit->env->func->variadic->ptr = 1;
  } else if(!strcmp(s_name(member->xid), "i")) {
    Instr instr = add_instr(emit, Vararg_int);
    instr->m_val = offset;
  } else if(!strcmp(s_name(member->xid), "f") || !strcmp(s_name(member->xid), "t") || !strcmp(s_name(member->xid), "d")) {
    Instr instr = add_instr(emit, Vararg_float);
    instr->m_val = offset;
  } else if(!strcmp(s_name(member->xid), "c") || !strcmp(s_name(member->xid), "p")) {
    Instr instr = add_instr(emit, Vararg_complex);
    instr->m_val = offset;
  } else if(!strcmp(s_name(member->xid), "v3")) {
    Instr instr = add_instr(emit, Vararg_Vec3);
    instr->m_val = offset;
  } else if(!strcmp(s_name(member->xid), "v4")) {
    Instr instr = add_instr(emit, Vararg_Vec4);
    instr->m_val = offset;
  } else if(!strcmp(s_name(member->xid), "o")) {
    Instr instr = add_instr(emit, Vararg_object);
    instr->m_val = offset;
  }
  return 1;
}

static m_bool emit_exp_dot_special(Emitter emit, Exp_Dot* member) {
  Type t = member->t_base;
  Value v = find_value(t, member->xid);
  m_bool emit_addr = member->self->emit_var;

  if(t->xid == te_complex || t->xid == te_polar)
    return emit_complex_member(emit, member->base, v,
        t->xid == te_complex ? "re" : "mod", emit_addr);
  else if(t->xid == te_vec3 || t->xid == te_vec4)
    return emit_vec_member(emit, member->base, v, emit_addr);
  return emit_vararg(emit, member);
}

static m_bool emit_dot_static_func(Emitter emit, Type type, Func func) {
  Instr push_i = add_instr(emit, Reg_Push_Imm);
  Instr func_i = add_instr(emit, Dot_Static_Func);
  push_i->m_val = (m_uint)type;
  func_i->m_val = (m_uint)func;
  return 1;
}

static m_bool emit_member_func(Emitter emit, Exp_Dot* member, Func func) {
  if(GET_FLAG(func, ae_flag_member)) { // member
    Instr func_i;
    if(emit_exp(emit, member->base, 0) < 0)
      CHECK_BB(err_msg(EMIT_, member->pos, "... in member function")) // LCOV_EXCL_LINE
    sadd_instr(emit, Reg_Dup_Last);
    func_i = add_instr(emit, Exp_Dot_Func);
    func_i->m_val = func->vt_index;
  }
  return 1;
}

static m_bool emit_member(Emitter emit, Value v, m_bool emit_addr) {
  Instr func_i = add_instr(emit, Exp_Dot_Data);
  func_i->m_val = v->offset;
  func_i->m_val2 = kindof(v->m_type);
  *(m_uint*)func_i->ptr = emit_addr;
  return 1;
}

static m_bool emit_exp_dot_instance(Emitter emit, Exp_Dot* member) {
  Type t_base = member->t_base;
  Value value = find_value(t_base, member->xid);
  m_bool emit_addr = member->self->emit_var;
  if(isa(member->self->type, &t_func_ptr) > 0) { // function poin ter
    if(GET_FLAG(value, ae_flag_member)) { // member
      if(emit_exp(emit, member->base, 0) < 0)
        CHECK_BB(err_msg(EMIT_, member->pos, "... in member function")) // LCOV_EXCL_LINE
      sadd_instr(emit, Reg_Dup_Last);
      return emit_member(emit, value, emit_addr);
    } else
      return emit_dot_static_data(emit, value, kindof(value->m_type), emit_addr);
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
      CHECK_BB(emit_dot_static_import_data(emit, value, t_base, emit_addr))
  }
  return 1;
}

static m_bool emit_exp_dot_static(Emitter emit, Exp_Dot* member) {
  Type t_base = member->t_base->d.actual_type;
  Value value = find_value(t_base, member->xid);

  if(isa(member->self->type, &t_function) > 0)
    return emit_dot_static_func(emit, t_base, value->func_ref);
  return emit_dot_static_import_data(emit, value, t_base, member->self->emit_var);
}

static m_bool emit_exp_dot(Emitter emit, Exp_Dot* member) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "dot member");
#endif
  if(is_special(member->t_base) > 0)
    return emit_exp_dot_special(emit, member);
  if(member->t_base->xid != te_class)
    return emit_exp_dot_instance(emit, member);
  return emit_exp_dot_static(emit, member);
}

static m_bool emit_func_def_global(Emitter emit, Value value) {
  Local* local = frame_alloc_local(emit->code->frame, value->m_type->size, 1, 0);
  Instr set_mem = add_instr(emit, Mem_Set_Imm);
  set_mem->m_val = value->offset = local->offset;
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
    if(!frame_alloc_local(emit->code->frame, SZ_INT, 1, 0))
      CHECK_BB(err_msg(EMIT_, func->def->pos, "(emit): internal error: cannot allocate local 'this'...")) // LCOV_EXCL_LINE
  }
  if(GET_FLAG(func->def, ae_flag_variadic)) {
    if(!frame_alloc_local(emit->code->frame, SZ_INT, 1, 0))
      CHECK_BB(err_msg(EMIT_, func->def->pos, "(emit): internal error: cannot allocate local 'vararg'...")) // LCOV_EXCL_LINE
      emit->code->stack_depth += SZ_INT;
  }
  return 1;
}

static m_bool emit_func_def_args(Emitter emit, Arg_List a) {
  while(a) {
    Value value = a->var_decl->value;
    Type type = value->m_type;
    Local* local;
    m_bool obj = !isprim(type);
    m_bool ref = a->type_decl->ref;
    emit->code->stack_depth += type->size;

    if(!(local = frame_alloc_local(emit->code->frame, type->size, ref, obj)))
      CHECK_BB(err_msg(EMIT_, a->pos,
        "(emit): internal error: cannot allocate local '%s'...", value->name))
    value->offset = local->offset;
    a = a->next;
  }
  return 1;
}

static m_bool emit_func_def_ensure(Emitter emit, m_uint size) {
  Instr instr;
  if(!size)
    return 1;
  instr = add_instr(emit, Reg_Push_ImmX);
  instr->m_val = size;
  emit_func_release(emit); // /04/04/2017
  vector_add(&emit->code->stack_return, (vtype)add_instr(emit, Goto));
  return 1;
}

static m_bool emit_func_def_return(Emitter emit) {
  m_uint i;
  for(i = 0; i < vector_size(&emit->code->stack_return); i++) {
    Instr instr = (Instr)vector_at(&emit->code->stack_return, i);
    instr->m_val = vector_size(&emit->code->code);
  }
  vector_clear(&emit->code->stack_return);
  sadd_instr(emit, Func_Return);
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

static m_bool emit_func_def(Emitter emit, Func_Def func_def) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "func def");
#endif
  Func func = func_def->d.func;

  if(func_def->types) { // don't check template definition
    func_def->flag &= ~ae_flag_template;
    return 1;
  }
  if(!emit->env->class_def)
    CHECK_BB(emit_func_def_global(emit, func->value_ref))
  CHECK_BB(emit_func_def_init(emit, func))
  CHECK_BB(emit_func_def_flag(emit, func))


  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_func_def_args(emit, func_def->arg_list))
  CHECK_BB(emit_stmt(emit, func_def->code, 0))
  CHECK_BB(emit_func_def_ensure(emit, func_def->ret_type->size))
  emit_pop_scope(emit);

  if(GET_FLAG(func_def, ae_flag_variadic) && (!emit->env->func->variadic ||
      !*(m_uint*)emit->env->func->variadic->ptr))
    CHECK_BB(err_msg(EMIT_, func_def->pos, "invalid variadic use"))
  CHECK_BB(emit_func_def_return(emit))
  CHECK_BB(emit_func_def_code(emit, func))
  emit->env->func = NULL;
  emit->code = (Code*)vector_pop(&emit->stack);
  return 1;
}

static m_bool emit_class_def(Emitter emit, Class_Def class_def) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "class def");
#endif
  Type type = class_def->type;
  m_bool ret = 1;
  Class_Body body = class_def->body;
  char c[strlen(type->name) + 7];

  if(type->info->class_data_size) {
    type->info->class_data = calloc(type->info->class_data_size, sizeof(char));
    if(!type->info->class_data)
      CHECK_BB(err_msg(EMIT_, class_def->pos, "OutOfMemory: while allocating static data '%s'\n", type->name)) // LCOV_EXCL_LINE
    }
  memset(type->info->class_data, 0, type->info->class_data_size);
  vector_add(&emit->env->class_stack, (vtype)emit->env->class_def);
  emit->env->class_def = type;
  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code();
  sprintf(c, "class %s", type->name);
  emit->code->name = strdup(c);

  emit->code->need_this = 1;
  emit->code->filename = strdup(emit->filename);
  emit->code->stack_depth += SZ_INT;
  if(!frame_alloc_local(emit->code->frame, SZ_INT, 1, 1))
    CHECK_BB(err_msg(EMIT_, class_def->pos, "internal error: cannot allocate local 'this'...")) // LCOV_EXCL_LINE

    while(body && ret > 0) {
      switch(body->section->type) {
        case ae_section_stmt:
          ret = emit_stmt_list(emit, body->section->d.stmt_list);
          break;
        case ae_section_func:
          ret = emit_func_def(emit, body->section->d.func_def);
          break;
        case ae_section_class:
          ret = emit_class_def(emit, body->section->d.class_def);
          break;
      }
      body = body->next;
    }

  if(ret > 0) {
    sadd_instr(emit, Func_Return);
    free_vm_code(type->info->pre_ctor);
    type->info->pre_ctor = emit_code(emit);
  } else
    free(type->info->class_data); // LCOV_EXCL_LINE
  emit->env->class_def = (Type)vector_pop(&emit->env->class_stack);
  emit->code = (Code*)vector_pop(&emit->stack);
  return ret;
}

m_bool emit_ast(Emitter emit, Ast ast, m_str filename) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "Ast %p", emit->env->context);
#endif
  Ast prog = ast;
  vtype i;
  int ret = 1;
  emit->filename = filename;
  emit->code = new_code();
  vector_clear(&emit->stack);
  frame_push_scope(emit->code->frame);
  sadd_instr(emit, start_gc);
  while(prog && ret > 0) {
    if(!prog->section)
      return 1;
    switch(prog->section->type) {
      case ae_section_stmt:
        ret = emit_stmt_list(emit, prog->section->d.stmt_list);
        break;
      case ae_section_func:
        ret = emit_func_def(emit, prog->section->d.func_def);
        break;
      case ae_section_class:
        ret = emit_class_def(emit, prog->section->d.class_def);
        break;
    }
    prog = prog->next;
  }
  sadd_instr(emit, stop_gc);
  if(emit->cases)
    free_map(emit->cases);
  emit_pop_scope(emit);
  if(ret < 0) { // should free all stack.
    //    for(i = 0; i < vector_size(&emit->stack); i++)
    //      free_code((Code*)vector_at(&emit->stack, i));
    for(i = 0; i < vector_size(&emit->code->code); i++)
      free((Instr)vector_at(&emit->code->code, i));
    free(filename);
    free_code(emit->code);
    free_ast(ast);
  }
  return ret;
}
