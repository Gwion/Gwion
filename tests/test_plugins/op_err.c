#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_name = { "invalid_var_name", SZ_INT, &t_object };

MFUN(test_mfun){}
IMPORT
{
  importer_oper_begin(importer, "int", "int", "int"); // ignore the check.
  importer_add_op(importer, 220, NULL, 1); // ignore the check.
  CHECK_BB(importer_oper_begin(importer, "int", "int", "int"))
  CHECK_BB(importer_add_op(importer, op_chuck, NULL, 1))
  return 1;
}
