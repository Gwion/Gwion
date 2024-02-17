#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

static MFUN(test_mfun) {}
GWION_IMPORT(invalid_array) {
  GWI_OB(gwi_class_ini(gwi, "InvalidArray", NULL))
  GWI_B(gwi_func_ini(gwi, "int[]", "func"))
  GWI_B(gwi_func_arg(gwi, "int[][]", "i"))
  GWI_B(gwi_func_arg(gwi, "int", "j[]"))
  GWI_B(gwi_func_arg(gwi, "int[]", "k[]"))
  GWI_B(gwi_func_arg(gwi, "int", "l"))
  GWI_B(gwi_func_end(gwi, test_mfun, ae_flag_static))

  GWI_B(gwi_func_ini(gwi, "int[]", "func"))
  GWI_B(gwi_func_arg(gwi, "int", "j[][]"))
  GWI_B(gwi_func_arg(gwi, "int[]", "+k[][][]"))
  GWI_B(gwi_func_arg(gwi, "int", "l"))
  GWI_B(gwi_func_end(gwi, test_mfun, ae_flag_static))

  GWI_B(gwi_func_ini(gwi, "int[]", "func"))
  GWI_B(gwi_func_arg(gwi, "+int", "j[][]"))
  GWI_B(gwi_func_arg(gwi, "int[]", "+k[][][]"))
  GWI_B(gwi_func_end(gwi, test_mfun, ae_flag_static))

  GWI_B(gwi_class_end(gwi))
  return GW_OK;
}
