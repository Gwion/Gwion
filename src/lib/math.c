#include <stdlib.h>
#include <math.h>
#include "type.h"
#include "instr.h"
#include "import.h"

static struct Type_ t_math = { "Math", 0, NULL};

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
  m_int min = *(m_int*)MEM(SZ_INT);
  m_int max = *(m_int*)MEM(SZ_INT * 2);
  m_int range = max - min;
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
  m_float min = *(m_float*)MEM(SZ_INT);
  m_float max = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = min + (max - min) * (sp_rand(shred->vm_ref->sp) / (m_float)RAND_MAX);
}

static SFUN(math_srand) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  srand(ret);
  *(m_float*)RETURN = ret;
}

static SFUN(math_sgn) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_uint*)RETURN = ret < 0. ? -1 : ret > 0. ? 1 : 0;
}

static SFUN(math_sin) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = sin(ret);
}

static SFUN(math_cos) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = cos(ret);
}

static SFUN(math_tan) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = tan(ret);
}

static SFUN(math_asin) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = asin(ret);
}

static SFUN(math_acos) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = acos(ret);
}

static SFUN(math_atan) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = atan(ret);
}

static SFUN(math_sinh) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = sinh(ret);
}

static SFUN(math_cosh) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = cosh(ret);
}

static SFUN(math_tanh) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = tanh(ret);
}

static SFUN(math_asinh) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = sinh(ret);
}

static SFUN(math_acosh) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = cosh(ret);
}

static SFUN(math_atanh) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = atanh(ret);
}

static SFUN(math_atan2) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = atan2(ret1, ret2);
}

static SFUN(math_hypot) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = hypot(ret1, ret2);
}

static SFUN(math_pow) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = pow(ret1, ret2);
}

static SFUN(math_sqrt) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = sqrt(ret);
}

static SFUN(math_exp) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = exp(ret);
}

static SFUN(math_log) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = log(ret);
}

static SFUN(math_log2) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = log2(ret);
}

static SFUN(math_log10) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = log10(ret);
}

static SFUN(math_floor) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = floor(ret);
}

static SFUN(math_ceil) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = floor(ret);
}

static SFUN(math_round) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = round(ret);
}

static SFUN(math_trunc) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = trunc(ret);
}

static SFUN(math_fmod) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = fmod(ret1, ret2);
}

static SFUN(math_remainder) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = remainder(ret1, ret2);
}

static SFUN(math_min) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = ret1 < ret2 ? ret1 : ret2;
}

static SFUN(math_max) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = ret1 > ret2 ? ret1 : ret2;
}

static SFUN(math_isinf) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_uint*)RETURN = isinf(ret);
}

static SFUN(math_isnan) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_uint*)RETURN = isnan(ret);
}

m_bool import_math(Importer importer) {
  CHECK_BB(importer_class_ini(importer,  &t_math, NULL, NULL))

  importer_func_ini(importer, "int", "abs", (m_uint)math_abs);
  importer_func_arg(importer, "int", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "fabs", (m_uint)math_fabs);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "int", "rand", (m_uint)math_rand);
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "int", "rand2", (m_uint)math_rand2);
  importer_func_arg(importer, "int", "min");
  importer_func_arg(importer, "int", "max");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "randf", (m_uint)math_randf);
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "rand2f", (m_uint)math_rand2f);
  importer_func_arg(importer, "float", "min");
  importer_func_arg(importer, "float", "max");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "srand", (m_uint)math_srand);
  importer_func_arg(importer, "float", "seed");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "int", "sgn", (m_uint)math_sgn);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "hypot", (m_uint)math_hypot);
  importer_func_arg(importer, "float", "min");
  importer_func_arg(importer, "float", "max");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "sin", (m_uint)math_sin);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "cos", (m_uint)math_cos);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "tan", (m_uint)math_tan);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "asin", (m_uint)math_asin);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "acos", (m_uint)math_acos);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "atan", (m_uint)math_atan);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "atan2", (m_uint)math_atan2);
  importer_func_arg(importer, "float", "value");
  importer_func_arg(importer, "float", "value2");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "sinh", (m_uint)math_sinh);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "cosh", (m_uint)math_cosh);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "tanh", (m_uint)math_tanh);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "asinh", (m_uint)math_asinh);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "acosh", (m_uint)math_acosh);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "atanh", (m_uint)math_atanh);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "pow", (m_uint)math_pow);
  importer_func_arg(importer, "float", "value");
  importer_func_arg(importer, "float", "value2");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "sqrt", (m_uint)math_sqrt);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "exp", (m_uint)math_exp);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "log", (m_uint)math_log);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "log2", (m_uint)math_log2);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "log10", (m_uint)math_log10);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "floor", (m_uint)math_floor);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "ceil", (m_uint)math_ceil);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "round", (m_uint)math_round);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "trunc", (m_uint)math_trunc);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "fmod", (m_uint)math_fmod);
  importer_func_arg(importer, "float", "value");
  importer_func_arg(importer, "float", "value2");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "remainder", (m_uint)math_remainder);
  importer_func_arg(importer, "float", "value");
  importer_func_arg(importer, "float", "value2");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "min", (m_uint)math_min);
  importer_func_arg(importer, "float", "value");
  importer_func_arg(importer, "float", "value2");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "max", (m_uint)math_max);
  importer_func_arg(importer, "float", "value");
  importer_func_arg(importer, "float", "value2");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "isinf", (m_uint)math_isinf);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "float", "isnan", (m_uint)math_isnan);
  importer_func_arg(importer, "float", "value");
  CHECK_BB(importer_func_end(importer, ae_flag_static))
  CHECK_BB(importer_class_end(importer))

  return 1;
}
