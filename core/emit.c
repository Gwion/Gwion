#include <string.h> /* memcpy */
#include "emit.h"
#include "err_msg.h"
#include "instr.h"
#include "vm.h"
#include "context.h"

static m_str emit_filename;

void free_Expression(Expression a); // absyn.h

static m_bool emit_Expression(Emitter emit, Expression exp, m_bool add_ref);
static m_bool emit_Stmt(Emitter emit, Stmt stmt, m_bool pop);
static m_bool emit_Dot_Member(Emitter emit, Dot_Member* member);
static m_bool emit_Stmt_List(Emitter emit, Stmt_List list);
/* static */ m_bool emit_Func_Call1(Emitter emit, Func func, Type type, int pos);
static m_bool emit_Stmt_Code(Emitter emit, Stmt_Code stmt, m_bool push);

static m_bool emit_Func_Def(Emitter emit, Func_Def func_def);
/* static */ Instr new_Instr()
{
  return (Instr)calloc(1, sizeof(struct Instr_));
}

// not static because used in operator.c
Instr add_instr(Emitter emit, f_instr f)
{
  Instr instr = new_Instr();
  instr->execute = f;
  vector_append(emit->code->code, (vtype)instr);
  return instr;
}

static void sadd_instr(Emitter emit, f_instr f)
{
  Instr instr = new_Instr();
  instr->execute = f;
  vector_append(emit->code->code, (vtype)instr);
}

Emitter new_Emitter(Env env)
{
  Emitter emit = calloc(1, sizeof(struct Emitter_));
  emit->env = NULL;
  emit->code = NULL;
  emit->stack = new_Vector();
  emit->spork = new_Vector();
  emit->funcs = new_Vector();
  emit->array = new_Vector();
  emit->context = NULL;
  emit->nspc = NULL;
  emit->func = NULL;
  emit->cases = NULL;
  emit->env = env;
  INIT_OO(emit, e_emit_obj);
  return emit;
}

void free_Emitter(Emitter a)
{
  vtype i;
  for(i = 0; i < vector_size(a->spork); i++) {
    Func f = (Func)vector_at(a->spork, i);
    rem_ref(&f->obj, f);
  }
  free_Vector(a->spork);
  free_Vector(a->stack);
  free_Vector(a->funcs);
  free_Vector(a->array);
  free(a);
}

Code* new_Code()
{
  Code* code = calloc(1, sizeof(Code));
  code->stack_depth = 0;
  code->need_this = 0;
  code->code = new_Vector();
  code->stack_break = new_Vector();
  code->stack_cont = new_Vector();
  code->stack_return = new_Vector();
  code->frame = new_Frame();
  return code;
}

void free_Code(Code* code)
{
  free_Vector(code->code);
  free_Vector(code->stack_break);
  free_Vector(code->stack_cont);
  free_Vector(code->stack_return);
  free_Frame(code->frame);
  free(code->name);
  free(code->filename);
  free(code);
}

static void emit_pop_scope(Emitter emit)
{
  m_uint i;
  Vector v = new_Vector();
  frame_pop_scope(emit->code->frame, v);
  for (i = 0; i < vector_size(v); i++) {
    Local* l = (Local*)vector_at(v, i);
    if (l->is_obj) {
      Instr instr = add_instr(emit, Release_Object2);
      instr->m_val = l->offset;
    }
    free(l);
  }
  free_Vector(v);
}

static void emit_add_code(Emitter emit, Instr instr)
{
  vector_append(emit->code->code, (vtype)instr);
}
static void emit_add_cont(Emitter emit, Instr instr)
{
  vector_append(emit->code->stack_cont, (vtype)instr);
}
static void emit_add_break(Emitter emit, Instr instr)
{
  vector_append(emit->code->stack_break, (vtype)instr);
}
static void emit_add_return(Emitter emit, Instr instr)
{
  vector_append(emit->code->stack_return, (vtype)instr);
}

static m_bool emit_instantiate_object(Emitter emit, Type type, Array_Sub array, m_bool is_ref);

static m_bool emit_symbol(Emitter emit, S_Symbol symbol, Value v, int emit_var, int pos)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "symbol %s (const:%i) %i %p", S_name(symbol), GET_FLAG(v, ae_value_const) + GET_FLAG(v, ae_value_enum), GET_FLAG(v, ae_value_static), v->owner_class);
#endif
  Instr instr;

  if (v->owner_class && (GET_FLAG(v, ae_value_member) || GET_FLAG(v, ae_value_static))) {
    m_bool ret;
    Expression base = new_Primary_Expression_from_ID("this", pos);
    Expression dot = new_exp_from_member_dot(base, S_name(symbol), pos);
    base->type = v->owner_class;
    dot->type = v->m_type;
    dot->d.exp_dot.t_base = v->owner_class;
    dot->emit_var = emit_var;
    if((ret = emit_Dot_Member(emit, &dot->d.exp_dot)) < 0)
      err_msg(EMIT_, pos, "(emit): internal error: symbol transformation failed..."); // LCOV_EXCL_LINE
    free_Expression(dot);
    return ret;
  }

  if(GET_FLAG(v, ae_value_const)) {
    if (v->func_ref) {
      instr = add_instr(emit, Reg_Push_Imm);
      instr->m_val = (m_uint)v->func_ref;
      return 1;
    } else if (isa(v->m_type, &t_float) > 0 || isa(v->m_type, &t_dur) > 0 || isa(v->m_type, &t_dur) > 0) {
      instr = add_instr(emit, Reg_Push_Imm2);
      instr->f_val = *(m_float*)v->ptr;
      return 1;
    } else {
      instr = add_instr(emit, Reg_Push_Imm);
      instr->m_val = (m_uint)v->ptr;
      instr->m_val2 = emit_var;
    }
    return 1;
  }

  if (emit_var) {
    instr = add_instr(emit, Reg_Push_Mem_Addr);
    instr->m_val = v->offset;
    instr->m_val2 = GET_FLAG(v, ae_value_global);
    return 1;
  } else {
    Kindof kind = kindof(v->m_type);
    instr = add_instr(emit, NULL); // dangerous
    instr->m_val = v->offset;
    instr->m_val2 = GET_FLAG(v, ae_value_global);
    switch(kind) {
      case Kindof_Int:
        instr->execute = Reg_Push_Mem;
        break;
      case Kindof_Float:
        instr->execute = Reg_Push_Mem2;
        break;
      case Kindof_Complex:
        instr->execute = Reg_Push_Mem_Complex;
        break;
      case Kindof_Vec3:
        instr->execute = Reg_Push_Mem_Vec3;
        break;
      case Kindof_Vec4:
        instr->execute = Reg_Push_Mem_Vec4;
        break;
      case Kindof_Void: break; // unreachable
    }
  }
  return 1;
}

VM_Code emit_to_code(Emitter emit)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "emit to code");
#endif
  Code* c = emit->code;
  VM_Code code = new_VM_Code(c->code, c->stack_depth, c->need_this, c->name, c->filename);
  free_Code(c);
  return code;
}

m_bool emit_Pre_Ctor(Emitter emit, Type type)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "pre ctor");
#endif
  if (type->parent)
    emit_Pre_Ctor(emit, type->parent);
  if (type->has_constructor) {
    Instr instr = add_instr(emit, Pre_Constructor);
    instr->m_val = (m_uint)type->info->pre_ctor;
    instr->m_val2 = (m_uint)emit->code->frame->curr_offset;
  }
  return 1;
}

static m_bool emit_pre_constructor_array(Emitter emit, Type type)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "pre ctor array");
#endif
  Instr top, bottom;
  m_uint start_index = vector_size(emit->code->code);
  top = add_instr(emit, Instr_Pre_Ctor_Array_Top);
  top->ptr = type;
  emit_Pre_Ctor(emit, type);
  bottom = add_instr(emit, Instr_Pre_Ctor_Array_Bottom);
  top->m_val = vector_size(emit->code->code);
  bottom->m_val = start_index;
  sadd_instr(emit, Instr_Pre_Ctor_Array_Post);
  return 1;
}

static m_bool emit_instantiate_object(Emitter emit, Type type, Array_Sub array, m_bool is_ref)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "instantiate object");
#endif
  if(type->array_depth) {
    CHECK_BB(emit_Expression(emit, array->exp_list, 0))
    VM_Array_Info* info = calloc(1, sizeof(VM_Array_Info));
    info->depth = type->array_depth;
    info->type = type;
    info->is_obj = isa(type->array_type, &t_object) > 0 ? 1 : 0;
    info->stack_offset = emit->code->frame->curr_offset;
    info->is_ref = is_ref;
    Instr alloc = add_instr(emit, Instr_Array_Alloc);
    alloc->ptr = info;
    if(!is_ref && info->is_obj)
      emit_pre_constructor_array(emit, type->array_type);
  } else if (isa(type, &t_object) > 0 && !is_ref) {
    Instr instr = add_instr(emit, Instantiate_Object);
    instr->ptr = type;
    emit_Pre_Ctor(emit, type);
  }
  return 1;
}

static m_bool emit_array_lit(Emitter emit, Array_Sub array)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "array lit");
#endif
  m_uint count = 0;
  CHECK_BB(emit_Expression(emit, array->exp_list, 0))
  Expression e = array->exp_list;
  while (e) {
    count++;
    e = e->next;
  }
  Type type = array->type;
  Instr instr = add_instr(emit, Instr_Array_Init);
  VM_Array_Info* info = calloc(1, sizeof(VM_Array_Info));
  info->type = type;
  info->length = count;
  instr->ptr = info;
  instr->m_val2 = kindof(type->array_type);
  return 1;
}

static m_bool emit_Array(Emitter emit, Array* array)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "array (access?)");
#endif
  Type type, base_type;
  Instr instr;
  m_uint depth = 0;
  Array_Sub sub = NULL;
  Expression exp = NULL;
  m_uint is_var = 0;

  type = array->self->type;
  base_type = array->base->type;
  is_var = array->self->emit_var;
  depth = base_type->array_depth - type->array_depth;
  if (!depth) {
    err_msg(EMIT_, array->pos, "internal error: array with 0 depth..."); // LCOV_EXCL_LINE
    return -1;                                                           // LCOV_EXCL_LINE
  }
  sub = array->indices;
  if (!sub) {
    err_msg(EMIT_, array->pos, "internal error: NULL array sub...");     // LCOV_EXCL_LINE
    return -1;                                                           // LCOV_EXCL_LINE
  }
  exp = sub->exp_list;
  if (!exp) {
    err_msg(EMIT_, array->pos, "internal error: NULL array exp...");     // LCOV_EXCL_LINE
    return -1;                                                           // LCOV_EXCL_LINE
  }
  CHECK_BB(emit_Expression(emit, array->base, 0))
  CHECK_BB(emit_Expression(emit, exp, 0))
  if (depth == 1) {
    instr = add_instr(emit, Instr_Array_Access);
    /*    instr->m_val  = (is_var || type->array_depth);*/
    instr->m_val = is_var;
    instr->m_val2 = kindof(type);
  } else {
    instr = add_instr(emit, Instr_Array_Access_Multi);
    instr->m_val = depth;
//    instr->m_val2 = kindof(array->base->type); // was 04/03/17
    instr->m_val2 = kindof(array->base->type->array_type);
    instr->ptr = (m_uint*)(m_uint)(is_var || type->array_depth);
  }
  return 1;
}

