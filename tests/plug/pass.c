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

ANN static m_bool pass(Env nv NUSED, Ast ast NUSED) { return GW_OK; }

GWION_IMPORT(array_test) {
  gwi_register_pass(gwi, "dummy", pass);
  return GW_OK;
}
