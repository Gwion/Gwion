#include <stdlib.h>
#include <math.h>
#include "type.h"
#include "instr.h"
#include "import.h"

static SFUN(math_abs) {
  *(m_uint*)RETURN = labs(*(m_int*)MEM(SZ_INT));
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

#define math1(func)                           \
static SFUN(math_##func) {                    \
  const m_float ret = *(m_float*)MEM(SZ_INT); \
  *(m_float*)RETURN = func(ret);              \
}

math1(fabs)
math1(sin)
math1(cos)
math1(tan)
math1(asin)
math1(acos)
math1(atan)
math1(sinh)
math1(cosh)
math1(tanh)
math1(asinh)
math1(acosh)
math1(atanh)
math1(sqrt)
math1(exp)
math1(log)
math1(log2)
math1(log10)
math1(floor)
math1(ceil)
math1(round)
math1(trunc)
math1(isinf)
math1(isnan)

static m_float min(m_float f1, m_float f2) { return f1 < f2 ? f1 : f2; }
static m_float max(m_float f1, m_float f2) { return f1 > f2 ? f1 : f2; }

#define math2(func)                                       \
static SFUN(math_##func) {                                \
  const m_float ret1 = *(m_float*)MEM(SZ_INT);            \
  const m_float ret2 = *(m_float*)MEM(SZ_INT + SZ_FLOAT); \
  *(m_float*)RETURN = func(ret1, ret2);                   \
}
math2(atan2)
math2(hypot)
math2(pow)
math2(fmod)
math2(remainder)
math2(min)
math2(max)

#define decl_math1(name, func)                   \
  gwi_func_ini(gwi, "float", name, math_##func); \
  gwi_func_arg(gwi, "float", "value");           \
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))    \

#define decl_math2(name, func)                   \
  gwi_func_ini(gwi, "float", name, math_##func); \
  gwi_func_arg(gwi, "float", "value1");          \
  gwi_func_arg(gwi, "float", "value2");          \
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))    \

ANN m_bool import_math(const Gwi gwi) {
  const Type t_math = gwi_mk_type(gwi, "Math", 0, NULL);
  CHECK_OB(t_math)

  CHECK_BB(gwi_class_ini(gwi, t_math, NULL, NULL))

  gwi_func_ini(gwi, "int", "abs", math_abs);
  gwi_func_arg(gwi, "int", "value");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "int", "rand", math_rand);
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "int", "rand2", math_rand2);
  gwi_func_arg(gwi, "int", "min");
  gwi_func_arg(gwi, "int", "max");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "float", "randf", math_randf);
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  decl_math2("rand2f", rand2f)

  decl_math1("fabs",  fabs)
  decl_math1("srand", srand)
  decl_math1("sgn",   sgn)
  decl_math1("sin",   sin)
  decl_math1("cos",   cos)
  decl_math1("tan",   tan)
  decl_math1("asin",  asin)
  decl_math1("acos",  acos)
  decl_math1("atan",  atan)
  decl_math1("sinh",  sinh)
  decl_math1("cosh",  cosh)
  decl_math1("tanh",  tanh)
  decl_math1("asinh", asinh)
  decl_math1("acosh", acosh)
  decl_math1("atanh", atanh)
  decl_math1("sqrt",  sqrt)
  decl_math1("exp",   exp)
  decl_math1("log",   log)
  decl_math1("log2",  log2)
  decl_math1("log10", log10)
  decl_math1("floor", floor)
  decl_math1("ceil",  ceil)
  decl_math1("round", round)
  decl_math1("trunc", trunc)
  decl_math1("isinf", isinf)
  decl_math1("isnan", isnan)

  decl_math2("hypot",     hypot)
  decl_math2("atan2",     atan2)
  decl_math2("pow",       pow)
  decl_math2("fmod",      fmod)
  decl_math2("remainder", remainder)
  decl_math2("min",       min)
  decl_math2("max",       max)
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
