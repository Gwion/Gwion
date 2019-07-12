#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "vm.h"
#include "gwion.h"
#include "value.h"
#include "type.h"

ANN static void free_value(Value a, Gwion gwion) {
  const Type t = !GET_FLAG(a->type, nonnull) ? a->type : a->type->e->parent;
  if(!GET_FLAG(a, func) && a->d.ptr && !GET_FLAG(a, union) &&
      !(GET_FLAG(a, enum) && GET_FLAG(a, builtin) && a->owner_class)
      && isa(t, t_object) < 0)
   _mp_free(gwion->mp, t->size, a->d.ptr);
  if(isa(t, t_class) > 0/* || isa(a->type, t_function) > 0*/)
    REM_REF(t, gwion)
  mp_free(gwion->mp, Value, a);
}

ANN Value new_value(MemPool p, const Type type, const m_str name) {
  const Value a = mp_calloc(p, Value);
  a->type       = type;
  a->name       = name;
  a->ref = new_refcount(p, free_value);
  return a;
}
