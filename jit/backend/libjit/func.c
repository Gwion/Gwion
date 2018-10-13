#include <stdlib.h>
#include <string.h>
#include <jit/jit.h>
#include "defs.h"
#include "absyn.h"
#include "type.h"
#include "instr.h"
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "sig.h"
#include "thread.h"
#include "code.h"
#include "ctrl.h"
#include "func.h"
#include "object.h"
#include "mpool.h"
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
#include <stdio.h>
ANN void next_pc(CC cc, const m_uint index) {
/*  jit_label_t* lbl = (jit_label_t*)map_get(&cc->label, 
index);
  if(lbl)
    JINSN(branch, lbl);
  else */{ //exit(2);
    CJval v = JCONST(nuint, index);
    JSTORER(cc->shred, JOFF(VM_Shred, pc), v);
  }
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
  CJval shreduler = JLOADR(cc->vm, __builtin_offsetof(VM, shreduler), void_ptr);
  Jval args[] = { shreduler, cc->shred, wake_time };
  CALL_NATIVE(shredule, "vppf", args);
}

ANN void cc_remove(CC cc, const m_bool b) {
  CJval shreduler = JLOADR(cc->vm, __builtin_offsetof(VM, shreduler), void_ptr);
  CJval rem = JCONST(int, b);
  Jval args[] = { shreduler, cc->shred, rem };
  CALL_NATIVE(shreduler_remove, "vppi", args);
}

ANN void cc_except(CC const cc, const m_str c) {
  CJval str = JCONST(void_ptr, (m_uint)c);
  Jval args[] = { cc->shred, str };
  CALL_NATIVE(exception, "vpp", args);
  jit_insn_default_return(cc->f);
}

ANN void cc_check(const CC cc, CJval v, const m_str c) {
  INIT_LABEL(lbl);
  JINSN(branch_if, v, &lbl);
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
  CJval nptr = CALL_NATIVE(xrealloc, "ppU", arg);
  JSTOREE(nptr, JCONST(nuint, 1), cap);
  JSTORER(v, JOFF(Vector, ptr), nptr);
  JINSN(label, &lbl);

  CJval _ptr = JLOADR(v, JOFF(Vector, ptr), void_ptr);
  Jval idx = JINSN(add, len, offset);
  JSTOREE(_ptr, idx, data);
  CJval one = JCONST(nuint, 1);
  len = JINSN(add, len, one);
  JSTOREE(_ptr, JCONST(nuint, 0), len);
}

#include "mpool_private.h"
static void jit_mp_alloc_inner2(CC cc, CJval p) {
  Jval i = JLOADR(p, __builtin_offsetof(struct pool, nblk), uint);
  CJval nblk = JINSN(shl, i, JCONST(uint, 1));
  JSTORER(p, __builtin_offsetof(struct pool, nblk), nblk);
  CJval sz  = JINSN(mul, JCONST(nuint, sizeof(uint8_t*)), nblk);
  Jval data = JLOADR(p, __builtin_offsetof(struct pool, data), void_ptr);
  CJval xrealloc_arg[] = { data, sz };
  data = CALL_NATIVE(xrealloc, "ppU", xrealloc_arg);
  JSTORER(p, __builtin_offsetof(struct pool, data), data);
  INIT_LABEL(lbl);
  JINSN(label, &lbl);
  JSTOREE(data, i, JCONST(void_ptr, NULL));
  i = JINSN(add, i, JCONST(uint, 1));
  CJval cond = JINSN(lt, i, nblk);
  JINSN(branch_if, cond, &lbl);
}

static void jit_mp_alloc_inner(CC cc, CJval p) {
  JSTORER(p, __builtin_offsetof(struct pool, obj_id), JCONST(uint, 0));
  Jval blk_id = JLOADR(p, __builtin_offsetof(struct pool, blk_id), int);
  blk_id = JINSN(add, blk_id, JCONST(int, 1));
  JSTORER(p, __builtin_offsetof(struct pool, blk_id), blk_id);
  INIT_LABEL(lbl);
  CJval nblk = JLOADR(p, __builtin_offsetof(struct pool, nblk), uint);
  CJval cond = JINSN(eq, blk_id, nblk);
  JINSN(branch_if_not, cond, &lbl);
  jit_mp_alloc_inner2(cc, p);
  JINSN(label, &lbl);
  INIT_LABEL(alloc);
  CJval data = JLOADR(p, __builtin_offsetof(struct pool, data), void_ptr);
  Jval blk = JLOADE(data, blk_id, void_ptr);
  JINSN(branch_if, blk, &alloc);
  CJval obj_sz = JLOADR(p, __builtin_offsetof(struct pool, obj_sz), uint);
  CJval xcalloc_arg[] = { JCONST(nuint, BLK), obj_sz };
  blk = CALL_NATIVE(xcalloc, "pUU", xcalloc_arg);
//  JSTOREE(data, blk_id, ptr);
  JSTOREE(data, blk_id, blk);
  JINSN(label, &alloc);
}

