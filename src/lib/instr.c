#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "map.h"
#include "absyn.h"
#include "err_msg.h"
#include "vm.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "func.h"
#include "array.h"
#include "nspc.h"

ANN static inline m_bool overflow_(const VM_Shred c) {
  return c->mem >  ((c->_reg + SIZEOF_REG) + (SIZEOF_MEM) - (MEM_STEP));
}

static inline void dl_return_push(const m_bit* retval, const VM_Shred shred,
  const m_uint size  __attribute__((unused))) {
  *(m_uint*)REG(0) = *(m_uint*)retval;
  PUSH_REG(shred, SZ_INT);
}

static inline void dl_return_push2(const m_bit* retval, const VM_Shred shred,
  const m_uint size  __attribute__((unused))) {
  *(m_float*)REG(0) = *(m_float*)retval;
  PUSH_REG(shred, SZ_FLOAT);
}

static inline void dl_return_push3(const m_bit* retval, const VM_Shred shred, const m_uint size) {
    memcpy(REG(0), retval, size);
    PUSH_REG(shred, size);
}

static void (*dl_return[])(const m_bit*, const VM_Shred, const m_uint) =
  { dl_return_push, dl_return_push2, dl_return_push3 };

INSTR(EOC) { GWDEBUG_EXE
  vm_shred_exit(shred);
}

INSTR(DTOR_EOC) { GWDEBUG_EXE
  const M_Object o = *(M_Object*)MEM(0);
  o->type_ref = o->type_ref->parent;
  o->ref = 1;
  _release(o, shred);
  _release(shred->me, shred);
  vm_shred_exit(shred);
}

INSTR(EOC2) { GWDEBUG_EXE
  shred->pc = 0;
  shreduler_remove(shred->vm->shreduler, shred, 0);
}

INSTR(RegPop) { GWDEBUG_EXE
  POP_REG(shred, instr->m_val);
}

#define describe_regpushimmxxx(name, type, size) \
INSTR(RegPush##name) { GWDEBUG_EXE               \
  *(type*)REG(0) = *(type*)instr->ptr;           \
  PUSH_REG(shred, size);                         \
}

describe_regpushimmxxx(Imm,  m_int,   SZ_INT)
describe_regpushimmxxx(Imm2, m_float, SZ_FLOAT)
INSTR(RegPushImm3) { GWDEBUG_EXE
  memcpy(REG(0), instr->ptr, instr->m_val2);
  PUSH_REG(shred, instr->m_val2);
}

INSTR(MemPushImm) { GWDEBUG_EXE
  *(m_uint*)MEM(0) = instr->m_val;
  PUSH_MEM(shred, SZ_INT);
}

INSTR(MemSetImm) { GWDEBUG_EXE
  *(m_uint*)MEM(instr->m_val) = instr->m_val2;
}

#define describe_regpushxxx(name, type, size)                          \
INSTR(RegPush##name) { GWDEBUG_EXE                                     \
  const m_bit* data = *(m_uint*)instr->ptr ? shred->base : shred->mem; \
  *(type*)REG(0) = *(type*)(data + instr->m_val);                      \
  PUSH_REG(shred, size);                                               \
}

describe_regpushxxx(Mem,  m_int,   SZ_INT)
describe_regpushxxx(Mem2, m_float, SZ_FLOAT)
INSTR(RegPushMem3) { GWDEBUG_EXE
  const m_bit* data = *(m_uint*)instr->ptr ? shred->base : shred->mem;
  memcpy(REG(0), (data + instr->m_val), instr->m_val2);
  PUSH_REG(shred, instr->m_val2);
}
INSTR(RegPushMem4) { GWDEBUG_EXE
  const m_bit* data = *(m_uint*)instr->ptr ? shred->base : shred->mem;
  *(m_bit**)REG(0) = (m_bit*)(data + instr->m_val);
  PUSH_REG(shred, SZ_INT);
}


INSTR(RegPushPtr) { GWDEBUG_EXE
  *(m_uint*)REG(-SZ_INT) = instr->m_val;
}

INSTR(RegPushCode) { GWDEBUG_EXE
  const Func f = *(Func*)REG(-SZ_INT);
  if(!f)
    Except(shred, "NullFuncPtrException");
  *(VM_Code*)REG(-SZ_INT) = f->code;
}

INSTR(RegDup) { GWDEBUG_EXE
  *(M_Object*)REG(0) = *(M_Object*)REG(-SZ_INT);
  PUSH_REG(shred, SZ_INT);
}

