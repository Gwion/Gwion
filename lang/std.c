#include <math.h>
#include <string.h>
#include <sndfile.h>
#include "type.h"
#include "dl.h"
#include "import.h"
#include "lang.h"
#include "bbq.h"

#define LOGTWO log(2)
#define LOGTEN log(10)
static double mtof( double f )
{
  if( f <= -1500 ) return (0);
  else if( f > 1499 ) return (mtof(1499));
// else return (8.17579891564 * exp(.0577622650 * f));
  // TODO: optimize
  else return ( pow(2, (f - 69) / 12.0) * 440.0 );
}

static double ftom( double f )
{
  // return (f > 0 ? 17.3123405046 * log(.12231220585 * f) : -1500);
  // TODO: optimize
  return (f > 0 ? (log(f / 440.0) / LOGTWO) * 12.0 + 69 : -1500);
}
static double powtodb( double f )
{
  if( f <= 0 ) return (0);
  else {
    double val = 100 + 10. / LOGTEN * log(f);
    return (val < 0 ? 0 : val);
  }
}


//-----------------------------------------------------------------------------
// name: rmstodb()
// desc: ...
//-----------------------------------------------------------------------------
static double rmstodb( double f )
{
  if( f <= 0 ) return (0);
  else {
    double val = 100 + 20. / LOGTEN * log(f);
    return (val < 0 ? 0 : val);
  }
}


//-----------------------------------------------------------------------------
// name: dbtopow()
// desc: ...
//-----------------------------------------------------------------------------
static double dbtopow( double f )
{
  if( f <= 0 )
    return (0);
  else {
    if( f > 870 ) f = 870;
    return (exp((LOGTEN * 0.1) * (f - 100.)));
  }
}
static double dbtorms( double f )
{
  if( f <= 0 )
    return (0);
  else {
    if( f > 485 ) f = 485;
    return (exp((LOGTEN * 0.05) * (f - 100.)));
  }
}

// start here
static SFUN(std_abs)
{
  RETURN->d.v_uint = labs(*(m_int*)(shred->mem + SZ_INT));
}

static SFUN(std_fabs)
{
#ifdef USE_DOUBLE
  RETURN->d.v_float = fabs(*(m_float*)(shred->mem + SZ_FLOAT));
#else
  RETURN->d.v_float = fabsf(*(m_float*)(shred->mem + SZ_FLOAT));
#endif
}

static SFUN(std_rand)
{
  RETURN->d.v_uint = sp_rand(shred->vm_ref->bbq->sp);
}

static SFUN(std_rand2)
{
  m_int min = *(m_int*)(shred->mem + SZ_INT);
  m_int max = *(m_int*)(shred->mem + SZ_INT * 2);
  m_int range = max - min;
  if ( range == 0 )
    RETURN->d.v_uint = min;
  else {
    if( range > 0 )
      RETURN->d.v_uint = min + (m_int)( (1.0 + range) * ( sp_rand(shred->vm_ref->bbq->sp) / (RAND_MAX + 1.0) )
                                      );
    else
      RETURN->d.v_uint = min - (m_int)( (-range + 1.0) * ( sp_rand(shred->vm_ref->bbq->sp) / (RAND_MAX + 1.0) )
                                      );
  }
}

static SFUN(std_randf)
{
  RETURN->d.v_float = 2.0 * sp_rand(shred->vm_ref->bbq->sp) / RAND_MAX - 1.0;
}

static SFUN(std_rand2f)
{
  m_float min = *(m_float*)(shred->mem + SZ_FLOAT);
  m_float max = *(m_float*)(shred->mem + SZ_FLOAT * 2);
  RETURN->d.v_float = min + (max - min) * (sp_rand(shred->vm_ref->bbq->sp) / (m_float)RAND_MAX);
}

static SFUN(std_srand)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  srand(ret);
  RETURN->d.v_float = ret;
}

static SFUN(std_sgn)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_uint = ret < 0. ? -1 : ret > 0. ? 1 : 0;
}

static SFUN(std_sin)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = sin(ret);
}

static SFUN(std_cos)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = cos(ret);
}

static SFUN(std_tan)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = tan(ret);
}

static SFUN(std_asin)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = asin(ret);
}

static SFUN(std_acos)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = acos(ret);
}

static SFUN(std_atan)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = atan(ret);
}

static SFUN(std_sinh)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = sinh(ret);
}

