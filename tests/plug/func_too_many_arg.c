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

GWION_IMPORT(func_too_many_arg) {
  GWI_BB(gwi_func_ini(gwi, "int", "test"))
  GWI_BB(gwi_func_arg(gwi, "int", "i"))
  GWI_BB(gwi_func_arg(gwi, "int", "j"))
  GWI_BB(gwi_func_arg(gwi, "int", "k"))
  GWI_BB(gwi_func_arg(gwi, "int", "l"))
  GWI_BB(gwi_func_arg(gwi, "int", "m"))
  GWI_BB(gwi_func_arg(gwi, "int", "n"))
  GWI_BB(gwi_func_arg(gwi, "int", "o"))
  GWI_BB(gwi_func_arg(gwi, "int", "p"))
  GWI_BB(gwi_func_arg(gwi, "int", "q"))
  GWI_BB(gwi_func_arg(gwi, "int", "r"))
  GWI_BB(gwi_func_arg(gwi, "int", "s"))
  GWI_BB(gwi_func_arg(gwi, "int", "t"))
  GWI_BB(gwi_func_arg(gwi, "int", "u"))
  GWI_BB(gwi_func_arg(gwi, "int", "v"))
  GWI_BB(gwi_func_arg(gwi, "int", "w"))
  GWI_BB(gwi_func_arg(gwi, "int", "x"))
  GWI_BB(gwi_func_arg(gwi, "int", "y"))
  GWI_BB(gwi_func_arg(gwi, "int", "z"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_none))
  return GW_OK;
}
