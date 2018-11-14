#ifndef __OBJECT
#define __OBJECT
typedef struct M_Object_  * M_Object;
struct M_Object_ {
  m_bit* data;
  m_uint ref;
  Type type_ref;
  struct pool* p;
};

ANN void instantiate_object(const VM_Shred, const Type);
ANN void free_object(const M_Object);
ANEW M_Object new_object(const VM_Shred, const Type);
ANEW M_Object new_M_UGen(void);
ANN ANEW M_Object new_array(const Type t, const m_uint length);
ANEW M_Object new_string(const VM_Shred, const m_str);
ANEW M_Object new_string2(const VM_Shred, const m_str);
ANEW M_Object new_shred(const VM_Shred);
ANN void __release(const M_Object, const VM_Shred);
ANN void exception(const VM_Shred, const m_str);

#define STRING(o)    (*(m_str*)    ((M_Object)o)->data)
#define ME(o)        (*(VM_Shred*) ((M_Object)o)->data)
#define EV_SHREDS(o) (*(Vector*)   ((M_Object)o)->data)
#define UGEN(o)      (*(UGen*)     ((M_Object)o)->data)
#define ARRAY(o)     (*(M_Vector*) ((M_Object)o)->data)
#define IO_FILE(o)   (*(FILE**)    (((M_Object)o)->data + SZ_INT))
#define Except(s, c) { exception(s, c); return; }
static inline void _release(const restrict M_Object obj, const restrict VM_Shred shred) {
  if(!--obj->ref)__release(obj, shred);
}
static inline void release(const restrict M_Object obj, const restrict VM_Shred shred) {
  if(obj)_release(obj, shred);
}
#endif
