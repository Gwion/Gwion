#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "nspc.h"
#include "vm.h"
#include "parse.h"
#include "gwion.h"

ANN static void free_type(Type a, Gwion gwion) {
  if(GET_FLAG(a, template))
    free_class_def(gwion->mp, a->e->def);
  if(a->nspc)
    REM_REF(a->nspc, gwion);
  mp_free(gwion->mp, Type, a);
}

Type new_type(MemPool p, const m_uint xid, const m_str name, const Type parent) {
  const Type type = mp_alloc(p, Type);
  type->xid    = xid;
  type->name   = name;
  type->e = mp_alloc(p, TypeInfo);
  type->e->parent = parent;
  if(type->e->parent)
    type->size = parent->size;
  INIT_OO(p, type, free_type);
  return type;
}

ANN Type type_copy(MemPool p, const Type type) {
  const Type a = new_type(p, type->xid, type->name, type->e->parent);
  a->nspc          = type->nspc;
  a->e->owner         = type->e->owner;
  a->size          = type->size;
  a->e->d.base_type   = type->e->d.base_type;
  a->array_depth   = type->array_depth;
  a->e->def           = type->e->def;
  return a;
}

ANN m_bool isa(const restrict Type var, const restrict Type parent) {
  return (var->xid == parent->xid) ? 1 : var->e->parent ? isa(var->e->parent, parent) : -1;
}

ANN Type find_common_anc(const restrict Type lhs, const restrict Type rhs) {
  return isa(lhs, rhs) > 0 ? rhs : isa(rhs, lhs) > 0 ? lhs : NULL;
}

#define describe_find(name, t)                                       \
ANN t find_##name(const Type type, const Symbol xid) {               \
  if(type->nspc) {                                                   \
  const t val = nspc_lookup_##name##0(type->nspc, xid);              \
  if(val)                                                            \
    return val;                                                      \
  }                                                                  \
  return type->e->parent ? find_##name(type->e->parent, xid) : NULL; \
}
describe_find(value, Value)
describe_find(func,  Func)

ANN Type typedef_base(Type t) {
  while(GET_FLAG(t, typedef))
    t = t->e->parent;
  return t;
}

ANN Type array_base(Type type) {
  const Type t = typedef_base(type);
  return t->e->d.base_type;
}

ANN Type array_type(const Env env, const Type base, const m_uint depth) {
  m_uint i = depth + 1;
  size_t len = strlen(base->name);
  char name[len + 2* depth + 1];
  strcpy(name, base->name);
  while(--i) {
    strcpy(name+len, "[]");
    len += 2;
  }
  const Symbol sym = insert_symbol(name);
  const Type type = nspc_lookup_type1(base->e->owner, sym);
  if(type)
    return type;
  const Type t = new_type(env->gwion->mp, t_array->xid, base->name, t_array);
  t->name = s_name(sym);
  t->size = SZ_INT;
  t->array_depth = depth + base->array_depth;
  t->e->d.base_type = base;
  t->nspc = t_array->nspc;
  ADD_REF(t->nspc);
  SET_FLAG(t, checked);
  t->e->owner = base->e->owner;
  nspc_add_type(base->e->owner, sym, t);
  return t;
}

__attribute__((returns_nonnull))
ANN Type template_parent(const Env env, const Type type) {
  const m_str name = get_type_name(env, type->name, 0);
  return nspc_lookup_type1(type->nspc->parent, insert_symbol(name));
}

ANN m_bool type_ref(Type t) {
  do {
    if(GET_FLAG(t, empty))
      return GW_OK;
    if(GET_FLAG(t, typedef) && t->e->def)
      if(t->e->def->base.ext && t->e->def->base.ext->array && !t->e->def->base.ext->array->exp)
        return GW_OK;
  } while((t = t->e->parent));
  return 0;
}

ANN m_str get_type_name(const Env env, const m_str s, const m_uint index) {
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
