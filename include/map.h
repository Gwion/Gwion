#ifndef __MAP
#define __MAP
#include "defs.h"
#include <symbol.h>

typedef m_uint vtype;
#include "map_private.h"
typedef struct Vector_ * Vector;
typedef struct Map_    * Map;

extern       Vector new_vector();
ANN extern       void   vector_init(Vector);
ANN extern const Vector vector_copy(Vector);
ANN extern       void   vector_copy2(const __restrict__ Vector, __restrict__ Vector);
ANN extern const m_int  vector_find(const Vector, const vtype);

ANN static inline void vector_set(const Vector v, const vtype i, const vtype arg) {
  v->ptr[i + OFFSET] = arg;
}
ANN static inline vtype vector_front(const Vector v) {
  return v->ptr[0] ? v->ptr[0 + OFFSET] : 0;
}
ANN static inline vtype vector_at(Vector v, const vtype i) {
  return (i >= v->ptr[0]) ? 0 : v->ptr[i + OFFSET];
}
ANN static inline vtype vector_back(const Vector v) {
  return v->ptr[v->ptr[0] + OFFSET - 1];
}
ANN static inline vtype vector_size(const Vector v) {
  return v->ptr[0];
}

extern ANN       void  vector_add(const Vector, const vtype);
extern ANN       void  vector_rem(const Vector, const vtype);
extern ANN const vtype vector_pop(const Vector);
extern ANN       void  vector_clear(const Vector);
extern ANN       void  free_vector(Vector vector);
extern ANN       void  vector_release(Vector vector);

extern Map new_map();
extern     void map_init();
extern ANN const vtype map_get(const Map, const vtype);
extern ANN const vtype map_at(const Map, const vtype);
extern ANN void map_set(const Map, const vtype, const vtype);
extern ANN void map_remove(const Map, const vtype);
extern ANN void map_commit(const __restrict__ Map, __restrict__ const Map);
extern ANN void map_clear(const Map);
extern ANN void free_map(Map);
extern ANN void map_release(Map);
ANN static inline const vtype map_size(const Map map) {
  return map->ptr[0];
}

extern           Scope  new_scope();
extern ANN       void   scope_init(Scope);
extern ANN const Vector scope_get(const Scope);
extern ANN const vtype  scope_lookup0(const Scope, const S_Symbol);
extern ANN const vtype  scope_lookup1(const Scope, const S_Symbol);
extern ANN const vtype  scope_lookup2(const Scope, const S_Symbol);
extern ANN       void   scope_add(const Scope, const S_Symbol, const vtype);
extern ANN       void   scope_commit(const Scope);
extern ANN       void   scope_push(const Scope);
extern ANN       void   scope_pop(const Scope);
extern ANN       void   free_scope(Scope);
extern ANN       void   scope_release(Scope);
#endif
