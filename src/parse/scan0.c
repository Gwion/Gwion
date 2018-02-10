#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"
#include "value.h"
#include "func.h"

static Type owner_type(Type t) {
  Nspc nspc = t->info;
  if(!nspc || !(nspc = nspc->parent) || !nspc->parent)
    return NULL;
  return nspc_lookup_type1(nspc->parent, insert_symbol(nspc->name));
}

static Vector get_types(Type t) {
  Vector v = new_vector();
  while(t) {
    if(GET_FLAG(t, ae_flag_template))
      vector_add(v, (vtype)t->def->tmpl->list.list);
    t = owner_type(t);
  }
  return v;
}

static ID_List id_list_copy(ID_List src) {
  ID_List tmp, list = new_id_list(src->xid, src->pos);
  src = src->next;
  tmp = list;
  while(src) {
    tmp->next = new_id_list(src->xid, src->pos);
    tmp = tmp->next;
    src = src->next;
  }
  return list;
}

static ID_List get_total_type_list(Type t) {
  Type parent = owner_type(t);
  if(!parent)
    return t->def->tmpl ? t->def->tmpl->list.list : NULL;
  Vector v = get_types(parent);
  ID_List base = (ID_List)vector_pop(v);
  if(!base) {
    free_vector(v);
    return t->def->tmpl ? t->def->tmpl->list.list : NULL;
  }
  ID_List tmp, types = id_list_copy(base);
  tmp = types;
  while(vector_size(v)) {
    base = (ID_List)vector_pop(v);
    tmp->next = id_list_copy(base);
    tmp = tmp->next;
  }
  tmp->next = t->def->tmpl->list.list;
  free_vector(v);
  return types;
}

static Value mk_class(Env env, Type base) {
  Type t = type_copy(&t_class);
  Value v = new_value(t, base->name);
  t->d.base_type = base;
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
  t->info = new_nspc(name);
  nspc_add_type(env->curr, ptr->xid, t);
  ptr->value = mk_class(env, t);
  return 1;
}

static m_bool scan0_stmt_typedef(Env env, Stmt_Typedef stmt) {
  ae_flag flag;
  Class_Def def;
  Type base = find_type(env, stmt->type->xid);
  Value v = nspc_lookup_value1(env->curr, stmt->xid);
  if(!base)
    CHECK_BB(type_unknown(stmt->type->xid, "typedef"))
  if(v)
    CHECK_BB(err_msg(SCAN0_, stmt->type->pos,
          "value '%s' already defined in this scope"
          " with type '%s'.", s_name(stmt->xid), v->m_type->name))
  CHECK_OB((base = scan_type(env, base, stmt->type)))
  if((isa(base, &t_object) < 0 && !stmt->type->array) || (stmt->type->array && !stmt->type->array->exp_list)) {
    Type t = NULL;
    if(stmt->type->array)
      base = array_type(base, stmt->type->array->depth);
    t = new_type(env->type_xid++, s_name(stmt->xid), base);
    t->size = base->size;
    SET_FLAG(t, ae_flag_checked);
    nspc_add_type(env->curr, stmt->xid, t);
    stmt->m_type = t;
  } else {
    flag = base->def ? base->def->flag : 0;
    CHECK_OB((def = new_class_def(flag, new_id_list(stmt->xid, stmt->pos),
      stmt->type, NULL, stmt->pos)))
    CHECK_BB(scan0_class_def(env, def))
    stmt->m_type = def->type;
}
  SET_FLAG(stmt->m_type, ae_flag_typedef);
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

static m_bool scan0_Stmt(Env env, Stmt stmt) {
  if(!stmt)
    return 1;
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
    if(env_class_def(env, NULL))
      CHECK_BB(err_msg(SCAN0_, class_def->pos,
            "more than one 'public' class defined..."))
    env_class_def(env, class_def);
    vector_add(&env->nspc_stack, (vtype)env->curr);
    env->curr = env->global_nspc;
  }
  return 1;
}

static m_bool scan0_class_def_pre(Env env, Class_Def class_def) {
  CHECK_BB(scan0_class_def_public(env, class_def))
  if(nspc_lookup_type1(env->curr, class_def->name->xid))
    CHECK_BB(err_msg(SCAN0_,  class_def->name->pos,
          "class/type '%s' is already defined in namespace '%s'",
          s_name(class_def->name->xid), env->curr->name))
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
  the_class->info = new_nspc(the_class->name);
  the_class->info->parent = env_class_def(env, NULL) == class_def ?
      env_nspc(env) : env->curr;
  the_class->def = class_def;
  if(strstr(the_class->name, "<")) {
    ID_List types = get_total_type_list(the_class);
    class_def->tmpl = new_tmpl_class(types, 0);
    nspc_add_type(env->curr->parent, class_def->name->xid, the_class);
    ADD_REF(the_class);
  } else
    nspc_add_type(env->curr, class_def->name->xid, the_class);
  if(class_def->tmpl)
    SET_FLAG(the_class, ae_flag_template);
  if(class_def->ext && class_def->ext->array)
    SET_FLAG(the_class, ae_flag_typedef);
  return the_class;
}

static m_bool scan0_class_def_post(Env env, Class_Def class_def) {
  (void)mk_class(env, class_def->type);
  if(env->curr == env->global_nspc)
    env->curr = (Nspc)vector_pop(&env->nspc_stack);
  return 1;
}

static m_bool scan0_section(Env env, Section* section) {
  if(section->section_type == ae_section_stmt)
    CHECK_BB(scan0_Stmt_List(env, section->d.stmt_list))
  else if(section->section_type == ae_section_class)
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
