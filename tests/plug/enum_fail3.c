#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(enum_fail3) {
  GWI_B(gwi_enum_ini(gwi, "test"))
  GWI_B(gwi_enum_add(gwi, "adc", 0))
  GWI_B(gwi_enum_ini(gwi, "error"))
  return true;
}
