#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_type = { "invalid_var_type", SZ_INT, &t_object };

MFUN(test_mfun){}
IMPORT
{
  CHECK_BB(importer_class_ini(importer, &t_invalid_var_type, NULL, NULL))
  importer_func_ini(importer, "int[]", "func", (m_uint)test_mfun);
    importer_func_arg(importer, "int[][]", "i");
    importer_func_arg(importer, "int", "j[]");
    importer_func_arg(importer, "int[]", "k[]");
    importer_func_arg(importer, "int", "l");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "int[]", "func", (m_uint)test_mfun);
    importer_func_arg(importer, "int", "j[][]");
    importer_func_arg(importer, "int[]", "+k[][][]");
    importer_func_arg(importer, "int", "l");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "int[]", "func", (m_uint)test_mfun);
    importer_func_arg(importer, "+int", "j[][]");
    importer_func_arg(importer, "int[]", "+k[][][]");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  CHECK_BB(importer_class_end(importer))
  return 1;
}
