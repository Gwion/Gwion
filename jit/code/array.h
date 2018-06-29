#include "jit/jit.h"
#include "gwjit_common.h"
#include "cc.h"
#include "q.h"
#include "sig.h"
#include "code.h"
/*
JIT_CODE(Array_Append) {
  CJval reg = push_reg(j, -instr->m_val);
  CJval o = JLOADR(reg, -SZ_INT, void_ptr);
  CJval nil  = JCONST(void_ptr, 0);
  INIT_LABEL(lbl);
  CJval cond = JINSN(eq, o, nil);
  JINSN(branch_if_not, cond, &lbl);
  cc_except(j, "NullPtrException");
  JINSN(label, &lbl);
  CJval data = JLOADR(o, JOFF(M_Object, data), void_ptr);
  CJval array = JLOADR(data, 0, void_ptr);
  CJval arg0[] = { array, reg };
  CALL_NATIVE(m_vector_add, "vpp", arg0);
  CJval arg1[] = { o, j->shred };
  CALL_NATIVE(_release, "vpp", arg1);
  JSTORER(reg, -SZ_INT, o);
}

JIT_CODE(Instr_Pre_Ctor_Array_Top) {
  INIT_LABEL(lbl0);
  INIT_LABEL(lbl1);
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval idx = JLOADR(reg, -SZ_INT*2, nuint);
  CJval len = JLOADR(reg, -SZ_INT, nuint);
  CJval cond = JINSN(lt, idx, len);
  JINSN(branch_if_not, cond, &lbl0);
  CJval type = JCONST(void_ptr, *(Type*)instr->ptr);
  CJval arg[] = { j->shred, type };
  CALL_NATIVE(instantiate_object, "vpp", arg);
  JINSN(branch, &lbl1);
  JINSN(label, &lbl0);
  INIT_LABEL(lbl2);
  CJval zero = JCONST(nuint, 0);
  CJval cond2 = JINSN(eq, len, zero);
  JINSN(branch_if_not, cond2, &lbl2);
  JSTORER(reg, -SZ_INT*3, zero);
  JINSN(label, &lbl2);
  next_pc(j, instr->m_val);
  JINSN(label, &lbl1);
}

JIT_CODE(Instr_Pre_Ctor_Array_Bottom) {
  CJval reg = push_reg(j, -SZ_INT);
  CJval arr = JLOADR(reg, -SZ_INT*3, void_ptr);
  CJval idx = JLOADR(reg, -SZ_INT*2, nint);
  CJval obj = JLOADR(reg, 0, void_ptr);
  CJval array = JINSN(load_elem, arr, idx, jit_type_void_ptr);
  JSTORER(array, 0, obj);
  CJval one = JCONST(nint, 1);
  CJval sum  = JINSN(add, idx, one);
  JSTORER(reg, -SZ_INT*2, sum);
  next_pc(j, instr->m_val);
}

JIT_CODE(Instr_Pre_Ctor_Array_Post) {
  CJval reg = push_reg(j, -SZ_INT*3);
  CJval arr = JLOADR(reg, 0, void_ptr);
  CJval nil = JCONST(void_ptr, 0);
  INIT_LABEL(lbl);
  CJval cond = JINSN(eq, arr, nil);
  JINSN(branch_if_not, cond, &lbl);
  CJval arg[] = { arr };
  CALL_NATIVE(free, "vp", arg);
  JINSN(label, &lbl);
}

static void jit_oob(const struct JitCC_* j, CJval obj,
  CJval dim, CJval idx, CJval base) {
  CJval zero = JCONST(nint, 0);
  CJval cond = JINSN(lt, idx, zero);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval arr  = JLOADR(data, 0, void_ptr);
  CJval len = JLOADR(arr, JOFF(M_Vector, len), nuint);
  INIT_LABEL(lbl);
  INIT_LABEL(lbl2);
  JINSN(branch_if, cond, &lbl);
  CJval uidx = JINSN(convert, idx, jit_type_nuint, 0);
  CJval cond1 = JINSN(ge, uidx, len);
  JINSN(branch_if_not, cond1, &lbl2);
  JINSN(label, &lbl);
  CJval arg0[] = { base, j->shred };
  CALL_NATIVE(_release, "vpp", arg0);
  CJval arg1[] = { dim, idx };
  CALL_NATIVE(array_msg, "vUi", arg1);
  cc_except(j, "ArrayOutofBounds");
  JINSN(label, &lbl2);
}

JIT_CODE(Instr_Array_Access) {
  CJval reg = push_reg(j, -SZ_INT*2);
  CJval obj = JLOADR(reg, 0, void_ptr);
  CJval nil  = JCONST(void_ptr, 0);
  CJval dim  = JCONST(nuint, 0);
  INIT_LABEL(lbl);
  CJval cond = JINSN(eq, obj, nil);
  JINSN(branch_if_not, cond, &lbl);
  cc_except(j, "NullPtrException");
  JINSN(label, &lbl);
  CJval idx = JLOADR(reg, SZ_INT, nint);
  jit_oob(j , obj, dim, idx, obj);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval arr  = JLOADR(data, 0, void_ptr);
  CJval size = JCONST(nuint, instr->m_val2);
  CJval emit = JCONST(int, instr->m_val);
  CJval arg[] = { j->shred, arr, idx, size, emit };
  CALL_NATIVE(array_push, "vppUUi", arg);
}

*/
