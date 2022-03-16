#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"

ANN m_bool compat_func(const restrict Func_Def lhs,
                       const restrict Func_Def rhs) {
  Arg_List args0 = lhs->base->args;
  Arg_List args1 = rhs->base->args;
  if(!args0 && !args1)
    return GW_OK;
  if((!args0 && args1) || (args0 && !args1))
    return GW_ERROR;
  if(args0->len != args1->len)
    return GW_ERROR;
  for(uint32_t i = 0; i < args0->len; i++) {
    Arg *arg0 = mp_vector_at(args0, Arg, i);
    Arg *arg1 = mp_vector_at(args1, Arg, i);
    if (arg0->type != arg1->type) return GW_ERROR;
  }
  return GW_OK;
}
