#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_name = { "invalid_var_name", SZ_INT, &t_object };

MFUN(test_mfun){}
IMPORT
{
  add_binary_op(env, 220,        &t_int, &t_int, &t_int, NULL,     1); // ignore the check.
  CHECK_BB(add_binary_op(env, op_chuck,        &t_int, &t_int, &t_int, NULL,     1))
  return 1;
}
