#include <stdlib.h>
#include "absyn.h"
#include "type.h"
#include "func.h"
#include "mpool.h"

POOL_HANDLE(Func, 1024)

ANN Func new_func(const m_str name, const Func_Def def) {
  Func func = mp_alloc(Func);
  func->name = name;
  func->def = def;
  INIT_OO(func, e_func_obj);
  return func;
}
Func new_func_simple() { return mp_alloc(Func); }

ANN void free_func(Func a) {
  if(GET_FLAG(a, ae_flag_ref)) {
    if(GET_FLAG(a, ae_flag_template)) {
      free_tmpl_list(a->def->tmpl);
      free_func_def_simple(a->def);
    }
  } else if(a->def)
    free_func_def(a->def);
  if(a->code)
    REM_REF(a->code);
  mp_free(Func, a);
}
void free_func_simple(Func a) { mp_free(Func, a); }

#include <string.h>
#include "env.h"
#include "type.h"
ANN Func get_func(const Env env, const Func_Def def) {
  Func f = def->func;
  CHECK_OO(f)
  m_str end = strrchr(f->name, '@'); // test end cause some template func do not have @x@env->curr->name
  if(end && GET_FLAG(f, ae_flag_ref)) {
    ++end;
    const size_t len = strlen(f->name) - strlen(end);
    char c[len + strlen(env->class_def->name) + 1];
    memset(c, 0, len + strlen(env->class_def->name) + 1);
    strncpy(c, f->name, len);
    strcat(c, env->class_def->name);
    f = nspc_lookup_func1(env->class_def->nspc, insert_symbol(c));
  }
  return f;
}
