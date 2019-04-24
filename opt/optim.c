#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "optim.h"
#include "constant.h"

m_bool optimize_const(const Env env, const Exp_Binary* bin) {
  return constant_folding(env, bin);
}
