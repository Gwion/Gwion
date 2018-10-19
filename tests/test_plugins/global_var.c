#include <stdlib.h>
#include "type.h"
#include "import.h"
#include "object.h"


GWION_IMPORT(global_var_test) {
//  ALLOC_PTR(i, m_uint, 1);
  M_Object i = new_string(NULL, "test");
  CHECK_BB(gwi_item_ini(gwi,"string", "i"))
  CHECK_BB(gwi_item_end(gwi, 0, i))
  return 1;
}
