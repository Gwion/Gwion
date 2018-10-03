#include <jit/jit.h>
#include "gwjit_common.h"
#include "jitter.h"
#include "cc.h"
#include "code.h"
#include "sig.h"
#include "code/func.h"

JIT_CODE(EOC) {
  CJval shreduler = JLOADR(cc->vm, JOFF(VM, shreduler), void_ptr);
  CJval arg[] = { shreduler, cc->shred, JCONST(int, 1) };
  CALL_NATIVE2(shreduler_remove, "vppi", arg);
}

JIT_CODE(DTOR_EOC) {
  CJval mem  = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval me   = JLOADR(cc->shred, JOFF(VM_Shred, me), void_ptr);
  CJval obj  = JLOADR(mem, 0, void_ptr);
  CJval type = JLOADR(obj, JOFF(M_Object, type_ref), void_ptr);
  CJval parent = JLOADR(type, JOFF(Type, parent), void_ptr);
  JSTORER(obj, JOFF(M_Object, type_ref), parent);
  CJval one = JCONST(nuint, 1);
  JSTORER(obj, JOFF(M_Object, ref), one);
  cc_release(cc, obj);
  cc_release(cc, me);
  CJval shreduler = JLOADR(cc->vm, JOFF(VM, shreduler), void_ptr);
  CJval arg[] = { shreduler, cc->shred, JCONST(int, 1) };
  CALL_NATIVE2(shreduler_remove, "vppi", arg);
}

#define jit_branch(name, type, sz, op) \
JIT_CODE(Branch##name) {               \
push_reg(cc, -sz*2);                   \
CJval lhs = JLOADR(cc->reg, 0, type);  \
CJval rhs = JLOADR(cc->reg, sz, type); \
CJval cond = JINSN(op, lhs, rhs);      \
INIT_LABEL(lbl)                        \
  INIT_LABEL(end)                      \
JINSN(branch_if_not, cond, &lbl);      \
next_pc(cc, instr->m_val);             \
jit_insn_default_return(cc->f);        \
  JINSN(branch, &end);\
JINSN(label, &lbl);                    \
JINSN(label, &end);\
}
jit_branch( EqInt,    nint,  SZ_INT,  eq)
jit_branch(NeqInt,    nint,  SZ_INT,  ne)
jit_branch( EqFloat, float, SZ_FLOAT, eq)
jit_branch(NeqFloat, float, SZ_FLOAT, ne)

JIT_CODE(BranchSwitch) {
  push_reg(cc, -SZ_INT);
  CJval idx = JLOADR(cc->reg, 0, nint);
  CJval map = JCONST(void_ptr, instr->m_val2);
  Jval arg[] = { map, idx };
  CJval pc = CALL_NATIVE2(map_get, "IpI", arg);
  INIT_LABEL(lbl)
  INIT_LABEL(end)
  JINSN(branch_if, pc, &lbl);
  next_pc(cc, instr->m_val);
  JINSN(branch, &end);
  JINSN(label, &lbl);
  JSTORER(cc->shred, JOFF(VM_Shred, pc), pc);
  JINSN(label, &end);
}

JIT_CODE(AllocWord) {
  CJval size = JCONST(nuint, instr->m_val2);
  CJval mem  = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval ptr  = JADDR(mem, instr->m_val);
  if(*(m_uint*)instr->ptr < 2) {
    CJval zero = JCONST(nint, 0);
    JINSN(memset, ptr, zero, size);
    if(*(m_uint*)instr->ptr)
      JSTORER(cc->reg, 0, ptr);
    else
      JINSN(memcpy, cc->reg, mem, size);
    push_reg(cc, instr->m_val2);
  } else
    JSTORER(ptr, 0, JLOADR(cc->reg, -SZ_INT, void_ptr));
}

JIT_CODE(AllocMember) {
  CJval size = JCONST(nuint, instr->m_val2);
  CJval mem  = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval obj  = JLOADR(mem, 0, void_ptr);
  CJval _data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval data = JADDR(_data, instr->m_val);
  if(*(m_uint*)instr->ptr)
    JSTORER(cc->reg, 0, data);
  else
    JINSN(memcpy, cc->reg, data, size);
  push_reg(cc, instr->m_val2);
}

