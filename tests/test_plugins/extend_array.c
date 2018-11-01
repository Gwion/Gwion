#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "type.h"
#include "instr.h"
#include "import.h"

GWION_IMPORT(extend_array_test) {
  Type t_array_ext;
  CHECK_OB((t_array_ext = gwi_mk_type(gwi, "ArrayExt", SZ_INT, NULL)))
  CHECK_BB(gwi_class_ini(gwi, t_array_ext, NULL, NULL))
  Type_Decl* td = new_type_decl(new_id_list(insert_symbol("float"), 0), 0);
  Exp e = new_exp_prim_int(1, 0);
  Array_Sub array = new_array_sub(e);
  add_type_decl_array(td, array);
  CHECK_BB(gwi_class_ext(gwi, td))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
