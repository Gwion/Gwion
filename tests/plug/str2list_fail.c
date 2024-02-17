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

GWION_IMPORT(str2list_fail) {
  GWI_B(gwi_item_ini(gwi, "SoemClass.child", "i"))
  GWI_B(gwi_item_end(gwi, ae_flag_none, num, 0))
  return true;
}