static SFUN(std_cosh)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = cosh(ret);
}

static SFUN(std_tanh)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = tanh(ret);
}

static SFUN(std_asinh)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = sinh(ret);
}

static SFUN(std_acosh)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = cosh(ret);
}

static SFUN(std_atanh)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = atanh(ret);
}

static SFUN(std_atan2)
{
  m_float ret1 = *(m_float*)(shred->mem + SZ_FLOAT);
  m_float ret2 = *(m_float*)(shred->mem + SZ_FLOAT * 2);
  RETURN->d.v_float = atan2(ret1, ret2);
}

static SFUN(std_hypot)
{
  m_float ret1 = *(m_float*)(shred->mem + SZ_FLOAT);
  m_float ret2 = *(m_float*)(shred->mem + SZ_FLOAT * 2);
  RETURN->d.v_float = hypot(ret1, ret2);
}

static SFUN(std_pow)
{
  m_float ret1 = *(m_float*)(shred->mem + SZ_FLOAT);
  m_float ret2 = *(m_float*)(shred->mem + SZ_FLOAT * 2);
  RETURN->d.v_float = pow(ret1, ret2);
}

static SFUN(std_sqrt)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = sqrt(ret);
}

static SFUN(std_exp)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = exp(ret);
}

static SFUN(std_log)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = log(ret);
}

static SFUN(std_log2)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = log2(ret);
}

static SFUN(std_log10)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = log10(ret);
}

static SFUN(std_floor)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = floor(ret);
}

static SFUN(std_ceil)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = floor(ret);
}

static SFUN(std_round)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = round(ret);
}

static SFUN(std_trunc)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_float = trunc(ret);
}

static SFUN(std_fmod)
{
  m_float ret1 = *(m_float*)(shred->mem + SZ_FLOAT);
  m_float ret2 = *(m_float*)(shred->mem + SZ_FLOAT * 2);
  RETURN->d.v_float = fmod(ret1, ret2);
}

static SFUN(std_remainder)
{
  m_float ret1 = *(m_float*)(shred->mem + SZ_FLOAT);
  m_float ret2 = *(m_float*)(shred->mem + SZ_FLOAT * 2);
  RETURN->d.v_float = remainder(ret1, ret2);
}

static SFUN(std_min)
{
  m_float ret1 = *(m_float*)(shred->mem + SZ_FLOAT);
  m_float ret2 = *(m_float*)(shred->mem + SZ_FLOAT * 2);
  RETURN->d.v_float = ret1 < ret2 ? ret1 : ret2;
}

static SFUN(std_max)
{
  m_float ret1 = *(m_float*)(shred->mem + SZ_FLOAT);
  m_float ret2 = *(m_float*)(shred->mem + SZ_FLOAT * 2);
  RETURN->d.v_float = ret1 > ret2 ? ret1 : ret2;
}

static SFUN(std_isinf)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_uint = isinf(ret);
}

static SFUN(std_isnan)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  RETURN->d.v_uint = isnan(ret);
}

static SFUN(std_clamp)
{
  m_int ret = *(m_int*)(shred->mem + SZ_INT);
  m_int min = *(m_int*)(shred->mem + SZ_INT * 2);
  m_int max = *(m_int*)(shred->mem + SZ_INT * 3);
  RETURN->d.v_uint = ret < min ? min : ret > max ? max : ret;
}

static SFUN(std_clampf)
{
  m_float ret = *(m_float*)(shred->mem + SZ_FLOAT);
  m_float min = *(m_float*)(shred->mem + SZ_FLOAT * 2);
  m_float max = *(m_float*)(shred->mem + SZ_FLOAT * 3);
  RETURN->d.v_float = ret < min ? min : ret > max ? max : ret;
}

static SFUN(std_scale)
{
  m_float ret  = *(m_float*)(shred->mem + SZ_FLOAT);
  m_float min1 = *(m_float*)(shred->mem + SZ_FLOAT * 2);
  m_float max1 = *(m_float*)(shred->mem + SZ_FLOAT * 3);
  m_float min2 = *(m_float*)(shred->mem + SZ_FLOAT * 4);
  m_float max2 = *(m_float*)(shred->mem + SZ_FLOAT * 5);
  RETURN->d.v_float = min2 + (max2 - min2) * ((ret - min1) / (max1 - min1));
}

