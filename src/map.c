#include <stdlib.h>
#include "map.h"

struct Vector_ {
  vtype ptr[MAP_MAX];
  vtype len;
};

struct Map_ {
  vtype key[MAP_MAX];
  vtype ptr[MAP_MAX];
  vtype len;
};

Vector new_Vector()
{
  Vector v = malloc(sizeof(struct Vector_));
  v->len = 0;
  return v;
}

Vector new_Vector_fixed(const vtype len)
{
  Vector v = malloc(sizeof(struct Vector_));
  v->len   = len;
  return v;
}

void vector_append(Vector v, vtype data)
{
  v->len++;
  v->ptr[v->len - 1] = (vtype)data;
}

Vector vector_copy(Vector v)
{
  Vector ret = new_Vector_fixed(v->len);
  vtype i;
  for(i = 0; i < v->len; i++)
    ret->ptr[i] = v->ptr[i];
  return ret;
}

long int vector_find(Vector v, vtype data)
{
  vtype i;
  for(i = 0; i < v->len; i++)
    if(v->ptr[i] == (vtype)data)
      return i;
  return -1;
}

void vector_set(Vector v, const vtype i, vtype arg)
{
  if(i >= v->len)
    return;
  v->ptr[i] = (vtype)arg;
}

void vector_remove(Vector v, const vtype index)
{
  Vector tmp;
  vtype i, j = 0;
  if(!v->len || index >= v->len)
    return;
  if(v->len == 1 && !index) {
    v->len--;
    return;
  }

  tmp = new_Vector_fixed(v->len - 1);
  for(i = 0; i < v->len; i++) {
    if(i != index) {
      tmp->ptr[j] = v->ptr[i];
      j++;
    }
  }
  *v = *tmp;
  free(tmp);
}

void vector_pop(Vector v)
{
  vector_remove(v, v->len - 1);
}

vtype vector_front(Vector v)
{
  if(!v->len)
    return 0;
  return (vtype)v->ptr[0];
}

vtype vector_at(Vector v, const vtype i)
{
  if(i >= v->len)
    return 0;
  return (vtype)v->ptr[i];
}

vtype vector_addr(Vector v, const vtype i)
{
  if(i >= v->len)
    return 0;
  return (vtype)&v->ptr[i];
}

vtype vector_back(Vector v)
{
  return (vtype)v->ptr[v->len - 1];
}

vtype vector_size(Vector v)
{
  return v->len;
}

void vector_clear(Vector v)
{
  v->len = 0;
}

void free_Vector(Vector v)
{
  free(v);
  v = NULL;
}

Map new_Map()
{
  Map map  = malloc(sizeof(struct Map_));
//  map->key = calloc(1, sizeof(vtype));
//  map->ptr = calloc(1, sizeof(vtype));
  map->len = 0;
  return map;
}

vtype map_get(Map map, vtype key)
{
  vtype i;
  for(i = 0; i < map->len; i++)
    if(map->key[i] == key)
      return (vtype)map->ptr[i];
  return 0;
}

vtype map_at(Map map, const vtype index)
{
  if(index > map->len)
    return 0;
  return map->ptr[index];
}

vtype map_key(Map map, const vtype index)
{
  if(index > map->len)
    return 0;
  return map->key[index];
}

void map_set(Map map, vtype key, vtype ptr)
{
  vtype i;
  for(i = 0; i < map->len; i++) {
    if(map->key[i] == key) {
      map->ptr[i] = ptr;
      return;
    }
  }
  map->len++;
  /*
  void* tmp = map->key;
  tmp = realloc(map->key, map->len * sizeof(vtype));
  if(!tmp)
  	exit(2);
  	map->key = tmp;
  tmp = realloc(map->ptr, map->len * sizeof(vtype));
  if(tmp)
  	map->ptr = tmp;
  */
//if(map->len > 1) {


//  map->key = realloc(map->key, map->len * sizeof(vtype));
//  map->ptr = realloc(map->ptr, map->len * sizeof(vtype));


//}
  map->key[map->len - 1] = key;
  map->ptr[map->len - 1] = ptr;
}

void map_remove(Map map, vtype key)
{
  vtype i;
  Map tmp = new_Map();
  for(i = 0; i < map->len; i++)
    if(map->key[i] != key)
      map_set(tmp, key, map->ptr[i]);
  *map = *tmp;
  free(tmp);
}

void map_commit(Map map, Map commit)
{
  vtype i;
  for(i = 0; i < commit->len; i++)
    map_set(map, commit->key[i], commit->ptr[i]);
}

void map_rollback(Map map, void (*_free)(vtype arg))
{
//exit(2);
//  vtype i;
//    for(i = 0; i < map->len; i++)
//        if(_free)
//            free(map->ptr[i]);
//    vector_clear(map->key);
//    vector_clear(map->ptr);
//  map->len = 0;
}

vtype map_size(Map map)
{
  return map->len;
}

void free_Map(Map map)
{
//  free(map->key);
//  free(map->ptr);
  free(map);
}

struct Scope_ {
  Map    commit_map;
  Vector vector;
};

vtype scope_lookup(Scope scope, S_Symbol xid, int climb)
{
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

void scope_add(Scope scope, S_Symbol xid, vtype value)
{
  Map map;
  if(vector_front(scope->vector) != vector_back(scope->vector))
    map = (Map)vector_back(scope->vector);
  else
    map = scope->commit_map;
  map_set(map, (vtype)xid, (vtype)value);
  /*  add_ref(((struct Value_*)value)->obj);*/
}

void scope_rem(Scope scope, S_Symbol xid)
{
  Map map;
  if(vector_front(scope->vector) != vector_back(scope->vector))
    map = (Map)vector_back(scope->vector);
  else
    map = scope->commit_map;
  map_remove(map, (vtype)xid);
  /*  rem_ref(((struct Value_*)value)->obj);*/
}

void scope_commit(Scope scope)
{
  Map map = (Map)vector_front(scope->vector);
  map_commit(map, scope->commit_map);
//  map_clean(scope->commit_map);
  free_Map(scope->commit_map);
  scope->commit_map = new_Map();
}

void scope_rollback(Scope scope, void (*_free)(vtype arg))
{
  map_rollback(scope->commit_map, _free);
}

void scope_push(Scope scope)
{
  vector_append(scope->vector, (vtype)new_Map());
}

void scope_pop(Scope scope)
{
  free_Map((Map)vector_back(scope->vector));
  vector_pop(scope->vector);
}

Scope new_Scope()
{
  Scope a = malloc(sizeof(struct Scope_));
  a->commit_map = new_Map();
  a->vector = new_Vector();
  scope_push(a);
  return a;
}

void free_Scope(Scope a)
{
  free_Map((Map)vector_front(a->vector));
  free_Vector(a->vector);
  free_Map(a->commit_map);
  free(a);
}

Vector scope_get(Scope s)
{
  vtype i, j;
  Vector ret = new_Vector();
  for(j = 0; j < vector_size(s->vector); j++) {
    Map map = (Map)vector_at(s->vector, j);
    for(i = 0; i < map->len; i++)
      vector_append(ret, map->ptr[i]);
  }
  for(i = 0; i < s->commit_map->len; i++)
    vector_append(ret, (vtype)s->commit_map->ptr[i]);
  return ret;
}