static m_bool emit_Vec(Emitter emit, Vec val)
{
  CHECK_BB(emit_Expression(emit, val->args, 0));
  m_int n = 3 - val->numdims;
  while (n > 0) {
    sadd_instr(emit, Reg_Push_Imm2);
    n--;
  }
  return 1;
}
static m_bool emit_Primary_Expression(Emitter emit, Primary_Expression* primary)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "primary");
#endif
  Expression e;
  Instr instr;
  m_uint temp;
  m_float f;

  switch (primary->type) {
  case ae_primary_var:
    if (primary->d.var == insert_symbol("this"))
      sadd_instr(emit, Reg_Push_This);
    else if (primary->d.var == insert_symbol("me"))
      sadd_instr(emit, Reg_Push_Me);
    else if (primary->d.var == insert_symbol("now"))
      sadd_instr(emit, Reg_Push_Now);
    else if (primary->d.var == insert_symbol("false"))
      sadd_instr(emit, Reg_Push_Imm);
    else if (primary->d.var == insert_symbol("true")) {
      instr = add_instr(emit, Reg_Push_Imm);
      instr->m_val = 1;
    } else if (primary->d.var == insert_symbol("maybe"))
      sadd_instr(emit, Reg_Push_Maybe);
    else if (primary->d.var == insert_symbol("null") || primary->d.var == insert_symbol("NULL"))
      sadd_instr(emit, Reg_Push_Imm);
    else
      emit_symbol(emit, primary->d.var, primary->value, primary->self->emit_var, primary->pos);
    break;

  case ae_primary_num:
    memcpy(&temp, &primary->d.num, sizeof(temp));
    instr = add_instr(emit, Reg_Push_Imm);
    instr->m_val = temp;
    break;

  case ae_primary_char:
    instr = add_instr(emit, Reg_Push_Imm);
    instr->m_val = str2char(primary->d.chr, primary->pos);
    break;

  case ae_primary_float:
    memcpy(&f, &primary->d.fnum, sizeof(f));
    instr = add_instr(emit, Reg_Push_Imm2);
    instr->f_val = f;
    break;

  case ae_primary_complex:
    CHECK_BB(emit_Expression(emit, primary->d.cmp->re, 0));
    break;

  case ae_primary_polar:
    CHECK_BB(emit_Expression(emit, primary->d.polar->mod, 0));
    break;
  case ae_primary_vec:
    CHECK_BB(emit_Vec(emit, primary->d.vec));
    break;

  case ae_primary_str:
    memcpy(&temp, &primary->d.str, sizeof(temp));
    instr = add_instr(emit, Reg_Push_Str);
    instr->m_val = temp;
    break;

  case ae_primary_array:
    CHECK_BB(emit_array_lit(emit, primary->d.array))
    break;
  case ae_primary_nil: // was Reg_Push_Imm
    break;
  case ae_primary_hack:
    e = primary->d.exp;
    CHECK_BB(emit_Expression(emit, e, 0))
    Vector types = new_Vector();
    e = primary->d.exp;
    while (e) {
      vector_append(types, (vtype)e->type);
      e = e->next;
    }
    instr = add_instr(emit, Gack);
    instr->ptr = types;
    break;
  }
  return 1;
}

static m_bool emit_Decl_Expression(Emitter emit, Decl_Expression* decl)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "decl");
#endif
  Decl_Expression* exp = decl;
  Var_Decl_List list = exp->list;

  while (list) {
    Var_Decl var_decl = list->self;
    Value value = var_decl->value;
    Type type = value->m_type;
    m_bool is_obj = isa(type, &t_object) > 0 || list->self->array;
    m_bool is_ref = decl->type->ref;
    Kindof kind = kindof(type);

    if (is_obj) {
      if (list->self->array) {
        if (list->self->array->exp_list) {
          CHECK_BB(emit_instantiate_object(emit, type, list->self->array, is_ref))
        } else if (!value->owner_class)
          vector_append(emit->array, (vtype)decl->m_type);
      } else if (!is_ref)
        CHECK_BB(emit_instantiate_object(emit, type, list->self->array, is_ref))
    }
    if(GET_FLAG(value, ae_value_member)) {
      Instr alloc_m = add_instr(emit, NULL);
      alloc_m->m_val = value->offset;
      switch(kind)  {
        case Kindof_Int     : alloc_m->execute = Alloc_Member_Word         ; break;
        case Kindof_Float   : alloc_m->execute = Alloc_Member_Word_Float   ; break;
        case Kindof_Complex : alloc_m->execute = Alloc_Member_Word_Complex ; break;
        case Kindof_Vec3    : alloc_m->execute = Alloc_Member_Word_Vec3    ; break;
        case Kindof_Vec4    : alloc_m->execute = Alloc_Member_Word_Vec4    ; break;
		case Kindof_Void: break; // LCOV_EXCL_LINE
      }
    } else {
      if (!emit->env->class_def || !decl->is_static) {
        Local* local = frame_alloc_local(emit->code->frame, decl->m_type->size, value->name, is_ref, is_obj);
        if (!local)
          return -1;
        value->offset   = local->offset;
        Instr alloc_g   = add_instr(emit, Alloc_Word);
        alloc_g->m_val  = local->offset;
        alloc_g->m_val2 = GET_FLAG(value, ae_value_global);
        switch(kind)  {
          case Kindof_Int     : alloc_g->execute = Alloc_Word         ; break;
          case Kindof_Float   : alloc_g->execute = Alloc_Word_Float   ; break;
          case Kindof_Complex : alloc_g->execute = Alloc_Word_Complex ; break;
          case Kindof_Vec3    : alloc_g->execute = Alloc_Word_Vec3    ; break;
          case Kindof_Vec4    : alloc_g->execute = Alloc_Word_Vec4    ; break;
		  case Kindof_Void: break; // LCOV_EXCL_LINE
        }
      } else { // static
        if(is_obj) {
          Code* code = emit->code;
          emit->code = (Code*)vector_back(emit->stack);
          CHECK_BB(emit_instantiate_object(emit, type, list->self->array, is_ref))
          // init the object in main code
          Instr push = add_instr(emit, Reg_Push_Imm);
          push->m_val = (m_uint)emit->env->class_def;
          Instr dot_static = add_instr(emit, Dot_Static_Data);
          dot_static->m_val = value->offset;
          dot_static->m_val2 = kindof(emit->env->class_def);
          dot_static->ptr = (m_uint*)1;
          sadd_instr(emit, Assign_Object);
          emit->code = code;
          // add ref
          Instr push_obj = add_instr(emit, Dot_Static_Data);
          push_obj->m_val = value->offset;
          sadd_instr(emit, Reg_AddRef_Object3);
          Instr pop2 = add_instr(emit, Reg_Pop_Word4);
          pop2->m_val = SZ_INT;
          return 1;
        } else {
          // emit the type
          Instr push = add_instr(emit, Reg_Push_Imm);
          push->m_val = (m_uint)emit->env->class_def;
          Instr dot_static = add_instr(emit, Dot_Static_Data);
          dot_static->m_val = value->offset;
          dot_static->m_val2 = kindof(emit->env->class_def);
          dot_static->ptr = (m_uint*)1;
        }
      }
    }
    if(is_obj) {
      if(list->self->array) {
        if(list->self->array->exp_list) {
          Instr assign = add_instr(emit, Assign_Object);
          assign->m_val = decl->self->emit_var;
        }
      }
      else if(!is_ref) {
        Instr assign  = add_instr(emit, Assign_Object);
        assign->m_val = decl->self->emit_var;
      }
    }
    list = list->next;
  }
  return 1;
}

static m_bool emit_Func_Call(Emitter emit, Func_Call* exp_func, m_bool spork);
static m_bool emit_Binary_Expression(Emitter emit, Binary_Expression* binary)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "binary");
#endif
  Instr instr;
  // function pointer assignement
  if (binary->op == op_at_chuck && isa(binary->lhs->type, &t_function) > 0 && isa(binary->rhs->type, &t_func_ptr) > 0) {
    Value v;
    CHECK_BB(emit_Expression(emit, binary->lhs, 1))
    CHECK_BB(emit_Expression(emit, binary->rhs, 1))
    instr = add_instr(emit, assign_func);
    switch(binary->rhs->exp_type) {
      case Dot_Member_type:
        v = find_value(binary->rhs->d.exp_dot.t_base, binary->rhs->d.exp_dot.xid);
        instr->m_val2 = v->offset;
        instr->m_val = 1;
        break;
      case Primary_Expression_type:
        if(GET_FLAG(binary->rhs->d.exp_primary.value, ae_value_member)) {
          v = binary->rhs->d.exp_primary.value;
          instr->m_val = 1;
          instr->m_val2 = v->offset;
        }
        break;
        case Decl_Expression_type:
          v = binary->rhs->d.exp_decl.list->self->value;
          instr->m_val2 = v->offset;
          break;
		default: // LCOV_EXCL_LINE // won't reach anyway.
          break; // LCOV_EXCL_LINE
    }
    return 1;
  }
  CHECK_BB(emit_Expression(emit, binary->lhs, 1))
  CHECK_BB(emit_Expression(emit, binary->rhs, 1))

  if (binary->op == op_chuck && isa(binary->rhs->type, &t_function) > 0)
    return emit_Func_Call1(emit, binary->func, binary->func->value_ref->m_type, binary->pos);

  // arrays
  if (binary->op == op_shift_left && (binary->lhs->type->array_depth == binary->rhs->type->array_depth + 1)
      && isa(binary->lhs->type->array_type, binary->rhs->type) > 0) {
    instr = add_instr(emit, Array_Append);
    instr->m_val = kindof(binary->rhs->type);
    return 1;
  }

  if (binary->lhs->type->array_depth || binary->rhs->type->array_depth) {
    if (binary->op == op_at_chuck && binary->lhs->type->array_depth == binary->rhs->type->array_depth)
      sadd_instr(emit, Assign_Object);
    return 1;
  }

  if(binary->op == op_at_chuck && isa(binary->rhs->type, &t_object) > 0 &&
      (isa(binary->lhs->type, &t_null) > 0 || isa(binary->lhs->type, &t_object) > 0)) {
    instr = add_instr(emit, Assign_Object);
    instr->m_val2 = 1;
    return 1;
  }
  CHECK_BB(get_instr(emit, binary->op, binary->lhs->type, binary->rhs->type))
  return 1;
}

