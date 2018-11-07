#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "object.h"
#include "import.h"

static MFUN(template_arg_fun) {}

GWION_IMPORT(template_arg_test) {
  Type t_template_arg;
  CHECK_OB((t_template_arg = gwi_mk_type(gwi, "TemplateArg", SZ_INT , NULL)))
  CHECK_BB(gwi_class_ini(gwi, t_template_arg, NULL, NULL))
  CHECK_BB(gwi_func_ini(gwi, "int", "set", template_arg_fun))
  CHECK_BB(gwi_func_arg(gwi, "Pair<Ptr<int>,float>","test"))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
