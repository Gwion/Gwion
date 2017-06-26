#include <string.h> /* memcpy */
#include "err_msg.h"
#include "instr.h"
#include "vm.h"
#include "context.h"
#include "func.h"
#include "emit.h"
#include "code_private.h"

static m_bool emit_exp(Emitter emit, Exp exp, m_bool add_ref);
static m_bool emit_stmt(Emitter emit, Stmt stmt, m_bool pop);
static m_bool emit_stmt_list(Emitter emit, Stmt_List list);
static m_bool emit_exp_dot(Emitter emit, Exp_Dot* member);
static m_bool emit_func_def(Emitter emit, Func_Def func_def);

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
    info->stack_offset = emit->code->frame->curr_offset;
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

static m_bool emit_symbol(Emitter emit, S_Symbol symbol, Value v, int emit_var, int pos) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "symbol %s (const:%i) %i %p", s_name(symbol), GET_FLAG(v, ae_flag_const) + GET_FLAG(v, ae_flag_enum), GET_FLAG(v, ae_flag_static), v->owner_class);
#endif
  Instr instr;

  if(v->owner_class && (GET_FLAG(v, ae_flag_member) || GET_FLAG(v, ae_flag_static))) {
    m_bool ret;
    Exp base = new_exp_prim_ID("this", pos);
    Exp dot = new_exp_dot(base, s_name(symbol), pos);
    base->type = v->owner_class;
    dot->type = v->m_type;
    dot->d.exp_dot.t_base = v->owner_class;
    dot->emit_var = emit_var;
    if((ret = emit_exp_dot(emit, &dot->d.exp_dot)) < 0)
      err_msg(EMIT_, pos, "(emit): internal error: symbol transformation failed..."); // LCOV_EXCL_LINE
    free_expression(dot);
    return ret;
  }

  if(GET_FLAG(v, ae_flag_const)) {
    if(v->func_ref) {
      instr = add_instr(emit, Reg_Push_Imm);
      instr->m_val = (m_uint)v->func_ref;
    } else if(isa(v->m_type, &t_float) > 0 || isa(v->m_type, &t_dur) > 0 || isa(v->m_type, &t_dur) > 0) {
      instr = add_instr(emit, Reg_Push_Imm2);
      *(m_float*)instr->ptr = *(m_float*)v->ptr;
    } else {
      instr = add_instr(emit, Reg_Push_Imm);
      instr->m_val = (emit_var ? (m_uint)&v->ptr : (m_uint)v->ptr);
    }
    return 1;
  }

  if(emit_var) {
    instr = add_instr(emit, Reg_Push_Mem_Addr);
    instr->m_val = v->offset;
    instr->m_val2 = GET_FLAG(v, ae_flag_global);
  } else {
    Kindof kind = kindof(v->m_type);
    Instr instr;
    f_instr f = NULL;
    switch(kind) {
      case Kindof_Int:
        f = Reg_Push_Mem;
        break;
      case Kindof_Float:
        f = Reg_Push_Mem2;
        break;
      case Kindof_Complex:
        f = Reg_Push_Mem_Complex;
        break;
      case Kindof_Vec3:
        f = Reg_Push_Mem_Vec3;
        break;
      case Kindof_Vec4:
        f = Reg_Push_Mem_Vec4;
        break;
      case Kindof_Void:
        break; // unreachable
    }
    instr         = add_instr(emit, f);
    instr->m_val  = v->offset;
    instr->m_val2 = GET_FLAG(v, ae_flag_global);
  }
  return 1;
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
  Array_Sub sub = NULL;
  Exp exp = NULL;
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

