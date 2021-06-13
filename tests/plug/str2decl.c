#include <stdlib.h>
#include <math.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "object.h"
#include "instr.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "gwi.h"

GWION_IMPORT(str2td) {
  GWI_OB(gwi_class_ini(gwi, "Test", NULL))
  GWI_OB(gwi_class_ini(gwi, "Child", NULL))
  GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_class_end(gwi))
  GWI_OB(gwi_class_ini(gwi, "Test2", "Test.Child"))
  return gwi_class_end(gwi);
}
