#include "defs.h"
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "lang.h"
#include "import.h"

struct M_Vector_ {
  char*  ptr;   // data
  m_uint len;   // number of elements * size
  m_uint size;  // size of objects
  m_uint depth;
  m_uint cap;
};


struct Type_ t_array  = { "@Array", SZ_INT, &t_object, te_array };

m_int o_array_vector;

DTOR(array_dtor) {
  if(o->type_ref->array_type) // maybe unnecessary. preferably check array depth
    if(o->d.array->depth > 1 || isa(o->type_ref->array_type, &t_object) > 0) {
      m_uint i;
      for(i = 0; i < o->d.array->len; i += SZ_INT)
        release(*(M_Object*)(o->d.array->ptr + i), shred);
    }
  free(o->d.array->ptr);
  REM_REF(o->type_ref);
}

M_Object new_M_Array(m_uint size, m_uint length, m_uint depth) {
  m_uint cap = 1;
  M_Object a = new_M_Object(NULL);
  initialize_object(a, &t_array);
  while(cap < length)
    cap *= 2;
  a->d.array 	    = malloc(sizeof(struct M_Vector_));
  a->d.array->ptr   = calloc(cap, size);
  a->d.array->cap   = cap;
  a->d.array->size  = size;
  a->d.array->len   = length;
  a->d.array->depth = depth;
  return a;
}

m_uint m_vector_size(M_Vector v) {
  return v->len;
}

m_uint m_vector_depth(M_Vector v) {
  return v->depth;
}

m_uint m_vector_cap(M_Vector v) {
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

VEC3_T v3_vector_at(M_Vector v, m_uint i) {
  return *(VEC3_T*)(v->ptr + i * v->size);
}

VEC4_T v4_vector_at(M_Vector v, m_uint i) {
  return *(VEC4_T*)(v->ptr + i * v->size);
}

#define CHECK_VEC_SIZE(v)   if(++v->len >= v->cap) { \
    v->cap *=2;                                   \
    v->ptr = realloc(v->ptr, v->cap * v->size);   \
  }                                               \

void i_vector_append(M_Vector v, m_uint i) {
  CHECK_VEC_SIZE(v)
  *(m_uint*)(v->ptr + (v->len - 1)*v->size) = i;
}

void f_vector_append(M_Vector v, m_float f) {
  CHECK_VEC_SIZE(v)
  *(m_float*)(v->ptr + (v->len - 1)*v->size) = f;
}

void c_vector_append(M_Vector v, m_complex c) {
  CHECK_VEC_SIZE(v)
  *(m_complex*)(v->ptr + (v->len - 1)*v->size) = c;
}

void v3_vector_append(M_Vector v, VEC3_T c) {
  CHECK_VEC_SIZE(v)
  *(VEC3_T*)(v->ptr + (v->len - 1)*v->size) = c;
}

