#include "defs.h"
#include "type.h"
#include "import.h"

IMPORT {
  gwi_oper_ini(gwi, "int", "int", "int"); // ignore the check
  gwi_oper_end(gwi, 220, NULL); // ignore the check.
  CHECK_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, NULL))
  return 1;
}
