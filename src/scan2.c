/** @file src/scan2.c
 * @brief second level scan
 */
#include <string.h> // use in Func_Def
#include "err_msg.h"
#include "absyn.h"
#include "func.h"

static m_bool scan2_Expression(Env env, Expression exp);
static m_bool scan2_Stmt_List(Env env, Stmt_List list);
static m_bool scan2_Stmt(Env env, Stmt* stmt);

m_bool scan2_Decl_Expression(Env env, Decl_Expression* decl)
{
  Var_Decl_List list = decl->list;
  Type type = decl->m_type;

  // shred are to be ref
  if(isa(type, &t_shred) > 0)
    decl->type->ref = 1;
  if(!type->size)
  {
    err_msg(SCAN2_, decl->pos,
      "cannot declare variables of size '0' (i.e. 'void')..." );
    return -1;
  }
  if(!decl->type->ref)
  {
    if(env->class_def && (type == env->class_def) && !env->class_scope)
    {
      err_msg(SCAN2_, decl->pos,
        "...(note: object of type '%s' declared inside itself)", type->name);
      return -1;
    }
  }

/*  if((isprim(type) > 0 || isa(type, &t_string ) > 0) && decl->type->ref)  // TODO: string*/
  if((isprim(type) > 0) && decl->type->ref)  // TODO: string
  {
    err_msg(SCAN2_, decl->pos,
        "cannot declare references (@) of primitive type '%s'...", type->name);
    err_msg(SCAN2_, decl->pos,
        "...(primitive types: 'int', 'float', 'time', 'dur')" );
    return -1;
  }

  while(list)
  {
    if(isres(env, list->self->xid, list->self->pos) > 0)
    {
      err_msg(SCAN2_, list->self->pos, "...in variable declaration", S_name(list->self->xid) );
      return -1;
    }
    if(namespace_lookup_value(env->curr, list->self->xid, 0))
    {
      err_msg(SCAN2_, list->self->pos, "variable %s has already been defined in the same scope...", S_name(list->self->xid));
      return -1;
    }
    if( list->self->array != NULL )
    {
      if(verify_array( list->self->array ) < 0)
        return -1;
      Type t2 = type;

      if( list->self->array->exp_list )
      {
        if(scan2_Expression(env, list->self->array->exp_list) < 0)
          return -1;
      }
      type = new_array_type(env, &t_array, list->self->array->depth, t2, env->curr);
      if( !list->self->array->exp_list )
        decl->type->ref = 1;
/*      SAFE_REF_ASSIGN( decl->ck_type, type );*/
      decl->m_type = type;
    }
    list->self->value = new_Value(env->context, type, S_name(list->self->xid));
    list->self->value->checked = 0;
    list->self->value->owner = env->curr;
    list->self->value->owner_class = env->func ? NULL : env->class_def;
    list->self->value->is_member = (env->class_def && !env->class_scope && !env->func && !decl->is_static);
    list->self->value->is_context_global = !env->class_def && !env->func;
    list->self->value->ptr = list->self->addr;
    namespace_add_value(env->curr, list->self->xid, list->self->value);


    // doc
    if(!env->class_def && ! env->func)
      context_add_value(env->context, list->self->value, list->self->value->obj);
    if(list->doc)
      list->self->value->doc = list->doc;

    list = list->next;
  }
  return 1;
}

