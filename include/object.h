#ifndef __OBJECT
#define __OBJECT
typedef struct M_Object_ *M_Object;
struct M_Object_ {
  m_bit *         data;
  Type            type_ref;
  struct Vector_  vtable;
  volatile size_t ref;
};

ANN void           instantiate_object(const VM_Shred, const Type);
ANN void           free_object(MemPool p, const M_Object);
ANEW M_Object      new_object(MemPool, const VM_Shred, const Type);
ANEW struct UGen_ *new_UGen(MemPool);
ANEW M_Object      new_M_UGen(const struct Gwion_ *);
ANN void           fork_clean(const VM_Shred, const Vector);
ANN ANEW M_Object  new_array(MemPool, const Type t, const m_uint length);
ANEW M_Object      new_string(MemPool, const VM_Shred, const m_str);
ANEW M_Object new_string2(const struct Gwion_ *, const VM_Shred, const m_str);
ANEW M_Object new_shred(const VM_Shred);
ANN void      fork_launch(const M_Object, const m_uint);
ANN void      __release(const M_Object, const VM_Shred);
ANN void      broadcast(const M_Object);

#define STRING(o)    (*(m_str *)((M_Object)o)->data)
#define ME(o)        (*(VM_Shred *)((M_Object)o)->data)
#define EV_SHREDS(o) (*(struct Vector_ *)((M_Object)o)->data)
#define UGEN(o)      (*(UGen *)((M_Object)o)->data)
#define ARRAY(o)     (*(M_Vector *)((M_Object)o)->data)
#define IO_FILE(o)   (*(FILE **)(((M_Object)o)->data + SZ_INT))

static inline void _release(const restrict M_Object obj,
                            const restrict VM_Shred shred) {
  if (!--obj->ref) __release(obj, shred);
}
static inline void release(const restrict M_Object obj,
                           const restrict VM_Shred shred) {
  if (obj) _release(obj, shred);
}
typedef void(f_release)(const VM_Shred shred, const Type t NUSED,
                        const m_bit *ptr);
#define RELEASE_FUNC(a)                                                        \
  void(a)(const VM_Shred shred, const Type t NUSED, const m_bit *ptr)
static inline RELEASE_FUNC(object_release) { release(*(M_Object *)ptr, shred); }
RELEASE_FUNC(struct_release);

static inline void struct_addref(const Gwion gwion, const Type type,
                                 const m_bit *ptr) {
  for (m_uint i = 0; i < vector_size(&type->info->tuple->types); ++i) {
    const Type t = (Type)vector_at(&type->info->tuple->types, i);
    if (isa(t, gwion->type[et_object]) > 0) {
      const M_Object o =
          *(M_Object *)(ptr + vector_at(&type->info->tuple->offset, i));
      ++o->ref;
    } else if (tflag(t, tflag_struct)) {
      struct_addref(
          gwion, t,
          *(m_bit **)(ptr + vector_at(&type->info->tuple->offset, i)));
    }
  }
}

static inline void compound_release(const VM_Shred shred, const Type t,
                                    const m_bit *ptr) {
  if (!tflag(t, tflag_struct))
    object_release(shred, t, ptr);
  else
    struct_release(shred, t, ptr);
}

#endif
