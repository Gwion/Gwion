#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

GWION_IMPORT(fptr_tmpl) {
  Type t_func_typedef;
  GWI_B((t_func_typedef = gwi_mk_type(gwi, "FuncTypedef", SZ_INT, NULL)))
  GWI_B(gwi_fptr_ini(gwi, "int", "test:[test]"))
  GWI_B(gwi_fptr_end(gwi, 0))
  return true;
}
