#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "instr.h"

SFUN(coverage_int) {
  puts("test");
  *(m_int*)RETURN = *(m_int*)MEM(0);
}

GWION_IMPORT(global_func_test) {
  CHECK_BB(gwi_func_ini(gwi, "int", "test", coverage_int))
  CHECK_BB(gwi_func_arg(gwi, "int", "i"))
  CHECK_BB(gwi_func_end(gwi, 0))
  return GW_OK;
}
