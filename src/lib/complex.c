#include <math.h>
#include <complex.h>
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "import.h"

struct Type_ t_complex = { "complex", SZ_COMPLEX };
struct Type_ t_polar   = { "polar",   SZ_COMPLEX };

m_int o_complex_real, o_complex_imag;
m_int o_polar_mod,    o_polar_phase;

static INSTR(complex_assign) { GWDEBUG_INSTR
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  *(m_complex*)REG(0) = (**(m_complex**)REG(0) = *(m_complex*)REG(SZ_INT));
  PUSH_REG(shred, SZ_COMPLEX);
}


#define describe(name, op) \
static INSTR(complex_##name) { GWDEBUG_INSTR \
  POP_REG(shred, SZ_COMPLEX); \
  *(m_complex*)REG(-SZ_COMPLEX) op##= *(m_complex*)REG(0); \
}
describe(plus,   +)
describe(minus,  -)
describe(times,  *)
describe(divide, /)

static INSTR(complex_r_assign) { GWDEBUG_INSTR
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  **(m_complex**)REG(SZ_COMPLEX) = *(m_complex*)REG(0);
  PUSH_REG(shred, SZ_COMPLEX);
}

#define describe_r(name, op) \
static INSTR(complex_r_##name) { GWDEBUG_INSTR \
  POP_REG(shred, SZ_COMPLEX + SZ_INT); \
  *(m_complex*)REG(0) = (**(m_complex**)REG(SZ_COMPLEX) op##= (*(m_complex*)REG(0))); \
  PUSH_REG(shred, SZ_COMPLEX); \
}
describe_r(plus,  +)
describe_r(minus, -)
describe_r(times, *)
describe_r(divide, /)

INSTR(complex_real) { GWDEBUG_INSTR
  POP_REG(shred, SZ_INT);
  if(instr->m_val)
    PUSH_REG(shred, SZ_INT)
  else {
    *(m_float*)REG(0) = **(m_float**)REG(0);  // coverity
    PUSH_REG(shred, SZ_FLOAT);
  }
}

INSTR(complex_imag) { GWDEBUG_INSTR
  POP_REG(shred, SZ_INT);
  if(instr->m_val) {
    m_float* f = &**(m_float**)REG(0);
    *(m_float**)REG(0) = &*(f + 1);
    PUSH_REG(shred, SZ_INT);
  } else {
    m_float* f = &**(m_float**)REG(0);
    *(m_float*)REG(0) = *(f + 1);
    PUSH_REG(shred, SZ_FLOAT);
  }
}

static INSTR(polar_plus) { GWDEBUG_INSTR
  POP_REG(shred, SZ_COMPLEX * 2);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = *(m_complex*)REG(SZ_COMPLEX);
  m_float re = creal(a) * cos(cimag(a)) + creal(b) * cos(cimag(b));
  m_float im = creal(a) * sin(cimag(a)) + creal(b) * sin(cimag(b));
  *(m_complex*)REG(0) = hypot(re, im) + atan2(im, re) * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_minus) { GWDEBUG_INSTR
  POP_REG(shred, SZ_COMPLEX * 2);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = *(m_complex*)REG(SZ_COMPLEX);
  m_float re = creal(a) * cos(cimag(a)) - creal(b) * cos(cimag(b));
  m_float im = creal(a) * sin(cimag(a)) - creal(b) * sin(cimag(b));
  *(m_complex*)REG(0) = hypot(re, im) + atan2(im, re) * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_times) { GWDEBUG_INSTR
  POP_REG(shred, SZ_COMPLEX * 2);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = *(m_complex*)REG(SZ_COMPLEX);
  m_float mag   = creal(a) * creal(b);
  m_float phase = cimag(a) + cimag(b);
  *(m_complex*)REG(0) = mag  + phase * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_divide) { GWDEBUG_INSTR
  POP_REG(shred, SZ_COMPLEX * 2);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = *(m_complex*)REG(SZ_COMPLEX);
  m_float mag   = creal(a) / creal(b);
  m_float phase = cimag(a) - cimag(b);
  *(m_complex*)REG(0) = mag  + phase * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_plus_r) { GWDEBUG_INSTR
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = **(m_complex**)REG(SZ_COMPLEX);
  m_float re = creal(a) * cos(cimag(a)) + creal(b) * cos(cimag(b));
  m_float im = creal(a) * sin(cimag(a)) + sin(cimag(b));
  **(m_complex**)REG(SZ_COMPLEX) = hypot(re, im) + atan2(im, re) * I;
  *(m_complex*)REG(0) = hypot(re, im) + atan2(im, re) * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_minus_r) { GWDEBUG_INSTR
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = **(m_complex**)REG(SZ_COMPLEX);
  m_float re, im;
  re = creal(a) * cos(cimag(a)) - creal(b) * cos(cimag(b));
  im = creal(a) * sin(cimag(a)) - sin(cimag(b));
  **(m_complex**)REG(SZ_COMPLEX) = hypot(re, im) + atan2(im, re) * I;
  *(m_complex*)REG(0) = hypot(re, im) + atan2(im, re) * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_times_r) { GWDEBUG_INSTR
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = **(m_complex**)REG(SZ_COMPLEX);
  m_float mag   = creal(a) * creal(b);
  m_float phase = cimag(a) + cimag(b);
  **(m_complex**)REG(SZ_COMPLEX) = mag  + phase * I;
  *(m_complex*)REG(0) = mag  + phase * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_divide_r) { GWDEBUG_INSTR
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = **(m_complex**)REG(SZ_COMPLEX);
  m_float mag   = creal(a) / creal(b);
  m_float phase = cimag(a) - cimag(b);
  **(m_complex**)REG(SZ_COMPLEX) = mag  + phase * I;
  *(m_complex*)REG(0) = mag  + phase * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

m_bool import_complex(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  &t_complex, NULL, NULL))
	gwi_item_ini(gwi, "float", "re");
  o_complex_real = gwi_item_end(gwi,   ae_flag_member, NULL);
  CHECK_BB(o_complex_real)
	gwi_item_ini(gwi, "float", "im");
  o_complex_imag = gwi_item_end(gwi,   ae_flag_member, NULL);
  CHECK_BB(o_complex_imag)
  CHECK_BB(gwi_class_end(gwi))

  CHECK_BB(gwi_class_ini(gwi,  &t_polar, NULL, NULL))
  CHECK_BB(gwi_item_ini(gwi, "float", "mod"))
  CHECK_BB((o_polar_mod = gwi_item_end(gwi,   ae_flag_member, NULL)))
  CHECK_BB(gwi_item_ini(gwi, "float", "phase"))
  CHECK_BB((o_polar_phase = gwi_item_end(gwi,   ae_flag_member, NULL)))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, "complex", "complex", "complex"))
  CHECK_BB(gwi_oper_add(gwi, opck_assign))
  CHECK_BB(gwi_oper_end(gwi, op_assign,        complex_assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,          complex_plus))
  CHECK_BB(gwi_oper_end(gwi, op_minus,         complex_minus))
  CHECK_BB(gwi_oper_end(gwi, op_times,         complex_times))
  CHECK_BB(gwi_oper_end(gwi, op_divide,        complex_divide))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,         complex_r_assign))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck,    complex_r_plus))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_minus_chuck,   complex_r_minus))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_times_chuck,   complex_r_times))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_divide_chuck,  complex_r_divide))
  CHECK_BB(gwi_oper_ini(gwi, "polar", "polar", "polar"))
  CHECK_BB(gwi_oper_add(gwi, opck_assign))
  CHECK_BB(gwi_oper_end(gwi, op_assign,        complex_assign))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,         complex_r_assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,          polar_plus))
  CHECK_BB(gwi_oper_end(gwi, op_minus,         polar_minus))
  CHECK_BB(gwi_oper_end(gwi, op_times,         polar_times))
  CHECK_BB(gwi_oper_end(gwi, op_divide,        polar_divide))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck,    polar_plus_r))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_minus_chuck,   polar_minus_r))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_times_chuck,   polar_times_r))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_divide_chuck,  polar_divide_r))
  return 1;
}