static m_bool emit_Cast_Expression1(Emitter emit, Type to, Type from)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "cast %s to %s", from->name, to->name);
#endif
  f_instr f;
  if (to->xid == from->xid)
    return 1;
  if (to->xid == t_int.xid && from->xid == t_float.xid)
    f = Cast_f2i;
  else if (to->xid == t_float.xid && from->xid == t_int.xid)
    f = Cast_i2f;
  else if (isa(from, to) > 0)
    return 1;
  else if(isa(from, &t_null) > 0 && isa(to, &t_object) > 0)
    return 1;
  else {
    err_msg(EMIT_, 0, "cannot cast '%s' to '%s'", from->name, to->name); // LCOV_EXCL_LINE
    return -1;                                                           // LCOV_EXCL_LINE
  }
  sadd_instr(emit, f);
  return 1;
}

static m_bool emit_Cast_Expression(Emitter emit, Cast_Expression* cast)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "cast");
#endif
  Type to = cast->self->type;
  Type from = cast->exp->type;
  if (isa(to, &t_func_ptr) > 0 && isa(from, &t_function) > 0) {
    Instr push = add_instr(emit, Reg_Push_Imm);
    push->m_val = (m_uint)cast->func;
    return 1;
  }
  CHECK_BB(emit_Expression(emit, cast->exp, 0))
  return emit_Cast_Expression1(emit, to, from);
}

static m_bool emit_Postfix_Expression(Emitter emit, Postfix_Expression* postfix)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "postfix");
#endif
  f_instr f;
  CHECK_BB(emit_Expression(emit, postfix->exp, 0))

  //  return
  /*get_instr(emit, postfix->op, postfix->exp->type, NULL);*/
  // emit
  switch (postfix->op) {
  case op_plusplus:
    if (postfix->exp->type->xid == t_int.xid)
      f = post_inc;
    else {
      err_msg(EMIT_, postfix->pos, // LCOV_EXCL_START
        "(emit): internal error: unhandled type '%s' for post '++' operator",
        postfix->exp->type->name);
      return -1;
    }                              // LCOV_EXCL_STOP
    break;

  case op_minusminus:
    if (postfix->exp->type->xid == t_int.xid)
      f = post_dec;
    else {
      err_msg(EMIT_, postfix->pos, // LCOV_EXCL_START
              "(emit): internal error: unhandled type '%s' for post '--' operator",
              postfix->exp->type->name);
      return -1;
    }                              // LCOV_EXCL_STOP
    break;

  default:
    err_msg(EMIT_, postfix->pos,   // LCOV_EXCL_START
            "(emit): internal error: unhandled postfix operator '%s'",
            op2str(postfix->op));
    return -1;
  }                                // LCOV_EXCL_STOP
  sadd_instr(emit, f);
  return 1;
}

static m_bool emit_Dur(Emitter emit, Exp_Dur* dur)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "dur");
#endif
  CHECK_BB(emit_Expression(emit, dur->base, 0))
  if(isa(dur->base->type, &t_int) > 0)
    sadd_instr(emit, Cast_i2f);
  CHECK_BB(emit_Expression(emit, dur->unit, 0))
  sadd_instr(emit, timesf);
  return 1;
}

/* static */ m_bool emit_Func_Call1(Emitter emit, Func func, Type type, int pos)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "func call1. '%s' offset: %i", func->name, emit->code->frame->curr_offset);
#endif
  m_bool is_ptr = 0;
  Instr code, offset, call;
  if (!func->code) { // calling function pointer in func
    Func f = namespace_lookup_func(emit->env->curr, insert_symbol(func->name), -1);
    if (!f) { //template with no list
      if (!func->def->is_template) {
        err_msg(EMIT_, func->def->pos, "function not emitted yet");
        return -1;
      }
      if(emit_Func_Def(emit, func->def) < 0) {
        err_msg(EMIT_, 0, "can't emit func."); // LCOV_EXCL_START
        return -1;
      }                                        // LCOV_EXCL_STOP
      func->code = func->def->func->code;
      code = add_instr(emit, Reg_Push_Ptr);
      code->ptr = func->code;
    } else {
      code = add_instr(emit, Reg_Push_Code);
      code->m_val = func->value_ref->offset;
      code->m_val2 = func->value_ref->owner_class ? 1 : 0;
      is_ptr = !func->value_ref->owner_class;
    }
  } else {
    code = add_instr(emit, Reg_Push_Ptr);
    code->ptr = func->code;
  }

  if(!emit->code->stack_depth && !emit->code->frame->curr_offset)
    sadd_instr(emit, Mem_Push_Imm);

  offset = add_instr(emit, Reg_Push_Imm);
  offset->m_val = emit->code->frame->curr_offset;
  call = add_instr(emit, Instr_Func_Call);
  call->m_val = func->def->s_type == ae_func_builtin ? kindof(func->def->ret_type) : emit->code->stack_depth;
  if (func->def->s_type == ae_func_builtin) {
    if (func->is_member)
      call->execute = Instr_Func_Call_Member;
    else
      call->execute = Instr_Func_Call_Static;
  }
  else if(!strcmp(S_name(func->def->name), "chuck")) { // should also handle other ops
    call->execute = Instr_Op_Call_Binary;
	call->m_val2  = (m_uint)func->def->arg_list->type;
	call->ptr     = func->def->arg_list->next->type;
  }
  if (func->def->is_template) {
    Instr clear = add_instr(emit, Free_Func);
    clear->m_val = (m_uint)func;
  }
  if(is_ptr)
    free(func);
  return 1;
}

static m_bool emit_Func_Args(Emitter emit, Func_Call* exp_func);
static m_bool emit_spork(Emitter emit, Func_Call* exp)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "spork");
#endif
  Instr op = NULL, push_code = NULL, spork = NULL;
  VM_Code code;

  CHECK_BB(emit_Func_Args(emit, exp))
  if (emit_Expression(emit, exp->func, 0) < 0) {
    err_msg(EMIT_, exp->pos, // LCOV_EXCL_START
            "(emit): internal error in evaluating function call...");
    return -1;
  }                          // LCOV_EXCL_STOP
  vector_append(emit->stack, (vtype)emit->code);
  emit->code = new_Code();
  sadd_instr(emit, start_gc);
  emit->code->need_this = exp->m_func->is_member;
  emit->code->name = strdup("spork~exp");
  emit->code->filename = strdup(emit_filename);
  op = add_instr(emit, Mem_Push_Imm);
  CHECK_BB(emit_Func_Call1(emit, exp->m_func, exp->ret_type, exp->pos))
  sadd_instr(emit, stop_gc);
  sadd_instr(emit, EOC);
  op->m_val = emit->code->stack_depth;

  code = emit_to_code(emit);
  exp->vm_code = code;
  //exp->vm_code->add_ref();
  emit->code = (Code*)vector_pop(emit->stack);

  Expression e = exp->args;
  m_uint size = 0;
  while (e) {
    size += e->cast_to ? e->cast_to->size : e->type->size;
    e = e->next;
  }

  push_code = add_instr(emit, Reg_Push_Imm);
  push_code->m_val = (m_uint)code;
  spork = add_instr(emit, Spork);
  spork->m_val = size;
  spork->m_val2 = (m_uint)code;
  return 1;
}

static m_bool emit_Unary(Emitter emit, Unary_Expression* exp_unary)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "exp_unary");
#endif
  Instr instr;
  Type t = exp_unary->self->type;
  if (exp_unary->op != op_spork && emit_Expression(emit, exp_unary->exp, 0) < 0)
    return -1;
  switch (exp_unary->op) {
  case op_plusplus:
    if (exp_unary->self->meta != ae_meta_var) {
      err_msg(EMIT_, exp_unary->self->pos, "(emit): target for '++' not mutable..."); // LCOV_EXCL_LINE
      return -1;                                                                      // LCOV_EXCL_LINE
    }
    if (isa(exp_unary->exp->type, &t_int) > 0)
      instr = add_instr(emit, pre_inc);
    break;
  case op_minusminus:
    if (exp_unary->self->meta != ae_meta_var) {
      err_msg(EMIT_, exp_unary->self->pos, "(emit): target for '--' not mutable..."); // LCOV_EXCL_LINE
      return -1;                                                                      // LCOV_EXCL_LINE
    }
    if (isa(exp_unary->exp->type, &t_int) > 0)
      instr = add_instr(emit, pre_dec);
    break;
  case op_exclamation:
    if (isa(exp_unary->exp->type, &t_int) > 0 || isa(exp_unary->self->type, &t_object) > 0)
      instr = add_instr(emit, not);
    else if (isa(exp_unary->exp->type, &t_float) > 0 || isa(t, &t_time) > 0 || isa(t, &t_dur) > 0)
      instr = add_instr(emit, notf);
    else {
      err_msg(EMIT_, exp_unary->self->pos, // LCOV_EXCL_START
        "(emit): internal error: unhandled type '%s' for ! operator",
         exp_unary->self->type->name);
      return -1;
    }                                      // LCOV_EXCL_STOP
    break;

  case op_spork:
    if(exp_unary->exp && exp_unary->exp->exp_type == Func_Call_type) {
      CHECK_BB(emit_spork(emit, &exp_unary->exp->d.exp_func))
    } else if (exp_unary->code) {
      Instr op = NULL, push_code = NULL, spork = NULL;
      VM_Code code;
      ID_List list = new_id_list("sporked", exp_unary->pos);
      Func f = new_Func("sporked", new_Func_Def(0, 0, new_Type_Decl(list, 0, exp_unary->pos), "sporked", NULL, exp_unary->code, exp_unary->pos));

      if (emit->env->class_def)
        sadd_instr(emit, Reg_Push_This);
      Instr push = add_instr(emit, Reg_Push_Imm);
      push->m_val = (m_uint)f;

      vector_append(emit->stack, (vtype)emit->code);
      emit->code = new_Code();
      f->is_member = emit->code->need_this = emit->env->class_def ? 1 : 0;
      emit->code->name = strdup("spork~code");
      emit->code->filename = strdup(emit_filename);
      op = add_instr(emit, Mem_Push_Imm);
      vector_append(emit->spork, (vtype)f);
      frame_push_scope(emit->code->frame);

      sadd_instr(emit, start_gc);
      CHECK_BB(emit_Stmt(emit, exp_unary->code, 0))
      sadd_instr(emit, stop_gc);
      emit_pop_scope(emit);
      instr = add_instr(emit, EOC);
      op->m_val = emit->code->stack_depth;
      code = emit_to_code(emit);
      emit->code = (Code*)vector_pop(emit->stack);
      push_code = add_instr(emit, Reg_Push_Imm);
      push_code->m_val = (m_uint)code;
      spork = add_instr(emit, Spork);
      spork->ptr = (m_uint*)(emit->env->func ? emit->env->func->def->stack_depth : 0); // don't push func info on the stack
      spork->m_val2 = (m_uint)code;
    } else {
      err_msg(EMIT_, exp_unary->pos, "(emit): internal error: sporking non-function call..."); // LCOV_EXCL_LINE
      return -1;                                                                               // LCOV_EXCL_LINE
    }
    break;

  case op_minus:
    if (isa(exp_unary->self->type, &t_int) > 0)
      instr = add_instr(emit, negate);
    else if (isa(exp_unary->self->type, &t_float) > 0)
      instr = add_instr(emit, negatef);
    else {
      err_msg(EMIT_, exp_unary->pos, // LCOV_EXCL_START
        "(emit): internal error: unhandled type '%s' for exp_unary '-' operator", exp_unary->exp->type->name);
      return -1;
    }                                // LCOV_EXCL_STOP
    break;

  case op_new:
    if(isa(exp_unary->self->type, &t_object) > 0)
      CHECK_BB(emit_instantiate_object(emit, exp_unary->self->type, exp_unary->array, exp_unary->type->ref))
    break;
  case op_sizeof:
    instr = add_instr(emit, Reg_Push_Imm);
    instr->m_val = exp_unary->self->type->size;
    break;
  default:
    err_msg(EMIT_, exp_unary->pos, // LCOV_EXCL_START
            "(emit): internal error: unhandled type '%s' for exp_unary '%s' operator", op2str(exp_unary->op));
    return -1;
  }                                // LCOV_EXCL_STOP
  return 1;
}

