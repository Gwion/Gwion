#include "jit/jit.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"

JIT_CODE(float_assign) {
  CJval reg = push_reg(cc, -SZ_INT);
  CJval src = JLOADR(reg, SZ_INT - SZ_FLOAT, float);
  CJval ptr = JLOADR(reg, -SZ_FLOAT, void_ptr);
  JSTORER(ptr, 0, src);
  JSTORER(reg, -SZ_FLOAT, src);
}

#define jit_describe(name, op)               \
JIT_CODE(Float##name) {                     \
  CJval reg = push_reg(cc, -SZ_FLOAT);        \
  CJval src = JLOADR(reg, 0, float);         \
  CJval tgt = JLOADR(reg, -SZ_FLOAT, float); \
  CJval ret = JINSN(op, tgt, src);          \
  JSTORER(reg, -SZ_FLOAT, ret);              \
}
jit_describe(Plus, add)
jit_describe(Minus, sub)
jit_describe(Times, mul)
jit_describe(Divide, div)

JIT_CODE(float_and) {
  push_reg(cc, SZ_INT-SZ_FLOAT*2);
  INIT_LABEL(end)
  INIT_LABEL(nok)
  CJval lhs = JLOADR(cc->reg, -SZ_INT, float);
  JINSN(branch_if_not, lhs, &nok);
  CJval rhs = JLOADR(cc->reg, SZ_FLOAT-SZ_INT, float);
  JINSN(branch_if_not, rhs, &nok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 1));
  JINSN(branch, &end);
  JINSN(label, &nok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 0));
  JINSN(label, &end);
}

JIT_CODE(float_or) {
  push_reg(cc, SZ_INT-SZ_FLOAT*2);
  INIT_LABEL(end)
  INIT_LABEL(ok)
  CJval lhs = JLOADR(cc->reg, -SZ_INT, float);
  JINSN(branch_if, lhs, &ok);
  CJval rhs = JLOADR(cc->reg, SZ_FLOAT-SZ_INT, float);
  JINSN(branch_if, rhs, &ok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 0));
  JINSN(branch, &end);
  JINSN(label, &ok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 1));
  JINSN(label, &end);
}

#define jit_describe_logical(name, op)                \
JIT_CODE(float_##name) {                              \
  CJval reg = push_reg(cc, SZ_INT-SZ_FLOAT*2);         \
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
  CJval src = JLOADR(cc->reg, -SZ_FLOAT, float);
  CJval ret = JINSN(neg, src);
  JSTORER(cc->reg, -SZ_FLOAT, ret);
}

JIT_CODE(float_not) {
  CJval reg = push_reg(cc, SZ_INT-SZ_FLOAT);
  CJval src = JLOADR(reg, -SZ_INT, float);
  CJval zero = JCONSTF(0.0);
  CJval ret1 = JINSN(eq, src, zero);
  JSTORER(reg, -SZ_INT, ret1);
}

JIT_CODE(float_r_assign) {
  CJval reg = push_reg(cc, -SZ_INT);
  CJval ptr = JLOADR(reg, 0, void_ptr);
  CJval src = JLOADR(reg, -SZ_FLOAT, float);
  JSTORER(ptr, 0, src);
}

#define jit_describe_r(name, op)             \
JIT_CODE(float_r_##name) {                   \
  CJval reg = push_reg(cc, -SZ_INT);          \
  CJval ptr = JLOADR(reg, 0, void_ptr);      \
  CJval src = JLOADR(ptr, 0, float);         \
  CJval tgt = JLOADR(reg, -SZ_FLOAT, float); \
  CJval ret = JINSN(op, src, tgt);          \
  JSTORER(ptr, 0, ret);                      \
  JSTORER(reg, -SZ_FLOAT, ret);              \
}
jit_describe_r(plus, add)
jit_describe_r(minus, sub)
jit_describe_r(mul, mul)
jit_describe_r(div, div)

JIT_CODE(int_float_assign) {
  CJval reg = push_reg(cc, -SZ_FLOAT);
  CJval ptr = JLOADR(reg, -SZ_INT, void_ptr);
  CJval src = JLOADR(reg, 0, float);
  CJval ret = JINSN(convert, src, jit_type_nint, 0);
  JSTORER(ptr, 0, ret);
  JSTORER(reg, -SZ_INT, ret);
}

#define jit_describe_if(name, op)                      \
JIT_CODE(int_float_##name) {                           \
  CJval reg = push_reg(cc, -SZ_INT);                    \
  CJval lhs = JLOADR(reg, -SZ_FLOAT, nint);            \
  CJval rhs = JLOADR(reg, SZ_INT-SZ_FLOAT, float);     \
  CJval val = JINSN(convert, lhs, jit_type_float, 0); \
  CJval ret = JINSN(op, val, rhs);                    \
  JSTORER(reg, -SZ_FLOAT, ret);                        \
}
jit_describe_if(plus,   add)
jit_describe_if(minus,  sub)
jit_describe_if(mul,  mul)
jit_describe_if(div, div)

