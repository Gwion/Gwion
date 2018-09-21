#include <string.h>
#include <jit/jit.h>
#include "map.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"

JIT_CODE(complex_assign) {
  push_reg(cc, -SZ_INT);
  CJval rea = JLOADR(cc->reg, SZ_INT - SZ_COMPLEX , float);
  CJval img = JLOADR(cc->reg, SZ_INT - SZ_FLOAT, float);
  CJval ptr = JLOADR(cc->reg, -SZ_COMPLEX, void_ptr);
  JSTORER(ptr, 0, rea);
  JSTORER(ptr, SZ_FLOAT, img);
  JSTORER(cc->reg, -SZ_COMPLEX, rea);
  JSTORER(cc->reg, -SZ_FLOAT, img);
}

#define jit_describe1(name, op)                                \
JIT_CODE(complex_##name) {                                     \
  push_reg(cc, -SZ_COMPLEX);                        \
  CJval lhs = JADDR(cc->reg, -SZ_COMPLEX);                         \
  CJval lreal = JLOADR(lhs, 0, float);                         \
  CJval limag = JLOADR(lhs, SZ_FLOAT, float);                  \
  CJval rhs   = JADDR(cc->reg, 0);                                 \
  CJval rreal = JLOADR(rhs, 0, float);                         \
  CJval rimag = JLOADR(rhs, SZ_FLOAT, float);                  \
  CJval real = JINSN(op, lreal, rreal);                       \
  CJval imag = JINSN(op, limag, rimag);                       \
  JSTORER(cc->reg, -SZ_COMPLEX, real);                             \
  JSTORER(cc->reg, -SZ_FLOAT, imag);                               \
}
jit_describe1(plus,   add)
jit_describe1(minus,  sub)

JIT_CODE(complex_mul) {
  push_reg(cc, -SZ_COMPLEX);
  CJval lhs   = JADDR(cc->reg, -SZ_COMPLEX);
  CJval lreal = JLOADR(lhs, 0, float);
  CJval limag = JLOADR(lhs, SZ_FLOAT, float);
  CJval rhs   = JADDR(cc->reg, 0);
  CJval rreal = JLOADR(rhs, 0, float);
  CJval rimag = JLOADR(rhs, SZ_FLOAT, float);
  CJval real1 = JINSN(mul, lreal, rreal);
  CJval real2 = JINSN(mul, limag, rimag);
  CJval real  = JINSN(sub, real1, real2);
  CJval imag1 = JINSN(mul, lreal, rimag);
  CJval imag2 = JINSN(mul, limag, rreal);
  CJval imag  = JINSN(add, imag1, imag2);
  JSTORER(cc->reg, -SZ_COMPLEX, real);
  JSTORER(cc->reg, -SZ_FLOAT, imag);
}

JIT_CODE(complex_div) {
  push_reg(cc, -SZ_COMPLEX);
  CJval lhs   = JADDR(cc->reg, -SZ_COMPLEX);
  CJval lreal = JLOADR(lhs, 0, float);
  CJval limag = JLOADR(lhs, SZ_FLOAT, float);
  CJval rhs   = JADDR(cc->reg, 0);
  CJval rreal = JLOADR(rhs, 0, float);
  CJval rimag = JLOADR(rhs, SZ_FLOAT, float);
  CJval real01 = JINSN(mul, lreal, rreal);
  CJval real02 = JINSN(mul, limag, rimag);
  CJval real0  = JINSN(add, real01, real02);
  CJval real11 = JINSN(mul, rreal, rreal);
  CJval real12 = JINSN(mul, rimag, rimag);
  CJval real1  = JINSN(add, real11, real12);
  CJval imag1 = JINSN(mul,  limag, rreal);
  CJval imag2 = JINSN(mul,  lreal, rimag);
  CJval imag0  = JINSN(sub,  imag1, imag2);
  CJval real  = JINSN(div,  real0, real1);
  CJval imag  = JINSN(div,  imag0, real1);
  JSTORER(cc->reg, -SZ_COMPLEX, real);
  JSTORER(cc->reg, -SZ_FLOAT, imag);
}

