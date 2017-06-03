#ifndef __ARRAY
#define __ARRAY
#include <complex.h>
#include "map.h"

typedef struct {
  m_uint length;
  m_uint depth;
  Type type;
  m_uint stack_offset;
  m_bool is_obj;
  m_bool is_ref;
} VM_Array_Info;

m_uint   m_vector_size(M_Vector v);

m_uint   i_vector_at(M_Vector v, m_uint i);
m_uint*  i_vector_addr(M_Vector v, m_uint i);
void     i_vector_set(M_Vector v, m_uint i, m_uint  data);

m_float  f_vector_at(M_Vector v, m_uint i);
m_float* f_vector_addr(M_Vector v, m_uint i);
void     f_vector_set(M_Vector v, m_uint i, m_float data);

#ifdef USE_DOUBLE
complex double  c_vector_at(M_Vector v, m_uint i);
complex double* c_vector_addr(M_Vector v, m_uint i);
void c_vector_set(M_Vector v, m_uint i, complex double data);
#else
complex float  c_vector_at(M_Vector v, m_uint i);
complex float*  c_vector_addr(M_Vector v, m_uint i);
void      c_vector_set(M_Vector v, m_uint i, complex float data);
#endif

m_vec3   v3_vector_at(M_Vector v, m_uint i);
m_vec3*  v3_vector_addr(M_Vector v, m_uint i);
void     v3_vector_set(M_Vector v, m_uint i, m_vec3  data);

m_vec4   v4_vector_at(M_Vector v, m_uint i);
m_vec4*  v4_vector_addr(M_Vector v, m_uint i);
void     v4_vector_set(M_Vector v, m_uint i, m_vec4  data);
#endif
