#include <stdlib.h>
#include "map.h"

struct Vector_
{
  long unsigned int *ptr;
	long unsigned int len;
};

struct Map_
{
	void **key;
	void **ptr;
	long unsigned int len;
};

Vector new_Vector()
{
  Vector v = calloc(1, sizeof(struct Vector_));
	v->ptr = calloc(1, sizeof(long unsigned int));
	v->len = 0;
  return v;
}
Vector new_Vector_fixed(const long unsigned int len)
{
  Vector v = calloc(1, sizeof(struct Vector_));
  v->ptr   = calloc(len,  sizeof(long unsigned int));
  v->len   = len;
  long unsigned int i;
  for(i = 0; i < len; i++)
  {
    v->ptr[i] = (long unsigned int )calloc(1, sizeof(long unsigned int));
    *(long unsigned int*)v->ptr[i] = 0;
  }
  return v;
}

void vector_append(Vector v, void* data)
{
  v->len++;
  v->ptr = realloc(v->ptr, v->len * sizeof(void*));
  v->ptr[v->len - 1] = (long unsigned int)data;
}

Vector vector_copy(Vector v)
{
  Vector ret = new_Vector_fixed(v->len);
  long unsigned int i;
  for(i = 0; i < v->len; i++)
    ret->ptr[i] = v->ptr[i];
  return ret;
}
long int vector_find(Vector v, void* data)
{
  long unsigned int i;
  for(i = 0; i < v->len; i++)
    if(v->ptr[i] == (long unsigned int)data)
      return i;
  return -1;
}

void vector_set(Vector v, const long unsigned int i, void* arg)
{
  if(i < 0 || i >= v->len)
    return;
  v->ptr[i] = (long unsigned int)arg;
}
void vector_remove(Vector v, const long unsigned int index)
{
  Vector tmp;
  long unsigned int i, j = 0;
  if(!v->len || index < 0 || index >= v->len)
    return;
  if(v->len == 1 && !index)
  {
/*     tmp->ptr[0] = 1;*/
     v->len--;
     return;
  }

  tmp = new_Vector_fixed(v->len -1);
  for(i = 0; i < v->len; i++)
  {
    if(i != index)
    {
      tmp->ptr[j] = v->ptr[i];
      j++;
    }
  }
  *v = *tmp;
  free(tmp);
}

void vector_pop(Vector v)
{
  vector_remove(v, v->len-1);
}

void* vector_front(Vector v)
{
  if(!v->len)
    return NULL;
  return (void*)v->ptr[0];
}

void* vector_at(Vector v, const long unsigned int i)
{
  if(i < 0 || i >= v->len)
    return NULL;
  return (void*)v->ptr[i];
}

void* vector_addr(Vector v, const long unsigned int i)
{
  if(i < 0 || i >= v->len)
    return NULL;
  return &v->ptr[i];
}

void* vector_back(Vector v)
{
  return (void*)v->ptr[v->len-1];
}
/*
void* vector_push_front(Vector v, long unsigned int data)
{
  Vector tmp;
  long unsigned int i;
  tmp = new_Vector_fixed(v->len + 1);
  tmp->ptr[0] = data;
  for(i = 0; i < v->len; i++)
  {
      tmp->ptr[i + 1] = v->ptr[i];
  }
  *v = *tmp;
  free(tmp);
}

void* vector_insert(Vector v, const long unsigned int index, long unsigned int data)
{
  Vector tmp;
  long unsigned int i, j;
  tmp = new_Vector_fixed(v->len + 1);
  tmp->ptr[0] = data;
  for(i = 0; i < v->len; i++)
  {
    if(i == index)
      tmp->ptr[i] = data;
    else
    {
      tmp->ptr[i] = v->ptr[j];
      j++;
    }
  }
  *v = *tmp;
  free(tmp);

}
*/
long unsigned int vector_size(Vector v)
{
  return v->len;
}

void vector_clear(Vector v)
{
  v->ptr = realloc(v->ptr, sizeof(long unsigned int));
  v->len = 0;
}
void free_Vector(Vector v)
{
	free(v->ptr);
  free(v);
  v = NULL;
}

Map new_Map()
{
	Map map  = calloc(1, sizeof(struct Map_));
	map->key = calloc(1, sizeof(void*));
	map->ptr = calloc(1, sizeof(void*));
	map->len = 0;
  return map;
}

