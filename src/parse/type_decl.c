#include "defs.h"
#include "err_msg.h"
#include "absyn.h"
#include "type.h"

const m_bool prim_ref(const Type_Decl* td, const Type t) {
  if(isa(t, &t_object) < 0)
    CHECK_BB(err_msg(SCAN2_, td->pos,
          "cannot declare/instantiate references (@) of primitive type '%s'...\n"
          "\t...(primitive types: 'int', 'float', 'time', 'dur',\n"
          "\t... complex, polar, Vec3, Vec4)",
          t->name))
  return 1;
}

Type type_decl_resolve(Env env, const Type_Decl* td) {
  Type t = find_type(env, td->xid);
  CHECK_OO(t)
  CHECK_OO((t = scan_type(env, t, td)))
  if(td->array)
    CHECK_OO((t = get_array(t, td->array, "type decl resolve")))
  return t;
}
