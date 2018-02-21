#include <stdlib.h>
#include "func.h"

Func new_func(m_str name, Func_Def def) {
  Func func = calloc(1, sizeof(struct Func_));
  func->name = name;
  func->def = def;
  INIT_OO(func, e_func_obj);
  return func;
}

void free_func(Func a) {
  if(GET_FLAG(a, ae_flag_ref)) {
    if(GET_FLAG(a, ae_flag_template)) {
      free_tmpl_list(a->def->tmpl);
      free(a->def);
    } //else if((m_uint)a->code > SZ_INT)
      //REM_REF(a->code)
//else printf("problem with code in %s\n", a->name);
  } else if(a->def && !GET_FLAG(a, ae_flag_template)) {
    if(a->code && !GET_FLAG(a->def, ae_flag_dtor))
        REM_REF(a->code);
    free_func_def(a->def);
  } else {
if(a->def)
    free_func_def(a->def);
if(a->code)
free_vm_code(a->code);
}
  free(a);
}

#include <string.h>
#include "env.h"
#include "type.h"
Func get_func(Env env, Func_Def def) {
  Func f = def->d.func;
  CHECK_OO(f)
  m_str end = strrchr(f->name, '@'); // test end cause some template func do not have @x@env->curr->name
  if(end && env->class_def && GET_FLAG(env->class_def, ae_flag_template)) {
    end++;
    size_t len = strlen(f->name) - strlen(end);
    char c[len + strlen(env->class_def->name) + 1];
    memset(c, 0, len + strlen(env->class_def->name) + 1);
    strncpy(c, f->name, len);
    strcat(c, env->class_def->name);
    f = nspc_lookup_func1(env->class_def->info, insert_symbol(c));
  }
  return f;
}
