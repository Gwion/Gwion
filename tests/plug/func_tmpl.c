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

static SFUN(func_tmpl_xfun) {}

GWION_IMPORT(func_tmpl) {
  GWI_B(gwi_func_ini(gwi, "int", "test:[A]"))
  GWI_B(gwi_func_arg(gwi, "A", "i"))
  GWI_B(gwi_func_end(gwi, func_tmpl_xfun, ae_flag_none))
  return GW_OK;
}
