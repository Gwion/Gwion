#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "type.h"
#include "constant.h"

ANN m_bool constant_int(const Exp e) {
  return (e->exp_type == ae_exp_primary &&
    e->d.exp_primary.primary_type == ae_primary_num) ||
    (e->exp_type == ae_exp_constprop && isa(e->type , t_int) > 0);
}

ANN m_bool constant_float(const Exp e) {
  return (e->exp_type == ae_exp_primary &&
    e->d.exp_primary.primary_type == ae_primary_float) ||
    (e->exp_type == ae_exp_constprop && isa(e->type , t_float) > 0);
}
