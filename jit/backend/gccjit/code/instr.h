#include "jitter.h"
#include "cc.h"
#include "code.h"

JIT_CODE(EOC) {
  TYPE arg_types[] = { GET_TYPE(pointer) };
  RVAL args[] = { cc->shred  };
  TYPE fptr_type = FPTR(GET_TYPE(void), 1, arg_types, 0);
  RVAL func = PCONST(fptr_type, vm_shred_exit);
  EVAL(cc->block, CALL(func, 1, args));
}

JIT_CODE(InitLoopCounter) {
// TODO use (l)abs
  push_reg(cc, -SZ_INT);
  RVAL jinstr = PCONST(GET_TYPE(instr), instr);
  LVAL ptr = cc_deref_field(cc, jinstr, "instr:m_val");
  LVAL tgt = gcc_jit_rvalue_dereference(CAST(AS_RVAL(ptr), GET_TYPE(m_uint*)), NULL);
  LVAL reg = cc_deref_field(cc, cc->shred, "shred:reg");
  LVAL val = gcc_jit_rvalue_dereference(CAST(AS_RVAL(reg), GET_TYPE(m_uint*)), NULL);
  ASSIGN(cc->block, tgt, AS_RVAL(val));
}

JIT_CODE(RegPushImm) {
  TYPE ptype = GET_TYPE(pointer);
  LVAL reg = cc_deref_field(cc, cc->shred, "shred:reg");
  RVAL len = ICONST(GET_TYPE(m_uint), instr->m_val);
  TYPE arg_types[] = { ptype, ptype, GET_TYPE(m_uint) };
  TYPE fptr_type = FPTR(ptype, 3, arg_types, 0);
  RVAL func = PCONST(fptr_type, memcpy);
  RVAL ptr = PCONST(ptype, instr->ptr);
  RVAL args[] = { AS_RVAL(reg), ptr, len };
  EVAL(cc->block, CALL(func, 3, args));
  push_reg(cc, instr->m_val);
}

JIT_CODE(RegPushDeref) {
  TYPE ptype = GET_TYPE(pointer);
  LVAL reg = cc_deref_field(cc, cc->shred, "shred:reg");
  RVAL len = ICONST(GET_TYPE(m_uint), instr->m_val);
  TYPE arg_types[] = { ptype, ptype, GET_TYPE(m_uint) };
  TYPE fptr_type = FPTR(ptype, 3, arg_types, 0);
  RVAL func = PCONST(fptr_type, memcpy);
  RVAL jinstr = PCONST(GET_TYPE(instr), instr);
  LVAL ptr = cc_deref_field(cc, jinstr, "instr:ptr");
  RVAL args[] = { AS_RVAL(reg), AS_RVAL(ptr), len };
  EVAL(cc->block, CALL(func, 3, args));
  push_reg(cc, instr->m_val);
}

JIT_CODE(DecIntAddr) {
  RVAL pval = PCONST(GET_TYPE(m_uint*), instr->m_val);
  LVAL val = gcc_jit_rvalue_dereference(pval, NULL);
  gcc_jit_block_add_assignment_op (cc->block, NULL, val,
  GCC_JIT_BINARY_OP_MINUS,
  gcc_jit_context_one(cc->ctx, GET_TYPE(m_uint)));
}

JIT_CODE(RegPop) {
  push_reg(cc, -instr->m_val);
}

JIT_CODE(Goto) {
  LVAL pc = cc_deref_field(cc, cc->shred, "shred:pc");
  RVAL val = ICONST(GET_TYPE(m_uint), instr->m_val);
  ASSIGN(cc->block, pc, val);
}

#define describe_jit_branch(name, type, sz, op)\
JIT_CODE(Branch##name) {\
  push_reg(cc, -sz*2);\
  LVAL reg = cc_deref_field(cc, cc->shred, "shred:reg");\
  BLOCK test = NEW_BLOCK("start eq int test");\
  BLOCK on_true = NEW_BLOCK("if");\
  BLOCK after = NEW_BLOCK("after");\
  BLOCK_JMP(cc->block, test);\
  RVAL ireg = CAST(AS_RVAL(reg), GET_TYPE(type));\
  RVAL zero = ICONST(cc_get_type(cc, "m_uint"), 0);\
  RVAL size  = ICONST(cc_get_type(cc, "m_uint"), 1);\
  LVAL lhs = ARRAY_ACCESS(ireg, zero);\
  LVAL rhs = ARRAY_ACCESS(ireg, size);\
  RVAL cond = COMP(op, AS_RVAL(lhs), AS_RVAL(rhs));\
  BLOCK_CND(test, cond, on_true, after);\
  LVAL pc = cc_deref_field(cc, cc->shred, "shred:pc");\
  RVAL val = ICONST(cc_get_type(cc, "m_uint"), instr->m_val);\
  ASSIGN(on_true, pc, val);\
  BLOCK_RET(on_true, after);\
}
describe_jit_branch(EqInt, m_int*, SZ_INT, EQ)
describe_jit_branch(NeqInt, m_int*, SZ_INT, NE)
describe_jit_branch(EqFloat, m_float*, SZ_FLOAT, EQ)
describe_jit_branch(NeqFloat, m_float*, SZ_FLOAT, NE)

