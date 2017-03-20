#include <stdlib.h>
#include <complex.h>
#include "type.h"

Value new_Value(Context context, Type type, m_str name)
{
  Value a               = (Value)calloc(1, sizeof(struct Value_));
  a->type               = 0;
  a->m_type             = type;
  a->name               = name;
  a->ptr                = NULL;
  a->func_ref           = NULL;
  a->offset             = 0;
  a->func_num_overloads = 0;
  a->checked            = 0;
  a->is_const           = 0;
  a->is_static          = 0;
  a->is_context_global  = 0;
  a->owner              = NULL;
  a->owner_class        = NULL;
  a->obj                = new_VM_Object(e_value_obj);
  return a;
}
void free_Value(Value a)
{
  if(a->ptr) {
    if(isprim(a->m_type) > 0)
      free(a->ptr);
  }
  free(a);
}