JIT_CODE(DotStatic) {
  CJval type  = JLOADR(cc->reg, -SZ_INT, void_ptr);
  CJval nspc  = JLOADR(type, JOFF(Type, nspc), void_ptr);
  CJval _data  = JLOADR(nspc, JOFF(Nspc, class_data), void_ptr);
  CJval data  = JADDR(_data, instr->m_val);
  if(*(m_uint*)instr->ptr)
    JSTORER(cc->reg, -SZ_INT, data);
  else {
    CJval _reg = JADDR(cc->reg, - SZ_INT);
    CJval size = JCONST(nuint, instr->m_val2);
    JINSN(memcpy, _reg, data, size);
  }
  push_reg(cc, instr->m_val2 - SZ_INT);
}

JIT_CODE(DotImport) {
  CJval data = JCONST(void_ptr, instr->m_val);
  if(*(m_uint*)instr->ptr)
    JSTORER(cc->reg, 0, data);
  else {
    CJval size = JCONST(nuint, instr->m_val2);
    JINSN(memcpy, cc->reg, data, size);
  }
  push_reg(cc, instr->m_val2);
}

JIT_CODE(GcIni) {
  CJval gc = JADDR(cc->shred, JOFF(VM_Shred, gc));
  jit_vector_add(cc, gc, JCONST(void_ptr, NULL));
}

JIT_CODE(GcAdd) {
  CJval gc = JADDR(cc->shred, JOFF(VM_Shred, gc));
  CJval obj = JLOADR(cc->reg, -SZ_INT,void_ptr);
  jit_vector_add(cc, gc, obj);
//  CJval arg[] = { gc, obj };
//  CALL_NATIVE2(vector_add, "vpp", arg);
}

ANN /*static inline */void gcend(const VM_Shred shred) {
  M_Object o;
  while((o = (M_Object)vector_pop(&shred->gc)))
    _release(o, shred);
}

JIT_CODE(GcEnd) {
  CJval arg[] = { cc->shred };
  CALL_NATIVE2(gcend, "vp", arg);
}

JIT_CODE(RegPop) {
  push_reg(cc, -instr->m_val);
}

JIT_CODE(RegPushImm) {
  if(instr->m_val == SZ_INT) {
    CJval ptr = JCONST(void_ptr, *(m_uint*)instr->ptr);
    JSTORER(cc->reg, 0, ptr);
  } else {
    CJval len = JCONST(nuint, instr->m_val);
    CJval ptr = JCONST(void_ptr, instr->ptr);
    JINSN(memcpy, cc->reg, ptr, len);
  }
  push_reg(cc, instr->m_val);
}

JIT_CODE(RegPushMemAddr) {
  CJval mem = *(m_uint*)instr->ptr ?
    JLOADR(cc->shred, JOFF(VM_Shred, base), void_ptr) :
    JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval ptr = JADDR(mem, instr->m_val);
  JSTORER(cc->reg, 0, ptr);
  push_reg(cc, SZ_INT);
}

JIT_CODE(MemPushImm) {
  CJval val = JCONST(nuint, instr->m_val);
  CJval _mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  JSTORER(_mem, 0, val);
  CJval  mem  = JADDR(_mem, SZ_INT);
  JSTORER(cc->shred, JOFF(VM_Shred, mem), mem);
}

JIT_CODE(MemSetImm) {
  CJval val = JCONST(nuint, instr->m_val2);
  CJval mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  JSTORER(mem, instr->m_val, val);
}

JIT_CODE(RegPushMem) {
  CJval len = JCONST(nuint, instr->m_val2);
  CJval mem = !*(m_uint*)instr->ptr ?
    JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr) :
    JLOADR(cc->shred, JOFF(VM_Shred, base), void_ptr);
  CJval ptr = JADDR(mem, instr->m_val);
  JINSN(memcpy, cc->reg, ptr, len);
  push_reg(cc, instr->m_val2);
}

JIT_CODE(RegPushPtr) {
  CJval val = JCONST(nuint, instr->m_val);
  JSTORER(cc->reg, -SZ_INT, val);
}

JIT_CODE(RegPushCode) {
  CJval f = JLOADR(cc->reg, -SZ_INT, void_ptr);
  cc_check(cc, f, "NullFuncPtrException");
  CJval code = JLOADR(f, JOFF(Func, code), void_ptr);
  JSTORER(cc->reg, -SZ_INT, code);

}

JIT_CODE(RegDup) {
  CJval  val = JLOADR(cc->reg, -SZ_INT, void_ptr);
  JSTORER(cc->reg, 0, val);
  push_reg(cc, SZ_INT);
}

