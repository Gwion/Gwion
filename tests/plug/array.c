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

MFUN(test_mfun) {}

GWION_IMPORT(array) {
  GWI_B(gwi_class_ini(gwi, "ArrayTest", NULL))
  GWI_B(gwi_item_ini(gwi, "int[]", "int_array"))
  GWI_B(gwi_item_end(gwi, 0, obj, NULL)) // import array var
  GWI_B(gwi_func_ini(gwi, "float[][]", "f"))
  GWI_B(gwi_func_end(gwi, test_mfun, ae_flag_none))
  GWI_B(gwi_func_ini(gwi, "float[][]", "g"))
  GWI_B(gwi_func_end(gwi, test_mfun, ae_flag_none))
  GWI_B(gwi_class_end(gwi))
  return true;
}
