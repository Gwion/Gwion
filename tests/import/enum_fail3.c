#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(enum_test) {
  GWI_BB(gwi_enum_ini(gwi, NULL))
    GWI_BB(gwi_enum_add(gwi, "adc", 0))
  GWI_BB(gwi_enum_ini(gwi, NULL))
  return GW_OK;
}
