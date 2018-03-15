#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_invalid_var_type = { "invalid_var_type", SZ_INT, t_object };

MFUN(test_mfun){}
IMPORT
{
  CHECK_BB(gwi_class_ini(gwi, &t_invalid_var_type, NULL, NULL))
  CHECK_BB(gwi_item_ini(gwi,".int", "test"))
  CHECK_BB(gwi_item_end(gwi, 0, NULL))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
