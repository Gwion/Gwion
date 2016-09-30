#include "defs.h"
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "lang.h"
#include "import.h"

#include "object.h"

m_int o_array_vector;
/*void array_ctor(M_Object o, VM_Shred shred)*/
/*void array_dtor(M_Object o, VM_Shred shred)*/

M_Object new_M_Array(m_uint size, m_uint length)
{
  M_Object a = new_M_Object();
	initialize_object(a, &t_array);
	a->array 	    = malloc(sizeof(M_Vector));
  a->array->ptr  = calloc(length ,size);
	a->array->size = size;
	a->array->len  = length*size;
  return a;
}

m_uint m_vector_size(M_Vector* v)
{
  return v->len/v->size;
/*  return v->len;*/
}

m_uint  i_vector_at(M_Vector* v, m_uint i)
{
  return *(m_uint*)(v->ptr + i*v->size);
}

m_float f_vector_at(M_Vector* v, m_uint i)
{
  return *(m_float*)(v->ptr + i*v->size);
}

complex c_vector_at(M_Vector* v, m_uint i)
{
  return *(complex*)(v->ptr + i*v->size);
}

VEC3_T v3_vector_at(M_Vector* v, m_uint i)
{
  return *(VEC3_T*)(v->ptr + i*v->size);
}

VEC4_T v4_vector_at(M_Vector* v, m_uint i)
{
  return *(VEC4_T*)(v->ptr + i*v->size);
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

void c_vector_append(M_Vector* v, complex c)
{
  v->len++;
  v->ptr = realloc(v->ptr, v->len * v->size);
  *(complex*)(v->ptr + (v->len - 1)*v->size) = c;
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

void c_vector_set(M_Vector* v, m_uint i, complex data)
{
  v->ptr = realloc(v->ptr, v->len * v->size);
  *(complex*)(v->ptr + i * v->size) = data;
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


m_uint*  i_vector_addr(M_Vector* v, m_uint i)
{
  return &*(m_uint*)(v->ptr + i*v->size);
}

m_float*  f_vector_addr(M_Vector* v, m_uint i)
{
  return &*(m_float*)(v->ptr + i*v->size);
}

complex*  c_vector_addr(M_Vector* v, m_uint i)
{
  return &*(complex*)(v->ptr + i*v->size);
}

VEC3_T*  v3_vector_addr(M_Vector* v, m_uint i)
{
  return &*(VEC3_T*)(v->ptr + i*v->size);
}

VEC4_T*  v4_vector_addr(M_Vector* v, m_uint i)
{
  return &*(VEC4_T*)(v->ptr + i*v->size);
}

void vm_vector_size(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
  if(!o)
    RETURN->v_uint = -1;
  RETURN->v_uint = m_vector_size(o->array);
}

INSTR(Array_Append)
{
  shred->reg -= SZ_INT;
  M_Object o;
  if(instr->m_val == Kindof_Int)
  {
    shred->reg -= SZ_INT;
    o = *(M_Object*)(shred->reg);
    i_vector_append(o->array, *(m_uint*)(shred->reg+SZ_INT));
  }
  else if(instr->m_val == Kindof_Float)
  {
    shred->reg -= SZ_FLOAT;
    o = *(M_Object*)(shred->reg);
    f_vector_append(o->array, *(m_float*)(shred->reg+SZ_INT));
  }
  else if(instr->m_val == Kindof_Complex)
  {
    shred->reg -= SZ_COMPLEX;
    o = *(M_Object*)(shred->reg);
    c_vector_append(o->array, *(complex*)(shred->reg+SZ_INT));
  }
  else if(instr->m_val == Kindof_Vec3)
  {
    shred->reg -= SZ_VEC3;
    o = *(M_Object*)(shred->reg);
    v3_vector_append(o->array, *(VEC3_T*)(shred->reg+SZ_INT));
  }
  else if(instr->m_val == Kindof_Vec4)
  {
    shred->reg -= SZ_VEC4;
    o = *(M_Object*)(shred->reg);
    v4_vector_append(o->array, *(VEC4_T*)(shred->reg+SZ_INT));
  }
  *(M_Object*)(shred->reg) = o;
  shred->reg += SZ_INT;
}

m_bool import_array(Env env)
{
  DL_Func* fun;
	CHECK_BB(add_global_type(env, &t_array))
  CHECK_BB(import_class_begin(env, &t_array, env->global_nspc, NULL, NULL))
  fun = new_DL_Func("int", "size", (m_uint)vm_vector_size);
  CHECK_OB(import_mfun(env, fun))
	env->class_def->doc = "vector structure";
	CHECK_BB(import_class_end(env))
  return 1;
}
