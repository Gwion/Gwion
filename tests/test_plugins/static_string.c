#include <stdlib.h>
#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "type.h"
#include "object.h"
#include "import.h"

GWION_IMPORT(static_string_test) {
  CHECK_BB(gwi_item_ini(gwi, "string", "self"))
  M_Object obj = new_string(NULL, "test static string");
  CHECK_BB(gwi_item_end(gwi, ae_flag_global, obj))
  return 1;
}
