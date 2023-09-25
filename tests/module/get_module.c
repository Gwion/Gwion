#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "object.h"
#include "instr.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "plug.h"
#include "driver.h"

GWMODINI(get_module) {
  puts(__func__);
  return NULL;
}

GWMODEND(get_module) { puts(__func__); }

GWION_IMPORT(get_module) {
  CHECK_BB(set_module(gwi->gwion, "get_module", (void *)1));
  GWI_OB(get_module(gwi->gwion, "get_module"))
  puts("test passed");
  get_module(gwi->gwion, "non_existant_module");
  return GW_OK;
}
