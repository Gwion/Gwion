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

GWION_IMPORT(union_test) {
  GWI_BB(gwi_union_ini(gwi, NULL, "my_union"))
  GWI_BB(gwi_union_add(gwi,"float", "f"))
  GWI_BB(gwi_union_add(gwi,"int", "i"))
  GWI_OB(gwi_union_end(gwi, 0))
  return GW_OK;
}
