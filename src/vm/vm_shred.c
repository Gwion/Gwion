#include <stdlib.h>
#include <string.h>
#include "vm.h"
#include "type.h"
#include "instr.h"
#include "mpool.h"

struct MemStack_ { char c[SIZEOF_MEM]; };
struct RegStack_ { char c[SIZEOF_REG]; };

POOL_HANDLE(VM_Shred, 512)
POOL_HANDLE(MemStack, 512)
POOL_HANDLE(RegStack, 512)

VM_Shred new_vm_shred(VM_Code c) {
  VM_Shred shred    = mp_alloc(VM_Shred);
  shred->base       = mp_alloc(MemStack);
  shred->_reg       = mp_alloc(RegStack);
  shred->reg        = shred->_reg;
  shred->mem        = shred->base;
  shred->_mem       = shred->base;
  shred->code       = c;
  shred->name       = strdup(c->name);
  vector_init(&shred->gc1);
  return shred;
}

static void vm_shred_free_args(Vector v) {
  LOOP_OPTIM
  for(m_uint i = vector_size(v) + 1; --i;)
    free((void*)vector_at(v, i - 1));
  free_vector(v);
}

static void free_shred_code(VM_Shred shred) {
  if(strncmp(shred->name, "spork~exp", 9))
    REM_REF(shred->code)
  if(shred->sporks.ptr) {
    LOOP_OPTIM
    for(m_uint i = vector_size(&shred->sporks) + 1; --i;)
       REM_REF(((VM_Code)vector_at(&shred->sporks, i - 1)))
    vector_release(&shred->sporks);
  }
}

void free_vm_shred(VM_Shred shred) {
  release(shred->me, shred);
  if(!strstr(shred->name, "spork~"))
    mp_free(MemStack, shred->base);
  else
    mp_free(MemStack, shred->_mem);
  mp_free(RegStack, shred->_reg);
  free_shred_code(shred);
  free(shred->name);
  vector_release(&shred->gc1);
  if(shred->args)
    vm_shred_free_args(shred->args);
  mp_free(VM_Shred, shred);
}
