#include "defs.h"
#include "type.h"
#include "import.h"

MFUN(test_mfun){}
IMPORT
{
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
