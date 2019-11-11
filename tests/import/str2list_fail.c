#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"


GWION_IMPORT(global_var_test) {
  GWI_BB(gwi_item_ini(gwi,"SoemClass.child", "i"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, NULL))
  return GW_OK;
}
