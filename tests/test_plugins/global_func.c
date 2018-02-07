#include <stdlib.h>
#include "type.h"
#include "instr.h"
#include "import.h"

SFUN(coverage_int) {
  puts("test");
  *(m_int*)RETURN = *(m_int*)MEM(SZ_INT);
}

IMPORT {
  CHECK_BB(importer_func_ini(importer, "int", "test", coverage_int))
  CHECK_BB(importer_func_arg(importer, "int", "i"))
  CHECK_BB(importer_func_end(importer, 0))
  return 1;
}
