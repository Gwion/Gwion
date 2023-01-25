#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "operator.h"
#include "object.h"
#include "instr.h"
#include "import.h"

GWION_IMPORT(non_driver) {
  (void)gwi;
  return GW_OK;
}
