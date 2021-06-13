#ifndef __ARRAY
#define __ARRAY

struct ArrayAccessInfo {
  struct Array_Sub_ array;
  const Type        type;
  const bool        is_var;
};

typedef struct ArrayInfo_ {
  m_int          depth;
  struct Vector_ type;
  Type           base;
  M_Object *     data;
  union {
    m_int *idx;    // for object array
    size_t length; // array init
  } d;
  bool is_obj;
} ArrayInfo;

ANN Type   check_array_access(const Env env, const Array_Sub array);
ANN m_bool emit_array_access(const Emitter                 emit,
                             struct ArrayAccessInfo *const info);
#endif
