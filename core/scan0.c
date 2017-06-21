#include "err_msg.h"
#include "absyn.h"
#include "context.h"
#include "type.h"

static m_bool scan0_Stmt_Typedef(Env env, Stmt_Ptr ptr) {
  Value v;
  m_str name = S_name(ptr->xid);
  Type type;
  Type t = new_type(te_func_ptr, name);
  t->owner = env->curr;
  t->array_depth = 0;
  t->size = SZ_INT;
  t->parent = &t_func_ptr;
  t->info = new_nspc(name, env->context->filename);
  nspc_add_type(env->curr, ptr->xid, t);
  type = type_copy(env, &t_class);
  type->d.actual_type = t;
  v = new_value(type, name);
  v->owner = env->curr;
  SET_FLAG(v, ae_flag_const | ae_flag_checked);
  nspc_add_value(env->curr, ptr->xid, v);
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
    CHECK_BB(scan0_Stmt_Typedef(env, &stmt->d.stmt_ptr))
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

  if(nspc_lookup_type(env->curr, class_def->name->xid, 1)) {
    CHECK_BB(err_msg(SCAN0_,  class_def->name->pos,
            "class/type '%s' is already defined in namespace '%s'",
            S_name(class_def->name->xid), env->curr->name))
  }

  if(isres(env, class_def->name->xid, class_def->name->pos) > 0) {
    CHECK_BB(err_msg(SCAN0_, class_def->name->pos, "...in class definition: '%s' is reserved",
            S_name(class_def->name->xid)))
  }

  the_class = new_type(env->type_xid++, S_name(class_def->name->xid));
  the_class->owner = env->curr;
  the_class->array_depth = 0;
  the_class->size = SZ_INT;
  the_class->info = new_nspc(the_class->name, env->context->filename);
  the_class->parent = &t_object;

  if(env->context->public_class_def == class_def)
    the_class->info->parent = env->context->nspc;
  else
    the_class->info->parent = env->curr;
  the_class->d.func = NULL;
  the_class->def = class_def;

  the_class->info->pre_ctor = new_vm_code(NULL, 0, 0, the_class->name, "[in code ctor definition]");
  nspc_add_type(env->curr, insert_symbol(the_class->name), the_class);
  vector_add(&env->nspc_stack, (vtype)env->curr);
  env->curr = the_class->info;
  vector_add(&env->class_stack, (vtype)env->class_def);
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

  env->class_def = (Type)vector_pop(&env->class_stack);
  env->curr = (Nspc)vector_pop(&env->nspc_stack);

  if(ret) {
    Value value;
    Type  type;
    type = type_copy(env, &t_class);
    type->d.actual_type = the_class;
    value = new_value(type, the_class->name);
    value->owner = env->curr;
    SET_FLAG(value, ae_flag_const | ae_flag_checked);
    nspc_add_value(env->curr, insert_symbol(value->name), value);
    vector_add(&env->context->new_values, (vtype)value);
    class_def->type = the_class;
  }
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
      if(prog->section->d.class_def->decl == ae_flag_public) {
        if(env->context->public_class_def != NULL) {
          CHECK_BB(err_msg(SCAN0_, prog->section->d.class_def->pos,
                  "more than one 'public' class defined..."))
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
