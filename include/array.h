#ifndef __ARRAY
#define __ARRAY

struct ArrayAccessInfo {
  struct Array_Sub_ array;
  const Type type;
  const m_bool is_var;
};

typedef struct M_Vector_  * M_Vector;
typedef struct ArrayInfo_ {
  m_int depth;
  struct Vector_   type;
  Type   base;
  M_Object* data;
  union {
    m_int* idx;    // for object array
    size_t length; // array init
  } d;
  uint is_ref;
  uint is_obj;
} ArrayInfo;

ANN m_uint m_vector_size(const M_Vector);
ANN void   m_vector_set(const M_Vector,  const m_uint, const void*);
ANN void   m_vector_get(const M_Vector,  const m_uint, void*);
ANN void   m_vector_add(const M_Vector,  const void*);
ANN m_bit* m_vector_addr(const M_Vector, const m_uint);
ANN void m_vector_rem(const M_Vector,  const m_uint);
ANEW M_Vector new_m_vector(MemPool, const m_uint size, const m_uint len);
ANN void free_m_vector(MemPool, M_Vector);
ANN Type check_array_access(const Env env, const Array_Sub array);
ANN m_bool emit_array_access(const Emitter emit, struct ArrayAccessInfo *const info);
#endif