void v4_vector_append(M_Vector v, VEC4_T c) {
  CHECK_VEC_SIZE(v)
  *(VEC4_T*)(v->ptr + (v->len - 1)*v->size) = c;
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

void v3_vector_set(M_Vector v, m_uint i, VEC3_T data) {
  *(VEC3_T*)(v->ptr + i * v->size) = data;
}

void v4_vector_set(M_Vector v, m_uint i, VEC4_T data) {
  *(VEC4_T*)(v->ptr + i * v->size) = data;
}

void m_vector_remove(M_Vector v, m_uint index) {
  char c[--v->len*v->size];
  if(index)
    memcpy(c, v->ptr, index*v->size);
  ++index;
  memcpy(c + (index-1) * v->size, v->ptr + index * v->size, (v->cap - index)*v->size);
  if(v->len > 2 && v->len < v->cap/2) {
    v->cap /= 2;
    v->ptr = realloc(v->ptr, v->cap*v->size);
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

MFUN(vm_vector_remove) {
  m_int index = *(m_int*)(shred + SZ_INT);
  M_Vector v = o->d.array;
  if(index < 0 || index >= v->len)
    return;
  m_vector_remove(v, index);
}
/*
MFUN(vm_vector_insert_i) {
  m_int index = *(m_int*)(shred + SZ_INT);
  m_int data  = *(m_int*)(shred + SZ_INT*2);
  M_Vector v = o->d.array;
  if(index < 0 || index >= v->len)
	return;
  m_vector_remove(v, index);
}

MFUN(vm_vector_insert_f) {
  m_int index = *(m_int*)(shred + SZ_INT);
  m_float data  = *(m_float*)(shred + SZ_INT*2);
  M_Vector v = o->d.array;
  if(index < 0 || index >= v->len)
	return;
  m_vector_remove(v, index);
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

VEC3_T*  v3_vector_addr(M_Vector v, m_uint i) {
  return &*(VEC3_T*)(v->ptr + i * v->size);
}

VEC4_T*  v4_vector_addr(M_Vector v, m_uint i) {
  return &*(VEC4_T*)(v->ptr + i * v->size);
}

MFUN(vm_vector_size) {
  RETURN->d.v_uint = o ? m_vector_size(o->d.array) : - 1;
}

MFUN(vm_vector_depth) {
  RETURN->d.v_uint = o ? m_vector_depth(o->d.array) : - 1;
}

MFUN(vm_vector_cap) {
  RETURN->d.v_uint = o ? m_vector_cap(o->d.array) : - 1;
}

INSTR(Array_Append) {
  POP_REG(shred, SZ_INT);
  M_Object o = NULL;
  if(instr->m_val == Kindof_Int) {
    POP_REG(shred, SZ_INT);
    o = *(M_Object*)(shred->reg);
    i_vector_append(o->d.array, *(m_uint*)(shred->reg + SZ_INT));
  } else if(instr->m_val == Kindof_Float) {
    POP_REG(shred, SZ_FLOAT);
    o = *(M_Object*)(shred->reg);
    f_vector_append(o->d.array, *(m_float*)(shred->reg + SZ_INT));
  } else if(instr->m_val == Kindof_Complex) {
    POP_REG(shred, SZ_COMPLEX);
    o = *(M_Object*)(shred->reg);
    c_vector_append(o->d.array, *(m_complex*)(shred->reg + SZ_INT));
  } else if(instr->m_val == Kindof_Vec3) {
    POP_REG(shred, SZ_VEC3);
    o = *(M_Object*)(shred->reg);
    v3_vector_append(o->d.array, *(VEC3_T*)(shred->reg + SZ_INT));
  } else if(instr->m_val == Kindof_Vec4) {
    POP_REG(shred, SZ_VEC4);
    o = *(M_Object*)(shred->reg);
    v4_vector_append(o->d.array, *(VEC4_T*)(shred->reg + SZ_INT));
  }
  release(o, shred);
  *(M_Object*)(shred->reg) = o;
  PUSH_REG(shred, SZ_INT);
}

m_bool import_array(Env env) {
  DL_Func* fun;
  CHECK_BB(add_global_type(env, &t_array))
  CHECK_OB(import_class_begin(env, &t_array, env->global_nspc, NULL, array_dtor))
  fun = new_dl_func("int", "size", (m_uint)vm_vector_size);
  CHECK_OB(import_mfun(env, fun))
  fun = new_dl_func("int", "depth", (m_uint)vm_vector_depth);
  CHECK_OB(import_mfun(env, fun))
  fun = new_dl_func("int", "cap", (m_uint)vm_vector_cap);
  CHECK_OB(import_mfun(env, fun))
  fun = new_dl_func("int", "remove", (m_uint)vm_vector_remove);
  dl_func_add_arg(fun, "int", "index");
  CHECK_OB(import_mfun(env, fun))
  env->class_def->doc = "vector structure";
  CHECK_BB(import_class_end(env))
  return 1;
}
