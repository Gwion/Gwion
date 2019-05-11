#ifndef __OO
#define __OO

typedef struct Type_      * Type;
typedef struct Nspc_      * Nspc;
typedef struct Value_     * Value;
typedef struct Func_      * Func;

struct VM_Object_ {
  void (*free)(void*,void*);
  uint16_t ref_count; // could be an unsigned short
};

#define HAS_OBJ struct VM_Object_* obj;
#define INIT_OO(mp, a, b) { (a)->obj = mp_alloc(mp, VM_Object); (a)->obj->ref_count = 1; (a)->obj->free= (void(*)(void*,void*))b; }
ANN static inline void rem_ref(MemPool mp, struct VM_Object_* a, void* ptr, void *gwion) {
  if(--a->ref_count)
    return;
  a->free(ptr, gwion);
  mp_free(mp, VM_Object, a);
}
#define ADD_REF(a)    { ++(a)->obj->ref_count; }
#define REM_REF(a, b)    { rem_ref(((Gwion)(b))->mp, (a)->obj, (a), (b)); }
#endif