JIT_CODE(complex_r_assign) {
  push_reg(cc, -SZ_INT);
  CJval rea = JLOADR(cc->reg, -SZ_COMPLEX , float);
  CJval img = JLOADR(cc->reg, -SZ_FLOAT, float);
  CJval ptr = JLOADR(cc->reg, 0, void_ptr);
  JSTORER(ptr, 0, rea);
  JSTORER(ptr, SZ_FLOAT, img);
  JSTORER(cc->reg, -SZ_COMPLEX, rea);
  JSTORER(cc->reg, -SZ_FLOAT, img);
}

#define jit_describe_r1(name, op)                              \
JIT_CODE(complex_r_##name) {                                   \
  push_reg(cc, -SZ_COMPLEX);                        \
  CJval lhs   = JADDR(cc->reg, -SZ_COMPLEX);                       \
  CJval lreal = JLOADR(lhs, 0, float);                         \
  CJval limag = JLOADR(lhs, SZ_FLOAT, float);                  \
  CJval rhs   = JLOADR(cc->reg, 0, void_ptr);                      \
  CJval rreal = JLOADR(rhs, 0, float);                         \
  CJval rimag = JLOADR(rhs, SZ_FLOAT, float);                  \
  CJval real = JINSN(op, rreal, lreal);                       \
  CJval imag = JINSN(op, rimag, limag);                       \
  CJval ptr = JLOADR(cc->reg, 0, void_ptr);                        \
  JSTORER(ptr, 0, real);                                       \
  JSTORER(ptr, SZ_FLOAT, imag);                                \
  JSTORER(cc->reg, -SZ_COMPLEX, real);                             \
  JSTORER(cc->reg, -SZ_FLOAT, imag);                               \
}
jit_describe_r1(plus,   add)
jit_describe_r1(minus,  sub)

JIT_CODE(complex_r_mul) {
  push_reg(cc, -SZ_COMPLEX);
  CJval lhs   = JADDR(cc->reg, -SZ_COMPLEX);
  CJval lreal = JLOADR(lhs, 0, float);
  CJval limag = JLOADR(lhs, SZ_FLOAT, float);
  CJval rhs   = JLOADR(cc->reg, 0, void_ptr);
  CJval rreal = JLOADR(rhs, 0, float);
  CJval rimag = JLOADR(rhs, SZ_FLOAT, float);
  CJval real1 = JINSN(mul, rreal, lreal);
  CJval real2 = JINSN(mul, rimag, limag);
  CJval real  = JINSN(sub, real1, real2);
  CJval imag1 = JINSN(mul, rreal, limag);
  CJval imag2 = JINSN(mul, rimag, lreal);
  CJval imag  = JINSN(add, imag1, imag2);
  CJval ptr = JLOADR(cc->reg, 0, void_ptr);
  JSTORER(ptr, 0, real);
  JSTORER(ptr, SZ_FLOAT, imag);
  JSTORER(cc->reg, -SZ_COMPLEX, real);
  JSTORER(cc->reg, -SZ_FLOAT, imag);
}

JIT_CODE(complex_r_div) {
  push_reg(cc, -SZ_COMPLEX);
  CJval lhs   = JADDR(cc->reg, -SZ_COMPLEX);
  CJval lreal = JLOADR(lhs, 0, float);
  CJval limag = JLOADR(lhs, SZ_FLOAT, float);
  CJval rhs   = JLOADR(cc->reg, 0, void_ptr);
  CJval rreal = JLOADR(rhs, 0, float);
  CJval rimag = JLOADR(rhs, SZ_FLOAT, float);
  CJval real01 = JINSN(mul, lreal, rreal);
  CJval real02 = JINSN(mul, limag, rimag);
  CJval real0  = JINSN(add, real01, real02);
  CJval real11 = JINSN(mul, rreal, rreal);
  CJval real12 = JINSN(mul, rimag, rimag);
  CJval real1  = JINSN(add, real11, real12);
  CJval imag1 = JINSN(mul,  limag, rreal);
  CJval imag2 = JINSN(mul,  lreal, rimag);
  CJval imag0  = JINSN(sub,  imag1, imag2);
  CJval real  = JINSN(div,  real0, real1);
  CJval imag  = JINSN(div,  imag0, real1);
  CJval ptr = JLOADR(cc->reg, 0, void_ptr);
  JSTORER(ptr, 0, real);
  JSTORER(ptr, SZ_FLOAT, imag);
  JSTORER(cc->reg, -SZ_COMPLEX, real);
  JSTORER(cc->reg, -SZ_FLOAT, imag);
}

