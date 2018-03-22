#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "value.h"
#include "func.h"

ANN m_bool scan0_class_def(const Env env, const Class_Def class_def);

ANN static Value mk_class(const Env env, const Type base) {
  const Type t = type_copy(t_class);
  const Value v = new_value(t, base->name);
  t->d.base_type = base;
  v->owner = env->curr;
  SET_FLAG(v, ae_flag_const | ae_flag_checked);
  nspc_add_value(env->curr, insert_symbol(base->name), v);
  return v;
}

ANN m_bool scan0_stmt_fptr(const Env env, const Stmt_Ptr ptr) { GWDEBUG_EXE
  const m_str name = s_name(ptr->xid);
  const Type t = new_type(t_func_ptr->xid, name, t_func_ptr);
  t->owner = env->curr;
  t->size = SZ_INT;
  t->info = new_nspc(name);
  nspc_add_type(env->curr, ptr->xid, t);
  ptr->value = mk_class(env, t);
  return 1;
}

ANN static m_bool scan0_stmt_typedef(const Env env, const Stmt_Typedef stmt) { GWDEBUG_EXE
  const Type base = type_decl_resolve(env, stmt->type);
  const Value v = nspc_lookup_value1(env->curr, stmt->xid);
  if(!base)
    CHECK_BB(type_unknown(stmt->type->xid, "typedef"))
  if(v)
    CHECK_BB(err_msg(SCAN0_, stmt->type->pos,
          "value '%s' already defined in this scope"
          " with type '%s'.", s_name(stmt->xid), v->m_type->name))
  if(!stmt->type->array || (stmt->type->array && !stmt->type->array->exp_list)) {
    Type t = NULL;
    t = new_type(env->type_xid++, s_name(stmt->xid), base);
    t->size = base->size;
    SET_FLAG(t, ae_flag_checked);
    if(stmt->type->array && !stmt->type->array->exp_list)
      SET_FLAG(t, ae_flag_empty);
    nspc_add_type(env->curr, stmt->xid, t);
    stmt->m_type = t;
  } else {
    const ae_flag flag = base->def ? base->def->flag : 0;
    const Class_Def def = new_class_def(flag, new_id_list(stmt->xid, stmt->pos),
      stmt->type, NULL, stmt->pos);
    CHECK_BB(scan0_class_def(env, def))
    REM_REF(base) // because it get's over referenced it scan_type for now
    stmt->m_type = def->type;
  }
  SET_FLAG(stmt->m_type, ae_flag_typedef);
  return 1;
}