JIT_CODE(RegAddRef) {
  CJval ptr = JLOADR(cc->reg, -SZ_INT, void_ptr);
  CJval obj = instr->m_val ? JLOADR(ptr, 0, void_ptr) : ptr;
  INIT_LABEL(lbl)
  JINSN(branch_if_not, obj, &lbl);
  CJval _ref = JLOADR(obj, JOFF(M_Object, ref), nuint);
  CJval one = JCONST(void_ptr, 1);
  CJval ref = JINSN(add, _ref, one);
  JSTORER(obj, JOFF(M_Object, ref), ref);
  JINSN(label, &lbl);
}

JIT_CODE(RegPushMe) {
  CJval  me = JLOADR(cc->shred, JOFF(VM_Shred,  me), void_ptr);
  JSTORER(cc->reg, 0, me);
  push_reg(cc, SZ_INT);
}

JIT_CODE(RegPushNow) {
  CJval   vm = JLOADR(cc->shred, JOFF(VM_Shred, vm), void_ptr);
  CJval  bbq = JLOADR(vm, __builtin_offsetof(VM, bbq), void_ptr);
  CJval  pos = JLOADR(bbq, JOFF(BBQ, pos), nuint);
  CJval f = JINSN(convert, pos, jit_type_float, 0);
  JSTORER(cc->reg, 0, f);
  push_reg(cc, SZ_FLOAT);
}

JIT_CODE(RegPushMaybe) {
  CJval  ret = CALL_NATIVE2(rand, "U", NULL);
  CJval  mid = JCONST(nuint, RAND_MAX / 2);
  CJval maybe = JINSN(gt, ret, mid);
  JSTORER(cc->reg, 0, maybe);
  push_reg(cc, SZ_INT);
}


JIT_CODE(InitLoopCounter) {
  push_reg(cc, -SZ_INT);
  CJval sp = JLOADR(cc->reg, 0, nint);
  CJval ret = JINSN(abs, sp);
  CJval val = JCONST(void_ptr, instr->m_val);
  JSTORER(val, 0, ret);
}

JIT_CODE(RegPushDeref) {
  CJval ptr = JCONST(void_ptr, instr->ptr);
  CJval src = JLOADR(ptr, 0, void_ptr);
  CJval tgt = JADDR(cc->reg, 0);
  CJval size = JCONST(nuint, instr->m_val);
  JINSN(memcpy, tgt, src, size);
  push_reg(cc, instr->m_val);
}

JIT_CODE(DecIntAddr) {
  CJval m_val = JCONST(nuint, instr->m_val);
  CJval val = JLOADR(m_val, 0, nuint);
  CJval one = JCONST(nuint, 1);
  CJval ret = JINSN(sub, val, one);
  JSTORER(m_val, 0, ret);
}

static inline void jit_push_me(CC cc, CJval sh) {
  CJval me = JLOADR(sh, JOFF(VM_Shred, me), void_ptr);
  push_reg(cc, SZ_INT);
  JSTORER(cc->reg, -SZ_INT, me);
}


static Jval jit_init_spork_shred(CC cc, CJval code) {
  CJval sh = CALL_NATIVE2(new_vm_shred, "pp", &code);
// ref ++
  Jval ref = JLOADR(code, JOFF(VM_Code, obj), nuint);
  ref = JINSN(add, ref, JCONST(nuint, 1));
  JSTORER(code, JOFF(VM_Code, obj), ref);
  JSTORER(sh, JOFF(VM_Shred, parent), cc->shred);
// if ! vector_i
  CJval child = JADDR(cc->shred, JOFF(VM_Shred, child));

{
  INIT_LABEL(lbl);
  CJval ptr = JLOADR(child, JOFF(Vector, ptr), void_ptr);
  JINSN(branch_if, ptr, &lbl);
  CJval arg0[] = { JCONST(nuint, MAP_CAP), JCONST(nuint, SZ_INT) };
  CJval data = CALL_NATIVE2(xcalloc, "pUU", arg0);
  JSTOREE(data, JCONST(nuint, 1), JCONST(nuint, MAP_CAP));
  JSTORER(child, JOFF(Vector, ptr), data);
  JINSN(label, &lbl);
}
  jit_vector_add(cc, child, sh);
  CJval base = JLOADR(cc->shred, JOFF(VM_Shred, base), void_ptr);
  JSTORER(sh, JOFF(VM_Shred, base), base);
  CJval vm = JLOADR(cc->shred, JOFF(VM_Shred, vm), void_ptr);
  Jval arg[] = { vm, sh };
  CALL_NATIVE2(vm_add_shred, "vpp", arg);
  return sh;
}

