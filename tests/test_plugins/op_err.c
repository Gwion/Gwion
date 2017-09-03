#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_name = { "invalid_var_name", SZ_INT, &t_object };

MFUN(test_mfun){}
IMPORT
{
  importer_add_op(importer, 220, "int", "int", "int", NULL, 1); // ignore the check.
  CHECK_BB(importer_add_op(importer, op_chuck, "int", "int", "int", NULL, 1))
  return 1;
}
