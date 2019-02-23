#ifndef __OO
#define __OO

typedef struct Type_      * Type;
typedef struct Nspc_      * Nspc;
typedef struct Value_     * Value;
typedef struct Func_      * Func;

struct VM_Object_ {
  uint16_t ref_count; // could be an unsigned short
  void (*free)(void*);
};

#define HAS_OBJ struct VM_Object_ obj;
#define INIT_OO(a, b) { (a)->obj.ref_count = 1; (a)->obj.free= (void(*)(void*))b; }
ANN static inline void rem_ref(struct VM_Object_* a, void* ptr) {
  if(--a->ref_count)
    return;
  a->free(ptr);
}
#define ADD_REF(a)    { ++(a)->obj.ref_count; }
#define REM_REF(a)    { rem_ref(&(a)->obj, (a)); }
#endif
