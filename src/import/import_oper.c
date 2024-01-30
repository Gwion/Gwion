#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "mpool.h"
#include "specialid.h"

ANN static Type _get_type(const Gwi gwi, const m_str s) {
  if (s == (m_str)OP_ANY_TYPE) return OP_ANY_TYPE;
  // str2type
  Type_Decl *td = gwi_str2td(gwi, s);
  const Type t  = known_type(gwi->gwion->env, td);
  free_type_decl(gwi->gwion->mp, td);
  return t;
}

ANN2(1) static inline Type gwi_get_type(const Gwi gwi, const m_str str) {
  return str ? _get_type(gwi, str) : NULL;
}

ANN2(1, 2)
static int import_op(const Gwi gwi, struct OperCK *const op, const f_instr f) {
  const Type lhs = gwi_get_type(gwi, op->lhs), rhs = gwi_get_type(gwi, op->rhs),
             ret              = gwi_get_type(gwi, op->ret);
  const struct Op_Func opfunc = {
      .ck = op->ck, .em = op->em, .effect = {.ptr = op->effect.ptr}};
  const struct Op_Import opi = {.lhs  = lhs,
                                .rhs  = rhs,
                                .ret  = ret,
                                .func = &opfunc,
                                .data = (uintptr_t)f,
                                .loc  = gwi->loc,
                                .op   = op->sym};
  const m_bool           b   = add_op(gwi->gwion, &opi);
  op->effect.ptr             = NULL;
  return b;
}

ANN2(1)
m_int gwi_oper_ini(const Gwi gwi, const restrict m_str l,
                   const restrict m_str r, const restrict m_str t) {
  gwi->oper->ret = t;
  gwi->oper->rhs = r;
  gwi->oper->lhs = l;
  return GW_OK;
}

ANN m_int gwi_oper_add(const Gwi gwi, Type (*ck)(Env, void *)) {
  gwi->oper->ck = ck;
  return GW_OK;
}

ANN m_int gwi_oper_emi(const Gwi gwi, const opem em) {
  gwi->oper->em = em;
  return GW_OK;
}

ANN void gwi_oper_eff(const Gwi gwi, const m_str effect) {
  if (!gwi->oper->effect.ptr) vector_init(&gwi->oper->effect);
  vector_add(&gwi->oper->effect, (m_uint)insert_symbol(gwi->gwion->st, effect));
}

ANN m_int gwi_oper_end(const Gwi gwi, const m_str op, const f_instr f) {
  if (gwi->gwion->data->cdoc) {
    gwfmt_indent(gwi->gwfmt);
    gwfmt_util(gwi->gwfmt, "{+C}operator{0} ");
    if (gwi->oper->lhs && !gwi->oper->rhs) {
      gwfmt_util(gwi->gwfmt, "{+}%s{0}",
           gwi->oper->lhs != (m_str)1 ? gwi->oper->lhs : "@Any");
      gwfmt_space(gwi->gwfmt);
    }
    if (gwi->oper->ret) {
      gwfmt_util(gwi->gwfmt, "{+}%s{0}",
           gwi->oper->ret != (m_str)1 ? gwi->oper->ret : "@Any");
      gwfmt_space(gwi->gwfmt);
    }
    gwfmt_util(gwi->gwfmt, "{/}%s{0}", op);
    gwfmt_space(gwi->gwfmt);
    gwfmt_lparen(gwi->gwfmt);
    if (gwi->oper->lhs && gwi->oper->rhs) {
      gwfmt_util(gwi->gwfmt, "{+}%s{0}",
           gwi->oper->lhs != (m_str)1 ? gwi->oper->lhs : "@Any");
      gwfmt_comma(gwi->gwfmt);
      gwfmt_space(gwi->gwfmt);
    }
    if (gwi->oper->rhs)
      gwfmt_util(gwi->gwfmt, "{+}%s{0}",
           gwi->oper->rhs != (m_str)1 ? gwi->oper->rhs : "@Any");
    gwfmt_rparen(gwi->gwfmt);
    gwfmt_sc(gwi->gwfmt);
    gwfmt_nl(gwi->gwfmt);
  }
  gwi->oper->sym   = insert_symbol(gwi->gwion->st, op);
  const m_bool ret = import_op(gwi, gwi->oper, f);
  gwi->oper->ck    = NULL;
  gwi->oper->em    = NULL;
  return ret;
}

ANN m_int gwi_oper_cond(const Gwi gwi, const m_str type, const f_instr f1,
                        const f_instr f2) {
  GWI_BB(gwi_oper_ini(gwi, NULL, type, "bool"))
  GWI_BB(gwi_oper_end(gwi, "@conditional", f1))
  GWI_BB(gwi_oper_end(gwi, "@unconditional", f2))
  return GW_OK;
}
