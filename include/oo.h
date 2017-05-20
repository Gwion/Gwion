#ifndef __OO
#define __OO

#include "defs.h"
#include "array.h"

typedef enum {
  e_emit_obj, e_namespace_obj, e_env_obj, e_context_obj,
  e_type_obj, e_value_obj, e_func_obj
} e_obj;


struct VM_Object_ {
  e_obj  type;
  m_uint ref_count;
};

static inline void add_ref(VM_Object a) { a->ref_count++; }
void rem_ref(VM_Object a, void* ptr);
#endif
