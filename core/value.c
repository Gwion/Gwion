#include <stdlib.h>
#include <complex.h>
#include "type.h"

Value new_Value(Type type, m_str name) {
  Value a               = (Value)calloc(1, sizeof(struct Value_));
  a->m_type             = type;
  a->name               = name;
  INIT_OO(a, e_value_obj);
  return a;
}
void free_Value(Value a) {
  if(a->ptr) {
    if(isprim(a->m_type) > 0)
      free(a->ptr);
  }
  free(a);
}
