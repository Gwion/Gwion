#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"

#define MAX(a, b) (a >= b ? a : b)
ANN void free_value(Value a, Gwion gwion) {
  const Type t = a->type;
  if (t->size > SZ_INT && !vflag(a, vflag_func) && a->d.ptr)
    _mp_free(gwion->mp, t->size, a->d.ptr);
  else if (is_class(gwion, t))
    type_remref(t, gwion);
  /* else */if (vflag(a, vflag_inner))
    type_remref(t, gwion);
  mp_free(gwion->mp, ValueFrom, a->from);
  mp_free(gwion->mp, Value, a);
}

ANN Value new_value(MemPool p, const Type type, const m_str name) {
  const Value a = mp_calloc(p, Value);
  a->from       = mp_calloc(p, ValueFrom);
  a->type       = type;
  a->name       = name;
  a->ref        = 1;
  return a;
}

ANN void valuefrom(const Env env, struct ValueFrom_ *from, const loc_t loc) {
  from->owner       = env->curr;
  from->owner_class = env->class_def;
  from->ctx         = env->context;
  from->filename    = env->name;
  from->loc         = loc;
}
