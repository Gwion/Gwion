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
GWION_IMPORT(begin_class) {
  Type t_invalid_var_name;
  GWI_OB((t_invalid_var_name =
              gwi_mk_type(gwi, "invalid_var_name", SZ_INT, "Object[]")))
  return GW_OK;
}
