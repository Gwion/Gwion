#include <string.h> /* memcpy */
#include "emit.h"
#include "err_msg.h"
#include "instr.h"
#include "vm.h"
#include "context.h"

static m_str emit_filename;

void free_Expression(Expression a); // absyn.h

static m_bool emit_Expression(Emitter emit, Expression exp, m_bool add_ref);
static m_bool emit_Stmt(Emitter emit, Stmt* stmt, m_bool pop);
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
  emit->obj = new_VM_Object(e_emit_obj);
  emit->env = env;
  return emit;
}

void free_Emitter(Emitter a)
{
  vtype i;
  for(i = 0; i < vector_size(a->spork); i++) {
    Func f = (Func)vector_at(a->spork, i);
    rem_ref(f->obj, f);
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
  debug_msg("emit", "symbol %s (const:%i) %i %p", S_name(symbol), v->is_const, v->is_static, v->owner_class);
#endif
  Instr instr;
  // HACK : instantiate type
  if (v->m_type->xid == t_class.xid && !v->m_type->initialize) {
    instr = add_instr(emit, Reg_Push_Imm);
    instr->m_val = (m_uint)v->m_type;
    v->m_type->initialize = 1;
    return 1;
  }
  if (v->owner_class && (v->is_member || v->is_static)) {
    m_bool ret;
    Expression base = new_Primary_Expression_from_ID("this", pos);
    Expression dot = new_exp_from_member_dot(base, S_name(symbol), pos);
    base->type = v->owner_class;
    dot->type = v->m_type;
    dot->d.exp_dot->t_base = v->owner_class;
    dot->emit_var = emit_var;
    if((ret = emit_Dot_Member(emit, dot->d.exp_dot)) < 0)
      err_msg(EMIT_, pos, "(emit): internal error: symbol transformation failed...");
    free_Expression(dot);
    return ret;
  }

  /* HACK: constant value */
  if (v->is_const) {
    if (v->func_ref) {
      instr = add_instr(emit, Reg_Push_Imm);
      /*      exit(2);*/
      instr->m_val = (m_uint)v->func_ref;
      return 1;
    }

    if (isa(v->m_type, &t_float) > 0 || isa(v->m_type, &t_dur) > 0 || isa(v->m_type, &t_dur) > 0) {
      instr = add_instr(emit, Reg_Push_Imm2);
      /*      instr->f_val = (m_float)(m_uint)v->ptr;*/
      instr->f_val = *(m_float*)v->ptr;
      return 1;
    } else {
      instr = add_instr(emit, Reg_Push_Imm);
      instr->m_val = (m_uint)v->ptr;
    }
    return 1;
  }

  /*  if(isa(v->m_type, &t_func_ptr) > 0)*/
  /*  {*/
  /*    instr = add_instr(emit, Reg_Push_Imm);*/
  /*    instr->m_val = &v->func_ref;*/
  /*    return 1;  */
  /*    exit(12);*/
  /*  }*/

  if (emit_var) {
    instr = add_instr(emit, Reg_Push_Mem_Addr);
    instr->m_val = v->offset;
    instr->m_val2 = v->is_context_global;
    return 1;
  } else {
    Kindof kind = kindof(v->m_type);
    instr = add_instr(emit, NULL); // dangerous
    instr->m_val = v->offset;
    instr->m_val2 = v->is_context_global;

    if (v->func_ref) {
      instr->execute = Reg_Push_Imm;
      instr->m_val = (m_uint)v->func_ref;
    } else {
      if (isa(v->m_type, &t_vararg) > 0) {
        m_uint offset = 0;
        Arg_List l = emit->env->func->def->arg_list;
        while (l) {
          offset += l->type->size;
          l = l->next;
        }
        instr->m_val = offset;
      }
      if (kind == Kindof_Int)
        instr->execute = Reg_Push_Mem;
      else if (kind == Kindof_Float)
        instr->execute = Reg_Push_Mem2;
      else if (kind == Kindof_Complex)
        instr->execute = Reg_Push_Mem_Complex;
      else if (kind == Kindof_Vec3)
        instr->execute = Reg_Push_Mem_Vec3;
      else if (kind == Kindof_Vec4)
        instr->execute = Reg_Push_Mem_Vec4;
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
  Instr top, bottom, post;
  m_uint start_index = vector_size(emit->code->code);
  top = add_instr(emit, Instr_Pre_Ctor_Array_Top);
  top->ptr = type;
  emit_Pre_Ctor(emit, type);
  bottom = add_instr(emit, Instr_Pre_Ctor_Array_Bottom);
  top->m_val = vector_size(emit->code->code);
  bottom->m_val = start_index;
  post = add_instr(emit, Instr_Pre_Ctor_Array_Post);
  (void)post; //prevent cppcheck warning
  return 1;
}

static m_bool emit_instantiate_object(Emitter emit, Type type, Array_Sub array, m_bool is_ref)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "instantiate object");
#endif
  if (type->array_depth) {
    CHECK_BB(emit_Expression(emit, array->exp_list, 0))
    VM_Array_Info* info = calloc(1, sizeof(VM_Array_Info));
    info->depth = type->array_depth;
//    info->type = type->array_type;
    info->type = type;
    info->is_obj = isa(type->array_type, &t_object) > 0 ? 1 : 0;
    info->stack_offset = emit->code->frame->curr_offset;
    Instr alloc = add_instr(emit, Instr_Array_Alloc);
    alloc->ptr = info; // is it uszeful
    if (!is_ref && isa(type->array_type, &t_object) > 0)
      emit_pre_constructor_array(emit, type->array_type);
  } else if (isa(type, &t_object) > 0 && !is_ref) {
    Instr instr = add_instr(emit, Instantiate_Object);
    instr->ptr = type;
    emit_Pre_Ctor(emit, type);
  }
  return 1;
}

static Array_Sub array_seed = NULL;
static m_bool emit_array_lit(Emitter emit, Array_Sub array)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "array lit");
#endif
  m_uint count = 0;
  if (emit_Expression(emit, array->exp_list, 0) < 0) {
    array_seed = NULL;
    return -1;
  }
  if(!array_seed)
	array_seed = array;
  Expression e = array->exp_list;
  while (e) {
    count++;
    e = e->next;
  }

  e = array->exp_list;
  Type type = array->type;
  Instr instr = add_instr(emit, Instr_Array_Init);
  VM_Array_Info* info = calloc(1, sizeof(VM_Array_Info));
  info->type = type;
  info->length = count;
  instr->ptr = info;
  if(array == array_seed) {
    Local* l;
    array_seed = NULL;
    if(!(l = frame_alloc_local(emit->code->frame, SZ_INT, "litteral array", 0, 1))) {
      err_msg(EMIT_, array->pos, "can't allocate litteral array.");
      return -1;
    }
    instr->m_val = l->offset;
  }
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
    err_msg(EMIT_, array->pos,
            "internal error: array with 0 depth...");
    return -1;
  }
  sub = array->indices;
  if (!sub) {
    err_msg(EMIT_, array->pos,
            "internal error: NULL array sub...");
    return -1;
  }
  exp = sub->exp_list;
  if (!exp) {
    err_msg(EMIT_, array->pos,
            "internal error: NULL array exp...");
    return -1;
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
    add_instr(emit, Reg_Push_Imm2);
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
  Local* l; // remove litteral string
  /*  m_int temp;*/
  m_uint temp;
  m_float f;
  /*  exit(0);*/
  switch (primary->type) {
  case ae_primary_var:
    if (primary->d.var == insert_symbol("this"))
      instr = add_instr(emit, Reg_Push_This);
    else if (primary->d.var == insert_symbol("me"))
      instr = add_instr(emit, Reg_Push_Me);
    else if (primary->d.var == insert_symbol("now"))
      instr = add_instr(emit, Reg_Push_Now);
    else if (primary->d.var == insert_symbol("false"))
      instr = add_instr(emit, Reg_Push_Imm);
    else if (primary->d.var == insert_symbol("true")) {
      instr = add_instr(emit, Reg_Push_Imm);
      instr->m_val = 1;
    } else if (primary->d.var == insert_symbol("maybe"))
      instr = add_instr(emit, Reg_Push_Maybe);
    else if (primary->d.var == insert_symbol("null") || primary->d.var == insert_symbol("NULL"))
      instr = add_instr(emit, Reg_Push_Imm);
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

  case ae_primary_str: // modified 13/01/17 'get rid of litteral strings'
    memcpy(&temp, &primary->d.str, sizeof(temp));
    l = frame_alloc_local(emit->code->frame, SZ_INT, (m_str)temp, 0, 1);
    instr = add_instr(emit, Reg_Push_Str);
    instr->m_val = temp;
    instr->m_val2 = l->offset;;
    break;

  case ae_primary_array:
    CHECK_BB(emit_array_lit(emit, primary->d.array))
    break;
  case ae_primary_nil:
    instr = add_instr(emit, Reg_Push_Imm);
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
  default:
    err_msg(EMIT_, primary->pos, "(emit): unhandled primary type '%i'...", primary->type);
    return -1;
  }
  return 1;
}

