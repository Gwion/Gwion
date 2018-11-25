#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "object.h"
#include "instr.h"
#include "import.h"

MFUN(test_mfun){}
GWION_IMPORT(begin_class) {
  Type t_invalid_var_name;
  CHECK_OB((t_invalid_var_name = gwi_mk_type(gwi, "invalid_var_name", SZ_INT, t_object)))
  CHECK_BB(gwi_class_ini(gwi, t_invalid_var_name, NULL, NULL))
  CHECK_BB(gwi_class_ini(gwi, t_invalid_var_name, NULL, NULL))
  return 1;
}
