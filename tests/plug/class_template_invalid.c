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

GWION_IMPORT(class_template_invalid) {
  GWI_B(gwi_class_ini(gwi, "ClassTemplate:[A,B]", NULL));
  GWI_B(gwi_item_ini(gwi, "A[]", "key"));
  GWI_B(gwi_item_end(gwi, ae_flag_none, obj, NULL));
  GWI_B(gwi_item_ini(gwi, "B[]", "value"));
  GWI_B(gwi_item_end(gwi, ae_flag_none, obj, NULL));
  GWI_B(gwi_func_ini(gwi, "int", "test:[C,D]"));
  GWI_B(gwi_func_end(gwi, (f_xfun)1, ae_flag_none));

  GWI_B(gwi_class_end(gwi));
  GWI_B(gwi_item_ini(gwi, "ClassTemplate:[:[int]Ptr,int[]]", "testObject"));
  GWI_B(gwi_item_end(gwi, ae_flag_none, obj, NULL));
  return true;
}