static m_bool emit_exp_primary(Emitter emit, Exp_Primary* prim) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "primary");
#endif
  Exp e;
  Instr instr;
  m_uint temp;
  m_float f;

  switch(prim->type) {
    case ae_primary_id:
      if(prim->d.var == insert_symbol("this"))
        sadd_instr(emit, Reg_Push_This);
      else if(prim->d.var == insert_symbol("me"))
        sadd_instr(emit, Reg_Push_Me);
      else if(prim->d.var == insert_symbol("now"))
        sadd_instr(emit, Reg_Push_Now);
      else if(prim->d.var == insert_symbol("false"))
        sadd_instr(emit, Reg_Push_Imm);
      else if(prim->d.var == insert_symbol("true")) {
        instr = add_instr(emit, Reg_Push_Imm);
        instr->m_val = 1;
      } else if(prim->d.var == insert_symbol("maybe"))
        sadd_instr(emit, Reg_Push_Maybe);
      else if(prim->d.var == insert_symbol("null") || prim->d.var == insert_symbol("NULL"))
        sadd_instr(emit, Reg_Push_Imm);
      else
        emit_symbol(emit, prim->d.var, prim->value, prim->self->emit_var, prim->pos);
      break;

    case ae_primary_num:
      memcpy(&temp, &prim->d.num, sizeof(temp));
      instr = add_instr(emit, Reg_Push_Imm);
      instr->m_val = temp;
      break;

    case ae_primary_char:
      instr = add_instr(emit, Reg_Push_Imm);
      instr->m_val = str2char(prim->d.chr, prim->pos);
      break;

    case ae_primary_float:
      memcpy(&f, &prim->d.fnum, sizeof(f));
      instr = add_instr(emit, Reg_Push_Imm2);
      *(m_float*)instr->ptr = f;
      break;

    case ae_primary_complex:
      CHECK_BB(emit_exp(emit, prim->d.cmp->re, 0));
      break;

    case ae_primary_polar:
      CHECK_BB(emit_exp(emit, prim->d.polar->mod, 0));
      break;

    case ae_primary_vec:
      CHECK_BB(emit_exp_prim_vec(emit, prim->d.vec));
      break;

    case ae_primary_str:
      memcpy(&temp, &prim->d.str, sizeof(temp));
      instr = add_instr(emit, Reg_Push_Str);
      instr->m_val = temp;
      break;

    case ae_primary_array:
      CHECK_BB(emit_exp_prim_array(emit, prim->d.array))
      break;

    case ae_primary_nil:
      break;

    case ae_primary_hack:
      CHECK_BB(emit_exp(emit, prim->d.exp, 0))
      Vector types = new_vector();
      e = prim->d.exp;
      while(e) {
        vector_add(types, (vtype)e->type);
        ADD_REF(e->type);
        e = e->next;
      }
      instr = add_instr(emit, Gack);
      *(Vector*)instr->ptr = types;
      break;
  }
  return 1;
}