static SFUN(std_system)
{
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  m_str str = STRING(obj);
  RETURN->d.v_uint = system(str);
  release(obj, shred);
}

static SFUN(std_getenv)
{
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  m_str str = STRING(obj);
  release(obj, shred);
  M_Object ret = new_M_Object();
  initialize_object(ret, &t_string);
  STRING(ret) = getenv(str);
  RETURN->d.v_object = ret;
}

static SFUN(std_setenv)
{
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  m_str key = STRING(obj);
  release(obj, shred);
  obj = *(M_Object*)(shred->mem + SZ_INT * 2);
  m_str value = STRING(obj);
  release(obj, shred);
  RETURN->d.v_uint = setenv(key, value, 1);
}

static SFUN(std_atoi)
{
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  m_str value = STRING(obj);
  release(obj, shred);
  RETURN->d.v_uint = atoi(value);
}

static SFUN(std_atof)
{
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  m_str value = STRING(obj);
  release(obj, shred);
  RETURN->d.v_float = atof(value);
}

static SFUN(std_itoa)
{
  char c[1024];
  int value = *(m_int*)(shred->mem + SZ_INT);
  M_Object ret = new_M_Object();
  initialize_object(ret, &t_string);
  sprintf(c, "%i", value);
  STRING(ret) = S_name(insert_symbol(c));
  RETURN->d.v_object = ret;
}

static SFUN(std_ftoa)
{
  char c[1024];
  m_float value = *(m_float*)(shred->mem + SZ_FLOAT);
  M_Object ret = new_M_Object();
  initialize_object(ret, &t_string);
  sprintf(c, "%f", value);
  STRING(ret) = S_name(insert_symbol(c));
  RETURN->d.v_object = ret;
}

static SFUN(std_ftoi)
{
  m_float value = *(m_float*)(shred->mem + SZ_FLOAT);
  m_int ret = value;
  RETURN->d.v_uint = ret;
}

static SFUN(std_mtof)
{
  RETURN->d.v_float = mtof(*(m_float*)(shred->mem + SZ_FLOAT));
}

static SFUN(std_ftom)
{
  RETURN->d.v_float = ftom(*(m_float*)(shred->mem + SZ_FLOAT));
}

static SFUN(std_powtodb)
{
  RETURN->d.v_float = powtodb(*(m_float*)(shred->mem + SZ_FLOAT));
}

static SFUN(std_rmstodb)
{
  RETURN->d.v_float = rmstodb(*(m_float*)(shred->mem + SZ_FLOAT));
}

static SFUN(std_dbtopow)
{
  RETURN->d.v_float = dbtopow(*(m_float*)(shred->mem + SZ_FLOAT));
}

static SFUN(std_dbtorms)
{
  RETURN->d.v_float = dbtorms(*(m_float*)(shred->mem + SZ_FLOAT));
}

static SFUN(std_dbtolin)
{
  RETURN->d.v_float = pow(10.0, *(m_float*)(shred->mem + SZ_FLOAT) / 20.0);
}

static SFUN(std_lintodb)
{
  RETURN->d.v_float = 20.0 * log10(*(m_float*)(shred->mem + SZ_FLOAT));
}

static struct Type_ t_math    = { "Math",    0, NULL};

