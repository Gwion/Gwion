#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "instr.h"
#include "emit.h"
#include "engine.h"
#include "driver.h"
#include "arg.h"
#include "gwion.h"
#include "compile.h"
#include "sound.h"

#ifdef VMBENCH
#include <time.h>
#include <bsd/sys/time.h>
#define VMBENCH_INI struct timespec ini, end, ret; \
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ini);
#define VMBENCH_END clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end); \
  timespecsub(&end, &ini, &ret);                                  \
  printf("timespec %lu.%09lu\n", ret.tv_sec, ret.tv_nsec);
#else
#define VMBENCH_INI
#define VMBENCH_END
#endif

ANN static struct BBQ_ * new_driverinfo(void) {
  struct BBQ_ * di = (struct BBQ_*)mp_alloc(BBQ);
  di->func = dummy_driver;
  di->run = vm_run;
  di->driver = (DriverData*)mp_alloc(DriverData);
  di->is_running = 1;
  return di;
}

ANN m_bool gwion_audio(const Gwion gwion) {
  struct BBQ_ *di = gwion->vm->bbq;
  // get driver from string.
  if(di->si->arg) {
    for(m_uint i = 0; i < map_size(&gwion->plug->drv); ++i) {
      const m_str name = (m_str)VKEY(&gwion->plug->drv, i);
      const size_t len = strlen(name);
      if(!strncmp(name, di->si->arg, len)) {
        di->func = (f_bbqset)VVAL(&gwion->plug->drv, i);
        break;
      }
    }
  }
  di->func(di->driver);
  VM* vm = gwion->vm;
  CHECK_BB(di->driver->ini(vm, di));
  bbq_alloc(di);
  return GW_OK;
}

ANN static inline m_bool gwion_engine(const Gwion gwion) {
  return type_engine_init(gwion->vm, &gwion->plug->vec[GWPLUG_IMPORT]) > 0;
}

ANN static inline void gwion_compile(const Gwion gwion, const Vector v) {
  for(m_uint i = 0; i < vector_size(v); i++)
    compile_filename(gwion, (m_str)vector_at(v, i));
}

ANN m_bool gwion_ini(const Gwion gwion, Arg* arg) {
  gwion->vm = new_vm();
  gwion->emit = new_emitter();
  gwion->env = new_env();
  gwion->emit->env = gwion->env;
  gwion->vm->gwion = gwion;
  gwion->env->gwion = gwion;
  gwion->vm->bbq = new_driverinfo();
  gwion->vm->bbq->si = mp_alloc(SoundInfo);
  gwion->vm->bbq->si->in = gwion->vm->bbq->si->out = 2;
  gwion->vm->bbq->si->sr = 48000;
  arg->si = gwion->vm->bbq->si;
  arg_parse(arg);
  gwion->plug = new_plug(&arg->lib);
  shreduler_set_loop(gwion->vm->shreduler, arg->loop);
  if(gwion_audio(gwion) > 0 && gwion_engine(gwion)) {
    gwion_compile(gwion, &arg->add);
    plug_run(gwion, &arg->mod);
    return GW_OK;
  }
  return GW_ERROR;
}

ANN void gwion_run(const Gwion gwion) {
  VM* vm = gwion->vm;
  VMBENCH_INI
  vm->bbq->driver->run(vm, vm->bbq);
  VMBENCH_END
}

ANN /* static */ void free_driverinfo(struct BBQ_* bbq, VM* vm) {
  mp_free(SoundInfo, bbq->si);
  if(bbq->driver->del)
    bbq->driver->del(vm, bbq);
  mp_free(DriverData, bbq->driver);
  mp_free(BBQ, bbq);
}

ANN void gwion_end(const Gwion gwion) {
  free_env(gwion->env);
  free_emitter(gwion->emit);
  free_vm(gwion->vm);
  free_plug(gwion);
  free_symbols();
}
