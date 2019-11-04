#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "gwion.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "instr.h"

static SFUN(func_tmpl_xfun) {
  
}

GWION_IMPORT(func_tmpl) {
  GWI_BB(gwi_func_ini(gwi, "voit", "test<~A~>"))
  GWI_BB(gwi_func_end(gwi, func_tmpl_xfun, ae_flag_none))
  return GW_OK;
}
