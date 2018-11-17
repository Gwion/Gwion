#include "gwion_util.h"
#include "oo.h"
#include "env.h"
#include "nspc.h"
#include "traverse.h"
#include "template.h"

ANN m_bool traverse_template(const Env env, const Class_Def def) {
  CHECK_BB(template_push_types(env, def->tmpl->list.list, def->tmpl->base))
  CHECK_BB(traverse_class_def(env, def))
  POP_RET(1);
}

ANN m_bool traverse_func_template(const Env env, const Func_Def def, const Type_List types) {
  CHECK_BB(template_push_types(env, def->tmpl->list, types))
  return traverse_func_def(env, def);
}
