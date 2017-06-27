#include <math.h>
#include "defs.h"
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
  if(instr->m_val) {
//    *(m_float**)REG(0) = &**(m_float**)REG(0); // coverity
    PUSH_REG(shred, SZ_INT);
  } else {
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

m_bool import_complex(Env env) {
  CHECK_BB(import_class_begin(env, &t_complex, env->global_nspc, NULL, NULL))
  o_complex_real = import_var(env, "float", "re",   ae_flag_member, NULL);
  CHECK_BB(o_complex_real)
  o_complex_imag = import_var(env, "float", "im",   ae_flag_member, NULL);
  CHECK_BB(o_complex_imag)
  CHECK_BB(import_class_end(env))

  CHECK_BB(import_class_begin(env, &t_polar, env->global_nspc, NULL, NULL))
  o_polar_mod = import_var(env, "float", "mod",   ae_flag_member, NULL);
  CHECK_BB(o_polar_mod)
  o_polar_phase = import_var(env, "float", "phase",   ae_flag_member, NULL);
  CHECK_BB(o_polar_phase)
  CHECK_BB(import_class_end(env))
  CHECK_BB(import_op(env, op_assign,        "complex", "complex", "complex", complex_assign,     0))
  CHECK_BB(import_op(env, op_plus,          "complex", "complex", "complex", complex_plus,       0))
  CHECK_BB(import_op(env, op_minus,         "complex", "complex", "complex", complex_minus,      0))
  CHECK_BB(import_op(env, op_times,         "complex", "complex", "complex", complex_times,      0))
  CHECK_BB(import_op(env, op_divide,        "complex", "complex", "complex", complex_divide,     0))
  CHECK_BB(import_op(env, op_chuck,         "complex", "complex", "complex", complex_r_assign,   0))
  CHECK_BB(import_op(env, op_plus_chuck,    "complex", "complex", "complex", complex_r_plus,     0))
  CHECK_BB(import_op(env, op_minus_chuck,   "complex", "complex", "complex", complex_r_minus,    0))
  CHECK_BB(import_op(env, op_times_chuck,   "complex", "complex", "complex", complex_r_times,    0))
  CHECK_BB(import_op(env, op_divide_chuck,  "complex", "complex", "complex", complex_r_divide,   0))
  CHECK_BB(import_op(env, op_assign,        "polar", "polar", "polar", complex_assign,           0))
  CHECK_BB(import_op(env, op_chuck,         "polar", "polar", "polar", complex_r_assign,         0))
  CHECK_BB(import_op(env, op_plus,          "polar", "polar", "polar", polar_plus,       0))
  CHECK_BB(import_op(env, op_minus,         "polar", "polar", "polar", polar_minus,      0))
  CHECK_BB(import_op(env, op_times,         "polar", "polar", "polar", polar_times,      0))
  CHECK_BB(import_op(env, op_divide,        "polar", "polar", "polar", polar_divide,     0))
  CHECK_BB(import_op(env, op_plus_chuck,    "polar", "polar", "polar", polar_plus_r,     0))
  CHECK_BB(import_op(env, op_minus_chuck,   "polar", "polar", "polar", polar_minus_r,    0))
  CHECK_BB(import_op(env, op_times_chuck,   "polar", "polar", "polar", polar_times_r,    0))
  CHECK_BB(import_op(env, op_divide_chuck,  "polar", "polar", "polar", polar_divide_r,   0))
  return 1;
}