static m_bool emit_exp_decl(Emitter emit, Exp_Decl* decl) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "decl");
#endif
  Exp_Decl* exp = decl;
  Var_Decl_List list = exp->list;
  f_instr f = NULL;
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
        switch(kind)  {
          case Kindof_Int:
            f = Alloc_Member_Word;
            break;
          case Kindof_Float:
            f = Alloc_Member_Word_Float;
            break;
          case Kindof_Complex:
            f = Alloc_Member_Word_Complex;
            break;
          case Kindof_Vec3:
            f = Alloc_Member_Word_Vec3;
            break;
          case Kindof_Vec4:
            f = Alloc_Member_Word_Vec4;
            break;
          case Kindof_Void:
            break;
        }
        alloc = add_instr(emit, f);
      } else {
        if(!emit->env->class_def || !decl->is_static) {
          Local* local = frame_alloc_local(emit->code->frame, type->size, value->name, is_ref, is_obj);
          CHECK_OB(local)
          value->offset   = local->offset;
          switch(kind)  {
            case Kindof_Int:
              f = Alloc_Word;
              break;
            case Kindof_Float:
              f = Alloc_Word_Float;
              break;
            case Kindof_Complex:
              f = Alloc_Word_Complex;
              break;
            case Kindof_Vec3:
              f = Alloc_Word_Vec3;
              break;
            case Kindof_Vec4:
              f = Alloc_Word_Vec4;
              break;
            case Kindof_Void:
              break;
          }
          alloc   = add_instr(emit, f);
          alloc->m_val2 = GET_FLAG(value, ae_flag_global);
        } else { // static
          Code* code = emit->code;
          if(is_obj && !is_ref) {
            emit->code = (Code*)vector_back(&emit->stack);
            CHECK_BB(emit_instantiate_object(emit, type, array, is_ref))
            Instr push = add_instr(emit, Reg_Push_Imm);
            push->m_val = (m_uint)emit->env->class_def;
            alloc = add_instr(emit, Dot_Static_Data);
            alloc->m_val2 = kindof(type); // was (erroneously I think) kindof(emit->env->class_def);
            *(m_uint*)alloc->ptr = 1;
            alloc->m_val = value->offset;
            Instr assign  = add_instr(emit, Assign_Object);
            assign->m_val = 0;
            /*add_instr(emit, Reg_AddRef_Object3);*/
            emit->code = code;
          }
          Instr push = add_instr(emit, Reg_Push_Imm);
          push->m_val = (m_uint)emit->env->class_def;
          alloc = add_instr(emit, Dot_Static_Data);
          alloc->m_val2 = kindof(type); // was (erroneously I think) kindof(emit->env->class_def);
          *(m_uint*)alloc->ptr = 1;
          alloc->m_val = value->offset;
          list = list->next;
          continue;
        }
      }
    alloc->m_val = value->offset;
    *(m_uint*)alloc->ptr = ((is_ref && !array) || isprim(type) > 0)  ? decl->self->emit_var : 1;
    if(is_obj) {
      if(array && array->exp_list) {
        Instr assign = add_instr(emit, Assign_Object);
        assign->m_val = decl->self->emit_var;
        ADD_REF(type);
      } else if(!is_ref) {
        Instr assign  = add_instr(emit, Assign_Object);
        assign->m_val = decl->self->emit_var;
      }
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

static m_bool emit_exp_call(Emitter emit, Exp_Func* exp_func, m_bool spork) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "func call");
#endif
  if(exp_func->types) {
    Func_Def def = exp_func->m_func->def;
    ID_List base_t = def->base;
    Type_List list = exp_func->types;
    if(exp_func->m_func->value_ref->owner_class) {
      vector_add(&emit->env->nspc_stack, (vtype)emit->env->curr);
      emit->env->curr = exp_func->m_func->value_ref->owner_class->info;
//      emit->env->curr = exp_func->m_func->value_ref->owner;
      vector_add(&emit->env->class_stack, (vtype)emit->env->class_def);
      emit->env->class_def = exp_func->m_func->value_ref->owner_class;
//      emit->env->class_scope = 0; // check utility
    }
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

    if(exp_func->args && !spork && emit_func_args(emit, exp_func) < 0)
      CHECK_BB(err_msg(EMIT_, exp_func->pos, "internal error in evaluating function arguments...")) // LCOV_EXCL_LINE
      if(emit_exp(emit, exp_func->func, 0) < 0)
        CHECK_BB(err_msg(EMIT_, exp_func->pos, "internal error in evaluating function call...")) // LCOV_EXCL_LINE
        if(GET_FLAG(exp_func->m_func->def, ae_flag_variadic) && !exp_func->args) { // handle empty call to variadic functions
          sadd_instr(emit, MkVararg);
          sadd_instr(emit, Reg_Push_Imm);
        }
    nspc_pop_type(emit->env->curr);
    if(exp_func->m_func->value_ref->owner_class) {
      emit->env->class_def = (Type)vector_pop(&emit->env->class_stack);
      emit->env->curr = (Nspc)vector_pop(&emit->env->nspc_stack);
    }
  } else {
    if(exp_func->args && !spork && emit_func_args(emit, exp_func) < 0)
      CHECK_BB(err_msg(EMIT_, exp_func->pos, "internal error in evaluating function arguments...")) // LCOV_EXCL_LINE
      if(emit_exp(emit, exp_func->func, 0) < 0)
        CHECK_BB(err_msg(EMIT_, exp_func->pos, "internal error in evaluating function call...")) // LCOV_EXCL_LINE
        if(GET_FLAG(exp_func->m_func->def, ae_flag_variadic) && !exp_func->args) { // handle empty call to variadic functions
          sadd_instr(emit, MkVararg);
          sadd_instr(emit, Reg_Push_Imm);
        }
  }
  return emit_exp_call1(emit, exp_func->m_func, exp_func->ret_type, exp_func->pos);
}

