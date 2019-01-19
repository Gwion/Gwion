#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "optim.h"
#include "constant.h"

ANN static void fold_exp(const Exp_Binary* bin) {
  const Exp n = bin->self->next;
  const Exp e = bin->self;
  free_exp(bin->lhs);
  free_exp(bin->rhs);
  e->exp_type = ae_exp_primary;
  e->d.exp_primary.self = e;
  e->next = n;
}

#define describe_xxx_exp(name, type, etype, target)            \
ANN static inline void name##_exp(const Exp e, const type t) { \
  e->d.exp_primary.primary_type = ae_primary_##etype;          \
  e->d.exp_primary.d.target = t;                               \
}
describe_xxx_exp(int,   m_int,   num,   num)
describe_xxx_exp(float, m_float, float, fnum)

#define CASE(op_op, l, op, r) case op_##op_op: ret = l op r; break;

#define DIV_BY_ZERO(l, op, r) if(r) ret = l op r; else \
         ERR_B(bin->rhs->pos, "ZeroDivideException")

#define COMMON_CASE(l, r)                     \
    CASE(add, l, +, r)                        \
    CASE(sub, l, -, r)                        \
    CASE(mul, l, /, r)                        \
    case op_div: DIV_BY_ZERO(l, / , r) break;

#define describe_fold_xxx(name, type, _l, _r, etype, opt)       \
ANN static m_bool fold_##name(const Exp_Binary* bin) {          \
  const union exp_primary_data *l = &bin->lhs->d.exp_primary.d; \
  const union exp_primary_data *r = &bin->rhs->d.exp_primary.d; \
  type ret = 0;                                                 \
  switch(bin->op) {                                             \
    COMMON_CASE(l->_l, r->_r)                                   \
    opt                                                         \
    default:                                                    \
      return GW_OK;                                             \
  }                                                             \
  const Exp e = bin->self;                                      \
  fold_exp(bin);                                                \
  etype##_exp(e, ret);                                          \
  return GW_OK;                                                 \
}
describe_fold_xxx(ii, m_int, num, num, int,
  case op_mod: DIV_BY_ZERO(l->num, % , r->num) break;
  CASE(shl, l->num, >>, r->num) CASE(shr, l->num, <<, r->num))
describe_fold_xxx(ff, m_float, fnum, fnum, float,)
describe_fold_xxx(if, m_float,  num, fnum, float,)
describe_fold_xxx(fi, m_float, fnum,  num, float,)

m_bool constant_folding(const Exp_Binary* bin) {
  if(constant_int(bin->lhs)) {
    if(constant_int(bin->rhs))
      CHECK_BB(fold_ii(bin))
    else if(constant_float(bin->rhs))
      CHECK_BB(fold_if(bin))
  } else if(constant_float(bin->lhs)) {
    if(constant_float(bin->rhs))
      CHECK_BB(fold_ff(bin))
    else if(constant_int(bin->rhs))
      CHECK_BB(fold_fi(bin))
  }
  return GW_OK;
}
