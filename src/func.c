#include <stdlib.h>
#include "map.h"
#include "func.h"
#include "oo.h"

Func new_Func(m_str name, Func_Def def)
{
  Func func = calloc(1, sizeof(struct Func_));
  func->name = name;
  func->def = def;
  func->next = NULL;
  func->code = NULL;
  func->value_ref = NULL;
  func->up = NULL;
  func->is_member = 0;
  func->obj = new_VM_Object(e_func_obj);
  func->variadic_start = NULL;
  return func;
}

void free_Func(Func a)
{
  free_Func_Def(a->def);
  free_VM_Code(a->code);
  free(a->name);
  free(a);
}
