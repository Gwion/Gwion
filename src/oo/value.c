#include <stdlib.h>
#include "gwion_util.h"
#include "oo.h"
#include "env.h"
#include "value.h"
#include "type.h"

ANN Value new_value(const Type type, const m_str name) {
  const Value a = mp_alloc(Value);
  a->type       = type;
  a->name       = name;
  INIT_OO(a, e_value_obj);
  return a;
}

ANN void free_value(Value a) {
  if(!GET_FLAG(a, ae_flag_func) && !GET_FLAG(a, ae_flag_constprop) && a->d.ptr && isa(a->type, t_object) < 0 &&
      /*if(!GET_FLAG(a, ae_flag_func) && a->d.ptr && isa(a->type, t_object) < 0 &&*/
      !GET_FLAG(a, ae_flag_enum))
    free(a->d.ptr);
  if(isa(a->type, t_class) > 0 || isa(a->type, t_function) > 0)
    REM_REF(a->type)
  mp_free(Value, a);
}

