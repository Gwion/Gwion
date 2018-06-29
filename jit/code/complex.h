#include <string.h>
#include "jit/jit.h"
#include "gwjit_common.h"
#include "cc.h"
#include "q.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"
/*
static jit_type_t jit_type_complex;
__attribute__((constructor(200)))
void ini() {
  jit_type_t fields[] = { jit_type_float, jit_type_float };
  jit_type_complex = jit_type_create_struct(fields, 2, 0);
}
__attribute__((destructor(200)))
void end() { jit_type_free(jit_type_complex); }

JIT_CODE(complex_assign) {
  CJval reg = push_reg(j, -SZ_INT);
  CJval src = JLOADR(reg, SZ_INT - SZ_COMPLEX , complex);
  CJval ptr = JLOADR(reg, -SZ_COMPLEX, void_ptr);
  JSTORER(ptr, 0, src);
  JSTORER(reg, -SZ_COMPLEX, src);
}

#define jit_describe1(name, op)                                \
JIT_CODE(complex_##name) {                                     \
  CJval reg = push_reg(j, -SZ_COMPLEX);                        \
  CJval lhs = JADDR(reg, -SZ_COMPLEX);                         \
  CJval lreal = JLOADR(lhs, 0, float);                         \
  CJval limag = JLOADR(lhs, SZ_FLOAT, float);                  \
  CJval rhs   = JADDR(reg, 0);                                 \
  CJval rreal = JLOADR(rhs, 0, float);                         \
  CJval rimag = JLOADR(rhs, SZ_FLOAT, float);                  \
  CJval real = JINSN(op, lreal, rreal);                       \
  CJval imag = JINSN(op, limag, rimag);                       \
  JSTORER(reg, -SZ_COMPLEX, real);                             \
  JSTORER(reg, -SZ_FLOAT, imag);                               \
}
jit_describe1(plus,   add)
jit_describe1(minus,  sub)

JIT_CODE(complex_times) {
  CJval reg = push_reg(j, -SZ_COMPLEX);
  CJval lhs   = JADDR(reg, -SZ_COMPLEX);
  CJval lreal = JLOADR(lhs, 0, float);
  CJval limag = JLOADR(lhs, SZ_FLOAT, float);
  CJval rhs   = JADDR(reg, 0);
  CJval rreal = JLOADR(rhs, 0, float);
  CJval rimag = JLOADR(rhs, SZ_FLOAT, float);
  CJval real1 = JINSN(mul, lreal, rreal);
  CJval real2 = JINSN(mul, limag, rimag);
  CJval real  = JINSN(sub, real1, real2);
  CJval imag1 = JINSN(mul, lreal, rimag);
  CJval imag2 = JINSN(mul, limag, rreal);
  CJval imag  = JINSN(add, imag1, imag2);
  JSTORER(reg, -SZ_COMPLEX, real);
  JSTORER(reg, -SZ_FLOAT, imag);
}

JIT_CODE(complex_divide) {
  CJval reg = push_reg(j, -SZ_COMPLEX);
  CJval lhs   = JADDR(reg, -SZ_COMPLEX);
  CJval lreal = JLOADR(lhs, 0, float);
  CJval limag = JLOADR(lhs, SZ_FLOAT, float);
  CJval rhs   = JADDR(reg, 0);
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
  JSTORER(reg, -SZ_COMPLEX, real);
  JSTORER(reg, -SZ_FLOAT, imag);
}

JIT_CODE(complex_r_assign) {
  CJval reg = push_reg(j, -SZ_INT);
  CJval src = JLOADR(reg, -SZ_COMPLEX, complex);
  CJval ptr = JLOADR(reg, 0, void_ptr);
  JSTORER(ptr, 0, src);
  JSTORER(reg, -SZ_COMPLEX, src);
}

#define jit_describe_r1(name, op)                              \
JIT_CODE(complex_r_##name) {                                   \
  CJval reg = push_reg(j, -SZ_COMPLEX);                        \
  CJval lhs   = JADDR(reg, -SZ_COMPLEX);                       \
  CJval lreal = JLOADR(lhs, 0, float);                         \
  CJval limag = JLOADR(lhs, SZ_FLOAT, float);                  \
  CJval rhs   = JLOADR(reg, 0, void_ptr);                      \
  CJval rreal = JLOADR(rhs, 0, float);                         \
  CJval rimag = JLOADR(rhs, SZ_FLOAT, float);                  \
  CJval real = JINSN(op, rreal, lreal);                       \
  CJval imag = JINSN(op, rimag, limag);                       \
  CJval ptr = JLOADR(reg, 0, void_ptr);                        \
  JSTORER(ptr, 0, real);                                       \
  JSTORER(ptr, SZ_FLOAT, imag);                                \
  JSTORER(reg, -SZ_COMPLEX, real);                             \
  JSTORER(reg, -SZ_FLOAT, imag);                               \
}
jit_describe_r1(plus,   add)
jit_describe_r1(minus,  sub)

JIT_CODE(complex_r_times) {
  CJval reg = push_reg(j, -SZ_COMPLEX);
  CJval lhs   = JADDR(reg, -SZ_COMPLEX);
  CJval lreal = JLOADR(lhs, 0, float);
  CJval limag = JLOADR(lhs, SZ_FLOAT, float);
  CJval rhs   = JLOADR(reg, 0, void_ptr);
  CJval rreal = JLOADR(rhs, 0, float);
  CJval rimag = JLOADR(rhs, SZ_FLOAT, float);
  CJval real1 = JINSN(mul, rreal, lreal);
  CJval real2 = JINSN(mul, rimag, limag);
  CJval real  = JINSN(sub, real1, real2);
  CJval imag1 = JINSN(mul, rreal, limag);
  CJval imag2 = JINSN(mul, rimag, lreal);
  CJval imag  = JINSN(add, imag1, imag2);
  CJval ptr = JLOADR(reg, 0, void_ptr);
  JSTORER(ptr, 0, real);
  JSTORER(ptr, SZ_FLOAT, imag);
  JSTORER(reg, -SZ_COMPLEX, real);
  JSTORER(reg, -SZ_FLOAT, imag);
}

JIT_CODE(complex_r_divide) {
  CJval reg = push_reg(j, -SZ_COMPLEX);
  CJval lhs   = JADDR(reg, -SZ_COMPLEX);
  CJval lreal = JLOADR(lhs, 0, float);
  CJval limag = JLOADR(lhs, SZ_FLOAT, float);
  CJval rhs   = JLOADR(reg, 0, void_ptr);
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
  CJval ptr = JLOADR(reg, 0, void_ptr);
  JSTORER(ptr, 0, real);
  JSTORER(ptr, SZ_FLOAT, imag);
  JSTORER(reg, -SZ_COMPLEX, real);
  JSTORER(reg, -SZ_FLOAT, imag);
}

JIT_CODE(complex_real) {
  if(!instr->m_val) {
    CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
    CJval rhs   = JLOADR(reg, -SZ_INT, void_ptr);
    CJval real  = JLOADR(rhs, 0, float);
    JSTORER(reg, -SZ_INT, real);
    CJval new = JADDR(reg, SZ_FLOAT - SZ_INT);
    JSTORER(j->shred, JOFF(VM_Shred, reg), new);
  }
}

JIT_CODE(complex_imag) {
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval ptr = JADDR(reg, -SZ_INT);
  if(instr->m_val) {
    CJval rhs   = JLOADR(reg, -SZ_INT, void_ptr);
    CJval imag  = JLOADR(rhs, SZ_FLOAT, float);
    CJval val = JINSN(address_of, imag);
    JSTORER(reg, -SZ_INT, val);
  } else {
    CJval rhs   = JLOADR(ptr, 0, void_ptr);
    CJval imag  = JLOADR(rhs, SZ_FLOAT, float);
    JSTORER(reg, -SZ_INT, imag);
    CJval new = JADDR(reg, SZ_FLOAT - SZ_INT);
    JSTORER(j->shred, JOFF(VM_Shred, reg), new);
  }
}
#ifdef USE_DOUBLE
#define hypot hypot
#else
#define hypot hypotf
#endif

#define jit_polar_def1(name, op)\
JIT_CODE(polar_##name) {\
  CJval reg = push_reg(j, -SZ_COMPLEX);\
  CJval are = JLOADR(reg, -SZ_COMPLEX, float);\
  CJval aim = JLOADR(reg, -SZ_FLOAT,   float);\
  CJval bre = JLOADR(reg, 0,           float);\
  CJval bim = JLOADR(reg, SZ_FLOAT,    float);\
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
  CJval re     = CALL_NATIVE(hypot, "fff", arg);\
  CJval im     = JINSN(atan2, im0, re0);\
  JSTORER(reg, -SZ_COMPLEX, re);\
  JSTORER(reg, -SZ_FLOAT, im);\
}
jit_polar_def1(plus,  add)
jit_polar_def1(minus, sub)

#define jit_polar_def2(name, op1, op2)\
JIT_CODE(polar_##name) {\
  CJval reg = push_reg(j, -SZ_COMPLEX);\
  CJval are = JLOADR(reg, -SZ_COMPLEX, float);\
  CJval aim = JLOADR(reg, -SZ_FLOAT,   float);\
  CJval bre = JLOADR(reg, 0,           float);\
  CJval bim = JLOADR(reg, SZ_FLOAT,    float);\
  CJval mag = JINSN(op1, are, bre);          \
  CJval pha = JINSN(op2, aim, bim);          \
  JSTORER(reg, -SZ_COMPLEX, mag);\
  JSTORER(reg, -SZ_FLOAT, pha);\
}
jit_polar_def2(times,  mul, add)
jit_polar_def2(divide, div, sub)

#define jit_polar_def1_r(name, op)\
JIT_CODE(polar_##name##_r) {\
  CJval reg = push_reg(j, -SZ_INT);\
  CJval are = JLOADR(reg, -SZ_COMPLEX, float);\
  CJval aim = JLOADR(reg, -SZ_FLOAT,   float);\
  CJval ptr = JLOADR(reg, 0,           void_ptr);\
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
  CJval re     = CALL_NATIVE(hypot, "fff", arg);\
  CJval im     = JINSN(atan2, im0, re0);\
  JSTORER(ptr, 0, re);\
  JSTORER(ptr, SZ_FLOAT, im);\
  JSTORER(reg, -SZ_COMPLEX, re);\
  JSTORER(reg, -SZ_FLOAT, im);\
}
jit_polar_def1_r(plus,  add)
jit_polar_def1_r(minus, sub)

#define jit_polar_def2_r(name, op1, op2)\
JIT_CODE(polar_##name##_r) {\
  CJval reg = push_reg(j, -SZ_COMPLEX);\
  CJval are = JLOADR(reg, -SZ_COMPLEX, float);\
  CJval aim = JLOADR(reg, -SZ_FLOAT,   float);\
  CJval ptr = JLOADR(reg, 0,           void_ptr);\
  CJval bre = JLOADR(ptr, 0,           float);\
  CJval bim = JLOADR(ptr, SZ_FLOAT,    float);\
  CJval mag = JINSN(op1, are, bre);          \
  CJval pha = JINSN(op2, aim, bim);          \
  JSTORER(ptr, 0, mag);\
  JSTORER(ptr, SZ_FLOAT, pha);\
  JSTORER(reg, -SZ_COMPLEX, mag);\
  JSTORER(reg, -SZ_FLOAT, pha);\
}
jit_polar_def2_r(times,  mul, add)
jit_polar_def2_r(divide, div, sub)
*/
