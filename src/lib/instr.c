#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "vm.h"
#include "absyn.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "func.h"

#define overflow_(c) (c->mem >  (c->_mem + (SIZEOF_MEM) - (MEM_STEP)))

static void dl_return_push(const m_bit* retval, const VM_Shred shred, const m_uint size) {
  memcpy(REG(0), retval, size);
  PUSH_REG(shred, size);
}

INSTR(EOC) { GWDEBUG_EXE
  vm_shred_exit(shred);
}

INSTR(EOC2) { GWDEBUG_EXE
  shred->next_pc = shred->pc = 0;
  shreduler_remove(shred->vm_ref->shreduler, shred, 0);
}

INSTR(Reg_Pop_Word) { GWDEBUG_EXE
  POP_REG(shred,  instr->m_val);
}

INSTR(Reg_Push_Imm) { GWDEBUG_EXE
  if(!*(m_uint*)instr->ptr)
    memset(REG(0), 0, instr->m_val);
  else
    memcpy(REG(0), instr->ptr, instr->m_val);
  PUSH_REG(shred, instr->m_val);
}

INSTR(Reg_Push_Mem_Addr) { GWDEBUG_EXE
  *(m_uint**)REG(0) = (m_uint*)((*(m_uint*)instr->ptr ?
        shred->base : shred->mem) + instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Mem_Push_Imm) { GWDEBUG_EXE
  *(m_uint*)MEM(0) = instr->m_val;
  PUSH_MEM(shred,  SZ_INT);
}

INSTR(Mem_Set_Imm) { GWDEBUG_EXE
  *(m_uint**)MEM(instr->m_val) = *(m_uint**)instr->ptr;
}

INSTR(assign_func) { GWDEBUG_EXE
  if(!instr->m_val) {
    POP_REG(shred,  SZ_INT);
    **(m_uint**)REG(0) = *(m_uint*)REG(-SZ_INT);
  } else {
    POP_REG(shred,  SZ_INT * 3);
    Func f = (Func) * (m_uint*)REG(0);
    M_Object obj = *(M_Object*)REG(SZ_INT);
    *(Func*)(obj->data + instr->m_val2) = f;
    *(m_uint*)REG(-SZ_INT) = *(m_uint*)REG(0);
    *(Func**)REG(SZ_INT * 3) = &f;
  }
}

INSTR(Reg_Push_Mem) { GWDEBUG_EXE
  memcpy(REG(0), *(m_uint*)instr->ptr ?
      (shred->base + instr->m_val) : MEM(instr->m_val), instr->m_val2);
  PUSH_REG(shred, instr->m_val2);
}

INSTR(Reg_Push_Ptr) { GWDEBUG_EXE
  *(m_uint*)REG(-SZ_INT) = *(m_uint*)instr->ptr;
}

INSTR(Reg_Push_Code) { GWDEBUG_EXE
  const Func f  =  *(Func*)(instr->m_val2 ? REG(-SZ_INT) : MEM(instr->m_val));
  if(!f)
    Except(shred, "NullFuncPtrException");
  *(VM_Code*)REG(-SZ_INT) = f->code;
}

INSTR(Reg_Dup_Last) { GWDEBUG_EXE
  *(m_uint*)REG(0) = *(m_uint*)REG(-SZ_INT);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Reg_Add_Ref) { GWDEBUG_EXE
  const M_Object obj = instr->m_val ? **(M_Object**)REG(-SZ_INT) :
    *(M_Object*)REG(-SZ_INT);
  if(obj)
    ++obj->ref;
}

INSTR(Reg_Push_Me) { GWDEBUG_EXE
  *(M_Object*)REG(0) = shred->me;
  PUSH_REG(shred, SZ_INT);
}

INSTR(Reg_Push_Now) { GWDEBUG_EXE
  *(m_float*)REG(0) = vm->sp->pos;
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(Reg_Push_Maybe) { GWDEBUG_EXE
  *(m_uint*)REG(0) = (sp_rand(shred->vm_ref->sp) > (SP_RANDMAX / 2));
  PUSH_REG(shred, SZ_INT);
}

INSTR(Alloc_Word) { GWDEBUG_EXE
  memset(MEM(instr->m_val), 0, instr->m_val2);
  if(*(m_uint*)instr->ptr) {
    *(m_bit**)REG(0) = &*(unsigned char*)MEM(instr->m_val);
    PUSH_REG(shred, SZ_INT);
  } else {
    memcpy(REG(0), MEM(instr->m_val), instr->m_val2);
    PUSH_REG(shred, instr->m_val2);
  }
}

/* branching */
INSTR(Branch_Switch) { GWDEBUG_EXE
  const Map map = *(Map*)instr->ptr;
  POP_REG(shred,  SZ_INT);
  shred->next_pc = (m_int)map_get(map, (vtype) * (m_int*)REG(0));
  if(!shred->next_pc)
    shred->next_pc = instr->m_val;
}

INSTR(Branch_Eq_Int) { GWDEBUG_EXE
  POP_REG(shred,  SZ_INT * 2);
  if(*(m_uint*)REG(0) == *(m_uint*)REG(SZ_INT))
    shred->next_pc = instr->m_val;
}

INSTR(Branch_Neq_Int) { GWDEBUG_EXE
  POP_REG(shred,  SZ_INT * 2);
  if(*(m_uint*)REG(0) != *(m_uint*)REG(SZ_INT))
    shred->next_pc = instr->m_val;
}

INSTR(Branch_Eq_Float) { GWDEBUG_EXE
  POP_REG(shred,  SZ_FLOAT * 2);
  if(*(m_float*)REG(0) == *(m_float*)REG(SZ_FLOAT))
    shred->next_pc = instr->m_val;
}

INSTR(Branch_Neq_Float) { GWDEBUG_EXE
  POP_REG(shred,  SZ_FLOAT * 2);
  if(*(m_float*)REG(0) != *(m_float*)REG(SZ_FLOAT))
    shred->next_pc = instr->m_val;
}

INSTR(Init_Loop_Counter) { GWDEBUG_EXE
  POP_REG(shred,  SZ_INT);
  m_int* sp = (m_int*)REG(0);
  (*(m_int*)instr->m_val) = (*sp >= 0 ? *sp : -*sp);
}

INSTR(Reg_Push_Deref) { GWDEBUG_EXE
  *(m_int*)REG(0) = *(m_int*)instr->m_val;
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Dec_int_Addr) { GWDEBUG_EXE
  --(*((m_int*)(instr->m_val)));
}

INSTR(Goto) { GWDEBUG_EXE
  shred->next_pc = instr->m_val;
}

ANN static VM_Shred init_spork_shred(const VM_Shred shred, const VM_Code code) {
  const VM_Shred sh = new_vm_shred(code);
  sh->parent = shred;
  if(!shred->child.ptr)
    vector_init(&shred->child);
  vector_add(&shred->child, (vtype)sh);
  sh->_mem = sh->base;
  sh->base = shred->base;
  vm_add_shred(shred->vm_ref, sh);
  return sh;
}

INSTR(Spork) { GWDEBUG_EXE
  m_uint this_ptr = 0;
  const VM_Code code = *(VM_Code*)REG(-SZ_INT);
  const VM_Shred sh = init_spork_shred(shred, code);
  POP_REG(shred,  SZ_INT*2);
  const Func func = *(Func*)REG(0);
  if(GET_FLAG(func, ae_flag_member)) {
    POP_REG(shred,  SZ_INT);
    this_ptr = *(m_uint*)REG(0);
  }
  POP_REG(shred,  instr->m_val);
  memcpy(sh->reg, shred->reg, instr->m_val);
  if(*(m_uint*)instr->ptr)
    memcpy(sh->mem, shred->mem, *(m_uint*)instr->ptr);
  PUSH_REG(sh, instr->m_val);

  if(GET_FLAG(func, ae_flag_member)) {
    *(m_uint*)sh->reg = this_ptr;
    PUSH_REG(sh, SZ_INT);
  }
  if(*(m_uint*)instr->ptr && GET_FLAG(code, _NEED_THIS_)) // sporked function
    *(m_uint*)sh->mem = this_ptr;
  *(Func*)sh->reg = func;
  PUSH_REG(sh, SZ_INT);
  *(M_Object*)REG(0) = sh->me;
  PUSH_REG(shred,  SZ_INT);
  if(instr->m_val2)
    ADD_REF(code)
}

// LCOV_EXCL_START
// IMPROVE ME
ANN static void handle_overflow(const VM_Shred shred) {
  gw_err("[Gwion](VM): StackOverflow: shred[id=%" UINT_F ":%s], PC=[%" UINT_F "]\n",
         shred->xid, shred->name, shred->pc);
  vm_shred_exit(shred);
}
// LCOV_EXCL_STOP

ANN static void shred_func_prepare(const VM_Shred shred) {
  POP_REG(shred,  SZ_INT * 2);
  const VM_Code code = *(VM_Code*)REG(0);
  const m_uint local_depth = *(m_uint*)REG(SZ_INT);
  const m_uint prev_stack = shred->mem == shred->base ? 0 : *(m_uint*)MEM(-SZ_INT);
  const m_uint push = prev_stack + local_depth;

  PUSH_MEM(shred, push + SZ_INT*4);
  *(m_uint*)MEM(-SZ_INT*4)  = push;
  *(m_uint*)MEM(-SZ_INT*3)  = (m_uint)shred->code;
  *(m_uint*)MEM(-SZ_INT*2)  = shred->pc + 1;
  *(m_uint*)MEM(-SZ_INT)  = (m_uint)code->stack_depth;
  shred->next_pc = 0;
  shred->code = code;
}

ANN static inline void shred_func_need_this(const VM_Shred shred) {
  *(m_uint*)MEM(0) = *(m_uint*)REG(shred->code->stack_depth - SZ_INT);
  PUSH_MEM(shred,  SZ_INT);
}

ANN static inline void shred_func_finish(const VM_Shred shred) {
  if(GET_FLAG(shred->code, _NEED_THIS_))
    POP_MEM(shred, SZ_INT);
  if(overflow_(shred))
    handle_overflow(shred);
}

INSTR(Instr_Exp_Func) { GWDEBUG_EXE
  shred_func_prepare(shred);
  const VM_Code code = shred->code;
  const m_uint stack_depth = code->stack_depth;
  if(stack_depth) {
    POP_REG(shred,  stack_depth);
    if(GET_FLAG(shred->code, _NEED_THIS_))
      shred_func_need_this(shred);
    memcpy(shred->mem, shred->reg, stack_depth
        - (GET_FLAG(code, _NEED_THIS_) ? SZ_INT : 0));
  }
  shred_func_finish(shred);
  return;
}

INSTR(Call_Binary) { GWDEBUG_EXE
  const Type l = (Type)instr->m_val2;
  const Type r = *(Type*)instr->ptr;
  shred_func_prepare(shred);
  POP_REG(shred,  l->size + (r ? r->size : 0));
  if(GET_FLAG(shred->code, _NEED_THIS_))
    shred_func_need_this(shred);
  memcpy(MEM(0), REG(0), l->size);
  if(r)
    memcpy(MEM(l->size), REG(l->size), r->size);
  shred_func_finish(shred);
}

INSTR(Dot_Static_Func) { GWDEBUG_EXE
  *(m_uint*)REG(-SZ_INT) = instr->m_val;
}

INSTR(member_function) { GWDEBUG_EXE
  *(VM_Code*)REG(-SZ_INT) = ((Func)vector_at(*(Vector*)instr->ptr, instr->m_val))->code;
}

INSTR(Exp_Dot_Func) { GWDEBUG_EXE
  const M_Object obj = *(M_Object*)REG(-SZ_INT);
  if(!obj)
    Except(shred, "NullPtrException");
  *(Func*)REG(-SZ_INT) = (Func)vector_at(obj->vtable, instr->m_val);
}

INSTR(Func_Static) { GWDEBUG_EXE
  const m_bit retval[instr->m_val];

  POP_REG(shred,  SZ_INT * 2);
  const VM_Code func = *(VM_Code*)REG(0);
  const f_sfun f     = (f_sfun)func->native_func;
  const m_uint local_depth = *(m_uint*)REG(SZ_INT);
  const m_uint stack_depth = func->stack_depth;
  PUSH_MEM(shred,  local_depth);
  if(stack_depth) {
    POP_REG(shred, stack_depth);
    memcpy(shred->mem + SZ_INT, shred->reg, stack_depth);
  }
  if(overflow_(shred)) {
    handle_overflow(shred);
    return;
  }
  f(retval, shred);
  dl_return_push(retval, shred, instr->m_val);
  POP_MEM(shred, local_depth);
}

ANN static void copy_member_args(const VM_Shred shred, const VM_Code func) {
  m_uint stack_depth = func->stack_depth;
  POP_REG(shred,  stack_depth);
  m_bit* mem = shred->mem;
  if(GET_FLAG(func, _NEED_THIS_)) {
    *(m_uint*)MEM(0) = *(m_uint*)REG(-SZ_INT + stack_depth);
    mem +=SZ_INT;
    stack_depth -= SZ_INT;
  }
  memcpy(mem, shred->reg, stack_depth);
}

INSTR(Func_Member) { GWDEBUG_EXE
  POP_REG(shred,  SZ_INT * 2);
  const VM_Code func = *(VM_Code*)REG(0);
  const m_uint local_depth =   *(m_uint*)REG(SZ_INT);
  PUSH_MEM(shred,  local_depth);
  if(func->stack_depth)
    copy_member_args(shred, func);
  if(overflow_(shred)) {
    handle_overflow(shred);
    return;
  }
  if(GET_FLAG(func, NATIVE_CTOR)) {
    const f_xtor f = (f_xtor)func->native_func;
    f(*(M_Object*)MEM(0), shred);
  } else {
    const m_bit retval[instr->m_val];
    const f_mfun f = (f_mfun)func->native_func;
    f((*(M_Object*)MEM(0)), retval, shred);
    dl_return_push(retval, shred, instr->m_val);
  }
  POP_MEM(shred,  local_depth);
}

INSTR(Func_Return) { GWDEBUG_EXE
  shred->next_pc = *(m_uint*)MEM(-SZ_INT*2);
  const VM_Code func = *(VM_Code*)MEM(-SZ_INT*3);
  POP_MEM(shred, *(m_uint*)MEM(-SZ_INT*4) + SZ_INT*4);
  shred->code = func;
}

ANN static void call_pre_constructor(const VM * vm, const VM_Shred shred,
  const VM_Code pre_ctor, const m_uint stack_offset) {
  *(m_uint*)REG(0) = *(m_uint*)REG(-SZ_INT); // ref dup last
  *(VM_Code*)REG(SZ_INT) = pre_ctor;
  *(m_uint*)REG(SZ_INT*2) = stack_offset;
  PUSH_REG(shred,  SZ_INT*3);
  if(!GET_FLAG(pre_ctor, NATIVE_NOT))
    Func_Member(vm, shred, NULL);
  else
    Instr_Exp_Func(vm, shred, NULL);
}

INSTR(Reg_Push_This) { GWDEBUG_EXE
  *(m_uint*)REG(0) = *(m_uint*)MEM(0);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Pre_Constructor) { GWDEBUG_EXE
  call_pre_constructor(vm, shred, (VM_Code)instr->m_val, instr->m_val2);
}

INSTR(Instantiate_Object) { GWDEBUG_EXE
  instantiate_object(shred, *(Type*)instr->ptr);
  vector_add(&shred->gc1, *(vtype*)REG(-SZ_INT));
}

INSTR(Alloc_Member) { GWDEBUG_EXE
  const M_Object obj = *(M_Object*)MEM(0);
  m_bit* data = obj->data + instr->m_val;
  memset(data, 0, instr->m_val2);
  if(*(m_uint*)instr->ptr)
    *(m_bit**)REG(0) = &*data;
  else
    memset(REG(0), *data, instr->m_val2);
  PUSH_REG(shred, instr->m_val2);
}

INSTR(Dot_Static_Data) { GWDEBUG_EXE
  const Type t = *(Type*)REG(-SZ_INT);
  m_bit* data = t->info->class_data + instr->m_val;
  if(*(m_uint*)instr->ptr)
    *(m_bit**)REG(-SZ_INT) = &*data;
  else
    memcpy(REG(-SZ_INT), data, instr->m_val2);
  PUSH_REG(shred, instr->m_val2 - SZ_INT);

}

INSTR(Dot_Static_Import_Data) { GWDEBUG_EXE
  if(!*(m_uint*)instr->ptr)
    memcpy(REG(0), (m_bit*)instr->m_val, instr->m_val2);
  else
    *(m_bit**)REG(0) = &*(m_bit*)(instr->m_val);
  PUSH_REG(shred, instr->m_val2);
}

INSTR(Exp_Dot_Data) { GWDEBUG_EXE
  const M_Object obj  = *(M_Object*)REG(-SZ_INT);
  if(!obj) Except(shred, "NullPtrException");
  const m_bit* c = (m_bit*)(obj->data + instr->m_val);
  if(!*(m_uint*)instr->ptr) {
    memcpy(REG(-SZ_INT), c, instr->m_val2);
    PUSH_REG(shred, instr->m_val2 - SZ_INT);
  }
  else
    memcpy(REG(-SZ_INT), &c, SZ_INT);
}

INSTR(Release_Object) { GWDEBUG_EXE
  release(*(M_Object*)MEM(instr->m_val), shred);
}

INSTR(start_gc) { GWDEBUG_EXE
  if(!shred->gc.ptr) //  dynamic assign
    vector_init(&shred->gc);
  vector_add(&shred->gc, (vtype)NULL); // enable scoping
}

INSTR(add2gc) { GWDEBUG_EXE
  vector_add(&shred->gc, *(vtype*)REG(-SZ_INT)); // enable scoping
}

INSTR(stop_gc) { GWDEBUG_EXE
  M_Object o;
  while((o = (M_Object)vector_pop(&shred->gc)))
    release(o, shred);
}

INSTR(AutoLoopStart) { GWDEBUG_EXE
  const M_Object o =  *(M_Object*)REG(-SZ_INT);
  M_Object ptr = *(M_Object*)MEM(instr->m_val + SZ_INT);
  const Type t = *(Type*)instr->ptr;
  if(t) {
    if(!*(m_uint*)MEM(instr->m_val)) {
      ptr = new_M_Object(shred);
      initialize_object(ptr, t);
      *(M_Object*)MEM(instr->m_val + SZ_INT) = ptr;
    }
    *(m_bit**)ptr->data = m_vector_addr(ARRAY(o), *(m_uint*)MEM(instr->m_val));
  } else
    m_vector_get(ARRAY(o), *(m_uint*)MEM(instr->m_val), MEM(instr->m_val + SZ_INT));
}

INSTR(AutoLoopEnd) { GWDEBUG_EXE
  const M_Object o =  *(M_Object*)REG(-SZ_INT);
  const M_Object ptr = *(M_Object*)MEM(instr->m_val + SZ_INT);
  const Type t = *(Type*)instr->ptr;
  ++(*(m_uint*)MEM(instr->m_val));
  if(*(m_uint*)MEM(instr->m_val) >= m_vector_size(ARRAY(o))) {
    shred->next_pc = instr->m_val2;
    POP_REG(shred, SZ_INT);
  }
  if(t)
    release(ptr, shred);
}

#ifdef GWCOV
INSTR(InstrCoverage) { GWDEBUG_EXE
  const m_str str = code_name(shred->name, 1);
  if(!strcmp(str, "[dtor]"))
    return;
  const size_t len = str ? strlen(str) : 6;
  char c[len + 4];
  sprintf(c, "%scov", str ? str : "gwion.");
  FILE* file = fopen(c, "a");
  fprintf(file, "%" UINT_F " %s \n", instr->m_val, instr->m_val2 ? "end" : "ini");
  fclose(file);
}
#endif

#ifdef OPTIMIZE
INSTR(PutArgsInMem) { GWDEBUG_EXE
  POP_REG(shred, instr->m_val)
  memcpy(shred->mem, shred->reg, instr->m_val);
}

INSTR(ConstPropSet) { GWDEBUG_EXE
  if(*(m_uint*)instr->ptr >= 2) {
    *(m_int**)(shred->reg) = (m_int*)(shred->mem + instr->m_val);
    PUSH_REG(shred, SZ_INT);
    return;
  }
  *(m_int*)(shred->mem + instr->m_val) = instr->m_val2;
  if(*(m_bool*)instr->ptr)
    *(m_int**)(shred->reg) = (m_int*)(shred->mem + instr->m_val);
  else
    *(m_int*)(shred->reg) = instr->m_val2;
  PUSH_REG(shred, SZ_INT);
  *(m_uint*)instr->ptr = 2 + *(m_uint*)instr->ptr;
}

INSTR(ConstPropGet) { GWDEBUG_EXE
  if(!*(m_uint*)instr->ptr)
    memcpy(REG(0), MEM(instr->m_val2), SZ_INT);
  else {
    memcpy(REG(0), &instr->m_val, SZ_INT);
//    memcpy(MEM(instr->m_val2), &instr->m_val, SZ_INT);
//    *(m_uint*)instr->ptr = 0;
  }
  PUSH_REG(shred, SZ_INT);
}


INSTR(InlineStart) { GWDEBUG_EXE
  const VM_Code f =  *(VM_Code*)instr->ptr;
  m_uint offset = f->stack_depth;

  PUSH_MEM(shred, instr->m_val + offset + SZ_INT);
  *(m_uint*)MEM(-SZ_INT) = instr->m_val2;
  POP_REG(shred, offset + SZ_INT)
  if(GET_FLAG(f, _NEED_THIS_)) {
    *(M_Object*)MEM(0) =  *(M_Object*)REG(instr->m_val-SZ_INT);
    offset -= SZ_INT;
    PUSH_MEM(shred, SZ_INT);
  }
  memcpy(shred->mem, shred->reg, offset);
  if(GET_FLAG(f, _NEED_THIS_))
    POP_MEM(shred, SZ_INT);
}

INSTR(InlineStop) { GWDEBUG_EXE
  POP_MEM(shred, instr->m_val + instr->m_val2 + SZ_INT)
}

INSTR(InlineGoto) { GWDEBUG_EXE
  shred->next_pc = instr->m_val + *(m_uint*)MEM(-SZ_INT);
}
#endif