static m_bool scan2_Func_Ptr(Env env, Func_Ptr* ptr)
{
  m_uint count = 1;
  Arg_List arg_list = ptr->args;
  Value v;
  if(namespace_lookup_value(env->curr, ptr->xid, 0))
  {
    err_msg(SCAN2_, ptr->pos, "variable %s has already been defined in the same scope...", S_name(ptr->xid));
    return -1;
  }
  // shouldn't it be later ?
/*  ptr->value = new_Value(env->context, &t_function, S_name(ptr->xid));*/
  ptr->value = new_Value(env->context, &t_func_ptr, S_name(ptr->xid));
  namespace_push_value(env->curr);
  while(arg_list)
  {
    if( arg_list->type->size == 0 )
    {
      err_msg(SCAN2_, arg_list->pos, "cannot declare variables of size '0' (i.e. 'void')...");
/*        goto error;*/
      return -1;
    }
    /*
    // check if reserved
    if( type_engine_check_reserved( env, arg_list->var_decl->xid, arg_list->linepos ) )
    {
        EM_error2( arg_list->linepos, "in function '%s'", S_name(f->name) );
        goto error;
    }
*/

    // primitive
    if( (isprim( arg_list->type ) > 0)
      && arg_list->type_decl->ref )  // TODO: string
    {
      err_msg(SCAN2_, arg_list->type_decl->pos,
          "cannot declare references (@) of primitive type '%s'...",
          arg_list->type->name);
      err_msg(SCAN2_, arg_list->type_decl->pos,
          "...(primitive types: 'int', 'float', 'time', 'dur')" );
      return -1;
/*      goto error;*/
    }

    if(arg_list->var_decl->array)
    {
      // verify there are no errors from the parser...
      if(verify_array(arg_list->var_decl->array) < 0)
          return -1;

      Type t = arg_list->type;
      Type t2 = t;
      // should be partial and empty []
      if(arg_list->var_decl->array->exp_list)
      {
        err_msg(SCAN2_, arg_list->pos, "in function '%s':", S_name(ptr->xid) );
        err_msg(SCAN2_, arg_list->pos, "argument %i '%s' must be defined with empty []'s",
            count, S_name(arg_list->var_decl->xid) );
        return -1;
/*        goto error;*/
      }
      // create the new array type
      t = new_array_type(env, &t_array, arg_list->var_decl->array->depth, t2, env->curr);
      // set ref
      arg_list->type_decl->ref = 1;
      arg_list->type = t;
/*      SAFE_REF_ASSIGN( arg_list->type, t );*/
    }
  // check if array

  // make new value
  v = calloc(1, sizeof(struct Value_));
  v->m_type = arg_list->type;
/*  v->doc = arg_list->doc;*/
/*  if(arg_list->doc)*/
/*    exit(2);*/
  v->name = S_name(arg_list->var_decl->xid);
  // remember the owner
  v->owner = env->curr;
  // function args not owned
/*  v->owner_class = NULL;*/
/*  v->is_member = 0;*/

    namespace_add_value(env->curr, arg_list->var_decl->xid, v);


  // stack
/*  v->offset = f->stack_depth;*/
/*  f->stack_depth += arg_list->type->size;*/
/*  f->stack_depth++;*/

    // remember
    arg_list->var_decl->value = v;

    // count
    count++;
    // next arg
    arg_list = arg_list->next;

  }
  namespace_pop_value(env->curr);

  ptr->value->checked = 1;
/*  if(env->class_def)*/
/*    namespace_add_value(env->class_def, ptr->xid, ptr->value);*/
/*  else*/
  namespace_add_value(env->curr, ptr->xid, ptr->value);

/*  Func_Def def = new_Func_Def(!env->class_def ? ae_key_func : ae_key_instance, ptr->key, ptr->type, S_name(ptr->xid), ptr->args, NULL, ptr->pos);*/
/*  Func_Def def = new_Func_Def(ae_key_func, ptr->key, ptr->type, S_name(ptr->xid), ptr->args, NULL, ptr->pos);*/
  Func_Def def = new_Func_Def(ae_key_func, !env->class_def ? ae_key_func : !ptr->key ? ae_key_instance : ae_key_static, ptr->type, S_name(ptr->xid), ptr->args, NULL, ptr->pos);
  def->ret_type = ptr->ret_type;
  ptr->func = new_Func(S_name(ptr->xid), def);
  ptr->value->func_ref = ptr->func;
  ptr->func->value_ref = ptr->value;
  if(env->class_def)
  {
    ptr->value->is_member   = !ptr->key;
    ptr->value->owner_class = env->class_def;
    ptr->func->is_member   = !ptr->key;
  }
/*  if(env->class_def)*/
/*    ptr->func->is_member   = 1;*/
  namespace_add_func(env->curr, ptr->xid, ptr->func);
  return 1;
}

static m_bool scan2_Primary_Expression(Env env, Primary_Expression* primary)
{
  if(primary->type == ae_primary_hack)
    CHECK_BB(scan2_Expression(env, primary->exp))
  return 1;
}

static m_bool scan2_Array(Env env, Array* array)
{
  CHECK_BB(verify_array(array->indices))
  CHECK_BB(scan2_Expression(env, array->base))
  CHECK_BB(scan2_Expression(env, array->indices->exp_list))
  return 1;
}

static m_bool scan2_Binary_Expression(Env env, Binary_Expression* binary )
{
  CHECK_BB(scan2_Expression(env, binary->lhs))
  CHECK_BB(scan2_Expression(env, binary->rhs))
  return 1;
}

