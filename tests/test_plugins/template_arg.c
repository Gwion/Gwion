#include "type.h"
#include "import.h"
#include "object.h"
#include "emit.h"

static struct Type_ t_template_arg = { "TemplateArg", SZ_INT, &t_object };

MFUN(template_arg_fun) {}

IMPORT{
  CHECK_BB(gwi_class_ini(gwi, &t_template_arg, NULL, NULL))
  CHECK_BB(gwi_func_ini(gwi, "int", "set", template_arg_fun))
  CHECK_BB(gwi_func_arg(gwi, "Pair<Ptr<int>,float>","test"))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
