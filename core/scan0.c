#include "err_msg.h"
#include "absyn.h"
#include "context.h"

static m_bool scan0_Func_Ptr(Env env, Stmt_Ptr ptr) {
  Value v;
  Type type;
  Type t = new_Type(te_func_ptr, S_name(ptr->xid));
  t->owner = env->curr;
  t->array_depth = 0;
  t->size = SZ_INT;
  t->info = new_NameSpace();
  t->info->filename = env->context->filename;
  t->parent = &t_func_ptr;
  namespace_add_type(env->curr, ptr->xid, t);
  type = type_copy(env, &t_class);
  type->actual_type = t;
  v = new_Value(type, S_name(ptr->xid));
  v->owner = env->curr;
  SET_FLAG(v, ae_value_const);
  SET_FLAG(v, ae_value_checked);
  namespace_add_value(env->curr, ptr->xid, v);
  ptr->value = v;
  return 1;
}

static m_bool scan0_Stmt(Env env, Stmt stmt) {
#ifdef DEBUG_SCAN0
  debug_msg("scan1", "stmt");
#endif
  if(!stmt)
    return 1;
  if(stmt->type == ae_stmt_funcptr)
    CHECK_BB(scan0_Func_Ptr(env, &stmt->d.stmt_ptr))
    return 1;
}
static m_bool scan0_Stmt_List(Env env, Stmt_List list) {
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

static m_bool scan0_Class_Def(Env env, Class_Def class_def) {
  Type the_class = NULL;
  m_bool ret = 1;
  Class_Body body = class_def->body;

  if(namespace_lookup_type(env->curr, class_def->name->xid, 1)) {
    err_msg(SCAN0_,  class_def->name->pos,
            "class/type '%s' is already defined in namespace '%s'",
            S_name(class_def->name->xid), env->curr->name);
    ret = -1;
    goto done;
  }

  if(isres(env, class_def->name->xid, class_def->name->pos) > 0) {
    err_msg(SCAN0_, class_def->name->pos, "...in class definition: '%s' is reserved",
            S_name(class_def->name->xid));
    ret = -1;
    goto done;
  }

  the_class = new_Type(get_type_xid(), S_name(class_def->name->xid));
  the_class->is_user = 1;
//  ADD_REF(the_class->obj);
  the_class->owner = env->curr;
  the_class->array_depth = 0;
  the_class->size = SZ_INT;
  the_class->info = new_NameSpace();
  the_class->info->filename = env->context->filename;
  the_class->parent = &t_object;
//  ADD_REF(the_class->info->obj);
  the_class->info->name = the_class->name;

  if(env->context->public_class_def == class_def)
    the_class->info->parent = env->context->nspc;
  else
    the_class->info->parent = env->curr;
  the_class->func = NULL;
  the_class->def = class_def;

  the_class->info->pre_ctor = new_VM_Code(NULL, 0, 0, the_class->name, "[in code ctor definition]");
  namespace_add_type(env->curr, insert_symbol(the_class->name), the_class);
  the_class->is_complete = 0;
  vector_append(env->nspc_stack, (vtype)env->curr);
  env->curr = the_class->info;
  vector_append(env->class_stack, (vtype)env->class_def);
  env->class_def = the_class;
  env->class_scope = 0;

  while(body && ret > 0) {
    switch(body->section->type) {
    case ae_section_stmt:
      ret = scan0_Stmt_List(env, body->section->d.stmt_list);
    case ae_section_func:
      break;
    case ae_section_class:
      ret = scan0_Class_Def(env, body->section->d.class_def);
      break;
    }
    body = body->next;
  }

  env->class_def = (Type)vector_pop(env->class_stack);
  env->curr = (NameSpace)vector_pop(env->nspc_stack);

  if(ret) {
    Value value;
    Type  type;
    type = type_copy(env, &t_class);
    type->actual_type = the_class;
    value = new_Value(type, the_class->name);
    value->owner = env->curr;
    SET_FLAG(value, ae_value_const);
    SET_FLAG(value, ae_value_checked);
    namespace_add_value(env->curr, insert_symbol(value->name), value);
    class_def->type = the_class;
//    if(env->curr == env->context->nspc) {
//      context_add_type(env->context, the_class, &the_class->obj);
    context_add_class(env->context, value, &value->obj);
//    }
  }
done:
  return ret;
}

m_bool scan0_Ast(Env env, Ast prog) {
  CHECK_OB(prog)
  while(prog) {
    switch(prog->section->type) {
    case ae_section_stmt:
      CHECK_BB(scan0_Stmt_List(env, prog->section->d.stmt_list))
      break;
    case ae_section_func:
      break;
    case ae_section_class:
      if(prog->section->d.class_def->decl == ae_key_public) {
        if(env->context->public_class_def != NULL) {
          err_msg(SCAN0_, prog->section->d.class_def->pos,
                  "more than one 'public' class defined...");
          return -1;
        }
        env->context->public_class_def = prog->section->d.class_def;
      }
      CHECK_BB(scan0_Class_Def(env, prog->section->d.class_def))
      break;
    }
    prog = prog->next;
  }
  return 1;
}
