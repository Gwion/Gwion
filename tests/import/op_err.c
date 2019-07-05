#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(op_err_test) {
  gwi_oper_ini(gwi, "int", "int", "int"); // ignore the check
  gwi_oper_end(gwi, 220, NULL); // ignore the check.
  CHECK_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, NULL))
  return GW_OK;
}
