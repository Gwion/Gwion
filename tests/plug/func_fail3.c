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

GWION_IMPORT(func_fail3) {
  GWI_B(gwi_func_ini(gwi, "Ptr:[int]", "test:[A]"))
  GWI_B(gwi_func_ini(gwi, "int", "test"))
  GWI_B(gwi_func_arg(gwi, "int", "i[][]"))
  GWI_B(gwi_func_end(gwi, (f_xfun)1, ae_flag_none))
  return GW_OK;
}
