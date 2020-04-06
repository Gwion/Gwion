#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"

ANN static void free_func(Func a, Gwion gwion) {
//  if(GET_FLAG(a, template) && !GET_FLAG(a, builtin)/* && a->def->d.code*/)
  if(GET_FLAG(a, template))
    free_func_def(gwion->mp, a->def);
  if(a->code)
    REM_REF(a->code, gwion);
  mp_free(gwion->mp, Func, a);
}

ANN Func new_func(MemPool p, const m_str name, const Func_Def def) {
  Func func = mp_calloc(p, Func);
  func->name = name;
  func->def = def;
  func->ref = new_refcount(p, free_func);
  return func;
}

ANN2(1,2) Symbol func_symbol(const Env env, const m_str nspc, const m_str base,
    const m_str tmpl, const m_uint i) {
  const size_t base_len = strlen(base);
  const size_t tmpl_len = !tmpl ? 0 : strlen(tmpl) + 4;
  const size_t nspc_len = strlen(nspc);
  const size_t idx_len = num_digit(i);
  const size_t len = base_len + tmpl_len + nspc_len + idx_len + 2;
  char name[len + 1];
  CHECK_BO(sprintf(name, "%s%s%s%s@%" UINT_F "@%s",
    base, !tmpl ? "" : "<~", !tmpl ? "" : tmpl, !tmpl ? "" : "~>",
    i, nspc))
  return insert_symbol(env->gwion->st, name);
}