static m_bool emit_Func_Args(Emitter emit, Func_Call* exp_func)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "func args");
#endif
  if (emit_Expression(emit, exp_func->args, 1) < 0) {
    err_msg(EMIT_, exp_func->pos, "(emit): internal error in emitting function call arguments..."); // LCOV_EXCL_LINE
    return -1;                                                                                      // LCOV_EXCL_LINE
  }
  if (exp_func->m_func->def->is_variadic) {
    m_uint offset = 0, size = 0;
    Expression e = exp_func->args;
    Arg_List l = exp_func->m_func->def->arg_list;
    Vector kinds = new_Vector();
    while (e) {
      if (!l) {
        size += e->type->size;
        vector_append(kinds, (vtype)kindof(e->type));
      } else
        offset += e->type->size;
      e = e->next;
      if (l)
        l = l->next;
    }
    Instr instr = add_instr(emit, MkVararg);
    instr->m_val = size;
    instr->m_val2 = (m_uint)kinds;
  }
  return 1;
}

static m_bool emit_Func_Call(Emitter emit, Func_Call* exp_func, m_bool spork)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "func call");
#endif
  // templating
  if  (exp_func->types) {
    if (exp_func->m_func->value_ref->owner_class) {
      vector_append(emit->env->nspc_stack, (vtype)emit->env->curr);
      emit->env->curr = exp_func->m_func->value_ref->owner_class->info;
      vector_append(emit->env->class_stack, (vtype)emit->env->class_def);
      emit->env->class_def = exp_func->m_func->value_ref->owner_class;
      emit->env->class_scope = 0;
    }
    ID_List base_t = exp_func->m_func->def->base;
    Type_List list = exp_func->types;
    namespace_push_type(emit->env->curr);
    while(base_t) {
      namespace_add_type(emit->env->curr, base_t->xid, find_type(emit->env, list->list));
      base_t = base_t->next;
      list = list->next;
    }
    exp_func->m_func->def->is_template = 1;
    CHECK_BB(scan1_Func_Def(emit->env, exp_func->m_func->def))
    CHECK_BB(scan2_Func_Def(emit->env, exp_func->m_func->def))
    CHECK_BB(check_Func_Def(emit->env, exp_func->m_func->def))
    namespace_pop_type(emit->env->curr);
    if(exp_func->m_func->value_ref->owner_class) {
      emit->env->class_def = (Type)vector_pop(emit->env->class_stack);
      emit->env->curr = (NameSpace)vector_pop(emit->env->nspc_stack);
    }
  }
  if (exp_func->args && !spork) {
    if  (emit_Func_Args(emit, exp_func) < 0) {
      err_msg(EMIT_, exp_func->pos, "internal error in evaluating function arguments..."); // LCOV_EXCL_LINE
      return -1;                                                                           // LCOV_EXCL_LINE
    }
  }
  if (emit_Expression(emit, exp_func->func, 0) < 0) {
    err_msg(EMIT_, exp_func->pos, "internal error in evaluating function call..."); // LCOV_EXCL_LINE
    return -1;                                                                      // LCOV_EXCL_LINE
  }
  if(exp_func->m_func->def->is_variadic && !exp_func->args) // handle empty call to variadic functions
  {
    sadd_instr(emit, MkVararg);
    sadd_instr(emit, Reg_Push_Imm);
  }
  return emit_Func_Call1(emit, exp_func->m_func, exp_func->ret_type, exp_func->pos);
}

static m_bool emit_implicit_cast(Emitter emit, Type from, Type to)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "implicit cast '%s' to '%s'", from->name, to->name);
#endif
  if (from->xid == t_int.xid && to->xid == t_float.xid)
    sadd_instr(emit, Cast_i2f);
  else if (from->xid == t_float.xid && to->xid == t_int.xid)
    sadd_instr(emit, Cast_f2i);
  else if (isa(to, from) < 0 && isa(from, to) < 0) {                                                  // LCOV_EXCL_LINE
    err_msg(EMIT_, 0, "(emit): internal error: cannot cast type '%s' to '%s'", from->name, to->name); // LCOV_EXCL_LINE
    return -1;                                                                                        // LCOV_EXCL_LINE
  }
  return 1;
}

static m_bool emit_exp_if(Emitter emit, If_Expression* exp_if)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "expression if");
#endif
  m_bool ret;
  Instr op = NULL, op2 = NULL;
  f_instr fop;
  namespace_push_value(emit->env->curr);
  CHECK_BB(emit_Expression(emit, exp_if->cond, 0))
  switch (exp_if->cond->type->xid) {
  case te_int:
    sadd_instr(emit, Reg_Push_Imm);
    fop = Branch_Eq_Int;
    break;
  case te_float:
  case te_dur:
  case te_time:
    sadd_instr(emit, Reg_Push_Imm2);
    fop = Branch_Eq_Float;
    break;

  default:
    err_msg(EMIT_, exp_if->cond->pos, // LCOV_EXCL_START
            "(emit): internal error: unhandled type '%s' in if condition",
            exp_if->cond->type->name);
    return -1;
  }                                   // LCOV_EXCL_STOP
  op = add_instr(emit, fop);
  ret = emit_Expression(emit, exp_if->if_exp, 0);
  if (!ret)
    return -1;
  op2 = add_instr(emit, Goto);
  op->m_val = vector_size(emit->code->code);
  ret = emit_Expression(emit, exp_if->else_exp, 0);
  namespace_pop_value(emit->env->curr);
  if (!ret)
    return 0;
  op2->m_val = vector_size(emit->code->code);
  return ret;
}

static m_bool emit_Expression(Emitter emit, Expression exp, m_bool add_ref)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "expression");
#endif
  Expression tmp = exp;
  while (tmp) {
    switch (tmp->exp_type) {
    case Decl_Expression_type:
      CHECK_BB(emit_Decl_Expression(emit, &tmp->d.exp_decl))
      break;
    case Primary_Expression_type:
      CHECK_BB(emit_Primary_Expression(emit, &tmp->d.exp_primary))
      break;
    case Unary_Expression_type:
      CHECK_BB(emit_Unary(emit, &tmp->d.exp_unary))
      break;
    case Binary_Expression_type:
      CHECK_BB(emit_Binary_Expression(emit, &tmp->d.exp_binary))
      break;
    case Postfix_Expression_type:
      CHECK_BB(emit_Postfix_Expression(emit, &tmp->d.exp_postfix))
      break;
    case Cast_Expression_type:
      CHECK_BB(emit_Cast_Expression(emit, &tmp->d.exp_cast))
      break;
    case Dot_Member_type:
      CHECK_BB(emit_Dot_Member(emit, &tmp->d.exp_dot))
      break;
    case Func_Call_type:
      CHECK_BB(emit_Func_Call(emit, &tmp->d.exp_func, 0))
      break;
    case Array_Expression_type:
      CHECK_BB(emit_Array(emit, &tmp->d.exp_array))
      break;
    case If_Expression_type:
      CHECK_BB(emit_exp_if(emit, &tmp->d.exp_if))
      break;
    case Dur_Expression_type:
      CHECK_BB(emit_Dur(emit, &tmp->d.exp_dur))
      break;
	}
    if (tmp->cast_to)
      CHECK_BB(emit_implicit_cast(emit, tmp->type, tmp->cast_to))
    if(add_ref && isprim(tmp->type) < 0 && isa(tmp->type, &t_void) < 0) {
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

static m_bool emit_If(Emitter emit, Stmt_If stmt)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "emit if");
#endif
  Instr op = NULL, op2 = NULL;
  f_instr f;
  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Expression(emit, stmt->cond, 0))

  switch (stmt->cond->type->xid) {

  case te_int:
    sadd_instr(emit, Reg_Push_Imm);
    f = Branch_Eq_Int;
    break;
  case te_float:
  case te_dur:
  case te_time:
    sadd_instr(emit, Reg_Push_Imm2);
    f = Branch_Eq_Float;
    break;

  default:
    // check for IO
    /*      if(isa(stmt->cond->type, &t_io))*/
    /*      {*/
    /*          // push 0*/
    /*          emit->append(new Chuck_Instr_Reg_Push_Imm(0));*/
    /*          op = new Chuck_Instr_Branch_Eq_int_IO_good(0);*/
    /*          break;*/
    /*      }*/

    if (isa(stmt->cond->type, &t_object) > 0) {
      sadd_instr(emit, Reg_Push_Imm);
      f = Branch_Eq_Int;
      break;
    }
    err_msg(EMIT_, stmt->cond->pos, // LCOV_EXCL_START
      "internal error: unhandled type '%s' in if condition", stmt->cond->type->name);
    return -1;
  }                                 // LCOV_EXCL_STOP
  op = add_instr(emit, f);
  {
    frame_push_scope(emit->code->frame);
    CHECK_BB(emit_Stmt(emit, stmt->if_body, 1))
    // pop stack
    emit_pop_scope(emit);
  }
  op2 = add_instr(emit, Goto);
  op->m_val = vector_size(emit->code->code);

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Stmt(emit, stmt->else_body, 1))
  emit_pop_scope(emit);

  op2->m_val = vector_size(emit->code->code);
  emit_pop_scope(emit);
  return 1;
}

static m_bool emit_Stmt_Code(Emitter emit, Stmt_Code stmt, m_bool push)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "emit stmt code");
#endif
  Stmt_List list = stmt->stmt_list;
  while (list) {
    CHECK_BB(emit_Stmt(emit, list->stmt, 1))
    list = list->next;
  }
  return 1;
}

