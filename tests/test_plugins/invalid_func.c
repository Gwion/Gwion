#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "type.h"
#include "import.h"

static MFUN(test_mfun){}
GWION_IMPORT(invalid_func_test) {
  Type t_invalid_var_type ;
  CHECK_OB((t_invalid_var_type = gwi_mk_type(gwi, "invalid_var_type",
          SZ_INT , NULL)))
  CHECK_BB(gwi_class_ini(gwi, t_invalid_var_type, NULL, NULL))
  CHECK_BB(gwi_func_ini(gwi, ".int", "i", test_mfun))
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
