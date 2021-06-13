#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"

ANN m_bool compat_func(const restrict Func_Def lhs,
                       const restrict Func_Def rhs) {
  Arg_List e1 = lhs->base->args;
  Arg_List e2 = rhs->base->args;

  while (e1 && e2) {
    if (e1->type != e2->type) return GW_ERROR;
    e1 = e1->next;
    e2 = e2->next;
  }
  if (e1 || e2) return GW_ERROR;
  return GW_OK;
}
