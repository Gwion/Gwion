#include <unistd.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "driver.h"
#include "gwion.h"
#include "engine.h"
#include "arg.h"
#include "compile.h"
#include "object.h"
#include "pass.h"
#include "shreduler_private.h"
#include "ugen.h"

ANN m_bool gwion_audio(const Gwion gwion) {
  Driver *const di = gwion->vm->bbq;
  if (di->si->arg) CHECK_BB(driver_ini(gwion));
  di->func(di->driver);
  CHECK_BB(di->driver->ini(gwion->vm, di));
  driver_alloc(di);
  return GW_OK;
}

ANN static VM_Shred gwion_cleaner(const Gwion gwion) {
  const VM_Code code =
      new_vmcode(gwion->mp, NULL, NULL, "in code dtor", 0, true, false);
  const VM_Shred shred = new_vm_shred(gwion->mp, code);
  vm_ini_shred(gwion->vm, shred);
  return shred;
}

ANN void gwion_cpy_blackhole(const Gwion gwion) {
  dummy_driver(gwion->vm->bbq->driver);
  const M_Object o  = new_M_UGen(gwion);
  const UGen     u  = UGEN(o);
  ugen_ini(gwion, u, 1, 1);
  ugen_gen(gwion, u, compute_mono, o, 0);
  vector_add(&gwion->vm->ugen, (vtype)u);
}

ANN VM *gwion_cpy(const VM *src) {
  const Gwion gwion  = mp_calloc(src->gwion->mp, Gwion);
  gwion->vm          = new_vm(src->gwion->mp, true);
  gwion->vm->gwion   = gwion;
  gwion->vm->bbq->si = soundinfo_cpy(src->gwion->mp, src->bbq->si);
  gwion->emit        = src->gwion->emit;
  gwion->env         = src->gwion->env;
  gwion->data        = cpy_gwiondata(src->gwion->mp, src->gwion->data);
  gwion->st          = src->gwion->st;
  gwion->mp          = src->gwion->mp;
  gwion->type        = src->gwion->type;
  gwion_cpy_blackhole(gwion);
  return gwion->vm;
}

ANN static void gwion_core(const Gwion gwion) {
  gwion->vm        = new_vm(gwion->mp, true);
  gwion->emit      = new_emitter(gwion->mp);
  gwion->env       = new_env(gwion->mp);
  gwion->emit->env = gwion->env;
  gwion->vm->gwion = gwion->emit->gwion = gwion->env->gwion = gwion;
}


ANN static Func gwion_locale(const Gwion gwion) {
   const Nspc nspc = gwion->env->curr;
   const Symbol sym = insert_symbol(gwion->st, "BasicLocale");
   const Value v = nspc_lookup_value1(nspc, sym);
   return v->d.func_ref;
}

ANN static m_bool gwion_ok(const Gwion gwion, CliArg *arg) {
  CHECK_BB(plug_ini(gwion, &arg->lib));
  shreduler_set_loop(gwion->vm->shreduler, arg->loop);
  if (gwion_audio(gwion) > 0) {
    plug_run(gwion, &arg->mod);
    if (type_engine_init(gwion)) {
      vector_add(&gwion->data->plugs->vec, (m_uint)gwion->env->global_nspc);
      gwion->vm->cleaner_shred = gwion_cleaner(gwion);
      gwion->emit->locale = gwion_locale(gwion);
      (void)arg_compile(gwion, arg);
      return GW_OK;
    }
  }
  return GW_ERROR;
}

#define LOCALE_INFO INSTALL_PREFIX "/share"

ANN static void doc_mode(const Gwion gwion) {
  struct Vector_ v;
  vector_init(&v);
  vector_add(&v, (m_uint) "scan0");
  pass_set(gwion, &v);
  vector_release(&v);
}

ANN m_bool gwion_ini(const Gwion gwion, CliArg *arg) {
#ifdef USE_GETTEXT
  setlocale(LC_ALL, NULL);
  bindtextdomain(GWION_PACKAGE, LOCALE_INFO);
  bindtextdomain(GWION_PACKAGE "_util", LOCALE_INFO);
  bindtextdomain(GWION_PACKAGE "_ast", LOCALE_INFO);
#endif
  gwion->mp = mempool_ini((sizeof(struct VM_Shred_) + SIZEOF_REG + SIZEOF_MEM));
  gwion->st = new_symbol_table(gwion->mp, 65347);
  gwion->ppa = mp_calloc(gwion->mp, PPArg);
  pparg_ini(gwion->mp, gwion->ppa);
  gwion_core(gwion);
  gwion->data = new_gwiondata(gwion->mp);
  gwion->type = (Type *)xcalloc(MAX_TYPE, sizeof(struct Type_ *));
  arg->si = gwion->vm->bbq->si = new_soundinfo(gwion->mp);
  new_passes(gwion);
  CHECK_BB(arg_parse(gwion, arg));
  if (arg->color == COLOR_NEVER)
    tcol_override_color_checks(0);
  else if (arg->color == COLOR_AUTO)
    tcol_override_color_checks(isatty(1));
  else if (arg->color == COLOR_ALWAYS)
    tcol_override_color_checks(1);
  if (!gwion->data->cdoc)
    pass_default(gwion);
  else
    doc_mode(gwion);
  return !arg->quit ? gwion_ok(gwion, arg) : GW_ERROR;
}

