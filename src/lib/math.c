#include <stdlib.h>
#include <math.h>
#include "type.h"
#include "instr.h"
#include "import.h"

static SFUN(math_abs) {
  *(m_uint*)RETURN = labs(*(m_int*)MEM(SZ_INT));
}

static SFUN(math_fabs) {
  *(m_float*)RETURN = fabs(*(m_float*)MEM(SZ_INT));
}

static SFUN(math_rand) {
  *(m_uint*)RETURN = sp_rand(shred->vm_ref->sp);
}

static SFUN(math_rand2) {
  const m_int min = *(m_int*)MEM(SZ_INT);
  const m_int max = *(m_int*)MEM(SZ_INT * 2);
  const m_int range = max - min;
  if(range == 0)
    *(m_uint*)RETURN = min;
  else {
    if(range > 0)
      *(m_uint*)RETURN = min + (m_int)((1.0 + range) * (sp_rand(shred->vm_ref->sp) / (RAND_MAX + 1.0)));
    else
      *(m_uint*)RETURN = min - (m_int)((-range + 1.0) * (sp_rand(shred->vm_ref->sp) / (RAND_MAX + 1.0)));
  }
}

static SFUN(math_randf) {
  *(m_float*)RETURN = 2.0 * sp_rand(shred->vm_ref->sp) / RAND_MAX - 1.0;
}

static SFUN(math_rand2f) {
  const m_float min = *(m_float*)MEM(SZ_INT);
  const m_float max = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = min + (max - min) * (sp_rand(shred->vm_ref->sp) / (m_float)RAND_MAX);
}

static SFUN(math_srand) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  srand(ret);
  *(m_float*)RETURN = ret;
}

static SFUN(math_sgn) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_uint*)RETURN = ret < 0. ? -1 : ret > 0. ? 1 : 0;
}

static SFUN(math_sin) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = sin(ret);
}

static SFUN(math_cos) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = cos(ret);
}

static SFUN(math_tan) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = tan(ret);
}

static SFUN(math_asin) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = asin(ret);
}

static SFUN(math_acos) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = acos(ret);
}

static SFUN(math_atan) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = atan(ret);
}

static SFUN(math_sinh) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = sinh(ret);
}

static SFUN(math_cosh) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = cosh(ret);
}

static SFUN(math_tanh) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = tanh(ret);
}

static SFUN(math_asinh) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = sinh(ret);
}

static SFUN(math_acosh) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = cosh(ret);
}

static SFUN(math_atanh) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = atanh(ret);
}

static SFUN(math_atan2) {
  const m_float ret1 = *(m_float*)MEM(SZ_INT);
  const m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = atan2(ret1, ret2);
}

static SFUN(math_hypot) {
  const m_float ret1 = *(m_float*)MEM(SZ_INT);
  const m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = hypot(ret1, ret2);
}

static SFUN(math_pow) {
  const m_float ret1 = *(m_float*)MEM(SZ_INT);
  const m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = pow(ret1, ret2);
}

static SFUN(math_sqrt) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = sqrt(ret);
}

static SFUN(math_exp) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = exp(ret);
}

static SFUN(math_log) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = log(ret);
}

static SFUN(math_log2) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = log2(ret);
}

static SFUN(math_log10) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = log10(ret);
}

static SFUN(math_floor) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = floor(ret);
}

static SFUN(math_ceil) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = floor(ret);
}

static SFUN(math_round) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = round(ret);
}

static SFUN(math_trunc) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = trunc(ret);
}

static SFUN(math_fmod) {
  const m_float ret1 = *(m_float*)MEM(SZ_INT);
  const m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = fmod(ret1, ret2);
}

static SFUN(math_remainder) {
  const m_float ret1 = *(m_float*)MEM(SZ_INT);
  const m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = remainder(ret1, ret2);
}

static SFUN(math_min) {
  const m_float ret1 = *(m_float*)MEM(SZ_INT);
  const m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = ret1 < ret2 ? ret1 : ret2;
}

static SFUN(math_max) {
  const m_float ret1 = *(m_float*)MEM(SZ_INT);
  const m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = ret1 > ret2 ? ret1 : ret2;
}

static SFUN(math_isinf) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_uint*)RETURN = isinf(ret);
}

static SFUN(math_isnan) {
  const m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_uint*)RETURN = isnan(ret);
}

ANN m_bool import_math(const Gwi gwi) {
  Type t_math;
  CHECK_OB((t_math = gwi_mk_type(gwi, "Math", 0, NULL)))


  CHECK_BB(gwi_class_ini(gwi,  t_math, NULL, NULL))

  gwi_func_ini(gwi, "int", "abs", math_abs);
  gwi_func_arg(gwi, "int", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "fabs", math_fabs);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "int", "rand", math_rand);
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "int", "rand2", math_rand2);
  gwi_func_arg(gwi, "int", "min");
  gwi_func_arg(gwi, "int", "max");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "randf", math_randf);
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "rand2f", math_rand2f);
  gwi_func_arg(gwi, "float", "min");
  gwi_func_arg(gwi, "float", "max");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "srand", math_srand);
  gwi_func_arg(gwi, "float", "seed");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "int", "sgn", math_sgn);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "hypot", math_hypot);
  gwi_func_arg(gwi, "float", "min");
  gwi_func_arg(gwi, "float", "max");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "sin", math_sin);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "cos", math_cos);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "tan", math_tan);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "asin", math_asin);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "acos", math_acos);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "atan", math_atan);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "atan2", math_atan2);
  gwi_func_arg(gwi, "float", "value");
  gwi_func_arg(gwi, "float", "value2");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "sinh", math_sinh);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "cosh", math_cosh);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "tanh", math_tanh);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "asinh", math_asinh);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "acosh", math_acosh);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "atanh", math_atanh);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "pow", math_pow);
  gwi_func_arg(gwi, "float", "value");
  gwi_func_arg(gwi, "float", "value2");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "sqrt", math_sqrt);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "exp", math_exp);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "log", math_log);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "log2", math_log2);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "log10", math_log10);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "floor", math_floor);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "ceil", math_ceil);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "round", math_round);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "trunc", math_trunc);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "fmod", math_fmod);
  gwi_func_arg(gwi, "float", "value");
  gwi_func_arg(gwi, "float", "value2");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "remainder", math_remainder);
  gwi_func_arg(gwi, "float", "value");
  gwi_func_arg(gwi, "float", "value2");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "min", math_min);
  gwi_func_arg(gwi, "float", "value");
  gwi_func_arg(gwi, "float", "value2");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "max", math_max);
  gwi_func_arg(gwi, "float", "value");
  gwi_func_arg(gwi, "float", "value2");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "isinf", math_isinf);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "isnan", math_isnan);
  gwi_func_arg(gwi, "float", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))
  CHECK_BB(gwi_class_end(gwi))

  return 1;
}
