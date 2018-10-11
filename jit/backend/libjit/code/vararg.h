#include "jit/jit.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "code.h"
#include "code/func.h"

ANN static inline Jval get_vararg(CC cc, const Instr instr) {
  CJval mem  = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  return JLOADR(mem, instr->m_val, void_ptr);
}

JIT_CODE(VarargTop) {
  CJval var = get_vararg(cc, instr);
  CJval data = JLOADR(var, JOFF(Vararg, d), void_ptr);
  INIT_LABEL(lbl);
  INIT_LABEL(end);
  JINSN(branch_if_not, data, &lbl);
  push_reg(cc, SZ_INT);
  JINSN(branch, &end);
  JINSN(label, &lbl);
  CJval pool = JCONST(nuint, sizeof(struct Vararg_));
  CJval arg[] = { pool, var };
  CALL_NATIVE2(_mp_free, "vpp", arg);
  next_pc(cc, instr->m_val2 + 1);
  jit_insn_default_return(cc->f);
  JINSN(label, &end);
}

JIT_CODE(VarargIni) {
  CJval pool = JCONST(nuint, sizeof(struct Vararg_));
  CJval arg[] = { pool };
  CJval var = CALL_NATIVE2(_mp_alloc, "pp", arg);
//  CJval var = jit_mp_alloc(cc, sizeof(struct Vararg_));
  if(instr->m_val) {
    push_reg(cc, -instr->m_val);
    CJval size = JCONST(nuint, instr->m_val);
    CJval arg[] = { size };
    CJval d = CALL_NATIVE2(xmalloc, "pU", arg);
    JINSN(memcpy, d, cc->reg, size);
    JSTORER(var, JOFF(Vararg, d), d);
  } else {
    if(*(m_uint*)instr->ptr)
      push_reg(cc, -SZ_INT);
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
      CJval k = CALL_NATIVE2(xmalloc, "pU", arg);
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
  push_reg(cc, SZ_INT);
  JSTORER(cc->reg, -SZ_INT, var);
}

JIT_CODE(VarargEnd) {
  push_reg(cc, SZ_INT);
  CJval var  = get_vararg(cc, instr);
  CJval d    = JLOADR(var, JOFF(Vararg, d), void_ptr);
  CJval o    = JLOADR(var, JOFF(Vararg, o), nuint);
  CJval k    = JLOADR(var, JOFF(Vararg, k), void_ptr);
  CJval i    = JLOADR(var, JOFF(Vararg, i), nuint);
  CJval s    = JLOADR(var, JOFF(Vararg, s), nuint);
  CJval size = JLOADE(k, i, nuint);
  CJval sum  = JINSN(add, o, size);
  JSTORER(var, JOFF(Vararg, o), sum);
  CJval one  = JCONST(nuint, 1);
  CJval idx  = JINSN(add, i, one);
  JSTORER(var, JOFF(Vararg, i), idx);
  CJval cond = JINSN(lt, idx, s);
  INIT_LABEL(lbl);
  INIT_LABEL(end);
  JINSN(branch_if_not, cond, &lbl);
  next_pc(cc, instr->m_val2);
  JINSN(branch, &end);
  JINSN(label, &lbl);
  CJval arg0[] = { d };
  CALL_NATIVE2(cc_free, "vp", arg0);
  CJval arg1[] = { k };
  CALL_NATIVE2(cc_free, "vp", arg1);
  CJval pool = JCONST(nuint, sizeof(struct Vararg_));
  CJval arg2[] = { pool, var };
  CALL_NATIVE2(_mp_free, "vpp", arg2);
  JINSN(label, &end);
}

JIT_CODE(VarargMember) {
  CJval var = get_vararg(cc, instr);
  CJval data   = JLOADR(var, JOFF(Vararg, d), void_ptr);
  CJval offset = JLOADR(var, JOFF(Vararg, o), nuint);
  CJval val    = JINSN(add, data, offset);
  CJval len = JCONST(nuint, instr->m_val2);
  JINSN(memcpy, cc->reg, val, len);
  push_reg(cc, instr->m_val2);
}

JIT_CODE(VarargAssign) {
  push_reg(cc, -SZ_INT);
  CJval obj = JLOADR(cc->reg, -SZ_INT, void_ptr);
  JSTORER(cc->reg, 0, obj);
}

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_vararg(struct Jit* j) {
  JIT_IMPORT(VarargTop);
  JIT_IMPORT(VarargIni);
  JIT_IMPORT(VarargEnd);
  JIT_IMPORT(VarargMember);
  JIT_IMPORT(VarargAssign);
}
