#ifndef __MAP
#define __MAP
#include "defs.h"
#include <symbol.h>

#define MAP_MAX 1024

typedef m_uint vtype;
typedef struct Vector_ * Vector;
typedef struct Map_    * Map;

extern Vector new_Vector();
extern Vector new_Vector_fixed(const vtype len);
extern Vector vector_copy(Vector v);
extern void vector_set(Vector vector, const vtype i, vtype data);
extern long int vector_find(Vector vector, vtype data);
extern void vector_append(Vector vector, vtype data);
extern void vector_remove(Vector vector, const vtype index);
extern void vector_pop(Vector vector);
extern vtype vector_front(Vector vector);
extern vtype vector_at(Vector vector, const vtype);
extern vtype vector_back(Vector vector);
extern vtype vector_size(Vector vector);
extern void vector_clear(Vector vector);
extern void free_Vector(Vector vector);

extern Map   new_Map();
extern vtype map_get(Map map, vtype key);
extern vtype map_at(Map map, const vtype index);
extern vtype map_key(Map map, const vtype index);
extern void  map_set(Map map, vtype key, vtype ptr);
extern void  map_remove(Map map, vtype key);
extern void  map_commit(Map map, Map commit);
extern void  map_rollback(Map map, void (*_free)(vtype));
extern vtype map_size(Map map);
extern void  free_Map(Map map);

extern Scope  new_Scope();
extern Vector scope_get(Scope a);
extern vtype  scope_lookup(Scope scope, S_Symbol xid, int climb);
extern void   scope_add(Scope scope, S_Symbol xid, vtype value);
extern void   scope_rem(Scope scope, S_Symbol xid);
extern void   scope_commit(Scope scope);
extern void   scope_rollback(Scope scope, void (*_free)(vtype arg));
extern void   scope_push(Scope scope);
extern void   scope_pop(Scope scope);
extern void   free_Scope(Scope a);
#endif