/*int*/ void jit_mp_alloc2(const CC cc) {
  CJval p = jit_value_get_param(cc->f, 0);
  CJval next = JLOADR(p, __builtin_offsetof(struct pool, next), void_ptr);
  CJval obj_sz = JLOADR(p, __builtin_offsetof(struct pool, obj_sz), uint);
  INIT_LABEL(recycle);
  JINSN(branch_if_not, next, &recycle);
  CJval next_next = JLOADR(next, __builtin_offsetof(struct Recycle, next), void_ptr);
//  CJval next_next = JLOADR(next, 0, next);
  JSTORER(p, __builtin_offsetof(struct pool, next), next_next);
  JINSN(memset, next, JCONST(nuint, 0), obj_sz);
  jit_insn_return(cc->f, next);
  JINSN(label, &recycle);
  Jval obj_id = JLOADR(p, __builtin_offsetof(struct pool, obj_id), uint);
  obj_id = JINSN(add, obj_id, JCONST(uint, 1));
  JSTORER(p, __builtin_offsetof(struct pool, obj_id), obj_id);
  INIT_LABEL(alloc);
  CJval alloc_cond = JINSN(eq, obj_id, JCONST(uint, BLK));
  JINSN(branch_if_not, alloc_cond, &alloc);
  jit_mp_alloc_inner(cc, p);
  JINSN(label, &alloc);

  Jval blk_id = JLOADR(p, __builtin_offsetof(struct pool, blk_id), int);
  CJval data = JLOADR(p, __builtin_offsetof(struct pool, data), void_ptr);
  Jval blk = JLOADE(data, blk_id, void_ptr);
  obj_id = JLOADR(p, __builtin_offsetof(struct pool, obj_id), uint);
  CJval mul = JINSN(mul, obj_id, obj_sz);
  CJval ret = JINSN(add, blk, mul);
  jit_insn_return(cc->f, ret);
}

// find correct header
jit_function_t get_jit_func(CC cc, const m_str s);

Jval jit_mp_alloc(CC cc, const m_uint size) {
//exit(2);
  struct pool* p = mp_ini(size);
  Jval arg[] = { JCONST(void_ptr, p) };
  return jit_insn_call(cc->f, "jit_mp_alloc",
    get_jit_func(cc, "jit_mp_alloc"), 0, arg, 1, JIT_CALL);
}


//void jit_mp_free2(jit_function f) {
void jit_mp_free2(CC cc, CJval p, CJval ptr) {
  CJval next = JLOADR(p, __builtin_offsetof(struct pool, next), void_ptr);
  JSTORER(p, __builtin_offsetof(struct pool, next), ptr);
  JSTORER(ptr, __builtin_offsetof(struct Recycle, next), next);
}

void jit_mp_free(CC cc, const m_uint size, CJval ptr) {
  struct pool* p = mp_ini(size);
  jit_mp_free2(cc, JCONST(void_ptr, p), ptr);
}

void jit_new_object(CC cc) {
  CJval shred = jit_value_get_param(cc->f, 0);
  Jval t = jit_value_get_param(cc->f, 1);
  CJval a = jit_mp_alloc(cc, sizeof(struct M_Object_));
  JSTORER(a, JOFF(M_Object, ref), JCONST(nuint, 1));
  JSTORER(a, JOFF(M_Object, type_ref), t);
  CJval nspc = JLOADR(t, JOFF(Type, nspc), void_ptr);
  CJval offset = JLOADR(nspc, JOFF(Nspc, offset), nuint);
  INIT_LABEL(lbl);
  JINSN(branch_if_not, offset, &lbl);
  INIT_LABEL(loop);
  INIT_LABEL(alloc);
  Jval type = JINSN(dup, t);
  JINSN(label, &loop);
  Jval p = JLOADR(type, JOFF(Type, p), void_ptr);
  JINSN(branch_if, p, &alloc);
  type = JLOADR(type, JOFF(Type, parent), void_ptr);
  JINSN(branch_if, type, &loop);
  JINSN(label, &alloc);
  JSTORER(a, JOFF(M_Object, p), p);
  Jval arg[] = { p };
  CJval data = jit_insn_call(cc->f, "jit_mp_alloc",
    get_jit_func(cc, "jit_mp_alloc"), 0, arg, 1, JIT_CALL);
  JSTORER(a, JOFF(M_Object, data), data);
  JINSN(label, &lbl);

  INIT_LABEL(gc);
  JINSN(branch_if_not, shred, &gc);
  CJval _gc = JADDR(shred, JOFF(VM_Shred, gc));
  jit_vector_add(cc, _gc, a);
  JINSN(label, &gc);
  jit_insn_return(cc->f, a);
}

ANN void cc_add2gc(CC cc, CJval obj) {
  CJval gc = JADDR(cc->shred, JOFF(VM_Shred, gc));
  jit_vector_add(cc, gc, obj);
}
/*
ANN Jval cc_inc(CC cc, Jval val) {
  return JINSN(add, val, JCONST(nuint, 1));
} // auto loop end int_pre/post

ANN Jval cc_dec(CC cc, Jval val) {
  return JINSN(add, val, JCONST(nint, -1));
} // dec int addr object assign varargEnd
*/
ANN void jit_type_add_ref(CC cc, CJval type) {
  CJval oo = JLOADR(type, JOFF(Type, obj), void_ptr);
  CJval ref = JINSN(add, oo, JCONST(nuint, 1));
  JSTORER(type, JOFF(Type, obj), ref);
}

