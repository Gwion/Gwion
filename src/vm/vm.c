#include <stdlib.h>
#include <dlfcn.h>
#include <pthread.h>
#include "vm.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "ugen.h"
#include "shreduler_private.h"

#ifdef CURSES
extern void gw_shred(VM_Shred shred);
#endif

VM* new_vm(const m_bool loop) {
  VM* vm = (VM*)xcalloc(1, sizeof(VM));
  vm->shreduler  = (Shreduler)xcalloc(1, sizeof(struct Shreduler_));
  vm->shreduler->vm = vm;
  vector_init(&vm->shred);
  vector_init(&vm->ugen);
  vector_init(&vm->plug);
  shreduler_set_loop(vm->shreduler, loop);
  return vm;
}

void vm_remove(const VM* vm, const m_uint index) {
  const Vector v = (Vector)&vm->shred;
  LOOP_OPTIM
  for(m_uint i = vector_size(v) + 1; i--;) {
    const VM_Shred sh = (VM_Shred)vector_at(v, i - 1);
    const Vector w = (Vector)&sh->child;
    if(sh->xid == index) {
      if(sh->child.ptr)
        LOOP_OPTIM
        for(m_uint j = vector_size(w) + 1; --j;)
          NullException((VM_Shred)vector_at(w, j - 1), "MsgRemove");
       NullException(sh, "MsgRemove");
       return;
    }
  }
}

ANN void free_vm(VM* vm) {
  if(vm->emit)
    free_emitter(vm->emit);
  LOOP_OPTIM
  for(m_uint i = vector_size(&vm->plug) + 1; --i;)
    dlclose((void*)vector_at(&vm->plug, i - 1));
  vector_release(&vm->plug);
  vector_release(&vm->shred);
  vector_release(&vm->ugen);
  if(vm->sp)
    sp_destroy(&vm->sp);
  free(vm->in);
  free(vm->shreduler);
  free(vm);
  free_symbols();
}

ANN void vm_add_shred(const VM* vm, const VM_Shred shred) {
  const Vector v = (Vector)&vm->shred;
  shred->vm_ref = (VM*)vm;
  if(!shred->me)
    shred->me = new_shred(shred);
  if(!shred->xid) {
    vector_add(v, (vtype)shred);
    shred->xid = ++vm->shreduler->n_shred;
  }
  shredule(vm->shreduler, shred, .5);
}

__attribute__((hot))
ANN static inline void vm_ugen_init(const VM* vm) {
  const Vector v = (Vector)&vm->ugen;
  LOOP_OPTIM
  for(m_uint i = vector_size(v) + 1; --i;) {
    const UGen u = (UGen)vector_at(v, i - 1);
    u->done = 0;
    if(GET_FLAG(u, UGEN_MULTI))
      LOOP_OPTIM
      for(m_uint j = u->connect.multi->n_chan + 1; --j;)
        UGEN(u->connect.multi->channel[j - 1])->done = 0;
  }
  const UGen hole = UGEN(vm->blackhole);
  hole->compute(hole);
  const UGen dac = UGEN(vm->dac);
  dac->compute(dac);
}

#ifdef CURSES
#define VM_INFO gw_shred(s->curr);
#else
#ifdef DEBUG_STACK
#define VM_INFO                                                              \
  if(s->curr)                                                                \
    gw_err("shred[%" UINT_F "] mem[%" INT_F"] reg[%" INT_F"]\n", shred->xid, \
    shred->mem - shred->_mem, shred->reg - shred->_reg);
#else
#define VM_INFO
#endif
#endif

void vm_run(const VM* vm) {
  const Shreduler s = vm->shreduler;
  VM_Shred shred;
  while((shred = shreduler_get(s))) {
    while(s->curr) {
      shred->pc = shred->next_pc++;
      const Instr instr = (Instr)vector_at(shred->code->instr, shred->pc);
      instr->execute(shred, instr);
      VM_INFO;
    }
  }
  if(!vm->is_running)
    return;
  vm_ugen_init(vm);
}
