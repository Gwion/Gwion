#include "jit/jit.h"
#include "gwjit_common.h"
#include "cc.h"
#include "q.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"
/*
JIT_CODE(instr_ptr_assign) {
  CJval reg = push_reg(j, -SZ_INT*2);
  CJval val = JLOADR(reg, -SZ_INT, void_ptr);
  CJval obj = JLOADR(reg, 0, void_ptr);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  JSTORER(data, 0, val);
}

JIT_CODE(instr_ptr_deref) {
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval obj = JLOADR(reg, -SZ_INT, void_ptr);
  CJval ptr = JADDR(reg, -SZ_INT);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  if(instr->m_val2) {
    CJval len = JCONST(nuint, SZ_INT);
    JINSN(memcpy, ptr, data, len);
  } else {
    CJval val = JLOADR(data, 0, void_ptr);
    CJval len = JCONST(nuint, instr->m_val);
    JINSN(memcpy, ptr, val, len);
  }
}
JIT_CODE(Cast2Ptr) {
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval val = JLOADR(reg, -SZ_INT, void_ptr);
  CJval arg0[] = { j->shred };
  CJval obj = CALL_NATIVE(new_M_Object, "pp", arg0);
  CJval len = JCONST(nuint, SZ_INT);
  CJval arg1[] = { len };
  CJval data = CALL_NATIVE(xmalloc, "pU", arg1);
  JSTORER(data, 0, val);
  JSTORER(reg, -SZ_INT, obj);
}
*/
