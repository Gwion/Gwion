struct M_Object_ {
  m_bit* data;
  m_uint ref;
  Type type_ref;
  struct pool* p;
};

ANN void instantiate_object(const VM_Shred, const Type);
ANN void free_object(const M_Object);
ANEW M_Object new_object(const VM_Shred, const Type);
ANN ANEW M_Object new_array(const Type t, const m_uint size,
  const m_uint length, const m_uint depth);
ANEW M_Object new_string(const VM_Shred, const m_str);
ANEW M_Object new_string2(const VM_Shred, const m_str);
ANN void __release(const M_Object, const VM_Shred);
ANN void exception(const VM_Shred, const m_str);

#define STRING(o)    (*(m_str*)    ((M_Object)o)->data)
#define ME(o)        (*(VM_Shred*) ((M_Object)o)->data)
#define EV_SHREDS(o) (*(Vector*)   ((M_Object)o)->data)
#define UGEN(o)      (*(UGen*)     ((M_Object)o)->data)
#define ARRAY(o)     (*(M_Vector*) ((M_Object)o)->data)
#define IO_FILE(o)   (*(FILE**)    (((M_Object)o)->data + SZ_INT))
#define Except(s, c) { exception(s, c); return; }
#define _release(a,b) if(!--(a)->ref)__release((a), (b))
#define release(a,b) if((a))_release((a), (b))
