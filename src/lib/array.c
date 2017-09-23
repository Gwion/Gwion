#include <math.h>
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"

struct Type_ t_array  = { "@Array", SZ_INT, &t_object, te_array };

m_int o_object_array;

struct M_Vector_ {
  char*  ptr;   // data
  m_uint len;   // number of elements * size
  m_uint size;  // size of objects
  m_uint depth;
  m_uint cap;
};

DTOR(array_dtor) {
  if(o->type_ref->d.array_type) {// maybe unnecessary. preferably check array depth
    if(ARRAY(o)->depth > 1 || isa(o->type_ref->d.array_type, &t_object) > 0) {
      m_uint i;
      for(i = 0; i < ARRAY(o)->len * SZ_INT; i += SZ_INT)
        release(*(M_Object*)(ARRAY(o)->ptr + i), shred);
    }
  }
  free(ARRAY(o)->ptr);
  free(ARRAY(o));
  REM_REF(o->type_ref);
}

M_Object new_M_Array(m_uint size, m_uint length, m_uint depth) {
  m_uint cap = 1;
  M_Object a = new_M_Object(NULL);
  initialize_object(a, &t_array);
  while(cap < length)
    cap *= 2;
  ARRAY(a)  	  = malloc(sizeof(struct M_Vector_));
  ARRAY(a)->ptr   = calloc(cap, size);
  ARRAY(a)->cap   = cap;
  ARRAY(a)->size  = size;
  ARRAY(a)->len   = length;
  ARRAY(a)->depth = depth;
  return a;
}

m_uint m_vector_size(M_Vector v) {
  return v->len;
}

static inline m_uint m_vector_depth(M_Vector v) {
  return v->depth;
}

static inline m_uint m_vector_cap(M_Vector v) {
  return v->cap;
}

void m_vector_get(M_Vector v, m_uint i, char* c) {
  memcpy(c, v->ptr + i * v->size, v->size);
}

m_uint  i_vector_at(M_Vector v, m_uint i) {
  return *(m_uint*)(v->ptr + i * v->size);
}

m_float f_vector_at(M_Vector v, m_uint i) {
  return *(m_float*)(v->ptr + i * v->size);
}

m_complex c_vector_at(M_Vector v, m_uint i) {
  return *(m_complex*)(v->ptr + i * v->size);
}

m_vec3 v3_vector_at(M_Vector v, m_uint i) {
  return *(m_vec3*)(v->ptr + i * v->size);
}

m_vec4 v4_vector_at(M_Vector v, m_uint i) {
  return *(m_vec4*)(v->ptr + i * v->size);
}

#define CHECK_VEC_SIZE(v)   if(++v->len >= v->cap) { \
    v->cap *=2;                                   \
    v->ptr = realloc(v->ptr, v->cap * v->size);   \
  }                                               \

void i_vector_add(M_Vector v, m_uint i) {
  CHECK_VEC_SIZE(v)
  *(m_uint*)(v->ptr + (v->len - 1)*v->size) = i;
}

void f_vector_add(M_Vector v, m_float f) {
  CHECK_VEC_SIZE(v)
  *(m_float*)(v->ptr + (v->len - 1)*v->size) = f;
}

void c_vector_add(M_Vector v, m_complex c) {
  CHECK_VEC_SIZE(v)
  *(m_complex*)(v->ptr + (v->len - 1)*v->size) = c;
}

void v3_vector_add(M_Vector v, m_vec3 c) {
  CHECK_VEC_SIZE(v)
  *(m_vec3*)(v->ptr + (v->len - 1)*v->size) = c;
}

void v4_vector_add(M_Vector v, m_vec4 c) {
  CHECK_VEC_SIZE(v)
  *(m_vec4*)(v->ptr + (v->len - 1)*v->size) = c;
}

void m_vector_set(M_Vector v, m_uint i, char* data) {
  memcpy(v->ptr + i * v->size, data,  v->size);
}

void i_vector_set(M_Vector v, m_uint i, m_uint data) {
  *(m_uint*)(v->ptr + i * v->size) = data;
}

void f_vector_set(M_Vector v, m_uint i, m_float data) {
  *(m_float*)(v->ptr + i * v->size) = data;
}

void c_vector_set(M_Vector v, m_uint i, m_complex data) {
  *(m_complex*)(v->ptr + i * v->size) = data;
}

void v3_vector_set(M_Vector v, m_uint i, m_vec3 data) {
  *(m_vec3*)(v->ptr + i * v->size) = data;
}