static m_bool scan2_Cast_Expression(Env env, Cast_Expression* cast)
{
  CHECK_BB(scan2_Expression(env, cast->exp))
  return 1;
}

static m_bool scan2_Postfix_Expression(Env env, Postfix_Expression* postfix )
{
  // check the exp
  CHECK_BB(scan2_Expression( env, postfix->exp ))
  switch(postfix->op)
  {
    case op_plusplus:
    case op_minusminus:
      // assignable?
      if(postfix->exp->meta != ae_meta_var)
      {
        err_msg(SCAN2_, postfix->exp->pos,
          "postfix operator '%s' cannot be used on non-mutable data-type...",op2str( postfix->op ));
        return -1;
      }
      // TODO: mark somewhere we need to post increment
      return 1;
    break;

    default:
      err_msg( SCAN2_, postfix->pos,
        "internal compiler error (pre-scan): unrecognized postfix '%i'", postfix->op );
      return -1;
  }
  return 1;
}

static m_bool scan2_Dur(Env env, Exp_Dur* dur)
{
  CHECK_BB(scan2_Expression(env, dur->base))
  CHECK_BB(scan2_Expression(env, dur->unit))
  return 1;
}

static m_bool scan2_Func_Call1(Env env, Expression func, Expression args, Func m_func)
{
  CHECK_BB(scan2_Expression(env, func))
  CHECK_BB(scan2_Expression(env, args))
  return 1;
}
 m_bool scan2_Func_Def(Env env, Func_Def f);
static m_bool scan2_Func_Call(Env env, Func_Call* func_call)
{
  if(func_call->types)
  {
    if(func_call->func->exp_type == Primary_Expression_type)
    {
      Value v = namespace_lookup_value(env->curr, func_call->func->primary_exp->var, 1);
      if(!v)
      {
        err_msg(SCAN2_, func_call->pos, "template call of non-existant function.");
        return -1;
      }
      Func_Def base = v->func_ref->def;
      if(!base->types)
      {
        err_msg(SCAN2_, func_call->pos, "template call of non-template function.");
        return -1;
      }
      return 1;
    }
    else if(func_call->func->exp_type == Dot_Member_type)
      return 1;
    else
      {
        err_msg(SCAN2_, func_call->pos, "unhandled exprssion type '%i' in template func call.", func_call->func->exp_type);
        return -1;
      }
  }
  return scan2_Func_Call1(env, func_call->func, func_call->args, func_call->m_func);
}

static m_bool scan2_Dot_Member(Env env, Dot_Member* member)
{
  CHECK_BB(scan2_Expression(env, member->base))
  return 1;
}
static m_bool scan2_exp_if(Env env, If_Expression* exp_if)
{
  CHECK_BB(scan2_Expression(env, exp_if->cond))
  CHECK_BB(scan2_Expression(env, exp_if->if_exp))
  CHECK_BB(scan2_Expression(env, exp_if->else_exp))
  return 1;
}
static m_bool scan2_Expression(Env env, Expression exp)
{
	Expression curr = exp;
	m_bool ret = 1;
	while(curr && ret > 0)
	{
    switch(exp->exp_type)
		{
			case Primary_Expression_type:
      ret = scan2_Primary_Expression(env, exp->primary_exp);
        break;
			case Decl_Expression_type:
				ret = scan2_Decl_Expression(env, exp->decl_exp);
        break;
			case Unary_Expression_type:
        if(exp->unary->code)
        {
          if(env->func)
            ret = scan2_Stmt(env, exp->unary->code);
          else
          {
            env->func = (Func)1; // check me
            ret = scan2_Stmt(env, exp->unary->code);
            env->func = NULL;
          }
        }
        else
          ret = 1;
        break;
			case Binary_Expression_type:
				ret = scan2_Binary_Expression(env, exp->binary_exp);
        break;
			case Postfix_Expression_type:
				ret = scan2_Postfix_Expression(env, exp->postfix_exp);
        break;
			case Cast_Expression_type:
				ret = scan2_Cast_Expression(env, exp->cast_exp);
        break;
			case Func_Call_type:
				ret = scan2_Func_Call(env, exp->func_call);
        break;
			case Array_Expression_type:
				ret = scan2_Array(env, exp->array);
        break;
			case Dot_Member_type:
				ret = scan2_Dot_Member(env, exp->dot_member);
        break;
			case If_Expression_type:
				ret = scan2_exp_if(env, exp->exp_if);
        break;
      case Dur_Expression_type:
				ret = scan2_Dur(env, exp->dur);
        break;
			default:
        err_msg(SCAN2_, exp->pos, "unhandled expression type '%i'", curr->exp_type);
				ret = -1;
				break;
		}
    curr = curr->next;
	}
	return ret;
}

