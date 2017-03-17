/** @file src/scan1.c
 *  @brief first level scan.
 */

#include "err_msg.h"
#include "absyn.h"
#include "type.h"

static m_bool scan1_Expression(Env env, Expression exp);
static m_bool scan1_Stmt_List(Env env, Stmt_List list);
static m_bool scan1_Stmt(Env env, Stmt* stmt);
static m_bool scan1_Func_Ptr(Env env, Func_Ptr* ptr);

m_bool scan1_Decl_Expression(Env env, Decl_Expression* decl)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "decl ref:%i", decl->type->ref);
#endif
  Var_Decl_List list = decl->list;
  Var_Decl var_decl = NULL;
  Type t = find_type(env, decl->type->xid);
  if(!t) {
    err_msg(SCAN1_, decl->pos, "type '%s' unknown in declaration ", S_name(decl->type->xid->xid));
    return -1;
  }
  /*
     if(t->def && t->def->iface)
     {
     err_msg(SCAN1_, decl->pos, "type '%s' is interface. cannot instantiate", S_name(decl->type->xid->xid));
     return -1;
     }
     */
  while(list) {
    var_decl = list->self;
    decl->num_decl++;
    if(var_decl->array) {
      CHECK_BB(verify_array(var_decl->array))
      if(var_decl->array->exp_list)
        CHECK_BB(scan1_Expression( env, var_decl->array->exp_list ))
        /* no need to scan subscripts at this point */
      }
    list = list->next;
  }
  decl->m_type = t;
  add_ref(decl->m_type->obj);
  return 1;
}

static m_bool scan1_Binary_Expression(Env env, Binary_Expression* binary)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1",  "binary");
#endif
  CHECK_BB(scan1_Expression(env, binary->lhs))
  CHECK_BB(scan1_Expression(env, binary->rhs))
  return 1;
}

static m_bool scan1_Primary_Expression(Env env, Primary_Expression* primary)
{
  if(primary->type == ae_primary_hack) // func ptr ?
    CHECK_BB(scan1_Expression(env, primary->d.exp))
    return 1;
}
/*
   static m_bool scan1_Array_Lit(Env env, Primary_Expression* exp)
   {
   if(verify_array( exp->array ) < 0)
   return -1;
   return 1;
   }
   */
static m_bool scan1_Array(Env env, Array* array)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "array");
#endif
  CHECK_BB(verify_array(array->indices))
  CHECK_BB(scan1_Expression(env, array->base))
  CHECK_BB(scan1_Expression(env, array->indices->exp_list))
  return 1;
}

static m_bool scan1_Cast_Expression(Env env, Cast_Expression* cast)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "cast");
#endif
  CHECK_BB(scan1_Expression(env, cast->exp))
  return 1;
}

static m_bool scan1_Postfix_Expression(Env env, Postfix_Expression* postfix )
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "postfix");
#endif
  // check the exp
  CHECK_BB(scan1_Expression(env, postfix->exp))

  switch( postfix->op ) {
  case op_plusplus:
  case op_minusminus:
    // assignable?
    if(postfix->exp->meta != ae_meta_var) {
      err_msg(SCAN1_, postfix->exp->pos, "postfix operator '%s' cannot be used on non-mutable data-type...", op2str(postfix->op));
      return -1;
    }

    // TODO: mark somewhere we need to post increment
    return 1;
    break;

  default:
    // no match
    err_msg( SCAN1_, postfix->pos,
             "internal compiler error (pre-scan): unrecognized postfix '%i'", op2str(postfix->op));
    return -1;
  }
  return 1;
}

static m_bool scan1_Dur(Env env, Exp_Dur* dur)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1",  "dur");
#endif
  CHECK_BB(scan1_Expression(env, dur->base))
  CHECK_BB(scan1_Expression(env, dur->unit))
  return 1;
}

static m_bool scan1_Func_Call1(Env env, Expression exp_func, Expression args,
                               Func func, int pos )
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "func call1");
#endif
  CHECK_BB(scan1_Expression(env, exp_func))
  CHECK_BB(args && scan1_Expression(env, args))
  return 1;
}

