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
  struct BBQ_ * di = (struct BBQ*)xcalloc(1, sizeof(struct BBQ_));
  di->func = dummy_driver;
  di->run = vm_run;
  di->driver = (DriverData*)xcalloc(1, sizeof(DriverData));
  return di;
}

ANN static Arg* new_arg(int argc, char** argv) {
  Arg *arg = (Arg*)xcalloc(1, sizeof(Arg));
  arg->argc = argc;
  arg->argv = argv;
  arg->loop = -1;
  return arg;
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

ANN static inline void gwion_compile(const Gwion gwion) {
  for(m_uint i = 0; i < vector_size(&gwion->arg->add); i++)
    compile_filename(gwion, (m_str)vector_at(&gwion->arg->add, i));
}
ANN m_bool gwion_ini(const Gwion gwion, int argc, char** argv) {
  gwion->vm = new_vm();
  gwion->emit = new_emitter();
  gwion->env = new_env();
  gwion->emit->env = gwion->env;
  gwion->vm->gwion = gwion;
  gwion->env->gwion = gwion;
  gwion->vm->bbq = new_driverinfo();
  gwion->arg = new_arg(argc, argv);
  gwion->arg->si = mp_alloc(SoundInfo);
  gwion->arg->si->in = gwion->arg->si->out = 2;
  gwion->arg->si->sr = 48000;
  gwion->plug = (PlugInfo*)xmalloc(sizeof(PlugInfo));
  arg_parse(gwion->arg);
  gwion->vm->bbq->si = gwion->arg->si;
  plug_discover(gwion->plug, &gwion->arg->lib);
  shreduler_set_loop(gwion->vm->shreduler, gwion->arg->loop);
  if(gwion_audio(gwion) > 0 && gwion_engine(gwion)) {
    gwion_compile(gwion);
    return GW_OK;
  }
  return GW_ERROR;
}

ANN void gwion_run(const Gwion gwion) {
  VM* vm = gwion->vm;
  vm->bbq->is_running = 1;
  plug_ini(gwion, &gwion->arg->mod);
  VMBENCH_INI
  vm->bbq->driver->run(vm, vm->bbq);
  VMBENCH_END
}

ANN /* static */ void free_driverinfo(struct BBQ_* di, VM* vm) {
  mp_free(SoundInfo, di->si);
  if(di->driver->del)
    di->driver->del(vm, di);
  xfree(di->driver);
  xfree(di);
}
ANN void gwion_end(const Gwion gwion) {
  plug_end(gwion);
  arg_release(gwion->arg);
  xfree(gwion->arg);
  free_env(gwion->env);
  free_emitter(gwion->emit);
  free_vm(gwion->vm);
  xfree(gwion->plug);
  free_symbols();
}
