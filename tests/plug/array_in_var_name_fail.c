#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

GWION_IMPORT(checker) {
  GWI_BB(gwi_union_ini(gwi, NULL, NULL))
  GWI_BB(gwi_union_add(gwi, "int", "|array[2][3]"))
  return GW_OK;
}