static m_bool scan1_Func_Call( Env env, Func_Call* exp_func )
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "func call");
#endif
  if(exp_func->types)
    return 1;
  /*
    if(exp_func->types) {
      Type_List list = exp_func->types;
      while(list) {
        Type t = find_type(env, list->list);
        if(!t) {
          err_msg(SCAN1_, exp_func->pos, "type '%s' unknown in template call", S_name(list->list->xid));
  //        free_Type_List(exp_func->types);
  //		free(exp_func->def);
          return -1;
        }
        list = list->next;
      }
    }
  */
  return scan1_Func_Call1( env, exp_func->func, exp_func->args, exp_func->m_func, exp_func->pos );
}

static m_bool scan1_Dot_Member(Env env, Dot_Member* member)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "dot member");
#endif
  CHECK_BB(scan1_Expression(env, member->base));
  return 1;
}

static m_bool scan1_exp_if(Env env, If_Expression* exp_if)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "exp if");
#endif
  CHECK_BB(scan1_Expression(env, exp_if->cond))
  CHECK_BB(scan1_Expression(env, exp_if->if_exp))
  CHECK_BB(scan1_Expression(env, exp_if->else_exp))
  return 1;
}

static m_bool scan1_Expression(Env env, Expression exp)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "exp %p", exp);
#endif
  Expression curr = exp;
  m_bool ret = 1;
  while(curr) {
    switch(curr->exp_type) {
    case Primary_Expression_type:
      ret = scan1_Primary_Expression(env, curr->d.exp_primary);
      break;
    case Decl_Expression_type:
      ret = scan1_Decl_Expression(env, curr->d.exp_decl);
      break;
    case Unary_Expression_type:
      if(exp->d.exp_unary->code)
        ret = scan1_Stmt(env, exp->d.exp_unary->code);
      else
        ret = 1;
      break;
    case Binary_Expression_type:
      ret = scan1_Binary_Expression(env, curr->d.exp_binary);
      break;
    case Postfix_Expression_type:
      ret = scan1_Postfix_Expression(env, curr->d.exp_postfix);
      break;
    case Cast_Expression_type:
      ret = scan1_Cast_Expression(env, curr->d.exp_cast);
      break;
    case Func_Call_type:
      ret = scan1_Func_Call(env, curr->d.exp_func);
      break;
    case Array_Expression_type:
      ret = scan1_Array(env, curr->d.exp_array);
      break;
    case Dot_Member_type:
      ret = scan1_Dot_Member(env, curr->d.exp_dot);
      break;
    case Dur_Expression_type:
      ret = scan1_Dur(env, curr->d.exp_dur);
      break;
    case If_Expression_type:
      ret = scan1_exp_if(env, curr->d.exp_if);
      break;
    default:
      err_msg(SCAN1_, exp->pos, "unhandled expression type '%i'", curr->exp_type);
      ret = -1;
      break;
    }
    if(ret < 0)
      return -1;
    curr = curr->next;
  }
  return ret;
}

static m_bool scan1_Stmt_Code(Env env, Stmt_Code stmt, m_bool push)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "code");
#endif
  int ret;
  env->class_scope++;
  if(push)
    namespace_push_value(env->curr);
  ret = scan1_Stmt_List(env, stmt->stmt_list);
  if(push)
    namespace_pop_value(env->curr);
  env->class_scope--;
  return ret;
}

static m_bool scan1_return(Env env, Stmt_Return stmt )
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "return");
#endif
  return stmt->val ? scan1_Expression(env, stmt->val) : 1;
}
// TODO: same as if - ensure the type in conditional is valid

static m_bool scan1_While(Env env, Stmt_While stmt)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "while");
#endif
  CHECK_BB(scan1_Expression(env, stmt->cond))
  CHECK_BB(scan1_Stmt(env, stmt->body))
  return 1;
}

static m_bool scan1_Until(Env env, Stmt_Until stmt)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "until");
#endif
  CHECK_BB(scan1_Expression(env, stmt->cond))
  CHECK_BB(scan1_Stmt(env, stmt->body))
  return 1;
}

static m_bool scan1_For(Env env, Stmt_For stmt)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "for");
#endif
  CHECK_BB(scan1_Stmt(env, stmt->c1))
  CHECK_BB(scan1_Stmt(env, stmt->c2))
  CHECK_BB(scan1_Expression(env, stmt->c3))
  CHECK_BB(scan1_Stmt(env, stmt->body))
  return 1;
}

static m_bool scan1_Loop(Env env, Stmt_Loop stmt)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "loop");
#endif
  CHECK_BB(scan1_Expression(env, stmt->cond))
  CHECK_BB(scan1_Stmt(env, stmt->body))
  return 1;
}

