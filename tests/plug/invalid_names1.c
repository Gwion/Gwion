#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "gwi.h"

GWION_IMPORT(invalid_names1) {
  DECL_B(const Type, t0, = gwi_mk_type(gwi, "T,", SZ_INT, NULL));
  gwi_add_type(gwi, t0);
  return true;
}
