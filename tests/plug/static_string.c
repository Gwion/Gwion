#include <stdlib.h>
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

GWION_IMPORT(static_string_test) {
  const M_Object o =
      new_object(gwi->gwion->mp, NULL, gwi->gwion->type[et_string]);
  STRING(o) = s_name(insert_symbol(gwi->gwion->st, "test static string"));
  GWI_BB(gwi_item_ini(gwi, "string", "self"))
  GWI_BB(gwi_item_end(gwi, ae_flag_global, obj, o))
  return GW_OK;
}
