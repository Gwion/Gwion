#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "traverse.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "func.h"
#include "nspc.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "mpool.h"
#include "specialid.h"

ANN2(1,3) static Type _get_type(const Env env, const m_str str, const loc_t pos) {
  m_uint depth = 0;
  const ID_List list = (str && str != (m_str)OP_ANY_TYPE) ? str2list(env, str, &depth, pos) : NULL;
  const Type  t = (str == (m_str) OP_ANY_TYPE) ? OP_ANY_TYPE : list ? find_type(env, list) : NULL;
  if(list)
    free_id_list(env->gwion->mp, list);
  return t ? (depth ? array_type(env, t, depth) : t) : NULL;
}

ANN2(1,3) static inline Type get_type(const Env env, const m_str str, const loc_t pos) {
  return str ? _get_type(env, str, pos) : NULL;
}

ANN2(1,2) static int import_op(const Gwi gwi, const DL_Oper* op,
    const f_instr f) {
  const Env env = gwi->gwion->env;
  const Type lhs = get_type(env, op->lhs, gwi->loc),
             rhs = get_type(env, op->rhs, gwi->loc),
             ret = _get_type(env, op->ret, gwi->loc);
  const struct Op_Import opi = { lhs, rhs, ret,
    op->ck, op->em, (uintptr_t)f, gwi->loc, op->op };
  return add_op(gwi->gwion, &opi);
}


ANN2(1) m_int gwi_oper_ini(const Gwi gwi, const restrict m_str l,
    const restrict m_str r, const restrict m_str t) {
  gwi->oper.ret = t;
  gwi->oper.rhs = r;
  gwi->oper.lhs = l;
  return GW_OK;
}

ANN m_int gwi_oper_add(const Gwi gwi, Type (*ck)(Env, void*, m_bool*)) {
  gwi->oper.ck = ck;
  return GW_OK;
}

ANN m_int gwi_oper_emi(const Gwi gwi, const opem em) {
  gwi->oper.em = em;
  return GW_OK;
}

ANN m_int gwi_oper_end(const Gwi gwi, const m_str op, const f_instr f) {
  gwi->oper.op = insert_symbol(gwi->gwion->st, op);
  const m_bool ret = import_op(gwi, &gwi->oper, f);
  gwi->oper.ck = NULL;
  gwi->oper.em = NULL;
  return ret;
}

ANN m_int gwi_oper_cond(const Gwi gwi, const m_str type,
  const f_instr f1, const f_instr f2) {
  GWI_BB(gwi_oper_ini(gwi, NULL, type, "int"))
  GWI_BB(gwi_oper_end(gwi, "@conditionnal", f1))
  GWI_BB(gwi_oper_end(gwi, "@unconditionnal", f2))
  return GW_OK;
}