JIT_CODE(ComplexReal) {
  if(!instr->m_val) {
    CJval rhs   = JLOADR(cc->reg, -SZ_INT, void_ptr);
    CJval real  = JLOADR(rhs, 0, float);
    JSTORER(cc->reg, -SZ_INT, real);
    push_reg(cc, SZ_FLOAT - SZ_INT);
  }
}

JIT_CODE(ComplexImag) {
  CJval ptr = JADDR(cc->reg, -SZ_INT);
  if(instr->m_val) {
    CJval rhs   = JLOADR(cc->reg, -SZ_INT, void_ptr);
    CJval imag  = JLOADR(rhs, SZ_FLOAT, float);
    CJval val = JINSN(address_of, imag);
    JSTORER(cc->reg, -SZ_INT, val);
  } else {
    CJval rhs   = JLOADR(ptr, 0, void_ptr);
    CJval imag  = JLOADR(rhs, SZ_FLOAT, float);
    JSTORER(cc->reg, -SZ_INT, imag);
    push_reg(cc, SZ_FLOAT - SZ_INT);
  }
}
#ifdef USE_DOUBLE
#define hypot hypot
#else
#define hypot hypotf
#endif

#define jit_polar_def1(name, op)\
JIT_CODE(polar_##name) {\
  push_reg(cc, -SZ_COMPLEX);\
  CJval are = JLOADR(cc->reg, -SZ_COMPLEX, float);\
  CJval aim = JLOADR(cc->reg, -SZ_FLOAT,   float);\
  CJval bre = JLOADR(cc->reg, 0,           float);\
  CJval bim = JLOADR(cc->reg, SZ_FLOAT,    float);\
  CJval cosaim = JINSN(cos, aim);\
  CJval sinaim = JINSN(sin, aim);\
  CJval cosbim = JINSN(cos, bim);\
  CJval sinbim = JINSN(sin, bim);\
  CJval re00   = JINSN(mul, are,  cosaim);\
  CJval re01   = JINSN(mul, bre,  cosbim);\
  CJval re0    = JINSN(op,  re00, re01);\
  CJval im00   = JINSN(mul, are,  sinaim);\
  CJval im01   = JINSN(mul, bre,  sinbim);\
  CJval im0    = JINSN(op,  im00, im01);\
  CJval arg[] = { re0, im0 };\
  CJval re     = CALL_NATIVE2(hypot, "fff", arg);\
  CJval im     = JINSN(atan2, im0, re0);\
  JSTORER(cc->reg, -SZ_COMPLEX, re);\
  JSTORER(cc->reg, -SZ_FLOAT, im);\
}
jit_polar_def1(plus,  add)
jit_polar_def1(minus, sub)

#define jit_polar_def2(name, op1, op2)\
JIT_CODE(polar_##name) {\
  push_reg(cc, -SZ_COMPLEX);\
  CJval are = JLOADR(cc->reg, -SZ_COMPLEX, float);\
  CJval aim = JLOADR(cc->reg, -SZ_FLOAT,   float);\
  CJval bre = JLOADR(cc->reg, 0,           float);\
  CJval bim = JLOADR(cc->reg, SZ_FLOAT,    float);\
  CJval mag = JINSN(op1, are, bre);          \
  CJval pha = JINSN(op2, aim, bim);          \
  JSTORER(cc->reg, -SZ_COMPLEX, mag);\
  JSTORER(cc->reg, -SZ_FLOAT, pha);\
}
jit_polar_def2(mul,  mul, add)
jit_polar_def2(div, div, sub)

