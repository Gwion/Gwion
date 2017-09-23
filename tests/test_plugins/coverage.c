#include "type.h"
#include "import.h"

static struct Type_ t_coverage = { "Coverage", SZ_INT, &t_object };

SFUN(coverage_int)     { *(m_uint*)RETURN    = 0; }
SFUN(coverage_float)   { *(m_float*)RETURN   = 0; }
SFUN(coverage_complex) { *(m_complex*)RETURN = 0; }
SFUN(coverage_vec3)    { m_vec3 v = {0,0,0};   *(m_vec3*)RETURN = v; }
SFUN(coverage_vec4)    { m_vec4 v = {0,0,0,0}; *(m_vec4*)RETURN = v; }


IMPORT
{
  CHECK_BB(importer_class_begin(importer, &t_coverage, NULL, NULL))
  importer_func_begin(importer, "int", "i", (m_uint)coverage_int);
  CHECK_BB(importer_add_fun(importer, ae_flag_static))
  importer_func_begin(importer, "float", "f", (m_uint)coverage_float);
  CHECK_BB(importer_add_fun(importer, ae_flag_static))
  importer_func_begin(importer, "complex", "c", (m_uint)coverage_complex);
  CHECK_BB(importer_add_fun(importer, ae_flag_static))
  importer_func_begin(importer, "Vec3", "v", (m_uint)coverage_vec3);
  CHECK_BB(importer_add_fun(importer, ae_flag_static))
  importer_func_begin(importer, "Vec4", "w", (m_uint)coverage_vec4);
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  m_uint* i = malloc(sizeof(m_uint));
  *i = 5;
  importer_add_var(importer, "int", "s_i", ae_flag_static | ae_flag_const, i);
  m_float* f = malloc(sizeof(m_float));
  *f = 2.1;
  importer_add_var(importer, "float", "s_f", ae_flag_static | ae_flag_const, (void*)f);

  m_complex* c = malloc(sizeof(m_complex));
  *c = 2.1;
  importer_add_var(importer, "complex", "s_c", ae_flag_static | ae_flag_const, (void*)c);

  m_vec3* v = malloc(sizeof(m_vec3));
  v->x = 2.1;
  v->y = 2.2;
  v->z = 2.3;
  importer_add_var(importer, "Vec3", "s_v", ae_flag_static | ae_flag_const, (void*)v);

  m_vec4* w = malloc(sizeof(m_vec4));
  w->x = 2.1;
  w->y = 2.2;
  w->z = 2.3;
  w->w = 2.4;
  importer_add_var(importer, "Vec4", "s_w", ae_flag_static | ae_flag_const, (void*)w);

  CHECK_BB(importer_class_end(importer))
  return 1;
}
