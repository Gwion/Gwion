#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "value.h"
#include "operator.h"
#include "import.h"

static MFUN(test_func) { puts("test"); }
GWION_IMPORT(typedef_test) {
  Type t_func_typedef;
  GWI_OB((t_func_typedef = gwi_mk_type(gwi, "FuncTypedef", SZ_INT , NULL)))
  GWI_BB(gwi_fptr_ini(gwi, "void", "PtrType"))
  GWI_OB(gwi_fptr_end(gwi, 0))
  
  GWI_BB(gwi_class_ini(gwi, t_func_typedef, NULL, NULL))
    GWI_BB(gwi_fptr_ini(gwi, "void", "PtrType"))
    GWI_OB(gwi_fptr_end(gwi, ae_flag_static))
    GWI_BB(gwi_func_ini(gwi, "void", "test_func", test_func))
    GWI_BB(gwi_func_end(gwi, ae_flag_static))
    GWI_BB(gwi_item_ini(gwi, "PtrType", "ptr"))
    GWI_BB(gwi_item_end(gwi, ae_flag_static, NULL))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
