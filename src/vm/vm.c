#include <stdlib.h>
#include <dlfcn.h>
#include <pthread.h>
#include "vm.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "ugen.h"
#include "shreduler_private.h"

// not the best place
ANEW struct Scanner_* new_scanner(const m_uint size);
ANN void free_scanner(struct Scanner_* scan);

VM* new_vm(const m_bool loop) {
  VM* vm = (VM*)xcalloc(1, sizeof(VM));
  vm->shreduler  = (Shreduler)xcalloc(1, sizeof(struct Shreduler_));
  vm->shreduler->vm = vm;
  vector_init(&vm->shred);
  vector_init(&vm->ugen);
  shreduler_set_loop(vm->shreduler, loop);
  vm->scan = new_scanner(127); // !!! magic number
  srand(time(NULL));
  return vm;
}

void vm_remove(const VM* vm, const m_uint index) {
  const Vector v = (Vector)&vm->shred;
  LOOP_OPTIM
  for(m_uint i = vector_size(v) + 1; i--;) {
    const VM_Shred sh = (VM_Shred)vector_at(v, i - 1);
    if(sh->xid == index)
       Except(sh, "MsgRemove");
  }
}

ANN void free_vm(VM* vm) {
  if(vm->emit)
    free_emitter(vm->emit);
  vector_release(&vm->shred);
  vector_release(&vm->ugen);
  xfree(vm->bbq->in);
  xfree(vm->bbq->out);
  xfree(vm->bbq);
  xfree(vm->shreduler);
  free_scanner(vm->scan);
  free(vm);
}

static  m_uint shred_ids;
ANN void vm_add_shred(const VM* vm, const VM_Shred shred) {
  const Vector v = (Vector)&vm->shred;
  vector_add(v, (vtype)shred);
  shred->vm_ref = (VM*)vm;
  shred->xid = ++shred_ids;
  shred->me = new_shred(shred);
  shredule(vm->shreduler, shred, .5);
}

__attribute__((hot))
ANN static inline void vm_ugen_init(const VM* vm) {
  const Vector v = (Vector)&vm->ugen;
  LOOP_OPTIM
  for(m_uint i = vector_size(v) + 1; --i;) {
    const UGen u = (UGen)vector_at(v, i - 1);
    u->done = 0;
    if(GET_FLAG(u, UGEN_MULTI)) {
      struct ugen_multi_* m = u->connect.multi;
      LOOP_OPTIM
      for(m_uint j = m->n_chan + 1; --j;)
        UGEN(m->channel[j - 1])->done = 0;
    }
  }
  const UGen hole = UGEN(vm->blackhole);
  hole->compute(hole);
  const UGen dac = UGEN(vm->dac);
  dac->compute(dac);
}

#ifdef DEBUG_STACK
#define VM_INFO                                                              \
  if(s->curr)                                                                \
    gw_err("shred[%" UINT_F "] mem[%" INT_F"] reg[%" INT_F"]\n", shred->xid, \
    shred->mem - (shred->_reg + SIZEOF_REG), shred->reg - shred->_reg);
#else
#define VM_INFO
#endif

#ifdef VMBENCH
#include <time.h>
static struct timespec exec_time;
#include <bsd/sys/time.h>
#endif

__attribute__((hot))
ANN void vm_run(const VM* vm) {
  const Shreduler s = vm->shreduler;
  VM_Shred shred;
  while((shred = shreduler_get(s))) {
//  pthread_mutex_lock(&vm->mutex);

#ifdef VMBENCH
struct timespec exec_ini, exec_end, exec_ret;
clock_gettime(CLOCK_THREAD_CPUTIME_ID, &exec_ini);
#endif
  do {
      const Instr instr = (Instr)vector_at(shred->code->instr, shred->pc++);
      instr->execute(shred, instr);
      VM_INFO;
    } while(s->curr);
//  pthread_mutex_unlock(&vm->mutex);
#ifdef VMBENCH
clock_gettime(CLOCK_THREAD_CPUTIME_ID, &exec_end);
timespecsub(&exec_end, &exec_ini, &exec_ret);
timespecadd(&exec_time, &exec_ret, &exec_time);
#endif
  }
  if(!vm->is_running) {
#ifdef VMBENCH
    printf("[VM] exec time %lu.%09lu\n", exec_time.tv_sec, exec_time.tv_nsec);
#endif
    return;
  }
  vm_ugen_init(vm);
}
