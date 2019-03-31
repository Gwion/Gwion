#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "object.h"

struct Stack_ {
  VM_Shred shred;
  char c[SIZEOF_REG];
  char d[SIZEOF_MEM];
};

static inline struct ShredInfo_ *new_shredinfo(const m_str name) {
  struct ShredInfo_ *info = mp_alloc(ShredInfo);
  info->name = strdup(name);
  return info;
}

static inline void free_shredinfo(struct ShredInfo_ *info) {
  free(info->name);
  if(info->args) {
    const Vector v = info->args;
    LOOP_OPTIM
    for(m_uint i = vector_size(v) + 1; --i;)
      free((void*)vector_at(v, i - 1));
    free_vector(v);
  }
  mp_free(ShredInfo, info);
}

VM_Shred new_vm_shred(VM_Code c) {
  const VM_Shred shred = mp_alloc(Stack);
  shred->code          = c;
  shred->reg           = (m_bit*)shred + sizeof(struct VM_Shred_);
  shred->base = shred->mem = shred->reg + SIZEOF_REG;
  shred->tick = mp_alloc(ShredTick);
  shred->tick->self = shred;
  shred->info = new_shredinfo(c->name);
  vector_init(&shred->gc);
  return shred;
}

void free_vm_shred(VM_Shred shred) {
  for(m_uint i = vector_size(&shred->gc) + 1; --i;)
    release((M_Object)vector_at(&shred->gc, i - 1), shred);
  vector_release(&shred->gc);
  REM_REF(shred->code, shred->info->vm->gwion);
  mp_free(ShredTick, shred->tick);
  free_shredinfo(shred->info);
  mp_free(Stack, shred);
}