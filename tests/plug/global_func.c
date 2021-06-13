#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "instr.h"

SFUN(coverage_int) {
  puts("test");
  *(m_int *)RETURN = *(m_int *)MEM(0);
}

GWION_IMPORT(global_func_test) {
  GWI_BB(gwi_func_ini(gwi, "int", "test"))
  GWI_BB(gwi_func_arg(gwi, "int", "i"))
  GWI_BB(gwi_func_end(gwi, coverage_int, ae_flag_none))
  return GW_OK;
}