static m_bool emit_exp_binary(Emitter emit, Exp_Binary* binary) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "binary");
#endif
  Instr instr;
  // function pointer assignement
  if(binary->op == op_at_chuck && isa(binary->lhs->type, &t_function) > 0 && isa(binary->rhs->type, &t_func_ptr) > 0) {
    Value v;
    CHECK_BB(emit_exp(emit, binary->lhs, 1))
    CHECK_BB(emit_exp(emit, binary->rhs, 1))
    instr = add_instr(emit, assign_func);
    switch(binary->rhs->exp_type) {
      case ae_exp_dot:
        v = find_value(binary->rhs->d.exp_dot.t_base, binary->rhs->d.exp_dot.xid);
        instr->m_val2 = v->offset;
        instr->m_val = 1;
        break;
      case ae_exp_primary:
        if(GET_FLAG(binary->rhs->d.exp_primary.value, ae_flag_member)) {
          v = binary->rhs->d.exp_primary.value;
          instr->m_val2 = v->offset;
          instr->m_val = 1;
        }
        break;
      case ae_exp_decl:
        v = binary->rhs->d.exp_decl.list->self->value;
        instr->m_val2 = v->offset;
        break;
      default:
        return -1;
    }
    return 1;
  }
  CHECK_BB(emit_exp(emit, binary->lhs, 1))
  CHECK_BB(emit_exp(emit, binary->rhs, 1))

  if(binary->op == op_chuck && isa(binary->rhs->type, &t_function) > 0)
    return emit_exp_call1(emit, binary->func, binary->func->value_ref->m_type, binary->pos);

  // arrays
  if(binary->op == op_shift_left && (binary->lhs->type->array_depth == binary->rhs->type->array_depth + 1)
      && isa(binary->lhs->type->d.array_type, binary->rhs->type) > 0) {
    instr = add_instr(emit, Array_Append);
    instr->m_val = kindof(binary->rhs->type);
    return 1;
  }

  if(binary->lhs->type->array_depth && binary->rhs->type->array_depth) {
    if(binary->op == op_at_chuck && binary->lhs->type->array_depth == binary->rhs->type->array_depth)
      sadd_instr(emit, Assign_Object);
    return 1;
  }

  if(binary->op == op_at_chuck && isa(binary->rhs->type, &t_object) > 0 &&
      (isa(binary->lhs->type, &t_null) > 0 || isa(binary->lhs->type, &t_object) > 0)) {
    instr = add_instr(emit, Assign_Object);
    instr->m_val2 = 1;
    return 1;
  }
  CHECK_OB(get_instr(emit, binary->op, binary->lhs->type, binary->rhs->type))
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
  else if(to->xid == t_int.xid && from->xid == t_float.xid)
    f = Cast_f2i;
  else if(to->xid == t_float.xid && from->xid == t_int.xid)
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

/* static */ m_bool emit_exp_call1(Emitter emit, Func func, Type type, int pos) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "func call1. '%s' offset: %i", func->name, emit->code->frame->curr_offset);
#endif
  m_bool is_ptr = 0;
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
      is_ptr = !func->value_ref->owner_class;
    }
  } else {
    code = add_instr(emit, Reg_Push_Ptr);
    *(VM_Code*)code->ptr = func->code;
  }

  if(!emit->code->stack_depth && !emit->code->frame->curr_offset)
    sadd_instr(emit, Mem_Push_Imm);

  offset = add_instr(emit, Reg_Push_Imm);
  offset->m_val = emit->code->frame->curr_offset;
  call = add_instr(emit, Instr_Exp_Func);
  call->m_val = GET_FLAG(func->def, ae_flag_builtin) ? kindof(func->def->ret_type) : emit->code->stack_depth;
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
  if(is_ptr)
    free(func);
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

  Exp e = exp->args;
  m_uint size = 0;
  while(e) {
    size += e->cast_to ? e->cast_to->size : e->type->size;
    e = e->next;
  }
  CHECK_BB(emit_exp_spork_finish(emit, code, NULL, size, 0))
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
  if(from->xid == t_int.xid && to->xid == t_float.xid)
    sadd_instr(emit, Cast_i2f);
  else if(from->xid == t_float.xid && to->xid == t_int.xid)
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

  if(stmt->c2)
    op->m_val = vector_size(&emit->code->code);
  emit_pop_stack(emit, action_index);
  return 1;
}

