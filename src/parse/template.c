#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "env.h"
#include "type.h"

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

static m_uint template_size(Env env, Class_Def c, Type_List call) {
  ID_List base = c->tmpl->list.list;
  m_uint size = strlen(c->type->name) + 3;
  while(base) {
    Type t = type_decl_resolve(env, call->list);
    size += strlen(t->name);
    call = call->next;
    base = base->next;
    if(base)
      size++;
  }
  return size;
}

static m_bool template_name(Env env, Class_Def c, Type_List call, m_str str) {
  ID_List base = c->tmpl->list.list;
  strcpy(str, c->type->name);
  strcat(str, "<");
  while(base) { // TODO: error checking
    Type t = type_decl_resolve(env, call->list);
    strcat(str, t->name);
    call = call->next;
    base = base->next;
    if(base)
      strcat(str, ",");
  }
  strcat(str, ">");
  return 1;
}

static ID_List template_id(Env env, Class_Def c, Type_List call) {
  m_uint size = template_size(env, c, call);
  char name[size];
  ID_List list;

  CHECK_BO(template_name(env, c, call, name))
  list = new_id_list(insert_symbol(name), call->pos);
  return list;
}

//static
m_bool template_match(ID_List base, Type_List call) {
  while(base) {
    if(!call)
      return -1;
    base = base->next;
    call = call->next;
  }
  if(call)
    return -1;
  return 1;
}

static Class_Def template_class(Env env, Class_Def def, Type_List call) {
  Type t;
  ID_List name = template_id(env, def, call);
  if((t = nspc_lookup_type1(env->curr, name->xid))) {
    free_id_list(name);
    return t->def;
  }
  return new_class_def(def->flag, name, def->ext, def->body, call->pos);
}

m_bool template_push_types(Env env, ID_List base, Type_List call) {
  nspc_push_type(env->curr);
  while(base) {
    CHECK_OB(call);
    Type t = type_decl_resolve(env, call->list);
    if(!t)
      CHECK_BB(type_unknown(call->list->xid, "template"))
    nspc_add_type(env->curr, base->xid, t);
    base = base->next;
    call = call->next;
  }
  return 1;
}

__attribute__((nonnull))
extern m_bool scan0_class_def(const Env, const Class_Def);
extern m_bool scan1_class_def(Env, Class_Def);

Type scan_type(Env env, Type t, const Type_Decl* type) {
  if(GET_FLAG(t, ae_flag_template)) {
    if(GET_FLAG(t, ae_flag_ref))
      return t;
    if(!type->types)
      CHECK_BO(err_msg(SCAN1_, type->pos,
        "you must provide template types for type '%s'", t->name))
    if(template_match(t->def->tmpl->list.list, type->types) < 0)
      CHECK_BO(err_msg(SCAN1_, type->pos, "invalid template types number"))

    CHECK_BO(template_push_types(env, t->def->tmpl->list.list, type->types))
    Class_Def a = template_class(env, t->def, type->types);
    if(a->type) {
      nspc_pop_type(env->curr);
      return a->type;
    }
    CHECK_BO(scan0_class_def(env, a))
    SET_FLAG(a->type, ae_flag_template);
    SET_FLAG(a->type, ae_flag_ref);
    a->type->owner = t->owner;
    if(GET_FLAG(t, ae_flag_builtin))
      SET_FLAG(a->type, ae_flag_builtin);
    CHECK_BO(scan1_class_def(env, a))
    nspc_pop_type(env->curr);
    if(t->info->dtor) {
      a->type->info->dtor = t->info->dtor;
      SET_FLAG(a->type, ae_flag_dtor);
      ADD_REF(t->info->dtor)
    }
    ID_List list = get_total_type_list(t);
    a->tmpl = new_tmpl_class(list, 0);
    a->tmpl->base = type->types;
    nspc_add_type(t->owner, insert_symbol(a->type->name), a->type);
    ADD_REF((t = a->type))
    t = a->type;
  } else if(type->types)
      CHECK_BO(err_msg(SCAN1_, type->pos,
            "type '%s' is not template. You should not provide template types", t->name))
  return t;
}
