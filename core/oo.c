#include <stdlib.h>
#include <stdio.h>
#include "map.h"
#include "context.h"
#include "func.h"
#include "oo.h"

#include "emit.h"

void rem_ref(VM_Object a, void* ptr) {
  if(!--a->ref_count) {
    switch(a->type) {
    case e_emit_obj:
      free_emitter(ptr);
      break;
    case e_namespace_obj:
      free_NameSpace(ptr);
      break;
    case e_env_obj:
      free_env(ptr);
      break;
    case e_type_obj:
      free_type(ptr);
      break;
    case e_value_obj:
      free_value(ptr);
      break;
    case e_context_obj:
      free_context(ptr);
      break;
    case e_func_obj:
      free_func(ptr);
      break;
    }
  }
  return;
}
