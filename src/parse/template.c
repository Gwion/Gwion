#include <string.h>
#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "err_msg.h"
#include "type.h"
#include "env.h"

ANN static Type owner_type(const Type t) {
  if(!t->nspc)
    return NULL;
  const Nspc nspc = t->nspc->parent;
  if(!nspc || !nspc->parent)
    return NULL;
  return nspc_lookup_type1(nspc->parent, insert_symbol(nspc->name));
}

ANEW ANN static Vector get_types(Type t) {
  const Vector v = new_vector();
  do if(GET_FLAG(t, ae_flag_template))
      vector_add(v, (vtype)t->def->tmpl->list.list);
  while((t = owner_type(t)));
  return v;
}

ANEW ANN static ID_List id_list_copy(ID_List src) {
  const ID_List list = new_id_list(src->xid, src->pos);
  ID_List tmp = list;
  src = src->next;
  while(src) {
    tmp->next = new_id_list(src->xid, src->pos);
    tmp = tmp->next;
    src = src->next;
  }
  return list;
}

ANN static ID_List get_total_type_list(const Type t) {
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

ANN static m_uint template_size(const Env env, const Class_Def c,
    Type_List call) {
  ID_List base = c->tmpl->list.list;
  m_uint size = strlen(c->type->name) + 3;
  while(base) {
    Type t = type_decl_resolve(env, call->td);
    size += strlen(t->name);
    call = call->next;
    base = base->next;
    if(base)
      ++size;
  }
  return size + 16;
}

ANN static m_bool template_name(const Env env, const Class_Def c, Type_List call, m_str s) {
  m_str str = s;
  ID_List base = c->tmpl->list.list;
  size_t len = strlen(c->type->name);
  strcpy(str, c->type->name);
  str += len;
  strcpy(str, "<");
  ++str;
  while(base) { // TODO: error checking
    Type t = type_decl_resolve(env, call->td);
    strcpy(str, t->name);
    str += strlen(t->name);
    call = call->next;
    base = base->next;
    if(base) {
      strcpy(str, ",");
      ++str;
    }
  }
  strcpy(str, ">");
  ++str;
  if(c->type->owner == env->global_nspc) {
    char ptr[16];
    sprintf(ptr, "%p", (void*)env->curr);
    ptr[15] = '0';
    strcpy(str, ptr);
  }
  return 1;
}

ANEW ANN static ID_List template_id(const Env env, const Class_Def c, const Type_List call) {
  m_uint size = template_size(env, c, call);
  char name[size];
  ID_List list;

  template_name(env, c, call, name);
  list = new_id_list(insert_symbol(name), call->td->xid->pos);
  return list;
}

ANN m_bool template_match(ID_List base, Type_List call) {
  do{
    CHECK_OB(call)
    call = call->next;
  } while((base = base->next));
  if(call)
    return -1;
  return 1;
}

ANN static Class_Def template_class(const Env env, const Class_Def def, const Type_List call) {
  const ID_List name = template_id(env, def, call);
  const Type t = nspc_lookup_type1(env->curr, name->xid);
  if(t) {
    free_id_list(name);
    return t->def;
  }
  return new_class_def(def->flag, name, def->ext, def->body);
}

ANN m_bool template_push_types(const Env env, ID_List base, Type_List tl) {
  Type_List call = tl;
  nspc_push_type(env->curr);
  do {
    if(!call) {
      nspc_pop_type(env->curr);
      return -1;
    }
    const Type t = known_type(env, call->td, "template");
    if(!t) {
      nspc_pop_type(env->curr);
      return -1;
    }
    nspc_add_type(env->curr, base->xid, t);
    call = call->next;
  } while((base = base->next));
  return 1;
}

extern ANN m_bool scan0_class_def(const Env, const Class_Def);
extern ANN m_bool scan1_class_def(Env, Class_Def);

ANN Type scan_type(const Env env, Type t, const Type_Decl* type) {
  if(GET_FLAG(t, ae_flag_template)) {
    if(GET_FLAG(t, ae_flag_ref))
      return t;
    if(!type->types)
      ERR_O(type->xid->pos,
        "you must provide template types for type '%s'", t->name)
    if(template_match(t->def->tmpl->list.list, type->types) < 0)
      ERR_O(type->xid->pos, "invalid template types number")

    CHECK_BO(template_push_types(env, t->def->tmpl->list.list, type->types))
    Class_Def a = template_class(env, t->def, type->types);
    if(a->type) {
      nspc_pop_type(env->curr);
      return a->type;
    }
    CHECK_BO(scan0_class_def(env, a))
    SET_FLAG(a->type, ae_flag_template);
    SET_FLAG(a->type, ae_flag_ref);
    SET_FLAG(a, ae_flag_ref);
    a->type->owner = t->owner;
    if(GET_FLAG(t, ae_flag_builtin))
      SET_FLAG(a->type, ae_flag_builtin);
    CHECK_BO(scan1_class_def(env, a))
    nspc_pop_type(env->curr);
    if(t->nspc->dtor) {
      a->type->nspc->dtor = t->nspc->dtor;
      SET_FLAG(a->type, ae_flag_dtor);
      ADD_REF(t->nspc->dtor)
    }
    ID_List list = get_total_type_list(t);
    a->tmpl = new_tmpl_class(list, 0);
    a->tmpl->base = type->types;
    nspc_add_type(t->owner, insert_symbol(a->type->name), a->type);
    t = a->type;
  } else if(type->types)
      ERR_O(type->xid->pos,
            "type '%s' is not template. You should not provide template types", t->name)
  return t;
}

ANN m_bool traverse_class_def(const Env, const Class_Def def);
ANN m_bool traverse_template(const Env env, const Class_Def def) {
  CHECK_BB(template_push_types(env, def->tmpl->list.list, def->tmpl->base))
  CHECK_BB(traverse_class_def(env, def))
  nspc_pop_type(env->curr);
  return 1;

}
