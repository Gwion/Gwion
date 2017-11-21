#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "map_private.h"

#define MAP_CAP 4
#define OFFSET 2

#define LEN(v)    (v)->ptr[0]
#define CAP(v)    (v)->ptr[1]
#define KEY(v, i) (v)->ptr[OFFSET + (i) * 2]
#define VAL(v, i) (v)->ptr[OFFSET + (i) * 2 + 1]

void map_clear(Map v) {
  v->ptr = realloc(v->ptr, (CAP(v) = MAP_CAP) * SZ_INT);
  LEN(v) = 0;
}

Map new_map() {
  Map map  = malloc(sizeof(struct Map_));
  map->ptr = calloc(MAP_CAP, SZ_INT);
  CAP(map) = MAP_CAP;
  return map;
}

void map_init(Map a) {
  a->ptr = calloc(MAP_CAP, SZ_INT);
  CAP(a) = MAP_CAP;
}

vtype map_get(const Map map, const vtype key) {
  vtype i;
  for(i = LEN(map) + 1; --i;)
    if(KEY(map, i - 1) == key)
      return VAL(map, i - 1);
  return 0;
}

vtype map_at(const Map map, const vtype index) {
  if(index > LEN(map))
    return 0;
  return VAL(map, index);
}

void map_set(Map map, const vtype key, const vtype ptr) {
  vtype i;
  for(i = 0; i < LEN(map); i++) {
    if(KEY(map, i) == key) {
      VAL(map, i) = ptr;
      return;
    }
  }
  if((OFFSET + LEN(map) * 2 + 1) > CAP(map)) {
    CAP(map) *= 2;
    map->ptr = realloc(map->ptr, CAP(map) * SZ_INT);
  }
  KEY(map, LEN(map)) = key;
  VAL(map, LEN(map)) = ptr;
  LEN(map)++;
} 

void map_remove(Map map, const vtype key) {
  vtype i;
  struct Map_ tmp;
  map_init(&tmp);
  for(i = 0; i < LEN(map); i++)
    if(KEY(map, i) != key)
      map_set(&tmp, key, VAL(map, i));
  free(map->ptr);
  map->ptr = tmp.ptr;
  LEN(map) = LEN(&tmp);
  CAP(map) = CAP(&tmp);
} 

void map_commit(Map map, const Map commit) {
  vtype i;
  for(i = 0; i < LEN(commit); i++)
    map_set(map, KEY(commit, i), VAL(commit, i));
} 

vtype map_size(const Map map) {
  return LEN(map);
} 

void free_map(Map map) {
  free(map->ptr);
  free(map);
}

void map_release(Map map) {
  free(map->ptr);
}
