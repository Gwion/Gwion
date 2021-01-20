#ifndef __ARRAY
#define __ARRAY

struct ArrayAccessInfo {
  struct Array_Sub_ array;
  const Type type;
  const m_bool is_var;
};

typedef struct M_Vector_  {
  m_bit* ptr;
} * M_Vector;
#define ARRAY_OFFSET SZ_INT * 4
#define ARRAY_PTR(array) (array->ptr + ARRAY_OFFSET)
#define ARRAY_LEN(array) *(m_uint*)(array->ptr)
#define ARRAY_SIZE(array) *(m_uint*)(array->ptr + SZ_INT)
#define ARRAY_CAP(array) *(m_uint*)(array->ptr + SZ_INT*2)

typedef struct ArrayInfo_ {
  m_int depth;
  struct Vector_   type;
  Type   base;
  M_Object* data;
  union {
    m_int* idx;    // for object array
    size_t length; // array init
  } d;
  uint is_obj;
} ArrayInfo;

ANN static inline m_uint m_vector_size(const M_Vector v) {
  return ARRAY_LEN(v);
}

ANN static inline void m_vector_get(const M_Vector v, const m_uint i, void* c) {
  const m_uint size = ARRAY_SIZE(v);
  memcpy(c, ARRAY_PTR(v) + i * size, size);
}

ANN static inline m_bit* m_vector_addr(const M_Vector v, const m_uint i) {
  return &*(m_bit*)(ARRAY_PTR(v) + i * ARRAY_SIZE(v));
}

ANN void   m_vector_set(const M_Vector,  const m_uint, const void*);
ANN void   m_vector_add(const M_Vector,  const void*);
ANN void m_vector_rem(const M_Vector,  const m_uint);
ANEW M_Vector new_m_vector(MemPool, const m_uint size, const m_uint len);
ANN void free_m_vector(MemPool, M_Vector);
ANN Type check_array_access(const Env env, const Array_Sub array);
ANN m_bool emit_array_access(const Emitter emit, struct ArrayAccessInfo *const info);
#endif
