#include <stdlib.h>
#include <math.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "vm.h"
#include "gwion.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "func.h"
#include "gwi.h"

GWION_IMPORT(fail_on_next_arg) {
  GWI_BB(gwi_func_ini(gwi, "void", "test"))
  GWI_BB(gwi_func_arg(gwi, "int", "i"))
  GWI_BB(gwi_func_arg(gwi, "int", "j"))
  return gwi_func_end(gwi, (f_xfun)1, ae_flag_none);
}
