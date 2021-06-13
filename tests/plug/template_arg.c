#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

static MFUN(template_arg_fun) {}
GWION_IMPORT(template_arg_test) {
  GWI_OB(gwi_class_ini(gwi, "TemplateArg", NULL))
  GWI_BB(gwi_func_ini(gwi, "int", "set"))
  GWI_BB(gwi_func_arg(gwi, "Pair<Ptr<int>,float>", "test"))
  GWI_BB(gwi_func_end(gwi, template_arg_fun, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
