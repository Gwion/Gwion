#include <string.h>
#include "jit/jit.h"
#include "gwjit_common.h"
#include "cc.h"
#include "q.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"
/*
JIT_CODE(Event_Wait) {
  CJval reg = push_reg(j, -SZ_FLOAT);
  CJval obj = JLOADR(reg, -SZ_INT, void_ptr);
  CJval null = JCONST(void_ptr, 0);
  CJval cond = JINSN(eq, obj, null);
  INIT_LABEL(lbl);
  JINSN(branch_if_not, cond, &lbl);
  cc_except(j, "NullEventWait");
  JINSN(label, &lbl);
  cc_remove(j, 0);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval vec = JLOADR(data, 0, void_ptr);
  CJval arg[] = { vec, j->shred };
  CALL_NATIVE(vector_add, "vpp", arg);
  CJval one = JCONST(nint, 1);
  JSTORER(reg, -SZ_INT, one);
  cc_release(j, obj);
}
*/
