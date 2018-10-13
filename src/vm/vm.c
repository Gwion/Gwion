#include <stdlib.h>
#include <string.h>
#include "vm.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "ugen.h"
#include "shreduler_private.h"
#include "emit.h"

#ifdef DEBUG_STACK
#include "err_msg.h"
#endif


static inline uint64_t splitmix64_stateless(uint64_t index) {
  uint64_t z = (index + UINT64_C(0x9E3779B97F4A7C15));
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}

static inline uint32_t rotl(const uint32_t x, int k) {
  return (x << k) | (x >> (32 -k));
}

/*xoroshiro32** */
uint32_t gw_rand(uint32_t s[2]) {
  const uint32_t s0 = s[0];
  const uint32_t s1 = s[1] ^ s0;
  const uint32_t ret = rotl(s0 * 0x9E3779BB, 5) * 5;
  s[0] = rotl(s0, 26) ^ s1 ^ (s1 << 9);
  s[1] = rotl(s1, 13);
  return ret;
}

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
  uint64_t seed = splitmix64_stateless(time(NULL));
  memcpy(vm->rand, &seed, sizeof(uint64_t));
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
  shred->vm = (VM*)vm;
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
#ifdef VMBENCH
struct timespec exec_ini, exec_end, exec_ret;
clock_gettime(CLOCK_THREAD_CPUTIME_ID, &exec_ini);
#endif
  do {
      const Instr instr = (Instr)vector_at(shred->code->instr, shred->pc++);
//if(instr->execute != JitExec)exit(shred->pc);
      instr->execute(shred, instr);
      VM_INFO;
    } while(s->curr);
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
