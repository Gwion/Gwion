#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "instr.h"

GWION_IMPORT(too_many_args) {
  GWI_BB(gwi_func_ini(gwi, "Ptr<[int]", "test<[A]"))
  GWI_BB(gwi_func_arg(gwi, "int", "i[][]"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, NULL))
  return GW_OK;
}
