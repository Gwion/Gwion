#include <string.h>
#include "gwion_util.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "nspc.h"
#include "template.h"

ANN static inline Type owner_type(const Type t) {
  const Nspc nspc = t->nspc ? t->nspc->parent : NULL;
  return (nspc && nspc->parent) ? nspc_lookup_type1(nspc->parent, insert_symbol(nspc->name)) : NULL;
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
  while((src = src->next))
    tmp = (tmp->next = new_id_list(src->xid, src->pos));
  return list;
}

ANN static ID_List get_total_type_list(const Type t) {
  const Type parent = owner_type(t);
  if(!parent)
    return t->def->tmpl ? t->def->tmpl->list.list : NULL;
  const Vector v = get_types(parent);
  const ID_List base = (ID_List)vector_pop(v);
  if(!base) {
    free_vector(v);
    return t->def->tmpl ? t->def->tmpl->list.list : NULL;
  }
  const ID_List types = id_list_copy(base);
  ID_List tmp = types;
  while(vector_size(v))
    tmp = (tmp->next = id_list_copy((ID_List)vector_pop(v)));
  tmp->next = t->def->tmpl->list.list;
  free_vector(v);
  return types;
}

struct tmpl_info {
  const  Class_Def cdef;
  Type_List        call;
  struct Vector_   type;
  struct Vector_   size;
  uint8_t index;
};

ANN static inline size_t tmpl_set(struct tmpl_info* info, const Type t) {
  vector_add(&info->type, (vtype)t);
  const size_t len = strlen(t->name);
  vector_add(&info->size, len);
  return len;
}

ANN static size_t template_size(const Env env, struct tmpl_info* info) {
  ID_List base = info->cdef->tmpl->list.list;
  size_t size = tmpl_set(info, info->cdef->type);
  do size += tmpl_set(info, type_decl_resolve(env, info->call->td));
  while((info->call = info->call->next) && (base = base->next) && ++size);
  return size + 16 + 3;
}

ANN static inline m_str tmpl_get(struct tmpl_info* info, m_str str) {
  const Type t = (Type)vector_at(&info->type, info->index);
  strcpy(str, t->name);
  return str += vector_at(&info->size, info->index);
}

ANN static void template_name(const Env env, struct tmpl_info* info, m_str s) {
  m_str str = s;
  str = tmpl_get(info, str);
  *str++ = '<';
  const m_uint size = vector_size(&info->type);
  for(info->index = 1; info->index < size; ++info->index) {
    str = tmpl_get(info, str);
    *str++ = (info->index < size - 1) ? ',' : '>';
   }
  if(info->cdef->type->owner == env->global_nspc)
    sprintf(str, "%p", (void*)env->curr);
  else
    *str = '\0';
}

ANEW ANN static ID_List template_id(const Env env, const Class_Def c, const Type_List call) {
  struct tmpl_info info = { .cdef=c, .call=call };
  vector_init(&info.type);
  vector_init(&info.size);
  char name[template_size(env, &info)];
  template_name(env, &info, name);
  vector_release(&info.type);
  vector_release(&info.size);
  return new_id_list(insert_symbol(name), call->td->xid->pos);
}

ANN m_bool template_match(ID_List base, Type_List call) {
  while((call = call->next) && (base = base->next));
  return !call ? 1 : -1;
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
    const Type t = call ? known_type(env, call->td, "template") : NULL;
    if(!t)
      POP_RET(-1);
    nspc_add_type(env->curr, base->xid, t);
    call = call->next;
  } while((base = base->next));
  if(!call)
    return 1;
  POP_RET(-1);
}

extern ANN m_bool scan0_class_def(const Env, const Class_Def);
extern ANN m_bool scan1_class_def(const Env, const Class_Def);
extern ANN m_bool traverse_func_def(const Env, const Func_Def);
extern ANN m_bool traverse_class_def(const Env, const Class_Def);

ANN Type scan_type(const Env env, const Type t, const Type_Decl* type) {
  if(GET_FLAG(t, ae_flag_template)) {
    if(GET_FLAG(t, ae_flag_ref))
      return t;
    if(!type->types)
      ERR_O(type->xid->pos,
        "you must provide template types for type '%s'", t->name)
    if(template_match(t->def->tmpl->list.list, type->types) < 0)
      ERR_O(type->xid->pos, "invalid template types number")
    CHECK_BO(template_push_types(env, t->def->tmpl->list.list, type->types))
    const Class_Def a = template_class(env, t->def, type->types);
    SET_FLAG(a, ae_flag_ref);
    if(a->type)
      POP_RET(a->type);
    CHECK_BO(scan0_class_def(env, a))
    SET_FLAG(a->type, ae_flag_template | ae_flag_ref);
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
    a->tmpl = new_tmpl_class(get_total_type_list(t), 0);
    a->tmpl->base = type->types;
    nspc_add_type(t->owner, insert_symbol(a->type->name), a->type);
    return a->type;
  } else if(type->types)
      ERR_O(type->xid->pos,
            "type '%s' is not template. You should not provide template types", t->name)
  return t;
}