static m_bool emit_Decl_Expression(Emitter emit, Decl_Expression* decl)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "decl");
#endif
  Decl_Expression* exp = decl;
  Var_Decl_List list;
  Var_Decl var_decl;
  Type type;
  Kindof kind;
  Local* local;
  Value value;
  Instr instr;
  m_bool is_ref;
  m_bool is_obj;
  //  m_bool is_init;
  list = exp->list;
  while (list) {
    var_decl = list->self;
    value = var_decl->value;
    type = value->m_type;
    is_obj = isa(type, &t_object) > 0 || list->self->array;
    is_ref = decl->type->ref;
    //    is_init = 0;
    kind = kindof(type);

    if (is_obj) {
      if (list->self->array) {
        if (list->self->array->exp_list) {
          //          is_init = 1;
          if (emit_instantiate_object(emit, type, list->self->array, is_ref) < 0)
            return -1;
        } else if (!value->owner_class)
          vector_append(emit->array, (vtype)decl->m_type);
      } else if (!is_ref) {
        //        is_init = 1;
        if (emit_instantiate_object(emit, type, list->self->array, is_ref) < 0)
          return -1;
      }
    }
    if (value->is_member) {
      Instr alloc_m = add_instr(emit, NULL);
      if (kind == Kindof_Int)
        alloc_m->execute = Alloc_Member_Word;
      else if (kind == Kindof_Float)
        alloc_m->execute = Alloc_Member_Word_Float;
      else if (kind == Kindof_Complex)
        alloc_m->execute = Alloc_Member_Word_Complex;
      else if (kind == Kindof_Vec3)
        alloc_m->execute = Alloc_Member_Word_Vec3;
      else if (kind == Kindof_Vec4)
        alloc_m->execute = Alloc_Member_Word_Vec4;
      alloc_m->m_val = value->offset;
    } else {
      if (!emit->env->class_def || !decl->is_static) {
        local = frame_alloc_local(emit->code->frame, decl->m_type->size, value->name, is_ref, is_obj);
        if (!local)
          return -1;
        value->offset = local->offset;
        instr = add_instr(emit, Alloc_Word);
        instr->m_val = local->offset;
        /*        instr->m_val2 = local->is_obj;*/
        instr->m_val2 = value->is_context_global;
        if (type->xid == t_float.xid || type->xid == t_time.xid || type->xid == t_dur.xid)
          instr->execute = Alloc_Word_Float;
        if (type->xid == t_complex.xid || type->xid == t_polar.xid)
          instr->execute = Alloc_Word_Complex;
        else if (isa(type, &t_vec3) > 0)
          instr->execute = Alloc_Word_Vec3;
        else if (isa(type, &t_vec4) > 0)
          instr->execute = Alloc_Word_Vec4;
      } else { // static
        if (is_obj) {
          Code* code = emit->code;
          emit->code = (Code*)vector_back(emit->stack);
          if (emit_instantiate_object(emit, type, list->self->array, is_ref) < 0)
            return -1;
          Instr push = add_instr(emit, Reg_Push_Imm);
          push->m_val = (m_uint)emit->env->class_def;
          Instr dot_static = add_instr(emit, Dot_Static_Data);
          dot_static->m_val = value->offset;
          dot_static->m_val2 = kindof(emit->env->class_def);
          dot_static->ptr = (m_uint*)1;
          instr = add_instr(emit, Assign_Object);
          emit->code = code;
          return 1;
        } else {
          // emit the type
          Instr push = add_instr(emit, Reg_Push_Imm);
          push->m_val = (m_uint)emit->env->class_def;
          /*          Instr dot_static   = add_instr(emit, Alloc_Dot_Static_Data);*/
          Instr dot_static = add_instr(emit, Dot_Static_Data);
          dot_static->m_val = value->offset;
          dot_static->m_val2 = kindof(emit->env->class_def);
          /*          dot_static->ptr    = emit->env->class_def; //hack*/
          dot_static->ptr = (m_uint*)1;
        }
      }
    }
    //   is_init = 0;
    // if object, assign
    if (is_obj) {
      // if array
      /*      if( decl->list->self->array )*/
      if (list->self->array) {
        // if not []
        /*        if( decl->list->self->array->exp_list )*/
        if (list->self->array->exp_list) {
          /*          exit(0);*/
          // set
          //          is_init = 1;
          // assign
          Instr assign = add_instr(emit, Assign_Object);
          assign->ptr = (m_uint*)1;
          assign->m_val = value->offset;
        }
      }
      /*      else if( !is_ref && !value->is_member)*/
      else if (!is_ref) {
        //        is_init = 1;
        Instr assign = add_instr(emit, Assign_Object);
        assign->ptr = (m_uint*)1;
        assign->m_val = value->offset;
        /*        assign->m_val2 = value->is_context_global;*/
      }
    }
    /*
    // HACK func pointer
    if(isa(type, &t_func_ptr) > 0)
    {
    if(!emit->env->class_def || !decl->is_static)
    {
    Instr set = add_instr(emit, Mem_Set_Imm);
    set->ptr = namespace_lookup_func(emit->env->curr, insert_symbol(value->m_type->name), -1);
    set->m_val = value->offset;

    }
    }
    */

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
  if (binary->op == op_at_chuck && isa(binary->lhs->type, &t_function) > 0 && isa(binary->rhs->type, &t_func_ptr) > 0) {
    CHECK_BB(emit_Expression(emit, binary->lhs, 1))
    CHECK_BB(emit_Expression(emit, binary->rhs, 1))
    instr = add_instr(emit, assign_func);
    return 1;
  }
  CHECK_BB(emit_Expression(emit, binary->lhs, 1))
  CHECK_BB(emit_Expression(emit, binary->rhs, 1))

  if (binary->op == op_chuck && isa(binary->rhs->type, &t_function) > 0)
    return emit_Func_Call1(emit, binary->func, binary->func->value_ref->m_type, binary->pos);

  // arrays
  if (binary->op == op_shift_left && (binary->lhs->type->array_depth == binary->rhs->type->array_depth + 1)
      /*)*/
      && isa(binary->lhs->type->array_type, binary->rhs->type) > 0) {
    /*    exit(2);*/
    instr = add_instr(emit, Array_Append);
    instr->m_val = kindof(binary->rhs->type);
    return 1;
  }

  if (binary->lhs->type->array_depth || binary->rhs->type->array_depth) {
    if (binary->op == op_at_chuck && binary->lhs->type->array_depth == binary->rhs->type->array_depth)
      instr = add_instr(emit, Assign_Object);
    return 1;
  }

  if (isa(binary->rhs->type, &t_object) > 0 && isa(binary->lhs->type, &t_null) > 0 && binary->op == op_at_chuck) {
    instr = add_instr(emit, Assign_Object);
    return 1;
  }
  (void)instr; // prevent cppcheck warning
  CHECK_BB(get_instr(emit, binary->op, binary->lhs->type, binary->rhs->type))
  return 1;
}

