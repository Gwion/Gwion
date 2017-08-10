#include <stdlib.h>
#include "type.h"

Type new_type(te_type xid, m_str name, Type parent) {
  Type type    = calloc(1, sizeof(struct Type_));
  type->xid    = xid;
  type->name   = name;
  type->parent = parent;
  INIT_OO(type, e_type_obj);
  return type;
}

void free_type(Type a) {
  if(a->info)
    REM_REF(a->info);
  if(!GET_FLAG(a, ae_flag_builtin) || a->parent == &t_int || isa(a, &t_class) > 0
      || isa(a, &t_function) > 0 || a->array_depth)
    free(a);
}

Type type_copy(Env env, Type type) {
  Type a         = calloc(1, sizeof(struct Type_));
  a->xid         = type->xid;
  a->name        = type->name;
  a->parent      = type->parent;
  a->info        = type->info;
  a->owner       = type->owner;
  a->size        = type->size;
  a->d.actual_type = type->d.actual_type;
  a->array_depth = type->array_depth;
  if(GET_FLAG(type, ae_flag_checked))
    SET_FLAG(a, ae_flag_checked);
  a->def         = type->def;
  INIT_OO(a, e_type_obj);
  return a;
}

