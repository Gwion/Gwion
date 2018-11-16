#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "oo.h"
#include "env.h"
#include "type.h"

ANN m_bool prim_ref(const Type_Decl* td, const Type t) {
  if(isa(t, t_object) < 0)
    ERR_B(td->xid->pos,
          "cannot declare/instantiate references (@) of primitive type '%s'...\n"
          "\t...(primitive types: 'int', 'float', 'time', 'dur',\n"
          "\t... complex, polar, Vec3, Vec4)",
          t->name)
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
    str = (m_str)xrealloc(str, src *= 2);
}

ANEW ANN static m_str td2str(const Env env, const Type_Decl* td) {
  m_uint depth = 1 + (td->array ? td->array->depth : 0);
  const size_t len = id_list_len(td->xid);
  size_t l = len + depth *2;
  const m_uint m = l;
  const m_str str = (m_str)xmalloc(m);
  m_str s = str;
  type_path(s, td->xid);
  s += len;
  while(--depth) {
    strcpy(s, "[]");
    s += 2;
  }
  Type_List tl = td->types;
  if(tl) {
    ++l;
    strcheck(s, m, l);
    strcpy(s, "<");
    ++s;
    while(tl) {
      m_str name = td2str(env, tl->td);
      const size_t nlen = strlen(name);
      l += nlen;
      strcheck(s, m, l);
      strcpy(s, name);
      s += nlen;
      free(name);
      tl = tl->next;
      if(tl) {
        strcpy(s, ",");
        ++s;
      }
    }
    strcpy(s, ">");
  }
  return str;
}

ANEW ANN m_str tl2str(const Env env, Type_List tl) {
  m_uint l = 0;
  const m_uint m = 32;
  const m_str str = (m_str)xmalloc(m);
  m_str s = str;
  do {
    const m_str name = td2str(env, tl->td);
    l += strlen(name);
    strcheck(s, m, l);
    strcpy(s, name);
    s += l;
    free(name);
    if(tl->next) {
      strcpy(s, ",");
      ++l;
      ++s;
    }
  } while((tl = tl->next));
  return str;
}

ANN static inline void* type_unknown(const ID_List id, const m_str orig) {
  char path[id_list_len(id)];
  type_path(path, id);
  err_msg(id->pos, "'%s' unknown type in %s", path, orig);
  return NULL;
}

ANN Type known_type(const Env env, const Type_Decl* td, const m_str orig) {
  return type_decl_resolve(env, td) ?: type_unknown(td->xid, orig);
}