static void emit_func_release(Emitter emit) {
  m_uint i;
  Vector v = emit->code->frame->stack;
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

static m_bool emit_Return(Emitter emit, Stmt_Return stmt)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "return");
#endif
  CHECK_BB(emit_Expression(emit, stmt->val, 0))
  emit_func_release(emit); // /04/04/2017
//  if(stmt->val && isa(stmt->val->type, &t_object) > 0) // void doesn't have ->val
//	add_instr(emit, Reg_AddRef_Object3);
  Instr op = add_instr(emit, Goto);
  emit_add_return(emit, op);


  return 1;
}
static m_bool emit_Continue(Emitter emit, Stmt_Continue cont)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "continue");
#endif
  Instr op = add_instr(emit, Goto);
  emit_add_cont(emit, op);
  return 1;
}
static m_bool emit_Break(Emitter emit, Stmt_Break cont)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "break");
#endif
  Instr op = add_instr(emit, Goto);
  emit_add_break(emit, op);
  return 1;
}

static m_bool emit_While(Emitter emit, Stmt_While stmt)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "func while");
#endif
  m_bool ret = 1;
  m_uint index = vector_size(emit->code->code);
  Instr op, goto_;
  f_instr f;
  frame_push_scope(emit->code->frame);
  emit_add_cont(emit, NULL);
  emit_add_break(emit, NULL);

  CHECK_BB(emit_Expression(emit, stmt->cond, 0))

  switch (stmt->cond->type->xid) {
  case te_int:
    sadd_instr(emit, Reg_Push_Imm);
    f = Branch_Eq_Int;
    break;

  case te_float:
  case te_dur:
  case te_time:
    sadd_instr(emit, Reg_Push_Imm2);
    f = Branch_Eq_Float;
    break;

  default:
    err_msg(EMIT_, stmt->cond->pos, // LCOV_EXCL_START
            "(emit): internal error: unhandled type '%s' in while conditional",
            stmt->cond->type->name);
    return -1;
  }                                 // LCOV_EXCL_STOP
  op = add_instr(emit, f);
  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Stmt(emit, stmt->body, 1)) // was '0' , then 'stmt->body->type == ae_stmt_code ? 0 : 1'
  emit_pop_scope(emit);

  goto_ = add_instr(emit, Goto);
  goto_->m_val = index;
  op->m_val = vector_size(emit->code->code);
  while (vector_size(emit->code->stack_cont) && vector_back(emit->code->stack_cont)) {
    Instr instr = (Instr)vector_pop(emit->code->stack_cont);
    instr->m_val = index;
  }
  while (vector_size(emit->code->stack_break) && vector_back(emit->code->stack_break)) {
    Instr instr = (Instr)vector_pop(emit->code->stack_break);
    instr->m_val = vector_size(emit->code->code);
  }
  emit_pop_scope(emit);
  vector_pop(emit->code->stack_cont);
  vector_pop(emit->code->stack_break);
  return ret;
}

static m_bool emit_Do_While(Emitter emit, Stmt_While stmt)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "do while");
#endif
  m_bool ret = 1;
  Instr op;
  m_uint index = vector_size(emit->code->code);
  f_instr f;
  // push stack
  frame_push_scope(emit->code->frame);

  vector_append(emit->code->stack_cont, (vtype)NULL);
  vector_append(emit->code->stack_break, (vtype)NULL);

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);

  // emit the cond
  CHECK_BB(emit_Expression(emit, stmt->cond, 0))

  switch (stmt->cond->type->xid) {
  case te_int:
    sadd_instr(emit, Reg_Push_Imm);
    f = Branch_Neq_Int;
    break;
  case te_float:
  case te_dur:
  case te_time:
    sadd_instr(emit, Reg_Push_Imm2);
    f = Branch_Neq_Float;
    break;
  default:
    // check for IO
    /*        if(isa(stmt->cond->type, &t_io))*/
    /*        {*/
    /*            // push 0*/
    /*            emit->append(new Chuck_Instr_Reg_Push_Imm(0));*/
    /*            op = new Chuck_Instr_Branch_Eq_int_IO_good(0);*/
    /*            break;*/
    /*        }*/

    err_msg(EMIT_, stmt->cond->pos, // LCOV_EXCL_START
            "(emit): internal error: unhandled type '%s' in do/while conditional",
            stmt->cond->type->name);
    return -1;
  }                                 // LCOV_EXCL_STOP
  op = add_instr(emit, f);
  op->m_val = index;
  while (vector_size(emit->code->stack_cont) && vector_back(emit->code->stack_cont)) {
    Instr instr = (Instr)vector_pop(emit->code->stack_cont);
    instr->m_val = index;
  }
  while (vector_size(emit->code->stack_break) && vector_back(emit->code->stack_break)) {
    Instr instr = (Instr)vector_pop(emit->code->stack_break);
    instr->m_val = vector_size(emit->code->code);
  }
  emit_pop_scope(emit);
  vector_pop(emit->code->stack_cont);
  vector_pop(emit->code->stack_break);
  return ret;
}

static m_bool emit_Until(Emitter emit, Stmt_Until stmt)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "until");
#endif
  Instr op;
  f_instr f;
  frame_push_scope(emit->code->frame);

  m_uint index = vector_size(emit->code->code);
  vector_append(emit->code->stack_cont, (vtype)NULL);
  vector_append(emit->code->stack_break, (vtype)NULL);

  CHECK_BB(emit_Expression(emit, stmt->cond, 0))

  // condition
  switch (stmt->cond->type->xid) {
  case te_int:
    sadd_instr(emit, Reg_Push_Imm);
    f = Branch_Neq_Int;
    break;
  case te_float:
  case te_dur:
  case te_time:
    sadd_instr(emit, Reg_Push_Imm2);
    f = Branch_Neq_Float;
    break;

  default:
    // check for IO
    /*      if(isa(stmt->cond->type, &t_io))*/
    /*      {*/
    // push 0
    /*          emit->append(new Chuck_Instr_Reg_Push_Imm(0));*/
    /*          op = new Chuck_Instr_Branch_Neq_int_IO_good(0);*/
    /*          break;*/
    /*      }*/

    err_msg(EMIT_, stmt->cond->pos, // LCOV_EXCL_START
            "(emit): internal error: unhandled type '%s' in until conditional",
            stmt->cond->type->name);
    return -1;
  }                                 // LCOV_EXCL_STOP
  op = add_instr(emit, f);
  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);

  Instr _goto = add_instr(emit, Goto);
  _goto->m_val = index;
  op->m_val = vector_size(emit->code->code);

  while (vector_size(emit->code->stack_cont) && vector_back(emit->code->stack_cont)) {
    Instr instr = (Instr)vector_pop(emit->code->stack_cont);
    instr->m_val = index;
  }
  while (vector_size(emit ->code->stack_break) && vector_back(emit->code->stack_break)) {
    Instr instr = (Instr)vector_pop(emit->code->stack_break);
    instr->m_val = vector_size(emit->code->code);
  }
  emit_pop_scope(emit);
  vector_pop(emit->code->stack_cont);
  vector_pop(emit->code->stack_break);
  return 1;
}
static m_bool emit_Do_Until(Emitter emit, Stmt_Until stmt)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "do until");
#endif
  Instr op;
  f_instr f;
  frame_push_scope(emit->code->frame);

  m_uint index = vector_size(emit->code->code);
  vector_append(emit->code->stack_cont,  (vtype)NULL);
  vector_append(emit->code->stack_break, (vtype)NULL);

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);

  CHECK_BB(emit_Expression(emit, stmt->cond, 0))

  switch (stmt->cond->type->xid) {
  case te_int:
    sadd_instr(emit, Reg_Push_Imm);
    f = Branch_Eq_Int;
    break;
  case te_float:
  case te_dur:
  case te_time:
    sadd_instr(emit, Reg_Push_Imm2);
    f = Branch_Eq_Float;
    break;

  default:
    err_msg(EMIT_, stmt->cond->pos, // LCOV_EXCL_START
            "(emit): internal error: unhandled type '%s' in do/until conditional",
            stmt->cond->type->name);
    return -1;
  }                                 // LCOV_EXCL_STOP
  /*  emit_add_code(emit, op);*/
  op = add_instr(emit, f);
  /*  op->m_val = vector_size(emit->code->code);*/
  op->m_val = index;

  while (vector_size(emit->code->stack_cont) && vector_back(emit->code->stack_cont)) {
    Instr instr = (Instr)vector_pop(emit->code->stack_cont);
    instr->m_val = index;
  }
  while (vector_size(emit->code->stack_break) && vector_back(emit->code->stack_break)) {
    Instr instr = (Instr)vector_pop(emit->code->stack_break);
    instr->m_val = vector_size(emit->code->code);
  }
  emit_pop_scope(emit);
  vector_pop(emit->code->stack_cont);
  vector_pop(emit->code->stack_break);
  return 1;
}

static m_bool emit_For(Emitter emit, Stmt_For stmt)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "for");
#endif
  f_instr f;
  Instr op = NULL;

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Stmt(emit, stmt->c1, 1))

  m_uint index = vector_size(emit->code->code);
  vector_append(emit->code->stack_cont, (vtype)NULL);
  vector_append(emit->code->stack_break, (vtype)NULL);

  CHECK_BB(emit_Stmt(emit, stmt->c2, 0))
  if (stmt->c2) {
    switch (stmt->c2->d.stmt_exp.val->type->xid) {
    case te_int:
      sadd_instr(emit, Reg_Push_Imm);
      f = Branch_Eq_Int;
      break;
    case te_float:
    case te_dur:
    case te_time:
      sadd_instr(emit, Reg_Push_Imm2);
      f = Branch_Eq_Float;
      break;

    default:
      // check for IO
      /*        if(isa(stmt->c2->d.stmt_exp->type, &t_io))*/
      /*        {*/
      // push 0
      /*          emit->append(new Chuck_Instr_Reg_Push_Imm(0));*/
      /*          op = new Chuck_Instr_Branch_Eq_int_IO_good(0);*/
      /*          break;*/
      /*        }*/

      err_msg(EMIT_, stmt->c2->d.stmt_exp.pos, // LCOV_EXCL_START
              "(emit): internal error: unhandled type '%s' in for conditional",
              stmt->c2->d.stmt_exp.val->type->name);
      return -1;
     }                                          // LCOV_EXCL_STOP
    // append the op
    op = add_instr(emit, f);
  }

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);
  m_uint action_index = vector_size(emit->code->code);
  if(stmt->c3) {
    CHECK_BB(emit_Expression(emit, stmt->c3, 0))
    Expression e = stmt->c3;
    m_uint num_words = 0;
    while (e) {
      num_words += e->type->size;
      e = e->next;
    }
    if (num_words) {
      Instr pop = add_instr(emit, Reg_Pop_Word4);
      pop->m_val = num_words;
    }
  }
  Instr _goto = new_Instr();
  _goto->execute = Goto;
  _goto->m_val = index;
  emit_add_code(emit, _goto);

  if (stmt->c2)
    op->m_val = vector_size(emit->code->code);

  while (vector_size(emit->code->stack_cont) && vector_back(emit->code->stack_cont)) {
    Instr instr = (Instr)vector_pop(emit->code->stack_cont);
    instr->m_val = action_index;
  }
  while (vector_size(emit->code->stack_break) && vector_back(emit->code->stack_break)) {
    Instr instr = (Instr)vector_pop(emit->code->stack_break);
    instr->m_val = vector_size(emit->code->code);
  }
  emit_pop_scope(emit);
  vector_pop(emit->code->stack_cont);
  vector_pop(emit->code->stack_break);
  return 1;
}

