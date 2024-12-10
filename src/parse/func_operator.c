#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"

ANN void func_operator(const Func_Def fdef, struct Op_Import *opi) {
  opi->op         = fdef->base->tag.sym;
  const m_str str = s_name(fdef->base->tag.sym);
  const uint  is_unary =
      fbflag(fdef->base, fbflag_unary) +
      (!strcmp(str, "@conditional") || !strcmp(str, "@unconditional"));
  ArgList *args = fdef->base->args;
  Arg *arg0 = args ? arglist_ptr_at(args, 0) : NULL;
  Arg *arg1 = (args && args->len >= 2) ? arglist_ptr_at(args, 1) : NULL;
  opi->lhs = is_unary ? NULL : args ? arg0->var.vd.value->type : NULL;
  if (strcmp(str, "@implicit"))
//    opi->rhs = args ? is_unary ? arg0->var_decl->value->type
    opi->rhs = args ? is_unary ? (arg0 ? arg0->var.vd.value->type : NULL)
                               : (arg1 ? arg1->var.vd.value->type : NULL)
                    : NULL;
  else
    opi->rhs = fdef->base->ret_type;
}
