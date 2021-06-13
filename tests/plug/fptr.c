#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

static MFUN(test_func) { puts("test"); }
GWION_IMPORT(typedef_test) {
  GWI_BB(gwi_fptr_ini(gwi, "void", "PtrType"))
  GWI_OB(gwi_fptr_end(gwi, 0))

  GWI_OB(gwi_class_ini(gwi, "FuncTypedef", NULL))
  GWI_BB(gwi_fptr_ini(gwi, "void", "PtrType"))
  GWI_OB(gwi_fptr_end(gwi, ae_flag_static))
  GWI_BB(gwi_func_ini(gwi, "void", "test_func"))
  GWI_BB(gwi_func_end(gwi, test_func, ae_flag_static))
  GWI_BB(gwi_item_ini(gwi, "PtrType", "ptr"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, obj, NULL))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
