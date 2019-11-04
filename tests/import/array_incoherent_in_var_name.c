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
#include "gwi.h"

GWION_IMPORT(checker) {
  gwi_item_ini(gwi, "int", "array[2][3][]");
  return GW_OK;
}
