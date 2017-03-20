#ifndef __ARRAY
#define __ARRAY
#include <complex.h>
#include "map.h"

typedef struct VM_Vector_* VM_Vector;
typedef struct {
  m_uint length;
  m_uint depth;
  Type type;
  m_bool is_obj;
  m_bool is_ref;
  m_uint stack_offset;
} VM_Array_Info;

typedef struct {
  char*  ptr;   // data
  m_uint len;   // number of elements * size
  m_uint size;  // size of objects
  m_uint depth;
} M_Vector;

struct VM_Array_ {
  M_Vector* vector;
//  Map    map;
  Type type;
  m_bool is_obj;
};

m_uint    m_vector_size(M_Vector* v);
m_uint    i_vector_at  (M_Vector* v, m_uint i);
m_float   f_vector_at  (M_Vector* v, m_uint i);
#ifdef USE_DOUBLE
complex double  c_vector_at  (M_Vector* v, m_uint i);
#else
complex float  c_vector_at  (M_Vector* v, m_uint i);
#endif
VEC3_T   v3_vector_at  (M_Vector* v, m_uint i);
VEC4_T   v4_vector_at  (M_Vector* v, m_uint i);
m_uint*   i_vector_addr(M_Vector* v, m_uint i);
m_float*  f_vector_addr(M_Vector* v, m_uint i);
#ifdef USE_DOUBLE
complex double*  c_vector_addr(M_Vector* v, m_uint i);
#else
complex float*  c_vector_addr(M_Vector* v, m_uint i);
#endif
VEC3_T*  v3_vector_addr(M_Vector* v, m_uint i);
VEC4_T*  v4_vector_addr(M_Vector* v, m_uint i);
//M_Object  o_vector_at((M_Vector* v, m_uint i); // check me
void      i_vector_set (M_Vector* v, m_uint i, m_uint  data);
void      f_vector_set (M_Vector* v, m_uint i, m_float data);
/*
#ifdef USE_DOUBLE
void      c_vector_set (M_Vector* v, m_uint i, complex double data);
#else
void      c_vector_set (M_Vector* v, m_uint i, complex float data);
#endif
void     v3_vector_set (M_Vector* v, m_uint i, VEC3_T  data);
void     v4_vector_set (M_Vector* v, m_uint i, VEC4_T  data);
*/
M_Object new_M_Array  (m_uint size, m_uint length, m_uint depth);
#endif
