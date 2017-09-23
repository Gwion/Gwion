#include <stdlib.h>
#include <string.h>
#include "map.h"

#define MAP_CAP 4
#define OFFSET 2

struct Vector_ {
  vtype* ptr;
};

struct Map_ {
  vtype* ptr;
};

Vector new_vector() {
  Vector v = malloc(sizeof(struct Vector_));
  v->ptr = calloc(MAP_CAP, sizeof(vtype));
  v->ptr[1] = MAP_CAP;
  return v;
}

void vector_init(struct Vector_* v) {
  v->ptr = calloc(MAP_CAP, sizeof(vtype));
  v->ptr[1] = MAP_CAP;
}

void vector_release(Vector v) {
  free(v->ptr);
}

void vector_add(Vector v, vtype data) {
  if(!(v->ptr[1] - v->ptr[0] - OFFSET))
    v->ptr = realloc(v->ptr, (v->ptr[1] *= 2) * sizeof(vtype));
  v->ptr[v->ptr[0]++ + OFFSET] = (vtype)data;
}

Vector vector_copy(Vector v) {
  Vector ret = malloc(sizeof(struct Vector_));
  ret->ptr = calloc(v->ptr[1], sizeof(vtype));
  memcpy(ret->ptr, v->ptr, v->ptr[1] * SZ_INT);
  return ret;
}

void vector_copy2(Vector v, Vector ret) {
  ret->ptr = realloc(ret->ptr, v->ptr[1] * sizeof(vtype));
  memcpy(ret->ptr, v->ptr, v->ptr[1] * SZ_INT);
}

m_int vector_find(Vector v, vtype data) {
  vtype i;
  for(i = v->ptr[0] + 1; --i;)
    if(v->ptr[i + OFFSET - 1] == (vtype)data)
      return i - 1;
  return -1;
}

void vector_set(Vector v, const vtype i, vtype arg) {
  v->ptr[i + OFFSET] = arg;
}

void vector_rem(Vector v, const vtype index) {
  vtype i;
  if(index >= v->ptr[0])
    return;
  for(i = index + 1; i < v->ptr[0]; i++)
    v->ptr[i - 1 + OFFSET] = v->ptr[i + OFFSET];
  if(--v->ptr[0] + OFFSET < v->ptr[1] / 2)
    v->ptr = realloc(v->ptr, (v->ptr[1] /= 2) * sizeof(vtype));
}

vtype vector_pop(Vector v) {
  vtype ret;
  if(!v->ptr[0])
    return 0;
  ret = v->ptr[v->ptr[0] + OFFSET - 1];
  vector_rem(v, v->ptr[0] - 1);
  return ret;
}

vtype vector_front(Vector v) {
  return v->ptr[0] ? v->ptr[0 + OFFSET] : 0;
}

vtype vector_at(Vector v, const vtype i) {
  return (i >= v->ptr[0]) ? 0 : v->ptr[i + OFFSET];
}

vtype vector_back(Vector v) {
  return v->ptr[v->ptr[0] + OFFSET - 1];
}

vtype vector_size(Vector v) {
  return v->ptr[0];
}

static inline void _clear(Vector v) {
  v->ptr = realloc(v->ptr, (v->ptr[1] = MAP_CAP) * sizeof(vtype));
  v->ptr[0] = 0;
}

void vector_clear(Vector a) {
  _clear(a);
}
void map_clear(Map    a) {
  _clear((Vector)a);
}

void free_vector(Vector v) {
  free(v->ptr);
  free(v);
  v = NULL;
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

struct Scope_ {
  struct Map_    commit_map;
  struct Vector_ vector;
};

vtype scope_lookup0(Scope scope, S_Symbol xid) {
  Map map = (Map)vector_back(&scope->vector);
  vtype ret = map_get(map, (vtype)xid);
  if(!ret && vector_back(&scope->vector) == vector_front(&scope->vector))
    ret = map_get(&scope->commit_map, (vtype)xid);
  return ret;
}

vtype scope_lookup1(Scope scope, S_Symbol xid) {
  m_uint i;
  vtype ret;
  for(i = vector_size(&scope->vector) + 1; --i;) {
    Map map = (Map)vector_at(&scope->vector, i - 1);
    if((ret = map_get(map, (vtype)xid)))
      break;
   }
  if(!ret)
    ret = map_get(&scope->commit_map, (vtype)xid);
  return ret;
}

vtype scope_lookup2(Scope scope, S_Symbol xid) {
  Map map = (Map)vector_front(&scope->vector);
  vtype ret = map_get(map, (vtype)xid);
  if(!ret)
    ret = map_get(&scope->commit_map, (vtype)xid);
  return ret;
}

vtype scope_lookup(Scope scope, S_Symbol xid, m_bool climb) {
  vtype ret = 0;
  if(climb == 0)
    ret = scope_lookup0(scope, xid); 
  else if(climb > 0)
    ret = scope_lookup1(scope, xid); 
  else
    ret = scope_lookup2(scope, xid);
  return ret;
}

void scope_add(Scope scope, S_Symbol xid, vtype value) {
  if(vector_front(&scope->vector) != vector_back(&scope->vector))
    map_set((Map)vector_back(&scope->vector), (vtype)xid, (vtype)value);
  else
    map_set(&scope->commit_map, (vtype)xid, (vtype)value);
}

void scope_commit(Scope scope) {
  map_commit((Map)vector_front(&scope->vector), &scope->commit_map);
  map_clear(&scope->commit_map);
}

void scope_push(Scope scope) {
  vector_add(&scope->vector, (vtype)new_map());
}

void scope_pop(Scope scope) {
  free_map((Map)vector_pop(&scope->vector));
}

void scope_init(Scope a) {
  vector_init((Vector)&a->commit_map);
  vector_init(&a->vector);
  scope_push(a);
}

void scope_release(Scope a) {
  free_map((Map)vector_front(&a->vector));
  vector_release(&a->vector);
  vector_release((Vector)&a->commit_map);
}

Vector scope_get(Scope s) {
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

