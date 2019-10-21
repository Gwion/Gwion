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
  GWI_BB(gwi_fptr_ini(gwi, "int", "<~A.~>test"))
  GWI_OB(gwi_fptr_end(gwi, 0))
  return GW_OK;
}
