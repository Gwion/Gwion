#include <stdlib.h>
#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "type.h"
#include "object.h"
#include "import.h"


GWION_IMPORT(global_var_test) {
//  ALLOC_PTR(i, m_uint, 1);
  M_Object i = new_string(NULL, "test");
  CHECK_BB(gwi_item_ini(gwi,"string", "i"))
  CHECK_BB(gwi_item_end(gwi, 0, i))
  return 1;
}
