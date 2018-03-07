#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "map_private.h"

#define MAP_CAP 4
#define OFFSET 2

ANN const vtype scope_lookup0(const Scope scope, const S_Symbol xid) {
  Map map = (Map)vector_back(&scope->vector);
  vtype ret = map_get(map, (vtype)xid);
  if(!ret && vector_back(&scope->vector) == vector_front(&scope->vector))
    ret = map_get(&scope->commit_map, (vtype)xid);
  return ret;
}

ANN const vtype scope_lookup1(const Scope scope, const S_Symbol xid) {
  m_uint i;
  vtype ret;
  for(i = vector_size(&scope->vector) + 1; --i;) {
    Map map = (Map)vector_at(&scope->vector, i - 1);
    if((ret = map_get(map, (vtype)xid)))
      return ret;
  }
  return map_get(&scope->commit_map, (vtype)xid);
}

ANN const vtype scope_lookup2(const Scope scope, const S_Symbol xid) {
  Map map = (Map)vector_front(&scope->vector);
  vtype ret = map_get(map, (vtype)xid);
  if(!ret)
    ret = map_get(&scope->commit_map, (vtype)xid);
  return ret;
}

ANN void scope_add(const Scope scope, const S_Symbol xid, const vtype value) {
  if(vector_front(&scope->vector) != vector_back(&scope->vector))
    map_set((Map)vector_back(&scope->vector), (vtype)xid, (vtype)value);
  else
    map_set(&scope->commit_map, (vtype)xid, (vtype)value);
}

ANN void scope_commit(const Scope scope) {
  map_commit((Map)vector_front(&scope->vector), &scope->commit_map);
  map_clear(&scope->commit_map);
}

ANN void scope_push(const Scope scope) {
  vector_add(&scope->vector, (vtype)new_map());
}

ANN void scope_pop(const Scope scope) {
  free_map((Map)vector_pop(&scope->vector));
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

ANN const Vector scope_get(const Scope s) {
  vtype i, j;
  Vector ret = new_vector();
  for(j = 0; j < vector_size(&s->vector); j++) {
    Map map = (Map)vector_at(&s->vector, j);
    for(i = 0; i < map->ptr[0]; i++)
      vector_add(ret, map->ptr[OFFSET + i * 2 + 1]);
  }
  for(i = 0; i < s->commit_map.ptr[0]; i++)
    vector_add(ret, (vtype)s->commit_map.ptr[OFFSET + i * 2 + 1]);
  return ret;
}

