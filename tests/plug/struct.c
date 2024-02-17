#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

SFUN(coverage_int) { *(m_uint *)RETURN = 0; }
SFUN(coverage_float) { *(m_float *)RETURN = 0; }

GWION_IMPORT(struct) {
  GWI_B(gwi_struct_ini(gwi, "Struct"))
  GWI_B(gwi_func_ini(gwi, "int", "i"))
  GWI_B(gwi_func_end(gwi, coverage_int, ae_flag_static))
  GWI_B(gwi_func_ini(gwi, "float", "f"))
  GWI_B(gwi_func_end(gwi, coverage_float, ae_flag_static))

  GWI_B(gwi_item_end(gwi, ae_flag_static, num, 5))

  GWI_B(gwi_item_ini(gwi, "int", "s_f"))
  GWI_B(gwi_item_end(gwi, ae_flag_static, fnum, 2.1))

  GWI_B(gwi_item_ini(gwi, "int", "sc_i"))
  GWI_B(gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, 5))

  GWI_B(gwi_item_ini(gwi, "float", "sc_f"))
  GWI_B(gwi_item_end(gwi, ae_flag_static | ae_flag_const, fnum, 2.1))

  GWI_B(gwi_item_ini(gwi, "int[][]", "test_array"))
  GWI_B(gwi_item_end(gwi, ae_flag_none, obj, NULL))

  GWI_B(gwi_struct_end(gwi))
  return true;
}
