#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(empty_union) {
  GWI_B(gwi_union_ini(gwi, "U"))
  GWI_B(gwi_union_end(gwi, 0))
  return true;
}
