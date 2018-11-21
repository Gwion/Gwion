#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "object.h"
#include "instr.h"
#include "import.h"

static MFUN(test_func) { puts("test"); }
GWION_IMPORT(typedef_test) {
  Type t_func_typedef;
  CHECK_OB((t_func_typedef = gwi_mk_type(gwi, "FuncTypedef", SZ_INT , NULL)))
  CHECK_BB(gwi_fptr_ini(gwi, "void", "PtrType"))
  CHECK_BB(gwi_fptr_end(gwi, 0))
  
  CHECK_BB(gwi_class_ini(gwi, t_func_typedef, NULL, NULL))
    CHECK_BB(gwi_fptr_ini(gwi, "void", "PtrType"))
    CHECK_BB(gwi_fptr_end(gwi, ae_flag_static))
    CHECK_BB(gwi_func_ini(gwi, "void", "test_func", test_func))
    CHECK_BB(gwi_func_end(gwi, ae_flag_static))
    CHECK_BB(gwi_item_ini(gwi, "PtrType", "ptr"))
    CHECK_BB(gwi_item_end(gwi, ae_flag_static, NULL))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