void v4_vector_set(M_Vector v, m_uint i, m_vec4 data) {
  *(m_vec4*)(v->ptr + i * v->size) = data;
}

void m_vector_rem(M_Vector v, m_uint index) {
  char c[--v->len * v->size];
  if(index)
    memcpy(c, v->ptr, index * v->size);
  ++index;
  memcpy(c + (index - 1) * v->size, v->ptr + index * v->size, (v->cap - index)*v->size);
  if(v->len > 2 && v->len < v->cap / 2) {
    v->cap /= 2;
    v->ptr = realloc(v->ptr, v->cap * v->size);
  }
  memcpy(v->ptr, c, v->cap * v->size);
}

MFUN(vm_vector_rem) {
  m_int index = *(m_int*)(shred + SZ_INT);
  M_Vector v = ARRAY(o);
  if(index < 0 || index >= v->len)
    return;
  m_vector_rem(v, index);
}

char* m_vector_addr(M_Vector v, m_uint i) {
  return &*(char*)(v->ptr + i * v->size);
}

m_uint*  i_vector_addr(M_Vector v, m_uint i) {
  return &*(m_uint*)(v->ptr + i * v->size);
}

m_float*  f_vector_addr(M_Vector v, m_uint i) {
  return &*(m_float*)(v->ptr + i * v->size);
}

m_complex*  c_vector_addr(M_Vector v, m_uint i) {
  return &*(m_complex*)(v->ptr + i * v->size);
}

m_vec3*  v3_vector_addr(M_Vector v, m_uint i) {
  return &*(m_vec3*)(v->ptr + i * v->size);
}

m_vec4*  v4_vector_addr(M_Vector v, m_uint i) {
  return &*(m_vec4*)(v->ptr + i * v->size);
}

MFUN(vm_vector_size) {
  *(m_uint*)RETURN = o ? m_vector_size(ARRAY(o)) : - 1;
}

MFUN(vm_vector_depth) {
  *(m_uint*)RETURN = o ? m_vector_depth(ARRAY(o)) : - 1;
}

MFUN(vm_vector_cap) {
  *(m_uint*)RETURN = o ? m_vector_cap(ARRAY(o)) : - 1;
}

INSTR(Array_Append) {
  POP_REG(shred, SZ_INT);
  M_Object o = NULL;
  if(instr->m_val == Kindof_Int) {
    POP_REG(shred, SZ_INT);
    o = *(M_Object*)REG(0);
    i_vector_add(ARRAY(o), *(m_uint*)REG(SZ_INT));
  } else if(instr->m_val == Kindof_Float) {
    POP_REG(shred, SZ_FLOAT);
    o = *(M_Object*)REG(0);
    f_vector_add(ARRAY(o), *(m_float*)REG(SZ_INT));
  } else if(instr->m_val == Kindof_Complex) {
    POP_REG(shred, SZ_COMPLEX);
    o = *(M_Object*)REG(0);
    c_vector_add(ARRAY(o), *(m_complex*)REG(SZ_INT));
  } else if(instr->m_val == Kindof_Vec3) {
    POP_REG(shred, SZ_VEC3);
    o = *(M_Object*)REG(0);
    v3_vector_add(ARRAY(o), *(m_vec3*)REG(SZ_INT));
  } else if(instr->m_val == Kindof_Vec4) {
    POP_REG(shred, SZ_VEC4);
    o = *(M_Object*)REG(0);
    v4_vector_add(ARRAY(o), *(m_vec4*)REG(SZ_INT));
  }
  release(o, shred);
  *(M_Object*)REG(0) = o;
  PUSH_REG(shred, SZ_INT);
}

m_bool import_array(Importer importer) {
  CHECK_BB(importer_class_begin(importer,  &t_array, NULL, array_dtor))

  o_object_array = importer_add_var(importer,  "int", "@array", ae_flag_member, NULL);
  CHECK_BB(o_object_array)

  importer_func_begin(importer, "int", "size", (m_uint)vm_vector_size);
  CHECK_BB(importer_add_fun(importer, 0))

  importer_func_begin(importer, "int", "depth", (m_uint)vm_vector_depth);
  CHECK_BB(importer_add_fun(importer, 0))

  importer_func_begin(importer, "int", "cap", (m_uint)vm_vector_cap);
  CHECK_BB(importer_add_fun(importer, 0))

  importer_func_begin(importer, "int", "remove", (m_uint)vm_vector_rem);
  importer_add_arg(importer, "int", "index");
  CHECK_BB(importer_add_fun(importer, 0))

  CHECK_BB(importer_class_end(importer))
  return 1;
}
