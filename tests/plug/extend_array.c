#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

GWION_IMPORT(extend_array) {
  GWI_B(gwi_class_ini(gwi, "ArrayExt", "float[1]"))
  GWI_B(gwi_class_end(gwi))
  return true;
}
