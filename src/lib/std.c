#include <math.h>
#include "type.h"
#include "instr.h"
#include "import.h"
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

static struct Type_ t_std  = { "Std",  0, NULL, te_std};

static SFUN(std_system) {
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_str str = STRING(obj);
  *(m_uint*)RETURN = system(str);
  release(obj, shred);
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

m_bool import_std(Importer importer) {
  CHECK_BB(importer_class_begin(importer,  &t_std, NULL, NULL))

  importer_func_begin(importer, "int", "clamp", (m_uint)std_clamp);
  importer_add_arg(importer, "int", "value");
  importer_add_arg(importer, "int", "min");
  importer_add_arg(importer, "int", "max");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "float", "clampf", (m_uint)std_clampf);
  importer_add_arg(importer, "float", "value");
  importer_add_arg(importer, "float", "min");
  importer_add_arg(importer, "float", "max");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "float", "scale", (m_uint)std_scale);
  importer_add_arg(importer, "float", "value");
  importer_add_arg(importer, "float", "srcmin");
  importer_add_arg(importer, "float", "srcmax");
  importer_add_arg(importer, "float", "dstmin");
  importer_add_arg(importer, "float", "dstmax");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "int", "system", (m_uint)std_system);
  importer_add_arg(importer, "string", "cmd");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "string", "getenv", (m_uint)std_getenv);
  importer_add_arg(importer, "string", "key");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "int", "setenv", (m_uint)std_setenv);
  importer_add_arg(importer, "string", "key");
  importer_add_arg(importer, "string", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "int", "atoi", (m_uint)std_atoi);
  importer_add_arg(importer, "string", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "float", "atof", (m_uint)std_atof);
  importer_add_arg(importer, "string", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "string", "itoa", (m_uint)std_itoa);
  importer_add_arg(importer, "float", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "string", "ftoa", (m_uint)std_ftoa);
  importer_add_arg(importer, "float", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "int", "ftoi", (m_uint)std_ftoi);
  importer_add_arg(importer, "float", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "float", "mtof", (m_uint)std_mtof);
  importer_add_arg(importer, "float", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "float", "ftom", (m_uint)std_ftom);
  importer_add_arg(importer, "float", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "float", "powtodb", (m_uint)std_powtodb);
  importer_add_arg(importer, "float", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "float", "rmstodb", (m_uint)std_rmstodb);
  importer_add_arg(importer, "float", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "float", "dbtopow", (m_uint)std_dbtopow);
  importer_add_arg(importer, "float", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "float", "dbtorms", (m_uint)std_dbtorms);
  importer_add_arg(importer, "float", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "float", "dbtolin", (m_uint)std_dbtolin);
  importer_add_arg(importer, "float", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  importer_func_begin(importer, "float", "lintodb", (m_uint)std_lintodb);
  importer_add_arg(importer, "float", "value");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))
  CHECK_BB(importer_class_end(importer))

  return 1;

}
