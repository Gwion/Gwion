#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_name = { "invalid_var_name", SZ_INT, &t_object };

MFUN(test_mfun){}
IMPORT
{
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_invalid_var_name, env->global_nspc, NULL, NULL))

  import_mvar(env, "int[]", "int_array", 0,0); // import array var
  dl_func_init(&fun, "float[][]", "f", (m_uint)test_mfun);
  CHECK_OB(import_mfun(env, &fun))
  dl_func_init(&fun, "float[][]", "g", (m_uint)test_mfun);
  CHECK_OB(import_mfun(env, &fun))

  CHECK_BB(import_class_end(env))
  return 1;
}
