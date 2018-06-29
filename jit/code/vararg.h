#include "jit/jit.h"
#include "gwjit_common.h"
#include "cc.h"
#include "q.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"
/*
ANN static inline Jval get_vararg(const JitCC* j, const Instr instr) {
  CJval mem  = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  return JLOADR(mem, instr->m_val, void_ptr);
}

JIT_CODE(Vararg_start) {
  CJval var = get_vararg(j, instr);
  CJval data = JLOADR(var, JOFF(Vararg, d), void_ptr);
  CJval null = JCONST(void_ptr, 0);
  CJval cond = JINSN(eq, data, null);
  INIT_LABEL(lbl);
  INIT_LABEL(end);
  JINSN(branch_if, cond, &lbl);
  push_reg(j, SZ_INT);
  JINSN(branch, &end);
  JINSN(label, &lbl);
  CJval pool = JCONST(void_ptr, Vararg_pool);
  CJval arg[] = { pool, var };
  CALL_NATIVE(_mp_free, "vpp", arg);
  next_pc(j, instr->m_val2 + 1);
  JINSN(label, &end);
}

JIT_CODE(MkVararg) {
  CJval pool = JCONST(void_ptr, Vararg_pool);
  CJval arg[] = { pool };
  CJval var = CALL_NATIVE(_mp_alloc, "pp", arg);
  if(instr->m_val) {
    CJval reg = push_reg(j, -instr->m_val);
    CJval size = JCONST(nuint, instr->m_val);
    CJval arg[] = { size };
    CJval d = CALL_NATIVE(xmalloc, "pU", arg);
    JINSN(memcpy, d, reg, size);
    JSTORER(var, JOFF(Vararg, d), d);
  } else {
    if(*(m_uint*)instr->ptr)
      push_reg(j, -SZ_INT);
    CJval null = JCONST(void_ptr, 0);
    JSTORER(var, JOFF(Vararg, d), null);
  }
  const Vector kinds = (Vector)instr->m_val2;
  if(kinds) {
    const m_uint len = vector_size(kinds);
    if(len) {
      CJval s = JCONST(nuint, len);
      JSTORER(var, JOFF(Vararg, s), s);
      CJval size = JCONST(nuint, len * SZ_INT);
      CJval arg[] = { size };
      CJval k = CALL_NATIVE(xmalloc, "pU", arg);
      CJval ptr = JCONST(void_ptr, kinds->ptr + OFFSET);
      JINSN(memcpy, k, ptr, size);
      JSTORER(var, JOFF(Vararg, k), k);
    }
  } else {
    CJval zero = JCONST(nuint, 0);
    JSTORER(var, JOFF(Vararg, s), zero);
    CJval null = JCONST(void_ptr, 0);
    JSTORER(var, JOFF(Vararg, d), null);
  }
  CJval zero = JCONST(nuint, 0);
  JSTORER(var, JOFF(Vararg, o), zero);
  JSTORER(var, JOFF(Vararg, i), zero);
  CJval reg = push_reg(j, SZ_INT);
  JSTORER(reg, -SZ_INT, var);
}

JIT_CODE(Vararg_end) {
  push_reg(j, SZ_INT);
  CJval var  = get_vararg(j, instr);
  CJval d    = JLOADR(var, JOFF(Vararg, d), void_ptr);
  CJval o    = JLOADR(var, JOFF(Vararg, o), nuint);
  CJval k    = JLOADR(var, JOFF(Vararg, k), void_ptr);
  CJval i    = JLOADR(var, JOFF(Vararg, i), nuint);
  CJval s    = JLOADR(var, JOFF(Vararg, s), nuint);
  CJval size = JINSN(load_elem, k, i, jit_type_nuint);
  CJval sum  = JINSN(add, o, size);
  JSTORER(var, JOFF(Vararg, o), sum);
  CJval one  = JCONST(nuint, 1);
  CJval idx  = JINSN(add, i, one);
  JSTORER(var, JOFF(Vararg, i), idx);
  CJval cond = JINSN(lt, idx, s);
  INIT_LABEL(lbl);
  INIT_LABEL(end);
  JINSN(branch_if_not, cond, &lbl);
  next_pc(j, instr->m_val2);
  JINSN(branch, &end);
  JINSN(label, &lbl);
  CJval arg0[] = { d };
  CALL_NATIVE(xfree, "vp", arg0);
  CJval arg1[] = { k };
  CALL_NATIVE(xfree, "vp", arg1);
  CJval pool = JCONST(void_ptr, Vararg_pool);
  CJval arg2[] = { pool, var };
  CALL_NATIVE(_mp_free, "vpp", arg2);
  JINSN(label, &end);
}

JIT_CODE(Vararg_Member) {
  CJval var = get_vararg(j, instr);
  CJval reg    = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval data   = JLOADR(var, JOFF(Vararg, d), void_ptr);
  CJval offset = JLOADR(var, JOFF(Vararg, o), nuint);
  CJval val    = JINSN(add, data, offset);
  CJval len = JCONST(nuint, instr->m_val2);
  JINSN(memcpy, reg, val, len);
  push_reg(j, instr->m_val2);
}

JIT_CODE(varobj_assign) {
  CJval reg = push_reg(j, -SZ_INT);
  CJval obj = JLOADR(reg, -SZ_INT, void_ptr);
  JSTORER(reg, 0, obj);
}
*/
