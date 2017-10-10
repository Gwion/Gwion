#include <stdlib.h>
#include <string.h>
#include "map.h"

#define MAP_CAP 4
#define OFFSET 2

struct Map_ {
  vtype* ptr;
};

void map_clear(Map v) {
  v->ptr = realloc(v->ptr, (v->ptr[1] = MAP_CAP) * sizeof(vtype));
  v->ptr[0] = 0;
}

Map new_map() {
  Map map  = malloc(sizeof(struct Map_));
  map->ptr = calloc(MAP_CAP, sizeof(vtype));
  map->ptr[1] = MAP_CAP;
  return map;
}

void map_init(Map a) {
  a->ptr = calloc(MAP_CAP, sizeof(vtype));
  a->ptr[1] = MAP_CAP;
}

vtype map_get(Map map, vtype key) {
  vtype i;
  for(i = map->ptr[0] + 1; --i;)
    if(map->ptr[OFFSET + (i - 1) * 2] == key)
      return map->ptr[OFFSET + (i - 1) * 2 + 1];
  return 0;
}

vtype map_at(Map map, const vtype index) {
  if(index > map->ptr[0])
    return 0;
  return map->ptr[OFFSET + index * 2 + 1];
}

void map_set(Map map, vtype key, vtype ptr) {
  vtype i;
  for(i = 0; i < map->ptr[0]; i++) {
    if(map->ptr[OFFSET + i * 2] == key) {
      map->ptr[OFFSET + i * 2 + 1] = ptr;
      return;
    }
  }
  if((OFFSET + map->ptr[0] * 2 + 1) > map->ptr[1]) {
    map->ptr[1] *= 2;
    map->ptr = realloc(map->ptr, map->ptr[1] * sizeof(vtype));
  }
  map->ptr[OFFSET + map->ptr[0] * 2] = key;
  map->ptr[OFFSET + map->ptr[0] * 2 + 1] = ptr;
  map->ptr[0]++;
}

void map_remove(Map map, vtype key) {
  vtype i;
  struct Map_ tmp;
  map_init(&tmp);
  for(i = 0; i < map->ptr[0]; i++)
    if(map->ptr[OFFSET + i * 2] != key)
      map_set(&tmp, key, map->ptr[OFFSET + i * 2 + 1]);
  free(map->ptr);
  map->ptr = tmp.ptr;
  map->ptr[0] = tmp.ptr[0];
  map->ptr[1] = tmp.ptr[1];
}

void map_commit(Map map, Map commit) {
  vtype i;
  for(i = 0; i < commit->ptr[0]; i++)
    map_set(map, commit->ptr[OFFSET + i * 2], commit->ptr[OFFSET + i * 2 + 1]);
}

vtype map_size(Map map) {
  return map->ptr[0];
}

void free_map(Map map) {
  free(map->ptr);
  free(map);
}

void map_release(Map map) {
  free(map->ptr);
}
