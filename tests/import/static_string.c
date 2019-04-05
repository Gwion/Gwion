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
#include "gwion.h"
#include "gwi.h"

GWION_IMPORT(static_string_test) {
  const M_Object obj = gwion_new_string(gwi->gwion, "test static string");
  CHECK_BB(gwi_item_ini(gwi, "string", "self"))
//  CHECK_BB(gwi_item_end(gwi, ae_flag_global, obj))
  CHECK_BB(gwi_item_end(gwi, 0, obj))
  return GW_OK;
}
