#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "engine.h"
#include "driver.h"
#include "gwion.h"
#include "arg.h"
#include "compile.h"
#include "object.h" // fork_clean
#include "pass.h" // fork_clean
#include "shreduler_private.h"

ANN static void driver_arg(const Gwion gwion, Driver *di) {
  for(m_uint i = 0; i < map_size(&gwion->data->plug->drv); ++i) {
    const m_str name = (m_str)VKEY(&gwion->data->plug->drv, i);
    const size_t len = strlen(name);
    if(!strncmp(name, di->si->arg, len)) {
      di->func = (f_bbqset)VVAL(&gwion->data->plug->drv, i);
      break;
    }
  }
}

ANN m_bool gwion_audio(const Gwion gwion) {
  Driver *const di = gwion->vm->bbq;
  if(di->si->arg)
    driver_arg(gwion, di);
  di->func(di->driver);
  CHECK_BB(di->driver->ini(gwion->vm, di));
  driver_alloc(di);
  return GW_OK;
}

ANN static inline m_bool gwion_engine(const Gwion gwion) {
  return type_engine_init(gwion->vm, &gwion->data->plug->vec[GWPLUG_IMPORT]) > 0;
}

ANN static inline void gwion_compile(const Gwion gwion, const Vector v) {
  for(m_uint i = 0; i < vector_size(v); i++)
    compile_filename(gwion, (m_str)vector_at(v, i));
}

ANN static void gwion_cleaner(const Gwion gwion) {
  const VM_Code code = new_vm_code(gwion->mp, NULL, 0, ae_flag_builtin, "in code dtor");
  gwion->vm->cleaner_shred = new_vm_shred(gwion->mp, code);
  vm_ini_shred(gwion->vm, gwion->vm->cleaner_shred);
}

ANN VM* gwion_cpy(const VM* src) {
  const Gwion gwion = mp_calloc(src->gwion->mp, Gwion);
  gwion->vm = new_vm(src->gwion->mp, 0);
  gwion->vm->gwion = gwion;
  gwion->vm->bbq->si = soundinfo_cpy(src->gwion->mp, src->bbq->si);
  gwion->emit = src->gwion->emit;
  gwion->env = src->gwion->env;
  gwion->data = cpy_gwiondata(src->gwion->mp, src->gwion->data);
  gwion->st = src->gwion->st;
  gwion->mp = src->gwion->mp;
  gwion->type = src->gwion->type;
  return gwion->vm;
}

ANN static void gwion_core(const Gwion gwion) {
  gwion->vm = new_vm(gwion->mp, 1);
  gwion->emit = new_emitter(gwion->mp);
  gwion->env = new_env(gwion->mp);
  gwion->emit->env = gwion->env;
  gwion->vm->gwion = gwion->emit->gwion = gwion->env->gwion = gwion;
}

ANN static m_bool gwion_ok(const Gwion gwion, Arg* arg) {
  gwion->data->plug = new_pluginfo(gwion->mp, &arg->lib);
  shreduler_set_loop(gwion->vm->shreduler, arg->loop);
  if(gwion_audio(gwion) > 0 && gwion_engine(gwion)) {
    gwion_cleaner(gwion);
    plug_run(gwion, &arg->mod);
    gwion_compile(gwion, &arg->add);
    return GW_OK;
  }
  return GW_ERROR;
}

ANN m_bool gwion_ini(const Gwion gwion, Arg* arg) {
  gwion->mp = mempool_ini((sizeof(struct VM_Shred_) + SIZEOF_REG + SIZEOF_MEM));
  gwion->st = new_symbol_table(gwion->mp, 65347);
  gwion->ppa = mp_calloc(gwion->mp, PPArg);
  pparg_ini(gwion->mp, gwion->ppa);
  gwion_core(gwion);
  gwion->data = new_gwiondata(gwion->mp);
  gwion->type = (Type*)xcalloc(MAX_TYPE, sizeof(struct Type_*));
  pass_default(gwion);
  arg->si = gwion->vm->bbq->si = new_soundinfo(gwion->mp);
  CHECK_BB(arg_parse(gwion, arg))
  return gwion_ok(gwion, arg);
}

ANN void gwion_run(const Gwion gwion) {
  VM* vm = gwion->vm;
  vm->bbq->driver->run(vm, vm->bbq);
}

ANN static inline void free_gwion_cpy(const Gwion gwion, const VM_Shred shred) {
  gwion_end_child(shred, gwion);
  free_vm(gwion->vm);
  free_gwiondata_cpy(gwion->mp, gwion->data);
  mp_free(gwion->mp, Gwion, gwion);
}

ANN static void fork_clean2(const VM_Shred shred, const Vector v) {
  for(m_uint i = 0; i < vector_size(v); ++i) {
    const Gwion gwion = (Gwion)vector_at(v, i);
    free_gwion_cpy(gwion, shred);
  }
  vector_release(v);
  v->ptr = NULL;
}

ANN void gwion_end_child(const VM_Shred shred, const Gwion gwion) {
  if(gwion->data->child.ptr)
    fork_clean(shred, &gwion->data->child);
  if(gwion->data->child2.ptr)
    fork_clean2(shred, &gwion->data->child2);
}

ANN void gwion_end(const Gwion gwion) {
  gwion_end_child(gwion->vm->cleaner_shred, gwion);
  free_env(gwion->env);
  if(gwion->vm->cleaner_shred)
    free_vm_shred(gwion->vm->cleaner_shred);
  free_emitter(gwion->mp, gwion->emit);
  free_vm(gwion->vm);
  pparg_end(gwion->ppa);
  mp_free(gwion->mp, PPArg, gwion->ppa);
  free_gwiondata(gwion);
  free_symbols(gwion->st);
  xfree(gwion->type);
  mempool_end(gwion->mp);
}

ANN static void env_header(const Env env) {
  if(env->class_def)
    gw_err(_("in class: '%s'\n"), env->class_def->name);
  if(env->func)
    gw_err(_("in function: '%s'\n"), env->func->name);
}

ANN void env_err(const Env env, const loc_t pos, const m_str fmt, ...) {
  if(env->context && env->context->error)
      return;
  env_header(env);
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
