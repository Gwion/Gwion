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

m_bool import_math(Env env) {
  DL_Func fun;

  CHECK_BB(import_class_begin(env, &t_math, NULL, NULL))

  dl_func_init(&fun, "int", "abs", (m_uint)math_abs);
  dl_func_add_arg(&fun, "int", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "fabs", (m_uint)math_fabs);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "int", "rand", (m_uint)math_rand);
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "int", "rand2", (m_uint)math_rand2);
  dl_func_add_arg(&fun, "int", "min");
  dl_func_add_arg(&fun, "int", "max");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "randf", (m_uint)math_randf);
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "rand2f", (m_uint)math_rand2f);
  dl_func_add_arg(&fun, "float", "min");
  dl_func_add_arg(&fun, "float", "max");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "srand", (m_uint)math_srand);
  dl_func_add_arg(&fun, "float", "seed");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "int", "sgn", (m_uint)math_sgn);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "hypot", (m_uint)math_hypot);
  dl_func_add_arg(&fun, "float", "min");
  dl_func_add_arg(&fun, "float", "max");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "sin", (m_uint)math_sin);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "cos", (m_uint)math_cos);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "tan", (m_uint)math_tan);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "asin", (m_uint)math_asin);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "acos", (m_uint)math_acos);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "atan", (m_uint)math_atan);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "atan2", (m_uint)math_atan2);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "value2");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "sinh", (m_uint)math_sinh);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "cosh", (m_uint)math_cosh);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "tanh", (m_uint)math_tanh);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "asinh", (m_uint)math_asinh);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "acosh", (m_uint)math_acosh);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "atanh", (m_uint)math_atanh);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "pow", (m_uint)math_pow);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "value2");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "sqrt", (m_uint)math_sqrt);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "exp", (m_uint)math_exp);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "log", (m_uint)math_log);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "log2", (m_uint)math_log2);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "log10", (m_uint)math_log10);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "floor", (m_uint)math_floor);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "ceil", (m_uint)math_ceil);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "round", (m_uint)math_round);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "trunc", (m_uint)math_trunc);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "fmod", (m_uint)math_fmod);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "value2");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "remainder", (m_uint)math_remainder);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "value2");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "min", (m_uint)math_min);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "value2");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "max", (m_uint)math_max);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "value2");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "isinf", (m_uint)math_isinf);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "isnan", (m_uint)math_isnan);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))
  CHECK_BB(import_class_end(env))

  return 1;
}
