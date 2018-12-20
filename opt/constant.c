#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "type.h"
#include "constant.h"

#define describe_constant_xxx(name, etype, mtype)                  \
ANN m_bool constant_##name(const Exp e) {                          \
  return (e->exp_type == ae_exp_primary &&                         \
    e->d.exp_primary.primary_type == ae_primary_##etype) ||        \
    (e->exp_type == ae_exp_constprop && isa(e->type , mtype) > 0); \
}
describe_constant_xxx(int,   num,  t_int)
describe_constant_xxx(float, float, t_float)
