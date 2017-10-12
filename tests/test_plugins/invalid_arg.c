#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_type = { "invalid_var_type", SZ_INT, &t_object };

MFUN(test_mfun){}
IMPORT
{
  CHECK_BB(importer_class_begin(importer, &t_invalid_var_type, NULL, NULL))
  importer_func_begin(importer, "int[]", "func", (m_uint)test_mfun);
    importer_add_arg(importer, ".int", "i");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  CHECK_BB(importer_class_end(importer))
  return 1;
}
