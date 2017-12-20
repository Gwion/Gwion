#include <stdlib.h>
#include <dlfcn.h>
#include <pthread.h>
#include "vm.h"
#include "type.h"
#include "instr.h"
#include "ugen.h"

Shreduler new_shreduler(VM* vm);
Shreduler free_shreduler(Shreduler s);

VM* new_vm(m_bool loop) {
  VM* vm = (VM*)calloc(1, sizeof(VM));
  if(pthread_mutex_init(&vm->mutex, NULL)) {
    free(vm);
    return NULL;
  }
  vm->shreduler  = new_shreduler(vm);
  vector_init(&vm->shred);
  vector_init(&vm->ugen);
  vector_init(&vm->plug);
  shreduler_set_loop(vm->shreduler, loop < 0 ? 0 : 1);
  return vm;
}

void free_vm(VM* vm) {
  m_uint i;
  if(vm->emit)
    free_emitter(vm->emit);
  for(i = vector_size(&vm->plug) + 1; --i;)
    dlclose((void*)vector_at(&vm->plug, i - 1));
  vector_release(&vm->plug);
  vector_release(&vm->shred);
  vector_release(&vm->ugen);
  if(vm->sp)
    sp_destroy(&vm->sp);
  free(vm->in);
  free_shreduler(vm->shreduler);
  free(vm);
  free_symbols();
}

void vm_add_shred(VM* vm, VM_Shred shred) {
  shred->vm_ref = vm;
  if(!shred->me)
    shred->me = new_shred(vm, shred);
  if(!shred->xid) {
    vector_add(&vm->shred, (vtype)shred);
    shred->xid = vm->shreduler->n_shred++;
  }
  shredule(vm->shreduler, shred, .5);
}

static void vm_run_shred(VM* vm, VM_Shred shred) {
  Instr instr;
  while(vm->shreduler->curr) {
    shred->pc = shred->next_pc++;
    instr = (Instr)vector_at(shred->code->instr, shred->pc);
    instr->execute(vm, shred, instr);
    /*fprintf(stderr, "shred[%" UINT_F "] mem[%" INT_F"] reg[%" INT_F"]\n", shred->xid,*/
    /*shred->mem - shred->_mem, shred->reg - shred->_reg);*/
    if(!shred->me)
     shreduler_remove(vm->shreduler, shred, 1);
  }
}

static void vm_ugen_init(VM* vm) {
  m_uint i;
  for(i = vector_size(&vm->ugen) + 1; --i;) {
    UGen u = (UGen)vector_at(&vm->ugen, i - 1);
    u->done = 0;
    if(u->channel) {
      m_uint j;
      for(j = u->n_chan + 1; --j;) // miss + 1
        UGEN(u->channel[j - 1])->done = 0;
    }
    if(u->trig)
      UGEN(u->trig)->done = 0;
  }
  ugen_compute(UGEN(vm->adc));
  ugen_compute(UGEN(vm->dac));
  ugen_compute(UGEN(vm->blackhole));
}

void vm_run(VM* vm) {
  VM_Shred shred;
  pthread_mutex_lock(&vm->mutex);
  while((shred = shreduler_get(vm->shreduler)))
    vm_run_shred(vm, shred);
  pthread_mutex_unlock(&vm->mutex);
  if(!vm->is_running)
    return;
  vm_ugen_init(vm);
}