static m_bool emit_Cast_Expression1(Emitter emit, Type to, Type from)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "cast %s to %s", from->name, to->name);
#endif
  Instr instr;
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
    err_msg(EMIT_, 0, "cannot cast '%s' to '%s'", from->name, to->name);
    return -1;
  }
  instr = add_instr(emit, f);
  (void)instr; // prevent cppcheck warning
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
    if (cast->exp->exp_type == Primary_Expression_type) {
      char name[1024];
      m_str nspc = strdup(from->name);
      strsep(&nspc, "@");
      strsep(&nspc, "@");
      sprintf(name, "%s@%i@%s", S_name(cast->exp->d.exp_primary->d.var), 0, nspc);
      free(nspc); // coverity
      Instr push = add_instr(emit, Reg_Push_Imm);
      push->m_val = (m_uint)cast->func;
      return 1;
    }
    /*    return emit_Expression( emit, cast->exp, 0);*/
  }
  CHECK_BB(emit_Expression(emit, cast->exp, 0))
  return emit_Cast_Expression1(emit, to, from);
}

static m_bool emit_Postfix_Expression(Emitter emit, Postfix_Expression* postfix)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "postfix");
#endif
  Instr instr;
  f_instr f;
  CHECK_BB(emit_Expression(emit, postfix->exp, 0))

  //  return
  /*get_instr(emit, postfix->op, postfix->exp->type, NULL);*/
  // emit
  switch (postfix->op) {
  case op_plusplus:
    if (postfix->exp->type->xid == t_int.xid)
      f = inc;
    else {
      err_msg(EMIT_, postfix->pos,
              "(emit): internal error: unhandled type '%s' for post '++' operator",
              postfix->exp->type->name);
      return -1;
    }
    break;

  case op_minusminus:
    if (postfix->exp->type->xid == t_int.xid)
      f = dec;
    else {
      err_msg(EMIT_, postfix->pos,
              "(emit): internal error: unhandled type '%s' for post '--' operator",
              postfix->exp->type->name);
      return -1;
    }
    break;

  default:
    err_msg(EMIT_, postfix->pos,
            "(emit): internal error: unhandled postfix operator '%s'",
            op2str(postfix->op));
    return -1;
  }
  instr = add_instr(emit, f);
  (void)instr; // prevent cppcheck warning
  return 1;

  /*²  err_msg(EMIT_, postfix->pos,*/
  /*    "(emit): internal error: unhandled postfix operator '%s'",*/
  /*    op2str(postfix->op));*/
  /*  return -1;*/
}

static m_bool emit_Dur(Emitter emit, Exp_Dur* dur)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "dur");
#endif
  Instr cast, mul;
  CHECK_BB(emit_Expression(emit, dur->base, 0))
  if (isa(dur->base->type, &t_int) > 0)
    cast = add_instr(emit, Cast_i2f);
  CHECK_BB(emit_Expression(emit, dur->unit, 0))
  mul = add_instr(emit, timesf);
  (void)cast, (void)mul; // prevent cppcheck warning
  return 1;
}

