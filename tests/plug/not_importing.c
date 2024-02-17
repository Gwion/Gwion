#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(not_importing) {
  GWI_B(gwi_enum_add(gwi, "adc", 0))
  return GW_OK;
}