static m_bool scan1_Switch(Env env, Stmt_Switch stmt)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "switch");
#endif
  return scan1_Expression(env, stmt->val) < 0 ? -1 : 1;
}

static m_bool scan1_Case(Env env, Stmt_Case stmt)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "case");
#endif
  return scan1_Expression(env, stmt->val) < 0 ? -1 : 1;
}

static m_bool scan1_If(Env env, Stmt_If stmt)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "if");
#endif
  CHECK_BB(scan1_Expression(env, stmt->cond))
  CHECK_BB(scan1_Stmt(env, stmt->if_body))
  if(stmt->else_body)
    CHECK_BB(scan1_Stmt(env, stmt->else_body))
    return 1;
}

static m_bool scan1_Enum(Env env, Stmt_Enum stmt)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "enum");
#endif
  NameSpace nspc = env->class_def ? env->class_def->info : env->curr;
  Type t = NULL;
  ID_List list = stmt->list;
  m_uint count = list ? 1 : 0;
  if(stmt->xid) {
    if(namespace_lookup_type(nspc, stmt->xid, 1)) {
      err_msg(SCAN1_, stmt->pos, "type '%s' already declared", S_name(stmt->xid));
      return -1;
    }
    if(namespace_lookup_value(env->curr, stmt->xid, 1)) {
      err_msg(SCAN1_, stmt->pos, "'%s' already declared as variable", S_name(stmt->xid));
      return -1;
    }
  }
  t = type_copy(env, &t_int);
  t->name = stmt->xid ? S_name(stmt->xid) : "int";
  t->parent = &t_int;
  namespace_add_type(nspc, stmt->xid, t);
  while(list) {
    Value v;
    if(namespace_lookup_value(nspc, list->xid, 0)) {
      err_msg(SCAN1_, stmt->pos, "in enum argument %i '%s' already declared as variable", count, S_name(list->xid));
      return -1;
    }
    v = new_Value(env->context, t, S_name(list->xid));
    add_ref(t->obj);
    add_ref(v->obj);
    v->is_const = 2;
    if(env->class_def) {
      v->owner_class = env->class_def;
      v->is_static = 1;
    }
    v->checked = 1;
    namespace_add_value(nspc, list->xid, v);
    vector_append(stmt->values, (vtype)v);
    list = list->next;
    count++;
  }
  return 1;
}