JIT_CODE(int_float_and) {
  push_reg(cc, -SZ_FLOAT);
  INIT_LABEL(end)
  INIT_LABEL(nok)
  CJval lhs = JLOADR(cc->reg, -SZ_INT, nint);
  JINSN(branch_if_not, lhs, &nok);
  CJval rhs = JLOADR(cc->reg, 0, float);
  JINSN(branch_if_not, rhs, &nok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 1));
  JINSN(branch, &end);
  JINSN(label, &nok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 0));
  JINSN(label, &end);
}

JIT_CODE(int_float_or) {
  push_reg(cc, -SZ_FLOAT);
  INIT_LABEL(end)
  INIT_LABEL(ok)
  CJval lhs = JLOADR(cc->reg, -SZ_INT, nint);
  JINSN(branch_if, lhs, &ok);
  CJval rhs = JLOADR(cc->reg, 0, float);
  JINSN(branch_if, rhs, &ok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 0));
  JINSN(branch, &end);
  JINSN(label, &ok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 1));
  JINSN(label, &end);
}

#define jit_describe_logical_if(name, op)              \
JIT_CODE(int_float_##name) {                           \
  CJval reg = push_reg(cc, -SZ_FLOAT);                  \
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
  CJval reg = push_reg(cc, -SZ_INT*2 + SZ_FLOAT);
  CJval ptr = JLOADR(reg, SZ_INT-SZ_FLOAT, void_ptr);
  CJval tgt = JLOADR(reg, -SZ_FLOAT, nint);
  CJval ret = JINSN(convert, tgt, jit_type_float, 0);
  JSTORER(ptr, 0, ret);
  JSTORER(reg, -SZ_FLOAT, ret);
}

#define jit_describe_r_if(name, op)                    \
JIT_CODE(int_float_r_##name) {                         \
  CJval reg = push_reg(cc, -SZ_INT*2 + SZ_FLOAT);       \
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
jit_describe_r_if(mul,  mul)
jit_describe_r_if(div, div)

JIT_CODE(float_int_assign) {
  CJval reg = push_reg(cc, -SZ_INT*2 + SZ_FLOAT);
  CJval src = JLOADR(reg, SZ_INT-SZ_FLOAT, nint);
  CJval ptr = JLOADR(reg, -SZ_FLOAT, void_ptr);
  CJval ret = JINSN(convert, src, jit_type_float, 0);
  JSTORER(ptr, 0, ret);
  JSTORER(reg, -SZ_FLOAT, ret);
}

#define jit_describe_fi(name, op)                      \
JIT_CODE(float_int_##name) {                           \
  CJval reg = push_reg(cc, -SZ_INT);                    \
  CJval lhs = JLOADR(reg, 0, nint);                    \
  CJval val = JINSN(convert, lhs, jit_type_float, 0); \
  CJval rhs = JLOADR(reg, -SZ_FLOAT, float);           \
  CJval ret = JINSN(op, val, rhs);                    \
  JSTORER(reg, -SZ_FLOAT, ret);                        \
}
jit_describe_fi(plus,   add)
jit_describe_fi(minus,  sub)
jit_describe_fi(mul,  mul)
jit_describe_fi(div, div)

JIT_CODE(float_int_and) {
  push_reg(cc, -SZ_FLOAT);
  INIT_LABEL(end)
  INIT_LABEL(nok)
  CJval lhs = JLOADR(cc->reg, -SZ_INT, float);
  JINSN(branch_if_not, lhs, &nok);
  CJval rhs = JLOADR(cc->reg, SZ_FLOAT-SZ_INT, nint);
  JINSN(branch_if_not, rhs, &nok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 1));
  JINSN(branch, &end);
  JINSN(label, &nok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 0));
  JINSN(label, &end);
}

JIT_CODE(float_int_or) {
  push_reg(cc, -SZ_FLOAT);
  INIT_LABEL(end)
  INIT_LABEL(ok)
  CJval lhs = JLOADR(cc->reg, -SZ_INT, float);
  JINSN(branch_if, lhs, &ok);
  CJval rhs = JLOADR(cc->reg, SZ_FLOAT-SZ_INT, nint);
  JINSN(branch_if, rhs, &ok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 0));
  JINSN(branch, &end);
  JINSN(label, &ok);
  JSTORER(cc->reg, -SZ_INT, JCONST(nuint, 1));
  JINSN(label, &end);
}