/* static */ m_bool emit_Func_Call1(Emitter emit, Func func, Type type, int pos)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "func call1. '%s' offset: %i", func->name, emit->code->frame->curr_offset);
#endif
  Instr code, offset, call;
  if (!func->code) { // calling function pointer in func
    Func f = namespace_lookup_func(emit->env->curr, insert_symbol(func->name), -1);
// [todo] emit_exp_func1 remove template stuff
    if (!f) { //template with no list
      if (!func->def->is_template) {
        err_msg(EMIT_, func->def->pos, "function not emitted yet");
        return -1;
      }
      if(emit_Func_Def(emit, func->def) < 0) {
        err_msg(EMIT_, 0, "can't emit func.");
        return -1;
      }
      func->code = func->def->func->code;
      code = add_instr(emit, Reg_Push_Ptr);
      code->ptr = func->code;
    } else {
      code = add_instr(emit, Reg_Push_Code);
      code->m_val = f->value_ref->offset;
    }
  } else {
    code = add_instr(emit, Reg_Push_Ptr);
    code->ptr = func->code;
  }

  if(!emit->code->stack_depth && !emit->code->frame->curr_offset)
    add_instr(emit, Mem_Push_Imm);
  offset = add_instr(emit, Reg_Push_Imm);
  offset->m_val = emit->code->frame->curr_offset;
  call = add_instr(emit, Instr_Func_Call);
  call->m_val = emit->code->stack_depth;
  if (func->def->s_type == ae_func_builtin) {
    call->m_val = kindof(type);
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

  // test on 13/01/17 'Keep track of returned Object'
  if(isa(func->def->ret_type, &t_object) > 0) {
    Local* l = frame_alloc_local(emit->code->frame, SZ_INT, func->name, 0, 1);
    Instr instr = add_instr(emit, Mem_Push_Ret);
    instr->m_val = l->offset;
  }

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
    err_msg(EMIT_, exp->pos,
            "(emit): internal error in evaluating function call...");
    return -1;
  }
  vector_append(emit->stack, (vtype)emit->code);
  emit->code = new_Code();
  emit->code->need_this = exp->m_func->is_member;
  emit->code->name = strdup("spork~exp");
  emit->code->filename = strdup(emit_filename);
  op = add_instr(emit, Mem_Push_Imm);
  CHECK_BB(emit_Func_Call1(emit, exp->m_func, exp->ret_type, exp->pos))
  add_instr(emit, EOC);
  op->m_val = emit->code->stack_depth;

  code = emit_to_code(emit);
  exp->vm_code = code;
  //exp->ck_vm_code->add_ref();
  emit->code = (Code*)vector_back(emit->stack);
  vector_pop(emit->stack);

  Expression e = exp->args;
  m_uint size = 0;
  while (e) {
    size += e->cast_to ? e->cast_to->size : e->type->size;
    e = e->next;
  }

  if(emit->code->need_this)
    size += SZ_INT;

  push_code = add_instr(emit, Reg_Push_Imm);
  push_code->m_val = (m_uint)code;
  spork = add_instr(emit, Spork);
  spork->m_val = size;
  spork->m_val2 = (m_uint)exp->m_func; // HACK
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
    if (exp_unary->self->meta != ae_meta_var || (exp_unary->self->exp_type == Primary_Expression_type && exp_unary->self->d.exp_primary->value->is_const)) { // TODO: check const
      err_msg(EMIT_, exp_unary->self->pos,
              "(emit): target for '++' not mutable...");
      return -1;
    }
    if (isa(exp_unary->exp->type, &t_int) > 0)
      instr = add_instr(emit, inc);
    break;
  case op_minusminus:
    if (exp_unary->self->meta != ae_meta_var || (exp_unary->self->exp_type == Primary_Expression_type && exp_unary->self->d.exp_primary->value->is_const)) { // TODO: check const
      err_msg(EMIT_, exp_unary->self->pos,
              "(emit): target for '--' not mutable...");
      return -1;
    }
    if (isa(exp_unary->exp->type, &t_int) > 0)
      instr = add_instr(emit, dec);
    break;
  case op_exclamation:
    if (isa(exp_unary->exp->type, &t_int) > 0 || isa(exp_unary->self->type, &t_object) > 0)
      instr = add_instr(emit, not);
    else if (isa(exp_unary->exp->type, &t_float) > 0 || isa(t, &t_time) > 0 || isa(t, &t_dur) > 0)
      instr = add_instr(emit, notf);
    else {
      err_msg(EMIT_, exp_unary->self->pos,
              "(emit): internal error: unhandled type '%s' for ! operator",
              exp_unary->self->type->name);
      return -1;
    }
    break;

  case op_spork:
    if (exp_unary->exp && exp_unary->exp->exp_type == Func_Call_type) {
      if (emit_spork(emit, exp_unary->exp->d.exp_func) < 0)
        return -1;
    }
    // spork ~ { ... }
    else if (exp_unary->code) {

      Instr op = NULL, push_code = NULL, spork = NULL;
      VM_Code code;
      ID_List list = new_id_list("sporked", exp_unary->pos);
      Func f = new_Func("sporked", new_Func_Def(0, 0, new_Type_Decl(list, 0, exp_unary->pos), "sporked", NULL, exp_unary->code, exp_unary->pos));

      if (emit->env->class_def)
        add_instr(emit, Reg_Push_This);
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
      CHECK_BB(emit_Stmt(emit, exp_unary->code, 0))
      emit_pop_scope(emit);
      op->m_val = emit->code->stack_depth;
      instr = add_instr(emit, EOC);
      op->m_val = emit->code->stack_depth;
      code = emit_to_code(emit);
      emit->code = (Code*)vector_back(emit->stack);
      vector_pop(emit->stack);
      push_code = add_instr(emit, Reg_Push_Imm);
      push_code->m_val = (m_uint)code;
      spork = add_instr(emit, Spork);
      spork->ptr = (m_uint*)(emit->env->func ? emit->env->func->def->stack_depth : 0); // don't push func info on the stack
    } else {
      err_msg(EMIT_, exp_unary->pos,
              "(emit): internal error: sporking non-function call...");
      return -1;
    }
    break;

  case op_minus:
    if (isa(exp_unary->self->type, &t_int) > 0)
      instr = add_instr(emit, negate);
    else if (isa(exp_unary->self->type, &t_float) > 0)
      instr = add_instr(emit, negatef);
    else {
      err_msg(EMIT_, exp_unary->pos,
              "(emit): internal error: unhandled type '%s' for exp_unary '-' operator",
              exp_unary->exp->type->name);
      return -1;
    }
    break;

  case op_new:
    if (isa(exp_unary->self->type, &t_object) > 0)
      CHECK_BB(emit_instantiate_object(emit, exp_unary->self->type, exp_unary->array, exp_unary->type->ref))
      break;
  default:
    err_msg(EMIT_, exp_unary->pos,
            "(emit): internal error: unhandled type '%s' for exp_unary '%s' operator", op2str(exp_unary->op));
    return -1;
  }
  (void)instr;
  return 1;
}

static m_bool emit_Func_Args(Emitter emit, Func_Call* exp_func)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "func args");
#endif
  if (emit_Expression(emit, exp_func->args, 1) < 0) {
    err_msg(EMIT_, exp_func->pos, "(emit): internal error in emitting function call arguments...");
    return -1;
  }
  if (exp_func->m_func->def->is_variadic) {
    /*    exit(6);*/
    m_uint offset = 0, size = 0;
    Expression e = exp_func->args;
    Arg_List l = exp_func->m_func->def->arg_list;
    Vector kinds = new_Vector();
    while (e) {
      if (!l) {
        /*        Instr instr  = add_instr(emit, Reg_Pop_Word4);*/
        /*        instr->m_val = e->type->size;*/
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
  if (exp_func->types) {
    if (exp_func->m_func->value_ref->owner_class) {
      vector_append(emit->env->nspc_stack, (vtype)emit->env->curr);
      emit->env->curr = exp_func->m_func->value_ref->owner_class->info;
      vector_append(emit->env->class_stack, (vtype)emit->env->class_def);
      emit->env->class_def = exp_func->m_func->value_ref->owner_class;
      emit->env->class_scope = 0;
    }
//    ID_List base_t = exp_func->base;
    ID_List base_t = exp_func->m_func->def->base;
    Type_List list = exp_func->types;
    namespace_push_type(emit->env->curr);
    // [template] - if overloaded, find the rigth 'base' (def->types)
    // [todo] - fix templates
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
    if (exp_func->m_func->value_ref->owner_class) {
      emit->env->class_def = (Type)vector_back(emit->env->class_stack);
      vector_pop(emit->env->class_stack);
      emit->env->curr = (NameSpace)vector_back(emit->env->nspc_stack);
      vector_pop(emit->env->nspc_stack);
    }
  }

  if (exp_func->args && !spork) {
    if (emit_Func_Args(emit, exp_func) < 0) {
      err_msg(EMIT_, exp_func->pos,
              "internal error in evaluating function arguments...");
      return -1;
    }
  }
  if (emit_Expression(emit, exp_func->func, 0) < 0) {
    err_msg(EMIT_, exp_func->pos,
            "internal error in evaluating function call...");
    return -1;
  }
  return emit_Func_Call1(emit, exp_func->m_func, exp_func->ret_type, exp_func->pos);
}

static m_bool emit_implicit_cast(Emitter emit, Type from, Type to)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "implicit cast '%s' to '%s'", from->name, to->name);
#endif
  Instr instr;
  if (from->xid == t_int.xid && to->xid == t_float.xid)
    instr = add_instr(emit, Cast_i2f);
  else if (from->xid == t_float.xid && to->xid == t_int.xid)
    instr = add_instr(emit, Cast_f2i);
  else if (isa(to, from) < 0 && isa(from, to) < 0) {
    err_msg(EMIT_, 0, "(emit): internal error: cannot cast type '%s' to '%s'", from->name, to->name);
    return -1;
  }
  (void)instr; // prevent cppcheck warning
  return 1;
}

