#include <string.h>
#include "jit/jit.h"
#include "gwjit_common.h"
#include "cc.h"
#include "q.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"
/*
#define jit_describe_string_logical(name, op)  \
JIT_CODE(String_##name) { GWDEBUG_EXE          \
  CJval reg = push_reg(j, -SZ_INT);            \
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
  CJval has = JINSN(eq, lhs, null);             \
  JINSN(branch_if, has, &nok);                \
  CJval ldata = JLOADR(lhs, JOFF(M_Object, data), void_ptr);  \
  CJval rdata = JLOADR(rhs, JOFF(M_Object, data), void_ptr);  \
  CJval lstr = JLOADR(ldata, 0, void_ptr);  \
  CJval rstr = JLOADR(rdata, 0, void_ptr);  \
  CJval arg[] = { lstr, rstr };               \
  CJval ret = CALL_NATIVE(strcmp, "ipp", arg); \
  CJval _eq = JINSN(op, ret, zero);           \
  JINSN(branch_if_not, _eq, &nok);            \
  CJval one = JCONST(nuint, 1);                \
  JSTORER(reg, -SZ_INT, one);                  \
  JINSN(branch, &end);                        \
  JINSN(label, &nok);                         \
  JSTORER(reg, -SZ_INT, zero);                 \
  JINSN(label, &end);                         \
  CJval arg0[] = { lhs, j->shred };            \
  CALL_NATIVE(release, "vpp", arg0);           \
  CJval arg1[] = { rhs, j->shred };            \
  CALL_NATIVE(release, "vpp", arg1);           \
}

/*
#define jit_describe_string_logical(op)        \
JIT_CODE(String_##op) { GWDEBUG_EXE            \
  CJval reg = push_reg(j, -SZ_INT);            \
  CJval lhs = JLOADR(reg, -SZ_INT, void_ptr);  \
  CJval rhs = JLOADR(reg, 0,       void_ptr);  \
  CJval null = JCONST(void_ptr, 0);            \
  CJval zero = JCONST(nuint, 0);               \
CJval l1 = JINSN(convert, lhs, jit_type_nuint, 0); \
CJval r1 = JINSN(convert, rhs, jit_type_nuint, 0); \
  CJval l = JINSN(eq, l1, zero);             \
  CJval r = JINSN(eq, r1, zero);             \
  CJval eq = JINSN(op, l, r);                 \
CJval ret = JINSN(convert, eq, jit_type_nuint, 0); \
  JSTORER(reg, -SZ_INT, ret);                 \
}
*/
//jit_describe_string_logical(eq, eq)
//jit_describe_string_logical(neq, ne)



JIT_CODE(Reg_Push_Str) {
  CJval reg = push_reg(j, SZ_INT);
  CJval str = JCONST(void_ptr, instr->m_val);
  CJval arg[] = { j->shred, str };
  CJval obj = CALL_NATIVE(new_String, "ppp", arg);
  JSTORER(reg, -SZ_INT, obj);
}
