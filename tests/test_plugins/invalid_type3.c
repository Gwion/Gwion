#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_type = { ".invalid_var_type", SZ_INT, &t_object };

MFUN(test_mfun){}
IMPORT
{
  CHECK_BB(importer_class_begin(importer, &t_invalid_var_type, NULL, NULL))
  importer_add_var(importer, ".int", "test", 0, NULL);
  CHECK_BB(importer_class_end(importer))
  return 1;
}
