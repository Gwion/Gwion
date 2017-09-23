struct M_Object_ {
  Vector vtable; // pointer reference to type->info->obj_v_table
  Type type_ref;
  m_uint ref;
  unsigned char* data;
};

//m_bool import_object(Importer import);
m_bool initialize_object(M_Object o, Type type);
M_Object new_M_Object(VM_Shred shred);
M_Object new_M_UGen();
M_Object new_M_Array(m_uint size, m_uint length, m_uint depth);
M_Object new_String(VM_Shred shred, m_str str);

void release(M_Object obj, VM_Shred shred);
void NullException(VM_Shred shred, const m_str c);

m_int o_object_ugen;
m_int o_object_array;


#define STRING(o) *((m_str*)((M_Object)o)->data + o_string_data)
#define ME(o) *((VM_Shred*)((M_Object)o)->data + o_shred_me)
#define EV_SHREDS(o) *((Vector*)((M_Object)o)->data + o_event_shred)
#define IO_FILE(o)  *(FILE**)(((M_Object)o)->data + o_fileio_file)
#define UGEN(o) (*(UGen*)(((M_Object)o)->data + o_object_ugen))
#define ARRAY(o) (*(M_Vector*)(((M_Object)o)->data + o_object_array))
#define Except(s, c) { NullException(s, c); return; }
