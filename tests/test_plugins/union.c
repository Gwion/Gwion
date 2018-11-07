#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "object.h"
#include "import.h"

GWION_IMPORT(union_test) {
  CHECK_BB(gwi_union_ini(gwi, NULL))
  CHECK_BB(gwi_union_add(gwi,"float", "f"))
  CHECK_BB(gwi_union_add(gwi,"int", "i"))
  CHECK_BB(gwi_union_end(gwi, 0))
  return 1;
}
