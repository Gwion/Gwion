#include <stdlib.h>
#include <math.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "vm.h"
#include "gwion.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "func.h"
#include "gwi.h"

GWION_IMPORT(str2decl) {
  GWI_OB(gwi_class_ini(gwi, "Test", NULL))
    GWI_OB(gwi_class_ini(gwi, "Child", NULL))
    GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_class_end(gwi))
  GWI_OB(gwi_class_ini(gwi, "Test2", "Test.Child"))
  return gwi_class_end(gwi);
}
