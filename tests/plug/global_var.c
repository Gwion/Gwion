#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

GWION_IMPORT(global_var_test) {
  const M_Object o =
      new_object(gwi->gwion->mp, NULL, gwi->gwion->type[et_string]);
  STRING(o) = s_name(insert_symbol(gwi->gwion->st, "test"));
  GWI_BB(gwi_item_ini(gwi, "string", "i"))
  GWI_BB(gwi_item_end(gwi, 0, obj, o))
  GWI_BB(gwi_item_ini(gwi, "float", "f"))
  GWI_BB(gwi_item_end(gwi, 0, fnum, 2.1))
  return GW_OK;
}
