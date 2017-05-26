struct M_Object_ {
  Vector vtable;
  Type type_ref;
  m_uint size, ref;
  union {
    M_Vector array;
    unsigned char* data;
  } d;
  UGen ugen;
};

m_bool import_object(Env env);
m_bool initialize_object(M_Object o, Type type);
M_Object new_M_Object(VM_Shred shred);
M_Object new_M_UGen();
M_Object new_M_Array  (m_uint size, m_uint length, m_uint depth);
M_Object new_String(VM_Shred shred, m_str str);

void release(M_Object obj, VM_Shred shred);
void NullException(VM_Shred shred, const m_str c);
#define Except(s, c) { NullException(s, c);return; }
