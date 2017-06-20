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
  vtype len;
  vtype cap;
};

Vector new_vector() {
  Vector v = malloc(sizeof(struct Vector_));
  v->ptr = calloc(MAP_CAP, sizeof(vtype));
  v->ptr[0] = 0;
  v->ptr[1] = MAP_CAP;
  return v;
}

void vector_add(Vector v, vtype data) {
  if(!(v->ptr[1] - v->ptr[0] - OFFSET))
    v->ptr = realloc(v->ptr, (v->ptr[1]*=2) * sizeof(vtype));
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

long int vector_find(Vector v, vtype data) {
  vtype i;
  for(i = 0; i < v->ptr[0]; i++)
    if(v->ptr[i + OFFSET] == (vtype)data)
      return i;
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
    v->ptr[i-1+OFFSET] = v->ptr[i+OFFSET];
  if(--v->ptr[0] + OFFSET < v->ptr[1]/2)
    v->ptr = realloc(v->ptr, (v->ptr[1]/=2) * sizeof(vtype));
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
  return (i >= v->ptr[0]) ? 0 :v->ptr[i + OFFSET];
}

vtype vector_back(Vector v) {
  return v->ptr[v->ptr[0] + OFFSET - 1];
}

vtype vector_size(Vector v) {
  return v->ptr[0];
}

void vector_clear(Vector v) {
  v->ptr = realloc(v->ptr, (v->ptr[1] = MAP_CAP) * sizeof(vtype));
  v->ptr[0]= 0;
}

void free_vector(Vector v) {
  free(v->ptr);
  free(v);
  v = NULL;
}

Map new_map() {
  Map map  = malloc(sizeof(struct Map_));
  map->ptr = calloc(MAP_CAP, sizeof(vtype));
  map->len = 0;
  map->cap = MAP_CAP;
  return map;
}

vtype map_get(Map map, vtype key) {
  vtype i;
  for(i = 0; i < map->len; i++)
    if(map->ptr[i*2] == key)
      return map->ptr[i*2+1];
  return 0;
}

vtype map_at(Map map, const vtype index) {
  if(index > map->len)
    return 0;
  return map->ptr[index*2+1];
}

void map_set(Map map, vtype key, vtype ptr) {
  vtype i;
  for(i = 0; i < map->len; i++) {
    if(map->ptr[i*2] == key) {
      map->ptr[i*2+1] = ptr;
      return;
    }
  }
  if((map->len*2 + 1) > map->cap) {
    map->cap = map->cap * 2;
    map->ptr = realloc(map->ptr, map->cap * sizeof(vtype));
  }
  map->ptr[map->len*2] = key;
  map->ptr[map->len*2+1] = ptr;
  map->len++;
}

void map_remove(Map map, vtype key) {
  vtype i;
  Map tmp = new_map();
  for(i = 0; i < map->len; i++)
    if(map->ptr[i*2] != key)
      map_set(tmp, key, map->ptr[i*2+1]);
  free(map->ptr);
  map->ptr = tmp->ptr;
  map->len = tmp->len;
  map->cap = tmp->cap;
  free(tmp);
}

void map_commit(Map map, Map commit) {
  vtype i;
  for(i = 0; i < commit->len; i++)
    map_set(map, commit->ptr[i*2], commit->ptr[i*2+1]);
}

vtype map_size(Map map) {
//  return map->ptr[0];
  return map->len;
}

void free_map(Map map) {
  free(map->ptr);
  free(map);
}

struct Scope_ {
  Map    commit_map;
  Vector vector;
};

vtype scope_lookup(Scope scope, S_Symbol xid, m_bool climb) {
  unsigned int i;
  vtype ret = 0;
  Map map;
  if(climb == 0) {
    map = (Map)vector_back(scope->vector);
    ret = map_get(map, (vtype)xid);
    if(!ret && vector_back(scope->vector) == vector_front(scope->vector))
      ret = map_get(scope->commit_map, (vtype)xid);
  } else if(climb > 0) {
    for(i = vector_size(scope->vector); i > 0; i--) {
      map = (Map)vector_at(scope->vector, i - 1);
      if((ret = map_get(map, (vtype)xid)))
        break;
    }
    if(!ret)
      ret = map_get(scope->commit_map, (vtype)xid);
  } else {
    map = (Map)vector_front(scope->vector);
    ret = map_get(map, (vtype)xid);
    if(!ret)
      ret = map_get(scope->commit_map, (vtype)xid);
  }
  return ret;
}

void scope_add(Scope scope, S_Symbol xid, vtype value) {
  Map map;
  if(vector_front(scope->vector) != vector_back(scope->vector))
    map = (Map)vector_back(scope->vector);
  else
    map = scope->commit_map;
  map_set(map, (vtype)xid, (vtype)value);
}

void scope_rem(Scope scope, S_Symbol xid) {
  Map map;
// to know how to reach those
//  if(vector_front(scope->vector) != vector_back(scope->vector))
//    map = (Map)vector_back(scope->vector);
//  else
  map = scope->commit_map;
  map_remove(map, (vtype)xid);
}

void scope_commit(Scope scope) {
  Map map = (Map)vector_front(scope->vector);
  map_commit(map, scope->commit_map);
  free_map(scope->commit_map);
  scope->commit_map = new_map();
}

void scope_push(Scope scope) {
  vector_add(scope->vector, (vtype)new_map());
}

void scope_pop(Scope scope) {
  free_map((Map)vector_back(scope->vector));
  vector_pop(scope->vector);
}

Scope new_scope() {
  Scope a = malloc(sizeof(struct Scope_));
  a->commit_map = new_map();
  a->vector = new_vector();
  scope_push(a);
  return a;
}

void free_scope(Scope a) {
  free_map((Map)vector_front(a->vector));
  free_vector(a->vector);
  free_map(a->commit_map);
  free(a);
}

Vector scope_get(Scope s) {
  vtype i, j;
  Vector ret = new_vector();
  for(j = 0; j < vector_size(s->vector); j++) {
    Map map = (Map)vector_at(s->vector, j);
    for(i = 0; i < map->len; i++)
      vector_add(ret, map->ptr[i*2+1]);
  }
  for(i = 0; i < s->commit_map->len; i++)
    vector_add(ret, (vtype)s->commit_map->ptr[i*2+1]);
  return ret;
}

