#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(enum_test) {
  GWI_BB(gwi_enum_ini(gwi, "tmp"))
  GWI_BB(gwi_enum_add(gwi, "adc", 0))
  GWI_OB(gwi_enum_end(gwi))
  return GW_OK;
}
