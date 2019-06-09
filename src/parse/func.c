#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "gwion.h"
#include "type.h"
#include "nspc.h"
#include "func.h"
#include "value.h"

ANN static void free_func(Func a, Gwion gwion) {
  if(GET_FLAG(a, template) && isa(actual_type(a->value_ref->type), t_fptr) < 0) {
    free_tmpl(gwion->mp, a->def->base->tmpl);
    free_func_base(gwion->mp, a->def->base);
    free_loc(gwion->mp, a->def->pos);
    mp_free(gwion->mp, Func_Def, a->def);
  }
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

#include <string.h>
#include "env.h"
#include "type.h"
#include "vm.h"
#include "gwion.h"
ANN Func get_func(const Env env, const Func_Def def) {
  DECL_OO(Func, f, = def->base->func)
  m_str end = strrchr(f->name, '@'); // test end cause some template func do not have @x@env->curr->name
  if(end && env->class_def && GET_FLAG(env->class_def, template)) {
    ++end;
    const size_t len = strlen(f->name) - strlen(end);
    const size_t elen = strlen(env->class_def->name);
    char c[len + elen + 1];
    memcpy(c, f->name, len);
    strcpy(c + len, env->class_def->name);
    return nspc_lookup_func1(env->class_def->nspc, insert_symbol(env->gwion->st, c));
  }
  return f;
}

ANN2(1,2) Symbol func_symbol(const Env env, const m_str nspc, const m_str base,
    const m_str tmpl, const m_uint i) {
  char* name;
  CHECK_BO(asprintf(&name, "%s%s%s%s@%" UINT_F "@%s",
    base, !tmpl ? "" : "<", !tmpl ? "" : tmpl, !tmpl ? "" : ">",
    i, nspc))
  const Symbol sym = insert_symbol(env->gwion->st, name);
  free(name);
  return sym;
}
