#include <jit/jit.h>
#include "gwjit_common.h"
#include "cc.h"
#include "q.h"
#include "code.h"
#include "sig.h"
#include "code/func.h"
/*
JIT_CODE(EOC) {
  CJval arg[] = { j->shred };
  CALL_NATIVE(vm_shred_exit, "vp", arg);
}

ANN inline static void code_rem_ref(VM_Code c) {
  --c->obj.ref_count;
}

JIT_CODE(DTOR_EOC) {
  CJval code  = JLOADR(j->shred, JOFF(VM_Shred, code), void_ptr);
  CJval arg0[] = { code };
  CALL_NATIVE(code_rem_ref, "vp", arg0);
  CJval arg1[] = { j->shred };
  CALL_NATIVE(vm_shred_exit, "vp", arg1);
}

#define jit_branch(name, type, sz, op) \
JIT_CODE(Branch_##name) {              \
CJval reg = push_reg(j, -sz*2);        \
CJval lhs = JLOADR(reg, 0, type);      \
CJval rhs = JLOADR(reg, sz, type);     \
CJval cond = JINSN(op, lhs, rhs);     \
INIT_LABEL(lbl)                        \
JINSN(branch_if_not, cond, &lbl);     \
next_pc(j, instr->m_val);              \
JINSN(label, &lbl);                   \
}
jit_branch( Eq_Int,   nint,  SZ_INT,   eq)
jit_branch(Neq_Int,   nint,  SZ_INT,   ne)
jit_branch( Eq_Float, float, SZ_FLOAT, eq)
jit_branch(Neq_Float, float, SZ_FLOAT, ne)

JIT_CODE(Branch_Switch) {
  CJval reg = push_reg(j, -SZ_INT);
  CJval idx = JLOADR(reg, 0, nint);
  CJval map = JCONST(void_ptr, (m_uint)*(Map*)instr->ptr);
  Jval arg[] = { map, idx };
  CJval pc = CALL_NATIVE(map_get, "IpI", arg);
  CJval zero = JCONST(nuint, 0);
  INIT_LABEL(lbl)
  INIT_LABEL(end)
  CJval cond = JINSN(eq, pc, zero);
  JINSN(branch_if_not, cond, &lbl);
  next_pc(j, instr->m_val);
  JINSN(branch, &end);
  JINSN(label, &lbl);
  JSTORER(j->shred, JOFF(VM_Shred, pc), pc);
  JINSN(label, &end);
}

JIT_CODE(Alloc_Word) {
  CJval zero = JCONST(nuint, 0);
  CJval size = JCONST(nuint, instr->m_val2);
  CJval reg  = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval mem  = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval ptr  = JADDR(mem, instr->m_val);
  JINSN(memset, ptr, zero, size);
  if(*(m_uint*)instr->ptr)
    JSTORER(reg, 0, ptr);
  else
    JINSN(memcpy, reg, mem, size);
  CJval _reg = JADDR(reg, instr->m_val2);
  JSTORER(j->shred, JOFF(VM_Shred, reg), _reg);
}

JIT_CODE(Alloc_Member) {
  CJval size = JCONST(nuint, instr->m_val2);
  CJval mem  = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval obj  = JLOADR(mem, 0, void_ptr);
  CJval _data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval data = JADDR(_data, instr->m_val);
  CJval reg  = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  if(*(m_uint*)instr->ptr)
    JSTORER(reg, 0, data);
  else
    JINSN(memcpy, reg, data, size);
  CJval _reg = JADDR(reg, instr->m_val2);
  JSTORER(j->shred, JOFF(VM_Shred, reg), _reg);
}

JIT_CODE(Dot_Static_Data) {
  CJval reg  = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval type  = JLOADR(reg, -SZ_INT, void_ptr);
  CJval nspc  = JLOADR(type, JOFF(Type, nspc), void_ptr);
  CJval _data  = JLOADR(nspc, JOFF(Nspc, class_data), void_ptr);
  CJval data  = JADDR(_data, instr->m_val);
  if(*(m_uint*)instr->ptr)
    JSTORER(reg, -SZ_INT, data);
  else {
    CJval _reg = JADDR(reg, - SZ_INT);
    CJval size = JCONST(nuint, instr->m_val2);
    JINSN(memcpy, _reg, data, size);
  }
  CJval _reg = JADDR(reg, instr->m_val2 - SZ_INT);
  JSTORER(j->shred, JOFF(VM_Shred, reg), _reg);
}

JIT_CODE(Dot_Static_Import_Data) {
  CJval reg  = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval data = JCONST(void_ptr, instr->m_val);
  if(*(m_uint*)instr->ptr)
    JSTORER(reg, 0, data);
  else {
    CJval size = JCONST(nuint, instr->m_val2);
    JINSN(memcpy, reg, data, size);
  }
  CJval _reg = JADDR(reg, instr->m_val2);
  JSTORER(j->shred, JOFF(VM_Shred, reg), _reg);
}
static Vector get_gc(VM_Shred shred) { return &shred->gc; }

JIT_CODE(start_gc) {
  CJval null  = JCONST(void_ptr, 0);
  Jval arg0[] = { j->shred };
  CJval gc = CALL_NATIVE(get_gc, "pp", arg0);
  Jval arg[] = { gc, null };
  CALL_NATIVE(vector_add, "vpp", arg);
}

JIT_CODE(add2gc) {
  Jval arg0[] = { j->shred };
  CJval gc = CALL_NATIVE(get_gc, "pp", arg0);
  CJval  reg  = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval  obj  = JLOADR(reg, -SZ_INT, void_ptr);
  Jval arg[] = { gc, obj };
  CALL_NATIVE(vector_add, "vpp", arg);
}

JIT_CODE(stop_gc) {
  Jval arg0[] = { j->shred };
  CJval gc = CALL_NATIVE(get_gc, "pp", arg0);
  INIT_LABEL(end)
  Jval arg[] = { gc };
  CJval null  = JCONST(void_ptr, 0);
  CJval obj = CALL_NATIVE(vector_pop, "pp", arg);
  CJval cond = JINSN(eq, obj, null);
  JINSN(branch_if, cond, &end);
  Jval arg2[] = { obj, j->shred };
  CALL_NATIVE(_release, "vpp", arg2);
  JINSN(label, &end);
}

JIT_CODE(Reg_Pop_Word) {
  push_reg(j, -instr->m_val);
}

JIT_CODE(Reg_Push_Imm) {
  CJval ptr = JCONST(void_ptr, instr->ptr);
  CJval len = JCONST(nuint, instr->m_val);
  CJval _reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  JINSN(memcpy, _reg, ptr, len);
  CJval  reg  = JADDR(_reg, instr->m_val);
  JSTORER(j->shred, JOFF(VM_Shred, reg), reg);
}

JIT_CODE(Reg_Push_Mem_Addr) {
  CJval mem = *(m_uint*)instr->ptr ?
    JLOADR(j->shred, JOFF(VM_Shred, base), void_ptr) :
    JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval ptr = JADDR(mem, instr->m_val);
  CJval _reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  JSTORER(_reg, 0, ptr);
  CJval  reg  = JADDR(_reg, SZ_INT);
  JSTORER(j->shred, JOFF(VM_Shred, reg), reg);

}

JIT_CODE(Mem_Push_Imm) {
  CJval val = JCONST(nuint, instr->m_val);
  CJval _mem = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  JSTORER(_mem, 0, val);
  CJval  mem  = JADDR(_mem, instr->m_val);
  JSTORER(j->shred, JOFF(VM_Shred, mem), mem);
}

JIT_CODE(Mem_Set_Imm) {
  CJval val = JCONST(nuint, *(m_uint*)instr->ptr);
  CJval mem = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  JSTORER(mem, instr->m_val, val);
}

JIT_CODE(assign_func) {
  CJval _reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  if(!instr->m_val) {
    CJval reg = JADDR(_reg, -SZ_INT);
    JSTORER(j->shred, JOFF(VM_Shred, reg), reg);
    CJval ptr = JLOADR(reg, 0, void_ptr);
    CJval fun = JLOADR(ptr, -SZ_INT, void_ptr);
    JSTORER(ptr, 0, fun);
  } else {
    CJval reg = JADDR(_reg, -SZ_INT * 3);
    JSTORER(j->shred, JOFF(VM_Shred, reg), reg);
    CJval f = JLOADR(reg, 0, void_ptr);
    CJval obj = JLOADR(reg, SZ_INT, void_ptr);
    CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
    JSTORER(data, instr->m_val2, f);
    JSTORER(reg, -SZ_INT, f);
    CJval addr = JINSN(address_of, f);
    JSTORER(reg, SZ_INT * 3, addr);
  }
}

JIT_CODE(Reg_Push_Mem) {
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval len = JCONST(nuint, instr->m_val2);
  CJval mem = !*(m_uint*)instr->ptr ?
    JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr) :
    JLOADR(j->shred, JOFF(VM_Shred, base), void_ptr);
  CJval ptr = JADDR(mem, instr->m_val);
  JINSN(memcpy, reg, ptr, len);
  CJval _reg = JADDR(reg, instr->m_val2);
  JSTORER(j->shred, JOFF(VM_Shred, reg), _reg);
}

JIT_CODE(Reg_Push_Ptr) {
  CJval val = JCONST(nuint, *(m_uint*)instr->ptr);
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  JSTORER(reg, -SZ_INT, val);
}

JIT_CODE(Reg_Push_Code) {
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval mem = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval f = instr->m_val2 ? JLOADR(reg, -SZ_INT, void_ptr) :
     JLOADR(mem, instr->m_val, void_ptr);
  INIT_LABEL(lbl);
  CJval  nil = JCONST(void_ptr, 0);
  CJval cond = JINSN(eq, f, nil);
  JINSN(branch_if_not, cond, &lbl);
  cc_except(j, "NullFuncPtrException");
  JINSN(label, &lbl);
  CJval code = JLOADR(f, JOFF(Func, code), void_ptr);
  JSTORER(reg, -SZ_INT, code);

}

JIT_CODE(Reg_Dup_Last) {
  CJval _reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval  val = JLOADR(_reg, -SZ_INT, void_ptr);
  JSTORER(_reg, 0, val);
  CJval  reg  = JADDR(_reg, SZ_INT);
  JSTORER(j->shred, JOFF(VM_Shred, reg), reg);
}

JIT_CODE(Reg_Add_Ref) {
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval ptr = JLOADR(reg, -SZ_INT, void_ptr);
  CJval obj = instr->m_val ? JLOADR(ptr, 0, void_ptr) : ptr;
  CJval nil = JCONST(void_ptr, 0);
  INIT_LABEL(lbl)
  CJval cond = JINSN(eq, obj, nil);
  JINSN(branch_if, cond, &lbl);
  CJval _ref = JLOADR(obj, JOFF(M_Object, ref), nuint);
  CJval one = JCONST(void_ptr, 1);
  CJval ref = JINSN(add, _ref, one);
  JSTORER(obj, JOFF(M_Object, ref), ref);
  JINSN(label, &lbl);
}

JIT_CODE(Reg_Push_Me) {
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval  me = JLOADR(j->shred, JOFF(VM_Shred,  me), void_ptr);
  JSTORER(reg, 0, me);
  CJval _reg = JADDR(reg, SZ_INT);
  JSTORER(j->shred, JOFF(VM_Shred, reg), _reg);
}

JIT_CODE(Reg_Push_Now) {
  CJval  reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval   vm = JLOADR(j->shred, JOFF(VM_Shred, vm_ref), void_ptr);
  CJval   sp = JLOADR(vm, __builtin_offsetof(VM, sp), void_ptr);
  CJval  pos = JLOADR(sp, __builtin_offsetof(sp_data, pos), nuint);
  CJval f = JINSN(convert, pos, jit_type_float, 0);
  JSTORER(reg, 0, f);
  CJval _reg = JADDR(reg, SZ_FLOAT);
  JSTORER(j->shred, JOFF(VM_Shred, reg), _reg);
}

JIT_CODE(Reg_Push_Maybe) {
  CJval  reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval   vm = JLOADR(j->shred, JOFF(VM_Shred, vm_ref), void_ptr);
  CJval   sp = JLOADR(vm, __builtin_offsetof(VM, sp), void_ptr);
  CJval arg[] = { sp };
  CJval  ret = CALL_NATIVE(sp_rand, "Up", arg);
  CJval  mid = JCONST(nuint, SP_RANDMAX / 2);
  CJval maybe = JINSN(gt, ret, mid);
  JSTORER(reg, 0, maybe);
  CJval _reg = JADDR(reg, SZ_INT);
  JSTORER(j->shred, JOFF(VM_Shred, reg), _reg);
}


JIT_CODE(Init_Loop_Counter) {
  CJval  _reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval reg = JADDR(_reg, SZ_INT);
  JSTORER(j->shred, JOFF(VM_Shred, reg), reg);
  CJval sp = JLOADR(reg, 0, nint);
  CJval ret = JINSN(abs, sp);
  CJval val = JCONST(void_ptr, instr->m_val);
  JSTORER(val, 0, ret);

}

JIT_CODE(Reg_Push_Deref) {
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval val = JCONST(nuint, *(m_uint*)instr->m_val);
  JSTORER(reg, 0, val);
  CJval _reg = JADDR(reg, SZ_INT);
  JSTORER(j->shred, JOFF(VM_Shred, reg), _reg);
}

JIT_CODE(Dec_int_Addr) {
  CJval val = JCONST(nuint, *(m_uint*)instr->m_val);
  CJval one = JCONST(nuint, 1);
  CJval ret = JINSN(add, val, one);
  CJval m_val = JCONST(nuint, instr->m_val);
  JSTORER(m_val, 0, ret);
}

ANN inline static void add_ref(VM_Code c) {
  ++c->obj.ref_count;
}

JIT_CODE(Spork) {
  Jval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  reg = JADDR(reg, -SZ_INT*2);
  JSTORER(j->shred, JOFF(VM_Shred, reg), reg);
  CJval code = JLOADR(reg, SZ_INT, void_ptr);
  CJval arg[] = { j->shred, code };
  CJval sh = CALL_NATIVE(init_spork_shred, "ppp", arg);
  CJval func = JLOADR(reg, 0, void_ptr);
{ // is member
  INIT_LABEL(lbl);
  CJval zero = JCONST(nuint, 0);
  CJval flag = JLOADR(func, JOFF(Func, flag), nuint);
  CJval member = cc_get_flag(j, flag, ae_flag_member);
  CJval cond = JINSN(eq, member, zero);
  JINSN(branch_if, cond, &lbl);
  reg = JADDR(reg, -SZ_INT);
  JSTORER(j->shred, JOFF(VM_Shred, reg), reg);
  JINSN(label, &lbl);
}

  if(instr->m_val) {
    CJval len = JCONST(nuint, instr->m_val);
    Jval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
    CJval _reg = JADDR(reg, -instr->m_val);
    Jval sh_reg = JLOADR(sh, JOFF(VM_Shred, reg), void_ptr);
    JINSN(memcpy, sh_reg, _reg, len);
    sh_reg = JADDR(sh_reg, instr->m_val);
    JSTORER(sh, JOFF(VM_Shred, reg), sh_reg);
  }
  if(*(m_uint*)instr->ptr) {
    CJval   _mem = JLOADR(j->shred, JOFF(VM_Shred, _mem), void_ptr);
    CJval    mem = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
    CJval sh_mem = JLOADR(sh, JOFF(VM_Shred, mem), void_ptr);
    CJval mem2 = JADDR(sh_mem, SZ_INT);
    CJval sz_mem = JCONST(nuint, SIZEOF_MEM);
    Jval len = JINSN(sub, mem, _mem);
    len = JINSN(sub, sz_mem, len);
    JINSN(memcpy, mem2, mem, len);
  }
{
// if member
  INIT_LABEL(lbl1);
  CJval zero = JCONST(nuint, 0);
  CJval flag = JLOADR(func, JOFF(Func, flag), nuint);
  CJval member = cc_get_flag(j, flag, ae_flag_member);
  CJval cond = JINSN(eq, member, zero);
  JINSN(branch_if, cond, &lbl1);
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval obj = JLOADR(reg, 0, void_ptr);
  if(!instr->m_val2) {
    Jval sh_reg = JLOADR(sh, JOFF(VM_Shred, reg), void_ptr);
    JSTORER(sh_reg, 0, obj);
    sh_reg = JADDR(sh_reg, SZ_INT);
    JSTORER(sh, JOFF(VM_Shred, reg), sh_reg);
  } else {
    CJval sh_mem = JLOADR(sh, JOFF(VM_Shred, mem), void_ptr);
    JSTORER(sh_mem, SZ_INT, obj);
  }
  JINSN(label, &lbl1);
}
  {
    Jval me = JLOADR(sh, JOFF(VM_Shred, me), void_ptr);
    Jval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
    JSTORER(reg, 0, me);
    CJval _reg = JADDR(reg, SZ_INT);
    JSTORER(j->shred, JOFF(VM_Shred, reg), _reg);
  }
  if(instr->m_val2) {
    CJval arg[] = { code };
    CALL_NATIVE(add_ref, "vp", arg);
  } else {
    Jval sh_reg = JLOADR(sh, JOFF(VM_Shred, reg), void_ptr);
    JSTORER(sh_reg, 0, func);
    CJval _reg = JADDR(sh_reg, SZ_INT);
    JSTORER(sh, JOFF(VM_Shred, reg), _reg);
  }
}

JIT_CODE(Goto) { next_pc(j, instr->m_val); }

JIT_CODE(Release_Object) {
  CJval  mem = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval  obj = JLOADR(mem, instr->m_val, void_ptr);
  Jval arg[] = { obj, j->shred };
  CALL_NATIVE(release, "vpp", arg);
}

JIT_CODE(Exp_Dot_Data) {
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval obj = JLOADR(reg, -SZ_INT, void_ptr);
  CJval nil = JCONST(void_ptr, 0);
  CJval cond = JINSN(eq, obj, nil);
  INIT_LABEL(lbl1)
  JINSN(branch_if_not, cond, &lbl1);
  cc_except(j, "NullPtrException");
  JINSN(label, &lbl1);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval  val = JADDR(data, instr->m_val);
  if(!*(m_uint*)instr->ptr) {
    CJval len = JCONST(nuint, instr->m_val2);
    CJval tgt = JADDR(reg, -SZ_INT);
    JINSN(memcpy, tgt, val, len);
    CJval ptr = JADDR(reg, instr->m_val2-SZ_INT);
    JSTORER(j->shred, JOFF(VM_Shred, reg), ptr);
  } else {
    CJval len = JCONST(nuint, SZ_INT);
    CJval tgt = JADDR(reg, -SZ_INT);
    CJval ptr = JINSN(address_of, val);
    JINSN(memcpy, tgt, ptr, len);
  }
}
/*
JIT_CODE(Instantiate_Object) {
  CJval type = JCONST(void_ptr, *(Type*)instr->ptr);
  Jval arg0[] = {j->shred, type};
  CJval obj = CALL_NATIVE(instantiate_object, "vpp", arg0);


//  jitcode_start_gc(j, instr);


//  CJval null  = JCONST(void_ptr, 0);
  Jval arg[] = { j->shred };
  CJval gc = CALL_NATIVE(get_gc, "pp", arg);
  Jval arg1[] = { gc, obj };
  CALL_NATIVE(vector_add, "vpp", arg1);

}
*/
/*
ANN static void jit_overflow(JitCC *const j) {
  Jval arg[] =  { j->shred };
  CJval ovf = CALL_NATIVE(overflow_, "ip", arg);
  CJval zero = JCONST(nuint, 0);
  CJval cond = JINSN(eq, ovf, zero);
  INIT_LABEL(lbl);
  JINSN(branch_if, cond, &lbl);
  cc_except(j, "StackOverflow");
  JINSN(label, &lbl);
}

JIT_CODE(Func_Member) {
  CJval _reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval reg = JADDR(_reg, -SZ_INT *2);
  JSTORER(j->shred, JOFF(VM_Shred, reg), reg);
  CJval code  = JLOADR(reg, 0, void_ptr);
  CJval depth = JLOADR(reg, SZ_INT, nuint);
  CJval _mem = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval mem = JINSN(add, _mem, depth);
  JSTORER(j->shred, JOFF(VM_Shred, mem), mem);
  Jval arg0[] =  { j->shred, code };
  CALL_NATIVE(copy_member_args, "vpp", arg0);
  CJval zero = JCONST(nuint, 0);
  jit_overflow(j);
  CJval f = JLOADR(code, JOFF(VM_Code, native_func), void_ptr);
  CJval obj = JLOADR(mem, 0, void_ptr);
  CJval flag = JLOADR(code, JOFF(VM_Code, flag), nuint);
  CJval ctor = cc_get_flag(j, flag, NATIVE_CTOR);
  CJval cond1 = JINSN(eq, ctor, zero);
  INIT_LABEL(lbl_ctor);
  INIT_LABEL(lbl_end);

  JINSN(branch_if, cond1, &lbl_ctor);
  Jval arg2[] = { obj, j->shred };
  JINSN(call_indirect, f, sig(&j->sig, "vpp"), arg2, 2, JIT_CALL_NOTHROW);

  JINSN(branch, &lbl_end);
  JINSN(label, &lbl_ctor);
  CJval len = JCONST(nuint, instr->m_val);
  CJval retval = JINSN(alloca, len);
  Jval arg3[] = { obj, retval, j->shred };
  JINSN(call_indirect, f, sig(&j->sig, "vppp"), arg3, 3, JIT_CALL_NOTHROW);
  Jval arg4[] = { retval, j->shred, len };
  CALL_NATIVE(dl_return_push, "vppp", arg4);
  JINSN(label, &lbl_end);
  JSTORER(j->shred, JOFF(VM_Shred, mem), _mem);
}


JIT_CODE(Func_Return) {
  CJval mem  = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval pc   = JLOADR(mem, -SZ_INT*2, nuint);
  JSTORER(j->shred, JOFF(VM_Shred, pc), pc);
  CJval code = JLOADR(mem, -SZ_INT*3, void_ptr);
  JSTORER(j->shred, JOFF(VM_Shred, code), code);
  CJval off1 = JCONST(nuint, SZ_INT*4);
  CJval off2 = JLOADR(mem, -SZ_INT*4, nuint);
  CJval off  = JINSN(add, off1, off2);
  CJval neg  = JINSN(neg, off);
  CJval _mem = JINSN(add, mem, neg);
  JSTORER(j->shred, JOFF(VM_Shred, mem), _mem);
}

JIT_CODE(Func_Static) {
  CJval _reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval reg = JADDR(_reg, -SZ_INT *2);
  JSTORER(j->shred, JOFF(VM_Shred, reg), reg);
  CJval code  = JLOADR(reg, 0, void_ptr);
  CJval depth = JLOADR(reg, SZ_INT, nuint);
  CJval _mem = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval mem = JINSN(add, _mem, depth);
  JSTORER(j->shred, JOFF(VM_Shred, mem), mem);
  CJval zero = JCONST(nuint, 0);

  INIT_LABEL(lbl);
  CJval stack = JLOADR(code, JOFF(VM_Code, stack_depth), nuint);
  CJval cond = JINSN(eq, stack, zero);
  JINSN(branch_if, cond, &lbl);
  CJval __reg = JINSN(sub, reg, stack);
  JSTORER(j->shred, JOFF(VM_Shred, reg), __reg);
  CJval __mem = JADDR(mem, SZ_INT);
  JINSN(memcpy, __mem, __reg, stack);
  JINSN(label, &lbl);

  jit_overflow(j);
  CJval len = JCONST(nuint, instr->m_val);
  CJval retval = JINSN(alloca, len);
  Jval arg2[] = { retval, j->shred };
  CJval f = JLOADR(code, JOFF(VM_Code, native_func), void_ptr);
  JINSN(call_indirect, f, sig(&j->sig, "vpp"), arg2, 2, JIT_CALL_NOTHROW);
  Jval arg[] = { retval, j->shred, len };
  CALL_NATIVE(dl_return_push, "vppp", arg);
  JSTORER(j->shred, JOFF(VM_Shred, mem), _mem);
}

JIT_CODE(Instr_Exp_Func) {
  Jval arg[] = { j->shred };
  CALL_NATIVE(shred_func_prepare, "vp", arg);

  CJval code  = JLOADR(j->shred, JOFF(VM_Shred, code), void_ptr);
  Jval depth = JLOADR(code, JOFF(VM_Code, stack_depth), nuint);

  INIT_LABEL(lbl);
  CJval zero = JCONST(nuint, 0);
  CJval cond = JINSN(eq, depth, zero);
  JINSN(branch_if, cond, &lbl);
//  CJval mem = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval _reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval reg = JINSN(sub, _reg, depth);
  JSTORER(j->shred, JOFF(VM_Shred, reg), reg);

  INIT_LABEL(lbl2);
  CJval flag = JLOADR(code, JOFF(VM_Code, flag), nuint);
  CJval this = cc_get_flag(j, flag, _NEED_THIS_);
  CJval cond2 = JINSN(eq, this, zero);
  JINSN(branch_if, cond2, &lbl2);
  CALL_NATIVE(shred_func_need_this, "vp", arg);
  CJval sz_int = JCONST(nuint, SZ_INT);
  CJval sub = JINSN(sub, depth, sz_int);
  JINSN(store, sub, depth);
  JINSN(label, &lbl2);
{
  CJval mem = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  JINSN(memcpy, mem, reg, depth);
}
  JINSN(label, &lbl);
  CALL_NATIVE(shred_func_finish, "vp", arg);
}


JIT_CODE(Call_Binary) {
  Jval arg[] = { j->shred };
  CALL_NATIVE(shred_func_prepare, "vp", arg);

  CJval code  = JLOADR(j->shred, JOFF(VM_Shred, code), void_ptr);
  const Type l = (Type)instr->m_val2;
  const Type r = *(Type*)instr->ptr;
  const m_uint size = l->size + (r ? r->size : 0);

  CJval _reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval reg = JADDR(_reg, -size);
  JSTORER(j->shred, JOFF(VM_Shred, reg), reg);

  INIT_LABEL(lbl);
  CJval flag = JLOADR(code, JOFF(VM_Code, flag), nuint);
  CJval this = cc_get_flag(j, flag, _NEED_THIS_);
  CJval zero = JCONST(nuint, 0);
  CJval cond = JINSN(eq, this, zero);
  JINSN(branch_if, cond, &lbl);
  CALL_NATIVE(shred_func_need_this, "vp", arg);
  JINSN(label, &lbl);
{
  CJval depth = JCONST(nuint, size);
  CJval mem = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  JINSN(memcpy, mem, reg, depth);
}
  CALL_NATIVE(shred_func_finish, "vp", arg);
}

JIT_CODE(member_function) {
  const Vector vtable = *(Vector*)instr->ptr;
  const Func f = (Func)vector_at(vtable, instr->m_val);
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval func = JCONST(void_ptr, f->code);
  JSTORER(reg, -SZ_INT, func);
}

JIT_CODE(Exp_Dot_Func) {
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval obj = JLOADR(reg, -SZ_INT, void_ptr);
  INIT_LABEL(lbl);
  CJval nil = JCONST(void_ptr, 0);
  CJval cond = JINSN(eq, obj, nil);
  JINSN(branch_if_not, cond, &lbl);
  cc_except(j, "NullPtrException");
  JINSN(label, &lbl);
  CJval type   = JLOADR(obj, JOFF(M_Object, type_ref), void_ptr);
  CJval nspc   = JLOADR(type, JOFF(Type, nspc), void_ptr);
  CJval vtable = JLOADR(nspc, JOFF(Nspc, vtable), void_ptr);
  CJval vec = JINSN(address_of, vtable);
  CJval arg[] = { vec, JCONST(nuint, instr->m_val) };
  CJval func = CALL_NATIVE(vector_at, "ppU", arg);
  JSTORER(reg, -SZ_INT, func);
}

JIT_CODE(Pre_Constructor) {
  const VM_Code pre_ctor = (VM_Code)instr->m_val;
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval obj = JLOADR(reg, -SZ_INT, void_ptr);
  JSTORER(reg, 0, obj);
  CJval code = JCONST(nuint, pre_ctor);
  JSTORER(reg, SZ_INT, code);
  CJval depth = JCONST(nuint, instr->m_val2);
  JSTORER(reg, SZ_INT*2, depth);
  CJval _reg = JADDR(reg, SZ_INT*3);
  JSTORER(j->shred, JOFF(VM_Shred, reg), _reg);
  if(!GET_FLAG(pre_ctor, NATIVE_NOT))
    jitcode_Func_Member(j, instr);
  else
    jitcode_Instr_Exp_Func(j, instr);

}

JIT_CODE(AutoLoopStart) {
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval obj = JLOADR(reg, -SZ_INT, void_ptr);
  INIT_LABEL(lbl);
  CJval nil = JCONST(void_ptr, 0);
  CJval cond = JINSN(eq, obj, nil);
  JINSN(branch_if_not, cond, &lbl);
  cc_except(j, "NullPtrException");
  JINSN(label, &lbl);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval array = JLOADR(data, 0, void_ptr);
  CJval mem = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  CJval idx = JLOADR(mem, instr->m_val, nuint);
  const Type t = *(Type*)instr->ptr;
  if(t) {
    INIT_LABEL(lbl);
    CJval zero = JCONST(nuint, 0);
    CJval cond = JINSN(eq, idx, zero);
    JINSN(branch_if_not, cond, &lbl);
    Jval new_arg[] = { j->shred };
    CJval _ptr = CALL_NATIVE(new_M_Object, "pp", new_arg);
    Jval ini_arg[] = { _ptr, JCONST(void_ptr, t) };
    CALL_NATIVE(initialize_object, "vpp", ini_arg);
    JSTORER(mem, instr->m_val + SZ_INT, _ptr);
    JINSN(label, &lbl);
    CJval ptr = JLOADR(mem, instr->m_val + SZ_INT, void_ptr);
    Jval arg[] = { array, idx };
    CJval ret = CALL_NATIVE(m_vector_addr, "ppU", arg);
    CJval ptr_data = JLOADR(ptr, JOFF(M_Object, data), void_ptr);
    JSTORER(ptr_data, 0, ret);
  } else {
    CJval ptr = JADDR(mem, instr->m_val + SZ_INT);
    Jval arg[] = { array, idx, ptr };
    CALL_NATIVE(m_vector_get, "vpUp", arg);
  }
}

JIT_CODE(AutoLoopEnd) {
  CJval mem = JLOADR(j->shred, JOFF(VM_Shred, mem), void_ptr);
  if(*(Type*)instr->ptr) {
    CJval ptr = JLOADR(mem, instr->m_val + SZ_INT, void_ptr);
    CJval arg[] = { ptr, j->shred };
    CALL_NATIVE(_release, "vpp", arg);
  }
  CJval idx = JLOADR(mem, instr->m_val, nuint);
  CJval one = JCONST(nuint, 1);
  CJval sum = JINSN(add, idx, one);
  JSTORER(mem, instr->m_val, sum);
  CJval reg = JLOADR(j->shred, JOFF(VM_Shred, reg), void_ptr);
  CJval obj = JLOADR(reg, -SZ_INT, void_ptr);
  CJval data = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval array = JLOADR(data, 0, void_ptr);
  CJval arg[] = { array };
  CJval size = CALL_NATIVE(m_vector_size, "Up", arg);
  CJval cond = JINSN(ge, sum, size);
  INIT_LABEL(lbl);
  JINSN(branch_if_not, cond, &lbl);
  next_pc(j, instr->m_val2);
  CJval _reg = JADDR(reg, -SZ_INT);
  JSTORER(j->shred, JOFF(VM_Shred, reg), _reg);
  JINSN(label, &lbl);
}

static inline void type_add_ref(Type t) { ++t->obj.ref_count; }
JIT_CODE(pop_array_class) {
  CJval reg   = push_reg(j, -SZ_INT);
  CJval obj   = JLOADR(reg, -SZ_INT, void_ptr);
  CJval tmp   = JLOADR(reg, 0, void_ptr);
  CJval tdata = JLOADR(tmp, JOFF(M_Object, data), void_ptr);
  CJval odata = JLOADR(obj, JOFF(M_Object, data), void_ptr);
  CJval array = JLOADR(tdata, 0, void_ptr);
  JSTORER(odata, 0, array);
  CJval arg0[] = { tdata };
  CALL_NATIVE(xfree, "vp", arg0);
  CJval arg1[] = { tmp };
  CALL_NATIVE(free_object, "vp", arg1);
  CJval type = JLOADR(obj, JOFF(M_Object, type_ref), void_ptr);
  CJval arg2[] = { type };
  CALL_NATIVE(type_add_ref, "vp", arg2);
}
*/
