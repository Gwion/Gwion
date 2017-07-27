#include <math.h>
#include <string.h>
#include <sndfile.h>
#include "type.h"
#include "instr.h"
#include "import.h"
#include "lang.h"
#include <dirent.h>
#ifdef USE_DOUBLE
#define fabs fabs
#undef USE_DOUBLE
#else
#define fabs fabsf
#endif
#include <soundpipe.h> // for sp_rand
#include "compile.h"
#include <err_msg.h>

#define LOGTWO log(2)
#define LOGTEN log(10)
static double mtof(double f) {
  if(f <= -1500) return (0);
  else if(f > 1499) return (mtof(1499));
// else return (8.17579891564 * exp(.0577622650 * f));
  else return (pow(2, (f - 69) / 12.0) * 440.0);
}

static double ftom(double f) {
  // return (f > 0 ? 17.3123405046 * log(.12231220585 * f) : -1500);
  return (f > 0 ? (log(f / 440.0) / LOGTWO) * 12.0 + 69 : -1500);
}
static double powtodb(double f) {
  if(f <= 0) return (0);
  else {
    double val = 100 + 10. / LOGTEN * log(f);
    return (val < 0 ? 0 : val);
  }
}


//-----------------------------------------------------------------------------
// name: rmstodb()
// desc: ...
//-----------------------------------------------------------------------------
static double rmstodb(double f) {
  if(f <= 0) return (0);
  else {
    double val = 100 + 20. / LOGTEN * log(f);
    return (val < 0 ? 0 : val);
  }
}


//-----------------------------------------------------------------------------
// name: dbtopow()
// desc: ...
//-----------------------------------------------------------------------------
static double dbtopow(double f) {
  if(f <= 0)
    return (0);
  else {
    if(f > 870) f = 870;
    return (exp((LOGTEN * 0.1) * (f - 100.)));
  }
}
static double dbtorms(double f) {
  if(f <= 0)
    return (0);
  else {
    if(f > 485) f = 485;
    return (exp((LOGTEN * 0.05) * (f - 100.)));
  }
}

// start here
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

static SFUN(std_clamp) {
  m_int ret = *(m_int*)MEM(SZ_INT);
  m_int min = *(m_int*)MEM(SZ_INT * 2);
  m_int max = *(m_int*)MEM(SZ_INT * 3);
  *(m_uint*)RETURN = ret < min ? min : ret > max ? max : ret;
}

static SFUN(std_clampf) {
  m_float ret = *(m_float*)MEM(SZ_INT);
  m_float min = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  m_float max = *(m_float*)MEM(SZ_INT + SZ_FLOAT * 2);
  *(m_float*)RETURN = ret < min ? min : ret > max ? max : ret;
}

static SFUN(std_scale) {
  m_float ret  = *(m_float*)MEM(SZ_INT);
  m_float min1 = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  m_float max1 = *(m_float*)MEM(SZ_INT + SZ_FLOAT * 2);
  m_float min2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT * 3);
  m_float max2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT * 4);
  *(m_float*)RETURN = min2 + (max2 - min2) * ((ret - min1) / (max1 - min1));
}

static SFUN(std_system) {
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_str str = STRING(obj);
  *(m_uint*)RETURN = system(str);
  release(obj, shred);
}

static SFUN(std_getenv) {
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_str str = getenv(STRING(obj));
  release(obj, shred);
  *(M_Object*)RETURN = str ? new_String(shred, str) : 0;
}

static SFUN(std_setenv) {
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_str key = STRING(obj);
  release(obj, shred);
  obj = *(M_Object*)MEM(SZ_INT * 2);
  m_str value = STRING(obj);
  release(obj, shred);
  *(m_uint*)RETURN = setenv(key, value, 1);
}

static SFUN(std_atoi) {
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_str value = STRING(obj);
  release(obj, shred);
  *(m_uint*)RETURN = atoi(value);
}

static SFUN(std_atof) {
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_str value = STRING(obj);
  release(obj, shred);
  *(m_float*)RETURN = atof(value);
}

