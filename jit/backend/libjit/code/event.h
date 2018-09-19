#include <string.h>
#include "jit/jit.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"

JIT_CODE(Event_Wait) {
  CJval reg = push_reg(cc, -SZ_FLOAT);
  CJval obj = JLOADR(reg, -SZ_INT, void_ptr);
  cc_check(cc, obj, "NullEventWait");
  cc_remove(cc, 0);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval vec = JLOADR(data, 0, void_ptr);
  jit_vector_add(cc, vec, cc->shred);
  CJval one = JCONST(nint, 1);
  JSTORER(reg, -SZ_INT, one);
  cc_release(cc, obj);
}

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_event(struct Jit* j) {
  JIT_IMPORT(Event_Wait);
}
