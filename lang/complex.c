#include <math.h>
#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "import.h"

struct Type_ t_complex = { "complex", SZ_COMPLEX, NULL, te_complex};
struct Type_ t_polar   = { "polar",   SZ_COMPLEX, NULL, te_polar};
m_int o_complex_real, o_complex_imag;
m_int o_polar_mod,    o_polar_phase;

// arithmetic
static INSTR(assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(complex) '=' %p %p", **(complex**)(shred->reg - SZ_COMPLEX * 2), *(complex*)(shred->reg - SZ_COMPLEX));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  *(complex*)(shred->reg) = (**(complex**)shred->reg = *(complex*)(shred->reg + SZ_COMPLEX));
  shred->reg += SZ_COMPLEX;
}
static INSTR(plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(complex) '+' %p %p", *(complex*)(shred->reg - SZ_COMPLEX * 2), *(complex*)(shred->reg - SZ_COMPLEX));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  *(complex*)shred->reg += *(complex*)(shred->reg + SZ_COMPLEX);
  shred->reg += SZ_COMPLEX;
}

static INSTR(minus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(complex) '-' %p %p", *(complex*)(shred->reg - SZ_COMPLEX * 2), *(complex*)(shred->reg - SZ_COMPLEX));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  *(complex*)shred->reg -= *(complex*)(shred->reg + SZ_COMPLEX);
  shred->reg += SZ_COMPLEX;
}

INSTR(times)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(complex) '*' %p %p", *(complex*)(shred->reg - SZ_COMPLEX * 2), *(complex*)(shred->reg - SZ_COMPLEX));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  *(complex*)shred->reg *= *(complex*)(shred->reg + SZ_COMPLEX);
  shred->reg += SZ_COMPLEX;
}

static INSTR(divide)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(complex) '/' %p %p", *(complex*)(shred->reg - SZ_COMPLEX * 2), *(complex*)(shred->reg - SZ_COMPLEX));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  *(complex*)shred->reg /= *(complex*)(shred->reg + SZ_COMPLEX);
  shred->reg += SZ_COMPLEX;
}

// reverse arithmetic
static INSTR(r_assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(complex) '=>' %p %p", *(complex*)(shred->reg - SZ_COMPLEX - SZ_INT), **(complex**)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_COMPLEX + SZ_INT;
  **(complex**)(shred->reg + SZ_COMPLEX) = *(complex*)shred->reg;
  shred->reg += SZ_COMPLEX;
}

static INSTR(r_plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(complex) '+=>' %p %p", *(complex*)(shred->reg - SZ_COMPLEX - SZ_INT), **(complex**)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_COMPLEX + SZ_INT;
  *(complex*)(shred->reg) = (**(complex**)(shred->reg + SZ_COMPLEX) += (*(complex*)shred->reg));
  shred->reg += SZ_COMPLEX;
}

static INSTR(r_minus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(complex) '-=>' %p %p", *(complex*)(shred->reg - SZ_COMPLEX - SZ_INT), **(complex**)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  *(complex*)(shred->reg) = (**(complex**)(shred->reg + SZ_COMPLEX) -= (*(complex*)shred->reg));
  shred->reg += SZ_COMPLEX;
}

static INSTR(r_times)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(complex) '*=>' %p %p", *(complex*)(shred->reg - SZ_COMPLEX - SZ_INT), **(complex**)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  *(complex*)(shred->reg) = (**(complex**)(shred->reg + SZ_COMPLEX) *= (*(complex*)shred->reg));
  shred->reg += SZ_COMPLEX;
}

static INSTR(r_divide)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(complex) '/=>' %p %p", *(complex*)(shred->reg - SZ_COMPLEX - SZ_INT), **(complex**)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  *(complex*)(shred->reg) = (**(complex**)(shred->reg + SZ_COMPLEX) /= (*(complex*)shred->reg));
  shred->reg += SZ_COMPLEX;
}

INSTR(complex_real)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(complex) 'real' %i",  instr->m_val);
  /*  debug_msg("instr", "(complex) 'real' #(%f, %f)", */
  /*    instr->m_val ? creal(*(complex*)(shred->reg - SZ_COMPLEX)), cimag(*(complex*)(shred->reg - SZ_COMPLEX)));*/
#endif
  shred->reg -= instr->m_val ? SZ_INT : SZ_COMPLEX;
  if(instr->m_val) {
    *(m_float**)(shred->reg) = &**(m_float**)shred->reg;
    shred->reg += SZ_INT;
  } else {
    *(m_float*)(shred->reg) = *((m_float*)shred->reg);
    shred->reg += SZ_FLOAT;
  }
}

