#include "defs.h"
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "lang.h"
#include "import.h"

#include "object.h"

m_int o_array_vector;

DTOR(array_dtor)
{
  if(o->type_ref->array_type) // maybe unnecessary. preferably check array depth
    if(o->d.array->depth > 1 || isa(o->type_ref->array_type, &t_object) > 0) {
      m_uint i;
      for(i = 0; i < o->d.array->len; i += SZ_INT)
        release(*(M_Object*)(o->d.array->ptr + i), shred);
  }
  free(o->d.array->ptr);
// should not compare to t_array. see new_M_Array(Type t, ...
  if(o->type_ref != &t_array) {
    if(!--o->type_ref->obj->ref_count) {
      free(o->type_ref->obj);
      free(o->type_ref);
    }
  } else err_msg(INSTR_, 0, "array type not valid. please fix and implement a better new M_Array");
}

M_Object new_M_Array(m_uint size, m_uint length, m_uint depth)
{
  M_Object a = new_M_Object();
  initialize_object(a, &t_array);
  a->d.array 	    = malloc(sizeof(M_Vector));
  a->d.array->ptr   = calloc(length, size);
  a->d.array->size  = size;
  a->d.array->len   = length * size;
  a->d.array->depth = depth;
  return a;
}

m_uint m_vector_size(M_Vector* v)
{
  return v->len / v->size;
  /*  return v->len;*/
}

m_uint  i_vector_at(M_Vector* v, m_uint i)
{
  return *(m_uint*)(v->ptr + i * v->size);
}

m_float f_vector_at(M_Vector* v, m_uint i)
{
  return *(m_float*)(v->ptr + i * v->size);
}

m_complex c_vector_at(M_Vector* v, m_uint i)
{
  return *(m_complex*)(v->ptr + i * v->size);
}

VEC3_T v3_vector_at(M_Vector* v, m_uint i)
{
  return *(VEC3_T*)(v->ptr + i * v->size);
}

VEC4_T v4_vector_at(M_Vector* v, m_uint i)
{
  return *(VEC4_T*)(v->ptr + i * v->size);
}

void i_vector_append(M_Vector* v, m_uint i)
{
  v->len++;
  v->ptr = realloc(v->ptr, v->len * v->size);
  *(m_uint*)(v->ptr + (v->len - 1)*v->size) = i;
}

void f_vector_append(M_Vector* v, m_float f)
{
  v->len++;
  v->ptr = realloc(v->ptr, v->len * v->size);
  *(m_float*)(v->ptr + (v->len - 1)*v->size) = f;
}

void c_vector_append(M_Vector* v, m_complex c)
{
  v->len++;
  v->ptr = realloc(v->ptr, v->len * v->size);
  *(m_complex*)(v->ptr + (v->len - 1)*v->size) = c;
}

void v3_vector_append(M_Vector* v, VEC3_T c)
{
  v->len++;
  v->ptr = realloc(v->ptr, v->len * v->size);
  *(VEC3_T*)(v->ptr + (v->len - 1)*v->size) = c;
}

void v4_vector_append(M_Vector* v, VEC4_T c)
{
  v->len++;
  v->ptr = realloc(v->ptr, v->len * v->size);
  *(VEC4_T*)(v->ptr + (v->len - 1)*v->size) = c;
}

void i_vector_set(M_Vector* v, m_uint i, m_uint data)
{
  v->ptr = realloc(v->ptr, v->len * v->size);
  *(m_uint*)(v->ptr + i * v->size) = data;
}

void f_vector_set(M_Vector* v, m_uint i, m_float data)
{
  v->ptr = realloc(v->ptr, v->len * v->size);
  *(m_float*)(v->ptr + i * v->size) = data;
}
/*
void c_vector_set(M_Vector* v, m_uint i, m_complex data)
{
  v->ptr = realloc(v->ptr, v->len * v->size);
  *(m_complex*)(v->ptr + i * v->size) = data;
}

void v3_vector_set(M_Vector* v, m_uint i, VEC3_T data)
{
  v->ptr = realloc(v->ptr, v->len * v->size);
  *(VEC3_T*)(v->ptr + i * v->size) = data;
}

void v4_vector_set(M_Vector* v, m_uint i, VEC4_T data)
{
  v->ptr = realloc(v->ptr, v->len * v->size);
  *(VEC4_T*)(v->ptr + i * v->size) = data;
}
*/

m_uint*  i_vector_addr(M_Vector* v, m_uint i)
{
  return &*(m_uint*)(v->ptr + i * v->size);
}

m_float*  f_vector_addr(M_Vector* v, m_uint i)
{
  return &*(m_float*)(v->ptr + i * v->size);
}

m_complex*  c_vector_addr(M_Vector* v, m_uint i)
{
  return &*(m_complex*)(v->ptr + i * v->size);
}

VEC3_T*  v3_vector_addr(M_Vector* v, m_uint i)
{
  return &*(VEC3_T*)(v->ptr + i * v->size);
}

VEC4_T*  v4_vector_addr(M_Vector* v, m_uint i)
{
  return &*(VEC4_T*)(v->ptr + i * v->size);
}

MFUN(vm_vector_size)
{
  RETURN->d.v_uint = o ? m_vector_size(o->d.array) : - 1;
}

INSTR(Array_Append)
{
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

m_bool import_array(Env env)
{
  DL_Func* fun;
  CHECK_BB(add_global_type(env, &t_array))
  CHECK_OB(import_class_begin(env, &t_array, env->global_nspc, NULL, array_dtor))
  fun = new_DL_Func("int", "size", (m_uint)vm_vector_size);
  CHECK_OB(import_mfun(env, fun))
  env->class_def->doc = "vector structure";
  CHECK_BB(import_class_end(env))
  return 1;
}
