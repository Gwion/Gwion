#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "context.h"
#include "type.h"
#include "func.h"
#include "value.h"
#include "instr.h"
#include "emit.h"
#include "engine.h"
#include "driver.h"
#include "gwion.h"
#include "arg.h"
#include "compile.h"
#include "object.h" // fork_clean
#include "pass.h" // fork_clean
#include "specialid.h" // fork_clean

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
  gwion->type = src->gwion->type;
  return gwion->vm;
}

ANN m_bool gwion_ini(const Gwion gwion, Arg* arg) {
  gwion->mp = mempool_ini((sizeof(struct VM_Shred_) + SIZEOF_REG + SIZEOF_MEM));
  gwion->st = new_symbol_table(gwion->mp, 65347);
  gwion->vm = new_vm(gwion->mp, 1);
  gwion->emit = new_emitter(gwion->mp);
  gwion->env = new_env(gwion->mp);
  gwion->emit->env = gwion->env;
  gwion->emit->gwion = gwion;
  gwion->vm->gwion = gwion;
  gwion->env->gwion = gwion;
  gwion->vm->bbq->si = new_soundinfo(gwion->mp);
  gwion->data = new_gwiondata(gwion->mp);
  gwion->type = (Type*)xmalloc(MAX_TYPE * sizeof(struct Type_*));
  pass_default(gwion);
  arg->si = gwion->vm->bbq->si;
  const m_bool ret = arg_parse(gwion, arg);
  if(ret) {
    gwion->emit->info->memoize = arg->memoize;
    gwion->plug = new_plug(gwion->mp, &arg->lib);
    shreduler_set_loop(gwion->vm->shreduler, arg->loop);
    if(gwion_audio(gwion) > 0 && gwion_engine(gwion)) {
      gwion_compile(gwion, &arg->add);
      plug_run(gwion, &arg->mod);
      return GW_OK;
    }
  }
  return GW_ERROR;
}

ANN void gwion_run(const Gwion gwion) {
  VM* vm = gwion->vm;
  vm->bbq->driver->run(vm, vm->bbq);
}

ANN static void fork_clean2(const Vector v) {
  for(m_uint i = 0; i < vector_size(v); ++i) {
    VM* vm = (VM*)vector_at(v, i);
    const Gwion gwion = vm->gwion;
    free_vm(vm);
    mp_free(gwion->mp, Gwion, gwion);
  }
  vector_release(v);
}

ANN void gwion_end(const Gwion gwion) {
  const VM_Code code = new_vm_code(gwion->mp, NULL, 0, ae_flag_builtin, "in code dtor");
  gwion->vm->cleaner_shred = new_vm_shred(gwion->mp, code);
  vm_add_shred(gwion->vm, gwion->vm->cleaner_shred);
  MUTEX_LOCK(gwion->vm->shreduler->mutex);
  if(gwion->data->child.ptr)
    fork_clean(gwion->vm->cleaner_shred, &gwion->data->child);
  if(gwion->data->child2.ptr)
    fork_clean2(&gwion->data->child2);
  MUTEX_UNLOCK(gwion->vm->shreduler->mutex);
  free_env(gwion->env);
  free_vm_shred(gwion->vm->cleaner_shred);
  free_emitter(gwion->mp, gwion->emit);
  free_vm(gwion->vm);
  free_plug(gwion);
  free_gwiondata(gwion->mp, gwion->data);
  free_symbols(gwion->st);
  xfree(gwion->type);
  mempool_end(gwion->mp);
}

ANN void env_err(const Env env, const struct YYLTYPE* pos, const m_str fmt, ...) {
  if(env->context && env->context->error)
      return;
  if(env->class_def)
    gw_err(_("in class: '%s'\n"), env->class_def->name);
  if(env->func)
    gw_err(_("in function: '%s'\n"), env->func->name);
  loc_header(pos, env->name);
  va_list arg;
  va_start(arg, fmt);
  vfprintf(stderr, fmt, arg);
  va_end(arg);
  fprintf(stderr, "\n");
  loc_err(pos, env->name);
  if(env->context)
    env->context->error = 1;
}

ANN struct SpecialId_* specialid_get(const Gwion gwion, const Symbol sym) {
  const Map map = &gwion->data->id;
  for(m_uint i = 0; i < map_size(map); ++i) {
    if(sym == (Symbol)VKEY(map, i))
      return (struct SpecialId_*)VVAL(map, i);
  }
  return NULL;
}
