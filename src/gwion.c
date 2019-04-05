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

#include "shreduler_private.h"
ANN VM* gwion_cpy(const VM* src) {
  const Gwion gwion = mp_alloc(src->gwion->p, Gwion);
  gwion->vm = new_vm(src->gwion->p);
  gwion->vm->gwion = gwion;
//gwion->vm->shreduler->bbq->is_running = 1;
  gwion->vm->bbq->si = soundinfo_cpy(src->gwion->p, src->bbq->si);
  gwion->emit = src->gwion->emit;
  gwion->env = src->gwion->env;
  gwion->freearg = src->gwion->freearg;
  gwion->st = src->gwion->st;
  gwion->p = src->gwion->p;
  return gwion->vm;
}
ANN m_bool gwion_ini(const Gwion gwion, Arg* arg) {
  gwion->p = mempool_ini((sizeof(VM_Shred) + SIZEOF_REG + SIZEOF_MEM) / SZ_INT);
  gwion->st = new_symbol_table(gwion->p, 65347);
  gwion->vm = new_vm(gwion->p);
printf("non fork vm: %p\n", gwion->vm->shreduler);
  gwion->emit = new_emitter();
  gwion->env = new_env(gwion->p);
  gwion->emit->env = gwion->env;
  gwion->emit->gwion = gwion;
  gwion->vm->gwion = gwion;
  gwion->env->gwion = gwion;
  gwion->vm->bbq->si = new_soundinfo(gwion->p);
  arg->si = gwion->vm->bbq->si;
  arg_parse(arg);
  gwion->plug = new_plug(gwion->p, &arg->lib);
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
  const VM_Code code = new_vm_code(gwion->p, NULL, 0, ae_flag_builtin, "in code dtor");
  gwion->vm->cleaner_shred = new_vm_shred(gwion->p, code);
  vm_add_shred(gwion->vm, gwion->vm->cleaner_shred);
  free_env(gwion->env);
  free_vm_shred(gwion->vm->cleaner_shred);
  free_emitter(gwion->emit);
  free_vm(gwion->vm);
  free_plug(gwion);
  map_release(&gwion->freearg);
  free_symbols(gwion->st);
  mempool_end(gwion->p);
}
