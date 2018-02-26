#include <stdlib.h>
#include "type.h"

Type new_type(m_uint xid, m_str name, Type parent) {
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
  if(GET_FLAG(a, ae_flag_builtin)) {
    if(a->parent && a->parent->array_depth)
      REM_REF(a->parent)
    if(GET_FLAG(a, ae_flag_template))
      free(a);
  } else {
    if(GET_FLAG(a, ae_flag_typedef) && GET_FLAG(a->parent, ae_flag_typedef))
      free_class_def(a->parent->def);
    free(a);
  }
}

Type type_copy(Type type) {
  Type a = new_type(type->xid, type->name, type->parent);
  a->info          = type->info;
  a->owner         = type->owner;
  a->size          = type->size;
  a->d.base_type = type->d.base_type;
  a->array_depth   = type->array_depth;
  a->def           = type->def;
  return a;
}
