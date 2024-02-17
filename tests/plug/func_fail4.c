#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "instr.h"

GWION_IMPORT(func_fail4) {
  GWI_B(gwi_func_ini(gwi, "Ptr:[int]", "test:[A]"))
  GWI_B(gwi_func_arg(gwi, "int", "i[][]"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  return GW_OK;
}
