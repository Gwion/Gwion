#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "gwi.h"

GWION_IMPORT(fail_on_next_arg2) {
  GWI_BB(gwi_func_ini(gwi, "void", "test"))
  GWI_BB(gwi_func_arg(gwi, "int", "i"))
  GWI_BB(gwi_func_arg(gwi, "i;t", "j"))
  GWI_BB(gwi_func_arg(gwi, "int", "j[]"))
  return gwi_func_end(gwi, (f_xfun)1, ae_flag_none);
}