INSTR(RegAddRef) { GWDEBUG_EXE
  const M_Object obj = instr->m_val ? **(M_Object**)REG(-SZ_INT) :
    *(M_Object*)REG(-SZ_INT);
  if(obj)
    ++obj->ref;
}

INSTR(RegPushMe) { GWDEBUG_EXE
  *(M_Object*)REG(0) = shred->me;
  PUSH_REG(shred, SZ_INT);
}

INSTR(RegPushNow) { GWDEBUG_EXE
  *(m_float*)REG(0) = shred->vm->bbq->pos;
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(RegPushMaybe) { GWDEBUG_EXE
  *(m_uint*)REG(0) = gw_rand(shred->vm->rand) > (UINT32_MAX / 2);
  PUSH_REG(shred, SZ_INT);
}

INSTR(AllocWord) { GWDEBUG_EXE
  *(m_uint*)MEM(instr->m_val) = 0;
  *(m_uint*)REG(0) = 0; // MEM(instr->m_val);
  PUSH_REG(shred, SZ_INT);
}

INSTR(AllocWord2) { GWDEBUG_EXE
  *(m_float*)MEM(instr->m_val) = 0.0;
  *(m_float*)REG(0) = 0.0; // MEM(instr->m_val);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(AllocWord3) { GWDEBUG_EXE
  memset(MEM(instr->m_val), 0, instr->m_val2);
  memset(REG(0), 0, instr->m_val2); // MEM(instr->m_val)
  PUSH_REG(shred, instr->m_val2);
}

INSTR(AllocWord4) { GWDEBUG_EXE
  memset(MEM(instr->m_val), 0, instr->m_val2);
  *(m_bit**)REG(0) = (m_bit*)MEM(instr->m_val);
  PUSH_REG(shred, SZ_INT);
}

/* branching */
INSTR(BranchSwitch) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  const Map map = (Map)instr->m_val2;
  shred->pc = (m_int)map_get(map, *(m_int*)REG(0));
  if(!shred->pc)
    shred->pc = instr->m_val;
}

#define branch(name, type, sz, op)      \
INSTR(Branch##name) { GWDEBUG_EXE      \
  POP_REG(shred, sz * 2);               \
  if(*(type*)REG(0) op *(type*)REG(sz)) \
    shred->pc = instr->m_val;           \
}
branch(EqInt,    m_int,     SZ_INT, ==)
branch(NeqInt,   m_int,     SZ_INT, !=)
branch(EqFloat, m_float,  SZ_FLOAT, ==)
branch(NeqFloat, m_float, SZ_FLOAT, !=)

INSTR(InitLoopCounter) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  (*(m_uint*)instr->m_val) = labs(*(m_int*)REG(0));
}

INSTR(RegPushDeref) { GWDEBUG_EXE
  *(m_uint*)REG(0) =  **(m_uint**)instr->ptr;
  PUSH_REG(shred, SZ_INT);
}

INSTR(RegPushDeref2) { GWDEBUG_EXE
  *(m_float*)REG(0) = *(m_float*)instr->ptr;
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(RegPushDeref3) { GWDEBUG_EXE
  memcpy(REG(0), *(void**)instr->ptr, instr->m_val2);
  PUSH_REG(shred, instr->m_val2);
}

INSTR(DecIntAddr) { GWDEBUG_EXE
  --(*((m_int*)(instr->m_val)));
}

INSTR(Goto) { GWDEBUG_EXE
  shred->pc = instr->m_val;
}

ANN static VM_Shred init_spork_shred(const VM_Shred shred, const VM_Code code) {
  const VM_Shred sh = new_vm_shred(code);
  ADD_REF(code)
  sh->parent = shred;
  if(!shred->child.ptr)
    vector_init(&shred->child);
  vector_add(&shred->child, (vtype)sh);
  sh->base = shred->base;
  vm_add_shred(shred->vm, sh);
  return sh;
}
#include "value.h"
static inline void push_me(VM_Shred shred, VM_Shred sh) {
  *(M_Object*)REG(0) = sh->me;
  PUSH_REG(shred, SZ_INT);
}

INSTR(SporkFunc) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT*2);
  const VM_Code code = *(VM_Code*)REG(SZ_INT);
  const VM_Shred sh = init_spork_shred(shred, code);
  const Func func = *(Func*)REG(0);
  const m_uint need = GET_FLAG(code, _NEED_THIS_) ? SZ_INT : 0;
  shred->reg -= instr->m_val + need;
  if(instr->m_val) {
    for(m_uint i = 0; i < instr->m_val; i+= SZ_INT)
      *(m_uint*)(sh->reg + i) = *(m_uint*)REG(i);
    sh->reg += instr->m_val;
  }
  if(need) {
    *(M_Object*)sh->reg = *(M_Object*)REG(instr->m_val);
    PUSH_REG(sh, SZ_INT);
  }
  push_me(shred, sh);
  *(Func*)sh->reg = func;
  PUSH_REG(sh, SZ_INT);
}

