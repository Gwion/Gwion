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

GWION_IMPORT(class_template_fail) {
  GWI_OB(gwi_class_ini(gwi, "ClassTemplate<~A,B~>", NULL))
  GWI_BB(gwi_func_ini(gwi, "int", "test"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
