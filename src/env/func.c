#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "clean.h"

ANN void free_func(Func a, Gwion gwion) {
  if (fflag(a, fflag_tmpl)) func_def_cleaner(gwion, a->def);
  if (a->code) vmcode_remref(a->code, gwion);
  if (a->_wait) free_mp_vector(gwion->mp, Value, a->_wait);
  mp_free(gwion->mp, Func, a);
}

ANN Func new_func(MemPool p, const m_str name, const Func_Def def) {
  Func func         = mp_calloc(p, Func);
  func->name        = name;
  func->def         = def;
  func->inline_mult = 1.0;
  func->ref         = 1;
  return func;
}

ANN2(1, 2)
Symbol func_symbol(const Env env, const m_str nspc, const m_str base,
                   const m_str tmpl, const m_uint i) {
  const size_t base_len = strlen(base);
  const size_t tmpl_len = !tmpl ? 0 : strlen(tmpl) + 4;
  const size_t nspc_len = strlen(nspc);
  const size_t idx_len  = num_digit(i);
  const size_t len      = base_len + tmpl_len + nspc_len + idx_len + 2;
  char         name[len + 1];
  CHECK_BO(sprintf(name, "%s%s%s%s@%" UINT_F "@%s", base, !tmpl ? "" : ":[",
                   !tmpl ? "" : tmpl, !tmpl ? "" : "]", i, nspc));
  return insert_symbol(env->gwion->st, name);
}

ANN void builtin_func(const Gwion gwion, const Func f, void *func_ptr) {
  set_vflag(f->value_ref, vflag_builtin);
  f->code = new_vmcode(gwion->mp, NULL, NULL, f->name, f->def->stack_depth, true, false);
  f->code->native_func = (m_uint)func_ptr;
  f->code->ret_type = f->def->base->ret_type;
  if(f->def->base->tmpl && f->def->base->tmpl->call) {
    const Specialized *spec = mp_vector_at(f->def->base->tmpl->list, Specialized, f->def->base->tmpl->list->len - 1);
    if(!strcmp(s_name(spec->xid), "...")) {
      f->code->types = new_mp_vector(gwion->mp, Type_Decl*, f->def->base->tmpl->call->len);
      for(uint32_t i = 0; i < f->def->base->tmpl->call->len; i++)  {
        Type_Decl *const td = *mp_vector_at(f->def->base->tmpl->call, Type_Decl*, i);
        mp_vector_set(f->code->types, Type, i, known_type(gwion->env, td));
      }
    }
  }
}
