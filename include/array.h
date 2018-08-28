typedef struct ArrayInfo_ {
  m_int depth;
  Type   type, base;
  M_Object* data;
  union {
    m_int* idx;    // for object array
    m_uint length; // array init
  } d;
  unsigned is_ref;
  unsigned is_obj;
} ArrayInfo;
ANN void free_array_info(ArrayInfo* info);
ANN m_uint     m_vector_size(const M_Vector v);
ANN void       m_vector_set(const M_Vector v,  const m_uint i, const void* data);
ANN void       m_vector_get(const M_Vector v,  const m_uint i, void* data);
ANN void       m_vector_add(const M_Vector v,  const void* data);
ANN m_bit*      m_vector_addr(const M_Vector v, const m_uint i);
ANN void m_vector_rem(const M_Vector v, const m_uint index);
