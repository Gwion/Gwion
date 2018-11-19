#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "nspc.h"

ANN2(2) Type new_type(const m_uint xid, const m_str name, const Type parent) {
  const Type type = mp_alloc(Type);
  type->xid    = xid;
  type->name   = name;
  type->parent = parent;
  INIT_OO(type, e_type_obj);
  return type;
}

ANN void free_type(Type a) {
  if(GET_FLAG(a, ae_flag_template))
    free_class_def(a->def);
  if(a->nspc)
    REM_REF(a->nspc);
  mp_free(Type, a);
}

ANN Type type_copy(const Type type) {
  const Type a = new_type(type->xid, type->name, type->parent);
  a->nspc          = type->nspc;
  a->owner         = type->owner;
  a->size          = type->size;
  a->d.base_type   = type->d.base_type;
  a->array_depth   = type->array_depth;
  a->def           = type->def;
  return a;
}

ANN m_bool isa(const restrict Type var, const restrict Type parent) {
  return (var->xid == parent->xid) ? 1 : var->parent ? isa(var->parent, parent) : -1;
}

ANN Type find_common_anc(const restrict Type lhs, const restrict Type rhs) {
  return isa(lhs, rhs) > 0 ? rhs : isa(rhs, lhs) > 0 ? lhs : NULL;
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

__attribute__((returns_nonnull))
ANN Type template_parent(const Type type) {
  const m_str name = get_type_name(type->name, 0);
  const Type t = nspc_lookup_type1(type->nspc->parent, insert_symbol(name));
  return t;
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

ANN m_str get_type_name(const m_str s, const m_uint index) {
  m_str name = strstr(s, "<");
  m_uint i = 0;
  m_uint lvl = 0;
  m_uint n = 1;
  const size_t len = name ? strlen(name) : 0;
  const size_t slen = strlen(s);
  const size_t tlen = slen -len + 1;
  char c[slen];

  if(!name)
    return index ? NULL : s_name(insert_symbol(s));
  if(index == 0) {
    snprintf(c, tlen, "%s", s);
    return s_name(insert_symbol(c));
  }
  while(*name++) {
    if(*name == '<')
      lvl++;
    else if(*name == '>' && !lvl--)
      break;
    if(*name == ',' && !lvl) {
      ++name;
      ++n;
    }
    if(n == index)
      c[i++] = *name;
  }
  c[i] = '\0';
  return strlen(c) ? s_name(insert_symbol(c)) : NULL;
}
