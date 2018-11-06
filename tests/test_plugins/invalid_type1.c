#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "type.h"
#include "object.h"
#include "import.h"

static MFUN(test_mfun){}
GWION_IMPORT(invalid_type1_test) {
  Type t_invalid_var_type;
  CHECK_OB((t_invalid_var_type = gwi_mk_type(gwi, "invalid_var_type",
          SZ_INT , NULL)))
  CHECK_BB(gwi_class_ini(gwi, t_invalid_var_type, NULL, NULL))
  CHECK_BB(gwi_item_ini(gwi,"i|nt", "test"))
  CHECK_BB(gwi_item_end(gwi, 0, NULL))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