static m_bool emit_stmt_loop(Emitter emit, Stmt_Loop stmt) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "loop");
#endif
  Instr init, op = NULL, deref, dec = NULL, _goto = NULL;
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
  m_uint value;
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
          err_msg(EMIT_, stmt->val->d.exp_primary.pos, "'maybe' is not constant.");
          return - 1;
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
    } else {
      err_msg(EMIT_, stmt->pos, "unhandled expression type '%i'", stmt->val->exp_type);
      return -1;
    }
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
      CHECK_OB((local = frame_alloc_local(emit->code->frame, sizeof(m_uint), v->name, 0, 0)))
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
    Local* local = frame_alloc_local(emit->code->frame, stmt->s, "union", 1, 0);
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

static m_bool emit_stmt(Emitter emit, Stmt stmt, m_bool pop) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "Stmt (pop: %s)", pop ? "yes" : "no");
#endif
  Instr instr;
  m_bool ret = 1;
  if(!stmt)
    return 1;
  switch(stmt->type) {
    case ae_stmt_exp:
      if(!stmt->d.stmt_exp.val)
        return 1;
      ret = emit_exp(emit, stmt->d.stmt_exp.val, 0);
      if(ret > 0 && pop && stmt->d.stmt_exp.val->type && stmt->d.stmt_exp.val->type->size > 0) {
        Exp exp = stmt->d.stmt_exp.val;
        if(exp->exp_type == ae_exp_primary && exp->d.exp_primary.type == ae_primary_hack)
          exp = exp->d.exp_primary.d.exp;
        while(exp) {
          instr = add_instr(emit, Reg_Pop_Word4);
          instr->m_val = (exp->exp_type == ae_exp_decl ? exp->d.exp_decl.num_decl * exp->type->size : exp->type->size);
          exp = exp->next;
        }
      }
      break;
    case ae_stmt_code:
      ret = emit_stmt_code(emit, &stmt->d.stmt_code, 1);
      break;
    case ae_stmt_if:
      ret = emit_stmt_if(emit, &stmt->d.stmt_if);
      break;
    case ae_stmt_return:
      ret = emit_stmt_return(emit, &stmt->d.stmt_return);
      break;
    case ae_stmt_break:
      ret = emit_stmt_break(emit, &stmt->d.stmt_break);
      break;
    case ae_stmt_continue:
      ret = emit_stmt_continue(emit, &stmt->d.stmt_continue);
      break;
    case ae_stmt_while:
      ret = stmt->d.stmt_while.is_do ? emit_stmt_do_while(emit, &stmt->d.stmt_while) :
            emit_stmt_while(emit, &stmt->d.stmt_while);
      break;
    case ae_stmt_until:
      ret = stmt->d.stmt_until.is_do ? emit_stmt_do_until(emit, &stmt->d.stmt_until) :
            emit_stmt_until(emit, &stmt->d.stmt_until);
      break;
    case ae_stmt_for:
      ret = emit_stmt_for(emit, &stmt->d.stmt_for);
      break;
    case ae_stmt_loop:
      ret = emit_stmt_loop(emit, &stmt->d.stmt_loop);
      break;
    case ae_stmt_gotolabel:
      ret = emit_stmt_gotolabel(emit, &stmt->d.stmt_gotolabel);
      break;
    case ae_stmt_case:
      ret = emit_stmt_case(emit, &stmt->d.stmt_case);
      break;
    case ae_stmt_enum:
      ret = emit_stmt_enum(emit, &stmt->d.stmt_enum);
      break;
    case ae_stmt_switch:
      ret = emit_stmt_switch(emit, &stmt->d.stmt_switch);
      break;
    case ae_stmt_funcptr:
      ret = emit_stmt_typedef(emit, &stmt->d.stmt_ptr);
      break;
    case ae_stmt_union:
      ret = emit_stmt_union(emit, &stmt->d.stmt_union);
      break;
  }
#ifdef DEBUG_EMIT
  debug_msg("emit", "Stmt %i", ret);
#endif
  return ret;
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

