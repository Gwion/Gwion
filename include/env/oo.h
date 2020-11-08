#ifndef __OO
#define __OO

typedef struct Gwion_      * Gwion;

typedef struct Type_      * Type;
typedef struct Nspc_      * Nspc;
typedef struct Value_     * Value;
typedef struct Func_      * Func;

typedef struct RefCount_ {
  void (*free)(void*,void*);
  uint16_t count; // could be an unsigned short
} RefCount;

#define HAS_OBJ RefCount* ref;
ANN static inline RefCount* new_refcount(MemPool mp, void(*free)(void*,void*)) {
  RefCount *ref = (RefCount*)mp_calloc(mp, RefCount);
  ref->count = 1;
  ref->free= free;
  return ref;
}
#define new_refcount(a, b) new_refcount(a, (void(*)(void*,void*))b)
ANN static inline void rem_ref(MemPool mp, RefCount* a, void* ptr, void *gwion) {
  if(--a->count)
    return;
  a->free(ptr, gwion);
  mp_free(mp, RefCount, a);
}
#define ADD_REF(a)    { ++(a)->ref->count; }
#define REM_REF(a, b)    { rem_ref(((Gwion)(b))->mp, (a)->ref, (a), (b)); }
#endif