JIT_CODE(SporkFunc) {
  push_reg(cc, -SZ_INT*2);
  CJval code = JLOADR(cc->reg, SZ_INT, void_ptr);
  CJval sh = jit_init_spork_shred(cc, code);
  Jval  sh_reg = JLOADR(sh, JOFF(VM_Shred, reg), void_ptr);
  CJval func = JLOADR(cc->reg, 0, void_ptr);
  CJval flag = JLOADR(code, JOFF(VM_Code, flag), nuint);
  CJval cond = cc_get_flag(cc, flag, _NEED_THIS_);

  INIT_LABEL(lbl);
  INIT_LABEL(end);
  JINSN(branch_if_not, cond, &lbl);
  push_reg(cc, -instr->m_val - SZ_INT);
  JINSN(branch, &end);
  JINSN(label, &lbl);
  if(instr->m_val)
    push_reg(cc, -instr->m_val);
  JINSN(label, &end);
  if(instr->m_val) {
    CJval size = JCONST(nuint, instr->m_val);
    JINSN(memcpy, sh_reg, cc->reg, size);
    sh_reg = JADDR(sh_reg, instr->m_val);
    JSTORER(sh, JOFF(VM_Shred, reg), sh_reg);
  }
  INIT_LABEL(need_lbl);
  JINSN(branch_if_not, cond, &need_lbl);
  CJval this_ptr = JLOADR(cc->reg, instr->m_val, void_ptr);
  JSTORER(sh_reg, 0, this_ptr);
  sh_reg = JADDR(sh_reg, SZ_INT);
  JSTORER(sh, JOFF(VM_Shred, reg), sh_reg);
  JINSN(label, &need_lbl);
  jit_push_me(cc, sh);
  sh_reg = JLOADR(sh, JOFF(VM_Shred, reg), void_ptr);
  JSTORER(sh_reg, 0, func);
  sh_reg = JADDR(sh_reg, SZ_INT);
  JSTORER(sh, JOFF(VM_Shred, reg), sh_reg);
}

JIT_CODE(SporkExp) {
  push_reg(cc, -SZ_INT*2);
  CJval code = JLOADR(cc->reg, SZ_INT, void_ptr);
  CJval sh = jit_init_spork_shred(cc, code);
  CJval flag = JLOADR(code, JOFF(VM_Code, flag), nuint);
  CJval cond = cc_get_flag(cc, flag, _NEED_THIS_);
  INIT_LABEL(lbl);
  JINSN(branch_if_not, cond, &lbl);
  push_reg(cc, -SZ_INT);
  JINSN(label, &lbl);
  CJval sh_mem = JLOADR(sh, JOFF(VM_Shred, mem), void_ptr);
  CJval mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  JINSN(memcpy, JADDR(sh_mem, SZ_INT), mem, JCONST(nuint, instr->m_val));
  jit_push_me(cc, sh);
}

JIT_CODE(Goto) {
  next_pc(cc, instr->m_val);
  jit_insn_default_return(cc->f);
}

JIT_CODE(ObjectRelease) {
  CJval  mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval  obj = JLOADR(mem, instr->m_val, void_ptr);
  cc_release2(cc, obj);
}

JIT_CODE(DotData) {
  CJval obj = JLOADR(cc->reg, -SZ_INT, void_ptr);
  cc_check(cc, obj, "NullPtrException");
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval  val = JADDR(data, instr->m_val);
  if(!*(m_uint*)instr->ptr) {
    CJval len = JCONST(nuint, instr->m_val2);
    CJval tgt = JADDR(cc->reg, -SZ_INT);
    JINSN(memcpy, tgt, val, len);
    push_reg(cc, instr->m_val2 -SZ_INT);
  } else {
    CJval len = JCONST(nuint, SZ_INT);
    CJval tgt = JADDR(cc->reg, -SZ_INT);
    CJval ptr = JINSN(address_of, val);
    JINSN(memcpy, tgt, ptr, len);
  }
}

JIT_CODE(ObjectInstantiate) {
  Jval arg[] = { cc->shred, JCONST(void_ptr, instr->m_val) };
  CJval obj = CALL_NATIVE2(new_object, "ppp", arg);
  JSTORER(cc->reg, 0, obj);
  push_reg(cc, SZ_INT);
}

