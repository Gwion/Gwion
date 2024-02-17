#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

ANN static bool pass(Env nv NUSED, Ast *ast NUSED) { return true; }

GWION_IMPORT(pass) {
  gwi_register_pass(gwi, "dummy", pass);
  return true;
}