static SFUN(std_itoa) {
  char c[1024];
  int value = *(m_int*)MEM(SZ_INT);
  sprintf(c, "%i", value);
  *(M_Object*)RETURN = new_String(shred, c);
}

static SFUN(std_ftoa) {
  char c[1024];
  m_float value = *(m_float*)MEM(SZ_INT);
  sprintf(c, "%f", value);
  *(M_Object*)RETURN = new_String(shred, c);
}

static SFUN(std_ftoi) {
  m_float value = *(m_float*)MEM(SZ_INT);
  m_int ret = value;
  *(m_uint*)RETURN = ret;
}

static SFUN(std_mtof) {
  *(m_float*)RETURN = mtof(*(m_float*)MEM(SZ_INT));
}

static SFUN(std_ftom) {
  *(m_float*)RETURN = ftom(*(m_float*)MEM(SZ_INT));
}

static SFUN(std_powtodb) {
  *(m_float*)RETURN = powtodb(*(m_float*)MEM(SZ_INT));
}

static SFUN(std_rmstodb) {
  *(m_float*)RETURN = rmstodb(*(m_float*)MEM(SZ_INT));
}

static SFUN(std_dbtopow) {
  *(m_float*)RETURN = dbtopow(*(m_float*)MEM(SZ_INT));
}

static SFUN(std_dbtorms) {
  *(m_float*)RETURN = dbtorms(*(m_float*)MEM(SZ_INT));
}

static SFUN(std_dbtolin) {
  *(m_float*)RETURN = pow(10.0, *(m_float*)MEM(SZ_INT) / 20.0);
}

static SFUN(std_lintodb) {
  *(m_float*)RETURN = 20.0 * log10(*(m_float*)MEM(SZ_INT));
}

static struct Type_ t_std  = { "Std",  0, NULL, te_std};
static struct Type_ t_math = { "Math", 0, NULL};

m_bool import_lib(Env env) {
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

  CHECK_BB(import_class_begin(env, &t_std, env->global_nspc, NULL, NULL))

  dl_func_init(&fun, "int", "clamp", (m_uint)std_clamp);
  dl_func_add_arg(&fun, "int", "value");
  dl_func_add_arg(&fun, "int", "min");
  dl_func_add_arg(&fun, "int", "max");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "clampf", (m_uint)std_clampf);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "min");
  dl_func_add_arg(&fun, "float", "max");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "scale", (m_uint)std_scale);
  dl_func_add_arg(&fun, "float", "value");
  dl_func_add_arg(&fun, "float", "srcmin");
  dl_func_add_arg(&fun, "float", "srcmax");
  dl_func_add_arg(&fun, "float", "dstmin");
  dl_func_add_arg(&fun, "float", "dstmax");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "int", "system", (m_uint)std_system);
  dl_func_add_arg(&fun, "string", "cmd");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "string", "getenv", (m_uint)std_getenv);
  dl_func_add_arg(&fun, "string", "key");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "int", "setenv", (m_uint)std_setenv);
  dl_func_add_arg(&fun, "string", "key");
  dl_func_add_arg(&fun, "string", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "int", "atoi", (m_uint)std_atoi);
  dl_func_add_arg(&fun, "string", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "atof", (m_uint)std_atof);
  dl_func_add_arg(&fun, "string", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "string", "itoa", (m_uint)std_itoa);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "string", "ftoa", (m_uint)std_ftoa);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "int", "ftoi", (m_uint)std_ftoi);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "mtof", (m_uint)std_mtof);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "ftom", (m_uint)std_ftom);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "powtodb", (m_uint)std_powtodb);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "rmstodb", (m_uint)std_rmstodb);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "dbtopow", (m_uint)std_dbtopow);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "dbtorms", (m_uint)std_dbtorms);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "dbtolin", (m_uint)std_dbtolin);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "float", "lintodb", (m_uint)std_lintodb);
  dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))
  CHECK_BB(import_class_end(env))

  return 1;
}

struct Type_ t_machine   = { "Machine",      0, NULL, te_machine};

