#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(union_name) {
  GWI_B(gwi_union_ini(gwi, "my_union"))
  GWI_B(gwi_union_add(gwi, "float", "f"))
  GWI_B(gwi_union_add(gwi, "int", "i"))
  GWI_B(gwi_union_end(gwi, 0))
  return true;
}
