#include <math.h>
#include "type.h"
#include "instr.h"
#include "import.h"

struct Type_ t_complex = { "complex", SZ_COMPLEX, NULL, te_complex};
struct Type_ t_polar   = { "polar",   SZ_COMPLEX, NULL, te_polar};

m_int o_complex_real, o_complex_imag;
m_int o_polar_mod,    o_polar_phase;

static INSTR(complex_assign) {
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  *(m_complex*)REG(0) = (**(m_complex**)REG(0) = *(m_complex*)REG(SZ_INT));
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_plus) {
  POP_REG(shred, SZ_COMPLEX * 2);
  *(m_complex*)REG(0) += *(m_complex*)REG(SZ_COMPLEX);
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_minus) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  *(m_complex*)REG(0) -= *(m_complex*)REG(SZ_INT);
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_times) {
  POP_REG(shred, SZ_COMPLEX * 2);
  *(m_complex*)REG(0) *= *(m_complex*)REG(SZ_INT);
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_divide) {
  POP_REG(shred, SZ_COMPLEX * 2);
  *(m_complex*)REG(0) /= *(m_complex*)REG(SZ_INT);
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_r_assign) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  **(m_complex**)REG(SZ_COMPLEX) = *(m_complex*)REG(0);
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_r_plus) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  *(m_complex*)REG(0) = (**(m_complex**)REG(SZ_COMPLEX) += (*(m_complex*)REG(0)));
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_r_minus) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  *(m_complex*)REG(0) = (**(m_complex**)REG(SZ_COMPLEX) -= (*(m_complex*)REG(0)));
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_r_times) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  *(m_complex*)REG(0) = (**(m_complex**)REG(SZ_COMPLEX) *= (*(m_complex*)REG(0)));
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(complex_r_divide) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  *(m_complex*)REG(0) = (**(m_complex**)REG(SZ_COMPLEX) /= (*(m_complex*)REG(0)));
  PUSH_REG(shred, SZ_COMPLEX);
}

INSTR(complex_real) {
  POP_REG(shred, SZ_INT);
  if(instr->m_val)
    PUSH_REG(shred, SZ_INT)
  else {
    *(m_float*)REG(0) = **(m_float**)REG(0);  // coverity
    PUSH_REG(shred, SZ_FLOAT);
  }
}

