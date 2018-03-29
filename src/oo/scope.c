#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "map_private.h"

ANN vtype scope_lookup0(const Scope scope, const Symbol xid) {
  const Map map = (Map)vector_back(&scope->vector);
  const vtype ret = map_get(map, (vtype)xid);
  if(!ret && vector_back(&scope->vector) == vector_front(&scope->vector))
    return map_get(&scope->commit_map, (vtype)xid);
  return ret;
}

ANN vtype scope_lookup1(const Scope scope, const Symbol xid) {
  for(m_uint i = vector_size(&scope->vector) + 1; --i;) {
    const Map map = (Map)vector_at(&scope->vector, i - 1);
    const vtype ret = map_get(map, (vtype)xid);
    if(ret)
      return ret;
  }
  return map_get(&scope->commit_map, (vtype)xid);
}

ANN vtype scope_lookup2(const Scope scope, const Symbol xid) {
  const Map map = (Map)vector_front(&scope->vector);
  const vtype ret = map_get(map, (vtype)xid);
  if(ret)
    return ret;
  return map_get(&scope->commit_map, (vtype)xid);
}

ANN void scope_add(const Scope scope, const Symbol xid, const vtype value) {
  if(vector_front(&scope->vector) != vector_back(&scope->vector))
    map_set((Map)vector_back(&scope->vector), (vtype)xid, (vtype)value);
  else
    map_set(&scope->commit_map, (vtype)xid, (vtype)value);
}

ANN void scope_commit(const Scope scope) {
  map_commit((Map)vector_front(&scope->vector), &scope->commit_map);
  map_clear(&scope->commit_map);
}

ANN void scope_init(Scope a) {
  vector_init((Vector)&a->commit_map);
  vector_init(&a->vector);
  scope_push(a);
}

ANN void scope_release(Scope a) {
  free_map((Map)vector_front(&a->vector));
  vector_release(&a->vector);
  vector_release((Vector)&a->commit_map);
}

ANN Vector scope_get(const Scope s) {
  const Vector ret = new_vector();
  for(m_uint j = 0; j < vector_size(&s->vector); j++) {
    const Map map = (Map)vector_at(&s->vector, j);
    for(m_uint i = 0; i < VLEN(map); i++)
      vector_add(ret, VVAL(map, i));
  }
  for(m_uint i = 0; i < VLEN(&s->commit_map); i++)
    vector_add(ret, VVAL(&s->commit_map, i));
  return ret;
}

