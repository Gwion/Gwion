struct M_Object_ {
  Vector vtable; // pointer reference to type->info->obj_v_table
  Type type_ref;
  m_uint ref;
  unsigned char* data;
};

m_bool initialize_object(M_Object o, const Type type);
void instantiate_object(const VM*, const VM_Shred, const Type);
void free_object(M_Object);
ANEW M_Object new_M_Object(const VM_Shred shred);
ANEW M_Object new_M_UGen();
ANEW M_Object new_M_Array(Type t, m_uint size, m_uint length, m_uint depth);
ANEW M_Object new_String(const VM_Shred shred, const m_str str);

void release(M_Object obj, const VM_Shred shred);
void NullException(const VM_Shred shred, const m_str c);

m_int o_object_ugen;
m_int o_object_array;


#define STRING(o) *((m_str*)((M_Object)o)->data + o_string_data)
#define ME(o) *((VM_Shred*)((M_Object)o)->data + o_shred_me)
#define EV_SHREDS(o) *((Vector*)((M_Object)o)->data + o_event_shred)
#define IO_FILE(o)  *(FILE**)(((M_Object)o)->data + o_fileio_file)
#define UGEN(o) (*(UGen*)(((M_Object)o)->data + o_object_ugen))
#define ARRAY(o) (*(M_Vector*)(((M_Object)o)->data + o_object_array))
#define Except(s, c) { NullException(s, c); return; }