ANN void gwion_run(const Gwion gwion) {
  VM *vm = gwion->vm;
  vm->bbq->driver->run(vm, vm->bbq);
}

ANN static inline void free_gwion_cpy(const Gwion gwion, const VM_Shred shred) {
  gwion_end_child(shred, gwion);
  const UGen u = (UGen)vector_front(&gwion->vm->ugen);
  M_Object blackhole = u->module.gen.data;
  release(blackhole, shred);
  free_vm(gwion->vm);
  free_gwiondata_cpy(gwion->mp, gwion->data);
  mp_free(gwion->mp, Gwion, gwion);
}

ANN static void fork_clean2(const VM_Shred shred, const Vector v) {
  for (m_uint i = 0; i < vector_size(v); ++i) {
    const Gwion gwion = (Gwion)vector_at(v, i);
    free_gwion_cpy(gwion, shred);
  }
  vector_release(v);
  v->ptr = NULL;
}

ANN void gwion_end_child(const VM_Shred shred, const Gwion gwion) {
  if (gwion->data->child.ptr) fork_clean(shred, &gwion->data->child);
  if (gwion->data->child2.ptr) fork_clean2(shred, &gwion->data->child2);
}

ANN void gwion_end(const Gwion gwion) {
  VM *vm = gwion->vm;
  vm_clean(vm, gwion);
  release_ctx(gwion->env->scope, gwion);
  if (gwion->data->plugs) free_plug(gwion);
  free_env(gwion->env);
  free_emitter(gwion->mp, gwion->emit);
  free_vm(vm);
  pparg_end(gwion->ppa);
  mp_free(gwion->mp, PPArg, gwion->ppa);
  free_gwiondata(gwion);
  free_symbols(gwion->st);
  xfree(gwion->type);
  mempool_end(gwion->mp);
}

ANN void env_error_footer(const Env env) {
  if (env->class_def && tflag(env->class_def, tflag_cdef))
    gwerr_secondary("in class", env->name, env->class_def->info->cdef->pos);
  if (env->func && env->func->def)
    gwerr_secondary("in function", env->name, env->func->def->base->pos);
}

ANN static void env_xxx(const Env env, const loc_t pos, const m_str fmt,
                        va_list arg) {
#ifndef __FUZZING__
  va_list tmpa;
  va_copy(tmpa, arg);
  const int size = vsnprintf(NULL, 0, fmt, tmpa);
  va_end(tmpa);
  char c[size + 1];
  vsprintf(c, fmt, arg);
  gwerr_basic(c, NULL, NULL, env->name, pos, 0);
  env_error_footer(env);
#endif
}

ANN static void _env_warn(const Env env, const loc_t pos, const m_str fmt,
                        va_list arg) {
#ifndef __FUZZING__
  va_list tmpa;
  va_copy(tmpa, arg);
  const int size = vsnprintf(NULL, 0, fmt, tmpa);
  va_end(tmpa);
  char c[size + 1];
  vsprintf(c, fmt, arg);
  gwerr_warn(c, NULL, NULL, env->name, pos);
  env_error_footer(env);
#endif
}

ANN void env_warn(const Env env, const loc_t pos, const m_str fmt, ...) {
#ifndef __FUZZING__
  va_list arg;
  va_start(arg, fmt);
  _env_warn(env, pos, fmt, arg);
  va_end(arg);
#endif
}

ANN void env_err(const Env env, const loc_t pos, const m_str fmt, ...) {
  if (env->context && env->context->error) return;
#ifndef __FUZZING__
  va_list arg;
  va_start(arg, fmt);
  env_xxx(env, pos, fmt, arg);
  va_end(arg);
#endif
  env_set_error(env, true);
}

ANN struct SpecialId_ *specialid_get(const Gwion gwion, const Symbol sym) {
  const Map map = &gwion->data->id;
  for (m_uint i = 0; i < map_size(map); ++i) {
    if (sym == (Symbol)VKEY(map, i)) return (struct SpecialId_ *)VVAL(map, i);
  }
  return NULL;
}

ANN void push_global(struct Gwion_ *gwion, const m_str name) {
  const Nspc nspc  = new_nspc(gwion->mp, name);
  nspc->parent     = gwion->env->global_nspc;
  gwion->env->curr = gwion->env->global_nspc = nspc;
}

ANN void pop_global(const Gwion gwion) {
   Nspc nspc = gwion->env->global_nspc, parent;
   do {
     parent = nspc->parent;
     nspc_remref(nspc, gwion);
   } while((nspc = parent));
}

ANN void gwion_set_debug(const Gwion gwion, const bool dbg) {
  gwion->emit->info->debug = dbg;
}
ANN void gwion_set_dump(const Gwion gwion, const bool dump) {
  gwion->emit->info->dump = dump;
}
ANN void gwion_set_cdoc(const Gwion gwion, const bool cdoc) {
  gwion->data->cdoc = cdoc;
}
