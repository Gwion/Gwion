#define PARSE
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "value.h"
#include "func.h"

ANN m_bool scan0_class_def(const Env env, const Class_Def class_def);

ANN static Value mk_class(const Type base) {
  const Type t = type_copy(t_class);
  const Value v = new_value(t, base->name);
  t->d.base_type = base;
  v->owner = base->owner;
  SET_FLAG(v, ae_flag_const | ae_flag_checked);
  nspc_add_value(base->owner, insert_symbol(base->name), v);
  return v;
}

ANN m_bool scan0_stmt_fptr(const Env env, const Stmt_Fptr stmt) { GWDEBUG_EXE
  CHECK_BB(env_access(env, stmt->td->flag))
  const m_str name = s_name(stmt->xid);
  const Value v = nspc_lookup_value1(env->curr, stmt->xid);
  if(v)
    ERR_B(SCAN0_, stmt->td->pos,
          "value '%s' already defined in this scope"
          " with type '%s'.", s_name(stmt->xid), v->type->name)
  const Type t = new_type(t_fptr->xid, name, t_fptr);
  t->owner = !(!env->class_def && GET_FLAG(stmt->td, ae_flag_global)) ?
    env->curr : env->global_nspc;
  t->size = SZ_INT;
  t->nspc = new_nspc(name);
  t->flag = stmt->td->flag;
  stmt->type = t;
  nspc_add_type(t->owner, stmt->xid, t);
  stmt->value = mk_class(t);
  return 1;
}

ANN static m_bool scan0_stmt_type(const Env env, const Stmt_Type stmt) { GWDEBUG_EXE
  CHECK_BB(env_access(env, stmt->td->flag))
  const Type base = type_decl_resolve(env, stmt->td);
  const Value v = nspc_lookup_value1(env->curr, stmt->xid);
  if(!base)
    CHECK_BB(type_unknown(stmt->td->xid, "typedef"))
  if(v)
    ERR_B(SCAN0_, stmt->td->pos,
          "value '%s' already defined in this scope"
          " with type '%s'.", s_name(stmt->xid), v->type->name)
  if(!stmt->td->types && (!stmt->td->array || !stmt->td->array->exp)) {
    const Type t = new_type(++env->type_xid, s_name(stmt->xid), base);
    t->size = base->size;
    const Nspc nspc = (!env->class_def && GET_FLAG(stmt->td, ae_flag_global)) ?
      env->global_nspc : env->curr;
    nspc_add_type(nspc, stmt->xid, t);
    t->owner = nspc;
    stmt->type = t;
    t->flag = stmt->td->flag | ae_flag_checked;
    if(stmt->td->array && !stmt->td->array->exp) {
//ADD_REF(base->nspc)
//REM_REF(t_array->nspc)
//ADD_REF((t->nspc = t_array->nspc))
//t->nspc = t_array->nspc;
      SET_FLAG(t, ae_flag_empty);

}
  } else {
    const ae_flag flag = base->def ? base->def->flag : 0;
    const Class_Def def = new_class_def(flag, new_id_list(stmt->xid, stmt->td->pos),
      stmt->td, NULL);
    CHECK_BB(scan0_class_def(env, def))
    if(stmt->td->array)
      REM_REF(base)
    stmt->type = def->type;
  }
  SET_FLAG(stmt->type, ae_flag_typedef);
  return 1;
}

ANN m_bool scan0_stmt_enum(const Env env, const Stmt_Enum stmt) { GWDEBUG_EXE
  CHECK_BB(env_access(env, stmt->flag))
  env_storage(env, &stmt->flag);
  if(stmt->xid && nspc_lookup_value0(env->curr, stmt->xid))
    ERR_B(SCAN0_, stmt->self->pos,
          "'%s' already declared as variable", s_name(stmt->xid))
  const Type t = type_copy(t_int);
  t->name = stmt->xid ? s_name(stmt->xid) : "int";
  t->parent = t_int;
  const Nspc nspc = GET_FLAG(stmt, ae_flag_global) ? env->global_nspc : env->curr;
  t->owner = nspc;
  nspc_add_type(nspc, stmt->xid, t);
  stmt->t = t;
  return 1;
}

