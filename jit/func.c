#include <stdlib.h>
#include <string.h>
#include <jit/jit.h>
#include "defs.h"
#include "absyn.h"
#include "operator.h"
#include "gwjit_common.h"
#include "cc.h"
#include "q.h"
#include "sig.h"
#include "code.h"
#include "ctrl.h"
#include "instr.h"
#define JIT_CALL JIT_CALL_NOTHROW

ANN Jval push_reg(JitCC* const j, const m_uint i) {
  CJval ptr = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval reg = JADDR(ptr, i);
  JSTORER(j->shred, JOFF(VM_Shred, reg), reg);
  return reg;
}

ANN inline void next_pc2(JitCC* const j, CJval v) {
  JSTORER(j->shred, JOFF(VM_Shred, pc), v);
}

ANN void next_pc(JitCC* const j, const m_uint index) {
  CJval v = JCONST(nuint, index);
  next_pc2(j, v);
}

ANN void cc_release(JitCC *const j, CJval  obj) {
  Jval args[] = { obj, j->shred };
  CALL_NATIVE(release, "vpp", args);
}

ANN void cc_shredule(JitCC *const j, CJval wake_time) {
  CJval vm = JLOADR(j->shred, JOFF(VM_Shred, vm_ref), void_ptr);
  CJval shreduler = JLOADR(vm, __builtin_offsetof(VM, shreduler), void_ptr);
  Jval args[] = { shreduler, j->shred, wake_time };
  CALL_NATIVE(shredule, "vppf", args);
}

ANN void cc_remove(JitCC *const j, const m_bool b) {
  CJval vm = JLOADR(j->shred, JOFF(VM_Shred, vm_ref), void_ptr);
  CJval shreduler = JLOADR(vm, __builtin_offsetof(VM, shreduler), void_ptr);
  CJval rem = JCONST(int, b);
  Jval args[] = { shreduler, j->shred, rem };
  CALL_NATIVE(shreduler_remove, "vppi", args);
}

ANN void cc_except(JitCC *const j, const m_str c) {
  CJval str = JCONST(void_ptr, (m_uint)c);
  Jval args[] = { j->shred, str };
  CALL_NATIVE(exception, "vpp", args);
  JINSN(default_return);
}

ANN Jval cc_get_flag(JitCC *const j, CJval a, ae_flag flag) {
  CJval b = JCONST(nuint, flag);
  CJval c = JINSN(and, a, b);
  return JINSN(eq, c, b);
}

m_str safe_string(M_Object a) {
  return a ? STRING(a) : NULL;
}
