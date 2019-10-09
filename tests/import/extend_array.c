#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "instr.h"
#include "vm.h"
#include "gwion.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

GWION_IMPORT(extend_array_test) {
  Type t_array_ext;
  CHECK_OB((t_array_ext = gwi_mk_type(gwi, "ArrayExt", SZ_INT, NULL)))
  CHECK_BB(gwi_class_ini(gwi, t_array_ext, NULL, NULL))
  Type_Decl* td = new_type_decl(gwi->gwion->st->p, new_id_list(gwi->gwion->st->p, insert_symbol(gwi->gwion->st, "float"), GWI_LOC));
  Exp e = new_exp_prim_int(gwi->gwion->st->p, 1, GWI_LOC);
  Array_Sub array = new_array_sub(gwi->gwion->st->p, e);
  add_type_decl_array(td, array);
  CHECK_BB(gwi_class_ext(gwi, td))
  CHECK_BB(gwi_class_end(gwi))
  return GW_OK;
}