static m_bool scan2_Stmt_Code(Env env, Stmt_Code stmt, m_bool push )
{
    env->class_scope++;
    if(push)
      namespace_push_value(env->curr);
    m_bool t = scan2_Stmt_List( env, stmt->stmt_list);
    if(push)
      namespace_pop_value(env->curr);
    env->class_scope--;
    return t;
}

static m_bool scan2_While(Env env, Stmt_While stmt)
{
  CHECK_BB(scan2_Expression(env, stmt->cond))
  CHECK_BB(scan2_Stmt(env, stmt->body))
  return 1;
}
static m_bool scan2_If(Env env, Stmt_If stmt)

{
  CHECK_BB(scan2_Expression(env, stmt->cond))
  CHECK_BB(scan2_Stmt(env, stmt->if_body))
  if(stmt->else_body)
    CHECK_BB(scan2_Stmt(env, stmt->else_body))
  return 1;
}
static m_bool scan2_Until(Env env, Stmt_Until stmt)
{
  CHECK_BB(scan2_Expression(env, stmt->cond))
  CHECK_BB(scan2_Stmt(env, stmt->body))
  return 1;
}

static m_bool scan2_For(Env env, Stmt_For stmt)
{
  CHECK_BB(scan2_Stmt(env, stmt->c1))
  CHECK_BB(scan2_Stmt(env, stmt->c2))
  CHECK_BB(scan2_Expression(env, stmt->c3))
  CHECK_BB(scan2_Stmt(env, stmt->body))
  return 1;
}

static m_bool scan2_Loop(Env env, Stmt_Loop stmt)
{
  CHECK_BB(scan2_Expression(env, stmt->cond))
  CHECK_BB(scan2_Stmt(env, stmt->body))
  return 1;
}

static m_bool scan2_return(Env env, Stmt_Return stmt )
{
  m_bool ret = -1;
  if(stmt->val)
    ret = scan2_Expression( env, stmt->val );
  else
    ret = 1;
  return ret;
}
static m_bool scan2_Switch(Env env, Stmt_Switch stmt)
{
  CHECK_BB(scan2_Expression(env, stmt->val))
  return 1;
}

static m_bool scan2_Case(Env env, Stmt_Case stmt)
{
  CHECK_BB(scan2_Expression(env, stmt->val))
  return 1;
}

static m_bool scan2_Goto_Label(Env env, Stmt_Goto_Label stmt)
{
#ifdef DEBUG_SCAN2
  debug_msg("scan1", "%s '%'", stmt->is_label ? "label": "goto", S_name(stmt->name));
#endif
  Map m;
  m_uint* key = env->class_def && !env->func ? (m_uint*)env->class_def : (m_uint*)env->func;
  if(stmt->is_label)
  {
    m = map_get(env->curr->label, key);
    if(!m) // create map for func
    {
      m = new_Map();
      map_set(env->curr->label, key, m);
    }
    if(map_get(m, stmt->name))
    {
      err_msg(SCAN2_, stmt->pos, "label '%s' already defined", S_name(stmt->name));
      return -1;
    }
    map_set(m, stmt->name, stmt);
    stmt->data.v = new_Vector();
  }
  return 1;
}

