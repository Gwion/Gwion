#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"

static OP_CHECK(opck_ptrext_assign) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  if(bin->rhs->meta != ae_meta_var)
    ERR_N(exp_self(bin)->pos, _("left side operand is constant"));
  bin->rhs->emit_var = 1;
  Type t = bin->rhs->type;
  do {
    Type u = bin->lhs->type;
    do {
      const m_str str = get_type_name(env, u->name, 1);
      if(str && !strcmp(t->name, str))
      return bin->lhs->type;
    } while((u = u->e->parent));
  } while((t = t->e->parent));
  return env->gwion->type[et_null];
}

static INSTR(instr_ptrext_assign) {
  POP_REG(shred, SZ_INT)
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  *(m_float**)o->data = *(m_float**)REG(0);
}

GWION_IMPORT(typedef_test) {
  GWI_OB(gwi_class_ini(gwi, "PtrExt", "Ptr<~float~>"))
  GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_oper_ini(gwi, "nonnull PtrExt", "float", NULL))
  GWI_BB(gwi_oper_var(gwi, SZ_INT))
  GWI_BB(gwi_oper_add(gwi, opck_ptrext_assign))
  GWI_BB(gwi_oper_end(gwi, "<=:", instr_ptrext_assign))
  return GW_OK;
}
