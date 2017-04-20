void object_dtor(M_Object o, VM_Shred shred);
m_bool import_object(Env env);
m_bool initialize_object(M_Object o, Type type);
M_Object new_M_UGen();
M_Object new_M_Object(VM_Shred shred);

M_Object new_String(VM_Shred shred, m_str str);
void release(M_Object obj, VM_Shred shred);
void NullException(VM_Shred shred);
#define Except(s) { NullException(s);return; }