void * map_get(Map map, void* key)
{
	long unsigned int i;
	for(i = 0; i < map->len; i++)
		if(map->key[i] == key)
			return map->ptr[i];
	return NULL;
}

void * map_at(Map map, const long unsigned int index)
{
	if(index > map->len)
		return NULL;
	return map->ptr[index];
}

void * map_key(Map map, const long unsigned int index)
{
	if(index > map->len)
		return NULL;
	return map->key[index];
}

void map_set(Map map, void* key, void* ptr)
{

	long unsigned int i;
	for(i = 0; i < map->len; i++)
  {
		if(map->key[i] == key)
    {
			map->ptr[i] = ptr;
			return;
		}
	}
	map->len++;
	map->key = realloc(map->key, map->len * sizeof(void*));
	map->ptr = realloc(map->ptr, map->len * sizeof(void*));
	map->key[map->len - 1] = key;
	map->ptr[map->len - 1] = ptr;
}

void map_remove(Map map, void* key)
{
	long unsigned int i;
	Map tmp = new_Map();
	for(i = 0; i < map->len; i++)
		if(map->key[i] != key)
			map_set(tmp, key, map->ptr[i]);
	*map = *tmp;
	free(tmp);
}

void map_commit(Map map, Map commit)
{
  long unsigned int i;
  for(i = 0; i < commit->len; i++)
    map_set(map, commit->key[i], commit->ptr[i]);
}

void map_rollback(Map map, void (*_free)(void* arg))
{
  long unsigned int i;
  for(i = 0; i < map->len; i++)
    if(_free)
      _free(map->ptr[i]);
  free_Map(map);
  map = new_Map();
}

long unsigned int map_size(Map map)
{
	return map->len;
}

void free_Map(Map map)
{
	free(map->key);
	free(map->ptr);
	free(map);
}

struct Scope_
{
  Map    commit_map;
  Vector vector;
};

 void* scope_lookup(Scope scope, S_Symbol xid, int climb)
{
  unsigned int i;
  void* ret = NULL;
  Map map;
  if(climb == 0)
  {
    map = vector_back(scope->vector);
    ret = map_get(map, xid);
    if(!ret && vector_back(scope->vector) == vector_front(scope->vector))
      ret = map_get(scope->commit_map, xid);
  }
  else if(climb > 0)
  {
    for(i = vector_size(scope->vector); i > 0; i--)
    {
      map = vector_at(scope->vector, i-1);
      if((ret = map_get(map, xid)))
        break;
    }
    if(!ret)
      ret = map_get(scope->commit_map, xid);
  }
  else
  {
    map = vector_front(scope->vector);
    ret = map_get(map, xid);
    if(!ret)
      ret = map_get(scope->commit_map, xid);
  }
  return ret;
}

 void scope_add(Scope scope, S_Symbol xid, void* value)
{
  Map map;
  if(vector_front(scope->vector) != vector_back(scope->vector))
    map = vector_back(scope->vector);
  else
    map = scope->commit_map;
  map_set(map, xid, value);
/*  add_ref(((struct Value_*)value)->obj);*/
}

 void scope_commit(Scope scope)
{
  Map map = vector_front(scope->vector);
  map_commit(map, scope->commit_map);
  free_Map(scope->commit_map);
  scope->commit_map = new_Map();
}

 void scope_rollback(Scope scope, void (*_free)(void* arg))
{
  map_rollback(scope->commit_map, _free);
}

 void scope_push(Scope scope)
{
  vector_append(scope->vector, new_Map());
}

Map scope_pop(Scope scope)
{
  Map map = vector_back(scope->vector);
  vector_pop(scope->vector);
  return map;
}

Scope new_Scope()
{
  Scope a = calloc(1, sizeof(struct Scope_));
  a->commit_map = new_Map();
  a->vector = new_Vector();
  scope_push(a);
  return a;
}

 void free_Scope(Scope a)
{
  free_Vector(a->vector);
  free_Map(a->commit_map);
  free(a);
}

// for doc
Vector scope_get(Scope s)
{
	long unsigned int i, j;
	Vector ret = new_Vector();
	for(j = 0; j < vector_size(s->vector); j++)
  {
    Map map = vector_at(s->vector, j);
    for(i = 0; i < map->len; i++)
		vector_append(ret, map->ptr[i]);
  }
	for(i = 0; i < s->commit_map->len; i++)
		vector_append(ret, s->commit_map->ptr[i]);
	return ret;
}
