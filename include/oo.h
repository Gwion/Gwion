#ifndef __OO
#define __OO

#include "defs.h"

typedef enum {
  e_emit_obj, e_nspc_obj,  e_context_obj,
  e_type_obj, e_value_obj, e_func_obj, e_code_obj
} e_obj;


struct VM_Object_ {
  e_obj  type;
  m_uint ref_count;
};

#define INIT_OO(a, b) { a->obj.type = b; a->obj.ref_count = 1; }
#define REM_REF(a)    { rem_ref(&a->obj, a); }
#define ADD_REF(a)    { a->obj.ref_count++; }
void rem_ref(VM_Object a, void* ptr);
#endif
