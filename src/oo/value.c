#include <stdlib.h>
#include "defs.h"
#include "oo.h"
#include "value.h"
#include "type.h"
#include "mpool.h"

POOL_HANDLE(Value, 2048)

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
  mp_free(Value, a);
}

