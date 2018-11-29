#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "func.h"
#include "nspc.h"

ANN m_bool scan0_class_def(const Env env, const Class_Def class_def);

ANN static Value mk_class(const Env env, const Type base) {
  const Type t = type_copy(t_class);
  const Value v = new_value(env->gwion, t, base->name);
  t->d.base_type = base;
  v->owner = base->owner;
  SET_FLAG(v, const | ae_flag_checked);
  nspc_add_value(base->owner, insert_symbol(base->name), v);
  return v;
}

ANN m_bool scan0_stmt_fptr(const Env env, const Stmt_Fptr stmt) { GWDEBUG_EXE
  CHECK_BB(env_access(env, stmt->td->flag))
  CHECK_BB(already_defined(env, stmt->xid, stmt->td->xid->pos)) // test for type ?
  const m_str name = s_name(stmt->xid);
  const Type t = new_type(t_fptr->xid, name, t_fptr);
  t->owner = !(!env->class_def && GET_FLAG(stmt->td, global)) ?
    env->curr : env->global_nspc;
  t->nspc = new_nspc(name);
  t->flag = stmt->td->flag;
  stmt->type = t;
  nspc_add_type(t->owner, stmt->xid, t);
  stmt->value = mk_class(env, t);
  return 1;
}

ANN static m_bool scan0_stmt_type(const Env env, const Stmt_Type stmt) { GWDEBUG_EXE
  CHECK_BB(env_access(env, stmt->td->flag))
  const Type base = known_type(env, stmt->td, "typedef");
  CHECK_OB(base)
  CHECK_BB(already_defined(env, stmt->xid, stmt->td->xid->pos)) // test for type ?
  if(!stmt->td->types && (!stmt->td->array || !stmt->td->array->exp)) {
    const Type t = new_type(++env->type_xid, s_name(stmt->xid), base);
    t->size = base->size;
    const Nspc nspc = (!env->class_def && GET_FLAG(stmt->td, global)) ?
      env->global_nspc : env->curr;
    nspc_add_type(nspc, stmt->xid, t);
    t->owner = nspc;
    stmt->type = t;
    t->flag = stmt->td->flag | ae_flag_checked;
    if(stmt->td->array && !stmt->td->array->exp)
      SET_FLAG(t, empty);
  } else {
    const ae_flag flag = base->def ? base->def->flag : 0;
    const Class_Def def = new_class_def(flag, new_id_list(stmt->xid, stmt->td->xid->pos),
      stmt->td, NULL);
    CHECK_BB(scan0_class_def(env, def))
    stmt->type = def->type;
  }
  SET_FLAG(stmt->type, typedef);
  return 1;
}

ANN m_bool scan0_stmt_enum(const Env env, const Stmt_Enum stmt) { GWDEBUG_EXE
  CHECK_BB(env_access(env, stmt->flag))
  env_storage(env, &stmt->flag);
  if(stmt->xid) {
    const Value v = nspc_lookup_value1(env->curr, stmt->xid);
    if(v)
      ERR_B(stmt->self->pos, "'%s' already declared as variable of type '%s'.",
        s_name(stmt->xid),  v->type->name)
    CHECK_BB(already_defined(env, stmt->xid, stmt->self->pos)) // test for type ?
  }
  const Type t = type_copy(t_int);
  t->name = stmt->xid ? s_name(stmt->xid) : "int";
  t->parent = t_int;
  const Nspc nspc = GET_FLAG(stmt, global) ? env->global_nspc : env->curr;
  t->owner = nspc;
  stmt->t = t;
  if(stmt->xid) {
    nspc_add_type(nspc, stmt->xid, t);
    mk_class(env, t);
  }
  return 1;
}

ANN static Type union_type(const Env env, const Nspc nspc, const Symbol s, const m_bool add) {
  const m_str name = s_name(s);
  const Type t = type_copy(t_union);
  t->name = name;
  t->nspc = new_nspc(name);
  t->nspc->parent = nspc;
  t->owner = nspc;
  if(add) {
    nspc_add_type(nspc, s, t);
    mk_class(env, t);
  }
  return t;
}

ANN static m_bool scan0_stmt_union(const Env env, const Stmt_Union stmt) { GWDEBUG_EXE
  CHECK_BB(env_access(env, stmt->flag))
  env_storage(env, &stmt->flag);
  if(stmt->xid) {
    CHECK_BB(already_defined(env, stmt->xid, stmt->self->pos))
    const Nspc nspc = !GET_FLAG(stmt, global) ?
      env->curr : env->global_nspc;
    const Type t = union_type(env, nspc, stmt->type_xid ?: stmt->xid,
       !!stmt->type_xid);
    stmt->value = new_value(env->gwion, t, s_name(stmt->xid));
    stmt->value->owner_class = env->class_def;
    stmt->value->owner = nspc;
    nspc_add_value(nspc, stmt->xid, stmt->value);
    SET_FLAG(stmt->value, checked | stmt->flag);
    if(env->class_def && !GET_FLAG(stmt, static))
      SET_FLAG(stmt->value, member);
    if(!stmt->type_xid)
      SET_FLAG(t, op);
  } else if(stmt->type_xid) {
    const Nspc nspc = !GET_FLAG(stmt, global) ?
      env->curr : env->global_nspc;
    stmt->type = union_type(env, nspc, stmt->type_xid, 1);
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
  if(GET_FLAG(class_def, global)) {
    vector_add(&env->nspc_stack, (vtype)env->curr);
    env->curr = env->global_nspc;
  }
  CHECK_BB(already_defined(env, class_def->name->xid, class_def->name->pos)) // test for type ?
  if(isres(class_def->name->xid) > 0) {
    ERR_B(class_def->name->pos, "...in class definition: '%s' is reserved",
          s_name(class_def->name->xid))
  }
  return 1;
}

ANN static Type scan0_class_def_init(const Env env, const Class_Def class_def) { GWDEBUG_EXE
  const Type t = new_type(++env->type_xid, s_name(class_def->name->xid), t_object);
  t->owner = env->curr;
  t->nspc = new_nspc(t->name);
  t->nspc->parent = GET_FLAG(class_def, global) ? env_nspc(env) : env->curr;
  t->def = class_def;
  t->flag = class_def->flag;
  if(!strstr(t->name, "<"))
    nspc_add_type(env->curr, class_def->name->xid, t);
  if(class_def->tmpl) {
    SET_FLAG(t, template);
    SET_FLAG(class_def, template);
  }
  if(class_def->ext && class_def->ext->array)
    SET_FLAG(t, typedef);
  return t;
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
    m_uint scope;
    env_push(env, class_def->type, class_def->type->nspc, &scope);
    do CHECK_BB(scan0_section(env, body->section))
    while((body = body->next));
    env_pop(env, scope);
  }
  (void)mk_class(env, class_def->type);
  if(GET_FLAG(class_def, global))
    env->curr = (Nspc)vector_pop(&env->nspc_stack);
  return 1;
}

ANN m_bool scan0_ast(const Env env, Ast prog) { GWDEBUG_EXE
  do CHECK_BB(scan0_section(env, prog->section))
  while((prog = prog->next));
  return 1;
}
