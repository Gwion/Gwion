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
  func->obj = new_VM_Object(e_func_obj);
  func->variadic_start = NULL;
  return func;
}

void free_Func(Func a)
{
//  if(a->code && !a->def->is_template) {
  if(a->code) {
//printf("a->name %s\n", a->name);
//printf("%s\n", a->code->name);
    if(a->def && !a->def->is_template) {
      if(a->def->spec != ae_func_spec_dtor) {
        free_VM_Code(a->code);
      } 
else {
/*
//        free_VM_Code(a->code);
        free(a->code->name);
        free(a->code->filename);
        free_Vector(a->code->instr);
        free(a->code);
//        free(a->code);
        a->value_ref->owner->dtor = NULL;
//rem_ref(a->value_ref->m_type->obj, a->value_ref->m_type);
free(a->value_ref->m_type->name);
free(a->value_ref->m_type->obj);
free_Type(a->value_ref->m_type);
rem_ref(a->value_ref->obj, a->value_ref);
//rem_ref(a->value_ref->obj, a->value_ref);
free(a->name);
*/
      }
    }
  }
  /*
    if(a->def && a->def->is_template) {
  //    free(a->def);
      //    free(a->name);
      free_Func_Def(a->def);
    } else
  */
  if(a->def && !a->is_template) {
    free_Func_Def(a->def);
  }
  free(a);
}