static m_bool scan2_Stmt(Env env, Stmt* stmt)
{
  m_bool ret = -1;
  Decl_List l;
  if(!stmt)
    return 1;
  switch(stmt->type)
  {
    case ae_stmt_exp:
      ret = scan2_Expression(env, stmt->stmt_exp);
      break;

    case ae_stmt_return:
        ret = scan2_return( env, stmt->stmt_return );
        break;

    case ae_stmt_code:
      env->class_scope++;
      ret = scan2_Stmt_Code( env, stmt->stmt_code, 1);
      env->class_scope--;
      break;

    case ae_stmt_if:
      env->class_scope++;
      namespace_push_value(env->curr);
      ret = scan2_If(env, stmt->stmt_if);
      namespace_pop_value(env->curr);
      env->class_scope--;
      break;

    case ae_stmt_while:
      env->class_scope++;
      namespace_push_value(env->curr);
      ret = scan2_While(env, stmt->stmt_while);
      namespace_pop_value(env->curr);
      env->class_scope--;
      break;

    case ae_stmt_until:
      env->class_scope++;
      namespace_push_value(env->curr);
      ret = scan2_Until(env, stmt->stmt_until);
      namespace_pop_value(env->curr);
      env->class_scope--;
      break;

    case ae_stmt_for:
      env->class_scope++;
      namespace_push_value(env->curr);
      ret = scan2_For(env, stmt->stmt_for);
      namespace_pop_value(env->curr);
      env->class_scope--;
     break;

    case ae_stmt_loop:
      env->class_scope++;
      namespace_push_value(env->curr);
      ret = scan2_Loop(env, stmt->stmt_loop);
      namespace_pop_value(env->curr);
      env->class_scope--;
      break;
    case ae_stmt_switch:
      env->class_scope++;
      namespace_push_value(env->curr);
      ret = scan2_Switch( env, stmt->stmt_switch);
      namespace_pop_value(env->curr);
      env->class_scope--;
      break;
    case ae_stmt_case:
      ret = scan2_Case(env, stmt->stmt_case);
      break;
    case ae_stmt_gotolabel:
      ret = scan2_Goto_Label(env, stmt->stmt_gotolabel);
      break;
    case ae_stmt_continue:
    case ae_stmt_break:
    case ae_stmt_enum:
      ret = 1;
      break;
    case ae_stmt_funcptr:
      ret = scan2_Func_Ptr(env, stmt->stmt_funcptr);
      break;
    case ae_stmt_union:
      l = stmt->stmt_union->l;
      while(l)
      {
        CHECK_BB(scan2_Decl_Expression(env, l->self))
        l= l->next;
      }
      ret = 1;
      break;

   default:
    err_msg(SCAN2_, stmt->pos, "unhandled statement type '%i', sorry.", stmt->type);
    ret = -1;
  }
  return ret;
}

static m_bool scan2_Stmt_List(Env env, Stmt_List list)
{
  Stmt_List curr = list;
  while(curr)
  {
    CHECK_BB(scan2_Stmt( env, curr->stmt ))
    curr = curr->next;
  }
  return 1;
}