JIT_CODE(GcIni) {
  LVAL gc = cc_deref_field(cc, cc->shred, "shred:gc");
  TYPE arg_types[] = { GET_TYPE(pointer), GET_TYPE(pointer) };
  RVAL args[] = { ADDR(gc), PCONST(GET_TYPE(pointer), NULL)  };
  TYPE fptr_type = FPTR(GET_TYPE(void), 2, arg_types, 0);
  RVAL func = PCONST(fptr_type, vector_add);
  EVAL(cc->block, CALL(func, 2, args));
}

JIT_CODE(GcAdd) {
  TYPE ptype = GET_TYPE(pointer);
  LVAL gc = cc_deref_field(cc, cc->shred, "shred:gc");
  TYPE arg_types[] = { ptype, ptype };
  LVAL reg = cc_deref_field(cc, cc->shred, "shred:reg");
  RVAL vreg = CAST(AS_RVAL(reg), GET_TYPE(object*));
  LVAL obj = ARRAY_ACCESS(vreg, ICONST(GET_TYPE(m_int), -1)); // ??? => -1
  RVAL args[] = { ADDR(gc), AS_RVAL(obj)  };
  TYPE fptr_type = FPTR(GET_TYPE(void), 2, arg_types, 0);
  RVAL func = PCONST(fptr_type, vector_add);
  EVAL(cc->block, CALL(func, 2, args));
}

JIT_CODE(GcEnd) {
  BLOCK b_test = NEW_BLOCK("garbage collector end. (test)");
  BLOCK_JMP(cc->block, b_test);
  RVAL gc = ADDR(cc_deref_field(cc, cc->shred, "shred:gc"));
  TYPE arg_types[] = { GET_TYPE(vector) };
  TYPE fptr_type = FPTR(GET_TYPE(object), 1, arg_types, 0);
  RVAL func = PCONST(fptr_type, vector_pop);
  RVAL obj = CALL(func, 1, &gc);
  RVAL cond = COMP(NE, obj, PCONST(GET_TYPE(object), NULL));
  BLOCK b_after = NEW_BLOCK("garbage collector end. (after)");
  BLOCK b_action = NEW_BLOCK("garbage collector end. (action)");
  BLOCK_CND(b_test, cond, b_action, b_after);
/*
{
  TYPE types[2] = { GET_TYPE(object), GET_TYPE(shred) };
  RVAL args[2] = { obj, cc->shred };
  TYPE fptr = FPTR(GET_TYPE(void), 2, types, 0);
  RVAL func = PCONST(fptr, __release);
  EVAL(b_action, CALL(func, 2, args));
}
*/
//  cc->block = b_action;
//  cc_release(cc, obj);
  BLOCK_JMP(b_action, b_test);
  cc->block = b_after;
}

JIT_CODE(ObjectInstantiate) {
  TYPE otype = GET_TYPE(pointer); // M_Object or rather object
  LVAL reg = cc_deref_field(cc, cc->shred, "shred:reg");
  TYPE arg_types[] = { otype, otype };
   RVAL args[] = { cc->shred, PCONST(otype, instr->m_val)  };// Type type
  TYPE fptr_type = FPTR(otype, 2, arg_types, 0);
  RVAL func = PCONST(fptr_type, new_object);
  RVAL obj = CALL(func, 2, args);
  RVAL vreg = CAST(AS_RVAL(reg), GET_TYPE(pointer*));
//  LVAL tgt = ARRAY_ACCESS(vreg, zero);
  LVAL tgt = gcc_jit_rvalue_dereference(vreg, NULL);
  ASSIGN(cc->block, tgt, obj);
  push_reg(cc, SZ_INT);
}

JIT_CODE(MemPushImm) {
  LVAL mem = cc_deref_field(cc, cc->shred, "shred:mem");
  RVAL idx = ICONST(GET_TYPE(m_uint), 0);
  RVAL val = ICONST(GET_TYPE(m_uint), instr->m_val);
  LVAL tgt_root = ARRAY_ACCESS(AS_RVAL(mem), idx);
  RVAL tgt_ptr = CAST(ADDR(tgt_root), GET_TYPE(m_uint*));
//  LVAL tgt = ARRAY_ACCESS(tgt_ptr, ICONST(GET_TYPE(m_uint), 0));
  LVAL tgt = gcc_jit_rvalue_dereference(tgt_ptr, NULL);
  ASSIGN(cc->block, tgt, val);
  push_mem(cc, SZ_INT);
}

