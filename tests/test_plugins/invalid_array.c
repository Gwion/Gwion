#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_type = { "invalid_var_type", SZ_INT, &t_object };

MFUN(test_mfun){}
IMPORT
{
  CHECK_BB(gwi_class_ini(gwi, &t_invalid_var_type, NULL, NULL))
  CHECK_BB(gwi_func_ini(gwi, "int[]", "func", test_mfun))
    CHECK_BB(gwi_func_arg(gwi, "int[][]", "i"))
    CHECK_BB(gwi_func_arg(gwi, "int", "j[]"))
    CHECK_BB(gwi_func_arg(gwi, "int[]", "k[]"))
    CHECK_BB(gwi_func_arg(gwi, "int", "l"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  CHECK_BB(gwi_func_ini(gwi, "int[]", "func", test_mfun))
    CHECK_BB(gwi_func_arg(gwi, "int", "j[][]"))
    CHECK_BB(gwi_func_arg(gwi, "int[]", "+k[][][]"))
    CHECK_BB(gwi_func_arg(gwi, "int", "l"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  CHECK_BB(gwi_func_ini(gwi, "int[]", "func", test_mfun))
    CHECK_BB(gwi_func_arg(gwi, "+int", "j[][]"))
    CHECK_BB(gwi_func_arg(gwi, "int[]", "+k[][][]"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
