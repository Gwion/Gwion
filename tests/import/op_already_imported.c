#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "instr.h"
#include "vm.h"
#include "gwion.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"


GWION_IMPORT(op_already_imported) {
  GWI_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  GWI_BB(gwi_oper_end(gwi, "=>", NULL))
  return GW_OK;
}
