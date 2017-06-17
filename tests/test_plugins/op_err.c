#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_name = { "invalid_var_name", SZ_INT, &t_object };

MFUN(test_mfun){}
IMPORT
{
  import_op(env, 220, "int", "int", "int", NULL, 1); // ignore the check.
  CHECK_BB(import_op(env, op_chuck, "int", "int", "int", NULL, 1))
  return 1;
}
