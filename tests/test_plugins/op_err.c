#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_name = { "invalid_var_name", SZ_INT, &t_object };

MFUN(test_mfun){}
IMPORT
{
  gwi_oper_ini(gwi, "int", "int", "int"); // ignore the check
  gwi_oper_end(gwi, 220, NULL); // ignore the check.
  CHECK_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, NULL))
  return 1;
}