INSTR(complex_imag) {
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

static INSTR(polar_plus) {
  POP_REG(shred, SZ_COMPLEX * 2);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = *(m_complex*)REG(SZ_COMPLEX);
  m_float re = creal(a) * cos(cimag(a)) + creal(b) * cos(cimag(b));
  m_float im = creal(a) * sin(cimag(a)) + creal(b) * sin(cimag(b));
  *(m_complex*)REG(0) = hypot(re, im) + atan2(im, re) * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_minus) {
  POP_REG(shred, SZ_COMPLEX * 2);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = *(m_complex*)REG(SZ_COMPLEX);
  m_float re = creal(a) * cos(cimag(a)) - creal(b) * cos(cimag(b));
  m_float im = creal(a) * sin(cimag(a)) - creal(b) * sin(cimag(b));
  *(m_complex*)REG(0) = hypot(re, im) + atan2(im, re) * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_times) {
  POP_REG(shred, SZ_COMPLEX * 2);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = *(m_complex*)REG(SZ_COMPLEX);
  m_float mag   = creal(a) * creal(b);
  m_float phase = cimag(a) + cimag(b);
  *(m_complex*)REG(0) = mag  + phase * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_divide) {
  POP_REG(shred, SZ_COMPLEX * 2);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = *(m_complex*)REG(SZ_COMPLEX);
  m_float mag   = creal(a) / creal(b);
  m_float phase = cimag(a) - cimag(b);
  *(m_complex*)REG(0) = mag  + phase * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_plus_r) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = **(m_complex**)REG(SZ_COMPLEX);
  m_float re = creal(a) * cos(cimag(a)) + creal(b) * cos(cimag(b));
  m_float im = creal(a) * sin(cimag(a)) + sin(cimag(b));
  **(m_complex**)REG(SZ_COMPLEX) = hypot(re, im) + atan2(im, re) * I;
  *(m_complex*)REG(0) = hypot(re, im) + atan2(im, re) * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_minus_r) {
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

static INSTR(polar_times_r) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = **(m_complex**)REG(SZ_COMPLEX);
  m_float mag   = creal(a) * creal(b);
  m_float phase = cimag(a) + cimag(b);
  **(m_complex**)REG(SZ_COMPLEX) = mag  + phase * I;
  *(m_complex*)REG(0) = mag  + phase * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

static INSTR(polar_divide_r) {
  POP_REG(shred, SZ_COMPLEX + SZ_INT);
  m_complex a = *(m_complex*)REG(0);
  m_complex b = **(m_complex**)REG(SZ_COMPLEX);
  m_float mag   = creal(a) / creal(b);
  m_float phase = cimag(a) - cimag(b);
  **(m_complex**)REG(SZ_COMPLEX) = mag  + phase * I;
  *(m_complex*)REG(0) = mag  + phase * I;
  PUSH_REG(shred, SZ_COMPLEX);
}

m_bool import_complex(Importer importer) {
  CHECK_BB(importer_class_ini(importer,  &t_complex, NULL, NULL))
	importer_item_ini(importer, "float", "re");
  o_complex_real = importer_item_end(importer,   ae_flag_member, NULL);
  CHECK_BB(o_complex_real)
	importer_item_ini(importer, "float", "im");
  o_complex_imag = importer_item_end(importer,   ae_flag_member, NULL);
  CHECK_BB(o_complex_imag)
  CHECK_BB(importer_class_end(importer))

  CHECK_BB(importer_class_ini(importer,  &t_polar, NULL, NULL))
	importer_item_ini(importer, "float", "mod");
  o_polar_mod = importer_item_end(importer,   ae_flag_member, NULL);
  CHECK_BB(o_polar_mod)
	importer_item_ini(importer, "float", "phase");
  o_polar_phase = importer_item_end(importer,   ae_flag_member, NULL);
  CHECK_BB(o_polar_phase)
  CHECK_BB(importer_class_end(importer))
  CHECK_BB(importer_oper_ini(importer, "complex", "complex", "complex"))
  CHECK_BB(importer_oper_add(importer, check_assign))
  CHECK_BB(importer_oper_end(importer, op_assign,        complex_assign,     0))
  CHECK_BB(importer_oper_end(importer, op_plus,          complex_plus,       0))
  CHECK_BB(importer_oper_end(importer, op_minus,         complex_minus,      0))
  CHECK_BB(importer_oper_end(importer, op_times,         complex_times,      0))
  CHECK_BB(importer_oper_end(importer, op_divide,        complex_divide,     0))
  CHECK_BB(importer_oper_add(importer, check_rassign))
  CHECK_BB(importer_oper_end(importer, op_chuck,         complex_r_assign,   0))
  CHECK_BB(importer_oper_add(importer, check_rassign))
  CHECK_BB(importer_oper_end(importer, op_plus_chuck,    complex_r_plus,     0))
  CHECK_BB(importer_oper_add(importer, check_rassign))
  CHECK_BB(importer_oper_end(importer, op_minus_chuck,   complex_r_minus,    0))
  CHECK_BB(importer_oper_add(importer, check_rassign))
  CHECK_BB(importer_oper_end(importer, op_times_chuck,   complex_r_times,    0))
  CHECK_BB(importer_oper_add(importer, check_rassign))
  CHECK_BB(importer_oper_end(importer, op_divide_chuck,  complex_r_divide,   0))
  CHECK_BB(importer_oper_ini(importer, "polar", "polar", "polar"))
  CHECK_BB(importer_oper_add(importer, check_assign))
  CHECK_BB(importer_oper_end(importer, op_assign,        complex_assign,           0))
  CHECK_BB(importer_oper_add(importer, check_rassign))
  CHECK_BB(importer_oper_end(importer, op_chuck,         complex_r_assign,         0))
  CHECK_BB(importer_oper_end(importer, op_plus,          polar_plus,       0))
  CHECK_BB(importer_oper_end(importer, op_minus,         polar_minus,      0))
  CHECK_BB(importer_oper_end(importer, op_times,         polar_times,      0))
  CHECK_BB(importer_oper_end(importer, op_divide,        polar_divide,     0))
  CHECK_BB(importer_oper_add(importer, check_rassign))
  CHECK_BB(importer_oper_end(importer, op_plus_chuck,    polar_plus_r,     0))
  CHECK_BB(importer_oper_add(importer, check_rassign))
  CHECK_BB(importer_oper_end(importer, op_minus_chuck,   polar_minus_r,    0))
  CHECK_BB(importer_oper_add(importer, check_rassign))
  CHECK_BB(importer_oper_end(importer, op_times_chuck,   polar_times_r,    0))
  CHECK_BB(importer_oper_add(importer, check_rassign))
  CHECK_BB(importer_oper_end(importer, op_divide_chuck,  polar_divide_r,   0))
  return 1;
}
