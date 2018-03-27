typedef struct VM_Array_Info_ {
  Type   type, base;
  m_uint length;
  m_uint depth;
  m_bool is_obj;
  m_bool is_ref;
} VM_Array_Info;
ANN void free_array_info(VM_Array_Info* info);
ANN m_uint     m_vector_size(const M_Vector v);
ANN void       m_vector_set(const M_Vector v,  const m_uint i, const void* data);
ANN void       m_vector_get(const M_Vector v,  const m_uint i, void* data);
ANN void       m_vector_add(const M_Vector v,  const void* data);
ANN char*      m_vector_addr(const M_Vector v, const m_uint i);
ANN void m_vector_rem(const M_Vector v, const m_uint index);
