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

ANN m_bool gwion_audio(const Gwion gwion) {
  Driver* di = gwion->vm->bbq;
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
  CHECK_BB(di->driver->ini(gwion->vm, di));
  driver_alloc(di);
  return GW_OK;
}

ANN static inline m_bool gwion_engine(const Gwion gwion) {
  return type_engine_init(gwion->vm, &gwion->plug->vec[GWPLUG_IMPORT]) > 0;
}

ANN static inline void gwion_compile(const Gwion gwion, const Vector v) {
  for(m_uint i = 0; i < vector_size(v); i++)
    compile_filename(gwion, (m_str)vector_at(v, i));
}


ANN VM* gwion_cpy(const VM* src) {
  const Gwion gwion = mp_alloc(Gwion);
  gwion->vm = new_vm();
  gwion->vm->gwion = gwion;
  gwion->vm->bbq->si = soundinfo_cpy(src->bbq->si);
  gwion->emit = src->gwion->emit;
  gwion->env = src->gwion->env;
  gwion->freearg = src->gwion->freearg;
  return gwion->vm;
}
ANN m_bool gwion_ini(const Gwion gwion, Arg* arg) {
  gwion->vm = new_vm();
  gwion->emit = new_emitter();
  gwion->env = new_env();
  gwion->emit->env = gwion->env;
  gwion->vm->gwion = gwion;
  gwion->env->gwion = gwion;
  gwion->vm->bbq->si = mp_alloc(SoundInfo);
  gwion->vm->bbq->si->in = gwion->vm->bbq->si->out = 2;
  gwion->vm->bbq->si->sr = 48000;
  arg->si = gwion->vm->bbq->si;
  arg_parse(arg);
  gwion->plug = new_plug(&arg->lib);
  map_init(&gwion->freearg);
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

ANN void gwion_end(const Gwion gwion) {
  const VM_Code code = new_vm_code(NULL, 0, ae_flag_builtin, "in code dtor");
  gwion->vm->cleaner_shred = new_vm_shred(code);
  gwion->vm->cleaner_shred->info->vm = gwion->vm;
  free_env(gwion->env);
  free_vm_shred(gwion->vm->cleaner_shred);
  free_emitter(gwion->emit);
  free_vm(gwion->vm);
  free_plug(gwion);
  map_release(&gwion->freearg);
  free_symbols();
}
