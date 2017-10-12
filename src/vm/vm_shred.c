#include <stdlib.h>
#include "vm.h"
#include "type.h"
#include "instr.h"

VM_Shred new_vm_shred(VM_Code c) {
  VM_Shred shred    = calloc(1, sizeof(struct VM_Shred_));
  shred->base       = calloc(SIZEOF_MEM, sizeof(char));
  shred->_reg       = calloc(SIZEOF_REG, sizeof(char));
  shred->reg        = shred->_reg;
  shred->mem        = shred->base;
  shred->_mem       = shred->base;
  shred->code       = c;
  shred->name       = strdup(c->name);
  vector_init(&shred->gc1);
  return shred;
}

static void vm_shred_free_args(Vector v) {
  m_uint i;
  for(i = 0; i < vector_size(v); i++)
    free((void*)vector_at(v, i));
  free_vector(v);
}

void free_vm_shred(VM_Shred shred) {
  release(shred->me, shred);
  if(strstr(shred->name, "spork~"))
    free(shred->_mem);
  else
    free(shred->base);
  free(shred->_reg);
  REM_REF(shred->code)
  free(shred->name);
  free(shred->filename);
  vector_release(&shred->gc1);
  if(shred->args)
    vm_shred_free_args(shred->args);
  free(shred);
}
