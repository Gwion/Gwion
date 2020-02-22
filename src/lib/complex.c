#include <complex.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"

#include "gwi.h"
#include "gack.h"

#define describe(name, op) \
static INSTR(Complex##name) {\
  POP_REG(shred, SZ_COMPLEX); \
  *(m_complex*)REG(-SZ_COMPLEX) op##= *(m_complex*)REG(0); \
}
describe(Add,   +)
describe(Sub,  -)
describe(Mul,  *)
describe(Div, /)

static INSTR(ComplexRAssign) {
  POP_REG(shred, SZ_INT);
  **(m_complex**)REG(0) = *(m_complex*)REG(-SZ_COMPLEX);
}

#define describe_r(name, op) \
static INSTR(ComplexR##name) {\
  POP_REG(shred, SZ_INT); \
  *(m_complex*)REG(-SZ_COMPLEX) = (**(m_complex**)REG(0) op##= (*(m_complex*)REG(-SZ_COMPLEX))); \
}
describe_r(Add,  +)
describe_r(Sub, -)
describe_r(Mul, *)
describe_r(Div, /)

INSTR(ComplexReal) {
//  if(!instr->m_val) { // other case skipped in emit.c
    *(m_float*)REG(-SZ_INT) = **(m_float**)REG(-SZ_INT);
    PUSH_REG(shred, SZ_FLOAT - SZ_INT);
//  }
}

INSTR(ComplexImag) {
  if(instr->m_val) {
    const m_float* f = *(m_float**)REG(-SZ_INT);
    *(const m_float**)REG(-SZ_INT) = (f + 1);
  } else {
    const m_float* f = *(m_float**)REG(-SZ_INT);
    *(m_float*)REG(-SZ_INT) = *(f + 1);
    PUSH_REG(shred, SZ_FLOAT - SZ_INT);
  }
}

#if defined __clang__ && defined BUILD_ON_WINDOWS
#define POLAR(a, b, c) m_complex a = { b, c };
#else
#define POLAR(a, b, c) m_complex a = b + c *I;
#endif

#define polar_def1(name, op)                                               \
static INSTR(Polar##name) {                                                \
  POP_REG(shred, SZ_COMPLEX);                                              \
  const m_complex a = *(m_complex*)REG(-SZ_COMPLEX);                       \
  const m_complex b = *(m_complex*)REG(0);                                 \
  const m_float re = creal(a) * cos(cimag(a)) op creal(b) * cos(cimag(b)); \
  const m_float im = creal(a) * sin(cimag(a)) op creal(b) * sin(cimag(b)); \
  POLAR(c, hypot(re, im), atan2(im, re))                                   \
  *(m_complex*)REG(-SZ_COMPLEX) = c;                                       \
}

polar_def1(Add,  +)
polar_def1(Sub, -)

#define polar_def2(name, op1, op2)                   \
static INSTR(Polar##name) {\
  POP_REG(shred, SZ_COMPLEX);                        \
  const m_complex a = *(m_complex*)REG(-SZ_COMPLEX); \
  const m_complex b = *(m_complex*)REG(0);           \
  const m_float mag   = creal(a) op1 creal(b);       \
  const m_float phase = cimag(a) op2 cimag(b);       \
  POLAR(c, mag, phase);                              \
  *(m_complex*)REG(-SZ_COMPLEX) = c;                 \
}
polar_def2(Mul, *, +)
polar_def2(Div, /, -)

#define polar_def1_r(name, op)                                             \
static INSTR(PolarR##name) {\
  POP_REG(shred, SZ_INT);                                                  \
  const m_complex a = *(m_complex*)REG(-SZ_COMPLEX);                       \
  const m_complex b = **(m_complex**)REG(0);                               \
  const m_float re = creal(a) * cos(cimag(a)) op creal(b) * cos(cimag(b)); \
  const m_float im = creal(a) * sin(cimag(a)) op creal(b) * sin(cimag(b)); \
  POLAR(c, hypot(re, im), atan2(im, re))                                   \
  *(m_complex*)REG(-SZ_COMPLEX) = **(m_complex**)REG(0) = c;               \
}
polar_def1_r(Add, +)
polar_def1_r(Sub, -)

#define polar_def2_r(name, op1, op2)                         \
static INSTR(PolarR##name) {                                 \
  POP_REG(shred, SZ_INT);                                    \
  const m_complex a = *(m_complex*)REG(-SZ_COMPLEX);         \
  const m_complex b = **(m_complex**)REG(0);                 \
  const m_float mag   = creal(a) op1 creal(b);               \
  const m_float phase = cimag(a) op2 cimag(b);               \
  POLAR(c, mag, phase);                                      \
  *(m_complex*)REG(-SZ_COMPLEX) = **(m_complex**)REG(0) = c; \
}
polar_def2_r(Mul, *, +)
polar_def2_r(Div, /, -)

static GACK(gack_complex) {
  INTERP_PRINTF("#(%.4f, %.4f)", *(m_float*)VALUE, *(m_float*)(VALUE + SZ_FLOAT));
}

static GACK(gack_polar) {
  INTERP_PRINTF("%%(%4f, %.4f*pi)", *(m_float*)VALUE, *(m_float*)(VALUE + SZ_FLOAT) / M_PI);
}

EQUALITY_OPER(complex, SZ_COMPLEX)

GWION_IMPORT(complex) {
// should be special
  const Type t_complex = gwi_class_spe(gwi, "complex", SZ_COMPLEX);
  GWI_BB(gwi_gack(gwi, t_complex, gack_complex))
  gwi->gwion->type[et_complex] = t_complex; // use func
	gwi_item_ini(gwi, "float", "re");
  GWI_BB(gwi_item_end(gwi,   ae_flag_member, NULL))
	gwi_item_ini(gwi, "float", "im");
  GWI_BB(gwi_item_end(gwi,   ae_flag_member, NULL))
  GWI_BB(gwi_class_end(gwi))
// should be special
  const Type t_polar   = gwi_class_spe(gwi, "polar", SZ_COMPLEX);
  gwi->gwion->type[et_polar] = t_polar;
  GWI_BB(gwi_gack(gwi, t_polar, gack_polar))
  GWI_BB(gwi_item_ini(gwi, "float", "mod"))
  GWI_BB(gwi_item_end(gwi,   ae_flag_member, NULL))
  GWI_BB(gwi_item_ini(gwi, "float", "phase"))
  GWI_BB(gwi_item_end(gwi,   ae_flag_member, NULL))
  GWI_BB(gwi_class_end(gwi))
  GWI_BB(gwi_oper_ini(gwi, "complex", "complex", "bool"))
  GWI_BB(gwi_oper_end(gwi, "==",          complex_eq))
  GWI_BB(gwi_oper_end(gwi, "!=",          complex_ne))
  GWI_BB(gwi_oper_ini(gwi, "complex", "complex", "complex"))
  GWI_BB(gwi_oper_end(gwi, "+",          ComplexAdd))
  GWI_BB(gwi_oper_end(gwi, "-",         ComplexSub))
  GWI_BB(gwi_oper_end(gwi, "*",         ComplexMul))
  GWI_BB(gwi_oper_end(gwi, "/",        ComplexDiv))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "=>",         ComplexRAssign))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "+=>",    ComplexRAdd))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "-=>",   ComplexRSub))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "*=>",   ComplexRMul))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "/=>",  ComplexRDiv))
  GWI_BB(gwi_oper_ini(gwi, "polar", "polar", "bool"))
  GWI_BB(gwi_oper_end(gwi, "==",          complex_eq))
  GWI_BB(gwi_oper_end(gwi, "!=",          complex_ne))
  GWI_BB(gwi_oper_ini(gwi, "polar", "polar", "polar"))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "=>",         ComplexRAssign))
  GWI_BB(gwi_oper_end(gwi, "+",          PolarAdd))
  GWI_BB(gwi_oper_end(gwi, "-",         PolarSub))
  GWI_BB(gwi_oper_end(gwi, "*",         PolarMul))
  GWI_BB(gwi_oper_end(gwi, "/",        PolarDiv))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "+=>",    PolarRAdd))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "-=>",   PolarRSub))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "*=>",   PolarRMul))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "/=>",  PolarRDiv))
  return GW_OK;
}
