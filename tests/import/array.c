#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

MFUN(test_mfun){}

GWION_IMPORT(array_test) {
  Type t_invalid_var_name;
  GWI_OB((t_invalid_var_name = gwi_mk_type(gwi, "invalid_var_name", SZ_INT, "Object")))
  GWI_BB(gwi_class_ini(gwi, t_invalid_var_name, NULL, NULL))
  GWI_BB(gwi_item_ini(gwi, "int[]", "int_array"))
  GWI_BB(gwi_item_end(gwi, 0, NULL)) // import array var
  GWI_BB(gwi_func_ini(gwi, "float[][]", "f", test_mfun))
  GWI_BB(gwi_func_end(gwi, 0))
  GWI_BB(gwi_func_ini(gwi, "float[][]", "g", test_mfun))
  GWI_BB(gwi_func_end(gwi, 0))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