INSTR(complex_imag)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(complex) 'imag' #(%f, %f)",
            creal(*(complex*)(shred->reg - SZ_COMPLEX)), cimag(*(complex*)(shred->reg - SZ_COMPLEX)));
#endif
  shred->reg -= instr->m_val ? SZ_INT : SZ_COMPLEX;
  if(instr->m_val) {
    m_float* f = &**(m_float**)(shred->reg);
    *(m_float**)(shred->reg) = &*(f + 1);
    shred->reg += SZ_INT;
  } else {
    *(m_float*)(shred->reg) = *(m_float*)(shred->reg + SZ_FLOAT);
    shred->reg += SZ_FLOAT;
  }
}

INSTR(polar_plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(polar) %p '+' %p", *(complex*)(shred->reg - SZ_COMPLEX - SZ_INT), *(complex*)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  complex a = *(complex*)(shred->reg);
  complex b = *(complex*)(shred->reg + SZ_COMPLEX);
  m_float re = creal(a) * cos(cimag(a)) + creal(b) * cos(cimag(b));
  m_float im = creal(a) * sin(cimag(a)) + sin(cimag(b));
  *(complex*)shred->reg = hypot(re, im) + atan2(im, re) * I;
  shred->reg += SZ_COMPLEX;
}

INSTR(polar_minus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(polar) %p '+' %p", *(complex*)(shred->reg - SZ_COMPLEX - SZ_INT), *(complex*)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  complex a = *(complex*)(shred->reg);
  complex b = *(complex*)(shred->reg + SZ_COMPLEX);
  m_float re = creal(a) * cos(cimag(a)) + creal(b) * cos(cimag(b));
  m_float im = creal(a) * sin(cimag(a)) + sin(cimag(b));
  *(complex*)shred->reg = hypot(re, im) + atan2(im, re) * I;
  shred->reg += SZ_COMPLEX;
}

INSTR(polar_times)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(polar) %p '+' %p", *(complex*)(shred->reg - SZ_COMPLEX - SZ_INT), *(complex*)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  complex a = *(complex*)(shred->reg);
  complex b = *(complex*)(shred->reg + SZ_COMPLEX);
  m_float mag   = creal(a) * creal(b);
  m_float phase = cimag(a) + cimag(b);
  *(complex*)shred->reg = mag  + phase * I;
  shred->reg += SZ_COMPLEX;
}

INSTR(polar_divide)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(polar) %p '+' %p", *(complex*)(shred->reg - SZ_COMPLEX - SZ_INT), *(complex*)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  complex a = *(complex*)(shred->reg);
  complex b = *(complex*)(shred->reg + SZ_COMPLEX);
  m_float mag   = creal(a) / creal(b);
  m_float phase = cimag(a) - cimag(b);
  *(complex*)shred->reg = mag  + phase * I;
  shred->reg += SZ_COMPLEX;
}

INSTR(polar_plus_r)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(polar) %p '+=>' %p", *(complex*)(shred->reg - SZ_COMPLEX - SZ_INT), **(complex**)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_COMPLEX + SZ_INT;
  complex a = *(complex*)(shred->reg);
  complex b = **(complex**)(shred->reg + SZ_COMPLEX);
  m_float re = creal(a) * cos(cimag(a)) + creal(b) * cos(cimag(b));
  m_float im = creal(a) * sin(cimag(a)) + sin(cimag(b));
  **(complex**)(shred->reg + SZ_COMPLEX) = hypot(re, im) + atan2(im, re) * I;
  *(complex*)shred->reg = hypot(re, im) + atan2(im, re) * I;
  shred->reg += SZ_COMPLEX;
}

INSTR(polar_minus_r)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(polar) %p '+=>' %p", *(complex*)(shred->reg - SZ_COMPLEX - SZ_INT), **(complex**)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_COMPLEX + SZ_INT;
  complex a = *(complex*)(shred->reg);
  complex b = **(complex**)(shred->reg + SZ_COMPLEX);
  m_float re, im;
  re = creal(a) * cos(cimag(a)) - creal(b) * cos(cimag(b));
  im = creal(a) * sin(cimag(a)) - sin(cimag(b));
  **(complex**)(shred->reg + SZ_COMPLEX) = hypot(re, im) + atan2(im, re) * I;
  *(complex*)shred->reg = hypot(re, im) + atan2(im, re) * I;
  shred->reg += SZ_COMPLEX;
}

INSTR(polar_times_r)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(polar) %p '*=>' %p", *(complex*)(shred->reg - SZ_COMPLEX - SZ_INT), **(complex**)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  complex a = *(complex*)(shred->reg);
  complex b = **(complex**)(shred->reg + SZ_COMPLEX);
  m_float mag   = creal(a) * creal(b);
  m_float phase = cimag(a) + cimag(b);
  **(complex**)(shred->reg + SZ_COMPLEX) = mag  + phase * I;
  *(complex*)shred->reg = mag  + phase * I;
  shred->reg += SZ_COMPLEX;
}