INSTR(SporkExp) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT*2);
  const VM_Code code = *(VM_Code*)REG(SZ_INT);
  const VM_Shred sh = init_spork_shred(shred, code);
  if(GET_FLAG(code, _NEED_THIS_))
    POP_REG(shred, SZ_INT);
  for(m_uint i = 0; i < instr->m_val; i+= SZ_INT)
    *(m_uint*)(sh->reg + i + SZ_INT) = *(m_uint*)MEM(i);
  push_me(shred, sh);
}

ANN static void shred_func_prepare(const VM_Shred shred) {
  POP_REG(shred, SZ_INT * 2);
  const VM_Code code = *(VM_Code*)REG(0);

  const m_uint local_depth = *(m_uint*)REG(SZ_INT);
  const m_uint prev_stack = *(m_uint*)MEM(-SZ_INT);
  const m_uint push = prev_stack + local_depth;

  PUSH_MEM(shred, push + SZ_INT*4);
  *(m_uint*)  MEM(-SZ_INT*4) = push;
  *(VM_Code*) MEM(-SZ_INT*3) = shred->code;
  *(m_uint*)  MEM(-SZ_INT*2) = shred->pc;
  *(m_uint*)  MEM(-SZ_INT)   = (m_uint)code->stack_depth;
  shred->pc = 0;
  shred->code = code;
}

ANN static inline void shred_func_need_this(const VM_Shred shred) {
  *(m_uint*)MEM(0) = *(m_uint*)REG(shred->code->stack_depth - SZ_INT);
  PUSH_MEM(shred, SZ_INT);
}

ANN static inline void shred_func_finish(const VM_Shred shred) {
  if(GET_FLAG(shred->code, _NEED_THIS_))
    POP_MEM(shred, SZ_INT);
  if(overflow_(shred))
    Except(shred, "StackOverflow");
}

INSTR(FuncPtr) { GWDEBUG_EXE
  const VM_Code code = *(VM_Code*)REG(-SZ_INT*2);
  if(GET_FLAG(code, NATIVE_NOT))
    FuncUsr(shred, instr);
  else if(GET_FLAG(code, _NEED_THIS_))
    FuncMember(shred, instr);
  else
    FuncStatic(shred, instr);
}

INSTR(FuncUsr) { GWDEBUG_EXE
  shred_func_prepare(shred);
  const VM_Code code = shred->code;
  m_uint stack_depth = code->stack_depth;
  if(stack_depth) {
    POP_REG(shred, stack_depth);
    if(GET_FLAG(shred->code, _NEED_THIS_)) {
      shred_func_need_this(shred);
      stack_depth -= SZ_INT;
    }
    for(m_uint i = 0; i < stack_depth; i+= SZ_INT)
      *(m_uint*)MEM(i) = *(m_uint*)REG(i);
  }
  shred_func_finish(shred);
}

INSTR(DotFunc) { GWDEBUG_EXE
  const M_Object obj = *(M_Object*)REG(-SZ_INT);
  if(!obj)
    Except(shred, "NullPtrException");
  *(Func*)REG(-SZ_INT) = (Func)vector_at(&obj->type_ref->nspc->vtable, instr->m_val);
}

INSTR(FuncStatic) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  const VM_Code code = *(VM_Code*)REG(0);
  const m_uint local_depth = *(m_uint*)REG(SZ_INT);
  PUSH_MEM(shred, local_depth);
  const m_uint stack_depth = code->stack_depth;
  if(stack_depth) {
    POP_REG(shred, stack_depth);
    for(m_uint i = 0; i < stack_depth; i+= SZ_INT)
      *(m_uint*)MEM(i) = *(m_uint*)REG(i);
  }
  if(overflow_(shred))
    Except(shred, "StackOverflow");
  const m_bit retval[instr->m_val];
  const f_sfun f = (f_sfun)code->native_func;
  f(retval, shred);
  if(instr->m_val)
    dl_return[instr->m_val2](retval, shred, instr->m_val);
  POP_MEM(shred, local_depth);
}

