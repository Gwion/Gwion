#ifndef __OO
#define __OO

#include "defs.h"
#include "array.h"

typedef enum {
  e_dll_obj, e_emit_obj, e_namespace_obj, e_env_obj, e_context_obj,
  e_ugen_obj, e_type_obj, e_value_obj, e_func_obj, e_array_obj
} e_obj;


struct VM_Object_ {
  e_obj  type;
  m_uint ref_count;
  m_bool lock;
  Vector ref;
};

VM_Object new_VM_Object(e_obj type);
void add_ref(VM_Object a);
void rem_ref(VM_Object a, void* ptr);


struct M_Object_ {
  Vector vtable;
  Type type_ref;
  m_uint size, ref;
  union {
    M_Vector* array;
    unsigned char* data;
  } d;
  UGen ugen;
};
M_Object new_M_UGen();
M_Object new_M_Object();
#endif