static m_bool emit_exp_if(Emitter emit, If_Expression* exp_if)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "expression if");
#endif
  m_bool ret;
  Instr instr, op = NULL, op2 = NULL;
  f_instr fop;
  namespace_push_value(emit->env->curr);
  CHECK_BB(emit_Expression(emit, exp_if->cond, 0))
  switch (exp_if->cond->type->xid) {
  case te_int:
    instr = add_instr(emit, Reg_Push_Imm);
    fop = Branch_Eq_Int;
    break;
  case te_float:
  case te_dur:
  case te_time:
    instr = add_instr(emit, Reg_Push_Imm2);
    fop = Branch_Eq_Float;
    break;

  default:
    err_msg(EMIT_, exp_if->cond->pos,
            "(emit): internal error: unhandled type '%s' in if condition",
            exp_if->cond->type->name);
    return -1;
  }
  (void)instr;
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
      CHECK_BB(emit_Decl_Expression(emit, tmp->d.exp_decl))
      break;
    case Primary_Expression_type:
      CHECK_BB(emit_Primary_Expression(emit, tmp->d.exp_primary))
      break;
    case Unary_Expression_type:
      CHECK_BB(emit_Unary(emit, tmp->d.exp_unary))
      break;
    case Binary_Expression_type:
      CHECK_BB(emit_Binary_Expression(emit, tmp->d.exp_binary))
      break;
    case Postfix_Expression_type:
      CHECK_BB(emit_Postfix_Expression(emit, tmp->d.exp_postfix))
      break;
    case Cast_Expression_type:
      CHECK_BB(emit_Cast_Expression(emit, tmp->d.exp_cast))
      break;
    case Dot_Member_type:
      CHECK_BB(emit_Dot_Member(emit, tmp->d.exp_dot))
      break;
    case Func_Call_type:
      CHECK_BB(emit_Func_Call(emit, tmp->d.exp_func, 0))
      break;
    case Array_Expression_type:
      CHECK_BB(emit_Array(emit, tmp->d.exp_array))
      break;
    case If_Expression_type:
      CHECK_BB(emit_exp_if(emit, tmp->d.exp_if))
      break;
    case Dur_Expression_type:
      CHECK_BB(emit_Dur(emit, tmp->d.exp_dur))
      break;
    default:
      err_msg(EMIT_, tmp->pos, "unhandled expression type '%i'\n", tmp->exp_type);
      return -1;
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
  /*  m_bool ret = 1;*/
  Instr instr, op = NULL, op2 = NULL;
  f_instr f;
  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Expression(emit, stmt->cond, 0))

  switch (stmt->cond->type->xid) {

  case te_int:
    instr = add_instr(emit, Reg_Push_Imm);
    f = Branch_Eq_Int;
    break;
  case te_float:
  case te_dur:
  case te_time:
    instr = add_instr(emit, Reg_Push_Imm2);
    f = Branch_Eq_Float;
    break;

  default:
    // check for IO
    /*      if( isa( stmt->cond->type, &t_io ) )*/
    /*      {*/
    /*          // push 0*/
    /*          emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );*/
    /*          op = new Chuck_Instr_Branch_Eq_int_IO_good( 0 );*/
    /*          break;*/
    /*      }*/

    if (isa(stmt->cond->type, &t_object) > 0) {
      instr = add_instr(emit, Reg_Push_Imm);
      f = Branch_Eq_Int;
      break;
    }
    err_msg(EMIT_, stmt->cond->pos,
            "(emit): internal error: unhandled type '%s' in if condition",
            stmt->cond->type->name);
    return -1;
  }
  (void)instr;
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
  // TODO: push
  return 1;
}

static m_bool emit_Return(Emitter emit, Stmt_Return stmt)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "return");
#endif
  CHECK_BB(emit_Expression(emit, stmt->val, 0))
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

