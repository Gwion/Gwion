#include <stdlib.h>
#include <stdio.h>
#include "map.h"
#include "context.h"
#include "func.h"
#include "oo.h"

extern void free_nspc(Nspc a);
extern void free_type(Type a);
extern void free_value(Value a);
extern void free_context(Context a);
extern void free_func(Func a);
extern void free_vm_code(VM_Code a);

void rem_ref(VM_Object a, void* ptr) {
  if(!--a->ref_count) {
    switch(a->type) {
      case e_nspc_obj:
        free_nspc(ptr);
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
      case e_code_obj:
        free_vm_code(ptr);
        break;
    }
  }
}
