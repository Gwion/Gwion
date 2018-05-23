struct M_Object_ {
  m_bit* data;
  Type type_ref;
  m_uint ref;
};

ANN void initialize_object(const M_Object, const Type);
ANN void instantiate_object(const VM_Shred, const Type);
ANN void free_object(const M_Object);
ANEW M_Object new_M_Object(const VM_Shred);
ANN ANEW M_Object new_M_Array(const Type t, const m_uint size,
  const m_uint length, const m_uint depth);
ANEW M_Object new_String(const VM_Shred, const m_str);
ANN void _release(const M_Object, const VM_Shred);
ANN void exception(const VM_Shred, const m_str);

#define STRING(o)    (*(m_str*)    ((M_Object)o)->data)
#define ME(o)        (*(VM_Shred*) ((M_Object)o)->data)
#define EV_SHREDS(o) (*(Vector*)   ((M_Object)o)->data)
#define UGEN(o)      (*(UGen*)     ((M_Object)o)->data)
#define ARRAY(o)     (*(M_Vector*) ((M_Object)o)->data)
#define IO_FILE(o)   (*(FILE**)    (((M_Object)o)->data + SZ_INT))
#define Except(s, c) { exception(s, c); return; }
#define release(a,b) if(a)_release((a), (b))
