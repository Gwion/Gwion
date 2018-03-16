#include "defs.h"
#include "type.h"
#include "import.h"


MFUN(test_mfun){}
IMPORT
{
  Type t_invalid_var_name;
  CHECK_BB((t_invalid_var_name = gwi_mk_type(gwi, "invalid_var_name", SZ_INT, t_object)))
  CHECK_BB(gwi_class_ini(gwi, t_invalid_var_name, NULL, NULL))
  CHECK_BB(gwi_item_ini(gwi,"int[]", "int_array"))
  CHECK_BB(gwi_item_end(gwi, 0, NULL)) // import array var
  CHECK_BB(gwi_func_ini(gwi, "float[][]", "f", test_mfun))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_func_ini(gwi, "float[][]", "g", test_mfun))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
