#ifndef __MAP
#define __MAP
typedef struct Vector_ * Vector;
typedef struct Map_ * Map;

extern Vector new_Vector();
extern Vector new_Vector_fixed(const long unsigned int len);
Vector vector_copy(Vector v);
extern void vector_set(Vector vector, const long unsigned i,void* data);
extern long int vector_find(Vector vector, void* data);
//extern void vector_set(Vector vector, const long unsigned i,const long unsigned int);
extern void vector_append(Vector vector, void* data);
extern void vector_remove(Vector vector, const long unsigned int index);
extern void vector_pop(Vector vector);
extern void* vector_front(Vector vector);
extern void* vector_at(Vector vector, const long unsigned int);
extern void* vector_back(Vector vector);
extern long unsigned int vector_size(Vector vector);
extern void vector_clear(Vector vector);
extern void free_Vector(Vector vector);

extern Map   new_Map();
extern void* map_get(Map map, void* key);
extern void* map_at(Map map, const long unsigned int index);
extern void* map_key(Map map, const long unsigned int index);
extern void  map_set(Map map, void* key, void* ptr);
extern void  map_remove(Map map, void* key);
extern void  map_commit(Map map, Map commit);
extern void  map_rollback(Map map, void (*_free)(void*));
extern long unsigned int map_size(Map map);
extern void  free_Map(Map map);

#include <symbol.h>
typedef struct Scope_* Scope;

extern Scope  new_Scope();
extern Vector scope_get(Scope a);
extern void*  scope_lookup(Scope scope, S_Symbol xid, int climb);
extern void   scope_add(Scope scope, S_Symbol xid, void* value);
extern void   scope_commit(Scope scope);
extern void   scope_rollback(Scope scope, void (*_free)(void* arg));
extern void   scope_push(Scope scope);
extern Map    scope_pop(Scope scope);
extern void   free_Scope(Scope a);

// maybe elsewhere ?
typedef struct fVector_ * fVector;
extern fVector new_fVector();
extern fVector new_fVector_fixed(const long unsigned int len);
extern double fvector_at(fVector vector, const long unsigned int);
extern double* fvector_addr(fVector vector, const long unsigned int);

#endif