//  Instr instr, op = new_Instr();
  Instr instr, op;
  Instr goto_;
  f_instr f;
  frame_push_scope(emit->code->frame);
  emit_add_cont(emit, NULL);
  emit_add_break(emit, NULL);

  CHECK_BB(emit_Expression(emit, stmt->cond, 0))

  switch (stmt->cond->type->xid) {
  case te_int:
    instr = add_instr(emit, Reg_Push_Imm);
    f = Branch_Eq_Int;
    break;

  case te_float:
  case te_dur:
  case te_time:
    instr = add_instr(emit, Reg_Push_Imm2);
    f = Branch_Eq_Float;
    break;

  default:
    err_msg(EMIT_, stmt->cond->pos,
            "(emit): internal error: unhandled type '%s' in while conditional",
            stmt->cond->type->name);
    return -1;
  }
  (void)instr;
  op = add_instr(emit, f);
  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Stmt(emit, stmt->body, 0))
  emit_pop_scope(emit);

  goto_ = add_instr(emit, Goto);
  goto_->m_val = index;
  op->m_val = vector_size(emit->code->code);
  while (vector_size(emit->code->stack_cont) && vector_back(emit->code->stack_cont)) {
    Instr instr = (Instr)vector_back(emit->code->stack_cont);
    instr->m_val = index;
    vector_pop(emit->code->stack_cont);
  }
  while (vector_size(emit->code->stack_break) && vector_back(emit->code->stack_break)) {
    Instr instr = (Instr)vector_back(emit->code->stack_break);
    instr->m_val = vector_size(emit->code->code);
    vector_pop(emit->code->stack_break);
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
  Instr instr, op = NULL;
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

  // the condition
  switch (stmt->cond->type->xid) {
  case te_int:
    // push 0
    instr = add_instr(emit, Reg_Push_Imm);
    f = Branch_Neq_Int;
    break;
  case te_float:
  case te_dur:
  case te_time:
    instr = add_instr(emit, Reg_Push_Imm);
    f = Branch_Neq_Float;
    break;
  default:
    // check for IO
    /*        if( isa( stmt->cond->type, &t_io ) )*/
    /*        {*/
    /*            // push 0*/
    /*            emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );*/
    /*            op = new Chuck_Instr_Branch_Eq_int_IO_good( 0 );*/
    /*            break;*/
    /*        }*/

    err_msg(EMIT_, stmt->cond->pos,
            "(emit): internal error: unhandled type '%s' in do/while conditional",
            stmt->cond->type->name);
    return -1;
  }
  /*  emit_add_code(emit, op);*/
  (void)instr;
  op = add_instr(emit, f);
  op->m_val = index;
  while (vector_size(emit->code->stack_cont) && vector_back(emit->code->stack_cont)) {
    Instr instr = (Instr)vector_back(emit->code->stack_cont);
    instr->m_val = index;
    vector_pop(emit->code->stack_cont);
  }
  while (vector_size(emit->code->stack_break) && vector_back(emit->code->stack_break)) {
    Instr instr = (Instr)vector_back(emit->code->stack_break);
    instr->m_val = vector_size(emit->code->code);
    vector_pop(emit->code->stack_break);
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
  /*  t_CKBOOL ret = TRUE;*/
  Instr instr, op = NULL;
  f_instr f;
  frame_push_scope(emit->code->frame);

  m_uint index = vector_size(emit->code->code);
  vector_append(emit->code->stack_cont, (vtype)NULL);
  vector_append(emit->code->stack_break, (vtype)NULL);

  CHECK_BB(emit_Expression(emit, stmt->cond, 0))

  // condition
  switch (stmt->cond->type->xid) {
  case te_int:
    instr = add_instr(emit, Reg_Push_Imm);
    f = Branch_Neq_Int;
    break;
  case te_float:
  case te_dur:
  case te_time:
    instr = add_instr(emit, Reg_Push_Imm2);
    f = Branch_Neq_Float;
    break;

  default:
    // check for IO
    /*      if( isa( stmt->cond->type, &t_io ) )*/
    /*      {*/
    // push 0
    /*          emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );*/
    /*          op = new Chuck_Instr_Branch_Neq_int_IO_good( 0 );*/
    /*          break;*/
    /*      }*/

    err_msg(EMIT_, stmt->cond->pos,
            "(emit): internal error: unhandled type '%s' in until conditional",
            stmt->cond->type->name);
    return -1;
  }
  // append the op
  /*  emit_add_code(emit, op);*/
  (void)instr;
  op = add_instr(emit, f);
  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);

  Instr _goto = add_instr(emit, Goto);
  _goto->m_val = index;
  op->m_val = vector_size(emit->code->code);

  while (vector_size(emit->code->stack_cont) && vector_back(emit->code->stack_cont)) {
    Instr instr = (Instr)vector_back(emit->code->stack_cont);
    instr->m_val = index;
    vector_pop(emit->code->stack_cont);
  }
  while (vector_size(emit ->code->stack_break) && vector_back(emit->code->stack_break)) {
    Instr instr = (Instr)vector_back(emit->code->stack_break);
    instr->m_val = vector_size(emit->code->code);
    vector_pop(emit->code->stack_break);
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
  Instr instr, op = NULL;
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
    instr = add_instr(emit, Reg_Push_Imm);
    f = Branch_Eq_Int;
    break;
  case te_float:
  case te_dur:
  case te_time:
    instr = add_instr(emit, Reg_Push_Imm);
    f = Branch_Eq_Float;
    break;

  default:
    err_msg(EMIT_, stmt->cond->pos,
            "(emit): internal error: unhandled type '%s' in do/until conditional",
            stmt->cond->type->name);
    return -1;
  }
  /*  emit_add_code(emit, op);*/
  (void)instr;
  op = add_instr(emit, f);
  /*  op->m_val = vector_size(emit->code->code);*/
  op->m_val = index;

  while (vector_size(emit->code->stack_cont) && vector_back(emit->code->stack_cont)) {
    Instr instr = (Instr)vector_back(emit->code->stack_cont);
    instr->m_val = index;
    vector_pop(emit->code->stack_cont);
  }
  while (vector_size(emit->code->stack_break) && vector_back(emit->code->stack_break)) {
    Instr instr = (Instr)vector_back(emit->code->stack_break);
    instr->m_val = vector_size(emit->code->code);
    vector_pop(emit->code->stack_break);
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
  f_instr f = NULL;
  Instr instr, op = NULL;

  // push the stack
  frame_push_scope(emit->code->frame);
  // emit the cond
  CHECK_BB(emit_Stmt(emit, stmt->c1, 1))

  m_uint index = vector_size(emit->code->code);
  vector_append(emit->code->stack_cont, (vtype)NULL);
  vector_append(emit->code->stack_break, (vtype)NULL);

  // emit the cond - keep the result on the stack
  CHECK_BB(emit_Stmt(emit, stmt->c2, 0))
  if (stmt->c2) {
    switch (stmt->c2->d.stmt_exp->type->xid) {
    case te_int:
      instr = add_instr(emit, Reg_Push_Imm);
      f = Branch_Eq_Int;
      break;
    case te_float:
    case te_dur:
    case te_time:
      instr = add_instr(emit, Reg_Push_Imm2);
      f = Branch_Eq_Float;
      break;

    default:
      // check for IO
      /*        if( isa( stmt->c2->d.stmt_exp->type, &t_io ) )*/
      /*        {*/
      // push 0
      /*          emit->append( new Chuck_Instr_Reg_Push_Imm( 0 ) );*/
      /*          op = new Chuck_Instr_Branch_Eq_int_IO_good( 0 );*/
      /*          break;*/
      /*        }*/

      err_msg(EMIT_, stmt->c2->d.stmt_exp->pos,
              "(emit): internal error: unhandled type '%s' in for conditional",
              stmt->c2->d.stmt_exp->type->name);
      return -1;
    }
    (void)instr;
    // append the op
    op = add_instr(emit, f);
  }

  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Stmt(emit, stmt->body, 1))
  emit_pop_scope(emit);
  // emit the action
  if(stmt->c3) {
    CHECK_BB(emit_Expression(emit, stmt->c3, 0))

    // HACK!
    Expression e = stmt->c3;
    m_uint num_words = 0;
    while (e) {
      if (e->type->size == SZ_FLOAT)
        num_words += SZ_FLOAT;
      else if (e->type->size == SZ_INT)
        num_words += SZ_INT;
      else if (e->type->size == SZ_COMPLEX)
        num_words += SZ_COMPLEX;
      else if (e->type->size != 0) {
        err_msg(EMIT_, e->pos,
                "(emit): internal error: non-void type size '%i' unhandled...",
                e->type->size);
        return -1;
      }
      //      num_words++;
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

  /*(void)instr;*/
  if (stmt->c2)
    op->m_val = vector_size(emit->code->code);

  while (vector_size(emit->code->stack_cont) && vector_back(emit->code->stack_cont)) {
    Instr instr = (Instr)vector_back(emit->code->stack_cont);
    instr->m_val = index;
    vector_pop(emit->code->stack_cont);
  }
  while (vector_size(emit->code->stack_break) && vector_back(emit->code->stack_break)) {
    Instr instr = (Instr)vector_back(emit->code->stack_break);
    instr->m_val = vector_size(emit->code->code);
    vector_pop(emit->code->stack_break);
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
  Instr init = NULL, op = NULL, deref, tmp = NULL, dec = NULL, _goto = NULL;
  m_int* counter = calloc(1, sizeof(m_int));
  m_uint index;
  Type type;

  frame_push_scope(emit->code->frame);
  /* coverity[leaked_storage] */
  CHECK_BB(emit_Expression(emit, stmt->cond, 0))
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
    add_instr(emit, Reg_Push_Imm);
    op = new_Instr();
    op->execute = Branch_Eq_Int;
    break;

  default:
    err_msg(EMIT_, stmt->cond->pos,
            "(emit): internal error: unhandled type '%s' in while conditional", type->name);
    return -1;
  }

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
    Instr instr = (Instr)vector_back(emit->code->stack_cont);
    instr->m_val = index;
    vector_pop(emit->code->stack_cont);
  }
  while (vector_size(emit->code->stack_break) && vector_back(emit->code->stack_break)) {
    Instr instr = (Instr)vector_back(emit->code->stack_break);
    instr->m_val = vector_size(emit->code->code);
    vector_pop(emit->code->stack_break);
  }
  emit_pop_scope(emit);
  vector_pop(emit->code->stack_cont);
  vector_pop(emit->code->stack_break);
  tmp = add_instr(emit, Free_Loop_Counter);
  tmp->m_val = (m_uint)counter;
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
      return -1;
    }
    for (i = 0; i < size; i++) {
      label = (Stmt_Goto_Label)vector_at(stmt->data.v, i);
      label->data.instr->m_val = vector_size(emit->code->code);
    }
    free(stmt->data.v);
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
  instr = add_instr(emit, Branch_Switch);
  instr->ptr = emit->cases = new_Map();
//  frame_push_scope(emit->code->frame);
  CHECK_BB(emit_Stmt(emit, stmt->stmt, 1))
//  emit_pop_scope(emit);
  instr->m_val = emit->default_case_index > -1 ? emit->default_case_index : vector_size(emit->code->code);
  emit->default_case_index = -1;
  while (vector_size(emit->code->stack_break) && vector_back(emit->code->stack_break)) {
    _break = (Instr)vector_back(emit->code->stack_break);
    _break->m_val = vector_size(emit->code->code);
    vector_pop(emit->code->stack_break);
  }
  vector_pop(emit->code->stack_break);
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
  if (!emit->cases) {
    err_msg(EMIT_, stmt->pos, "case found outside switch statement. this is not allowed for now");
    return -1;
  }
  if (stmt->val->exp_type == Primary_Expression_type) {
    if (stmt->val->d.exp_primary->type == ae_primary_num)
      value = stmt->val->d.exp_primary->d.num;
    else {
      if (!stmt->val->d.exp_primary->value->is_const) {
        err_msg(EMIT_, stmt->pos, "value is not const. this is not allowed for now");
        return -1;
      }
      value = stmt->val->d.exp_primary->value->is_const == 2 ? (m_uint)stmt->val->d.exp_primary->value->ptr : // for enum
              *(m_uint*)stmt->val->d.exp_primary->value->ptr;                                                   // for primary variable
    }
  } else if (stmt->val->exp_type == Dot_Member_type) {
    t = isa(stmt->val->d.exp_dot->t_base, &t_class) > 0 ? stmt->val->d.exp_dot->t_base->actual_type : stmt->val->d.exp_dot->t_base;
    v = find_value(t, stmt->val->d.exp_dot->xid);
    value = v->is_const == 2 ? t->info->class_data[v->offset] : *(m_uint*)v->ptr;
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

static m_bool emit_Func_Ptr(Emitter emit, Func_Ptr* ptr)
{
  namespace_add_func(emit->env->curr, ptr->xid, ptr->func);
  vector_append(emit->funcs, (vtype)ptr);
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

static m_bool emit_Stmt(Emitter emit, Stmt* stmt, m_bool pop)
{
#ifdef DEBUG_EMIT
  debug_msg("emit", "Stmt (pop: %s)", pop ? "yes" : "no");
#endif
  Instr instr;
  Decl_List l;
  Local* local;
  Var_Decl_List var_list;
  m_bool is_member;
  m_bool ret = 1;
  if (!stmt)
    return 1;
  switch (stmt->type) {
  case ae_stmt_exp:
    if (!stmt->d.stmt_exp)
      return 1;
    ret = emit_Expression(emit, stmt->d.stmt_exp, 0);
// check 'stmt->d.stmt_exp->type'for switch
    if (ret > 0 && pop && stmt->d.stmt_exp->type && stmt->d.stmt_exp->type->size > 0) {
      Expression exp = stmt->d.stmt_exp;
      if (exp->exp_type == Primary_Expression_type && exp->d.exp_primary->type == ae_primary_hack)
        exp = exp->d.exp_primary->d.exp;
      while (exp) {
        // get rid of primary str ?
        /*          if(exp->exp_type == Primary_Expression_type && exp->d.exp_primary->type == ae_primary_str)*/
        instr = add_instr(emit, Reg_Pop_Word4);
        /*        if (isa(exp->type, &t_complex) > 0 && exp->exp_type != Decl_Expression_type)
                  exp->type->size = SZ_COMPLEX * exp->d.exp_decl->num_decl;
                if (isa(exp->type, &t_polar) > 0 && exp->exp_type != Decl_Expression_type)
                  exp->type->size = SZ_COMPLEX * exp->d.exp_decl->num_decl; */
        //        instr->m_val = exp->exp_type == Decl_Expression_type ? exp->d.exp_decl->num_decl * exp->type->size : exp->type->size;
        instr->m_val = (exp->exp_type == Decl_Expression_type ? exp->d.exp_decl->num_decl * SZ_INT : exp->type->size);
        exp = exp->next;
      }
    }
    if (ret < 0)
      return -1;
    break;
  case ae_stmt_code:
    ret = emit_Stmt_Code(emit, stmt->d.stmt_code, 1);
    break;
  case ae_stmt_if:
    ret = emit_If(emit, stmt->d.stmt_if);
    break;
  case ae_stmt_return:
    ret = emit_Return(emit, stmt->d.stmt_return);
    break;

  case ae_stmt_break:
    ret = emit_Break(emit, stmt->d.stmt_break);
    break;

  case ae_stmt_continue:
    ret = emit_Continue(emit, stmt->d.stmt_continue);
    break;

  case ae_stmt_while:
    if (stmt->d.stmt_while->is_do)
      ret = emit_Do_While(emit, stmt->d.stmt_while);
    else
      ret = emit_While(emit, stmt->d.stmt_while);
    break;
  case ae_stmt_until:
    if (stmt->d.stmt_until->is_do)
      ret = emit_Do_Until(emit, stmt->d.stmt_until);
    else
      ret = emit_Until(emit, stmt->d.stmt_until);
    break;
  case ae_stmt_for:
    ret = emit_For(emit, stmt->d.stmt_for);
    break;
  case ae_stmt_loop:
    ret = emit_Loop(emit, stmt->d.stmt_loop);
    break;
  case ae_stmt_gotolabel:
    ret = emit_Goto_Label(emit, stmt->d.stmt_gotolabel);
    break;
  case ae_stmt_case:
    ret = emit_Case(emit, stmt->d.stmt_case);
    break;
  case ae_stmt_enum:
    ret = emit_Enum(emit, stmt->d.stmt_enum);
    break;
  case ae_stmt_switch:
    ret = emit_Switch(emit, stmt->d.stmt_switch);
    break;
  case ae_stmt_funcptr:
    ret = emit_Func_Ptr(emit, stmt->d.stmt_funcptr);
    break;
  case ae_stmt_union:
    l = stmt->d.stmt_union->l;
    is_member = l->self->list->self->value->is_member;
    if (!is_member) {
      local = frame_alloc_local(emit->code->frame, stmt->d.stmt_union->s, "union", 1, 0);
      stmt->d.stmt_union->o = local->offset;
    } else
      local = NULL;
    while (l) {
      var_list = l->self->list;
      while (var_list) {
        var_list->self->value->offset = stmt->d.stmt_union->o;
        var_list = var_list->next;
      }
      l = l->next;
    }
    /*      switch(stmt->stmt_union->s)*/
    /*      {*/
    /*        case SZ_COMPLEX:*/
    /*          add_instr(emit, is_member ? Alloc_Word_Complex : Alloc_Word_Complex);*/
    /*          break;*/
    /*        case SZ_INT:*/
    /*          add_instr(emit, is_member ?  Alloc_Member_Word : Alloc_Word);*/
    /*          break;*/
    /*    case SZ_FLOAT:*/
    /*      add_instr(emit, is_member ? Alloc_Member_Word_Float : Alloc_Word_Float);*/
    /*      break;*/
    /*  }*/
    /*    }*/
    /*    Instr instr = add_instr(emit, Reg_Pop_Word4);*/
    /*    instr->m_val = stmt->stmt_union->s;*/

    ret = 1;
    break;
  default:
    err_msg(EMIT_, stmt->pos, "unhandled statement type. sorry");
    ret = -1;
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
      Instr dup = add_instr(emit, Reg_Dup_Last_Vec3);
      (void)dup; // prevent cppcheck warning
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
      Instr dup = add_instr(emit, Reg_Dup_Last_Vec4);
      (void)dup; // prevent cppcheck warning
      Instr f = add_instr(emit, member_function);
      f->ptr = t_base->info->obj_v_table;
      f->m_val = value->func_ref->vt_index;
      return 1;
    }
    //		else return -1;
    instr->m_val = emit_addr;
    //    instr->m_val = emit_addr;
    return 1;
  }
  if (t_base->xid == t_vararg.xid) {
    m_uint offset = 0;
    Arg_List l = emit->env->func->def->arg_list;
    while (l) {
      offset += l->type->size;
      l = l->next;
    }
    if (!strcmp(S_name(member->xid), "start")) {
      if (emit->env->func->variadic_start) {
        err_msg(EMIT_, 0, "vararg.start already used. this is an error");
        return -1;
      }
      emit->env->func->variadic_start = add_instr(emit, Vararg_start);
      emit->env->func->variadic_start->m_val = offset;
      emit->env->func->variadic_index = vector_size(emit->code->code);
      return 1;
    }
    if (!strcmp(S_name(member->xid), "end")) {
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
      if (value->is_member) { // member
        if (emit_Expression(emit, member->base, 0) < 0) {
          err_msg(EMIT_, member->pos, "... in member function");
          return -1;
        }
        push_i = add_instr(emit, Reg_Dup_Last);
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
          err_msg(EMIT_, member->pos, "... in member function");
          return -1;
        }
        push_i = add_instr(emit, Reg_Dup_Last);
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
      if (value->is_member) { // member
        CHECK_BB(emit_Expression(emit, member->base, 0))
        instr = add_instr(emit, Dot_Member_Data);
        instr->m_val = offset;
        instr->m_val2 = kindof(value->m_type);
        instr->ptr = (m_uint*)emit_addr;
      } else { // static
        if (value->ptr && value->is_import) { // from C
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
      if (value->ptr && value->is_import) {
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
    err_msg(EMIT_, func_def->pos,
            "function '%s' already emitted...", S_name(func_def->name));
    return -1;
  }

  if (emit->env->func) {
    err_msg(EMIT_, func_def->pos,
            "internal error: nested function definition...", func_def->pos);
    return -1;
  }

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
  sprintf(c, "%s%s%s( ... )", emit->env->class_def ? emit->env->class_def->name : "", emit->env->class_def ? "." : " ", func->name);
  emit->code->name = strdup(c);
  emit->code->need_this = func->is_member;
  emit->code->filename = strdup(emit_filename);

  Arg_List a = func_def->arg_list;
  m_bool is_obj = 0;
  m_bool is_ref = 0;

  if(func->is_member) {
    emit->code->stack_depth += SZ_INT;
    if (!frame_alloc_local(emit->code->frame, SZ_INT, "this", 1, 0)) {
      err_msg(EMIT_, a->pos, "(emit): internal error: cannot allocate local 'this'...");
      return -1;
    }
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
      err_msg(EMIT_, a->pos, "(emit): internal error: cannot allocate local '%s'...", value->name);
      return -1;
    }
    value->offset = local->offset;
    a = a->next;
  }
  if (func_def->is_variadic) {
    if (!frame_alloc_local(emit->code->frame, type->size, "vararg", is_ref, is_obj)) {
      err_msg(EMIT_, func_def->pos, "(emit): internal error: cannot allocate local 'vararg'...");
      return -1;
    }
    emit->code->stack_depth += SZ_INT;
  }

  // TODO: make sure the calculated stack depth is the same as func_def->stack depth
  // taking into account member function

  // add references for objects in the arguments (added 1.3.0.0)
  // NOTE: this isn't in use since currently the caller is reference counting
  // the arguments -- this is to better support sporking, which is more
  // asynchronous.  the code below is left in as reference for callee ref counting
  // emit->addref_on_scope();

  // emit the code
  CHECK_BB(emit_Stmt(emit, func_def->code, 0))

  // ensure return
  if (func_def->ret_type && func_def->ret_type->xid != t_void.xid) {
    add_instr(emit, Reg_Push_Imm);
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
  Instr instr = add_instr(emit, Func_Return);
  func->code = emit_to_code(emit);
  (void)instr; // prevent cppcheck warning
  if (func->def->spec == ae_func_spec_dtor) {
    /*    func->code->stack_depth = SZ_INT;*/
    emit->env->class_def->info->dtor = func->code;
    emit->env->class_def->has_destructor = 1;
  } else if (func->def->spec == ae_func_spec_op)
    operator_set_func(emit->env, func, func->def->arg_list->type, func->def->arg_list->next->type);

  // add reference
  add_ref(func->obj);
  emit->env->func = NULL;

  // delete the code ?
  /*  rem_ref(emit->code->obj, emit->code);*/
  //  free_Code(emit->code);
  emit->code = (Code*)vector_back(emit->stack);
  vector_pop(emit->stack);
  return 1;
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
    //  if (type->info->pre_ctor != NULL && type->info->pre_ctor->instr != NULL && vector_size(type->info->pre_ctor->instr)) {
    err_msg(EMIT_, class_def->pos,
            "(emit): class '%s' already emitted...", type->name);
    return -1;
  }
  type->info->class_data = calloc(type->info->class_data_size, sizeof(char));
  if (!type->info->class_data) {
    err_msg(EMIT_, class_def->pos, "OutOfMemory: while allocating static data '%s'\n", type->name);
    ret = -1;
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
    err_msg(EMIT_, class_def->pos,
            "(emit): internal error: cannot allocate local 'this'...");
    return -1;
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
    Instr instr = add_instr(emit, Func_Return);
    (void)instr; // prevent cppcheck warning
    free_VM_Code(type->info->pre_ctor);
    type->info->pre_ctor = emit_to_code(emit);
    /*    type->info->pre_ctor->add_ref();*/
  } else {
    /*    SAFE_DELETE(type->info->pre_ctor);*/
    free(type->info->class_data);
  }
  emit->env->class_def = (Type)vector_back(emit->env->class_stack);
  vector_pop(emit->env->class_stack);
  // delete the code
  //    SAFE_DELETE( emit->code );
  emit->code = (Code*)vector_back(emit->stack);
  vector_pop(emit->stack);
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
  // handle func pointer
  for(i = 0; i < vector_size(emit->funcs); i++) {
    Func_Ptr* ptr = (Func_Ptr*)vector_at(emit->funcs, i);
    scope_rem(emit->env->curr->func, ptr->xid);
  }
  // handle empty array type
  for(i = 0; i < vector_size(emit->array); i++) {
    Type t = (Type)vector_at(emit->array, i);
    free(t->obj);
    free(t); // all this should be:na
    // rem_ref(t->obj, t);
  }
  vector_clear(emit->array);
  if (ret < 0) {
    free_Ast(ast);
  }
  emit_pop_scope(emit);
  return ret;
}