ANN static void jit_overflow(CC const cc) {
  CJval mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval _reg = JLOADR(cc->shred, JOFF(VM_Shred, _reg), void_ptr);
  CJval reg = JADDR(_reg, SIZEOF_REG);
  CJval val = JINSN(add, reg, JCONST(nuint, (SIZEOF_MEM) - (MEM_STEP)));
  CJval ovf = JINSN(gt, mem, val);
  INIT_LABEL(lbl);
  JINSN(branch_if_not, ovf, &lbl);
  cc_except(cc, "StackOverflow");
  JINSN(label, &lbl);
}

static inline void jit_dl_return_push(CC cc, CJval retval, const m_uint size) {
  JINSN(memcpy, cc->reg, retval, JCONST(nuint, size));
  push_reg(cc, size);
}

ANN static inline void jit_copy_member_args(CC cc, const CJval code) {
  CJval stack_depth = JLOADR(code, JOFF(VM_Code, stack_depth), nuint);
  push_reg2(cc, JINSN(neg, stack_depth));
  CJval depth = JINSN(sub, stack_depth, JCONST(nuint, SZ_INT));
  Jval mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval _ptr = JINSN(add, cc->reg, depth);
  CJval ptr = JLOADR(_ptr, 0, void_ptr);
  JSTORER(mem, 0, ptr);
  mem = JADDR(mem, SZ_INT);
  INIT_LABEL(lbl);
  JINSN(branch_if_not, depth, &lbl);
  JINSN(memcpy, mem, cc->reg, depth);
  JINSN(label, &lbl);
}
JIT_CODE(FuncMember) {
  push_reg(cc, -SZ_INT*2);
  CJval code  = JLOADR(cc->reg, 0, void_ptr);
  CJval depth = JLOADR(cc->reg, SZ_INT, nuint);
  CJval _mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  Jval mem = JINSN(add, _mem, depth);
  JSTORER(cc->shred, JOFF(VM_Shred, mem), mem);
  CJval f = JLOADR(code, JOFF(VM_Code, native_func), void_ptr);
  jit_copy_member_args(cc, code);
  jit_overflow(cc);
  CJval obj = JLOADR(mem, 0, void_ptr);
  CJval len = JCONST(nuint, instr->m_val);
  CJval retval = JINSN(alloca, len);
  Jval arg3[] = { obj, retval, cc->shred };
  jit_type_t type = sig(&cc->sig, "vppp", jit_abi_fastcall);
  JINSN(call_indirect, f, type, arg3, 3, JIT_CALL);
  jit_type_free(type);
  jit_dl_return_push(cc, retval, instr->m_val);
  JSTORER(cc->shred, JOFF(VM_Shred, mem), _mem);
}


JIT_CODE(FuncReturn) {
  CJval mem  = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval pc   = JLOADR(mem, -SZ_INT*2, nuint);
  JSTORER(cc->shred, JOFF(VM_Shred, pc), pc);
  CJval code = JLOADR(mem, -SZ_INT*3, void_ptr);
  JSTORER(cc->shred, JOFF(VM_Shred, code), code);
  CJval off1 = JCONST(nuint, SZ_INT*4);
  CJval off2 = JLOADR(mem, -SZ_INT*4, nuint);
  CJval off  = JINSN(add, off1, off2);
  CJval neg  = JINSN(neg, off);
  CJval _mem = JINSN(add, mem, neg);
  JSTORER(cc->shred, JOFF(VM_Shred, mem), _mem);
}

JIT_CODE(FuncStatic) {
  push_reg(cc, -SZ_INT*2);
  CJval code = JLOADR(cc->reg, 0, void_ptr);
  CJval local_depth = JLOADR(cc->reg, SZ_INT, nint);
  Jval mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  mem = JINSN(add, mem, local_depth);
  JSTORER(cc->shred, JOFF(VM_Shred, mem), mem);
  CJval stack_depth = JLOADR(code, JOFF(VM_Code, stack_depth), nint);
  INIT_LABEL(lbl);
  JINSN(branch_if_not, stack_depth, &lbl);
  CJval ndepth = JINSN(neg, stack_depth);
  push_reg2(cc, ndepth);
  CJval _mem = JADDR(mem, SZ_INT);
  JINSN(memcpy, _mem, cc->reg, stack_depth);
  JINSN(label, &lbl);
  jit_overflow(cc);
  CJval size = JCONST(nuint, instr->m_val);
  CJval retval = JINSN(alloca, size);
  CJval func = JLOADR(code, JOFF(VM_Code, native_func), void_ptr);
  Jval  func_arg[] = { retval, cc->shred };
  jit_type_t type1 = sig(&cc->sig, "vpp", jit_abi_fastcall);
  JINSN(call_indirect, func, type1, func_arg, 2, JIT_CALL);
  jit_type_free(type1);
  jit_dl_return_push(cc, retval, instr->m_val);
  mem = JINSN(sub, mem, local_depth);
  JSTORER(cc->shred, JOFF(VM_Shred, mem), mem);
}

