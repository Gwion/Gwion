#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "object.h"
#include "instr.h"
#include "import.h"

GWION_IMPORT(static_string_test) {
  CHECK_BB(gwi_item_ini(gwi, "string", "self"))
  M_Object obj = new_string(NULL, "test static string");
  CHECK_BB(gwi_item_end(gwi, ae_flag_global, obj))
  return GW_OK;
}
