#include "jitter.h"
#include "cc.h"
#include "code.h"
/*
static M_Object add2gc(VM_Shred shred, M_Object obj) {
  vector_add(&shred->gc, (vtype)obj);
  return obj;
}
*/
static M_Object jit_new_array(VM_Shred shred, Type t, m_uint size,
  m_uint length, m_uint depth) {
  M_Object obj = new_array(t, size, length, depth);
  vector_add(&shred->gc, (vtype)obj);
  return obj;
}
JIT_CODE(ArrayInit) {
  const ArrayInfo* info = *(ArrayInfo**)instr->ptr;
  push_reg(cc, -(instr->m_val2 * info->d.length  -SZ_INT));
//  TYPE arg_types[5] = { GET_TYPE(shred), GET_TYPE(pointer),
  TYPE arg_types[4] = { GET_TYPE(pointer),
    GET_TYPE(m_uint), GET_TYPE(m_uint), GET_TYPE(m_uint) };
  RVAL args[4] = {
    PCONST(GET_TYPE(pointer), info->type),
    ICONST(GET_TYPE(m_uint), info->base->size),
    ICONST(GET_TYPE(m_uint), info->d.length),
    ICONST(GET_TYPE(m_uint), info->depth)
  };
  TYPE fptr_type = FPTR(GET_TYPE(object), 4, arg_types, 0);
  RVAL func = PCONST(fptr_type, new_array);
  RVAL obj = CALL(func, 4, args);

  {
/*
    LVAL gc = cc_deref_field(cc, cc->shred, "shred:gc");
    TYPE arg_types[2] = { GET_TYPE(vector), GET_TYPE(pointer) };
    RVAL args[2] = { ADDR(gc), obj };
    TYPE fptr_type = FPTR(GET_TYPE(void), 2, arg_types, 0);
    RVAL func = PCONST(fptr_type, vector_add);
    EVAL(cc->block, CALL(func, 2, args));
*/
/*
    TYPE arg_types[2] = { GET_TYPE(shred), GET_TYPE(object) };
    RVAL args[2] = { cc->shred, obj };
    TYPE fptr_type = FPTR(GET_TYPE(void), 2, arg_types, 0);
    RVAL func = PCONST(fptr_type, add2gc);
    EVAL(cc->block, CALL(func, 2, args));
*/
  }

  // add ref

  LVAL type = cc_deref_field(cc, obj, "object:type_ref");
//  RVAL type = PCONST(GET_TYPE(type), info->type);
  LVAL oo   = cc_deref_field(cc, AS_RVAL(type), "type:obj");
  LVAL ref  = //cc_deref_field(cc, ADDR(oo), "oo:ref_count");
   gcc_jit_lvalue_access_field(oo, NULL, cc_get_field(cc, "oo:ref_count"));
  gcc_jit_block_add_assignment_op (
    cc->block, NULL, ref, GCC_JIT_BINARY_OP_PLUS,
  gcc_jit_context_one(cc->ctx, GET_TYPE(m_uint)));


  LVAL reg = cc_deref_field(cc, cc->shred, "shred:reg");
  LVAL tgt = ARRAY_ACCESS(CAST(AS_RVAL(reg), GET_TYPE(object*)),
    ICONST(GET_TYPE(m_uint), -1));
 // memcpy
{
  LVAL data = cc_deref_field(cc, obj, "object:data");
  LVAL array = gcc_jit_rvalue_dereference(CAST(AS_RVAL(data), GET_TYPE(vector*)), NULL);
  LVAL ptr = cc_deref_field(cc, AS_RVAL(array), "vector:ptr");
  LVAL tgt_root = ARRAY_ACCESS(CAST(AS_RVAL(ptr), GET_TYPE(pointer*)),
     ICONST(GET_TYPE(m_uint), 4)); // array_offset
  RVAL size = ICONST(GET_TYPE(m_uint), instr->m_val2 * info->d.length);
  TYPE arg_types[3] = { GET_TYPE(pointer), GET_TYPE(pointer), GET_TYPE(m_uint) };
  RVAL args[3] = { ADDR(tgt_root), ADDR(tgt), size };
  TYPE fptr_type = FPTR(GET_TYPE(void*), 3, arg_types, 0);
  RVAL func = PCONST(fptr_type, memcpy);
  EVAL(cc->block, CALL(func, 3, args));
}
  // assign to shred->reg[-SZ_INT];


  ASSIGN(cc->block, tgt, obj);
/*
{
    TYPE arg_types[2] = { GET_TYPE(shred), GET_TYPE(instr) };
    RVAL args[2] = { cc->shred, PCONST(GET_TYPE(instr), NULL) };
    TYPE fptr_type = FPTR(GET_TYPE(void), 2, arg_types, 0);
    RVAL func = PCONST(fptr_type, GcAdd);
    EVAL(cc->block, CALL(func, 2, args));
}
*/
}

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_array(struct Jit* j) {
//  JIT_IMPORT(ArrayInit)
}
