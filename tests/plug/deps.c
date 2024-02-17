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

GWION_IMPORT(deps) {
  (void)gwi;
  return true;
}

static const m_str deps[] = {"array", NULL};
GWDEPEND(deps) { return deps; }
