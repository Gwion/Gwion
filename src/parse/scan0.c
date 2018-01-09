#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "func.h"

static Value mk_class(Env env, Type base) {
  Type t = type_copy(&t_class);
  Value v = new_value(t, base->name);
  t->d.actual_type = base;
  v->owner = env->curr;
  SET_FLAG(v, ae_flag_const | ae_flag_checked);
  nspc_add_value(env->curr, insert_symbol(base->name), v);
  return v;
}

m_bool scan0_class_def(Env env, Class_Def class_def);
m_bool scan0_stmt_fptr(Env env, Stmt_Ptr ptr) {
  m_str name = s_name(ptr->xid);
  Type t = new_type(te_func_ptr, name, &t_func_ptr);
  t->owner = env->curr;
  t->size = SZ_INT;
  t->info = new_nspc(name, env_filename(env));
  nspc_add_type(env->curr, ptr->xid, t);
  ptr->value = mk_class(env, t);
  return 1;
}

static m_bool scan0_stmt_typedef(Env env, Stmt_Typedef stmt) {
  Type base = find_type(env, stmt->type->xid);
  Value v = nspc_lookup_value1(env->curr, stmt->xid);
  if(v)
    CHECK_BB(err_msg(SCAN0_, stmt->type->pos,
          "value '%s' already defined in this scope"
          " with type '%s'.", s_name(stmt->xid), v->m_type->name))
  if(!base) {
    char c[id_list_len(stmt->type->xid)];
    type_path(c, stmt->type->xid);
    CHECK_BB(err_msg(SCAN0_, stmt->type->pos,
          "type '%s' unknown in typedef '%s'",
          s_name(stmt->type->xid->xid), s_name(stmt->xid)))
  }
  if(stmt->type->array) {
    Type t = base;
    base = new_array_type(env, stmt->type->array->depth, t, env->curr);
    base->e.exp_list = stmt->type->array->exp_list;
  } else {
    Type t = base;
    base = type_copy(t);
    if(t->info)
      ADD_REF(t->info);
    base->parent = t;
    base->d.array_type = t;
    stmt->m_type = t;
  }
  SET_FLAG(base, ae_flag_typedef);
  base->name = s_name(stmt->xid);
  nspc_add_type(env->curr, stmt->xid, base);
  v = mk_class(env, base);
  SET_FLAG(v, ae_flag_typedef);
  SET_FLAG(v->m_type, ae_flag_typedef);
  return 1;
}

static m_bool check_enum_xid(Env env, Stmt_Enum stmt) {
  if(stmt->xid) {
    if(nspc_lookup_type0(env->curr, stmt->xid))
      CHECK_BB(err_msg(SCAN1_, stmt->pos,
            "type '%s' already declared", s_name(stmt->xid)))
    if(nspc_lookup_value0(env->curr, stmt->xid))
      CHECK_BB(err_msg(SCAN1_, stmt->pos,
            "'%s' already declared as variable", s_name(stmt->xid)))
  }
  return 1;
}

m_bool scan0_stmt_enum(Env env, Stmt_Enum stmt) {
  Type t;
  if(!env->class_def && GET_FLAG(stmt, ae_flag_private))
    CHECK_BB(err_msg(SCAN1_, stmt->pos, "'private' can only be used at class scope."))
  CHECK_BB(check_enum_xid(env, stmt))
  t = type_copy(&t_int);
  t->name = stmt->xid ? s_name(stmt->xid) : "int";
  t->parent = &t_int;
  nspc_add_type(env->curr, stmt->xid, t);
  stmt->t = t;
  return 1;
}

static m_bool scan0_stmt_union(Env env, Stmt_Union stmt) {
  if((GET_FLAG(stmt, ae_flag_static) || GET_FLAG(stmt, ae_flag_private)) &&
      !env->class_def)
      CHECK_BB(err_msg(SCAN1_, stmt->pos,
            "'static' and 'private' can only be used at class scope."))
  if(stmt->xid) {
    m_str name = s_name(stmt->xid);
    Type t = type_copy(&t_union);
    t->name = name;
    t->info = new_nspc(name, "[union declarator]");
    t->info->parent = env->curr;
    stmt->value = new_value(t, name);
    stmt->value->owner_class = env->class_def;
    stmt->value->owner = env->curr;
    nspc_add_value(env->curr, stmt->xid, stmt->value);
    SET_FLAG(stmt->value, ae_flag_checked | stmt->flag);
    if(env->class_def && !GET_FLAG(stmt, ae_flag_static)) // TODO: enable static
      SET_FLAG(stmt->value, ae_flag_member);
  }
  return 1;
}

static m_bool scan0_Stmt(Env env, Stmt stmt) {
  if(!stmt)
    return 1;
  if(stmt->type == ae_stmt_funcptr)
    CHECK_BB(scan0_stmt_fptr(env, &stmt->d.stmt_ptr))
  else if(stmt->type == ae_stmt_typedef)
    CHECK_BB(scan0_stmt_typedef(env, &stmt->d.stmt_type))
  else if(stmt->type == ae_stmt_enum)
    CHECK_BB(scan0_stmt_enum(env, &stmt->d.stmt_enum))
  else if(stmt->type == ae_stmt_union)
    CHECK_BB(scan0_stmt_union(env, &stmt->d.stmt_union))
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
  if(GET_FLAG(class_def, ae_flag_global)) {
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
  the_class->size = SZ_INT;
  the_class->info = new_nspc(the_class->name, env_filename(env));
  the_class->info->parent = env_class_def(env, NULL) == class_def ?
    env_nspc(env) : env->curr;
  the_class->e.def = class_def;
  the_class->info->pre_ctor = new_vm_code(NULL, 0, 0, the_class->name, "[in code ctor definition]");
  if(strstr(the_class->name, "<")) {
    nspc_add_type(env->curr->parent, class_def->name->xid, the_class);
    ADD_REF(the_class);
  } else
    nspc_add_type(env->curr, class_def->name->xid, the_class);
  if(class_def->types)
    SET_FLAG(the_class, ae_flag_template);
  return the_class;
}

static m_bool scan0_class_def_post(Env env, Class_Def class_def) {
  (void)mk_class(env, class_def->type);
  if(class_def->home)
    env->curr = (Nspc)vector_pop(&env->nspc_stack);
  else
    class_def->home = env->curr;
  return 1;
}

static m_bool scan0_section(Env env, Section* section) {
  if(section->type == ae_section_stmt)
    CHECK_BB(scan0_Stmt_List(env, section->d.stmt_list))
  else if(section->type == ae_section_class)
      CHECK_BB(scan0_class_def(env, section->d.class_def))
  return 1;
}

m_bool scan0_class_def(Env env, Class_Def class_def) {
  Class_Body body = class_def->body;

  CHECK_BB(scan0_class_def_pre(env, class_def))
  CHECK_OB((class_def->type = scan0_class_def_init(env, class_def)))
  CHECK_BB(env_push_class(env, class_def->type))
  while(body) {
    CHECK_BB(scan0_section(env, body->section))
    body = body->next;
  }
  CHECK_BB(env_pop_class(env))
  CHECK_BB(scan0_class_def_post(env, class_def))
  return 1;
}

m_bool scan0_Ast(Env env, Ast prog) {
  while(prog) {
    CHECK_BB(scan0_section(env, prog->section))
    prog = prog->next;
  }
  return 1;
}