m_bool import_lib(Env env)
{
  DL_Func* fun;

  CHECK_BB(add_global_type(env, &t_math))
  CHECK_OB(import_class_begin(env, &t_math, env->global_nspc, NULL, NULL))
  env->class_def->doc = "Standard library";

  fun = new_DL_Func("int", "abs",    (m_uint)std_abs);
  dl_func_add_arg(fun, "int", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "fabs", (m_uint)std_fabs);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("int", "rand",    (m_uint)std_rand);
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("int", "rand2",   (m_uint)std_rand2);
  dl_func_add_arg(fun, "int", "min");
  dl_func_add_arg(fun, "int", "max");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "randf",  (m_uint)std_randf);
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "rand2f", (m_uint)std_rand2f);
  dl_func_add_arg(fun, "float", "min");
  dl_func_add_arg(fun, "float", "max");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "srand",  (m_uint)std_srand);
  dl_func_add_arg(fun, "float", "seed");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("int", "sgn",      (m_uint)std_sgn);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "hypot", (m_uint)std_hypot);
  dl_func_add_arg(fun, "float", "min");
  dl_func_add_arg(fun, "float", "max");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "sin",      (m_uint)std_sin);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "cos",      (m_uint)std_cos);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "tan",      (m_uint)std_tan);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "asin",      (m_uint)std_asin);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "acos",      (m_uint)std_acos);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "atan",      (m_uint)std_atan);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "atan2",      (m_uint)std_atan2);
  dl_func_add_arg(fun, "float", "value");
  dl_func_add_arg(fun, "float", "value2");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "sinh",      (m_uint)std_sinh);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "cosh",      (m_uint)std_cosh);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "tanh",      (m_uint)std_tanh);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "asinh",      (m_uint)std_asinh);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "acosh",      (m_uint)std_acosh);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "atanh",      (m_uint)std_atanh);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "pow",      (m_uint)std_pow);
  dl_func_add_arg(fun, "float", "value");
  dl_func_add_arg(fun, "float", "value2");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "sqrt",      (m_uint)std_sqrt);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "exp",      (m_uint)std_exp);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "log",      (m_uint)std_log);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "log2",      (m_uint)std_log2);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "log10",      (m_uint)std_log10);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "floor",      (m_uint)std_floor);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "ceil",      (m_uint)std_ceil);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "round",      (m_uint)std_round);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "trunc",      (m_uint)std_trunc);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "fmod",      (m_uint)std_fmod);
  dl_func_add_arg(fun, "float", "value");
  dl_func_add_arg(fun, "float", "value2");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "remainder",      (m_uint)std_remainder);
  dl_func_add_arg(fun, "float", "value");
  dl_func_add_arg(fun, "float", "value2");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "min",      (m_uint)std_min);
  dl_func_add_arg(fun, "float", "value");
  dl_func_add_arg(fun, "float", "value2");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "max",      (m_uint)std_max);
  dl_func_add_arg(fun, "float", "value");
  dl_func_add_arg(fun, "float", "value2");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "isinf",      (m_uint)std_isinf);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "isnan",      (m_uint)std_isnan);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))
  CHECK_BB(import_class_end(env))

  CHECK_BB(add_global_type(env, &t_std))
  CHECK_OB(import_class_begin(env, &t_std, env->global_nspc, NULL, NULL))
  env->class_def->doc = "Standard library";

  fun = new_DL_Func("int", "clamp",     (m_uint)std_clamp);
  dl_func_add_arg(fun, "int", "value");
  dl_func_add_arg(fun, "int", "min");
  dl_func_add_arg(fun, "int", "max");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "clampf", (m_uint)std_clampf);
  dl_func_add_arg(fun, "float", "value");
  dl_func_add_arg(fun, "float", "min");
  dl_func_add_arg(fun, "float", "max");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "scale", (m_uint)std_scale);
  dl_func_add_arg(fun, "float", "value");
  dl_func_add_arg(fun, "float", "srcmin");
  dl_func_add_arg(fun, "float", "srcmax");
  dl_func_add_arg(fun, "float", "dstmin");
  dl_func_add_arg(fun, "float", "dstmax");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("int", "system",  (m_uint)std_system);
  dl_func_add_arg(fun, "string", "cmd");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("string", "getenv",  (m_uint)std_getenv);
  dl_func_add_arg(fun, "string", "key");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("int", "setenv",  (m_uint)std_setenv);
  dl_func_add_arg(fun, "string", "key");
  dl_func_add_arg(fun, "string", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("int", "atoi",  (m_uint)std_atoi);
  dl_func_add_arg(fun, "string", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "atof",  (m_uint)std_atof);
  dl_func_add_arg(fun, "string", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("string", "itoa",  (m_uint)std_itoa);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("string", "ftoa",  (m_uint)std_ftoa);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("int", "ftoi",  (m_uint)std_ftoi);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "mtof",  (m_uint)std_mtof);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "ftom",  (m_uint)std_ftom);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "powtodb",  (m_uint)std_powtodb);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "rmstodb",  (m_uint)std_rmstodb);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "dbtopow",  (m_uint)std_dbtopow);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "dbtorms",  (m_uint)std_dbtorms);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "dbtolin",  (m_uint)std_dbtolin);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))

  fun = new_DL_Func("float", "lintodb",  (m_uint)std_lintodb);
  dl_func_add_arg(fun, "float", "value");
  CHECK_OB(import_sfun(env, fun))
  CHECK_BB(import_class_end(env))

  return 1;
}
