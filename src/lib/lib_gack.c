#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "gack.h"

static GACK(gack_gack) { INTERP_PRINTF("%s", *(m_str *)VALUE) }

static OP_CHECK(opck_gack_implicit) {
  const struct Implicit *imp  = (struct Implicit *)data;
  return imp->t;
}

static OP_EMIT(opem_gack_implicit) {
  const struct Implicit *imp  = (struct Implicit *)data;
  const Type t = imp->e->d.prim.d.exp->cast_to ?: imp->e->d.prim.d.exp->type;
  if(t == imp->t) {
    const Instr cpy = emit_add_instr(emit, Reg2RegOther); // kind
    cpy->m_val2 = SZ_INT;
    emit_regmove(emit, imp->t->size - SZ_INT);
  } else {
    emit_regmove(emit, SZ_INT);
    struct Op_Import opi = {.lhs = t,
                          .op  = insert_symbol(emit->gwion->st, "@implicit"),
                          .rhs = imp->t};
    CHECK_BB(op_emit(emit, &opi));
    emit_regmove(emit, -SZ_INT);
    const Instr cpy = emit_add_instr(emit, Reg2RegOther); // kind
    cpy->m_val = cpy->m_val2 = imp->t->size;
  }
  return GW_OK;
}

GWION_IMPORT(gack) {

  gwidoc(gwi, "a type for *pretty print*.");
  const Type t_gack = gwi_mk_type(gwi, "@Gack", SZ_INT, NULL);
  GWI_BB(gwi_gack(gwi, t_gack, gack_gack))
  GWI_BB(gwi_set_global_type(gwi, t_gack, et_gack));

  gwidoc(gwi, "@Gack implicit cast");
  GWI_BB(gwi_oper_ini(gwi, "@Gack", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_gack_implicit))
  GWI_BB(gwi_oper_emi(gwi, opem_gack_implicit))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))

  return GW_OK;
}
