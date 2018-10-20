#include "type.h"
#include "mpool.h"

ANN2(2) Type new_type(const m_uint xid, const m_str name, const Type parent) {
  const Type type = mp_alloc(Type);
  type->xid    = xid;
  type->name   = name;
  type->parent = parent;
  INIT_OO(type, e_type_obj);
  return type;
}

ANN void free_type(Type a) {
  if(GET_FLAG(a, ae_flag_template))
    free_class_def(a->def);
  if(a->nspc)
    REM_REF(a->nspc);
  mp_free(Type, a);
}

ANN Type type_copy(const Type type) {
  const Type a = new_type(type->xid, type->name, type->parent);
  a->nspc          = type->nspc;
  a->owner         = type->owner;
  a->size          = type->size;
  a->d.base_type   = type->d.base_type;
  a->array_depth   = type->array_depth;
  a->def           = type->def;
  return a;
}
