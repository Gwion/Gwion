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

ANN static DriverInfo* new_driverinfo(void) {
  DriverInfo *di = (DriverInfo*)xcalloc(1, sizeof(DriverInfo));
  di->sr = 48000;
  di->in = di->out = 2;
  di->func = dummy_driver;
  di->run = vm_run;
  di->driver = (Driver*)xcalloc(1, sizeof(Driver));
  return di;
}

ANN void gwion_init(const Gwion gwion, Arg* arg) {
  gwion->vm = new_vm();
  gwion->emit = new_emitter();
  gwion->env = new_env();
  gwion->emit->env = gwion->env;
  gwion->vm->gwion = gwion;
  gwion->env->gwion = gwion;
  gwion->di = new_driverinfo();
  gwion->plug = (PlugInfo*)xmalloc(sizeof(PlugInfo));
  arg_parse(arg, gwion->di);
  plug_discover(gwion->plug, &arg->lib);
  shreduler_set_loop(gwion->vm->shreduler, arg->loop);
}

ANN m_bool gwion_audio(const Gwion gwion) {
  DriverInfo *di = gwion->di;
  // get driver from string.
  if(di->arg) {
    for(m_uint i = 0; i < map_size(&gwion->plug->drv); ++i) {
      const m_str name = (m_str)VKEY(&gwion->plug->drv, i);
      const size_t len = strlen(name);
      if(!strncmp(name, di->arg, len)) {
        di->func = (f_diset)VVAL(&gwion->plug->drv, i);
        break;
      }
    }
  }
  di->func(di->driver);
  VM* vm = gwion->vm;
  CHECK_BB(gwion->di->driver->ini(vm, di));
  vm->bbq = new_bbq(di);
  return GW_OK;
}

ANN m_bool gwion_engine(const Gwion gwion) {
  return type_engine_init(gwion->vm, &gwion->plug->vec[GWPLUG_IMPORT]) > 0;
}

ANN void gwion_run(const Gwion gwion) {
  VM* vm = gwion->vm;
  vm->is_running = 1;
  VMBENCH_INI
  gwion->di->driver->run(vm, gwion->di);
  VMBENCH_END
}

ANN static void free_driverinfo(DriverInfo* di, VM* vm) {
  if(di->driver->del)
    di->driver->del(vm, di);
  xfree(di->driver);
  xfree(di);
}
ANN void gwion_release(const Gwion gwion) {
  free_driverinfo(gwion->di, gwion->vm);
  plug_end(gwion);
  free_env(gwion->env);
  free_emitter(gwion->emit);
  free_vm(gwion->vm);
  xfree(gwion->plug);
  free_symbols();
}
