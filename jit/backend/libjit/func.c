#include <stdlib.h>
#include <string.h>
#include <jit/jit.h>
#include "defs.h"
#include "absyn.h"
#include "operator.h"
#include "instr.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "thread.h"
#include "code.h"
#include "ctrl.h"
#define JIT_CALL JIT_CALL_NOTHROW

ANN void cc_free(void* v) { xfree(v); }

ANN Jval push_reg(CC const cc, const m_int i) {
  CJval ptr = JLOADR(cc->shred, JOFF(VM_Shred, reg), void_ptr);
  cc->reg = JADDR(ptr, i);
//  cc->reg = JADDR(cc->reg, i);
  JSTORER(cc->shred, JOFF(VM_Shred, reg), cc->reg);
  return cc->reg;
}

ANN void push_reg2(CC const cc, CJval i) {
//  CJval ptr = JLOADR(cc->shred, JOFF(VM_Shred, reg), void_ptr);
//  cc->reg = JINSN(add, ptr, i);
  cc->reg = JINSN(add, cc->reg, i);
  JSTORER(cc->shred, JOFF(VM_Shred, reg), cc->reg);
}

ANN void next_pc(CC cc, const m_uint index) {
  CJval v = JCONST(nuint, index);
  JSTORER(cc->shred, JOFF(VM_Shred, pc), v);
}

ANN void cc_release(CC cc, CJval  obj) {
  Jval ref = JLOADR(obj, JOFF(M_Object, ref), nuint);
  ref = JINSN(sub, ref, JCONST(nuint, 1));
  JSTORER(obj, JOFF(M_Object, ref), ref);
  INIT_LABEL(lbl);
  JINSN(branch_if, ref, &lbl);
  Jval args[] = { obj, cc->shred };
  CALL_NATIVE2(__release, "vpp", args);
  JINSN(label, &lbl);
}

ANN void cc_release2(CC cc, CJval  obj) {
  CJval null = JCONST(void_ptr, 0);
  CJval cond = JINSN(eq, obj, null);
  INIT_LABEL(lbl);
  JINSN(branch_if, cond, &lbl);
  cc_release(cc, obj);
  JINSN(label, &lbl);
}

ANN void cc_shredule(CC cc, CJval wake_time) {
  CJval vm = JLOADR(cc->shred, JOFF(VM_Shred, vm_ref), void_ptr);
  CJval shreduler = JLOADR(vm, __builtin_offsetof(VM, shreduler), void_ptr);
  Jval args[] = { shreduler, cc->shred, wake_time };
  CALL_NATIVE2(shredule, "vppf", args);
}

ANN void cc_remove(CC cc, const m_bool b) {
  CJval vm = JLOADR(cc->shred, JOFF(VM_Shred, vm_ref), void_ptr);
  CJval shreduler = JLOADR(vm, __builtin_offsetof(VM, shreduler), void_ptr);
  CJval rem = JCONST(int, b);
  Jval args[] = { shreduler, cc->shred, rem };
  CALL_NATIVE2(shreduler_remove, "vppi", args);
}

ANN void cc_except(CC const cc, const m_str c) {
  CJval str = JCONST(void_ptr, (m_uint)c);
  Jval args[] = { cc->shred, str };
  CALL_NATIVE2(exception, "vpp", args);
  jit_insn_default_return(cc->f);
}

ANN void cc_check(const CC cc, CJval v, const m_str c) {
  CJval nil = JCONST(nuint, 0);
  INIT_LABEL(lbl);
  CJval cond = JINSN(eq, v, nil);
  JINSN(branch_if_not, cond, &lbl);
  cc_except(cc, c);
  JINSN(label, &lbl);
}

ANN Jval cc_get_flag(CC cc, CJval a, ae_flag flag) {
  CJval b = JCONST(nuint, flag);
  CJval c = JINSN(and, a, b);
  return JINSN(eq, c, b);
}
#include "dlfcn.h"
Jval cc_call(CC cc, const m_str name, const m_str s, Jval *arg) {
  const void* func = dlsym(NULL, name);
  Jval f = JCONST(void_ptr, func);
  return jit_insn_call_indirect_vtable(cc->f, f,
    sig(&cc->sig, s, jit_abi_fastcall), arg, strlen(s) - 1, JIT_CALL);
}

ANN void jit_vector_add(CC cc, CJval v, CJval data) {
  CJval offset = JCONST(nuint, OFFSET);
  Jval ptr = JLOADR(v, JOFF(Vector, ptr), void_ptr);
  Jval len = JLOADE(ptr, JCONST(nuint, 0), nuint);
  Jval cap = JLOADE(ptr, JCONST(nuint, 1), nuint);
  INIT_LABEL(lbl);
  CJval sub = JINSN(sub, cap , len);
  CJval cond = JINSN(eq, sub, offset);
  JINSN(branch_if_not, cond, &lbl);
  cap = JINSN(mul, cap , JCONST(nuint, 2));
  CJval size = JINSN(mul, cap, JCONST(nuint, SZ_INT));
  CJval arg[] = { ptr, size };
  CJval nptr = CALL_NATIVE2(xrealloc, "ppU", arg);
//  JSTORER(nptr, SZ_INT, cap);
  JSTOREE(nptr, JCONST(nuint, 1), cap);
  JSTORER(v, JOFF(Vector, ptr), nptr);
  JINSN(label, &lbl);

  CJval _ptr = JLOADR(v, JOFF(Vector, ptr), void_ptr);
  Jval idx = JINSN(add, len, offset);
  JSTOREE(_ptr, idx, data);
// ++len
  CJval one = JCONST(nuint, 1);
  len = JINSN(add, len, one);
//  JSTORER(_ptr, 0, len);
  JSTOREE(_ptr, JCONST(nuint, 0), len);
//  JSTORER(_ptr, 0, len);
//  CJval arg[] = { v, data };
//  CALL_NATIVE2(vector_add, "vpp", arg);
//  CALL_NATIVE(vector_add, "vpp", arg);
}
