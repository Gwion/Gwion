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
  CHECK_BB(add_global_type(env, &t_invalid_var_name))
  CHECK_BB(import_class_begin(env, &t_invalid_var_name, env->global_nspc, NULL, NULL))

  import_mvar(env, "int[]", "int_array", 0,0,"invalid name"); // import array var
  fun = new_DL_Func("float[]", "f", (m_uint)test_mfun);
  CHECK_OB(import_mfun(env, fun))

  CHECK_BB(import_class_end(env))
}
