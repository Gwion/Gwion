#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "func.h"
#include "value.h"
#include "instr.h"
#include "emit.h"
#include "engine.h"
#include "driver.h"
#include "arg.h"
#include "gwion.h"
#include "compile.h"
#include "object.h" // fork_clean

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
  const Gwion gwion = mp_calloc(src->gwion->mp, Gwion);
  gwion->vm = new_vm(src->gwion->mp, 0);
  gwion->vm->gwion = gwion;
  gwion->vm->bbq->si = soundinfo_cpy(src->gwion->mp, src->bbq->si);
  gwion->emit = src->gwion->emit;
  gwion->env = src->gwion->env;
  gwion->data = src->gwion->data;
  gwion->st = src->gwion->st;
  gwion->mp = src->gwion->mp;
  return gwion->vm;
}

ANN m_bool gwion_ini(const Gwion gwion, Arg* arg) {
  gwion->mp = mempool_ini((sizeof(VM_Shred) + SIZEOF_REG + SIZEOF_MEM) / SZ_INT);
  gwion->st = new_symbol_table(gwion->mp, 65347);
  gwion->vm = new_vm(gwion->mp, 1);
  gwion->emit = new_emitter();
  gwion->env = new_env(gwion->mp);
  gwion->emit->env = gwion->env;
  gwion->emit->gwion = gwion;
  gwion->vm->gwion = gwion;
  gwion->env->gwion = gwion;
  gwion->vm->bbq->si = new_soundinfo(gwion->mp);
  arg->si = gwion->vm->bbq->si;
  arg_parse(arg);
  gwion->emit->memoize = arg->memoize;
  gwion->plug = new_plug(gwion->mp, &arg->lib);
  gwion->data = new_gwiondata(gwion->mp);
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
  vm->bbq->driver->run(vm, vm->bbq);
}

ANN void gwion_end(const Gwion gwion) {
  const VM_Code code = new_vm_code(gwion->mp, NULL, 0, ae_flag_builtin, "in code dtor");
  gwion->vm->cleaner_shred = new_vm_shred(gwion->mp, code);
  vm_add_shred(gwion->vm, gwion->vm->cleaner_shred);
  MUTEX_LOCK(gwion->vm->shreduler->mutex);
  if(gwion->data->child.ptr)
    fork_clean(gwion->vm, &gwion->data->child);
  MUTEX_UNLOCK(gwion->vm->shreduler->mutex);
  free_env(gwion->env);
  free_vm_shred(gwion->vm->cleaner_shred);
  free_emitter(gwion->emit);
  free_vm(gwion->vm);
  free_plug(gwion);
  free_gwiondata(gwion->mp, gwion->data);
  free_symbols(gwion->st);
  mempool_end(gwion->mp);
}

ANN void env_err(const Env env, const struct YYLTYPE* pos, const m_str fmt, ...) {
  loc_err(pos, env->name);
  if(env->class_def)
    gw_err("in class: '%s'\n", env->class_def->name);
  if(env->func)
    gw_err("in function: '%s'\n", env->func->name);
  va_list arg;
  va_start(arg, fmt);
  vfprintf(stderr, fmt, arg);
  va_end(arg);
  fprintf(stderr, "\n");
}
