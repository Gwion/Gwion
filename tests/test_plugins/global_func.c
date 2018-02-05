#include <stdlib.h>
#include "type.h"
#include "import.h"

SFUN(coverage_int)    { }

IMPORT {
  CHECK_BB(importer_func_ini(importer, "void", "test", (m_uint)coverage_int))
  CHECK_BB(importer_func_end(importer, 0))
  return 1;
}
