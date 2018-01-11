typedef struct {
  Type   type;
  m_uint length;
  m_uint depth;
  m_uint size;
  m_bool is_obj;
  m_bool is_ref;
} VM_Array_Info;

m_uint     m_vector_size(M_Vector v);
void       m_vector_set(M_Vector v, m_uint i, char* data);
void       m_vector_get(M_Vector v, m_uint i, char* data);
char*      m_vector_addr(M_Vector v, m_uint i);

m_uint     i_vector_at(M_Vector v, m_uint i);
m_uint*    i_vector_addr(M_Vector v, m_uint i);
void       i_vector_set(M_Vector v, m_uint i, m_uint  data);

m_float    f_vector_at(M_Vector v, m_uint i);
m_float*   f_vector_addr(M_Vector v, m_uint i);
void       f_vector_set(M_Vector v, m_uint i, m_float data);

m_complex  c_vector_at(M_Vector v, m_uint i);
m_complex* c_vector_addr(M_Vector v, m_uint i);
void       c_vector_set(M_Vector v, m_uint i, m_complex data);

m_vec3     v3_vector_at(M_Vector v, m_uint i);
m_vec3*    v3_vector_addr(M_Vector v, m_uint i);
void       v3_vector_set(M_Vector v, m_uint i, m_vec3  data);

m_vec4     v4_vector_at(M_Vector v, m_uint i);
m_vec4*    v4_vector_addr(M_Vector v, m_uint i);
void       v4_vector_set(M_Vector v, m_uint i, m_vec4  data);
