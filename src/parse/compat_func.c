#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"

ANN bool compat_func(const restrict Func_Def lhs,
                       const restrict Func_Def rhs) {
  ArgList *args0 = lhs->base->args;
  ArgList *args1 = rhs->base->args;
  if(!args0 && !args1)
    return true;
  if((!args0 && args1) || (args0 && !args1))
    return false;
  if(args0->len != args1->len)
    return false;
  for(uint32_t i = 0; i < args0->len; i++) {
    const Arg arg0 = arglist_at(args0, i);
    const Arg arg1 = arglist_at(args1, i);
    if (arg0.type != arg1.type) return false;
  }
  return true;
}
