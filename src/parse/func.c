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
  if(a->def && !GET_FLAG(a, ae_flag_template)) {
    if(a->code && !GET_FLAG(a->def, ae_flag_dtor))
        REM_REF(a->code);
    free_func_def(a->def);
  }
  free(a);
}

