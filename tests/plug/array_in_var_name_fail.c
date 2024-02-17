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

GWION_IMPORT(array_in_var_name_fail) {
  GWI_B(gwi_union_ini(gwi, "U"))
  GWI_B(gwi_union_add(gwi, "int", "i"))
  return true;
}
