#include "dl.h"
#include "type.h"
#include "import.h"

void dl_return_push(const DL_Return retval, VM_Shred shred, int kind) {
  if(kind == Kindof_Int) {
    *(m_uint*)REG(0) = retval.d.v_uint;
    PUSH_REG(shred, SZ_INT);
  } else if(kind == Kindof_Float) {
    *(m_float*)REG(0) = retval.d.v_float;
    PUSH_REG(shred, SZ_FLOAT);
  } else if(kind == Kindof_Complex) {
    *(m_complex*)REG(0) = retval.d.v_complex;
    PUSH_REG(shred, SZ_COMPLEX);
  } else if(kind == Kindof_Vec3) {
    *(m_vec3*)REG(0) = retval.d.v_vec3;
    PUSH_REG(shred, SZ_VEC3);
  } else if(kind == Kindof_Vec4) {
    *(m_vec4*)REG(0) = retval.d.v_vec4;
    PUSH_REG(shred, SZ_VEC4);
  }
  return;
}

void dl_func_init(DL_Func* a, const m_str t, const m_str n, m_uint addr) {
  a->name = n;
  a->type = t;
  a->addr = addr;
  a->n_arg = 0;
}

static DL_Value* new_DL_Value(const m_str t, const m_str  n, m_bool c, void* addr) {
  DL_Value* a = malloc(sizeof(DL_Value));
  a->name = n;
  a->type = t;
  return a;
}

DL_Value* dl_func_add_arg(DL_Func* a, const m_str t, const m_str  n) {
  DL_Value* v = new_DL_Value(t, n, 0, NULL);
  a->args[a->n_arg++] = v;
  return v;
}