static m_bool emit_Loop(Emitter emit, Stmt_Loop stmt)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "loop");
#endif
  Instr init, op = NULL, deref, dec = NULL, _goto = NULL;
  m_int* counter;
  m_uint index;
  Type type;

  frame_push_scope(emit->code->frame);
  /* coverity[leaked_storage] */
  CHECK_BB(emit_Expression(emit, stmt->cond, 0))
  counter = calloc(1, sizeof(m_int));
  init = add_instr(emit, Init_Loop_Counter);
  init->m_val = (m_uint)counter;
  index = vector_size(emit->code->code);
  vector_append(emit->code->stack_cont, (vtype)NULL);
  vector_append(emit->code->stack_break, (vtype)NULL);
  deref = add_instr(emit, Reg_Push_Deref);
  deref->m_val = (m_uint)counter;
  type = stmt->cond->cast_to ? stmt->cond->cast_to : stmt->cond->type;

  switch (type->xid) {
  case te_int:
    sadd_instr(emit, Reg_Push_Imm);
    op = new_Instr();
    op->execute = Branch_Eq_Int;
    break;

  default:
    err_msg(EMIT_, stmt->cond->pos, // LCOV_EXCL_START
            "(emit): internal error: unhandled type '%s' in while conditional", type->name);
    return -1;
  }                                 // LCOV_EXCL_STOP

  emit_add_code(emit, op);
  dec = add_instr(emit, Dec_int_Addr);
  dec->m_val = (m_uint)counter;

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);

  _goto = add_instr(emit, Goto);
  _goto->m_val = index;

  op->m_val = vector_size(emit->code->code);
  while (vector_size(emit->code->stack_cont) && vector_back(emit->code->stack_cont)) {
    Instr instr = (Instr)vector_pop(emit->code->stack_cont);
    instr->m_val = index;
  }
  while (vector_size(emit->code->stack_break) && vector_back(emit->code->stack_break)) {
    Instr instr = (Instr)vector_pop(emit->code->stack_break);
    instr->m_val = vector_size(emit->code->code);
  }
  emit_pop_scope(emit);
  vector_pop(emit->code->stack_cont);
  vector_pop(emit->code->stack_break);
  return 1;
}

static m_bool emit_Goto_Label(Emitter emit, Stmt_Goto_Label stmt)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "%s '%s'", stmt->is_label ? "label" : "goto", S_name(stmt->name));
#endif
  m_uint i, size;
  Stmt_Goto_Label label;
  if (!stmt->is_label)
    stmt->data.instr = add_instr(emit, Goto);
  else {
    if (emit->cases && !strcmp(S_name(stmt->name), "default")) {
      if (emit->default_case_index != -1) {
        err_msg(EMIT_, stmt->pos, "default case already defined");
        return -1;
      }
      emit->default_case_index = vector_size(emit->code->code);
      return 1;
    }

    size = vector_size(stmt->data.v);
    if (!size) {
      err_msg(EMIT_, stmt->pos, "label '%s' defined but not used.", S_name(stmt->name));
      free_Vector(stmt->data.v);
      return -1;
    }
    for (i = size + 1; --i;) {
      label = (Stmt_Goto_Label)vector_at(stmt->data.v, i - 1);
      label->data.instr->m_val = vector_size(emit->code->code);
    }
    free_Vector(stmt->data.v);
  }
  return 1;
}

static m_bool emit_Switch(Emitter emit, Stmt_Switch stmt)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "emit switch");
#endif
  Instr instr, _break;
  emit_add_break(emit, NULL);
  CHECK_BB(emit_Expression(emit, stmt->val, 0))
  if (emit->cases) {
    err_msg(EMIT_, stmt->pos, "swith inside an other switch. this is not allowed for now");
    return -1;
  }
  emit->default_case_index = -1;
  sadd_instr(emit, start_gc);
  instr = add_instr(emit, Branch_Switch);
  instr->ptr = emit->cases = new_Map();

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Stmt(emit, stmt->stmt, 1))
  emit_pop_scope(emit);
  instr->m_val = emit->default_case_index > -1 ? emit->default_case_index : vector_size(emit->code->code);
  emit->default_case_index = -1;
  while (vector_size(emit->code->stack_break) && vector_back(emit->code->stack_break)) {
    _break = (Instr)vector_pop(emit->code->stack_break);
    _break->m_val = vector_size(emit->code->code);
  }
  vector_pop(emit->code->stack_break);
  sadd_instr(emit, stop_gc);
  emit->cases = NULL;
  return 1;
}

static m_bool emit_Case(Emitter emit, Stmt_Case stmt)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "case");
#endif
  m_uint value;
  Value v;
  Type t;
  if(!emit->cases) {
    err_msg(EMIT_, stmt->pos, "case found outside switch statement. this is not allowed for now");
    return -1;
  }
  if (stmt->val->exp_type == Primary_Expression_type) {
    if (stmt->val->d.exp_primary.type == ae_primary_num)
      value = stmt->val->d.exp_primary.d.num;
    else {
      if(stmt->val->d.exp_primary.d.var == insert_symbol("true"))
        value = 1;
      else if(stmt->val->d.exp_primary.d.var == insert_symbol("false"))
        value = 0;
      else if(stmt->val->d.exp_primary.d.var == insert_symbol("maybe")) {
        err_msg(EMIT_, stmt->val->d.exp_primary.pos, "'maybe' is not constant.");
        return -1;
      } else  {
        if(!GET_FLAG(stmt->val->d.exp_primary.value, ae_value_const)) {
          err_msg(EMIT_, stmt->pos, "value is not const. this is not allowed for now");
          return -1;
        }
//        value = stmt->val->d.exp_primary.value->is_const == 2 ? (m_uint)stmt->val->d.exp_primary.value->ptr : // for enum
//              *(m_uint*)stmt->val->d.exp_primary.value->ptr;                                                   // for primary variable
          value = (m_uint)stmt->val->d.exp_primary.value->ptr; // assume enum.
      }
    }
  } else if(stmt->val->exp_type == Dot_Member_type) {
    t = isa(stmt->val->d.exp_dot.t_base, &t_class) > 0 ? stmt->val->d.exp_dot.t_base->actual_type : stmt->val->d.exp_dot.t_base;
    v = find_value(t, stmt->val->d.exp_dot.xid);
    value = GET_FLAG(v, ae_value_enum) ? t->info->class_data[v->offset] : *(m_uint*)v->ptr;
  } else {
    err_msg(EMIT_, stmt->pos, "unhandled expression type '%i'", stmt->val->exp_type);
    return -1;
  }
  if (map_get(emit->cases, (vtype)value)) {
    err_msg(EMIT_, stmt->pos, "duplicated cases value %i", value);
    return -1;
  }

  map_set(emit->cases, (vtype)value, (vtype)vector_size(emit->code->code));
  return 1;
}

static m_bool emit_Func_Ptr(Emitter emit, Stmt_Ptr ptr)
{
  namespace_add_func(emit->env->curr, ptr->xid, ptr->func);
  vector_append(emit->funcs, (vtype)ptr);
if(ptr->key) {
//rem_ref(ptr->func->obj, ptr->func);
//add_ref(ptr->func->obj);
  scope_rem(ptr->value->owner_class->info->func, ptr->xid);
}
  return 1;
}

static m_bool emit_Enum(Emitter emit, Stmt_Enum stmt)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "enum");
#endif
  m_uint i;
  Local* local;
  Value v;
  for (i = 0; i < vector_size(stmt->values); i++) {
    v = (Value)vector_at(stmt->values, i);
    if (!emit->env->class_def) {
      local = frame_alloc_local(emit->code->frame, sizeof(m_uint), v->name, 0, 0);
      if (!local)
        return -1;
      v->offset = local->offset;
      v->ptr = (void*)i;
    } else
      emit->env->class_def->info->class_data[v->offset] = i;
  }
  return 1;
}

static m_bool emit_Stmt_Union(Emitter emit, Stmt_Union stmt)
{
  m_bool is_member;
  Decl_List l = stmt->l;
  Local* local;
  Var_Decl_List var_list;
  is_member = GET_FLAG(l->self->list->self->value, ae_value_member);
  if (!is_member) {
    local = frame_alloc_local(emit->code->frame, stmt->s, "union", 1, 0);
    stmt->o = local->offset;
  } else
    local = NULL;
  while (l) {
    var_list = l->self->list;
    while (var_list) {
      var_list->self->value->offset = stmt->o;
      var_list = var_list->next;
    }
    l = l->next;
  }
  return 1;
}

