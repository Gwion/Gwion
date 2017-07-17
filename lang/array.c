#include "defs.h"
#include "err_msg.h"
#include "import.h"

struct M_Vector_ {
  char*  ptr;   // data
  m_uint len;   // number of elements * size
  m_uint size;  // size of objects
  m_uint depth;
  m_uint cap;
};


struct Type_ t_array  = { "@Array", SZ_INT, &t_object, te_array };

m_int o_object_array;

DTOR(array_dtor) {
  if(o->type_ref->d.array_type) {// maybe unnecessary. preferably check array depth
    if(ARRAY(o)->depth > 1 || isa(o->type_ref->d.array_type, &t_object) > 0) {
      m_uint i;
      for(i = 0; i < ARRAY(o)->len * SZ_INT; i += SZ_INT)
        release(*(M_Object*)(ARRAY(o)->ptr + i), shred);
    }
  }
  free(ARRAY(o)->ptr);
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
/*
void m_vector_insert(M_Vector v, m_uint index, char* data)
{
  char c[++v->len*v->size];
  if(index)
    memcpy(c, v->ptr, index*v->size);
  memcpy(c+(index*v->size), data, v->size);
  ++index;
  if(v->len > 2 && v->len < v->cap/2)
    v->cap /= 2;
  memcpy(c + (index) * v->size, v->ptr + index * v->size, (v->cap - index)*v->size);
  if(v->len > 2 && v->len < v->cap/2)
    v->ptr = realloc(v->ptr, v->cap*v->size);
  memcpy(v->ptr, c, v->cap * v->size);
}
*/

MFUN(vm_vector_rem) {
  m_int index = *(m_int*)(shred + SZ_INT);
  M_Vector v = ARRAY(o);
  if(index < 0 || index >= v->len)
    return;
  m_vector_rem(v, index);
}
/*
MFUN(vm_vector_insert_i) {
  m_int index = *(m_int*)(shred + SZ_INT);
  m_int data  = *(m_int*)(shred + SZ_INT*2);
  M_Vector v = o->d.array;
  if(index < 0 || index >= v->len)
	return;
  m_vector_rem(v, index);
}

MFUN(vm_vector_insert_f) {
  m_int index = *(m_int*)(shred + SZ_INT);
  m_float data  = *(m_float*)(shred + SZ_INT*2);
  M_Vector v = o->d.array;
  if(index < 0 || index >= v->len)
	return;
  m_vector_rem(v, index);
}
*/
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
  RETURN->d.v_uint = o ? m_vector_size(ARRAY(o)) : - 1;
}

MFUN(vm_vector_depth) {
  RETURN->d.v_uint = o ? m_vector_depth(ARRAY(o)) : - 1;
}

MFUN(vm_vector_cap) {
  RETURN->d.v_uint = o ? m_vector_cap(ARRAY(o)) : - 1;
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

m_bool import_array(Env env) {
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_array, env->global_nspc, NULL, array_dtor))

  o_object_array = import_var(env, "int", "@array", ae_flag_member, NULL);
  CHECK_BB(o_object_array)

  dl_func_init(&fun, "int", "size", (m_uint)vm_vector_size);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "depth", (m_uint)vm_vector_depth);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "cap", (m_uint)vm_vector_cap);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "remove", (m_uint)vm_vector_rem);
  dl_func_add_arg(&fun, "int", "index");
  CHECK_BB(import_fun(env, &fun, 0))

  CHECK_BB(import_class_end(env))
  return 1;
}