ANN static inline void copy_member_args(const VM_Shred shred, const VM_Code func) {
  const m_uint stack_depth = func->stack_depth;
  const m_uint depth = stack_depth -SZ_INT;
  POP_REG(shred, stack_depth);
  *(m_uint*)MEM(0) = *(m_uint*)REG(depth);
    for(m_uint i = 0; i < stack_depth; i+= SZ_INT)
      *(m_uint*)MEM(i+SZ_INT) = *(m_uint*)REG(i);
}

INSTR(FuncMember) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  const VM_Code code = *(VM_Code*)REG(0);
  const m_uint local_depth =   *(m_uint*)REG(SZ_INT);
  PUSH_MEM(shred, local_depth);
  copy_member_args(shred, code);
  if(overflow_(shred))
    Except(shred, "StackOverflow");
  if(GET_FLAG(code, NATIVE_CTOR)) {
    const f_xtor f = (f_xtor)code->native_func;
    f(*(M_Object*)MEM(0), shred);
  } else {
    const m_bit retval[instr->m_val];
    const f_mfun f = (f_mfun)code->native_func;
    f((*(M_Object*)MEM(0)), retval, shred);
    if(instr->m_val)
      dl_return[instr->m_val2](retval, shred, instr->m_val);
  }
  POP_MEM(shred, local_depth);
}

INSTR(FuncReturn) { GWDEBUG_EXE
  shred->pc = *(m_uint*)MEM(-SZ_INT*2);
  shred->code = *(VM_Code*)MEM(-SZ_INT*3);
  POP_MEM(shred, *(m_uint*)MEM(-SZ_INT*4) + SZ_INT*4);
}

INSTR(PreCtor) { GWDEBUG_EXE
  const VM_Code pre_ctor = (VM_Code)instr->m_val;
  *(m_uint*)REG(0) = *(m_uint*)REG(-SZ_INT);
  *(VM_Code*)REG(SZ_INT) = pre_ctor;
  *(m_uint*)REG(SZ_INT*2) = instr->m_val2;
  PUSH_REG(shred, SZ_INT*3);
  if(!GET_FLAG(pre_ctor, NATIVE_NOT))
    FuncMember(shred, NULL);
  else
    FuncUsr(shred, NULL);
}

INSTR(ObjectInstantiate) { GWDEBUG_EXE
  const M_Object o = new_object(shred, (Type)instr->m_val);
  *(M_Object*)REG(0) = o;
  PUSH_REG(shred, SZ_INT);
}

INSTR(PushNull) { GWDEBUG_EXE
  *(m_uint*)REG(0) = 0;
  PUSH_REG(shred, SZ_INT);
}

