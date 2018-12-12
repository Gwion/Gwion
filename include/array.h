#ifndef __ARRAY
#define __ARRAY
typedef struct M_Vector_  * M_Vector;
typedef struct ArrayInfo_ {
  m_int depth;
//  Type   type, base;
  struct Vector_   type;
  Type   base;
  M_Object* data;
  union {
    m_int* idx;    // for object array
    size_t length; // array init
  } d;
  unsigned is_ref : 1;
  unsigned is_obj : 1;
} ArrayInfo;

ANN m_uint     m_vector_size(const M_Vector v);
ANN void       m_vector_set(const M_Vector v,  const m_uint i, const void* data);
ANN void       m_vector_get(const M_Vector v,  const m_uint i, void* data);
ANN void       m_vector_add(const M_Vector v,  const void* data);
ANN m_bit*      m_vector_addr(const M_Vector v, const m_uint i);
ANN void m_vector_rem(const M_Vector v, const m_uint index);
ANEW M_Vector new_m_vector(const m_uint);
void free_m_vector(M_Vector);
#endif
