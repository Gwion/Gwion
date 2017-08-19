#include <math.h>
#include "type.h"
#include "instr.h"
#include "import.h"

static struct Type_ t_math = { "Math", 0, NULL};

static SFUN(std_abs) {
  *(m_uint*)RETURN = labs(*(m_int*)MEM(SZ_INT));
}

static SFUN(std_fabs) {
  *(m_float*)RETURN = fabs(*(m_float*)MEM(SZ_INT));
}

static SFUN(std_rand) {
  *(m_uint*)RETURN = sp_rand(shred->vm_ref->sp);
}

static SFUN(std_rand2) {
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

static SFUN(std_randf) {
  *(m_float*)RETURN = 2.0 * sp_rand(shred->vm_ref->sp) / RAND_MAX - 1.0;
}

static SFUN(std_rand2f) {
  m_float min = *(m_float*)MEM(SZ_INT);
  m_float max = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = min + (max - min) * (sp_rand(shred->vm_ref->sp) / (m_float)RAND_MAX);
}

static SFUN(std_srand) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  srand(ret);
  *(m_float*)RETURN = ret;
}

static SFUN(std_sgn) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_uint*)RETURN = ret < 0. ? -1 : ret > 0. ? 1 : 0;
}

static SFUN(std_sin) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = sin(ret);
}

static SFUN(std_cos) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = cos(ret);
}

static SFUN(std_tan) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = tan(ret);
}

static SFUN(std_asin) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = asin(ret);
}

static SFUN(std_acos) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = acos(ret);
}

static SFUN(std_atan) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = atan(ret);
}

static SFUN(std_sinh) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = sinh(ret);
}

static SFUN(std_cosh) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = cosh(ret);
}

static SFUN(std_tanh) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = tanh(ret);
}

static SFUN(std_asinh) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = sinh(ret);
}

static SFUN(std_acosh) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = cosh(ret);
}

static SFUN(std_atanh) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = atanh(ret);
}

static SFUN(std_atan2) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = atan2(ret1, ret2);
}

static SFUN(std_hypot) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = hypot(ret1, ret2);
}

static SFUN(std_pow) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = pow(ret1, ret2);
}

static SFUN(std_sqrt) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = sqrt(ret);
}

static SFUN(std_exp) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = exp(ret);
}

static SFUN(std_log) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = log(ret);
}

static SFUN(std_log2) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = log2(ret);
}

static SFUN(std_log10) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = log10(ret);
}

static SFUN(std_floor) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = floor(ret);
}

static SFUN(std_ceil) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = floor(ret);
}

static SFUN(std_round) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = round(ret);
}

static SFUN(std_trunc) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = trunc(ret);
}

static SFUN(std_fmod) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = fmod(ret1, ret2);
}

static SFUN(std_remainder) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = remainder(ret1, ret2);
}

static SFUN(std_min) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = ret1 < ret2 ? ret1 : ret2;
}

static SFUN(std_max) {
  m_float ret1 = *(m_float*)MEM(SZ_INT);
  m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  *(m_float*)RETURN = ret1 > ret2 ? ret1 : ret2;
}

static SFUN(std_isinf) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_uint*)RETURN = isinf(ret);
}

static SFUN(std_isnan) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  *(m_uint*)RETURN = isnan(ret);
}

m_bool import_math(Env env) {
  DL_Func fun;

  CHECK_BB(import_class_begin(env, &t_math, env->global_nspc, NULL, NULL))

  dl_func_init(&fun, "int", "abs", (m_uint)std_abs);
  dl_func_add_arg(&fun, "int", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "fabs", (m_uint)std_fabs);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "int", "rand", (m_uint)std_rand);
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "int", "rand2", (m_uint)std_rand2);
  dl_func_add_arg(&fun, "int", "min");
  dl_func_add_arg(&fun, "int", "max");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "randf", (m_uint)std_randf);
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "rand2f", (m_uint)std_rand2f);
  dl_func_add_arg(&fun, "float", "min");
  dl_func_add_arg(&fun, "float", "max");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "srand", (m_uint)std_srand);
  dl_func_add_arg(&fun, "float", "seed");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "int", "sgn", (m_uint)std_sgn);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "hypot", (m_uint)std_hypot);
  dl_func_add_arg(&fun, "float", "min");
  dl_func_add_arg(&fun, "float", "max");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "sin", (m_uint)std_sin);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "cos", (m_uint)std_cos);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "tan", (m_uint)std_tan);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "asin", (m_uint)std_asin);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "acos", (m_uint)std_acos);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "atan", (m_uint)std_atan);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "atan2", (m_uint)std_atan2);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "value2");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "sinh", (m_uint)std_sinh);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "cosh", (m_uint)std_cosh);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "tanh", (m_uint)std_tanh);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "asinh", (m_uint)std_asinh);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "acosh", (m_uint)std_acosh);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "atanh", (m_uint)std_atanh);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "pow", (m_uint)std_pow);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "value2");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "sqrt", (m_uint)std_sqrt);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "exp", (m_uint)std_exp);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "log", (m_uint)std_log);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "log2", (m_uint)std_log2);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "log10", (m_uint)std_log10);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "floor", (m_uint)std_floor);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "ceil", (m_uint)std_ceil);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "round", (m_uint)std_round);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "trunc", (m_uint)std_trunc);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "fmod", (m_uint)std_fmod);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "value2");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "remainder", (m_uint)std_remainder);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "value2");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "min", (m_uint)std_min);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "value2");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "max", (m_uint)std_max);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "value2");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "isinf", (m_uint)std_isinf);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "isnan", (m_uint)std_isnan);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))
  CHECK_BB(import_class_end(env))

  return 1;
}
