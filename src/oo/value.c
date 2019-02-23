#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "value.h"
#include "type.h"

ANN static void free_value(Value a) {
  if(!GET_FLAG(a, func) && a->d.ptr &&
      !(GET_FLAG(a, enum) && GET_FLAG(a, builtin) && a->owner_class)
      && isa(a->type, t_object) < 0)
   _mp_free(a->type->size, a->d.ptr);
  if(isa(a->type, t_class) > 0 || isa(a->type, t_function) > 0 || GET_FLAG(a->type, op))
    REM_REF(a->type)
  mp_free(Value, a);
}

ANN Value new_value(struct Gwion_* gwion, const Type type, const m_str name) {
  const Value a = mp_alloc(Value);
  a->type       = type;
  a->name       = name;
  a->gwion = gwion;
  INIT_OO(a, free_value);
  return a;
}
