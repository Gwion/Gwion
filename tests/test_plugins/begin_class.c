#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_name = { "invalid_var_name", SZ_INT, &t_object };

MFUN(test_mfun){}
IMPORT
{
  DL_Func* fun;
  CHECK_BB(import_class_begin(env, &t_invalid_var_name, env->global_nspc, NULL, NULL))
  CHECK_BB(import_class_begin(env, &t_invalid_var_name, env->global_nspc, NULL, NULL))
  return 1;
}
