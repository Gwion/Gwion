#include "jit/jit.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"

#define TEST0_JIT(val) \
cc_check(cc, val, "ZeroDivideException");

#define _describe_jit(prefix, sz, type, name, action, ...)                 \
JIT_CODE(prefix##_##name) {                                                \
  CJval reg = push_reg(cc, -SZ_INT);\
  CJval lhs = JLOADR(reg, -SZ_INT, nint);                      \
  CJval rhs = JLOADR(reg, 0, type);                            \
   __VA_ARGS__                                                             \
  action                                                                   \
}

#define describe_jit(name, op, ...) _describe_jit(int, SZ_INT, nint, name, {\
  CJval ret = JINSN(op, lhs, rhs); \
  JSTORER(reg, -SZ_INT, ret); }, __VA_ARGS__)

#define describe_r_jit(name, op, ...) _describe_jit(int_r, SZ_INT, void_ptr, name, {\
  CJval val = JLOADR(rhs, 0, nint);\
  CJval ret = JINSN(op, val, lhs);\
  JSTORER(rhs, 0, ret); \
  JSTORER(reg, -SZ_INT, ret);\
},__VA_ARGS__)

#define describe_logical_jit(name, op) _describe_jit(int, SZ_INT, nint, name, {\
  CJval ret = JINSN(op, lhs, rhs); \
  JSTORER(reg, -SZ_INT, ret);},)

describe_jit(plus,   add,)
describe_jit(minus,  sub,)
describe_jit(times,  mul,)
describe_jit(divide, div, TEST0_JIT(rhs))
describe_jit(modulo, rem, TEST0_JIT(rhs))

describe_logical_jit(eq,    eq)
describe_logical_jit(neq,   ne)
describe_logical_jit(gt,    gt)
describe_logical_jit(ge,    ge)
describe_logical_jit(lt,    lt)
describe_logical_jit(le,    le)
describe_logical_jit(sl,    shl)
describe_logical_jit(sr,    shr)
describe_logical_jit(sand,  and)
describe_logical_jit(sor,   or)
describe_logical_jit(xor,   xor)

describe_r_jit(plus,   add,)
describe_r_jit(minus,  sub,)
describe_r_jit(times,  mul,)
describe_r_jit(divide, div, TEST0_JIT(lhs))
describe_r_jit(modulo, rem, TEST0_JIT(lhs))
describe_r_jit(sl,     shl,)
describe_r_jit(sr,     shr,)
describe_r_jit(sand,   and,)
describe_r_jit(sor,    or,)
describe_r_jit(sxor,   xor,)

JIT_CODE(IntNot) {
  CJval null = JCONST(nint, 0);
  CJval val  = JLOADR(cc->reg, -SZ_INT, nint);
  CJval ret  = JINSN(ne, val, null);
  JSTORER(cc->reg, -SZ_INT, ret);
}

JIT_CODE(int_negate) {
  CJval val = JLOADR(cc->reg, -SZ_INT, nint);
  CJval ret = JINSN(neg, val);
  JSTORER(cc->reg, -SZ_INT, ret);
}

#define describe_post_jit(name, op) \
JIT_CODE(int_post_##name) { \
  CJval ptr  = JLOADR(cc->reg, -SZ_INT, void_ptr); \
  CJval val  = JLOADR(ptr, 0, nint); \
  CJval null = JCONST(nint, 1); \
  JSTORER(cc->reg, -SZ_INT, val); \
  CJval ret = JINSN(op, val, null); \
  JSTORER(ptr, 0, ret); \
}

describe_post_jit(inc, add)
describe_post_jit(dec, sub)

#define describe_pre_jit(name, op) \
JIT_CODE(int_pre_##name) { \
  CJval ptr = JLOADR(cc->reg, -SZ_INT, void_ptr); \
  CJval val = JLOADR(ptr, 0, nint); \
  CJval null = JCONST(nint, 1); \
  CJval ret = JINSN(op, val, null); \
  JSTORER(ptr, 0, ret); \
  JSTORER(cc->reg, -SZ_INT, ret); \
}

