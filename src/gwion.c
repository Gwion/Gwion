#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "instr.h"
#include "emit.h"
#include "engine.h"
#include "gwion.h"
#include "arg.h"


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

ANN void gwion_init(const Gwion gwion, const Vector args) {
  gwion->vm = new_vm();
  gwion->emit = new_emitter();
  gwion->env = new_env();
  gwion->emit->env = gwion->env;
  gwion->vm->gwion = gwion;
  gwion->env->gwion = gwion;
  gwion->driver = (Driver*)xcalloc(1, sizeof(Driver));
  gwion->plug = (PlugInfo*)xmalloc(sizeof(PlugInfo));
  plug_discover(gwion->plug, args);
}

ANN m_bool gwion_audio(const Gwion gwion, DriverInfo* di) {
  // get driver from string.
  if(di->arg) {
    for(m_uint i = 0; i < map_size(&gwion->plug->drv); ++i) {
      const m_str name = (m_str)VKEY(&gwion->plug->drv, i);
      const size_t len = strlen(name);
      if(!strncmp(name, di->arg, len)) {
        di->func = (void (*)(struct driver_wrapper *))VVAL(&gwion->plug->drv, i);
        break;
      }
    }
  }
  di->func(gwion->driver);
  VM* vm = gwion->vm;
  return gwion->driver->ini(vm, di) > 0 &&
    (vm->bbq = new_bbq(di));
}

ANN m_bool gwion_engine(const Gwion gwion) {
  return type_engine_init(gwion->vm, &gwion->plug->vec[GWPLUG_IMPORT]) > 0;
}

ANN void gwion_run(const Gwion gwion, DriverInfo* di) {
  VM* vm = gwion->vm;
  vm->is_running = 1;
  VMBENCH_INI
  gwion->driver->run(vm, di);
  VMBENCH_END
}

ANN void gwion_release(const Gwion gwion, DriverInfo* di) {
  if(gwion->driver->del)
    gwion->driver->del(gwion->vm, di);
  xfree(gwion->driver);
  plug_end(gwion);
  free_env(gwion->env);
  free_emitter(gwion->emit);
  free_vm(gwion->vm);
  xfree(gwion->plug);
  free_symbols();
}
