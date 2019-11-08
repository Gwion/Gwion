#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "object.h"
#include "gwion.h"

struct Stack_ {
  VM_Shred shred;
  char c[SIZEOF_REG];
  char d[SIZEOF_MEM];
};

static inline struct ShredInfo_ *new_shredinfo(MemPool p, const m_str name) {
  struct ShredInfo_ *info = mp_calloc(p, ShredInfo);
  info->mp = p;
  info->name = mstrdup(p, name);
  return info;
}

static inline void free_shredinfo(MemPool mp, struct ShredInfo_ *info) {
  free_mstr(mp, info->name);
  if(info->args) {
    const Vector v = info->args;
    LOOP_OPTIM
    for(m_uint i = vector_size(v) + 1; --i;)
      xfree((void*)vector_at(v, i - 1));
    free_vector(mp, v);
  }
  mp_free(mp, ShredInfo, info);
}

VM_Shred new_vm_shred(MemPool p, VM_Code c) {
  const VM_Shred shred = mp_calloc(p, Stack);
  shred->code = c;
  shred->reg  = (m_bit*)shred + sizeof(struct VM_Shred_);
  shred->base = shred->mem = shred->reg + SIZEOF_REG;
  shred->info = new_shredinfo(p, c->name);
  shred->info->orig = c;
  vector_init(&shred->gc);
  return shred;
}

void free_vm_shred(VM_Shred shred) {
  for(m_uint i = vector_size(&shred->gc) + 1; --i;)
    release((M_Object)vector_at(&shred->gc, i - 1), shred);
  vector_release(&shred->gc);
  REM_REF(shred->info->orig, shred->info->vm->gwion);
  const MemPool mp = shred->info->mp;
  mp_free(mp, ShredTick, shred->tick);
  free_shredinfo(mp, shred->info);
  mp_free(mp, Stack, shred);
}
