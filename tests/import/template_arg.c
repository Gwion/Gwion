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

static MFUN(template_arg_fun) {}
GWION_IMPORT(template_arg_test) {
  Type t_template_arg;
  GWI_OB((t_template_arg = gwi_mk_type(gwi, "TemplateArg", SZ_INT , NULL)))
  GWI_BB(gwi_class_ini(gwi, t_template_arg, NULL, NULL))
  GWI_BB(gwi_func_ini(gwi, "int", "set", template_arg_fun))
  GWI_BB(gwi_func_arg(gwi, "Pair<Ptr<int>,float>","test"))
  GWI_BB(gwi_func_end(gwi, 0))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
