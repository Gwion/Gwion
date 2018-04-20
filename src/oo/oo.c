#include "defs.h"
#include "oo.h"

extern ANN void free_nspc(void* a);
extern ANN void free_type(void* a);
extern ANN void free_value(void* a);
extern ANN void free_context(void* a);
extern ANN void free_func(void* a);
extern ANN void free_vm_code(void* a);

typedef void (*cleaner)(void*);
static cleaner cleaners[] = { free_nspc, free_context, free_type,
  free_value, free_func, free_vm_code };

ANN void rem_ref(const VM_Object a, void* ptr) {
  if(--a->ref_count)
    return;
  cleaners[a->type](ptr);
}
