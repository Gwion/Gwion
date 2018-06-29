#include "jit/jit.h"
#include "gwjit_common.h"
#include "cc.h"
#include "q.h"
#include "sig.h"
#include "code.h"
#include "ctrl.h"
#include "func.h"
/*
JIT_CODE(Assign_Object) {
  CJval reg = push_reg(j, -SZ_INT);
  CJval lhs = JLOADR(reg, -SZ_INT, void_ptr);
  CJval ptr = JLOADR(reg, 0, void_ptr);
  CJval rhs = JLOADR(ptr, 0, void_ptr);
  cc_release(j, rhs);
  cc_release(j, rhs);
  if(instr->m_val)
    JSTORER(reg, -SZ_INT, lhs);
  JSTORER(ptr, 0, lhs);
}

#define jit_describe_logical(name, op)        \
JIT_CODE(name##_Object) {                     \
  CJval reg = push_reg(j, -SZ_INT);           \
  CJval lhs = JLOADR(reg, -SZ_INT, void_ptr); \
  CJval rhs = JLOADR(reg, 0, void_ptr);       \
  CJval ret = JINSN(op, lhs, rhs);           \
  CJval val = JINSN(convert, ret, jit_type_nuint, 0);           \
  JSTORER(reg, -SZ_INT, val);                 \
  cc_release(j, lhs);                         \
  cc_release(j, rhs);                         \
}
jit_describe_logical(eq, eq)
jit_describe_logical(neq, ne)
*/
