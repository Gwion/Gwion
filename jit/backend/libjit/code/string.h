#include <string.h>
#include "jit/jit.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"


// insn_memcmp instead of strcmp ?
#define jit_describe_string_logical(name, op)  \
JIT_CODE(String_##name) { GWDEBUG_EXE          \
  CJval reg = push_reg(cc, -SZ_INT);            \
  CJval lhs = JLOADR(reg, -SZ_INT, void_ptr);  \
  CJval rhs = JLOADR(reg, 0,       void_ptr);  \
  CJval null = JCONST(void_ptr, 0);            \
  CJval l = JINSN(eq, lhs, null);             \
  CJval r = JINSN(eq, rhs, null);             \
  CJval zero = JCONST(nuint, 0);               \
  CJval eq = JINSN(op, l, r);                 \
  INIT_LABEL(nok);                             \
  INIT_LABEL(end);                             \
  JINSN(branch_if_not, eq, &nok);             \
  JINSN(branch_if_not, lhs, &nok);                \
  CJval ldata = JLOADR(lhs, JOFF(M_Object, data), void_ptr);  \
  CJval rdata = JLOADR(rhs, JOFF(M_Object, data), void_ptr);  \
  CJval lstr = JLOADR(ldata, 0, void_ptr);  \
  CJval rstr = JLOADR(rdata, 0, void_ptr);  \
  CJval arg[] = { lstr, rstr };               \
  CJval ret = CALL_NATIVE(strcmp, "ipp", arg); \
  JINSN(branch_if, ret, &nok);            \
  CJval one = JCONST(nuint, 1);                \
  JSTORER(reg, -SZ_INT, one);                  \
  JINSN(branch, &end);                        \
  JINSN(label, &nok);                         \
  JSTORER(reg, -SZ_INT, zero);                 \
  JINSN(label, &end);                         \
  cc_release(cc, lhs); \
  cc_release(cc, rhs); \
}
jit_describe_string_logical(eq, eq)
jit_describe_string_logical(neq, ne)

JIT_CODE(RegPushStr) {
  CJval str = JCONST(void_ptr, instr->m_val);
  CJval arg[] = { cc->shred, str };
  CJval obj = CALL_NATIVE(new_string2, "ppp", arg);
  JSTORER(cc->reg, 0, obj);
  push_reg(cc, SZ_INT);
}

JIT_CODE(String_Assign) {
  CJval reg = push_reg(cc, -SZ_INT);
  CJval lhs = JLOADR(reg, -SZ_INT, void_ptr);
  CJval rhs = JLOADR(reg, 0, void_ptr);
  cc_release2(cc, lhs);
  cc_check(cc, rhs, "NullStringException");
  Jval str = JCONST(void_ptr, "");
  INIT_LABEL(lbl)
  JINSN(branch_if_not, lhs, &lbl);
  CJval data = JLOADR(lhs, JOFF(M_Object, data), void_ptr);
  str = JLOADR(data, 0, void_ptr);
  JINSN(label, &lbl);
  CJval arg[] = { cc->shred, rhs, str };
  CALL_NATIVE(push_string, "vppp", arg);
}

#define JIT_IMPORT(a) jit_code_import(j, a ,jitcode_##a);
void jit_code_import_string(struct Jit* j) {
  JIT_IMPORT(String_eq)
  JIT_IMPORT(String_neq)
  JIT_IMPORT(RegPushStr)
  JIT_IMPORT(String_Assign)
}
