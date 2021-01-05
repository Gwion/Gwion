#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "vm.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

GWION_IMPORT(op_already_imported) {
  GWI_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  GWI_BB(gwi_oper_end(gwi, "=>", NULL))
  GWI_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  GWI_BB(gwi_oper_end(gwi, "=>", NULL))
  return GW_OK;
}
