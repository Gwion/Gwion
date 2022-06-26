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
  else if (vflag(a, vflag_inner))
    type_remref(t, gwion);

  mp_free(gwion->mp, ValueFrom, a->from);
  mp_free(gwion->mp, Value, a);
}

ANN Value new_value(const Env env, const Type type, const m_str name, const loc_t loc) {
  const Value a     = mp_calloc(env->gwion->mp, Value);
  a->from           = mp_calloc(env->gwion->mp, ValueFrom);
  a->type           = type;
  a->name           = name;
  a->ref            = 1;
  a->from->filename = env->name;
  a->from->loc      = loc;
  return a;
}

ANN void valuefrom(const Env env, ValueFrom *from) {
  from->owner       = env->curr;
  from->owner_class = env->scope->depth ? NULL : env->class_def;
  from->ctx         = env->context;
}