INSTR(PushNull2) { GWDEBUG_EXE
  *(m_float*)REG(0) = 0.0;
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(PushNull3) { GWDEBUG_EXE
  memset(REG(0), 0, instr->m_val2); // 0 <=> *data
  PUSH_REG(shred, instr->m_val2);
}

INSTR(AllocMember4) { GWDEBUG_EXE
  const M_Object obj = *(M_Object*)MEM(0);
  *(const m_bit**)REG(0) = obj->data + instr->m_val;
  PUSH_REG(shred, SZ_INT);
}

INSTR(DotStatic) { GWDEBUG_EXE
  const Type t = *(Type*)REG(-SZ_INT);
  m_uint *const data = (m_uint*)(t->nspc->class_data + instr->m_val);
  *(m_uint*)REG(-SZ_INT) = *data;
}

INSTR(DotStatic2) { GWDEBUG_EXE
  const Type t = *(Type*)REG(-SZ_INT);
  m_float *const data = (m_float*)(t->nspc->class_data + instr->m_val);
  *(m_float*)REG(-SZ_INT) = *data;
  PUSH_REG(shred, SZ_FLOAT - SZ_INT);
}

INSTR(DotStatic3) { GWDEBUG_EXE
  const Type t = *(Type*)REG(-SZ_INT);
  const m_bit* data = t->nspc->class_data + instr->m_val;
  memcpy(REG(-SZ_INT), data, instr->m_val2);
  PUSH_REG(shred, instr->m_val2 - SZ_INT);
}

INSTR(DotStatic4) { GWDEBUG_EXE
  const Type t = *(Type*)REG(-SZ_INT);
  const m_bit* data = t->nspc->class_data + instr->m_val;
  *(m_bit**)REG(-SZ_INT) = (m_bit*)data;
}

INSTR(DotImport) { GWDEBUG_EXE
  *(m_uint*)REG(0) =  *(m_uint*)instr->m_val;
  PUSH_REG(shred, SZ_INT);
}

INSTR(DotImport2) { GWDEBUG_EXE
  *(m_float*)REG(0) =  *(m_float*)instr->m_val;
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(DotImport3) { GWDEBUG_EXE
  memcpy(REG(0), (m_bit*)instr->m_val, instr->m_val2);
  PUSH_REG(shred, instr->m_val2);
}

INSTR(DotImport4) { GWDEBUG_EXE
  *(m_bit**)REG(0) = (m_bit*)(instr->m_val);
  PUSH_REG(shred, SZ_INT);
}

INSTR(DotMember) { GWDEBUG_EXE
  const M_Object obj  = *(M_Object*)REG(-SZ_INT);
  if(!obj)
    Except(shred, "NullPtrException");
  *(m_uint*)REG(-SZ_INT) = *(m_uint*)(obj->data + instr->m_val);
}

INSTR(DotMember2) { GWDEBUG_EXE
  const M_Object obj  = *(M_Object*)REG(-SZ_INT);
  if(!obj)
    Except(shred, "NullPtrException");
  *(m_float*)REG(-SZ_INT) = *(m_float*)(obj->data + instr->m_val);
  PUSH_REG(shred, SZ_FLOAT - SZ_INT);
}

INSTR(DotMember3) { GWDEBUG_EXE
  const M_Object obj  = *(M_Object*)REG(-SZ_INT);
  if(!obj)
    Except(shred, "NullPtrException");
  memcpy(REG(-SZ_INT), (obj->data + instr->m_val), instr->m_val2);
  PUSH_REG(shred, instr->m_val2 - SZ_INT);
}

INSTR(DotMember4) { GWDEBUG_EXE
  const M_Object obj  = *(M_Object*)REG(-SZ_INT);
  if(!obj)
    Except(shred, "NullPtrException");
  *(m_bit**)REG(-SZ_INT) = (m_bit*)(obj->data + instr->m_val);
}

INSTR(ObjectRelease) { GWDEBUG_EXE
  release(*(M_Object*)MEM(instr->m_val), shred);
}

INSTR(GcIni) { GWDEBUG_EXE
  vector_add(&shred->gc, (vtype)NULL);
}

INSTR(GcAdd) { GWDEBUG_EXE
  vector_add(&shred->gc, *(vtype*)REG(-SZ_INT));
}

INSTR(GcEnd) { GWDEBUG_EXE
  M_Object o;
  while((o = (M_Object)vector_pop(&shred->gc)))
    _release(o, shred);
}

INSTR(AutoLoopStart) { GWDEBUG_EXE
  const M_Object o =  *(M_Object*)REG(-SZ_INT);
  if(!o)
    Except(shred, "NullPtrException");
  const m_uint idx = *(m_uint*)MEM(instr->m_val);
  const Type t = (Type)instr->m_val2;
  if(t) {
    M_Object ptr = *(M_Object*)MEM(instr->m_val + SZ_INT);
    if(!idx) {
      ptr = new_object(shred, t);
      *(M_Object*)MEM(instr->m_val + SZ_INT) = ptr;
    }
    *(m_bit**)ptr->data = m_vector_addr(ARRAY(o), idx);
  } else
    m_vector_get(ARRAY(o), idx, MEM(instr->m_val + SZ_INT));
}

INSTR(AutoLoopEnd) { GWDEBUG_EXE
  if(instr->m_val2) {
    const M_Object ptr = *(M_Object*)MEM(instr->m_val + SZ_INT);
    _release(ptr, shred);
  }
  m_uint* idx = (m_uint*)MEM(instr->m_val);
  ++*idx;
  const M_Object o =  *(M_Object*)REG(-SZ_INT);
  if(*idx >= m_vector_size(ARRAY(o))) {
    shred->pc = *(m_uint*)instr->ptr;
    POP_REG(shred, SZ_INT);
  }
}

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
  else
    memcpy(REG(0), &instr->m_val, SZ_INT);
  PUSH_REG(shred, SZ_INT);
}
#endif

INSTR(PopArrayClass) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  const M_Object obj = *(M_Object*)REG(-SZ_INT);
  const M_Object tmp = *(M_Object*)REG(0);
  ARRAY(obj) = ARRAY(tmp);
  free_object(tmp);
  ADD_REF(obj->type_ref) // add ref to typedef array type
}
