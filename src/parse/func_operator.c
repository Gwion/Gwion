#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"

ANN void func_operator(const Func_Def fdef, struct Op_Import *opi) {
  opi->op         = fdef->base->xid;
  const m_str str = s_name(fdef->base->xid);
  const uint  is_unary =
      fbflag(fdef->base, fbflag_unary) +
      (!strcmp(str, "@conditional") || !strcmp(str, "@unconditional"));
  const Arg_List args = fdef->base->args;
  opi->lhs = is_unary ? NULL : args ? args->var_decl->value->type : NULL;
  if (strcmp(str, "@implicit"))
    opi->rhs = args ? is_unary ? args->var_decl->value->type
                               : (args->next ? args->next->var_decl->value->type
                                             : NULL)
                    : NULL;
  else
    opi->rhs = fdef->base->ret_type;
}