describe_pre_jit(inc, add)
describe_pre_jit(dec, sub)

JIT_CODE(int_cmp) {
  CJval val = JLOADR(cc->reg, -SZ_INT, nint);
  CJval ret = JINSN(not, val);
  JSTORER(cc->reg, -SZ_INT, ret);
}

JIT_CODE(int_assign) {
  CJval reg  = push_reg(cc, -SZ_INT);
  CJval ret = JLOADR(reg, 0, nint);
  CJval ptr = JLOADR(reg, -SZ_INT, void_ptr);
  JSTORER(ptr, 0, ret);
  JSTORER(reg, -SZ_INT, ret);
}

JIT_CODE(int_r_assign) {
  CJval reg  = push_reg(cc, -SZ_INT);
  CJval ret = JLOADR(reg, -SZ_INT, nint);
  CJval ptr = JLOADR(reg, 0, void_ptr);
  JSTORER(ptr, 0, ret);
}

JIT_CODE(int_and) {
  push_reg(cc, -SZ_INT);
  INIT_LABEL(end)
  INIT_LABEL(nok)
  CJval lhs = JLOADR(cc->reg, -SZ_INT, void_ptr);
  JINSN(branch_if_not, lhs, &nok);
  CJval rhs = JLOADR(cc->reg, 0, void_ptr);
  JINSN(branch_if_not, rhs, &nok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 1));
  JINSN(branch, &end);
  JINSN(label, &nok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 0));
  JINSN(label, &end);
}

JIT_CODE(int_or) {
  push_reg(cc, -SZ_INT);
  INIT_LABEL(end)
  INIT_LABEL(ok)
  CJval lhs = JLOADR(cc->reg, -SZ_INT, void_ptr);
  JINSN(branch_if, lhs, &ok);
  CJval rhs = JLOADR(cc->reg, 0, void_ptr);
  JINSN(branch_if, rhs, &ok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 0));
  JINSN(branch, &end);
  JINSN(label, &ok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 1));
  JINSN(label, &end);
}
#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_int(struct Jit* j) {
  JIT_IMPORT(int_assign)
  JIT_IMPORT(int_negate)
  JIT_IMPORT(int_cmp)
  JIT_IMPORT(IntNot)
  JIT_IMPORT(int_r_assign)
  JIT_IMPORT(int_plus)
  JIT_IMPORT(int_minus)
  JIT_IMPORT(int_times)
  JIT_IMPORT(int_divide)
  JIT_IMPORT(int_modulo)
  JIT_IMPORT(int_and)
  JIT_IMPORT(int_or)
  JIT_IMPORT(int_eq)
  JIT_IMPORT(int_neq)
  JIT_IMPORT(int_gt)
  JIT_IMPORT(int_ge)
  JIT_IMPORT(int_lt)
  JIT_IMPORT(int_le)
  JIT_IMPORT(int_sl)
  JIT_IMPORT(int_sr)
  JIT_IMPORT(int_sand)
  JIT_IMPORT(int_sor)
  JIT_IMPORT(int_xor)
  JIT_IMPORT(int_pre_inc)
  JIT_IMPORT(int_pre_dec)
  JIT_IMPORT(int_post_inc)
  JIT_IMPORT(int_post_dec)
  JIT_IMPORT(int_r_plus)
  JIT_IMPORT(int_r_minus)
  JIT_IMPORT(int_r_times)
  JIT_IMPORT(int_r_divide)
  JIT_IMPORT(int_r_modulo)
  JIT_IMPORT(int_r_sl)
  JIT_IMPORT(int_r_sr)
  JIT_IMPORT(int_r_sor)
  JIT_IMPORT(int_r_sxor)
  JIT_IMPORT(int_r_sand)
}
