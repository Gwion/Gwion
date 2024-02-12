#ifndef __OBJECT
#define __OBJECT
typedef struct M_Object_ *M_Object;
struct M_Object_ {
  Type                  type_ref;
  /*volatile */uint64_t ref;
  m_bit                 data[];
};

ANN void           instantiate_object(const VM_Shred, const Type);
ANN void           free_object(MemPool p, const M_Object);
ANEW M_Object      new_object(MemPool, const Type);
ANEW struct UGen_ *new_UGen(MemPool);
ANEW M_Object      new_M_UGen(const struct Gwion_ *);
ANN void           fork_clean(const VM_Shred, const Vector);
ANN ANEW M_Object  new_array(MemPool, const Type t, const m_uint length);
ANEW M_Object new_string(const struct Gwion_ *, const m_str);
ANEW M_Object new_shred(const VM_Shred);
ANN void      fork_launch(const M_Object);
ANN void      __release(const M_Object, const VM_Shred);
ANN void      broadcast(const M_Object);

#define STRING(o)    (*(m_str *)((M_Object)o)->data)
#define ME(o)        (*(VM_Shred *)((M_Object)o)->data)
#define EV_SHREDS(o) (*(struct Vector_ *)((M_Object)o)->data)
#define UGEN(o)      (*(UGen *)((M_Object)o)->data)
#define ARRAY(o)     (&*(struct M_Vector_ *)((M_Object)o)->data)
#define IO_FILE(o)   (*(FILE **)(((M_Object)o)->data + SZ_INT))

#ifdef USE_HELGRIND
#include "valgrind/helgrind.h"
#endif
static inline void _release(const restrict M_Object obj,
                            const restrict VM_Shred shred) {
  if (!--obj->ref) {
#ifdef USE_HELGRIND
    ANNOTATE_HAPPENS_AFTER(obj);
    ANNOTATE_HAPPENS_BEFORE_FORGET_ALL(obj);
#endif
    __release(obj, shred);
  }
#ifdef USE_HELGRIND
 else {
  ANNOTATE_HAPPENS_BEFORE(obj);
  }
#endif
}
static inline void release(const restrict M_Object obj,
                           const restrict VM_Shred shred) {
  if (obj) _release(obj, shred);
}
typedef void(f_release)(const VM_Shred shred, const Type t NUSED,
                        const m_bit *ptr);
#define RELEASE_FUNC(a)                                                        \
  void(a)(const VM_Shred shred, const Type t NUSED, const m_bit *ptr)
RELEASE_FUNC(struct_release);
RELEASE_FUNC(union_release);
static inline RELEASE_FUNC(object_release) { release(*(M_Object *)ptr, shred); }
static inline RELEASE_FUNC(compound_release) {
  if (!tflag(t, tflag_struct))
    object_release(shred, t, ptr);
  else if (!tflag(t, tflag_union))
    struct_release(shred, t, ptr);
  else union_release(shred, t, ptr);
}
static inline RELEASE_FUNC(anytype_release) {
  if (tflag(t, tflag_release))
    compound_release(shred, t, ptr);
}
static inline void object_addref(const m_bit *ptr) {
  M_Object o = (M_Object)ptr;
  if(o) o->ref++;
}

ANN static inline void union_addref(const Type type, const m_bit*);
ANN static inline void struct_addref(const Type type, const m_bit*);
ANN static inline void compound_addref(const Type t, const m_bit *ptr) {
  if (!tflag(t, tflag_struct))
    object_addref(ptr);
  else if (!tflag(t, tflag_union))
    struct_addref(t, ptr);
  else union_addref(t, ptr);
}

ANN static inline void struct_addref(const Type type, const m_bit *ptr) {
  const Vector v = &type->info->tuple->types;
  for (m_uint i = 0; i < vector_size(v); ++i) {
    const Type t = (Type)vector_at(v, i);
    const m_bit *data = *(m_bit**)(ptr + vector_at(v, i));
    if (tflag(t, tflag_release)) compound_addref(t, data);
  }
}

ANN static inline void union_addref(const Type t, const m_bit *ptr) {
  const m_uint idx = *(m_uint *)ptr;
  if (idx) {
    const Map   map = &t->nspc->info->value->map;
    const Value v   = (Value)map_at(map, idx - 1);
    if (tflag(v->type, tflag_release))
      compound_addref(v->type, ptr + SZ_INT);
  }
}

ANN static inline void anytype_addref(const Type t, const m_bit *ptr) {
  if(tflag(t, tflag_release)) compound_addref(t, ptr);
}

ANN bool not_from_owner_class(const Env, const Type, const Value, const loc_t);
#endif