static m_bool emit_Stmt(Emitter emit, Stmt stmt, m_bool pop)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "Stmt (pop: %s)", pop ? "yes" : "no");
#endif
  Instr instr;
  m_bool ret = 1;
  if (!stmt)
    return 1;
  switch (stmt->type) {
  case ae_stmt_exp:
    if (!stmt->d.stmt_exp.val)
      return 1;
    ret = emit_Expression(emit, stmt->d.stmt_exp.val, 0);
// check 'stmt->d.stmt_exp->type'for switch
    if (ret > 0 && pop && stmt->d.stmt_exp.val->type && stmt->d.stmt_exp.val->type->size > 0) {
      Expression exp = stmt->d.stmt_exp.val;
      if (exp->exp_type == Primary_Expression_type && exp->d.exp_primary.type == ae_primary_hack)
        exp = exp->d.exp_primary.d.exp;
      while (exp) {
        // get rid of primary str ?
        /*          if(exp->exp_type == Primary_Expression_type && exp->d.exp_primary->type == ae_primary_str)*/
        instr = add_instr(emit, Reg_Pop_Word4);
        /*        if (isa(exp->type, &t_complex) > 0 && exp->exp_type != Decl_Expression_type)
                  exp->type->size = SZ_COMPLEX * exp->d.exp_decl->num_decl;
                if (isa(exp->type, &t_polar) > 0 && exp->exp_type != Decl_Expression_type)
                  exp->type->size = SZ_COMPLEX * exp->d.exp_decl->num_decl; */
        //        instr->m_val = exp->exp_type == Decl_Expression_type ? exp->d.exp_decl->num_decl * exp->type->size : exp->type->size;
        instr->m_val = (exp->exp_type == Decl_Expression_type ? exp->d.exp_decl.num_decl * SZ_INT : exp->type->size);
        exp = exp->next;
      }
    }
    if (ret < 0)
      return -1;
    break;
  case ae_stmt_code:
    ret = emit_Stmt_Code(emit, &stmt->d.stmt_code, 1);
    break;
  case ae_stmt_if:
    ret = emit_If(emit, &stmt->d.stmt_if);
    break;
  case ae_stmt_return:
    ret = emit_Return(emit, &stmt->d.stmt_return);
    break;

  case ae_stmt_break:
    ret = emit_Break(emit, &stmt->d.stmt_break);
    break;

  case ae_stmt_continue:
    ret = emit_Continue(emit, &stmt->d.stmt_continue);
    break;

  case ae_stmt_while:
    if (stmt->d.stmt_while.is_do)
      ret = emit_Do_While(emit, &stmt->d.stmt_while);
    else
      ret = emit_While(emit, &stmt->d.stmt_while);
    break;
  case ae_stmt_until:
    if (stmt->d.stmt_until.is_do)
      ret = emit_Do_Until(emit, &stmt->d.stmt_until);
    else
      ret = emit_Until(emit, &stmt->d.stmt_until);
    break;
  case ae_stmt_for:
    ret = emit_For(emit, &stmt->d.stmt_for);
    break;
  case ae_stmt_loop:
    ret = emit_Loop(emit, &stmt->d.stmt_loop);
    break;
  case ae_stmt_gotolabel:
    ret = emit_Goto_Label(emit, &stmt->d.stmt_gotolabel);
    break;
  case ae_stmt_case:
    ret = emit_Case(emit, &stmt->d.stmt_case);
    break;
  case ae_stmt_enum:
    ret = emit_Enum(emit, &stmt->d.stmt_enum);
    break;
  case ae_stmt_switch:
    ret = emit_Switch(emit, &stmt->d.stmt_switch);
    break;
  case ae_stmt_funcptr:
    ret = emit_Func_Ptr(emit, &stmt->d.stmt_ptr);
    break;
  case ae_stmt_union:
    ret = emit_Stmt_Union(emit, &stmt->d.stmt_union);
    break;
  }
#ifdef DEBUG_EMIT
  debug_msg("emit", "Stmt %i", ret);
#endif
  return ret;
}

static m_bool emit_Stmt_List(Emitter emit, Stmt_List list)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "Stmt list");
#endif
  Stmt_List tmp = list;
  while (tmp) {
    CHECK_BB(emit_Stmt(emit, tmp->stmt, 1))
    tmp = tmp->next;
  }
  return 1;
}

static m_bool emit_Dot_Member(Emitter emit, Dot_Member* member)
{
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

  t_base = base_static ? member->t_base->actual_type : member->t_base;
  if (t_base->xid == t_complex.xid) {
    if (member->base->meta == ae_meta_var)
      member->base->emit_var = 1;
    CHECK_BB(emit_Expression(emit, member->base, 0))
    value = find_value(t_base, member->xid);
    if (!strcmp(value->name, "re"))
      instr = add_instr(emit, complex_real);
    else
      instr = add_instr(emit, complex_imag);
    instr->m_val = emit_addr;
    return 1;
  } else if (t_base->xid == t_polar.xid) {
    if (member->base->meta == ae_meta_var)
      member->base->emit_var = 1;
    CHECK_BB(emit_Expression(emit, member->base, 0))
    value = find_value(t_base, member->xid);
    if (!strcmp(value->name, "mod"))
      instr = add_instr(emit, complex_real);
    else
      instr = add_instr(emit, complex_imag);
    instr->m_val = emit_addr;
    return 1;
  } else if (t_base->xid == t_vec3.xid) {
    Instr instr;
    if (member->base->meta == ae_meta_var)
      member->base->emit_var = 1;
    CHECK_BB(emit_Expression(emit, member->base, 0))
    value = find_value(t_base, member->xid);
    if (!strcmp(value->name, "x"))
      instr = add_instr(emit, vec3_x);
    else if (!strcmp(value->name, "y"))
      instr = add_instr(emit, vec3_y);
    else if (!strcmp(value->name, "z"))
      instr = add_instr(emit, vec3_z);
    else {
      sadd_instr(emit, Reg_Dup_Last_Vec3);
      Instr f = add_instr(emit, member_function);
      f->ptr = t_base->info->obj_v_table;
      f->m_val = value->func_ref->vt_index;
      return 1;
    }
    instr->m_val = emit_addr;
    return 1;
  } else if (t_base->xid == t_vec4.xid) {
    Instr instr;
    if (member->base->meta == ae_meta_var)
      member->base->emit_var = 1;
    CHECK_BB(emit_Expression(emit, member->base, 0))
    value = find_value(t_base, member->xid);
    if (!strcmp(value->name, "x"))
      instr = add_instr(emit, vec4_x);
    else if (!strcmp(value->name, "y"))
      instr = add_instr(emit, vec4_y);
    else if (!strcmp(value->name, "z"))
      instr = add_instr(emit, vec4_z);
    else if (!strcmp(value->name, "w"))
      instr = add_instr(emit, vec4_w);
    else {
      sadd_instr(emit, Reg_Dup_Last_Vec4);
      Instr f = add_instr(emit, member_function);
      f->ptr = t_base->info->obj_v_table;
      f->m_val = value->func_ref->vt_index;
      return 1;
    }
    instr->m_val = emit_addr;
    return 1;
  }
  if(t_base->xid == t_vararg.xid) {
    m_uint offset = 0;
    Arg_List l = emit->env->func->def->arg_list;
    while (l) {
      offset += l->type->size;
      l = l->next;
    }
    if (!strcmp(S_name(member->xid), "start")) {
      if (emit->env->func->variadic_start) {
        err_msg(EMIT_, 0, "vararg.start already used. this is an error");
        free(emit->env->func->variadic_start);
        return -1;
      }
      emit->env->func->variadic_start = add_instr(emit, Vararg_start);
      emit->env->func->variadic_start->m_val = offset;
      emit->env->func->variadic_index = vector_size(emit->code->code);
      return 1;
    }
    if (!strcmp(S_name(member->xid), "end")) {
      if(!emit->env->func->variadic_start) {
        err_msg(EMIT_, 0, "vararg.start not used before vararg.end. this is an error");
        return -1;
      }
      Instr instr = add_instr(emit, Vararg_end);
      instr->m_val = offset;
      instr->m_val2 = emit->env->func->variadic_index;
      emit->env->func->variadic_start->m_val2 = vector_size(emit->code->code);
      return 1;
    } else if (!strcmp(S_name(member->xid), "i")) {
      Instr instr = add_instr(emit, Vararg_int);
      instr->m_val = offset;
      return 1;
    } else if (!strcmp(S_name(member->xid), "f") || !strcmp(S_name(member->xid), "t") || !strcmp(S_name(member->xid), "d")) {
      Instr instr = add_instr(emit, Vararg_float);
      instr->m_val = offset;
      return 1;
    } else if (!strcmp(S_name(member->xid), "c") || !strcmp(S_name(member->xid), "p")) {
      Instr instr = add_instr(emit, Vararg_complex);
      instr->m_val = offset;
      return 1;
    } else if (!strcmp(S_name(member->xid), "v3")) {
      Instr instr = add_instr(emit, Vararg_Vec3);
      instr->m_val = offset;
      return 1;
    } else if (!strcmp(S_name(member->xid), "v4")) {
      Instr instr = add_instr(emit, Vararg_Vec4);
      instr->m_val = offset;
      return 1;
    } else if (!strcmp(S_name(member->xid), "o")) {
      Instr instr = add_instr(emit, Vararg_object);
      instr->m_val = offset;
      return 1;
    }

    /*    exit(76);*/
    /*  return -1;*/
  }
  /*
     else if(!base_static && t_base->xid == t_string.xid || t_base->xid == t_array.xid)
     { // these types have namespace but no vars...
     value = find_value(t_base, member->xid);
     func = value->func_ref;
     CHECK_BB(emit_Expression(emit, member->base, 0))
     push_i = add_instr(emit, Reg_Dup_Last);
     func_i = add_instr(emit, member_function);
     func_i->m_val = func->vt_index;
     func_i->ptr   = t_base;
     return 1;
     }
     */
  if (!base_static) { // called from instance
    if (isa(member->self->type, &t_func_ptr) > 0) { // function pointer
      value = find_value(t_base, member->xid);
      if(GET_FLAG(value, ae_value_member)) { // member
        if (emit_Expression(emit, member->base, 0) < 0) {
          err_msg(EMIT_, member->pos, "... in member function"); // LCOV_EXCL_START
          return -1;
        } // LCOV_EXCL_STOP
        sadd_instr(emit, Reg_Dup_Last);
        func_i = add_instr(emit, Dot_Member_Data);
        func_i->m_val = value->offset;
        func_i->m_val2 = Kindof_Int;
        func_i->ptr = (m_uint*)emit_addr;
        return 1;
      } else {
        push_i = add_instr(emit, Reg_Push_Imm);
        func_i = add_instr(emit, Dot_Static_Data);
        push_i->m_val = (m_uint)t_base;
        func_i->m_val = (m_uint)offset;
        func_i->m_val2 = (m_uint)kindof(value->m_type);
        func_i->ptr = (m_uint*)emit_addr;
      }
    } else if (isa(member->self->type, &t_function) > 0) { // function
      value = find_value(t_base, member->xid);
      func = value->func_ref;
      if (func->is_member) { // member
        if (emit_Expression(emit, member->base, 0) < 0) {
          err_msg(EMIT_, member->pos, "... in member function"); // LCOV_EXCL_LINE
          return -1;                                             // LCOV_EXCL_LINE
        }
        sadd_instr(emit, Reg_Dup_Last);
        func_i = add_instr(emit, Dot_Member_Func);
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
      if(GET_FLAG(value, ae_value_member)) { // member
        CHECK_BB(emit_Expression(emit, member->base, 0))
        instr = add_instr(emit, Dot_Member_Data);
        instr->m_val = offset;
        instr->m_val2 = kindof(value->m_type);
        instr->ptr = (m_uint*)emit_addr;
      } else { // static
        if (value->ptr && GET_FLAG(value, ae_value_import)) { // from C
          func_i = add_instr(emit, Dot_Static_Import_Data);
          func_i->m_val = (m_uint)value->ptr;
          func_i->m_val2 = kindof(value->m_type);
          func_i->ptr = (m_uint*)emit_addr;
        } else { // from code
          push_i = add_instr(emit, Reg_Push_Imm);
          func_i = add_instr(emit, Dot_Static_Data);
          push_i->m_val = (m_uint)t_base;
          func_i->m_val = (m_uint)offset;
          func_i->m_val2 = (m_uint)kindof(value->m_type);
          func_i->ptr = (m_uint*)emit_addr;
        }
      }
    }
  } else { // static
    if (isa(member->self->type, &t_function) > 0) {
      value = find_value(t_base, member->xid);
      func = value->func_ref;
      push_i = add_instr(emit, Reg_Push_Imm);
      func_i = add_instr(emit, Dot_Static_Func);
      push_i->m_val = (m_uint)t_base;
      func_i->m_val = (m_uint)func;
    } else {
      value = find_value(t_base, member->xid);
      if (value->ptr && GET_FLAG(value, ae_value_import)) {
        func_i = add_instr(emit, Dot_Static_Import_Data);
        func_i->m_val = (m_uint)value->ptr;
        func_i->m_val2 = kindof(value->m_type);
        func_i->ptr = (m_uint*)emit_addr;
      } else {
        push_i = add_instr(emit, Reg_Push_Imm);
        func_i = add_instr(emit, Dot_Static_Data);
        push_i->m_val = (m_uint)t_base;
        func_i->m_val = (m_uint)value->offset;
        func_i->m_val2 = (m_uint)kindof(value->m_type);
        func_i->ptr = (m_uint*)emit_addr;
      }
    }
  }
  return 1;
}

static m_bool emit_Func_Def(Emitter emit, Func_Def func_def)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "func def");
#endif
  Func func = func_def->func;
  Value value = func->value_ref;
  Type type = value->m_type;
  Local* local = NULL;

  if (func->code) {
    err_msg(EMIT_, func_def->pos, "function '%s' already emitted...", S_name(func_def->name)); // LCOV_EXCL_LINE
    return -1;                                                                                 // LCOV_EXCL_LINE
  }

  /*if (emit->env->func) {*/
  /*err_msg(EMIT_, func_def->pos,*/
  /*"internal error: nested function definition...", func_def->pos);*/
  /*return -1;*/
  /*}*/

  if (func_def->types) // don't check template definition
    return 1;

  if (!emit->env->class_def) {
    local = frame_alloc_local(emit->code->frame, value->m_type->size, value->name, 1, 0);
    value->offset = local->offset;
    Instr set_mem = add_instr(emit, Mem_Set_Imm);
    set_mem->m_val = value->offset;
    set_mem->ptr = func;
  }

  emit->env->func = func;
  vector_append(emit->stack, (vtype)emit->code);
  emit->code = new_Code();
  char c[256];
  sprintf(c, "%s%s%s(...)", emit->env->class_def ? emit->env->class_def->name : "", emit->env->class_def ? "." : " ", func->name);
  emit->code->name = strdup(c);
  emit->code->need_this = func->is_member;
  emit->code->filename = strdup(emit_filename);

  Arg_List a = func_def->arg_list;
  m_bool is_obj = 0;
  m_bool is_ref = 0;

  if(func->is_member) {
    emit->code->stack_depth += SZ_INT;
    if(!frame_alloc_local(emit->code->frame, SZ_INT, "this", 1, 0)) {
      err_msg(EMIT_, a->pos, "(emit): internal error: cannot allocate local 'this'..."); // LCOV_EXCL_START
      goto error;
    }                                                                                    // LCOV_EXCL_STOP
  }

  frame_push_scope(emit->code->frame);
  while (a) {
    value = a->var_decl->value;
    type = value->m_type;
    is_obj = !isprim(type);
    is_ref = a->type_decl->ref;
    emit->code->stack_depth += type->size;

    local = frame_alloc_local(emit->code->frame, type->size, value->name, is_ref, is_obj);

    if (!local) {
      err_msg(EMIT_, a->pos, "(emit): internal error: cannot allocate local '%s'...", value->name); // LCOV_EXCL_START
      goto error;
    }                                                                                               // LCOV_EXCL_STOP
    value->offset = local->offset;
    a = a->next;
  }
  if (func_def->is_variadic) {
    if (!frame_alloc_local(emit->code->frame, type->size, "vararg", is_ref, is_obj)) {
      err_msg(EMIT_, func_def->pos, "(emit): internal error: cannot allocate local 'vararg'..."); // LCOV_EXCL_START
      return -1;
    }                                                                                        // LCOV_EXCL_STOP
    emit->code->stack_depth += SZ_INT;
  }

  if(emit_Stmt(emit, func_def->code, 0) < 0)
    goto error;

  // ensure return
  if (func_def->ret_type && func_def->ret_type->xid != t_void.xid) {
    emit_func_release(emit); // /04/04/2017
    Kindof k = kindof(func_def->ret_type);
    f_instr f = Reg_Push_Imm;
	switch(k) {
		case Kindof_Int:
			f = Reg_Push_Imm;
            break;
		case Kindof_Float:
			f = Reg_Push_Imm2;
            break;
		case Kindof_Complex:
			f = Reg_Push_ImmC;
            break;
		case Kindof_Vec3:
			f = Reg_Push_ImmV3;
            break;
		case Kindof_Vec4:
			f = Reg_Push_ImmV4;
            break;
        case Kindof_Void: // won't reach
          goto error;
    }
    sadd_instr(emit, f);
    Instr goto_instr = add_instr(emit, Goto);
    emit_add_return(emit, goto_instr);
  }
  emit_pop_scope(emit);

  m_uint i;
  for (i = 0; i < vector_size(emit->code->stack_return); i++) {
    Instr instr = (Instr)vector_at(emit->code->stack_return, i);
    instr->m_val = vector_size(emit->code->code);
  }
  free_Vector(emit->code->stack_return);
  emit->code->stack_return = new_Vector();
  sadd_instr(emit, Func_Return);
  func->code = emit_to_code(emit);
  if (func->def->spec == ae_func_spec_dtor) {
    /*    func->code->stack_depth = SZ_INT;*/
    emit->env->class_def->info->dtor = func->code;
    emit->env->class_def->has_destructor = 1;
  } else if (func->def->spec == ae_func_spec_op)
    operator_set_func(emit->env, func, func->def->arg_list->type, func->def->arg_list->next->type);
  // add reference
  add_ref(&func->obj);
  emit->env->func = NULL;

  // delete the code ?
  /*  rem_ref(emit->code->obj, emit->code);*/
  //  free_Code(emit->code);
  emit->code = (Code*)vector_pop(emit->stack);
  return 1;