static m_bool emit_exp_dot(Emitter emit, Exp_Dot* member) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "dot member");
#endif
  Instr instr, func_i, push_i;
  Type t_base;
  m_uint emit_addr = member->self->emit_var;
  m_bool base_static = member->t_base->xid == t_class.xid;
  Func func = NULL;
  Value value = NULL;
  m_uint offset = 0;

  t_base = base_static ? member->t_base->d.actual_type : member->t_base;
  if(t_base->xid == t_complex.xid) {
    if(member->base->meta == ae_meta_var)
      member->base->emit_var = 1;
    CHECK_BB(emit_exp(emit, member->base, 0))
    value = find_value(t_base, member->xid);
    if(!strcmp(value->name, "re"))
      instr = add_instr(emit, complex_real);
    else
      instr = add_instr(emit, complex_imag);
    instr->m_val = emit_addr;
    return 1;
  } else if(t_base->xid == t_polar.xid) {
    if(member->base->meta == ae_meta_var)
      member->base->emit_var = 1;
    CHECK_BB(emit_exp(emit, member->base, 0))
    value = find_value(t_base, member->xid);
    if(!strcmp(value->name, "mod"))
      instr = add_instr(emit, complex_real);
    else
      instr = add_instr(emit, complex_imag);
    instr->m_val = emit_addr;
    return 1;
  } else if(t_base->xid == t_vec3.xid) {
    Instr instr;
    if(member->base->meta == ae_meta_var)
      member->base->emit_var = 1;
    CHECK_BB(emit_exp(emit, member->base, 0))
    value = find_value(t_base, member->xid);
    if(!strcmp(value->name, "x"))
      instr = add_instr(emit, vec3_x);
    else if(!strcmp(value->name, "y"))
      instr = add_instr(emit, vec3_y);
    else if(!strcmp(value->name, "z"))
      instr = add_instr(emit, vec3_z);
    else {
      sadd_instr(emit, Reg_Dup_Last_Vec3);
      instr = add_instr(emit, member_function);
      *(Vector*)instr->ptr = &t_base->info->obj_v_table;
      instr->m_val = value->func_ref->vt_index;
      return 1;
    }
    instr->m_val = emit_addr;
    return 1;
  } else if(t_base->xid == t_vec4.xid) {
    Instr instr;
    if(member->base->meta == ae_meta_var)
      member->base->emit_var = 1;
    CHECK_BB(emit_exp(emit, member->base, 0))
    value = find_value(t_base, member->xid);
    if(!strcmp(value->name, "x"))
      instr = add_instr(emit, vec4_x);
    else if(!strcmp(value->name, "y"))
      instr = add_instr(emit, vec4_y);
    else if(!strcmp(value->name, "z"))
      instr = add_instr(emit, vec4_z);
    else if(!strcmp(value->name, "w"))
      instr = add_instr(emit, vec4_w);
    else {
      sadd_instr(emit, Reg_Dup_Last_Vec4);
      instr = add_instr(emit, member_function);
      *(Vector*)instr->ptr = &t_base->info->obj_v_table;
      instr->m_val = value->func_ref->vt_index;
      return 1;
    }
    instr->m_val = emit_addr;
    return 1;
  }
  if(t_base->xid == t_vararg.xid) {
    m_uint offset = 0;
    Arg_List l = emit->env->func->def->arg_list;
    while(l) {
      offset += l->type->size;
      l = l->next;
    }
    if(!strcmp(s_name(member->xid), "start")) {
      if(emit->env->func->variadic->instr) {
        free(emit->env->func->variadic->instr);
        CHECK_BB(err_msg(EMIT_, 0, "vararg.start already used. this is an error"))
      }
      emit->env->func->variadic->instr = add_instr(emit, Vararg_start);
      emit->env->func->variadic->instr->m_val = offset;
      emit->env->func->variadic->instr->m_val2 = vector_size(&emit->code->code);
    }
    if(!strcmp(s_name(member->xid), "end")) {
      if(!emit->env->func->variadic->instr)
        CHECK_BB(err_msg(EMIT_, 0, "vararg.start not used before vararg.end. this is an error"))
        Instr instr = add_instr(emit, Vararg_end);
      instr->m_val = offset;
      instr->m_val2 = emit->env->func->variadic->instr->m_val2;
      emit->env->func->variadic->instr->m_val2 = vector_size(&emit->code->code);
      *(m_uint*)emit->env->func->variadic->instr->ptr = 1;
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
  if(!base_static) { // called from instance
    if(isa(member->self->type, &t_func_ptr) > 0) { // function pointer
      value = find_value(t_base, member->xid);
      if(GET_FLAG(value, ae_flag_member)) { // member
        if(emit_exp(emit, member->base, 0) < 0)
          CHECK_BB(err_msg(EMIT_, member->pos, "... in member function")) // LCOV_EXCL_LINE
          sadd_instr(emit, Reg_Dup_Last);
        func_i = add_instr(emit, Exp_Dot_Data);
        func_i->m_val = value->offset;
        func_i->m_val2 = Kindof_Int;
        *(m_uint*)func_i->ptr = emit_addr;
        return 1;
      } else {
        push_i = add_instr(emit, Reg_Push_Imm);
        func_i = add_instr(emit, Dot_Static_Data);
        push_i->m_val = (m_uint)t_base;
        func_i->m_val = (m_uint)offset;
        func_i->m_val2 = (m_uint)kindof(value->m_type);
        *(m_uint*)func_i->ptr = emit_addr;
      }
    } else if(isa(member->self->type, &t_function) > 0) { // function
      value = find_value(t_base, member->xid);
      func = value->func_ref;
      if(GET_FLAG(func, ae_flag_member)) { // member
        if(emit_exp(emit, member->base, 0) < 0)
          CHECK_BB(err_msg(EMIT_, member->pos, "... in member function")) // LCOV_EXCL_LINE
          sadd_instr(emit, Reg_Dup_Last);
        func_i = add_instr(emit, Exp_Dot_Func);
        func_i->m_val = func->vt_index;
      } else { // static
        push_i = add_instr(emit, Reg_Push_Imm);
        push_i->m_val = (m_uint)t_base;
        func_i = add_instr(emit, Dot_Static_Func);
        func_i->m_val = (m_uint)func;
      }
    } else { // variable
      value = find_value(t_base, member->xid);
      offset = value->offset;
      if(GET_FLAG(value, ae_flag_member)) { // member
        CHECK_BB(emit_exp(emit, member->base, 0))
        instr = add_instr(emit, Exp_Dot_Data);
        instr->m_val = offset;
        instr->m_val2 = kindof(value->m_type);
        *(m_uint*)instr->ptr = emit_addr;
      } else { // static
        if(value->ptr && GET_FLAG(value, ae_flag_builtin)) { // from C
          func_i = add_instr(emit, Dot_Static_Import_Data);
          func_i->m_val = (m_uint)value->ptr;
          func_i->m_val2 = kindof(value->m_type);
          *(m_uint*)func_i->ptr = emit_addr;
        } else { // from code
          push_i = add_instr(emit, Reg_Push_Imm);
          func_i = add_instr(emit, Dot_Static_Data);
          push_i->m_val = (m_uint)t_base;
          func_i->m_val = (m_uint)offset;
          func_i->m_val2 = (m_uint)kindof(value->m_type);
          *(m_uint*)func_i->ptr = emit_addr;
        }
      }
    }
  } else { // static
    if(isa(member->self->type, &t_function) > 0) {
      value = find_value(t_base, member->xid);
      func = value->func_ref;
      push_i = add_instr(emit, Reg_Push_Imm);
      func_i = add_instr(emit, Dot_Static_Func);
      push_i->m_val = (m_uint)t_base;
      func_i->m_val = (m_uint)func;
    } else {
      value = find_value(t_base, member->xid);
      if(value->ptr && GET_FLAG(value, ae_flag_builtin)) {
        func_i = add_instr(emit, Dot_Static_Import_Data);
        func_i->m_val = (m_uint)value->ptr;
        func_i->m_val2 = kindof(value->m_type);
        *(m_uint*)func_i->ptr = emit_addr;
      } else {
        push_i = add_instr(emit, Reg_Push_Imm);
        func_i = add_instr(emit, Dot_Static_Data);
        push_i->m_val = (m_uint)t_base;
        func_i->m_val = (m_uint)value->offset;
        func_i->m_val2 = (m_uint)kindof(value->m_type);
        *(m_uint*)func_i->ptr = emit_addr;
      }
    }
  }
  return 1;
}

static m_bool emit_func_def(Emitter emit, Func_Def func_def) {
#ifdef DEBUG_EMIT
  debug_msg("emit", "func def");
#endif
  Func func = func_def->d.func;
  Value value = func->value_ref;
  Type type = value->m_type;
  Local* local = NULL;
  Arg_List a = func_def->arg_list;
  char c[(emit->env->class_def ? strlen(emit->env->class_def->name) + 1 : 0) + strlen(func->name) + 6];
  m_bool is_obj = 0;
  m_bool is_ref = 0;

  if(func->code)
    CHECK_BB(err_msg(EMIT_, func_def->pos, "function '%s' already emitted...", s_name(func_def->name))) // LCOV_EXCL_LINE

    if(func_def->types) { // don't check template definition
      func_def->flag &= ~ae_flag_template;
      return 1;
    }

  if(!emit->env->class_def) {
    local = frame_alloc_local(emit->code->frame, value->m_type->size, value->name, 1, 0);
    value->offset = local->offset;
    Instr set_mem = add_instr(emit, Mem_Set_Imm);
    set_mem->m_val = value->offset;
    *(Func*)set_mem->ptr = func;
  }

  emit->env->func = func;
  vector_add(&emit->stack, (vtype)emit->code);
  emit->code = new_code();
  sprintf(c, "%s%s%s(...)", emit->env->class_def ? emit->env->class_def->name : "", emit->env->class_def ? "." : "", func->name);
  emit->code->name = strdup(c);
  emit->code->need_this = GET_FLAG(func, ae_flag_member);
  emit->code->filename = strdup(emit->filename);

  if(GET_FLAG(func, ae_flag_member)) {
    emit->code->stack_depth += SZ_INT;
    if(!frame_alloc_local(emit->code->frame, SZ_INT, "this", 1, 0))
      CHECK_BB(err_msg(EMIT_, a->pos, "(emit): internal error: cannot allocate local 'this'...")) // LCOV_EXCL_LINE
    }

  frame_push_scope(emit->code->frame);
  while(a) {
    value = a->var_decl->value;
    type = value->m_type;
    is_obj = !isprim(type);
    is_ref = a->type_decl->ref;
    emit->code->stack_depth += type->size;

    local = frame_alloc_local(emit->code->frame, type->size, value->name, is_ref, is_obj);

    if(!local) {
      err_msg(EMIT_, a->pos, "(emit): internal error: cannot allocate local '%s'...", value->name); // LCOV_EXCL_START
      goto error;
    }                                                                                               // LCOV_EXCL_STOP
    value->offset = local->offset;
    a = a->next;
  }
  if(GET_FLAG(func_def, ae_flag_variadic)) {
    if(!frame_alloc_local(emit->code->frame, type->size, "vararg", is_ref, is_obj))
      CHECK_BB(err_msg(EMIT_, func_def->pos, "(emit): internal error: cannot allocate local 'vararg'...")) // LCOV_EXCL_LINE
      emit->code->stack_depth += SZ_INT;
  }

  if(emit_stmt(emit, func_def->code, 0) < 0)
    goto error;

  // ensure return
  if(func_def->ret_type && func_def->ret_type->xid != t_void.xid) {
    Instr instr = add_instr(emit, Reg_Push_ImmX);
    instr->m_val = func_def->ret_type->size;
    emit_func_release(emit); // /04/04/2017
    vector_add(&emit->code->stack_return, (vtype)add_instr(emit, Goto));
  }
  emit_pop_scope(emit);

  if(GET_FLAG(func_def, ae_flag_variadic) && (!emit->env->func->variadic->instr ||
      !*(m_uint*)emit->env->func->variadic->instr->ptr))
    CHECK_BB(err_msg(EMIT_, func_def->pos, "invalid variadic use"))
    m_uint i;
  for(i = 0; i < vector_size(&emit->code->stack_return); i++) {
    Instr instr = (Instr)vector_at(&emit->code->stack_return, i);
    instr->m_val = vector_size(&emit->code->code);
  }
  vector_clear(&emit->code->stack_return);
  sadd_instr(emit, Func_Return);
  func->code = emit_code(emit);
  if(GET_FLAG(func->def, ae_flag_dtor)) {
    emit->env->class_def->info->dtor = func->code;
    SET_FLAG(emit->env->class_def, ae_flag_dtor);
  } else if(GET_FLAG(func->def, ae_flag_op))
    operator_set_func(emit->env, func, func->def->arg_list->type, func->def->arg_list->next->type);
  emit->env->func = NULL;
  emit->code = (Code*)vector_pop(&emit->stack);
  return 1;
error:
  free_code(emit->code);
  emit->code = (Code*)vector_pop(&emit->stack);
  return -1;
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
  if(!frame_alloc_local(emit->code->frame, SZ_INT, "this", 1, 1))
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
