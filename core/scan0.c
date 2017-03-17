#include "err_msg.h"
#include "absyn.h"
#include "context.h"

static m_bool scan0_Func_Ptr(Env env, Func_Ptr* ptr)
{
  Type t = new_Type(env->context);
  add_ref(t->obj);
  t->xid = te_user;
  t->name = S_name(ptr->xid);
  t->owner = env->curr;
  t->array_depth = 0;
  t->size = SZ_INT;
  t->info = new_NameSpace(); // done filename // do we nedd this one ?
  t->info->filename = env->context->filename;
  t->parent = &t_func_ptr;
//  ptr->ref = t;
  namespace_add_type(env->curr, ptr->xid, t);  // URGENT: make this global
  return 1;
}
static m_bool scan0_Class_Def(Env env, Class_Def class_def)
{
  Type the_class = NULL;
  m_bool ret = 1;
  Class_Body body = class_def->body;
  if(class_def->home) {
    /* err_msg(SCAN0_, class_def->pos, "target namespace: '%s'", class_def->home->name); */
    vector_append(env->nspc_stack, (vtype)env->curr);
    env->curr = class_def->home;
  }

  if(namespace_lookup_type( env->curr, class_def->name->xid, 1)) {
    err_msg(SCAN0_,  class_def->name->pos,
            "class/type '%s' is already defined in namespace '%s'",
            S_name(class_def->name->xid), env->curr->name);
    ret = -1;
    goto done;
  }

  if(isres(env, class_def->name->xid, class_def->name->pos) > 0) {
    err_msg(SCAN0_, class_def->name->pos, "...in class definition: '%s' is reserved",
            S_name(class_def->name->xid) );
    ret = -1;
    goto done;
  }

  the_class = new_Type(env->context);
  add_ref(the_class->obj);
  the_class->xid = te_user;
  the_class->name = S_name(class_def->name->xid);
  the_class->owner = env->curr;
  the_class->array_depth = 0;
  the_class->size = SZ_INT;
  the_class->info = new_NameSpace(); // done filename
  the_class->info->filename = env->context->filename;
  the_class->parent = &t_object;
  add_ref(the_class->info->obj);
  the_class->info->name = the_class->name;

  if(env->context->public_class_def == class_def)
    the_class->info->parent = env->context->nspc;
  else
    the_class->info->parent = env->curr;
  the_class->func = NULL;
  the_class->def = class_def;

  the_class->info->pre_ctor = new_VM_Code(NULL, 0, 0, the_class->name, "[in code ctor definition]");
  namespace_add_type(env->curr, insert_symbol(the_class->name), the_class);  // URGENT: make this global
  the_class->is_complete = 0;
  vector_append(env->nspc_stack, (vtype)env->curr);
  env->curr = the_class->info;
  vector_append(env->class_stack, (vtype)env->class_def);
  env->class_def = the_class;
  env->class_scope = 0;

  while( body && ret > 0) {
    switch( body->section->type ) {
    case ae_section_stmt:
    case ae_section_func:
      break;
    case ae_section_class:
      ret = scan0_Class_Def( env, body->section->d.class_def );
      break;
    }
    body = body->next;
  }

  env->class_def = (Type)vector_back(env->class_stack);
  vector_pop(env->class_stack);
  env->curr = (NameSpace)vector_back(env->nspc_stack);
  vector_pop(env->nspc_stack);

  if(ret) {
    Value value;
    Type  type;

    type = type_copy(env, &t_class);
    type->actual_type = the_class;
    value = new_Value(env->context, type, the_class->name);
    value->owner = env->curr;
    value->is_const = 1;
    value->is_member = 0;
    value->checked = 1;
//    add_ref(value->obj);
    add_ref(the_class->obj);
//the_class->obj->ref_count = 2;
    namespace_add_value(env->curr, insert_symbol(value->name), value);
    class_def->type = the_class;
    if(env->curr == env->context->nspc) {
      context_add_type(env->context, the_class, the_class->obj);
// 04/01/17
      context_add_class(env->context, value, value->obj);
    } else if(class_def->decl != ae_key_public) {
context_add_class(env->context, value, value->obj);
}

  }
  if(class_def->home) {
    env->curr = (NameSpace)vector_back(env->nspc_stack);
    vector_pop(env->nspc_stack);
  } else // set the current namespace as home
    class_def->home = env->curr;
done:
  return ret;
}
static m_bool scan0_Stmt(Env env, Stmt* stmt)
{
#ifdef DEBUG_SCAN0
  debug_msg("scan1", "stmt");
#endif
  m_bool ret = -1;
  if(!stmt)
    return 1;
  switch(stmt->type) {
  case ae_stmt_funcptr:
    ret = scan0_Func_Ptr(env, stmt->d.stmt_funcptr);
    break;
  default:
    ret = 1;
  }
  return ret;
}
static m_bool scan0_Stmt_List(Env env, Stmt_List list)
{
#ifdef DEBUG_SCAN0
  debug_msg("scan1", "stmt list");
#endif
  Stmt_List curr = list;
  while(curr) {
    CHECK_BB(scan0_Stmt(env, curr->stmt))
    curr = curr->next;
  }
  return 1;
}

m_bool scan0_Ast(Env env, Ast prog)
{
  m_bool ret = 1;
  CHECK_OB(prog)
  while( prog && ret > 0) {
    switch( prog->section->type ) {
    case ae_section_stmt:
      ret = scan0_Stmt_List(env, prog->section->d.stmt_list);
      break;
    case ae_section_func:
      break;
    case ae_section_class:
      if( prog->section->d.class_def->decl == ae_key_public ) {
        if(env->context->public_class_def != NULL) {
          err_msg(SCAN0_, prog->section->d.class_def->pos,
                  "more than one 'public' class defined..." );
          return -1;
        }
//        prog->section->d.class_def->home = env->user_nspc ? env->user_nspc : env->global_nspc;
        prog->section->d.class_def->home = env->global_nspc;
        env->context->public_class_def = prog->section->d.class_def;
      }
      ret = scan0_Class_Def( env, prog->section->d.class_def );
      break;
    default:
      err_msg(SCAN0_, prog->pos,
              "internal error: unrecognized program section in type checker pre-scan..." );
      ret = -1;
      break;
    }
    prog = prog->next;
  }
  return ret;
}