#define jit_describe_logical_fi(name, op)              \
JIT_CODE(float_int_##name) {                           \
  CJval reg = push_reg(cc, -SZ_FLOAT);                  \
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
  CJval reg = push_reg(cc, -SZ_FLOAT);
  CJval ptr = JLOADR(reg, SZ_FLOAT - SZ_INT, void_ptr);
  CJval tgt = JLOADR(reg, -SZ_INT, float);
  CJval ret = JINSN(convert, tgt, jit_type_nint, 0);
  JSTORER(ptr, 0, ret);
  JSTORER(reg, -SZ_INT, ret);
}

#define jit_describe_r_fi(name, op)                     \
JIT_CODE(float_int_r_##name) {                          \
  CJval reg = push_reg(cc, -SZ_FLOAT);                   \
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
jit_describe_r_fi(mul,  mul)
jit_describe_r_fi(div, div)

JIT_CODE(Time_Advance) {
  CJval reg = push_reg(cc, -SZ_FLOAT);
  CJval f = JLOADR(reg, -SZ_FLOAT, float);
  CJval wake = JLOADR(cc->shred, JOFF(VM_Shred, wake_time), float);
  CJval sum = JINSN(add, f, wake);
  JSTORER(reg, -SZ_FLOAT, sum);
  cc_shredule(cc, f);
}

JIT_CODE(CastI2F) {
  CJval reg = push_reg(cc, -(SZ_INT-SZ_FLOAT));
  CJval val = JLOADR(reg, -SZ_FLOAT, nint);
  CJval ret = JINSN(convert, val, jit_type_float, 0);
  JSTORER(reg, -SZ_FLOAT, ret);
}

JIT_CODE(CastF2I) {
  CJval reg = push_reg(cc, -(SZ_FLOAT-SZ_INT));
  CJval val = JLOADR(reg, -SZ_INT, float);
  CJval ret = JINSN(convert, val, jit_type_nint, 0);
  JSTORER(reg, -SZ_INT, ret);
}

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_float(struct Jit* j) {
  JIT_IMPORT(float_assign)
  JIT_IMPORT(FloatPlus)
  JIT_IMPORT(FloatMinus)
  JIT_IMPORT(FloatTimes)
  JIT_IMPORT(FloatDivide)
  JIT_IMPORT(float_or)
  JIT_IMPORT(float_and)
  JIT_IMPORT(float_eq)
  JIT_IMPORT(float_neq)
  JIT_IMPORT(float_gt)
  JIT_IMPORT(float_ge)
  JIT_IMPORT(float_lt)
  JIT_IMPORT(float_le)
  JIT_IMPORT(float_negate)
  JIT_IMPORT(float_not)
  JIT_IMPORT(float_r_assign)
  JIT_IMPORT(float_r_plus)
  JIT_IMPORT(float_r_minus)
  JIT_IMPORT(float_r_mul)
  JIT_IMPORT(float_r_div)
  JIT_IMPORT(int_float_assign)
  JIT_IMPORT(int_float_plus)
  JIT_IMPORT(int_float_minus)
  JIT_IMPORT(int_float_mul)
  JIT_IMPORT(int_float_div)
  JIT_IMPORT(int_float_and)
  JIT_IMPORT(int_float_or)
  JIT_IMPORT(int_float_eq)
  JIT_IMPORT(int_float_neq)
  JIT_IMPORT(int_float_gt)
  JIT_IMPORT(int_float_ge)
  JIT_IMPORT(int_float_lt)
  JIT_IMPORT(int_float_le)
  JIT_IMPORT(int_float_r_assign)
  JIT_IMPORT(int_float_r_plus)
  JIT_IMPORT(int_float_r_minus)
  JIT_IMPORT(int_float_r_mul)
  JIT_IMPORT(int_float_r_div)
  JIT_IMPORT(float_int_assign)
  JIT_IMPORT(float_int_plus)
  JIT_IMPORT(float_int_minus)
  JIT_IMPORT(float_int_mul)
  JIT_IMPORT(float_int_div)
  JIT_IMPORT(float_int_and)
  JIT_IMPORT(float_int_or)
  JIT_IMPORT(float_int_eq)
  JIT_IMPORT(float_int_neq)
  JIT_IMPORT(float_int_gt)
  JIT_IMPORT(float_int_ge)
  JIT_IMPORT(float_int_lt)
  JIT_IMPORT(float_int_le)
  JIT_IMPORT(float_int_r_assign)
  JIT_IMPORT(float_int_r_plus)
  JIT_IMPORT(float_int_r_minus)
  JIT_IMPORT(float_int_r_mul)
  JIT_IMPORT(float_int_r_div)
  JIT_IMPORT(Time_Advance)
  JIT_IMPORT(CastI2F)
  JIT_IMPORT(CastF2I)
}
