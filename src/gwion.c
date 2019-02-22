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
  plug_ini(gwion->plug, args);
}

ANN m_bool gwion_audio(const Gwion gwion, DriverInfo* di) {
  di->func(gwion->driver);
  VM* vm = gwion->vm;
  return gwion->driver->ini(vm, di) > 0 &&
    (vm->bbq = new_bbq(di));
}

ANN m_bool gwion_engine(const Gwion gwion) {
  return type_engine_init(gwion->vm, &gwion->plug[GWPLUG_IMPORT]) > 0;
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
  free_symbols();
}
