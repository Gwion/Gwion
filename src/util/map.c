#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "map_private.h"
#include "mpool.h"

POOL_HANDLE(Map, 2048)

ANN void map_clear(const Map v) {
  v->ptr = (m_uint*)xrealloc(v->ptr, (VCAP(v) = MAP_CAP) * SZ_INT);
  VLEN(v) = 0;
}

ANEW Map new_map() {
  const Map map  = mp_alloc(Map);
  map->ptr = (m_uint*)xcalloc(MAP_CAP, SZ_INT);
  VCAP(map) = MAP_CAP;
  return map;
}

ANN void map_init(const Map a) {
  a->ptr = (m_uint*)xcalloc(MAP_CAP, SZ_INT);
  VCAP(a) = MAP_CAP;
}

ANN vtype map_get(const Map map, const vtype key) {
  for(vtype i = VLEN(map) + 1; --i;)
    if(VKEY(map, i - 1) == key)
      return VVAL(map, i - 1);
  return 0;
}

ANN void map_set(const Map map, const vtype key, const vtype ptr) {
  for(vtype i = VLEN(map) + 1; --i;) {
    if(VKEY(map, i - 1) == key) {
      VVAL(map, i - 1) = ptr;
      return;
    }
  }
  if((OFFSET + VLEN(map) * 2 + 1) > VCAP(map)) {
    VCAP(map) *= 2;
    map->ptr = (m_uint*)xrealloc(map->ptr, VCAP(map) * SZ_INT);
  }
  VKEY(map, VLEN(map)) = key;
  VVAL(map, VLEN(map)) = ptr;
  ++VLEN(map);
}

ANN void map_remove(const Map map, const vtype key) {
  struct Map_ tmp;
  map_init(&tmp);
  for(vtype i = 0; i < VLEN(map); ++i)
    if(VKEY(map, i) != key)
      map_set(&tmp, key, VVAL(map, i));
  free(map->ptr);
  map->ptr = tmp.ptr;
}

ANN void map_commit(const restrict Map map, const restrict Map commit) {
  for(vtype i = 0; i < VLEN(commit); ++i)
    map_set(map, VKEY(commit, i), VVAL(commit, i));
}

ANN void free_map(const Map map) {
  free(map->ptr);
  mp_free(Map, map);
}

ANN void map_release(const Map map) {
  free(map->ptr);
}