INSTR(polar_divide_r)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "(polar) %p '/=>' %p", *(complex*)(shred->reg - SZ_COMPLEX - SZ_INT), **(complex**)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_COMPLEX * 2;
  complex a = *(complex*)(shred->reg);
  complex b = **(complex**)(shred->reg + SZ_COMPLEX);
  m_float mag   = creal(a) / creal(b);
  m_float phase = cimag(a) - cimag(b);
  **(complex**)(shred->reg + SZ_COMPLEX) = mag  + phase * I;
  *(complex*)shred->reg = mag  + phase * I;
  shred->reg += SZ_COMPLEX;
}


m_bool import_complex(Env env)
{
  CHECK_BB(add_global_type(env, &t_complex))
  CHECK_BB(import_class_begin(env, &t_complex, env->global_nspc, NULL, NULL))
  o_complex_real = import_mvar(env, "float", "re",   0, 0, "real part");
  CHECK_BB(o_complex_real)
  o_complex_imag = import_mvar(env, "float", "im",   0, 0, "imaginary part");
  CHECK_BB(o_complex_imag)
  CHECK_BB(import_class_end(env))

  CHECK_BB(add_global_type(env, &t_polar))
  CHECK_BB(import_class_begin(env, &t_polar, env->global_nspc, NULL, NULL))
  o_polar_mod = import_mvar(env, "float", "mod",   0, 0, "real part");
  CHECK_BB(o_polar_mod)
  o_polar_phase = import_mvar(env, "float", "phase",   0, 0, "imaginary part");
  CHECK_BB(o_polar_phase)
  CHECK_BB(import_class_end(env))
// arithmetic
  CHECK_BB(add_binary_op(env, op_assign,        &t_complex, &t_complex, &t_complex, assign,     0))
  CHECK_BB(add_binary_op(env, op_plus,          &t_complex, &t_complex, &t_complex, plus,       0))
  CHECK_BB(add_binary_op(env, op_minus,         &t_complex, &t_complex, &t_complex, minus,      0))
  CHECK_BB(add_binary_op(env, op_times,         &t_complex, &t_complex, &t_complex, times,      0))
  CHECK_BB(add_binary_op(env, op_divide,        &t_complex, &t_complex, &t_complex, divide,     0))
  /*	CHECK_BB(add_binary_op(env, op_percent,       &t_complex, &t_complex, &t_complex, modulo,     0)) */
// reverse arithmetic
  CHECK_BB(add_binary_op(env, op_chuck,         &t_complex, &t_complex, &t_complex, r_assign,   0))
  CHECK_BB(add_binary_op(env, op_plus_chuck,    &t_complex, &t_complex, &t_complex, r_plus,     0))
  CHECK_BB(add_binary_op(env, op_minus_chuck,   &t_complex, &t_complex, &t_complex, r_minus,    0))
  CHECK_BB(add_binary_op(env, op_times_chuck,   &t_complex, &t_complex, &t_complex, r_times,    0))
  CHECK_BB(add_binary_op(env, op_divide_chuck,  &t_complex, &t_complex, &t_complex, r_divide,   0))
  /*	CHECK_BB(add_binary_op(env, op_modulo_chuck,  &t_complex, &t_complex, &t_complex, r_modulo,   0)) */
  CHECK_BB(add_binary_op(env, op_plus,          &t_polar, &t_polar, &t_polar, polar_plus,       0))
  CHECK_BB(add_binary_op(env, op_minus,         &t_polar, &t_polar, &t_polar, polar_minus,      0))
  CHECK_BB(add_binary_op(env, op_times,         &t_polar, &t_polar, &t_polar, polar_times,      0))
  CHECK_BB(add_binary_op(env, op_divide,        &t_polar, &t_polar, &t_polar, polar_divide,     0))
  CHECK_BB(add_binary_op(env, op_plus_chuck,    &t_polar, &t_polar, &t_polar, polar_plus_r,     0))
  CHECK_BB(add_binary_op(env, op_minus_chuck,   &t_polar, &t_polar, &t_polar, polar_minus_r,    0))
  CHECK_BB(add_binary_op(env, op_times_chuck,   &t_polar, &t_polar, &t_polar, polar_times_r,    0))
  CHECK_BB(add_binary_op(env, op_divide_chuck,  &t_polar, &t_polar, &t_polar, polar_divide_r,   0))
  t_complex.doc   = "complex numbers";
  t_polar.doc     = "polar   numbers";
  t_complex.size = 16;
  t_polar.size = 16;
  return 1;
}
