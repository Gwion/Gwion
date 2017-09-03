#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_type = { ".invalid_var_type", SZ_INT, &t_object };

MFUN(test_mfun){}
IMPORT
{
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_invalid_var_type, NULL, NULL))
  import_var(env, ".int", "test", 0, NULL);
  CHECK_BB(import_class_end(env))
  return 1;
}