m_bool scan2_Func_Def(Env env, Func_Def f)
{
  Type type = NULL;
  Value value = NULL;
  Func func = NULL;
  Value overload = NULL;
  Value v;
  Arg_List arg_list = NULL;
  m_str func_name = S_name(f->name);
  m_str orig_name = func_name;
  m_uint count = 0;
  if(env->func)
  {
    err_msg(SCAN2_, f->pos, "nested function definitions are not (yet) allowed");
    goto error;
  }


  if(f->types)
  {
    func = new_Func(func_name, f);
    overload = namespace_lookup_value(env->curr,  f->name, 0);
    if(overload)
      func->next = overload->func_ref->next;
    func->is_member = f->static_decl != ae_key_static && env->class_def;
    value = new_Value(env->context, &t_function, func_name);
    value->is_const = 1;
    value->owner = env->curr;
    value->owner_class = env->class_def;
    value->is_member = func->is_member;
    value->is_context_global = (env->class_def == NULL);
    value->func_ref = func;
    add_ref(value->func_ref->obj);
    func->value_ref = value;
    add_ref(value->obj);
    f->func = func;
    add_ref(f->func->obj);
    value->checked = 1;
    if(overload)
      overload->func_num_overloads++;
    else
      namespace_add_value(env->curr, insert_symbol(orig_name), value);
    char name[256];
    sprintf(name, "%s<template>@%li@%s", S_name(f->name),
			overload ? overload->func_num_overloads : 0, env->curr->name);
    namespace_add_value(env->curr, insert_symbol(name), value);
    return 1;
  }

  // overload
  overload = namespace_lookup_value(env->curr,  f->name, 0);
  if(overload)
  {
    if(isa(overload->m_type, &t_function) < 0)
    {
      err_msg(SCAN2_, f->pos, "function name '%s' is already used by another value", S_name(f->name));
      return -1;
    }
    else
    {
      if(!overload->func_ref)
      {
        err_msg(SCAN2_, f->pos, "internal error: missing function '%s'", overload->name );
        return -1;
      }
    }
  }
  char tmp[256];
  memset(tmp, 0, 256);
  m_uint len = strlen(func_name) + strlen("0") + strlen(env->curr->name) + 3;

  if(overload)
  {
    len = strlen(func_name) + ((overload->func_num_overloads + 1)% 10) + strlen(env->curr->name) + 3;
    snprintf(tmp, len + 1, "%s@%li@%s", func_name, ++overload->func_num_overloads, env->curr->name);
  }
  else
    snprintf(tmp, len + 1, "%s@0@%s", func_name, env->curr->name);

  func_name = strdup(tmp);
  func = new_Func(func_name, f);
  func->is_member = f->static_decl != ae_key_static && env->class_def;

  if(f->s_type == ae_func_builtin) // actual builtin func import
  {
    func->code = new_VM_Code(new_Vector(), 0, func->def->stack_depth, S_name(f->name), "builtin func code");
    func->code->need_this = func->is_member;
    func->code->native_func = (m_uint)func->def->dl_func_ptr;
  }

  type = calloc(1, sizeof(struct Type_));
  type->xid = te_function;
  type->name = strdup(tmp);
  type->parent = &t_function;
  type->size = sizeof(void *);
  type->func = func;

  value = new_Value(env->context, type, func_name);
  value->is_const = 1;
  value->owner = env->curr;
  value->owner_class = env->class_def;
  value->is_member = func->is_member;
  value->is_context_global = (env->class_def == NULL);
  value->func_ref = func;
  add_ref(value->func_ref->obj);
  func->value_ref = value;
  add_ref(value->obj);
  f->func = func;
  add_ref(f->func->obj);

  if(overload)
  {
    func->next = overload->func_ref->next;
    overload->func_ref->next = func;
  }

  if(!f->ret_type) // template return value
    f->ret_type = find_type(env, f->type_decl->xid);

  if(isprim(f->ret_type) > 0 && f->type_decl->ref)  // TODO: string
  {
    err_msg(SCAN2_,  f->type_decl->pos,
      "FUNC cannot declare references (@) of primitive type '%s'...\n", f->ret_type->name );
    err_msg(SCAN2_, f->type_decl->pos,
      "...(primitive types: 'int', 'float', 'time', 'dur')" );
    goto error;
  }

  arg_list = f->arg_list;
  count = 1;
  f->stack_depth = func->is_member ? sizeof(void *) : 0;

  namespace_push_value(env->curr);

  while(arg_list)
  {
    if(!arg_list->type) // template
      arg_list->type = find_type(env, arg_list->type_decl->xid);
    if(!arg_list->type->size)
    {
      err_msg(SCAN2_, arg_list->pos, "cannot declare variables of size '0' (i.e. 'void')...");
        goto error;
    }

/*
    // check if reserved
    if(is_res(env, arg_list->var_decl->xid, arg_list->linepos) > 0)
    {
      err_msg(SCAN2_,  arg_list->pos, "in function '%s'", S_name(f->name) );
      goto error;
    }
*/

    // primitive
    if( (isprim( arg_list->type ) > 0)
      && arg_list->type_decl->ref )  // TODO: string
    {
      err_msg(SCAN2_, arg_list->type_decl->pos,
          "cannot declare references (@) of primitive type '%s'...",
          arg_list->type->name);
      err_msg(SCAN2_, arg_list->type_decl->pos,
          "...(primitive types: 'int', 'float', 'time', 'dur')" );
      goto error;
    }

    if(arg_list->var_decl->array)
    {
      CHECK_BB(verify_array(arg_list->var_decl->array))

      Type t = arg_list->type;
      Type t2 = t;
      if(arg_list->var_decl->array->exp_list) // should be partial and empty []
      {
        err_msg(SCAN2_, arg_list->pos, "in function '%s':", S_name(f->name) );
        err_msg(SCAN2_, arg_list->pos, "argument %i '%s' must be defined with empty []'s",
            count, S_name(arg_list->var_decl->xid) );
        goto error;
      }

      // create the new array type
      t = new_array_type(env, &t_array, arg_list->var_decl->array->depth, t2, env->curr);

      // set ref
      arg_list->type_decl->ref = 1;
      arg_list->type = t;
/*      SAFE_REF_ASSIGN( arg_list->type, t );*/
    }

  // make new value
  v = calloc(1, sizeof(struct Value_));
  v->m_type = arg_list->type;
/*  v->doc = arg_list->doc;*/
  v->name = S_name(arg_list->var_decl->xid);
  // remember the owner
  v->owner = env->curr;
  // function args not owned
/*  v->owner_class = NULL;*/
/*  v->is_member = 0;*/

  namespace_add_value(env->curr, arg_list->var_decl->xid, v);
  // stack
  v->offset = f->stack_depth;
  f->stack_depth += arg_list->type->size;

  // remember
  arg_list->var_decl->value = v;

  // count
  count++;
  // next arg
  arg_list = arg_list->next;
  }

  if(f->is_variadic)
    f->stack_depth += SZ_INT;
  namespace_pop_value(env->curr);

  if(f->spec == ae_func_spec_op)
  {
    m_str str = S_name(f->name);
    str = strsep(&str, "@");
    add_binary_op(env, name2op(str), f->arg_list->var_decl->value->m_type, f->arg_list->next->var_decl->value->m_type, f->ret_type, NULL, 0);
    // free op
    return 1;
  }

  value->checked = 1;
  // while making doc
  if(!env->class_def)
    context_add_func(env->context, func, func->obj);

  if(!overload)
  {
    namespace_add_value(env->curr, insert_symbol(orig_name), value);
    namespace_add_value(env->curr, insert_symbol(value->name), value);
//    namespace_add_func( env->curr, insert_symbol(func->name), func ); // template. is it necessary ?
  }
  else
  {
    namespace_add_value(env->curr, insert_symbol(func->name), value);
//    namespace_add_func( env->curr, insert_symbol(func->name), func );
    // make sure returns are equal, if not templating
    if(overload->func_ref->def->ret_type) // template func don't check ret_type case
    if(!f->is_template) // template func don't check ret_type case
    if(f->ret_type->xid != overload->func_ref->def->ret_type->xid)
    {
      err_msg(SCAN2_,  f->pos, "function signatures differ in return type... '%s' and '%s'",
        f->ret_type->name, overload->func_ref->def->ret_type->name );
      if(env->class_def)
        err_msg(SCAN2_, f->pos,
          "function '%s.%s' matches '%s.%s' but cannot overload...",
          env->class_def->name, S_name(f->name),
         	value->owner_class->name, S_name(f->name) );
			goto error;
    }
  }
  namespace_add_func( env->curr, insert_symbol(func->name), func ); // template. is it necessary ?

  env->func = func;
  namespace_push_value(env->curr);

  if(f->code && scan2_Stmt_Code(env, f->code->stmt_code, 0) < 0)
  {
    err_msg(SCAN2_, f->pos, "...in function '%s'", S_name(f->name) );
    goto error;
  }

  namespace_pop_value(env->curr);
  env->func = NULL;

  if(f->type_decl->doc)
    func->doc = f->type_decl->doc;

  return 1;
error:
  if(func)
  {
    env->func = NULL;
    rem_ref(func->obj, func);
  }
  return -1;
}

