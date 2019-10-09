#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

GWION_IMPORT(static_string_test) {
  const M_Object obj = new_object(gwi->gwion->mp, NULL, gwi->gwion->type[et_string]);
  STRING(obj) = s_name(insert_symbol(gwi->gwion->st, "test static string"));
  CHECK_BB(gwi_item_ini(gwi, "string", "self"))
  CHECK_BB(gwi_item_end(gwi, ae_flag_global, obj))
  return GW_OK;
}
