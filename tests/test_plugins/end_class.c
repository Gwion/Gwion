#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "vm.h"
#include "oo.h"
#include "env.h"
#include "operator.h"
#include "object.h"
#include "import.h"

MFUN(test_mfun){}
GWION_IMPORT(end_class) {
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
