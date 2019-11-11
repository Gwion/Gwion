#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(union_member) {
  GWI_OB(gwi_class_ini(gwi, "UnionMember", NULL))
    GWI_BB(gwi_union_ini(gwi, "U", NULL))
    GWI_BB(gwi_union_add(gwi,"float", "f"))
    GWI_BB(gwi_union_add(gwi,"int[]", "i"))
    GWI_OB(gwi_union_end(gwi, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