static m_bool scan1_Stmt(Env env, Stmt* stmt)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "stmt");
#endif
  m_bool ret = -1;
  Decl_List l;
  if(!stmt)
    return 1;
  // DIRTY!!! happens when '1, new Object', for instance
  if(stmt->type == 3 && !stmt->d.stmt_for) // bad thing in parser, continue
    return 1;

  switch( stmt->type ) {
  case ae_stmt_exp:
    ret = scan1_Expression(env, stmt->d.stmt_exp);
    break;
  case ae_stmt_code:
    env->class_scope++;
    ret = scan1_Stmt_Code( env, stmt->d.stmt_code, 1);
    env->class_scope--;
    break;
  case ae_stmt_return:
    ret = scan1_return( env, stmt->d.stmt_return);
    break;

  case ae_stmt_if:
    env->class_scope++;
    namespace_push_value(env->curr);
    ret = scan1_If(env, stmt->d.stmt_if);
    namespace_pop_value(env->curr);
    env->class_scope--;
    break;
  case ae_stmt_while:
    env->class_scope++;
    namespace_push_value(env->curr);
    ret = scan1_While( env, stmt->d.stmt_while);
    namespace_pop_value(env->curr);
    env->class_scope--;
    break;
  case ae_stmt_for:
    env->class_scope++;
    namespace_push_value(env->curr);
    ret = scan1_For( env, stmt->d.stmt_for);
    namespace_pop_value(env->curr);
    env->class_scope--;
    break;
  case ae_stmt_until:
    env->class_scope++;
    namespace_push_value(env->curr);
    ret = scan1_Until( env, stmt->d.stmt_until);
    namespace_pop_value(env->curr);
    env->class_scope--;
    break;
  case ae_stmt_loop:
    env->class_scope++;
    namespace_push_value(env->curr);
    ret = scan1_Loop( env, stmt->d.stmt_loop);
    namespace_pop_value(env->curr);
    env->class_scope--;
    break;
  case ae_stmt_switch:
    env->class_scope++;
    namespace_push_value(env->curr);
    ret = scan1_Switch( env, stmt->d.stmt_switch);
    namespace_pop_value(env->curr);
    env->class_scope--;
    break;
  case ae_stmt_case:
    ret = scan1_Case(env, stmt->d.stmt_case);
    break;
  case ae_stmt_enum:
    ret = scan1_Enum(env, stmt->d.stmt_enum);
    break;
  case ae_stmt_continue:
  case ae_stmt_break:
  case ae_stmt_gotolabel:
    ret = 1;
    break;
  case ae_stmt_funcptr:
    ret = scan1_Func_Ptr(env, stmt->d.stmt_funcptr);
    break;
  case ae_stmt_union:
    l = stmt->d.stmt_union->l;
    while(l) {
      if(!l->self) {
        err_msg(SCAN1_, stmt->pos, "invalid union declaration.");
        return -1;
      }
      Var_Decl_List list = l->self->list;
      Var_Decl var_decl = NULL;
/*
      if(!l->self->type) { // weird bug
        err_msg(SCAN1_, l->self->pos, "must povide type declaration in union");
        free(stmt->d.stmt_union);
        stmt->d.stmt_union = NULL;
//fake_type
//l->self->type = calloc(1, sizeof(Type_Decl));
//l->self->type->xid = calloc(1, sizeof(struct ID_List_));
        return -1;
      }
*/
      Type t = find_type(env, l->self->type->xid);
      if(!t) {// TODO better typename
        err_msg(SCAN1_, l->self->pos, "unknown type '%s' in union declaration ", S_name(l->self->type->xid->xid));
        return -1;
      }
      while(list) {
        var_decl = list->self;
        l->self->num_decl++;
        if(var_decl->array) {
          CHECK_BB(verify_array(var_decl->array))
          if(var_decl->array->exp_list) {
            err_msg(SCAN1_, l->self->pos, "array declaration must be empty in union.");
            return -1;
          }
        }
        list = list->next;
      }
      l->self->m_type = t;
      add_ref(l->self->m_type->obj);
      l = l->next;
    }
    ret = 1;
    break;
  default:
    err_msg(SCAN1_, stmt->pos, "unhandled statement type '%i', sorry.", stmt->type);
    ret = -1;
  }
  return ret;
}
static m_bool scan1_Stmt_List(Env env, Stmt_List list)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "stmt list");
#endif
  Stmt_List curr = list;
  while(curr) {
    CHECK_BB(scan1_Stmt(env, curr->stmt))
    curr = curr->next;
  }
  return 1;
}

#include "func.h"

static m_bool scan1_Func_Ptr(Env env, Func_Ptr* ptr)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "func ptr");
#endif
  // TODO: remove type on errors
  Arg_List arg_list;
  int count = 1;
  arg_list = ptr->args;
  ptr->ret_type = find_type(env, ptr->type->xid);

  if(!ptr->ret_type) {
    err_msg(SCAN1_, ptr->pos, "unknown type '%s' in func ptr declaration",  S_name(ptr->xid));
    free_Type_Decl(ptr->type);
    return -1;
  }

  if(!env->class_def && ptr->key == ae_key_static) {
    err_msg(SCAN1_, ptr->pos, "can't declare func pointer static outside of a class");
    free_Type_Decl(ptr->type);
    return -1;
  }

  while(arg_list) {
    arg_list->type = find_type(env, arg_list->type_decl->xid);
    if(!arg_list->type) {
      m_str path = type_path(arg_list->type_decl->xid);
      err_msg(SCAN1_, arg_list->pos, "'%s' unknown type in argument %i of func %s", path,  count, S_name(ptr->xid));
      free_Type_Decl(ptr->type);
      free(path);
      return -1;
    }
    count++;
    arg_list = arg_list->next;
  }
  return 1;
}


