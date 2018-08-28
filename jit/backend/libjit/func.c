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

ANN Jval push_reg(JitCC* const cc, const m_int i) {
  CJval ptr = JLOADR(cc->shred, JOFF(VM_Shred, reg), void_ptr);
  cc->reg = JADDR(ptr, i);
  JSTORER(cc->shred, JOFF(VM_Shred, reg), cc->reg);
  return cc->reg;
}

ANN void push_reg2(JitCC* const cc, CJval i) {
  CJval ptr = JLOADR(cc->shred, JOFF(VM_Shred, reg), void_ptr);
  cc->reg = JINSN(add, ptr, i);
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
  CALL_NATIVE(__release, "vpp", args);
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
  CALL_NATIVE(shredule, "vppf", args);
}

ANN void cc_remove(CC cc, const m_bool b) {
  CJval vm = JLOADR(cc->shred, JOFF(VM_Shred, vm_ref), void_ptr);
  CJval shreduler = JLOADR(vm, __builtin_offsetof(VM, shreduler), void_ptr);
  CJval rem = JCONST(int, b);
  Jval args[] = { shreduler, cc->shred, rem };
  CALL_NATIVE(shreduler_remove, "vppi", args);
}

ANN void cc_except(JitCC *const cc, const m_str c) {
  CJval str = JCONST(void_ptr, (m_uint)c);
  Jval args[] = { cc->shred, str };
  CALL_NATIVE(exception, "vpp", args);
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

m_str safe_string(M_Object a) {
  return a ? STRING(a) : NULL;
}

Jval cc_call(CC cc, void* func, const m_str s, Jval *arg) {
  Jval f = (Jval)map_get(&cc->vtable, (vtype)func);
  if(!f) {
    f = JCONST(void_ptr, func);
    map_set(&cc->vtable, (vtype)func, (vtype)f);
  }
  return jit_insn_call_indirect_vtable(cc->f, f,
    sig(&cc->sig, s, jit_abi_fastcall), arg, strlen(s) - 1, JIT_CALL);
}


ANN void jit_vector_add(CC cc, CJval v, CJval data) {
  CJval offset = JCONST(nuint, OFFSET);
  Jval ptr = JLOADR(v, JOFF(Vector, ptr), void_ptr);
  Jval len = JINSN(load_elem, ptr, JCONST(nuint, 0), jit_type_nuint);
  Jval cap = JINSN(load_elem, ptr, JCONST(nuint, 1), jit_type_nuint);
  INIT_LABEL(lbl);
  CJval sub = JINSN(sub, cap , len);
  CJval cond = JINSN(eq, sub, offset);
  JINSN(branch_if_not, cond, &lbl);
  cap = JINSN(mul, cap , JCONST(nuint, 2));
  CJval size = JINSN(mul, cap, JCONST(nuint, SZ_INT));
  CJval arg[] = { ptr, size };
  CJval nptr = CALL_NATIVE(xrealloc, "ppU", arg);
  JSTORER(nptr, SZ_INT, cap);
  JSTORER(v, JOFF(Vector, ptr), nptr);
  JINSN(label, &lbl);

  CJval _ptr = JLOADR(v, JOFF(Vector, ptr), void_ptr);
  Jval idx = JINSN(add, len, offset);
  JINSN(store_elem, _ptr, idx, data);
// ++len
  CJval one = JCONST(nuint, 1);
  len = JINSN(add, len, one);
  JSTORER(_ptr, 0, len);
}
