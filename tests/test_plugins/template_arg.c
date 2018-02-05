#include "type.h"
#include "import.h"
#include "object.h"
#include "emit.h"

static struct Type_ t_template_arg = { "TemplateArg", SZ_INT, &t_object };

MFUN(template_arg_fun) {}

IMPORT{
  CHECK_BB(importer_class_ini(importer, &t_template_arg, NULL, NULL))
  CHECK_BB(importer_func_ini(importer, "int", "set", (m_uint)template_arg_fun))
  CHECK_BB(importer_func_arg(importer, "Pair<Ptr<int>,float>","test"))
  CHECK_BB(importer_func_end(importer, 0))
  CHECK_BB(importer_class_end(importer))
  return 1;
}
