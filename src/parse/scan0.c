#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "func.h"

m_bool scan0_class_def(Env env, Class_Def class_def);
m_bool scan0_stmt_fptr(Env env, Stmt_Ptr ptr) {
  Value v;
  m_str name = s_name(ptr->xid);
  Type type;
  Type t = new_type(te_func_ptr, name, &t_func_ptr);
  t->owner = env->curr;
  t->array_depth = 0;
  t->size = SZ_INT;
  t->info = new_nspc(name, env_filename(env));
  nspc_add_type(env->curr, ptr->xid, t);
  type = type_copy(&t_class);
  type->d.actual_type = t;
  v = new_value(type, name);
  v->owner = env->curr;
  SET_FLAG(v, ae_flag_const | ae_flag_checked);
  nspc_add_value(env->curr, ptr->xid, v);
  ptr->value = v;
  return 1;
}

static m_bool scan0_Stmt(Env env, Stmt stmt) {
  if(!stmt)
    return 1;
  if(stmt->type == ae_stmt_funcptr)
    CHECK_BB(scan0_stmt_fptr(env, &stmt->d.stmt_ptr))
  return 1;
}

static m_bool scan0_Stmt_List(Env env, Stmt_List list) {
  Stmt_List curr = list;
  while(curr) {
    CHECK_BB(scan0_Stmt(env, curr->stmt))
    curr = curr->next;
  }
  return 1;
}

static m_bool scan0_class_def_public(Env env, Class_Def class_def) {
  if(class_def->decl == ae_flag_public) {
    if(env_class_def(env, NULL)) {
      CHECK_BB(err_msg(SCAN0_, class_def->pos,
                       "more than one 'public' class defined..."))
    }
    class_def->home = env->global_nspc; // migth be user
    env_class_def(env, class_def);
    vector_add(&env->nspc_stack, (vtype)env->curr);
    env->curr = class_def->home;
  }
  return 1;
}

static m_bool scan0_class_def_pre(Env env, Class_Def class_def) {
  CHECK_BB(scan0_class_def_public(env, class_def))
  if(nspc_lookup_type1(env->curr, class_def->name->xid)) {
    CHECK_BB(err_msg(SCAN0_,  class_def->name->pos,
                     "class/type '%s' is already defined in namespace '%s'",
                     s_name(class_def->name->xid), env->curr->name))
  }

  if(isres(class_def->name->xid, class_def->name->pos) > 0) {
    CHECK_BB(err_msg(SCAN0_, class_def->name->pos, "...in class definition: '%s' is reserved",
                     s_name(class_def->name->xid)))
  }
  return 1;
}

static Type scan0_class_def_init(Env env, Class_Def class_def) {
  Type the_class = new_type(env->type_xid++, s_name(class_def->name->xid), &t_object);
  the_class->owner = env->curr;
  the_class->array_depth = 0;
  the_class->size = SZ_INT;
  the_class->info = new_nspc(the_class->name, env_filename(env));
  the_class->info->parent = env_class_def(env, NULL) == class_def ?
    env_nspc(env) : env->curr;
  the_class->d.func = NULL;
  the_class->def = class_def;
  the_class->info->pre_ctor = new_vm_code(NULL, 0, 0, the_class->name, "[in code ctor definition]");
  nspc_add_type(env->curr, class_def->name->xid, the_class);
  if(class_def->types)
    SET_FLAG(the_class, ae_flag_template);
  return the_class;
}

static m_bool scan0_class_def_body(Env env, Class_Body body) {
  while(body) {
    switch(body->section->type) {
      case ae_section_stmt:
        CHECK_BB(scan0_Stmt_List(env, body->section->d.stmt_list))
      case ae_section_func:
        break;
      case ae_section_class:
        CHECK_BB(scan0_class_def(env, body->section->d.class_def))
        break;
    }
    body = body->next;
  }
  return 1;
}

static m_bool scan0_class_def_post(Env env, Class_Def class_def) {
  Value value;
  Type  type;
  type = type_copy(&t_class);
  type->d.actual_type = class_def->type;
  value = new_value(type, class_def->type->name);
  value->owner = env->curr;
  SET_FLAG(value, ae_flag_const | ae_flag_checked);
  nspc_add_value(env->curr, insert_symbol(class_def->type->name), value);
  if(class_def->home)
    env->curr = (Nspc)vector_pop(&env->nspc_stack);
  else
    class_def->home = env->curr;
  return 1;
}

m_bool scan0_class_def(Env env, Class_Def class_def) {
  m_bool ret;

  CHECK_BB(scan0_class_def_pre(env, class_def))
  CHECK_OB((class_def->type = scan0_class_def_init(env, class_def)))
  CHECK_BB(env_push_class(env, class_def->type))
  ret = scan0_class_def_body(env, class_def->body);
  CHECK_BB(env_pop_class(env))
  CHECK_BB(scan0_class_def_post(env, class_def))
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
        CHECK_BB(scan0_class_def(env, prog->section->d.class_def))
        break;
    }
    prog = prog->next;
  }
  return 1;
}
