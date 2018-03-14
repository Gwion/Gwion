#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "map_private.h"
#include "mpool.h"

POOL_HANDLE(Map, 2048)

ANN void map_clear(Map v) {
  v->ptr = realloc(v->ptr, (VCAP(v) = MAP_CAP) * SZ_INT);
  VLEN(v) = 0;
}

Map new_map() {
  Map map  = mp_alloc(Map);
  map->ptr = calloc(MAP_CAP, SZ_INT);
  VCAP(map) = MAP_CAP;
  return map;
}

ANN void map_init(Map a) {
  a->ptr = calloc(MAP_CAP, SZ_INT);
  VCAP(a) = MAP_CAP;
}

ANN const vtype map_get(const Map map, const vtype key) {
  vtype i;
  for(i = VLEN(map) + 1; --i;)
    if(VKEY(map, i - 1) == key)
      return VVAL(map, i - 1);
  return 0;
}

ANN const vtype map_at(const Map map, const vtype index) {
  if(index > VLEN(map))
    return 0;
  return VVAL(map, index);
}

ANN void map_set(const Map map, const vtype key, const vtype ptr) {
  vtype i;
  for(i = VLEN(map) + 1; --i;) {
    if(VKEY(map, i - 1) == key) {
      VVAL(map, i - 1) = ptr;
      return;
    }
  }
  if((OFFSET + VLEN(map) * 2 + 1) > VCAP(map)) {
    VCAP(map) *= 2;
    map->ptr = realloc(map->ptr, VCAP(map) * SZ_INT);
  }
  VKEY(map, VLEN(map)) = key;
  VVAL(map, VLEN(map)) = ptr;
  VLEN(map)++;
}

ANN void map_remove(const Map map, const vtype key) {
  vtype i;
  struct Map_ tmp;
  map_init(&tmp);
  for(i = 0; i < VLEN(map); i++)
    if(VKEY(map, i) != key)
      map_set(&tmp, key, VVAL(map, i));
  free(map->ptr);
  map->ptr = tmp.ptr;
  VLEN(map) = VLEN(&tmp);
  VCAP(map) = VCAP(&tmp);
}

ANN void map_commit(const restrict Map map, const restrict Map commit) {
  vtype i;
  for(i = 0; i < VLEN(commit); i++)
    map_set(map, VKEY(commit, i), VVAL(commit, i));
}

ANN void free_map(Map map) {
  free(map->ptr);
  mp_free(Map, map);
}

ANN void map_release(Map map) {
  free(map->ptr);
}
