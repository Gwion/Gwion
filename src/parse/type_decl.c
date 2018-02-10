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