static m_str randstring(VM* vm, int length) {
  char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";
  size_t stringLen = 26 * 2 + 10 + 2;
  char *randomString;

  randomString = malloc(sizeof(char) * (length + 1));

  if(!randomString) {
    return (char*)0;
  }

  for(int n = 0; n < length; n++) {
    unsigned int key = sp_rand(vm->sp) % stringLen;
    randomString[n] = string[key];
  }

  randomString[length] = '\0';

  return randomString;
}

static SFUN(machine_add) {
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(!obj)
    return;
  m_str str = STRING(obj);
  release(obj, shred);
  if(!str)
    return;
  *(m_uint*)RETURN = compile(shred->vm_ref, str);
}

static SFUN(machine_check) {
  Ast ast;
  FILE* file;
  m_str prefix, filename, s;
  M_Object prefix_obj = *(M_Object*)MEM(SZ_INT);
  M_Object code_obj = *(M_Object*)MEM(SZ_INT * 2);

  *(m_uint*)RETURN = 0;
  if(!prefix_obj)
    prefix = ".";
  else {
    prefix = STRING(prefix_obj);
    release(prefix_obj, shred);
  }
  if(!code_obj)
    return;
  char c[strlen(prefix) + 17];
  filename = randstring(shred->vm_ref, 12);
  sprintf(c, "%s/%s", prefix, filename);
  if(!(file = fopen(c, "w"))) {
    free(filename);
    return;
  }
  fprintf(file, "%s\n", STRING(code_obj));
  release(code_obj, shred);
  fclose(file);
  free(filename);
  if(!(ast = parse(c)))
    return;
  s = strdup(c);
  if(type_engine_check_prog(shred->vm_ref->emit->env, ast, s) < 0)
    return;
  free(s);
  free_ast(ast);
  *(m_uint*)RETURN = 1;
}

static SFUN(machine_compile) {

  m_str prefix, filename;
  FILE* file;
  M_Object prefix_obj = *(M_Object*)MEM(SZ_INT);
  M_Object code_obj = *(M_Object*)MEM(SZ_INT * 2);

  *(m_uint*)RETURN = 0;
  if(!prefix_obj)
    prefix = ".";
  else {
    prefix = STRING(prefix_obj);
    release(prefix_obj, shred);
  }
  if(!code_obj) return;
  char c[strlen(prefix) + 17];
  filename = randstring(shred->vm_ref, 12);
  sprintf(c, "%s/%s.gw", prefix, filename);
  if(!(file = fopen(c, "w"))) {
    free(filename);
    return;
  }
  fprintf(file, "%s\n", STRING(code_obj));
  release(code_obj, shred);
  fclose(file);
  free(filename);
  compile(shred->vm_ref, c);
}

static SFUN(machine_shreds) {
  int i;
  VM* vm = shred->vm_ref;
  VM_Shred sh;
  M_Object obj = new_M_Array(SZ_INT, vector_size(&vm->shred), 1);
  for(i = 0; i < vector_size(&vm->shred); i++) {
    sh = (VM_Shred)vector_at(&vm->shred, i);
    i_vector_set(ARRAY(obj), i, sh->xid);
  }
  vector_add(&shred->gc, (vtype)obj);
  *(M_Object*)RETURN = obj;
}

m_bool import_machine(Env env) {
  DL_Func fun;

  CHECK_BB(import_class_begin(env, &t_machine, env->global_nspc, NULL, NULL))
  dl_func_init(&fun, "void",  "add", (m_uint)machine_add);
  dl_func_add_arg(&fun,       "string",  "filename");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "int[]", "shreds", (m_uint)machine_shreds);
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "int",  "check", (m_uint)machine_check);
  dl_func_add_arg(&fun,      "string",  "prefix");
  dl_func_add_arg(&fun,      "string",  "code");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "void", "compile", (m_uint)machine_compile);
  dl_func_add_arg(&fun,      "string",  "prefix");
  dl_func_add_arg(&fun,      "string",  "filename");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  CHECK_BB(import_class_end(env))
  return 1;
}
