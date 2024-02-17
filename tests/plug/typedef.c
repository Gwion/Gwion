#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(typedef) {
  GWI_B(gwi_typedef_ini(gwi, "int", "Typedef"))
  GWI_B(gwi_typedef_end(gwi, ae_flag_none))
  return true;
}