static m_bool scan2_Class_Def(Env env, Class_Def class_def)
{
  m_bool ret = 1;
  Class_Body body = class_def->body;
  Type the_class = class_def->type;

  vector_append(env->nspc_stack, env->curr);
  env->curr = the_class->info;
  vector_append(env->class_stack, env->class_def);
  env->class_def = the_class;
  env->class_scope = 0;

  while(body && ret > 0)
  {
    switch( body->section->type )
    {
      case ae_section_stmt:
        ret = scan2_Stmt_List(env, body->section->stmt_list);
        break;
      case ae_section_class:
        ret = scan2_Class_Def(env, body->section->class_def);
        break;
      case ae_section_func:
        ret = scan2_Func_Def(env, body->section->func_def);
        break;
    }
    body = body->next;
  }

  env->class_def = vector_back(env->class_stack);
  vector_pop(env->class_stack);
  env->curr = vector_back(env->nspc_stack);
  vector_pop(env->nspc_stack);

  if(class_def->doc)
    the_class->doc = class_def->doc;

  return ret;
}

m_bool scan2_Ast(Env env, Ast ast)
{
  Ast prog = ast;
  m_bool ret = 1;
  while(prog && ret > 0)
  {
    switch(prog->section->type)
    {
      case ae_section_stmt:
        ret = scan2_Stmt_List(env, prog->section->stmt_list);
        break;
      case ae_section_func:
        ret = scan2_Func_Def(env, prog->section->func_def);
        break;
      case ae_section_class:
        ret = scan2_Class_Def(env, prog->section->class_def);
        break;
    }
    prog = prog->next;
  }
  return ret;
}
