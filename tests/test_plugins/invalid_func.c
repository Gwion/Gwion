#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_type = { "invalid_var_type", SZ_INT, &t_object };

MFUN(test_mfun){}
IMPORT
{
  DL_Func* fun;
  CHECK_BB(add_global_type(env, &t_invalid_var_type))
  CHECK_BB(import_class_begin(env, &t_invalid_var_type, env->global_nspc, NULL, NULL))
  fun = new_DL_Func(".int", "i", (m_uint)test_mfun);
  CHECK_OB(import_sfun(env, fun))

  CHECK_BB(import_class_end(env))
}
