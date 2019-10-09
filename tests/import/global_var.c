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


GWION_IMPORT(global_var_test) {
//  ALLOC_PTR(i, m_uint, 1);
  const M_Object obj = new_object(gwi->gwion->mp, NULL, gwi->gwion->type[et_string]);
  STRING(obj) = s_name(insert_symbol(gwi->gwion->st, "test"));
  GWI_BB(gwi_item_ini(gwi,"string", "i"))
  GWI_BB(gwi_item_end(gwi, 0, obj))
  return GW_OK;
}
