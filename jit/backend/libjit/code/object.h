#include "jit/jit.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "code.h"
#include "ctrl.h"
#include "func.h"

JIT_CODE(ObjectAssign) {
  if(instr->m_val2 == 2) {
    if(instr->m_val)
      JSTORER(cc->reg, -SZ_INT, JADDR(cc->reg, -SZ_INT));
    return;
  }
  CJval reg = push_reg(cc, -SZ_INT);
  CJval lhs = JLOADR(reg, -SZ_INT, void_ptr);
  CJval ptr = JLOADR(reg, 0, void_ptr);
  if(!instr->m_val2) {
    CJval rhs = JLOADR(ptr, 0, void_ptr);
    INIT_LABEL(lbl)
    JINSN(branch_if_not, rhs, &lbl);
    CJval ref = JLOADR(rhs, JOFF(M_Object, ref), nuint);
    CJval one = JCONST(nuint, 1);
    CJval sum = JINSN(sub, ref, one);
    JSTORER(rhs, JOFF(M_Object, ref), sum);
    cc_release(cc, rhs);
    JINSN(label, &lbl);
  }
  if(instr->m_val)
    JSTORER(reg, -SZ_INT, JADDR(reg, -SZ_INT));
  JSTORER(ptr, 0, lhs);
}

#define jit_describe_logical(name, op)        \
JIT_CODE(name##Object) {                     \
  CJval reg = push_reg(cc, -SZ_INT);           \
  CJval lhs = JLOADR(reg, -SZ_INT, void_ptr); \
  CJval rhs = JLOADR(reg, 0, void_ptr);       \
  CJval ret = JINSN(op, lhs, rhs);           \
  CJval val = JINSN(convert, ret, jit_type_nuint, 0);           \
  JSTORER(reg, -SZ_INT, val);                 \
  cc_release2(cc, lhs);                         \
  cc_release2(cc, rhs);                         \
}
jit_describe_logical(Eq, eq)
jit_describe_logical(Neq, ne)

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_object(struct Jit* j) {
  JIT_IMPORT(ObjectAssign);
  JIT_IMPORT(EqObject);
  JIT_IMPORT(NeqObject);
}
