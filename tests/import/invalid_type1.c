#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "value.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(invalid_type1_test) {
  GWI_OB(gwi_class_ini(gwi, "invalid_var_type", NULL))
  GWI_BB(gwi_item_ini(gwi,"i|nt", "test"))
  GWI_BB(gwi_item_end(gwi, 0, NULL))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