JIT_CODE(MemSetImm) {
  LVAL mem = cc_deref_field(cc, cc->shred, "shred:mem");
  RVAL idx = ICONST(GET_TYPE(m_uint), instr->m_val);
  RVAL val = ICONST(GET_TYPE(m_uint), instr->m_val2);
  LVAL tgt_root = ARRAY_ACCESS(AS_RVAL(mem), idx);
  RVAL tgt_ptr = CAST(ADDR(tgt_root), GET_TYPE(m_uint*));
//  LVAL tgt = ARRAY_ACCESS(tgt_ptr, ICONST(GET_TYPE(m_uint), 0));
  LVAL tgt = gcc_jit_rvalue_dereference(tgt_ptr, NULL);
  ASSIGN(cc->block, tgt, val);
}

JIT_CODE(ObjectRelease) {
  LVAL mem = cc_deref_field(cc, cc->shred, "shred:mem");
  LVAL obj_root = ARRAY_ACCESS(AS_RVAL(mem), ICONST(GET_TYPE(m_uint), instr->m_val));
  RVAL obj_ptr = CAST(ADDR(obj_root), GET_TYPE(object*));
//  LVAL obj = ARRAY_ACCESS(obj_ptr, ICONST(GET_TYPE(m_uint), 0));
  LVAL obj = gcc_jit_rvalue_dereference(obj_ptr, NULL);
  cc_release2(cc, AS_RVAL(obj));
}

JIT_CODE(AllocWord) {
  RVAL zero = ICONST(GET_TYPE(m_bool), 0);
  RVAL size = ICONST(GET_TYPE(m_uint), instr->m_val2);
  LVAL reg = cc_deref_field(cc, cc->shred, "shred:reg");
  LVAL mem = cc_deref_field(cc, cc->shred, "shred:mem");
  LVAL obj_root = ARRAY_ACCESS(AS_RVAL(mem), ICONST(GET_TYPE(m_uint), instr->m_val));
  RVAL obj = ADDR(obj_root);
//  RVAL obj = ADDR(_obj_ptr);
//  LVAL _obj = ARRAY_ACCESS(obj_ptr, ICONST(GET_TYPE(m_uint), 0));
//  RVAL obj = ADDR(_obj);
// memset
  TYPE arg_types[] = { GET_TYPE(pointer), GET_TYPE(m_bool), GET_TYPE(m_uint) };
  RVAL args[] = { obj, zero, size  };// Type type
  TYPE fptr_type = FPTR(GET_TYPE(pointer), 3, arg_types, 0);
  RVAL func = PCONST(fptr_type, memset);
  EVAL(cc->block, CALL(func, 3, args));
  if(*(m_uint*)instr->ptr) {
//    LVAL tgt = ARRAY_ACCESS(CAST(AS_RVAL(reg), GET_TYPE(pointer*)), zero);
    LVAL tgt = gcc_jit_rvalue_dereference(CAST(AS_RVAL(reg), GET_TYPE(pointer*)), NULL);
    ASSIGN(cc->block, tgt, obj);
//exit(2);
  } else {
    TYPE arg_types[] = { GET_TYPE(pointer), GET_TYPE(pointer), GET_TYPE(m_uint) };
    RVAL args[] = { AS_RVAL(reg), obj, size };// Type type
    TYPE fptr_type = FPTR(GET_TYPE(pointer), 3, arg_types, 0);
    RVAL func = PCONST(fptr_type, memcpy);
    EVAL(cc->block, CALL(func, 3, args));
  }
  push_reg2(cc, size);
}

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_instr(struct Jit* j) {
  JIT_IMPORT(RegPushImm);
//  JIT_IMPORT(InitLoopCounter);
  JIT_IMPORT(RegPushDeref);
  JIT_IMPORT(DecIntAddr);
  JIT_IMPORT(RegPop)
  JIT_IMPORT(Goto);
  JIT_IMPORT(EOC);
  JIT_IMPORT(GcIni);
  JIT_IMPORT(GcAdd);
//  JIT_IMPORT(GcEnd);
  JIT_IMPORT(BranchEqInt);
  JIT_IMPORT(BranchNeqInt);
  JIT_IMPORT(BranchEqFloat);
  JIT_IMPORT(BranchNeqFloat);
  JIT_IMPORT(ObjectInstantiate)
  JIT_IMPORT(MemPushImm);
  JIT_IMPORT(MemSetImm);
  JIT_IMPORT(ObjectRelease)
  JIT_IMPORT(AllocWord)
}