error:
  free_Code(emit->code);
  emit->code = (Code*)vector_pop(emit->stack);
  return -1;
}

static m_bool emit_Class_Def(Emitter emit, Class_Def class_def)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "class def");
#endif
  Type type = class_def->type;
  m_bool ret = 1;
  Class_Body body = class_def->body;

  if (type->info->pre_ctor != NULL && type->info->pre_ctor->instr != NULL) {
    err_msg(EMIT_, class_def->pos, "(emit): class '%s' already emitted...", type->name); // LCOV_EXCL_LINE
    return -1;                                                                           // LCOV_EXCL_LINE
  } 
  if(type->info->class_data_size) {
    type->info->class_data = calloc(type->info->class_data_size, sizeof(char));
    if (!type->info->class_data) {
      err_msg(EMIT_, class_def->pos, "OutOfMemory: while allocating static data '%s'\n", type->name); // LCOV_EXCL_LINE
      return -1;                                                                                      // LCOV_EXCL_LINE
     }
  } 
  memset(type->info->class_data, 0, type->info->class_data_size);
  // set the class
  vector_append(emit->env->class_stack, (vtype)emit->env->class_def);
  emit->env->class_def = type;
  vector_append(emit->stack, (vtype)emit->code);
  emit->code = new_Code();
  char c[256];
  //  emit->code->name = c;
  //  emit->code->name = malloc(sizeof(char));
  //  sprintf(emit->code->name, "class %s", type->name);
  sprintf(c, "class %s", type->name);
  emit->code->name = strdup(c);

  emit->code->need_this = 1;
  emit->code->filename = strdup(emit_filename);
  emit->code->stack_depth += SZ_INT;
  if (!frame_alloc_local(emit->code->frame, SZ_INT, "this", 1, 1)) {
    err_msg(EMIT_, class_def->pos, "(emit): internal error: cannot allocate local 'this'..."); // LCOV_EXCL_LINE
    return -1;                                                                                 // LCOV_EXCL_LINE
  }

  while (body && ret > 0) { 
    switch (body->section->type) {
    case ae_section_stmt:
      ret = emit_Stmt_List(emit, body->section->d.stmt_list);
      break;

    case ae_section_func:
      ret = emit_Func_Def(emit, body->section->d.func_def);
      break;

    case ae_section_class:
      ret = emit_Class_Def(emit, body->section->d.class_def);
      break;
    }
    body = body->next;
  }

  if (ret > 0) {
    sadd_instr(emit, Func_Return);
    free_VM_Code(type->info->pre_ctor);
    type->info->pre_ctor = emit_to_code(emit);
    /*    type->info->pre_ctor->add_ref();*/
  } else
    free(type->info->class_data); // LCOV_EXCL_LINE
  emit->env->class_def = (Type)vector_pop(emit->env->class_stack);
  // delete the code
  //    SAFE_DELETE(emit->code);
  emit->code = (Code*)vector_pop(emit->stack);
  return ret;
}

m_bool emit_Ast(Emitter emit, Ast ast, m_str filename)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "Ast %p", emit->env->context);
#endif
  Ast prog = ast;
  vtype i;
  int ret = 1;
  emit_filename = filename;
  emit->code = new_Code();
  emit->context = emit->env->context;
  emit->nspc = emit->context->nspc;
  emit->func = NULL;
  free_Vector(emit->stack);
  emit->stack = new_Vector();
  //  emit->code->name = strdup(emit_filename);
  frame_push_scope(emit->code->frame);
  sadd_instr(emit, start_gc);
  while (prog && ret > 0) {
    if (!prog->section)
      return 1;
    switch (prog->section->type) {
    case ae_section_stmt:
      ret = emit_Stmt_List(emit, prog->section->d.stmt_list);
      break;
    case ae_section_func:
      ret = emit_Func_Def(emit, prog->section->d.func_def);
      break;
    case ae_section_class:
      ret = emit_Class_Def(emit, prog->section->d.class_def);
      break;
    }
    prog = prog->next;
  }
  sadd_instr(emit, stop_gc);
  if(emit->cases)
    free_Map(emit->cases);
  // handle func pointer
  for(i = 0; i < vector_size(emit->funcs); i++) {
    Stmt_Ptr ptr = (Stmt_Ptr)vector_at(emit->funcs, i);
    scope_rem(emit->env->curr->func, ptr->xid);
  }
  // handle empty array type
  for(i = 0; i < vector_size(emit->array); i++) {
    Type t = (Type)vector_at(emit->array, i);
    if(!--t->obj.ref_count) {
      free(t);
    } // all this should be
      // rem_ref(t->obj, t);
  }
  vector_clear(emit->array);
  emit_pop_scope(emit);
  if (ret < 0) { // should free all stack.
//    for(i = 0; i < vector_size(emit->stack); i++)
//      free_Code((Code*)vector_at(emit->stack, i));
    for(i = 0; i < vector_size(emit->code->code); i++)
      free((Instr)vector_at(emit->code->code, i));
    free(filename);
    free_Code(emit->code);
    free_Ast(ast);
  }
  return ret;
}