ANN static m_bool check_enum_xid(const Env env, const Stmt_Enum stmt) {
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

ANN m_bool scan0_stmt_enum(const Env env, const Stmt_Enum stmt) { GWDEBUG_EXE
  Type t;
  if(!env->class_def && GET_FLAG(stmt, ae_flag_private))
    CHECK_BB(err_msg(SCAN1_, stmt->pos, "'private' can only be used at class scope."))
  CHECK_BB(check_enum_xid(env, stmt))
  t = type_copy(t_int);
  t->name = stmt->xid ? s_name(stmt->xid) : "int";
  t->parent = t_int;
  nspc_add_type(env->curr, stmt->xid, t);
  stmt->t = t;
  return 1;
}

ANN static m_bool scan0_stmt_union(const Env env, const Stmt_Union stmt) { GWDEBUG_EXE
  if((GET_FLAG(stmt, ae_flag_static) || GET_FLAG(stmt, ae_flag_private)) &&
      !env->class_def)
      CHECK_BB(err_msg(SCAN1_, stmt->pos,
            "'static' and 'private' can only be used at class scope."))
  if(stmt->xid) {
    const m_str name = s_name(stmt->xid);
    const Type t = type_copy(t_union);
    t->name = name;
    t->info = new_nspc(name);
    t->info->parent = env->curr;
    stmt->value = new_value(t, name);
    stmt->value->owner_class = env->class_def;
    stmt->value->owner = env->curr;
    nspc_add_value(env->curr, stmt->xid, stmt->value);
    SET_FLAG(stmt->value, ae_flag_checked | stmt->flag);
    if(env->class_def && !GET_FLAG(stmt, ae_flag_static))
      SET_FLAG(stmt->value, ae_flag_member);
  }
  return 1;
}

ANN static m_bool scan0_Stmt(const Env env, const Stmt stmt) { GWDEBUG_EXE
  if(stmt->stmt_type == ae_stmt_funcptr)
    CHECK_BB(scan0_stmt_fptr(env, &stmt->d.stmt_ptr))
  else if(stmt->stmt_type == ae_stmt_typedef)
    CHECK_BB(scan0_stmt_typedef(env, &stmt->d.stmt_type))
  else if(stmt->stmt_type == ae_stmt_enum)
    CHECK_BB(scan0_stmt_enum(env, &stmt->d.stmt_enum))
  else if(stmt->stmt_type == ae_stmt_union)
    CHECK_BB(scan0_stmt_union(env, &stmt->d.stmt_union))
  return 1;
}

ANN static m_bool scan0_Stmt_List(const Env env, Stmt_List l) { GWDEBUG_EXE
  do CHECK_BB(scan0_Stmt(env, l->stmt))
  while((l = l->next));
  return 1;
}

ANN static m_bool scan0_class_def_public(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  if(GET_FLAG(class_def, ae_flag_global)) {
    if(env_class_def(env, NULL))
      CHECK_BB(err_msg(SCAN0_, class_def->pos,
            "more than one 'public' class defined..."))
    env_class_def(env, class_def);
    vector_add(&env->nspc_stack, (vtype)env->curr);
    env->curr = env->global_nspc;
  }
  return 1;
}

ANN static m_bool scan0_class_def_pre(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  CHECK_BB(scan0_class_def_public(env, class_def))
  if(nspc_lookup_type1(env->curr, class_def->name->xid))
    CHECK_BB(err_msg(SCAN0_,  class_def->name->pos,
          "class/type '%s' is already defined in namespace '%s'",
          s_name(class_def->name->xid), env->curr->name))
  if(isres(class_def->name->xid) > 0) {
    CHECK_BB(err_msg(SCAN0_, class_def->name->pos, "...in class definition: '%s' is reserved",
          s_name(class_def->name->xid)))
  }
  return 1;
}

ANN static Type scan0_class_def_init(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  const Type the_class = new_type(++env->type_xid, s_name(class_def->name->xid), t_object);
  the_class->owner = env->curr;
  the_class->size = SZ_INT;
  the_class->info = new_nspc(the_class->name);
  the_class->info->parent = env_class_def(env, NULL) == class_def ?
      env_nspc(env) : env->curr;
  the_class->def = class_def;
  if(!strstr(the_class->name, "<"))
    nspc_add_type(env->curr, class_def->name->xid, the_class);
  if(class_def->tmpl)
    SET_FLAG(the_class, ae_flag_template);
  if(class_def->ext && class_def->ext->array)
    SET_FLAG(the_class, ae_flag_typedef);
  return the_class;
}

ANN static m_bool scan0_class_def_post(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  (void)mk_class(env, class_def->type);
  if(env->curr == env->global_nspc)
    env->curr = (Nspc)vector_pop(&env->nspc_stack);
  return 1;
}

ANN static m_bool scan0_section(const Env env, const Section* section) { GWDEBUG_EXE
  if(section->section_type == ae_section_stmt)
    CHECK_BB(scan0_Stmt_List(env, section->d.stmt_list))
  else if(section->section_type == ae_section_class)
      CHECK_BB(scan0_class_def(env, section->d.class_def))
  return 1;
}

ANN m_bool scan0_class_def(const Env env, const Class_Def class_def) { GWDEBUG_EXE
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

ANN m_bool scan0_ast(const Env env, Ast prog) { GWDEBUG_EXE
  do CHECK_BB(scan0_section(env, prog->section))
  while((prog = prog->next));
  return 1;
}
