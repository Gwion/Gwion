#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(union_test) {
  GWI_BB(gwi_union_ini(gwi, "U:[A]"))
  GWI_BB(gwi_union_add(gwi, "float", "f"))
  GWI_BB(gwi_union_add(gwi, "int", "i"))
  GWI_BB(gwi_union_add(gwi, "A", "a"))
  GWI_BB(gwi_union_ini(gwi, "U:[A]"))
  GWI_OB(gwi_union_end(gwi, 0))
  return GW_OK;
}
