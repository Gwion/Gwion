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
  fun = new_dl_func("int", "i", (m_uint)coverage_int);
  CHECK_OB(import_sfun(env, fun))
  fun = new_dl_func("float", "f", (m_uint)coverage_float);
  CHECK_OB(import_sfun(env, fun))
  fun = new_dl_func("complex", "c", (m_uint)coverage_complex);
  CHECK_OB(import_sfun(env, fun))
  fun = new_dl_func("Vec3", "v", (m_uint)coverage_vec3);
  CHECK_OB(import_sfun(env, fun))
  fun = new_dl_func("Vec4", "w", (m_uint)coverage_vec4);
  CHECK_OB(import_sfun(env, fun))

  m_uint* i = malloc(sizeof(m_uint));
  *i = 5;
  import_svar(env, "int", "s_i", 1, 0, i, "");
  m_float* f = malloc(sizeof(m_float));
  *f = 2.1;
  import_svar(env, "float", "s_f", 1, 0, (void*)f, "");

  m_complex* c = malloc(sizeof(m_complex));
  *c = 2.1;
  import_svar(env, "complex", "s_c", 1, 0, (void*)c, "");

  VEC3_T* v = malloc(sizeof(VEC3_T));
  v->x = 2.1;
  v->y = 2.2;
  v->z = 2.3;
  import_svar(env, "Vec3", "s_v", 1, 0, (void*)v, "");

  VEC4_T* w = malloc(sizeof(VEC4_T));
  w->x = 2.1;
  w->y = 2.2;
  w->z = 2.3;
  w->w = 2.4;
  import_svar(env, "Vec4", "s_w", 1, 0, (void*)w, "");

  CHECK_BB(import_class_end(env))
  return 1;
}
