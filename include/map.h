#ifndef __MAP
#define __MAP
#include "defs.h"
#include <symbol.h>

typedef m_uint vtype;
#include "map_private.h"
typedef struct Vector_ * Vector;
typedef struct Map_    * Map;

extern       Vector new_vector();
extern       void   vector_init(Vector);
extern const Vector vector_copy(Vector);
extern       void   vector_copy2(const restrict Vector, restrict Vector);
extern const m_int  vector_find(const Vector, const vtype);

__attribute__((nonnull))
static inline void vector_set(const Vector v, const vtype i, const vtype arg) {
  v->ptr[i + OFFSET] = arg;
}
__attribute__((nonnull))
static inline vtype vector_front(const Vector v) {
  return v->ptr[0] ? v->ptr[0 + OFFSET] : 0;
}
__attribute__((nonnull))
static inline vtype vector_at(Vector v, const vtype i) {
  return (i >= v->ptr[0]) ? 0 : v->ptr[i + OFFSET];
}
__attribute__((nonnull))
static inline vtype vector_back(const Vector v) {
  return v->ptr[v->ptr[0] + OFFSET - 1];
}
__attribute__((nonnull))
static inline vtype vector_size(const Vector v) {
  return v->ptr[0];
}

extern       void  vector_add(const Vector, const vtype);
extern       void  vector_rem(const Vector, const vtype);
extern const vtype vector_pop(const Vector);
extern       void  vector_clear(const Vector);
extern       void  free_vector(Vector vector);
extern       void  vector_release(Vector vector);

extern       Map    new_map();
extern       void   map_init();
extern const vtype  map_get(const Map, const vtype);
extern const vtype  map_at(const Map, const vtype);
extern       void   map_set(const Map, const vtype, const vtype);
extern       void   map_remove(const Map, const vtype);
extern       void   map_commit(const restrict Map, restrict const Map);
extern       void   map_clear(const Map);
extern       void   free_map(Map);
extern       void   map_release(Map);
__attribute__((nonnull))
static inline const vtype map_size(const Map map) {
  return map->ptr[0];
}



extern       Scope  new_scope();
extern       void   scope_init(Scope);
extern const Vector scope_get(const Scope);
extern const vtype  scope_lookup0(const Scope, const S_Symbol);
extern const vtype  scope_lookup1(const Scope, const S_Symbol);
extern const vtype  scope_lookup2(const Scope, const S_Symbol);
extern       void   scope_add(const Scope, const S_Symbol, const vtype);
extern       void   scope_commit(const Scope);
extern       void   scope_push(const Scope);
extern       void   scope_pop(const Scope);
extern       void   free_scope(Scope);
extern       void   scope_release(Scope);
#endif
