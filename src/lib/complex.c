#include <complex.h>
#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "object.h"
#include "import.h"

static INSTR(ComplexAssign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  *(m_complex*)REG(-SZ_COMPLEX) = (**(m_complex**)REG(-SZ_COMPLEX) =
     *(m_complex*)REG(SZ_INT-SZ_COMPLEX));
}

#define describe(name, op) \
static INSTR(Complex##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_COMPLEX); \
  *(m_complex*)REG(-SZ_COMPLEX) op##= *(m_complex*)REG(0); \
}
describe(Add,   +)
describe(Sub,  -)
describe(Mul,  *)
describe(Div, /)

static INSTR(ComplexRAssign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  **(m_complex**)REG(0) = *(m_complex*)REG(-SZ_COMPLEX);
}

#define describe_r(name, op) \
static INSTR(ComplexR##name) { GWDEBUG_EXE \
  POP_REG(shred, SZ_INT); \
  *(m_complex*)REG(-SZ_COMPLEX) = (**(m_complex**)REG(0) op##= (*(m_complex*)REG(-SZ_COMPLEX))); \
}
describe_r(Add,  +)
describe_r(Sub, -)
describe_r(Mul, *)
describe_r(Div, /)

INSTR(ComplexReal) { GWDEBUG_EXE
//  if(!instr->m_val) { // other case skipped in emit.c
    *(m_float*)REG(-SZ_INT) = **(m_float**)REG(-SZ_INT);
    PUSH_REG(shred, SZ_FLOAT - SZ_INT);
//  }
}

INSTR(ComplexImag) { GWDEBUG_EXE
  if(instr->m_val) {
    const m_float* f = *(m_float**)REG(-SZ_INT);
    *(const m_float**)REG(-SZ_INT) = (f + 1);
  } else {
    const m_float* f = *(m_float**)REG(-SZ_INT);
    *(m_float*)REG(-SZ_INT) = *(f + 1);
    PUSH_REG(shred, SZ_FLOAT - SZ_INT);
  }
}

#define polar_def1(name, op)                                               \
static INSTR(Polar##name) { GWDEBUG_EXE                                   \
  POP_REG(shred, SZ_COMPLEX);                                              \
  const m_complex a = *(m_complex*)REG(-SZ_COMPLEX);                       \
  const m_complex b = *(m_complex*)REG(0);                                 \
  const m_float re = creal(a) * cos(cimag(a)) op creal(b) * cos(cimag(b)); \
  const m_float im = creal(a) * sin(cimag(a)) op creal(b) * sin(cimag(b)); \
  *(m_complex*)REG(-SZ_COMPLEX) = hypot(re, im) + atan2(im, re) * I;       \
}

polar_def1(Add,  +)
polar_def1(Sub, -)

#define polar_def2(name, op1, op2)                   \
static INSTR(Polar##name) { GWDEBUG_EXE             \
  POP_REG(shred, SZ_COMPLEX);                        \
  const m_complex a = *(m_complex*)REG(-SZ_COMPLEX); \
  const m_complex b = *(m_complex*)REG(0);           \
  const m_float mag   = creal(a) op1 creal(b);       \
  const m_float phase = cimag(a) op2 cimag(b);       \
  *(m_complex*)REG(-SZ_COMPLEX) = mag  + phase * I;  \
}
polar_def2(Mul, *, +)
polar_def2(Div, /, -)

#define polar_def1_r(name, op)                                             \
static INSTR(PolarR##name) { GWDEBUG_EXE                               \
  POP_REG(shred, SZ_INT);                                                  \
  const m_complex a = *(m_complex*)REG(-SZ_COMPLEX);                       \
  const m_complex b = **(m_complex**)REG(0);                               \
  const m_float re = creal(a) * cos(cimag(a)) op creal(b) * cos(cimag(b)); \
  const m_float im = creal(a) * sin(cimag(a)) op creal(b) * sin(cimag(b)); \
  *(m_complex*)REG(-SZ_COMPLEX) = **(m_complex**)REG(0) =                  \
    hypot(re, im) + atan2(im, re) * I;                                     \
}
polar_def1_r(Add, +)
polar_def1_r(Sub, -)

#define polar_def2_r(name, op1, op2)                      \
static INSTR(PolarR##name) { GWDEBUG_EXE              \
  POP_REG(shred, SZ_INT);                                 \
  const m_complex a = *(m_complex*)REG(-SZ_COMPLEX);      \
  const m_complex b = **(m_complex**)REG(0);              \
  const m_float mag   = creal(a) op1 creal(b);            \
  const m_float phase = cimag(a) op2 cimag(b);            \
  *(m_complex*)REG(-SZ_COMPLEX) = **(m_complex**)REG(0) = \
    mag + phase * I;                                      \
}
polar_def2_r(Mul, *, +)
polar_def2_r(Div, /, -)

GWION_IMPORT(complex) {
  CHECK_BB(gwi_class_ini(gwi,  t_complex, NULL, NULL))
	gwi_item_ini(gwi, "float", "re");
  CHECK_BB(gwi_item_end(gwi,   ae_flag_member, NULL))
	gwi_item_ini(gwi, "float", "im");
  CHECK_BB(gwi_item_end(gwi,   ae_flag_member, NULL))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_class_ini(gwi,  t_polar, NULL, NULL))
  CHECK_BB(gwi_item_ini(gwi, "float", "mod"))
  CHECK_BB(gwi_item_end(gwi,   ae_flag_member, NULL))
  CHECK_BB(gwi_item_ini(gwi, "float", "phase"))
  CHECK_BB(gwi_item_end(gwi,   ae_flag_member, NULL))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, "complex", "complex", "complex"))
  CHECK_BB(gwi_oper_add(gwi, opck_assign))
  CHECK_BB(gwi_oper_end(gwi, op_assign,        ComplexAssign))
  CHECK_BB(gwi_oper_end(gwi, op_add,          ComplexAdd))
  CHECK_BB(gwi_oper_end(gwi, op_sub,         ComplexSub))
  CHECK_BB(gwi_oper_end(gwi, op_mul,         ComplexMul))
  CHECK_BB(gwi_oper_end(gwi, op_div,        ComplexDiv))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,         ComplexRAssign))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_radd,    ComplexRAdd))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_rsub,   ComplexRSub))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_rmul,   ComplexRMul))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_rdiv,  ComplexRDiv))
  CHECK_BB(gwi_oper_ini(gwi, "polar", "polar", "polar"))
  CHECK_BB(gwi_oper_add(gwi, opck_assign))
  CHECK_BB(gwi_oper_end(gwi, op_assign,        ComplexAssign))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,         ComplexRAssign))
  CHECK_BB(gwi_oper_end(gwi, op_add,          PolarAdd))
  CHECK_BB(gwi_oper_end(gwi, op_sub,         PolarSub))
  CHECK_BB(gwi_oper_end(gwi, op_mul,         PolarMul))
  CHECK_BB(gwi_oper_end(gwi, op_div,        PolarDiv))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_radd,    PolarRAdd))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_rsub,   PolarRSub))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_rmul,   PolarRMul))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_rdiv,  PolarRDiv))
  return 1;
}