#define jit_polar_def1_r(name, op)\
JIT_CODE(polar_##name##_r) {\
  push_reg(cc, -SZ_INT);\
  CJval are = JLOADR(cc->reg, -SZ_COMPLEX, float);\
  CJval aim = JLOADR(cc->reg, -SZ_FLOAT,   float);\
  CJval ptr = JLOADR(cc->reg, 0,           void_ptr);\
  CJval bre = JLOADR(ptr, 0,           float);\
  CJval bim = JLOADR(ptr, SZ_FLOAT,    float);\
  CJval cosaim = JINSN(cos, aim);\
  CJval sinaim = JINSN(sin, aim);\
  CJval cosbim = JINSN(cos, bim);\
  CJval sinbim = JINSN(sin, bim);\
  CJval re00   = JINSN(mul, are,  cosaim);\
  CJval re01   = JINSN(mul, bre,  cosbim);\
  CJval re0    = JINSN(op,  re00, re01);\
  CJval im00   = JINSN(mul, are,  sinaim);\
  CJval im01   = JINSN(mul, bre,  sinbim);\
  CJval im0    = JINSN(op,  im00, im01);\
  CJval arg[] = { re0, im0 };\
  CJval re     = CALL_NATIVE2(hypot, "fff", arg);\
  CJval im     = JINSN(atan2, im0, re0);\
  JSTORER(ptr, 0, re);\
  JSTORER(ptr, SZ_FLOAT, im);\
  JSTORER(cc->reg, -SZ_COMPLEX, re);\
  JSTORER(cc->reg, -SZ_FLOAT, im);\
}
jit_polar_def1_r(plus,  add)
jit_polar_def1_r(minus, sub)

#define jit_polar_def2_r(name, op1, op2)\
JIT_CODE(polar_##name##_r) {\
  push_reg(cc, -SZ_COMPLEX);\
  CJval are = JLOADR(cc->reg, -SZ_COMPLEX, float);\
  CJval aim = JLOADR(cc->reg, -SZ_FLOAT,   float);\
  CJval ptr = JLOADR(cc->reg, 0,           void_ptr);\
  CJval bre = JLOADR(ptr, 0,           float);\
  CJval bim = JLOADR(ptr, SZ_FLOAT,    float);\
  CJval mag = JINSN(op1, are, bre);          \
  CJval pha = JINSN(op2, aim, bim);          \
  JSTORER(ptr, 0, mag);\
  JSTORER(ptr, SZ_FLOAT, pha);\
  JSTORER(cc->reg, -SZ_COMPLEX, mag);\
  JSTORER(cc->reg, -SZ_FLOAT, pha);\
}
jit_polar_def2_r(mul,  mul, add)
jit_polar_def2_r(div, div, sub)

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_complex(struct Jit* j) {
  JIT_IMPORT(complex_assign)
  JIT_IMPORT(complex_plus)
  JIT_IMPORT(complex_minus)
  JIT_IMPORT(complex_mul)
  JIT_IMPORT(complex_div)
  JIT_IMPORT(complex_r_assign)
  JIT_IMPORT(complex_r_plus)
  JIT_IMPORT(complex_r_minus)
  JIT_IMPORT(complex_r_mul)
  JIT_IMPORT(complex_r_div)
  JIT_IMPORT(ComplexReal)
  JIT_IMPORT(ComplexImag)
  JIT_IMPORT(polar_plus)
  JIT_IMPORT(polar_minus)
  JIT_IMPORT(polar_mul)
  JIT_IMPORT(polar_div)
  // nalmeing incons
  JIT_IMPORT(polar_plus_r)
  JIT_IMPORT(polar_minus_r)
  JIT_IMPORT(polar_mul_r)
  JIT_IMPORT(polar_div_r)
}
