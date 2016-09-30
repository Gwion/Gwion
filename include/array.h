#ifndef __ARRAY
#define __ARRAY
#include "map.h"
typedef struct VM_Vector_* VM_Vector;
typedef struct
{
  m_uint length;
  m_uint depth;
  Type type;
  m_bool is_obj;
  m_bool is_ref;
  m_uint stack_offset;
} VM_Array_Info;

typedef struct
{
  char*  ptr;   // data
  m_uint len;   // number of elements * size
  m_uint size;  // size of objects
} M_Vector;

struct VM_Array_
{
  M_Vector* vector;
//  Map    map;
  Type type;
	m_bool is_obj;
};

m_uint    m_vector_size(M_Vector* v);
m_uint    i_vector_at  (M_Vector* v, m_uint i);
m_float   f_vector_at  (M_Vector* v, m_uint i);
complex   c_vector_at  (M_Vector* v, m_uint i);
VEC3_T   v3_vector_at  (M_Vector* v, m_uint i);
VEC4_T   v4_vector_at  (M_Vector* v, m_uint i);
m_uint*   i_vector_addr(M_Vector* v, m_uint i);
m_float*  f_vector_addr(M_Vector* v, m_uint i);
complex*  c_vector_addr(M_Vector* v, m_uint i);
VEC3_T*  v3_vector_addr(M_Vector* v, m_uint i);
VEC4_T*  v4_vector_addr(M_Vector* v, m_uint i);
void      i_vector_set (M_Vector* v, m_uint i, m_uint  data);
void      f_vector_set (M_Vector* v, m_uint i, m_float data);
void      c_vector_set (M_Vector* v, m_uint i, complex data);
void     v3_vector_set (M_Vector* v, m_uint i, VEC3_T  data);
void     v4_vector_set (M_Vector* v, m_uint i, VEC4_T  data);
M_Object new_M_Array  (m_uint size, m_uint length);
#endif
