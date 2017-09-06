#include "defs.h"
#include "oo.h"

extern void free_nspc(void* a);
extern void free_type(void* a);
extern void free_value(void* a);
extern void free_context(void* a);
extern void free_func(void* a);
extern void free_vm_code(void* a);

typedef void (*cleaner)(void*);
static cleaner cleaners[] = { free_nspc, free_context, free_type, 
  free_value, free_func, free_vm_code };

void rem_ref(VM_Object a, void* ptr) {
  if(!--a->ref_count)
    cleaners[a->type](ptr);
}
