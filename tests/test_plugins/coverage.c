#include <stdlib.h>
#include "type.h"
#include "import.h"

SFUN(coverage_int)     { *(m_uint*)RETURN    = 0; }
SFUN(coverage_float)   { *(m_float*)RETURN   = 0; }
SFUN(coverage_complex) { *(m_complex*)RETURN = 0; }
SFUN(coverage_vec3)    { m_vec3 v = {0,0,0};   *(m_vec3*)RETURN = v; }
SFUN(coverage_vec4)    { m_vec4 v = {0,0,0,0}; *(m_vec4*)RETURN = v; }

IMPORT {
  Type t_coverage;
  CHECK_OB((t_coverage = gwi_mk_type(gwi, "Coverage", SZ_INT, t_object)))
  CHECK_BB(gwi_class_ini(gwi, t_coverage, NULL, NULL))
  CHECK_BB(gwi_func_ini(gwi, "int", "i", coverage_int))
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))
  CHECK_BB(gwi_func_ini(gwi, "float", "f", coverage_float))
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))
  CHECK_BB(gwi_func_ini(gwi, "complex", "c", coverage_complex))
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))
  CHECK_BB(gwi_func_ini(gwi, "Vec3", "v", coverage_vec3))
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))
  CHECK_BB(gwi_func_ini(gwi, "Vec4", "w", coverage_vec4))
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  m_uint* i = malloc(sizeof(m_uint));
  *i = 5;
  CHECK_BB(gwi_item_ini(gwi,"int", "s_i"))
  CHECK_BB(gwi_item_end(gwi, ae_flag_static | ae_flag_const, i))
  m_float* f = malloc(sizeof(m_float));
  *f = 2.1;
  CHECK_BB(gwi_item_ini(gwi,"float", "s_f"))
  CHECK_BB(gwi_item_end(gwi, ae_flag_static | ae_flag_const, (void*)f))

  m_complex* c = malloc(sizeof(m_complex));
  *c = 2.1;
  CHECK_BB(gwi_item_ini(gwi,"complex", "s_c"))
  CHECK_BB(gwi_item_end(gwi, ae_flag_static | ae_flag_const, (void*)c))

  m_vec3* v = malloc(sizeof(m_vec3));
  v->x = 2.1;
  v->y = 2.2;
  v->z = 2.3;
  CHECK_BB(gwi_item_ini(gwi,"Vec3", "s_v"))
  CHECK_BB(gwi_item_end(gwi, ae_flag_static | ae_flag_const, (void*)v))

  m_vec4* w = malloc(sizeof(m_vec4));
  w->x = 2.1;
  w->y = 2.2;
  w->z = 2.3;
  w->w = 2.4;
  CHECK_BB(gwi_item_ini(gwi,"Vec4", "s_w"))
  CHECK_BB(gwi_item_end(gwi, ae_flag_static | ae_flag_const, (void*)w))

  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
