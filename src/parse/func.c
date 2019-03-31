#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "nspc.h"
#include "func.h"

ANN static void free_func(Func a, void *gwion) {
  if(GET_FLAG(a, template)) {
    free_tmpl_list(a->def->tmpl);
    mp_free(Func_Def, a->def);
  }
  if(a->code)
    REM_REF(a->code, gwion);
  mp_free(Func, a);
}

ANN Func new_func(const m_str name, const Func_Def def) {
  Func func = mp_alloc(Func);
  func->name = name;
  func->def = def;
  INIT_OO(func, free_func);
  return func;
}

#include <string.h>
#include "env.h"
#include "type.h"
ANN Func get_func(const Env env, const Func_Def def) {
  Func f = def->func;
  CHECK_OO(f)
  m_str end = strrchr(f->name, '@'); // test end cause some template func do not have @x@env->curr->name
  if(end && env->class_def && GET_FLAG(env->class_def, template)) {
    ++end;
    const size_t len = strlen(f->name) - strlen(end);
    const size_t elen = strlen(env->class_def->name);
    char c[len + elen + 1];
    memcpy(c, f->name, len);
    strcpy(c + len, env->class_def->name);
    return nspc_lookup_func1(env->class_def->nspc, insert_symbol(c));
  }
  return f;
}

ANN2(1,2) Symbol func_symbol(const m_str nspc, const m_str base,
    const m_str tmpl, const m_uint i) {
  char* name;
  CHECK_BO(asprintf(&name, "%s%s%s%s@%" UINT_F "@%s",
    base, !tmpl ? "" : "<", !tmpl ? "" : tmpl, !tmpl ? "" : ">",
    i, nspc))
  const Symbol sym = insert_symbol(name);
  free(name);
  return sym;
}