JIT_CODE(FuncPtr) {
  INIT_LABEL(end)
  INIT_LABEL(mid)
  INIT_LABEL(post)
  CJval arg[] = { cc->shred, JCONST(void_ptr, instr) };
  CJval code = JLOADR(cc->reg, -SZ_INT*2, void_ptr);
  CJval flag = JLOADR(code, JOFF(VM_Code, flag), nuint);
  CJval native = cc_get_flag(cc, flag, NATIVE_NOT);
  JINSN(branch_if_not, native, &mid);
  CALL_NATIVE2(FuncUsr, "vpp", arg);
  JINSN(branch, &end);
  JINSN(label, &mid);
  CJval member = cc_get_flag(cc, flag, _NEED_THIS_);
  JINSN(branch_if_not, member, &post);
  CALL_NATIVE2(FuncMember, "vpp", arg);
  JINSN(branch, &end);
  JINSN(label, &post);
  CALL_NATIVE2(FuncStatic, "vpp", arg);
  JINSN(label, &end);
push_reg(cc, 0);
}

// static Jval (return code)
static Jval jit_shred_func_prepare(CC cc) {
  push_reg(cc, -SZ_INT*2);
  CJval code = JLOADR(cc->reg, 0, void_ptr);
  CJval depth = JLOADR(cc->reg, SZ_INT, nuint);
  Jval mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval stack = JLOADR(mem, -SZ_INT, nuint);
  CJval push = JINSN(add, depth, stack);
  mem = JADDR(mem, SZ_INT*4);
  mem = JINSN(add, mem, push);
  JSTORER(cc->shred, JOFF(VM_Shred, mem), mem);
  JSTORER(mem, -SZ_INT*4, push);
  JSTORER(mem, -SZ_INT*3, JLOADR(cc->shred, JOFF(VM_Shred, code), void_ptr));
  JSTORER(mem, -SZ_INT*2, JLOADR(cc->shred, JOFF(VM_Shred, pc), nuint));
  JSTORER(mem, -SZ_INT, JLOADR(code, JOFF(VM_Code, stack_depth), nuint));
  JSTORER(cc->shred, JOFF(VM_Shred, pc), JCONST(nuint, 0));
  JSTORER(cc->shred, JOFF(VM_Shred, code), code);
  return code;
}

static void jit_shred_func_need_this(CC cc, CJval depth) {
  CJval mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval _depth = JINSN(sub, depth, JCONST(nuint, SZ_INT));
  CJval _reg = JINSN(add, cc->reg, _depth);
  CJval this_ptr = JLOADR(_reg, 0, void_ptr);
  JSTORER(mem, 0, this_ptr);
  CJval _mem = JADDR(mem, SZ_INT);
  JSTORER(cc->shred, JOFF(VM_Shred, mem), _mem);
}

static void jit_shred_func_finish(CC cc, CJval need_this) {
  INIT_LABEL(lbl);
  JINSN(branch_if_not, need_this, &lbl);
  Jval mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  mem = JADDR(mem, -SZ_INT);
  JSTORER(cc->shred, JOFF(VM_Shred, mem), mem);
  JINSN(label, &lbl);
  jit_overflow(cc);
}

