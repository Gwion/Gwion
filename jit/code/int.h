#include "jit/jit.h"
#include "gwjit_common.h"
#include "cc.h"
#include "q.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"
/*
#define TEST0_JIT(val) \
INIT_LABEL(lbl) \
CJval zero = JCONST(nint, 0);\
CJval cond = JINSN(eq, val, zero);\
JINSN(branch_if_not, cond, &lbl);\
cc_except(j, "ZeroDivideException");\
JINSN(label, &lbl);

#define _describe_jit(prefix, sz, type, name, action, ...)                 \
JIT_CODE(prefix##_##name) {                                                \
  CJval reg = push_reg(j, -SZ_INT);\
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

JIT_CODE(int_not) {
  CJval reg  = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval null = JCONST(nint, 0);
  CJval val  = JLOADR(reg, -SZ_INT, nint);
  CJval ret  = JINSN(ne, val, null);
  JSTORER(reg, -SZ_INT, ret);
}

JIT_CODE(int_negate) {
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval val = JLOADR(reg, -SZ_INT, nint);
  CJval ret = JINSN(neg, val);
  JSTORER(reg, -SZ_INT, ret);
}

#define describe_post_jit(name, op) \
JIT_CODE(int_post_##name) { \
  CJval reg  = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr); \
  CJval ptr  = JLOADR(reg, -SZ_INT, void_ptr); \
  CJval val  = JLOADR(ptr, 0, nint); \
  CJval null = JCONST(nint, 1); \
  JSTORER(reg, -SZ_INT, val); \
  CJval ret = JINSN(op, val, null); \
  JSTORER(ptr, 0, ret); \
}

describe_post_jit(inc, add)
describe_post_jit(dec, sub)

#define describe_pre_jit(name, op) \
JIT_CODE(int_pre_##name) { \
  CJval reg  = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr); \
  CJval ptr = JLOADR(reg, -SZ_INT, void_ptr); \
  CJval val = JLOADR(ptr, 0, nint); \
  CJval null = JCONST(nint, 1); \
  CJval ret = JINSN(op, val, null); \
  JSTORER(ptr, 0, ret); \
  JSTORER(reg, -SZ_INT, ret); \
}

describe_pre_jit(inc, add)
describe_pre_jit(dec, sub)

JIT_CODE(int_cmp) {
  CJval reg  = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval val = JLOADR(reg, -SZ_INT, nint);
  CJval ret = JINSN(not, val);
  JSTORER(reg, -SZ_INT, ret);
}

JIT_CODE(int_assign) {
  CJval reg  = push_reg(j, -SZ_INT);
  CJval ret = JLOADR(reg, 0, nint);
  CJval ptr = JLOADR(reg, -SZ_INT, void_ptr);
  JSTORER(ptr, 0, ret);
  JSTORER(reg, -SZ_INT, ret);
}

JIT_CODE(int_r_assign) {
  CJval reg  = push_reg(j, -SZ_INT);
  CJval ret = JLOADR(reg, -SZ_INT, nint);
  CJval ptr = JLOADR(reg, 0, void_ptr);
  JSTORER(ptr, 0, ret);
}

JIT_CODE(int_and) {
  CJval reg  = push_reg(j, -SZ_INT);
  CJval one = JCONST(nint, 1);
  CJval zero = JCONST(nint, 0);
  INIT_LABEL(end)
  INIT_LABEL(nok)
  CJval lhs = JLOADR(reg, -SZ_INT, void_ptr);
  CJval ok1 = JINSN(eq, lhs, zero);
  JINSN(branch_if, ok1, &nok);
  CJval rhs = JLOADR(reg, 0, void_ptr);
  CJval ok2 = JINSN(eq, rhs, zero);
  JINSN(branch_if, ok2, &nok);
  JSTORER(reg, -SZ_INT, one);
  JINSN(branch, &end);
  JINSN(label, &nok);
  JSTORER(reg, -SZ_INT, zero);
  JINSN(label, &end);
}

JIT_CODE(int_or) {
  CJval reg  = push_reg(j, -SZ_INT);
  CJval one = JCONST(nint, 1);
  CJval zed = JCONST(nint, 0);
  INIT_LABEL(end)
  INIT_LABEL(ok)
  CJval lhs = JLOADR(reg, -SZ_INT, void_ptr);
  CJval ok1 = JINSN(eq, lhs, zed);
  JINSN(branch_if_not, ok1, &ok);
  CJval rhs = JLOADR(reg, 0, void_ptr);
  CJval ok2 = JINSN(eq, rhs, zed);
  JINSN(branch_if_not, ok2, &ok);
  JSTORER(reg, -SZ_INT, zed);
  JINSN(branch, &end);
  JINSN(label, &ok);
  JSTORER(reg, -SZ_INT, one);
  JINSN(label, &end);
}
*/
