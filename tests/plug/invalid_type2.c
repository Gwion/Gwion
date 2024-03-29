#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(invalid_type2) {
  GWI_B(gwi_class_ini(gwi, "invalid_var_type", NULL))
  GWI_B(gwi_item_ini(gwi, ".int", "test"))
  GWI_B(gwi_item_end(gwi, 0, num, 0))
  GWI_B(gwi_class_end(gwi))
  return true;
}
