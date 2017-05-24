#include <stdlib.h>
#include "map.h"
#include "func.h"
#include "oo.h"

Func new_Func(m_str name, Func_Def def)
{
  Func func = calloc(1, sizeof(struct Func_));
//  func->name = strdup(name);
  func->name = name;
  func->def = def;
  func->next = NULL;
  func->code = NULL;
  func->value_ref = NULL;
  func->up = NULL;
  func->is_member = 0;
  func->variadic_start = NULL;
  INIT_OO(func, e_func_obj);
  return func;
}

void free_Func(Func a)
{
  if(a->code) {
    if(a->def && !a->def->is_template) {
      if(a->def->spec != ae_func_spec_dtor) {
        free_VM_Code(a->code);
      }
    }
  }
  if(a->def && !a->is_template)
    free_Func_Def(a->def);
  free(a);
}
