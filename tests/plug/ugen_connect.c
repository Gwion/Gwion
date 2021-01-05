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
#include "ugen.h"

GWION_IMPORT(ugen_connect) {
  const UGen ug0 = (UGen)vector_at(&gwi->gwion->vm->ugen, 0);
  const UGen ug1 = (UGen)vector_at(&gwi->gwion->vm->ugen, 1);
  ugen_connect(ug0, ug1);
  ugen_disconnect(ug0, ug1);
  return GW_OK;
}