ANN static m_bool scan0_stmt_union(const Env env, const Stmt_Union stmt) { GWDEBUG_EXE
  CHECK_BB(env_access(env, stmt->flag))
  env_storage(env, &stmt->flag);
  if(stmt->xid) {
    const m_str name = s_name(stmt->xid);
    const Type t = type_copy(t_union);
    const Nspc nspc = !GET_FLAG(stmt, ae_flag_global) ?
      env->curr : env->global_nspc;
    t->name = name;
    t->nspc = new_nspc(name);
    t->nspc->parent = nspc;
    t->owner = nspc;
    stmt->value = new_value(t, name);
    stmt->value->owner_class = env->class_def;
    stmt->value->owner = nspc;
    nspc_add_value(nspc, stmt->xid, stmt->value);
    SET_FLAG(stmt->value, ae_flag_checked | stmt->flag);
    if(env->class_def && !GET_FLAG(stmt, ae_flag_static))
      SET_FLAG(stmt->value, ae_flag_member);
  }
  return 1;
}

ANN static m_bool scan0_Stmt(const Env env, const Stmt stmt) { GWDEBUG_EXE
  if(stmt->stmt_type == ae_stmt_fptr)
    return scan0_stmt_fptr(env, &stmt->d.stmt_fptr);
  if(stmt->stmt_type == ae_stmt_type)
    return scan0_stmt_type(env, &stmt->d.stmt_type);
  if(stmt->stmt_type == ae_stmt_enum)
    return scan0_stmt_enum(env, &stmt->d.stmt_enum);
  if(stmt->stmt_type == ae_stmt_union)
    return scan0_stmt_union(env, &stmt->d.stmt_union);
  return 1;
}

ANN static m_bool scan0_Stmt_List(const Env env, Stmt_List l) { GWDEBUG_EXE
  do CHECK_BB(scan0_Stmt(env, l->stmt))
  while((l = l->next));
  return 1;
}

ANN static m_bool scan0_class_def_pre(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  CHECK_BB(env_access(env, class_def->flag))
  env_storage(env, &class_def->flag);
  if(GET_FLAG(class_def, ae_flag_global)) {
    vector_add(&env->nspc_stack, (vtype)env->curr);
    env->curr = env->global_nspc;
  }
  if(nspc_lookup_type1(env->curr, class_def->name->xid))
    ERR_B(SCAN0_,  class_def->name->pos,
          "class/type '%s' is already defined in namespace '%s'",
          s_name(class_def->name->xid), env->curr->name)
  if(isres(class_def->name->xid) > 0) {
    ERR_B(SCAN0_, class_def->name->pos, "...in class definition: '%s' is reserved",
          s_name(class_def->name->xid))
  }
  return 1;
}

ANN static Type scan0_class_def_init(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  const Type the_class = new_type(++env->type_xid, s_name(class_def->name->xid), t_object);
  the_class->owner = env->curr;
  the_class->size = SZ_INT;
  the_class->nspc = new_nspc(the_class->name);
  the_class->nspc->parent = GET_FLAG(class_def, ae_flag_global) ?
      env_nspc(env) : env->curr;
  the_class->def = class_def;
  the_class->flag = class_def->flag;
  if(!strstr(the_class->name, "<"))
    nspc_add_type(env->curr, class_def->name->xid, the_class);
  if(class_def->tmpl)
    SET_FLAG(the_class, ae_flag_template);
  if(class_def->ext && class_def->ext->array)
    SET_FLAG(the_class, ae_flag_typedef);
  return the_class;
}

ANN static m_bool scan0_section(const Env env, const Section* section) { GWDEBUG_EXE
  if(section->section_type == ae_section_stmt)
    return scan0_Stmt_List(env, section->d.stmt_list);
  if(section->section_type == ae_section_class)
    return scan0_class_def(env, section->d.class_def);
  return 1;
}

ANN m_bool scan0_class_def(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  CHECK_BB(scan0_class_def_pre(env, class_def))
  CHECK_OB((class_def->type = scan0_class_def_init(env, class_def)))
  if(class_def->body) {
    Class_Body body = class_def->body;
    m_uint class_scope;
    env_push(env, class_def->type, class_def->type->nspc, &class_scope);
    do CHECK_BB(scan0_section(env, body->section))
    while((body = body->next));
    env_pop(env, class_scope);
  }
  (void)mk_class(class_def->type);
  if(GET_FLAG(class_def, ae_flag_global))
    env->curr = (Nspc)vector_pop(&env->nspc_stack);
  return 1;
}

ANN m_bool scan0_ast(const Env env, Ast prog) { GWDEBUG_EXE
  do CHECK_BB(scan0_section(env, prog->section))
  while((prog = prog->next));
  return 1;
}
