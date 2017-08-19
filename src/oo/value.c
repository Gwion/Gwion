#include <stdlib.h>
#include "type.h"

Value new_value(Type type, m_str name) {
  Value a               = (Value)calloc(1, sizeof(struct Value_));
  a->m_type             = type;
  a->name               = name;
  INIT_OO(a, e_value_obj);
  return a;
}

void free_value(Value a) {
  if(a->ptr) {
    if(isprim(a->m_type) > 0 && !GET_FLAG(a, ae_flag_enum))
      free(a->ptr);
  }
  free(a);
}