m_bool scan1_Func_Def(Env env, Func_Def f)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "func def");
#endif

  if(f->spec == ae_func_spec_dtor && !env->class_def) {
    err_msg(SCAN1_, f->pos, "dtor must be in class def!!");
    goto error;
  }

  if(f->types)  // templating. nothing to be done here
    return 1;

  if(env->class_def && env->class_def->def && env->class_def->def->iface)
    f->static_decl = ae_key_abstract;
  Arg_List arg_list = NULL;
  m_uint count = 0;

  f->ret_type = find_type(env, f->type_decl->xid);

  if(!f->ret_type) {
    err_msg(SCAN1_, f->pos, "scan1: unknown return type '%s' of func '%s'",
            S_name(f->type_decl->xid->xid), S_name(f->name));
//free(f);
//    free_Func_Def(f);
    return -1;
//    goto error;
  }

  // check if array
  if(f->type_decl->array) {
    CHECK_BB(verify_array(f->type_decl->array))

    Type t = NULL;
    Type t2 = f->ret_type;
    // should be partial and empty []
    if(f->type_decl->array->exp_list) {
      err_msg(SCAN1_, f->type_decl->array->pos, "in function '%s':", S_name(f->name) );
      err_msg(SCAN1_, f->type_decl->array->pos, "return array type must be defined with empty []'s" );
      goto error;
    }

    // create the new array type
    t = new_array_type(env, &t_array, f->type_decl->array->depth, t2, env->curr);

    // TODO: verify
    f->type_decl->ref = 1;
    f->ret_type = t;
    /*    SAFE_REF_ASSIGN( f->ret_type, t );*/
  }
  arg_list = f->arg_list;
  count = 1;

  while(arg_list) {
    arg_list->type = find_type(env, arg_list->type_decl->xid);
// check this
    if(!arg_list->type)
      /*if(!arg_list->type && !(arg_list->type = namespace_lookup_type(env->curr, f->type_decl->xid->xid, 1)))*/
      /*if(!arg_list->type || !(arg_list->type = namespace_lookup_type(env->curr, f->type_decl->xid->xid, 1)))*/
    {
      m_str path = type_path(arg_list->type_decl->xid);
      err_msg(SCAN1_, arg_list->pos, "'%s' unknown type in argument %i of func %s", path, count, S_name(f->name));
      free(path);
      goto error;
    }
    count++;
    arg_list = arg_list->next;
  }
  // if operator,
  if(f->spec == ae_func_spec_op) {
    // check argument number
    if(count > 3 || count == 1) {
      err_msg(SCAN1_, f->pos, "operators can only have one or two arguments\n");
      goto error;
    }
    // and name
    if(name2op(S_name(f->name)) < 0) {
      err_msg(SCAN1_, f->pos, "%s is not a valid operator name\n", S_name(f->name));
      goto error;
    }
  }
  if(f->code && scan1_Stmt_Code(env, f->code->d.stmt_code, 0) < 0) {
    err_msg(SCAN1_, f->pos, "...in function '%s'\n", S_name(f->name));
    goto error;
  }
  return 1;
error:
//  free_Func_Def(f);
  return -1;
}

static m_bool scan1_Class_Def(Env env, Class_Def class_def)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "class def");
#endif
  m_bool ret = 1;
  Class_Body body = class_def->body;
  Type the_class = class_def->type;
  vector_append(env->nspc_stack, (vtype)env->curr);
  env->curr = the_class->info;
  vector_append(env->class_stack, (vtype)env->class_def);
  env->class_def = the_class;
  env->class_scope = 0;

  while(body) {
    switch(body->section->type) {
    case ae_section_stmt:
      ret = scan1_Stmt_List(env, body->section->d.stmt_list);
      break;
    case ae_section_func:
      ret = scan1_Func_Def(env, body->section->d.func_def);
      break;
    case ae_section_class:
      ret = scan1_Class_Def(env, body->section->d.class_def);
      break;
    }
    CHECK_BB(ret)
    body = body->next;
  }
  env->class_def = (Type)vector_back(env->class_stack);
  vector_pop(env->class_stack);
  env->curr = (NameSpace)vector_back(env->nspc_stack);
  vector_pop(env->nspc_stack);
  return ret;
}

m_bool scan1_Ast(Env env, Ast ast)
{
#ifdef DEBUG_SCAN1
  debug_msg("scan1", "Ast");
#endif
  Ast prog = ast;
  m_bool ret = 1;
  while(prog && ret > 0) {
    switch(prog->section->type) {
    case ae_section_stmt:
      ret = scan1_Stmt_List(env, prog->section->d.stmt_list);
      break;
    case ae_section_func:
      ret = scan1_Func_Def(env, prog->section->d.func_def);
      break;
    case ae_section_class:
      ret = scan1_Class_Def(env, prog->section->d.class_def);
      break;
    }
    CHECK_BB(ret);
    prog = prog->next;
  }
  return ret;
}
