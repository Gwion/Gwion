#ifndef __OO
#define __OO

typedef struct VM_Object_ * VM_Object;
typedef struct Type_      * Type;
typedef enum {
  e_nspc_obj,  e_context_obj,
  e_type_obj, e_value_obj, e_func_obj, e_code_obj
} e_obj;

struct VM_Object_ {
  m_uint ref_count;
  e_obj  type;
} __attribute__((packed));


#define HAS_OBJ struct VM_Object_ obj;
#define INIT_OO(a, b) { (a)->obj.type = b; (a)->obj.ref_count = 1; }
#define REM_REF(a)    { rem_ref(&(a)->obj, (a)); }
#define ADD_REF(a)    { ++(a)->obj.ref_count; }
ANN void rem_ref(const VM_Object a, void* ptr);
#endif
