typedef struct {
  Type   type, base;
  m_uint length;
  m_uint depth;
  m_bool is_obj;
  m_bool is_ref;
} VM_Array_Info;

m_uint     m_vector_size(M_Vector v);
void       m_vector_set(M_Vector v,  const m_uint i, const void* data);
void       m_vector_get(M_Vector v,  const m_uint i, void* data);
void       m_vector_add(M_Vector v,  const void* data);
char*      m_vector_addr(M_Vector v, const m_uint i);
void m_vector_rem(M_Vector v, const m_uint index);
