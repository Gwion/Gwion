#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"

ANN const m_bool prim_ref(const Type_Decl* td, const Type t) {
  if(isa(t, t_object) < 0)
    CHECK_BB(err_msg(SCAN2_, td->pos,
          "cannot declare/instantiate references (@) of primitive type '%s'...\n"
          "\t...(primitive types: 'int', 'float', 'time', 'dur',\n"
          "\t... complex, polar, Vec3, Vec4)",
          t->name))
  return 1;
}

ANN Type type_decl_resolve(const Env env, const Type_Decl* td) {
  Type t = find_type(env, td->xid);
  CHECK_OO(t)
  CHECK_OO((t = scan_type(env, t, td)))
  if(td->array)
    CHECK_OO((t = array_type(t, td->array->depth)))
  return t;
}

ANN static inline void strcheck(m_str str, m_uint src, const m_uint tgt) {
  while(tgt >= src)
    str = xrealloc(str, src *= 2);
}

ANEW ANN static m_str td2str(const Env env, const Type_Decl* td) {
  m_uint depth = td->array ? td->array->depth : 0;
  m_uint l = id_list_len(td->xid) + depth *2;
  m_uint m = l;
  m_str s = xmalloc(m);
  type_path(s, td->xid);
  while(depth--)
    strcat(s, "[]");
  Type_List tl = td->types;
  if(tl) {
    l += 2;
    strcheck(s, m, l);
    strcat(s, "<");
    while(tl) {
      m_str name = td2str(env, tl->list);
      l += strlen(name);
      strcheck(s, m, l);
      strcat(s, name);
      free(name);
      tl = tl->next;
      if(tl)
        strcat(s, ",");
    }
    strcat(s, ">");
  }
  return s;
}

ANEW ANN m_str tl2str(const Env env, Type_List tl) {
  m_uint l = 0;
  m_uint m = 32;
  m_str s = xmalloc(m);
  memset(s, 0, 32);
  while(tl) {
    m_str name = td2str(env, tl->list);
    l += strlen(name) + 1;
    strcheck(s, m, l);
    strcat(s, name);
    free(name);
    tl = tl->next;
    if(tl)
      strcat(s, ",");
  }
  return s;
}
