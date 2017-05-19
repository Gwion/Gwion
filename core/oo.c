#include <stdlib.h>
#include <stdio.h>
#include "map.h"
#include "context.h"
#include "func.h"
#include "oo.h"

#include "emit.h"

static m_bool our_locks_in_effects = 0;

VM_Object new_VM_Object(e_obj type)
{
  VM_Object a = malloc(sizeof(struct VM_Object_));
  a->type = type;
  a->ref_count = 0;
  a->lock = 0;
  return a;
}

void rem_ref(VM_Object a, void* ptr)
{
  if((!--a->ref_count && !a->lock) || !our_locks_in_effects) {
    switch(a->type) {
    case e_emit_obj:
      free_Emitter(ptr);
      break;
    case e_namespace_obj:
      free_NameSpace(ptr);
      break;
    case e_env_obj:
      free_Env(ptr);
      break;
    case e_type_obj:
      free_Type(ptr);
      break;
    case e_value_obj:
      free_Value(ptr);
      break;
    case e_context_obj:
      free_Context(ptr);
      break;
    case e_func_obj:
      free_Func(ptr);
      break;
    }
    free(a);
  }
  return;
}