JIT_CODE(FuncUsr) {
  CJval code = jit_shred_func_prepare(cc);
  CJval flag = JLOADR(code, JOFF(VM_Code, flag), nuint);
  CJval this = cc_get_flag(cc, flag, _NEED_THIS_);
  Jval depth = JLOADR(code, JOFF(VM_Code, stack_depth), nuint);

  INIT_LABEL(lbl);
  JINSN(branch_if_not, depth, &lbl);
  CJval ndepth = JINSN(neg, depth);
  push_reg2(cc, ndepth);
  INIT_LABEL(lbl2);
  if(instr) {
    JINSN(branch_if_not, this, &lbl2);
    jit_shred_func_need_this(cc, depth);
    CJval sz_int = JCONST(nuint, SZ_INT);
    CJval sub = JINSN(sub, depth, sz_int);
    JINSN(store, sub, depth);
    JINSN(label, &lbl2);
  {
    INIT_LABEL(lbl);
    JINSN(branch_if_not, depth, &lbl);
    CJval mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
    JINSN(memcpy, mem, cc->reg, depth);
    JINSN(label, &lbl);
  }
  } else {
    Jval mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
    CJval ptr = JLOADR(cc->reg, 0, void_ptr);
    JSTORER(mem, 0, ptr);
    mem = JADDR(mem, SZ_INT);
    JSTORER(cc->shred, JOFF(VM_Shred, mem), mem);
  }
  JINSN(label, &lbl);
  jit_shred_func_finish(cc, this);
}


JIT_CODE(FuncOp) {
  CJval code = jit_shred_func_prepare(cc);
  const Type l = (Type)instr->m_val2;
  const Type r = *(Type*)instr->ptr;
  const m_uint size = l->size + (r ? r->size : 0);

  push_reg(cc, -size);
  INIT_LABEL(lbl);
  CJval depth = JCONST(nuint, size);
  CJval flag = JLOADR(code, JOFF(VM_Code, flag), nuint);
  CJval this = cc_get_flag(cc, flag, _NEED_THIS_);
  JINSN(branch_if_not, this, &lbl);
  jit_shred_func_need_this(cc, depth);
  JINSN(label, &lbl);
  CJval mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  JINSN(memcpy, mem, cc->reg, depth);
  jit_shred_func_finish(cc, this);
}

JIT_CODE(DotFunc) {
  CJval obj = JLOADR(cc->reg, -SZ_INT, void_ptr);
  cc_check(cc, obj, "NullPtrExcption");
  CJval type   = JLOADR(obj, JOFF(M_Object, type_ref), void_ptr);
  CJval nspc   = JLOADR(type, JOFF(Type, nspc), void_ptr);
  CJval vec = JADDR(nspc, JOFF(Nspc, vtable));
  CJval ptr   = JLOADR(vec, JOFF(Vector, ptr), void_ptr);
  CJval func = JLOADE(ptr, JCONST(nuint, instr->m_val + OFFSET), void_ptr);
  JSTORER(cc->reg, -SZ_INT, func);
}

JIT_CODE(PreCtor) {
  const VM_Code pre_ctor = (VM_Code)instr->m_val;
  CJval obj = JLOADR(cc->reg, -SZ_INT, void_ptr);
  if(!GET_FLAG(pre_ctor, NATIVE_NOT)) { // fast builtin constructor
    Jval arg[] = { obj, cc->shred };
    CALL_NATIVE(pre_ctor->native_func, "vpp", arg);
  } else {
    JSTORER(cc->reg, 0, obj);
    CJval code = JCONST(nuint, pre_ctor);
    JSTORER(cc->reg, SZ_INT, code);
    CJval depth = JCONST(nuint, instr->m_val2);
    JSTORER(cc->reg, SZ_INT*2, depth);
    push_reg(cc, SZ_INT*3);
    jitcode_FuncUsr(cc, NULL);
  }
}


JIT_CODE(AutoLoopStart) {
  CJval obj = JLOADR(cc->reg, -SZ_INT, void_ptr);
  cc_check(cc, obj, "NullPtrException");
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval array = JLOADR(data, 0, void_ptr);
  CJval mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval idx = JLOADR(mem, instr->m_val, nuint);
  const Type t = (Type)instr->m_val2;
  if(t) {
    INIT_LABEL(lbl);
    JINSN(branch_if, idx, &lbl);
    Jval new_arg[] = { cc->shred, JCONST(void_ptr, t) };
    CJval _ptr = CALL_NATIVE2(new_object, "ppp", new_arg);
    JSTORER(mem, instr->m_val + SZ_INT, _ptr);
    JINSN(label, &lbl);
    CJval ptr = JLOADR(mem, instr->m_val + SZ_INT, void_ptr);
    Jval arg[] = { array, idx };
    CJval ret = CALL_NATIVE2(m_vector_addr, "ppU", arg);
    CJval ptr_data = JLOADR(ptr, JOFF(M_Object, data), void_ptr);
    JSTORER(ptr_data, 0, ret);
  } else {
    CJval ptr = JADDR(mem, instr->m_val + SZ_INT);
    Jval arg[] = { array, idx, ptr };
    CALL_NATIVE2(m_vector_get, "vpUp", arg);
  }
}

