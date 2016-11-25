#include "dl.h"
#include "type.h"

void dl_return_push(const DL_Return retval, VM_Shred shred, int kind)
{
    if(kind == Kindof_Int) {
        *(m_uint*)shred->reg = retval.v_uint;
        shred->reg += SZ_INT;
    } else if(kind == Kindof_Float) {
        *(m_float*)shred->reg = retval.v_float;
        shred->reg += SZ_FLOAT;
    } else if(kind == Kindof_Complex) {
        *(complex*)shred->reg = retval.v_complex;
        shred->reg += SZ_COMPLEX;
    } else if(kind == Kindof_Vec3) {
        *(VEC3_T*)shred->reg = retval.v_vec3;
        shred->reg += SZ_VEC3;
    } else if(kind == Kindof_Vec4) {
        *(VEC4_T*)shred->reg = retval.v_vec4;
        shred->reg += SZ_VEC4;
    }
    return;
}

DL_Func* new_DL_Func(const m_str t, const m_str n, m_uint addr)
{
    DL_Func* a = malloc(sizeof(DL_Func));
    a->ctor = NULL;
    a->name = n;
    a->type = t;
    a->addr = addr;
    a->args = new_Vector();
    return a;
}

void freeDL_Func(DL_Func* a)
{
    free_Vector(a->args);
    free(a);
}

DL_Value* new_DL_Value(const m_str t, const m_str  n, m_bool c, void* addr)
{
    DL_Value* a = malloc(sizeof(DL_Value));
    a->name = n;
    a->type = t;
    a->is_const  = c;
    a->static_addr = addr;
    a->doc = NULL;
    return a;
}

void freeDL_Value(DL_Value* a)
{
    /*  if(addr)*/
    /*    free(addr);*/
    free(a);
}

DL_Value* dl_func_add_arg(DL_Func* a, const m_str t, const m_str  n)
{
    DL_Value* v = new_DL_Value( t, n, 0, NULL );
    vector_append(a->args, (vtype)v);
    return v;
}
