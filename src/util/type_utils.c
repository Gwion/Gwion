#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "map.h"
#include "absyn.h"
#include "oo.h"
#include "value.h"
#include "type.h"
#include "err_msg.h"

ANN m_bool isa(const restrict Type var, const restrict Type parent) {
  return (var->xid == parent->xid) ? 1 : var->parent ? isa(var->parent, parent) : -1;
}

ANN m_bool isres(const Symbol xid) {
  const m_str s = s_name(xid);
  if(!strcmp(s, "this") || !strcmp(s, "now") ||
     !strcmp(s, "me")   || !strcmp(s, "vararg") ||
     !name2op(s)) {
    err_msg(TYPE_, 0, "%s is reserved.", s_name(xid));
    return 1;
  }
  return -1;
}

ANN Type find_common_anc(const restrict Type lhs, const restrict Type rhs) {
  return isa(lhs, rhs) > 0 ? rhs : isa(rhs, lhs) > 0 ? lhs : NULL;
}

m_uint num_digit(const m_uint i) {
  return i ? (m_uint)floor(log10(i) + 1) : 1;
}

ANN static Type find_typeof(const Env env, ID_List path) {
  Value v = nspc_lookup_value2(env->curr, path->xid);
  Type t = actual_type(v->type);
  path = path->next;
  while(path) {
    CHECK_OO((v = find_value(t, path->xid)))
    t = v->type;
    path = path->next;
  }
  return v->type;
}

ANN Type find_type(const Env env, ID_List path) {
  Type type;

  if(path->ref)
    return find_typeof(env, path->ref);
  CHECK_OO((type = nspc_lookup_type1(env->curr, path->xid)))
  Nspc nspc = type->nspc;
  path = path->next;

  while(path) {
    const Symbol xid = path->xid;
    Type t = nspc_lookup_type1(nspc, xid);
    while(!t && type && type->parent) {
      t = nspc_lookup_type2(type->parent->nspc, xid);
      type = type->parent;
    }
    if(!t)
      ERR_O(UTIL_, path->pos,
            "...(cannot find class '%s' in nspc '%s')", s_name(xid), nspc->name)
    type = t;
    nspc = type->nspc;
    path = path->next;
  }
  return type;
}


#define describe_find(name, t)                                 \
ANN t find_##name(const Type type, const Symbol xid) {         \
  const t val = nspc_lookup_##name##2(type->nspc, xid);        \
  if(val)                                                      \
    return val;                                                \
  return type->parent ? find_##name(type->parent, xid) : NULL; \
}
describe_find(value, Value)
describe_find(func,  Func)

ANN m_uint id_list_len(ID_List l) {
  m_uint len = 0;
  do len += strlen(s_name(l->xid));
  while((l = l->next) && ++len);
  return len + 1;
}

ANN void type_path(const m_str str, ID_List l) {
  m_str s = str;
  do {
    const m_str name = s_name(l->xid);
    strcpy(s, name);
    s += strlen(name);
    if(l->next)
      strcpy(s++, ".");
  }
  while((l = l->next));
}

ANN Type array_base(Type t) {
  while(GET_FLAG(t, ae_flag_typedef))
    t = t->parent;
  return t->d.base_type;
}

ANN Type array_type(const Type base, const m_uint depth) {
  m_uint i = depth + 1;
  size_t len = strlen(base->name);
  char name[len + 2* depth + 1];

  strcpy(name, base->name);
  while(--i) {
    strcpy(name+len, "[]");
    len += 2;
  }
  const Symbol sym = insert_symbol(name);
  const Type type = nspc_lookup_type1(base->owner, sym);
  if(type)
    return type;
  const Type t = new_type(t_array->xid, base->name, t_array);
  t->name = s_name(sym);
  t->size = SZ_INT;
  t->array_depth = depth;
  t->d.base_type = base;
  t->nspc = t_array->nspc;
  ADD_REF(t->nspc);
  SET_FLAG(t, ae_flag_checked);
  t->owner = base->owner;
  nspc_add_type(base->owner, sym, t);
  return t;
}

m_bool check_array_empty(const Array_Sub a, const m_str orig) {
  if(a->exp)
    ERR_B(SCAN1_, a->pos, "type must be defined with empty []'s"
          " in %s declaration", orig)
  return 1;
}

ANN m_bool type_ref(Type t) {
  do {
    if(GET_FLAG(t, ae_flag_empty))
      return 1;
    if(GET_FLAG(t, ae_flag_typedef) && t->def)
      if(t->def->ext && t->def->ext->array && !t->def->ext->array->exp)
        return 1;
  } while((t = t->parent));
  return 0;
}

ANN m_bool already_defined(const Env env, const Symbol s, const int pos) {
  const Value v = nspc_lookup_value0(env->curr, s);
  return v ? err_msg(TYPE_, pos,
    "'%s' already declared as variable of type '%s'.", s_name(s), v->type->name) : 1;
}
