#include "gwion_util.h"
#include "gwion_ast.h"
#include "absyn.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "gwion.h"
#include "value.h"
#include "operator.h"
#include "object.h"
#include "instr.h"
#include "import.h"

MFUN(test_mfun){}
GWION_IMPORT(end_class) {
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
