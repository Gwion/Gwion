#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(typedef_fail) {
  GWI_BB(gwi_typedef_ini(gwi, "int", "Typedef:[A]"))
  GWI_BB(gwi_typedef_ini(gwi, "int", "Typedef:[A]"))
  return GW_OK;
}
