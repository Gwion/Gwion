#include "jit/jit.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"

JIT_CODE(instr_ptr_assign) {
  push_reg(cc, -SZ_INT*2);
  CJval val = JLOADR(cc->reg, -SZ_INT, void_ptr);
  CJval obj = JLOADR(cc->reg, 0, void_ptr);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  JSTORER(data, 0, val);
}

JIT_CODE(instr_ptr_deref) {
  CJval obj = JLOADR(cc->reg, -SZ_INT, void_ptr);
  CJval ptr = JADDR(cc->reg, -SZ_INT);
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
  CJval val = JLOADR(cc->reg, -SZ_INT, void_ptr);
  CJval arg0[] = { cc->shred, t_ptr };
//  CJval obj = CALL_NATIVE2(new_object, "pp", arg0);
  CJval obj = jit_insn_call(cc->f, "jit_new_object",
    get_jit_func(cc, "jit_new_object"), 0, arg0, 2, JIT_CALL);
  CJval data = JLOADR(obj, JOFF(M_Object , data), void_ptr);
  JSTORER(data, 0, val);
  JSTORER(cc->reg, -SZ_INT, obj);
}

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_ptr(struct Jit* j) {
  JIT_IMPORT(instr_ptr_assign)
  JIT_IMPORT(instr_ptr_deref)
  JIT_IMPORT(Cast2Ptr)
}
