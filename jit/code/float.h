#include "jit/jit.h"
#include "gwjit_common.h"
#include "cc.h"
#include "q.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"
/*
JIT_CODE(float_assign) {
  CJval reg = push_reg(j, -SZ_INT);
  CJval src = JLOADR(reg, SZ_INT - SZ_FLOAT, float);
  CJval ptr = JLOADR(reg, -SZ_FLOAT, void_ptr);
  JSTORER(ptr, 0, src);
  JSTORER(reg, -SZ_FLOAT, src);
}

#define jit_describe(name, op)               \
JIT_CODE(float_##name) {                     \
  CJval reg = push_reg(j, -SZ_FLOAT);        \
  CJval src = JLOADR(reg, 0, float);         \
  CJval tgt = JLOADR(reg, -SZ_FLOAT, float); \
  CJval ret = JINSN(op, tgt, src);          \
  JSTORER(reg, -SZ_FLOAT, ret);              \
}
jit_describe(plus, add)
jit_describe(minus, sub)
jit_describe(times, mul)
jit_describe(divide, div)

JIT_CODE(float_and) {
  CJval reg = push_reg(j, SZ_INT-SZ_FLOAT*2);
  CJval one = JCONST(nint, 1);
  CJval zero = JCONST(nint, 0);
  CJval zedf = JCONSTF(0.0);
  INIT_LABEL(end)
  INIT_LABEL(nok)
  CJval lhs = JLOADR(reg, -SZ_INT, float);
  CJval ok1 = JINSN(eq, lhs, zedf);
  JINSN(branch_if, ok1, &nok);
  CJval rhs = JLOADR(reg, SZ_FLOAT-SZ_INT, float);
  CJval ok2 = JINSN(eq, rhs, zedf);
  JINSN(branch_if, ok2, &nok);
  JSTORER(reg, -SZ_INT, one);
  JINSN(branch, &end);
  JINSN(label, &nok);
  JSTORER(reg, -SZ_INT, zero);
  JINSN(label, &end);
}

JIT_CODE(float_or) {
  CJval reg = push_reg(j, SZ_INT-SZ_FLOAT*2);
  CJval one = JCONST(nint, 1);
  CJval zed = JCONST(nint, 0);
  CJval zedf = JCONSTF(0.0);
  INIT_LABEL(end)
  INIT_LABEL(ok)
  CJval lhs = JLOADR(reg, -SZ_INT, float);
  CJval ok1 = JINSN(eq, lhs, zedf);
  JINSN(branch_if_not, ok1, &ok);
  CJval rhs = JLOADR(reg, SZ_FLOAT-SZ_INT, float);
  CJval ok2 = JINSN(eq, rhs, zedf);
  JINSN(branch_if_not, ok2, &ok);
  JSTORER(reg, -SZ_INT, zed);
  JINSN(branch, &end);
  JINSN(label, &ok);
  JSTORER(reg, -SZ_INT, one);
  JINSN(label, &end);
}

#define jit_describe_logical(name, op)                \
JIT_CODE(float_##name) {                              \
  CJval reg = push_reg(j, SZ_INT-SZ_FLOAT*2);         \
  CJval src = JLOADR(reg, SZ_FLOAT-SZ_INT, float);    \
  CJval tgt = JLOADR(reg, -SZ_INT, float);            \
  CJval ret = JINSN(op, tgt, src);                   \
  CJval val = JINSN(convert, ret, jit_type_nint, 0); \
  JSTORER(reg, -SZ_INT, val);                         \
}
jit_describe_logical(eq,  eq)
jit_describe_logical(neq, ne)
jit_describe_logical(lt, lt)
jit_describe_logical(le, le)
jit_describe_logical(gt, gt)
jit_describe_logical(ge, ge)

JIT_CODE(float_negate) {
  CJval reg  = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval src = JLOADR(reg, -SZ_FLOAT, float);
  CJval ret = JINSN(neg, src);
  JSTORER(reg, -SZ_FLOAT, ret);
}

JIT_CODE(float_not) {
  CJval reg = push_reg(j, SZ_INT-SZ_FLOAT);
  CJval src = JLOADR(reg, -SZ_INT, float);
  CJval zero = JCONSTF(0.0);
  CJval ret1 = JINSN(eq, src, zero);
  JSTORER(reg, -SZ_INT, ret1);
}

JIT_CODE(float_r_assign) {
  CJval reg = push_reg(j, -SZ_INT);
  CJval ptr = JLOADR(reg, 0, void_ptr);
  CJval src = JLOADR(reg, -SZ_FLOAT, float);
  JSTORER(ptr, 0, src);
}

#define jit_describe_r(name, op)             \
JIT_CODE(float_r_##name) {                   \
  CJval reg = push_reg(j, -SZ_INT);          \
  CJval ptr = JLOADR(reg, 0, void_ptr);      \
  CJval src = JLOADR(ptr, 0, float);         \
  CJval tgt = JLOADR(reg, -SZ_FLOAT, float); \
  CJval ret = JINSN(op, src, tgt);          \
  JSTORER(ptr, 0, ret);                      \
  JSTORER(reg, -SZ_FLOAT, ret);              \
}
jit_describe_r(plus, add)
jit_describe_r(minus, sub)
jit_describe_r(times, mul)
jit_describe_r(divide, div)

JIT_CODE(int_float_assign) {
  CJval reg = push_reg(j, -SZ_FLOAT);
  CJval ptr = JLOADR(reg, -SZ_INT, void_ptr);
  CJval src = JLOADR(reg, 0, float);
  CJval ret = JINSN(convert, src, jit_type_nint, 0);
  JSTORER(ptr, 0, ret);
  JSTORER(reg, -SZ_INT, ret);
}

#define jit_describe_if(name, op)                      \
JIT_CODE(int_float_##name) {                           \
  CJval reg = push_reg(j, -SZ_INT);                    \
  CJval lhs = JLOADR(reg, -SZ_FLOAT, nint);            \
  CJval rhs = JLOADR(reg, SZ_INT-SZ_FLOAT, float);     \
  CJval val = JINSN(convert, lhs, jit_type_float, 0); \
  CJval ret = JINSN(op, val, rhs);                    \
  JSTORER(reg, -SZ_FLOAT, ret);                        \
}
jit_describe_if(plus,   add)
jit_describe_if(minus,  sub)
jit_describe_if(times,  mul)
jit_describe_if(divide, div)

JIT_CODE(int_float_and) {
  CJval reg = push_reg(j, -SZ_FLOAT);
  CJval one = JCONST(nint, 1);
  CJval zero = JCONST(nint, 0);
  CJval zedf = JCONSTF(0.0);
  INIT_LABEL(end)
  INIT_LABEL(nok)
  CJval lhs = JLOADR(reg, -SZ_INT, nint);
  CJval ok1 = JINSN(eq, lhs, zero);
  JINSN(branch_if, ok1, &nok);
  CJval rhs = JLOADR(reg, 0, float);
  CJval ok2 = JINSN(eq, rhs, zedf);
  JINSN(branch_if, ok2, &nok);
  JSTORER(reg, -SZ_INT, one);
  JINSN(branch, &end);
  JINSN(label, &nok);
  JSTORER(reg, -SZ_INT, zero);
  JINSN(label, &end);
}

JIT_CODE(int_float_or) {
  CJval reg = push_reg(j, -SZ_FLOAT);
  CJval one = JCONST(nint, 1);
  CJval zed = JCONST(nint, 0);
  CJval zedf = JCONSTF(0.0);
  INIT_LABEL(end)
  INIT_LABEL(ok)
  CJval lhs = JLOADR(reg, -SZ_INT, nint);
  CJval ok1 = JINSN(eq, lhs, zed);
  JINSN(branch_if_not, ok1, &ok);
  CJval rhs = JLOADR(reg, 0, float);
  CJval ok2 = JINSN(eq, rhs, zedf);
  JINSN(branch_if_not, ok2, &ok);
  JSTORER(reg, -SZ_INT, zed);
  JINSN(branch, &end);
  JINSN(label, &ok);
  JSTORER(reg, -SZ_INT, one);
  JINSN(label, &end);
}

#define jit_describe_logical_if(name, op)              \
JIT_CODE(int_float_##name) {                           \
  CJval reg = push_reg(j, -SZ_FLOAT);                  \
  CJval lhs = JLOADR(reg, -SZ_INT, nint);              \
  CJval ptr = JINSN(convert, lhs, jit_type_float, 0); \
  CJval rhs = JLOADR(reg, 0, float);                   \
  CJval val = JINSN(op, ptr, rhs);                    \
  CJval ret = JINSN(convert, val, jit_type_nint, 0);  \
  JSTORER(reg, -SZ_INT, ret);                          \
}
jit_describe_logical_if(eq,  eq)
jit_describe_logical_if(neq, ne)
jit_describe_logical_if(gt,  gt)
jit_describe_logical_if(ge,  ge)
jit_describe_logical_if(lt,  lt)
jit_describe_logical_if(le,  le)

JIT_CODE(int_float_r_assign) {
  CJval reg = push_reg(j, -SZ_INT*2 + SZ_FLOAT);
  CJval ptr = JLOADR(reg, SZ_INT-SZ_FLOAT, void_ptr);
  CJval tgt = JLOADR(reg, -SZ_FLOAT, nint);
  CJval ret = JINSN(convert, tgt, jit_type_float, 0);
  JSTORER(ptr, 0, ret);
  JSTORER(reg, -SZ_FLOAT, ret);
}

#define jit_describe_r_if(name, op)                    \
JIT_CODE(int_float_r_##name) {                         \
  CJval reg = push_reg(j, -SZ_INT*2 + SZ_FLOAT);       \
  CJval ptr = JLOADR(reg, SZ_INT-SZ_FLOAT, void_ptr);  \
  CJval src = JLOADR(ptr, 0, float);                   \
  CJval tgt = JLOADR(reg, -SZ_FLOAT, nint);            \
  CJval val = JINSN(convert, tgt, jit_type_float, 0); \
  CJval ret = JINSN(op, src, val);                    \
  JSTORER(ptr, 0, ret);                                \
  JSTORER(reg, -SZ_FLOAT, ret);                        \
}
jit_describe_r_if(plus,   add)
jit_describe_r_if(minus,  sub)
jit_describe_r_if(times,  mul)
jit_describe_r_if(divide, div)

JIT_CODE(float_int_assign) {
  CJval reg = push_reg(j, -SZ_INT*2 + SZ_FLOAT);
  CJval src = JLOADR(reg, SZ_INT-SZ_FLOAT, nint);
  CJval ptr = JLOADR(reg, -SZ_FLOAT, void_ptr);
  CJval ret = JINSN(convert, src, jit_type_float, 0);
  JSTORER(ptr, 0, ret);
  JSTORER(reg, -SZ_FLOAT, ret);
}

#define jit_describe_fi(name, op)                      \
JIT_CODE(float_int_##name) {                           \
  CJval reg = push_reg(j, -SZ_INT);                    \
  CJval lhs = JLOADR(reg, 0, nint);                    \
  CJval val = JINSN(convert, lhs, jit_type_float, 0); \
  CJval rhs = JLOADR(reg, -SZ_FLOAT, float);           \
  CJval ret = JINSN(op, val, rhs);                    \
  JSTORER(reg, -SZ_FLOAT, ret);                        \
}
jit_describe_fi(plus,   add)
jit_describe_fi(minus,  sub)
jit_describe_fi(times,  mul)
jit_describe_fi(divide, div)

JIT_CODE(float_int_and) {
  CJval reg = push_reg(j, -SZ_FLOAT);
  CJval one = JCONST(nint, 1);
  CJval zero = JCONST(nint, 0);
  CJval zedf = JCONSTF(0.0);
  INIT_LABEL(end)
  INIT_LABEL(nok)
  CJval lhs = JLOADR(reg, -SZ_INT, float);
  CJval ok1 = JINSN(eq, lhs, zedf);
  JINSN(branch_if, ok1, &nok);
  CJval rhs = JLOADR(reg, SZ_FLOAT-SZ_INT, nint);
  CJval ok2 = JINSN(eq, rhs, zero);
  JINSN(branch_if, ok2, &nok);
  JSTORER(reg, -SZ_INT, one);
  JINSN(branch, &end);
  JINSN(label, &nok);
  JSTORER(reg, -SZ_INT, zero);
  JINSN(label, &end);
}

JIT_CODE(float_int_or) {
  CJval reg = push_reg(j, -SZ_FLOAT);
  CJval one = JCONST(nint, 1);
  CJval zed = JCONST(nint, 0);
  CJval zedf = JCONSTF(0.0);
  INIT_LABEL(end)
  INIT_LABEL(ok)
  CJval lhs = JLOADR(reg, -SZ_INT, float);
  CJval ok1 = JINSN(eq, lhs, zedf);
  JINSN(branch_if_not, ok1, &ok);
  CJval rhs = JLOADR(reg, SZ_FLOAT-SZ_INT, nint);
  CJval ok2 = JINSN(eq, rhs, zed);
  JINSN(branch_if_not, ok2, &ok);
  JSTORER(reg, -SZ_INT, zed);
  JINSN(branch, &end);
  JINSN(label, &ok);
  JSTORER(reg, -SZ_INT, one);
  JINSN(label, &end);
}

#define jit_describe_logical_fi(name, op)              \
JIT_CODE(float_int_##name) {                           \
  CJval reg = push_reg(j, -SZ_FLOAT);                  \
  CJval lhs = JLOADR(reg, -SZ_INT, float);             \
  CJval rhs = JLOADR(reg, SZ_FLOAT-SZ_INT, nint);      \
  CJval val = JINSN(convert, lhs, jit_type_nint, 0);  \
  CJval ret = JINSN(op, val, rhs);                    \
  JSTORER(reg, -SZ_INT, ret);                          \
}
jit_describe_logical_fi(eq,  eq)
jit_describe_logical_fi(neq, ne)
jit_describe_logical_fi(gt,  gt)
jit_describe_logical_fi(ge,  ge)
jit_describe_logical_fi(lt,  lt)
jit_describe_logical_fi(le,  le)

JIT_CODE(float_int_r_assign) {
  CJval reg = push_reg(j, -SZ_FLOAT);
  CJval ptr = JLOADR(reg, SZ_FLOAT - SZ_INT, void_ptr);
  CJval tgt = JLOADR(reg, -SZ_INT, float);
  CJval ret = JINSN(convert, tgt, jit_type_nint, 0);
  JSTORER(ptr, 0, ret);
  JSTORER(reg, -SZ_INT, ret);
}

#define jit_describe_r_fi(name, op)                     \
JIT_CODE(float_int_r_##name) {                          \
  CJval reg = push_reg(j, -SZ_FLOAT);                   \
  CJval ptr = JLOADR(reg, SZ_FLOAT - SZ_INT, void_ptr); \
  CJval src = JLOADR(ptr, 0, nint);                     \
  CJval tgt = JLOADR(reg, -SZ_INT, float);              \
  CJval val = JINSN(convert, tgt, jit_type_nint, 0);   \
  CJval ret = JINSN(op, src, val);                     \
  JSTORER(ptr, 0, ret);                                 \
  JSTORER(reg, -SZ_INT, ret);                           \
}
jit_describe_r_fi(plus,   add)
jit_describe_r_fi(minus,  sub)
jit_describe_r_fi(times,  mul)
jit_describe_r_fi(divide, div)

JIT_CODE(Time_Advance) {
  CJval reg = push_reg(j, -SZ_FLOAT);
  CJval f = JLOADR(reg, -SZ_FLOAT, float);
  CJval wake = JLOADR(j->shred, JOFF(VM_Shred, wake_time), float);
  CJval sum = JINSN(add, f, wake);
  JSTORER(reg, -SZ_FLOAT, sum);
  cc_shredule(j, f);
}
*/
