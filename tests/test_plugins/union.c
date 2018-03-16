#include "defs.h"
#include "type.h"
#include "import.h"

IMPORT {
  CHECK_BB(gwi_union_ini(gwi, NULL))
  CHECK_BB(gwi_union_add(gwi,"float", "f"))
  CHECK_BB(gwi_union_add(gwi,"int", "i"))
  CHECK_BB(gwi_union_end(gwi, 0))
  return 1;
}