JIT_CODE(AutoLoopEnd) {
  CJval mem = JLOADR(cc->shred, JOFF(VM_Shred, mem), void_ptr);
  if(instr->m_val2) {
    CJval ptr = JLOADR(mem, instr->m_val + SZ_INT, void_ptr);
    cc_release(cc, ptr);
  }
  CJval idx = JLOADR(mem, instr->m_val, nuint);
  CJval one = JCONST(nuint, 1);
  CJval sum = JINSN(add, idx, one);
  JSTORER(mem, instr->m_val, sum);
  CJval obj = JLOADR(cc->reg, -SZ_INT, void_ptr);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval array = JLOADR(data, 0, void_ptr);
  CJval arg[] = { array };
  CJval size = CALL_NATIVE2(m_vector_size, "Up", arg);
  CJval cond = JINSN(ge, sum, size);
  INIT_LABEL(lbl);
  JINSN(branch_if_not, cond, &lbl);
  next_pc(cc, *(m_uint*)instr->ptr);
  jit_insn_default_return(cc->f);
  push_reg(cc, -SZ_INT);
  JINSN(label, &lbl);
}

JIT_CODE(PopArrayClass) {
  push_reg(cc, -SZ_INT);
  CJval obj   = JLOADR(cc->reg, -SZ_INT, void_ptr);
  CJval tmp   = JLOADR(cc->reg, 0, void_ptr);
  CJval tdata = JLOADR(tmp, JOFF(M_Object, data), void_ptr);
  CJval odata = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval array = JLOADR(tdata, 0, void_ptr);
  JSTORER(odata, 0, array);
  Jval arg1[] = { tmp };
  CALL_NATIVE2(free_object, "vp", arg1);
  CJval type = JLOADR(obj, JOFF(M_Object, type_ref), void_ptr);
  jit_type_add_ref(cc, type);
}

#define JIT_IMPORT(a) jit_code_import(j, a, jitcode_##a);
void jit_code_import_instr(struct Jit* j) {
  JIT_IMPORT(EOC)
  JIT_IMPORT(DTOR_EOC)
  JIT_IMPORT(BranchEqInt)
  JIT_IMPORT(BranchNeqInt)
  JIT_IMPORT(BranchEqFloat)
  JIT_IMPORT(BranchNeqFloat)
  JIT_IMPORT(BranchSwitch)
  JIT_IMPORT(AllocWord)
  JIT_IMPORT(AllocMember)
  JIT_IMPORT(PreCtor)
  JIT_IMPORT(FuncReturn)
  JIT_IMPORT(FuncMember)
  JIT_IMPORT(FuncPtr)
  JIT_IMPORT(FuncStatic)
  JIT_IMPORT(FuncUsr)
  JIT_IMPORT(FuncOp)
  JIT_IMPORT(RegPushPtr)
  JIT_IMPORT(RegPushCode)
  JIT_IMPORT(Goto)
  JIT_IMPORT(RegDup)
  JIT_IMPORT(RegAddRef)
  JIT_IMPORT(RegPushMem)
  JIT_IMPORT(RegPushMe)
  JIT_IMPORT(RegPushNow)
  JIT_IMPORT(RegPushMaybe)
  JIT_IMPORT(RegPushMemAddr)
  JIT_IMPORT(MemPushImm)
  JIT_IMPORT(MemSetImm)
  JIT_IMPORT(InitLoopCounter)
  JIT_IMPORT(RegPushDeref)
  JIT_IMPORT(DecIntAddr)
  JIT_IMPORT(AutoLoopStart)
  JIT_IMPORT(AutoLoopEnd)
  JIT_IMPORT(RegPushImm)
  JIT_IMPORT(RegPop)
  JIT_IMPORT(DotFunc)
  JIT_IMPORT(DotData)
  JIT_IMPORT(DotStatic)
  JIT_IMPORT(DotImport)
  JIT_IMPORT(ObjectInstantiate)
  JIT_IMPORT(ObjectRelease)
  JIT_IMPORT(GcIni)
  JIT_IMPORT(GcAdd)
  JIT_IMPORT(GcEnd)
  JIT_IMPORT(PopArrayClass)
  JIT_IMPORT(SporkFunc)
  JIT_IMPORT(SporkExp)
}
