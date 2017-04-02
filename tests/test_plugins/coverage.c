#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

static struct Type_ t_coverage = { "Coverage", SZ_INT, &t_object };

SFUN(coverage_int)     { RETURN->d.v_uint    = 0; }
SFUN(coverage_float)   { RETURN->d.v_float   = 0; }
SFUN(coverage_complex) { RETURN->d.v_complex = 0; }
SFUN(coverage_vec3)    { VEC3_T v = {0,0,0};   RETURN->d.v_vec3 = v; }
SFUN(coverage_vec4)    { VEC4_T v = {0,0,0,0}; RETURN->d.v_vec4 = v; }


IMPORT
{
  DL_Func* fun;
  CHECK_BB(add_global_type(env, &t_coverage))
  CHECK_BB(import_class_begin(env, &t_coverage, env->global_nspc, NULL, NULL))
  fun = new_DL_Func("int", "i", (m_uint)coverage_int);
  CHECK_OB(import_sfun(env, fun))
  fun = new_DL_Func("float", "f", (m_uint)coverage_float);
  CHECK_OB(import_sfun(env, fun))
  fun = new_DL_Func("complex", "c", (m_uint)coverage_complex);
  CHECK_OB(import_sfun(env, fun))
  fun = new_DL_Func("Vec3", "v", (m_uint)coverage_vec3);
  CHECK_OB(import_sfun(env, fun))
  fun = new_DL_Func("Vec4", "w", (m_uint)coverage_vec4);
  CHECK_OB(import_sfun(env, fun))
  CHECK_BB(import_class_end(env))
}
