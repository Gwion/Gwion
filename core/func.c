#include <stdlib.h>
#include "map.h"
#include "func.h"
#include "oo.h"

Func new_func(m_str name, Func_Def def) {
  Func func = calloc(1, sizeof(struct Func_));
  func->name = name;
  func->def = def;
  INIT_OO(func, e_func_obj);
  return func;
}

void free_func(Func a) {
  if(a->code) {
    if(a->def && !a->def->is_template) {
      if(a->def->spec != ae_func_spec_dtor) {
        free_VM_Code(a->code);
      }
    }
  }
  if(a->def && !a->is_template)
    free_func_def(a->def);
  free(a);
}
