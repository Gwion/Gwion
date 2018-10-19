#include "defs.h"
#include "type.h"
#include "import.h"

MFUN(test_mfun){}
GWION_IMPORT(end_class) {
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
