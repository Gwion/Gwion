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

static void dl_return_push(const char* retval, VM_Shred shred, m_uint size) {
  memcpy(REG(0), retval, size);
  PUSH_REG(shred, size);
}

INSTR(EOC) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Shred [%i]: End of Code", shred->xid);
#endif
  release(shred->me, shred);
  shred->me = NULL;
}

INSTR(Reg_Pop_Word4) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] pop %i", instr->m_val);
#endif
  POP_REG(shred,  instr->m_val);
}

INSTR(Reg_Push_Imm) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push imm %p", (void*)instr->m_val);
#endif
  *(m_uint*)REG(0) = instr->m_val;
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Reg_Push_Imm2) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push imm2 %f", *(m_float*)instr->ptr);
#endif
  *(m_float*)REG(0) = *(m_float*)instr->ptr;
  PUSH_REG(shred,  SZ_FLOAT);
}

INSTR(Reg_Push_ImmX) {
  memset(shred->reg, 0, instr->m_val);
  PUSH_REG(shred,  instr->m_val);
}

INSTR(Reg_Push_Mem_Addr) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push mem addr %i %i", instr->m_val, instr->m_val2);
#endif
  *(m_uint**)REG(0) = (m_uint*)((instr->m_val2 ? shred->base : shred->mem) + instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Mem_Push_Imm) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[mem] push imm %i %i", instr->m_val, instr->m_val2);
#endif
  *(m_uint*)MEM(0) = instr->m_val;
  PUSH_MEM(shred,  SZ_INT);
}

INSTR(Mem_Set_Imm) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[mem] set imm [%i] %p", instr->m_val, *(m_uint*)instr->ptr);
#endif
  *(m_uint**)MEM(instr->m_val) = *(m_uint**)instr->ptr;
}

INSTR(assign_func) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "assign func");
#endif
  if(!instr->m_val) {
    POP_REG(shred,  SZ_INT * 2);
    **(m_uint**)REG(SZ_INT) = *(m_uint*)REG(0);
  } else {
    POP_REG(shred,  SZ_INT * 4);
    Func f = (Func) * (m_uint*)REG(SZ_INT);
    M_Object obj = *(M_Object*)REG(SZ_INT * 2);
    *(Func*)(obj->data + instr->m_val2) = f;
    *(m_uint*)REG(0) = *(m_uint*)REG(SZ_INT);
    *(Func**)REG(SZ_INT * 4) = &f;
  }
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Reg_Push_Mem) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] reg push '%s' [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  *(m_uint*)REG(0) = *(m_uint*)(instr->m_val2 ? (shred->base + instr->m_val) : MEM(instr->m_val));
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Reg_Push_Mem2) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] reg push float '%s' [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  *(m_float*)REG(0) = *(m_float*)(instr->m_val2 ? (shred->base + instr->m_val) : MEM(instr->m_val));
  PUSH_REG(shred,  SZ_FLOAT);
}

INSTR(Reg_Push_Mem_Complex) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] 'complex' push mem '%s' [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  *(m_complex*)REG(0) = *(m_complex*)(instr->m_val2 ? (shred->base + instr->m_val) : MEM(instr->m_val));
  PUSH_REG(shred, SZ_COMPLEX);
}

INSTR(Reg_Push_Mem_Vec3) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] 'vec3' push mem '%s' [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  *(m_vec3*)REG(0) = *(m_vec3*)(instr->m_val2 ? (shred->base + instr->m_val) : MEM(instr->m_val));
  PUSH_REG(shred, SZ_VEC3);
}

INSTR(Reg_Push_Mem_Vec4) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] 'vec4' push mem '%s' [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  *(m_vec4*)REG(0) = *(m_vec4*)(instr->m_val2 ? (shred->base + instr->m_val) : MEM(instr->m_val));
  PUSH_REG(shred, SZ_VEC4);
}

INSTR(Reg_Push_Ptr) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push ptr (%p)", *(m_uint*)instr->ptr);
#endif
  *(m_uint*)REG(-SZ_INT) = *(m_uint*)instr->ptr;
}

INSTR(Reg_Push_Code) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push code [%i] (%i)", instr->m_val, instr->m_val2);
#endif
  Func f;
  if(!(f =  *(Func*)(instr->m_val2 ? REG(-SZ_INT) : MEM(instr->m_val)))) {
    err_msg(INSTR_, 0, "trying to call empty func pointer.");
    if(instr->m_val2) // if any, release owner on error
      release(*(M_Object*)REG(-SZ_INT * 2), shred);
    Except(shred, "NullFuncPtrException");
  }
  *(VM_Code*)REG(-SZ_INT) = f->code;
}

INSTR(Reg_Dup_Last) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] dup last %p", *(m_uint*)REG(-SZ_INT));
#endif
  *(m_uint*)REG(0) = *(m_uint*)REG(-SZ_INT);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Reg_AddRef_Object3) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] add ref %i %p", instr->m_val, instr->m_val ? **(M_Object**)REG(-SZ_INT) : * (M_Object*)REG(-SZ_INT));
#endif
  M_Object obj = instr->m_val ? **(M_Object**)REG(-SZ_INT) : *(M_Object*)REG(-SZ_INT);
  if(obj)
    obj->ref++;
}

INSTR(Reg_Push_Me) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push me %p", (void*)shred->me);
#endif
  *(M_Object*)REG(0) = shred->me;
  PUSH_REG(shred, SZ_INT);
}

INSTR(Reg_Push_Now) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push now %f", (m_float)vm->sp->pos);
#endif
  *(m_float*)REG(0) = vm->sp->pos;
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(Reg_Push_Maybe) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push maybe");
#endif
  *(m_uint*)REG(0) = (sp_rand(shred->vm_ref->sp) > (RAND_MAX / 2)) ? 1 : 0;
  PUSH_REG(shred, SZ_INT);
}

INSTR(Alloc_Word) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "alloc_word '%s' (%p)[%i]", instr->m_val2 ? "base" : "mem", (void*) * (m_uint*)MEM(instr->m_val), instr->m_val);
#endif
  *(m_uint*)MEM(instr->m_val) = 0; // since template
  if(*(m_uint*)instr->ptr)
    *(m_uint**)REG(0) = &*(m_uint*)MEM(instr->m_val);
  else
    *(m_uint*)REG(0) = *(m_uint*)MEM(instr->m_val);
  PUSH_REG(shred, SZ_INT);
}

INSTR(Alloc_Word_Float) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "instr alloc word float %s [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  *(m_float*)MEM(instr->m_val) = 0; // since template
  if(*(m_uint*)instr->ptr) {
    *(m_float**)REG(0) = &*(m_float*)MEM(instr->m_val);
    PUSH_REG(shred, SZ_INT);
  } else {
    *(m_float*)REG(0) = *(m_float*)MEM(instr->m_val);
    PUSH_REG(shred, SZ_FLOAT);
  }
}

INSTR(Alloc_Word_Complex) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "instr alloc word complex %s [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  if(*(m_uint*)instr->ptr) {
    *(m_complex**)REG(0) = &*(m_complex*)MEM(instr->m_val);
    PUSH_REG(shred, SZ_INT);
  } else {
    *(m_complex*)REG(0) = *(m_complex*)MEM(instr->m_val);
    PUSH_REG(shred, SZ_COMPLEX);
  }
}

INSTR(Alloc_Word_Vec3) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "instr alloc word vec3 %s [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  if(*(m_uint*)instr->ptr) {
    *(m_vec3**)REG(0) = &*(m_vec3*)MEM(instr->m_val);
    PUSH_REG(shred,  SZ_INT);
  } else {
    *(m_vec3*)REG(0) = *(m_vec3*)MEM(instr->m_val);
    PUSH_REG(shred,  SZ_VEC3);
  }
}

INSTR(Alloc_Word_Vec4) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "instr alloc word vec4 %s [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  if(*(m_uint*)instr->ptr) {
    *(m_vec4**)REG(0) = &*(m_vec4*)MEM(instr->m_val);
    PUSH_REG(shred,  SZ_INT);
  } else {
    *(m_vec4*)REG(0) = *(m_vec4*)MEM(instr->m_val);
    PUSH_REG(shred,  SZ_VEC4);
  }
}

/* branching */
INSTR(Branch_Switch) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "branch switch %i %i", *(m_int*)REG(-SZ_INT), instr->m_val);
#endif
  Map map = *(Map*)instr->ptr;
  POP_REG(shred,  SZ_INT);
  shred->next_pc = (m_int)map_get(map, (vtype) * (m_int*)REG(0));
  if(!shred->next_pc)
    shred->next_pc = instr->m_val;
}

INSTR(Branch_Eq_Int) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "branch eq int %i %i %i", instr->m_val,  *(m_int**)REG(-SZ_INT * 2),  *(m_int**)REG(-SZ_INT));
#endif
  POP_REG(shred,  SZ_INT * 2);
  if(*(m_uint*)REG(0) == *(m_uint*)REG(SZ_INT))
    shred->next_pc = instr->m_val;
}

INSTR(Branch_Neq_Int) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "branch !=");
#endif
  POP_REG(shred,  SZ_INT * 2);
  if(*(m_uint*)REG(0) != *(m_uint*)REG(SZ_INT))
    shred->next_pc = instr->m_val;
}

INSTR(Branch_Eq_Float) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "branch eq int %i %f %f", instr->m_val,  *(m_float*)REG(- SZ_FLOAT * 2),  *(m_float*)REG(- SZ_FLOAT));
#endif
  POP_REG(shred,  SZ_FLOAT * 2);
  if(*(m_float*)REG(0) == *(m_float*)REG(SZ_FLOAT))
    shred->next_pc = instr->m_val;
}

INSTR(Branch_Neq_Float) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "branch != float %f %f", *(m_float*)REG(- SZ_FLOAT * 2), *(m_float*)REG(- SZ_FLOAT));
#endif
  POP_REG(shred,  SZ_FLOAT * 2);
  if(*(m_float*)REG(0) != *(m_float*)REG(SZ_FLOAT))
    shred->next_pc = instr->m_val;
}

INSTR(Init_Loop_Counter) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "loop: init counter");
#endif
  POP_REG(shred,  SZ_INT);
  m_int* sp = (m_int*)REG(0);
  (*(m_int*)instr->m_val) = (*sp >= 0 ? *sp : -*sp);
}

INSTR(Reg_Push_Deref) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "reg  push deref %i", instr->m_val);
#endif
  *(m_int*)REG(0) = *(m_int*)instr->m_val;
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Dec_int_Addr) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "dec int addr");
#endif
  (*((m_int*)(instr->m_val)))--;
}

INSTR(Goto) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "goto %i", instr->m_val);
#endif
  shred->next_pc = instr->m_val;
}

INSTR(Cast_i2f) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "cast i2f");
#endif
  POP_REG(shred,  SZ_INT);
  *(m_float*)REG(0) = *(m_int*)REG(0);
  PUSH_REG(shred,  SZ_FLOAT);
}

INSTR(Cast_f2i) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "castf2i");
#endif
  POP_REG(shred,  SZ_FLOAT);
  *(m_int*)REG(0) = *(m_float*)REG(0);
  PUSH_REG(shred,  SZ_INT);
}

static VM_Shred init_spork_shred(VM_Shred shred, VM_Code code) {
  VM_Shred sh = new_vm_shred(code);
  sh->parent = shred;
  if(!shred->child.ptr)
    vector_init(&shred->child);
  vector_add(&shred->child, (vtype)sh);
  sh->_mem = sh->base;
  sh->base = shred->base;
  sh->me = new_shred(vm, sh);
  vm_add_shred(vm, sh);
  return sh;
}

INSTR(Spork) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Spork");
#endif
  m_uint this_ptr = 0;
  VM_Code code;
  Func func;

  POP_REG(shred,  SZ_INT);
  code = *(VM_Code*)REG(0);
  VM_Shred sh = init_spork_shred(shred, code);
  POP_REG(shred,  SZ_INT);
  func = *(Func*)REG(0);
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
  if(*(m_uint*)instr->ptr && code->need_this) // sporked function
    *(m_uint*)sh->mem = this_ptr;
  *(Func*)sh->reg = func;
  PUSH_REG(sh, SZ_INT);
  *(M_Object*)REG(0) = sh->me;
  PUSH_REG(shred,  SZ_INT);
  if(instr->m_val2)
    ADD_REF(code)
}

// LCOV_EXCL_START
void handle_overflow(VM_Shred shred) {
  fprintf(stderr,
          "[Gwion](VM): StackOverflow: shred[id=%lu:%s], PC=[%lu]\n",
          shred->xid, shred->name, shred->pc);
}
// LCOV_EXCL_STOP

static void shred_func_prepare(VM_Shred shred, Instr instr) {
  POP_REG(shred,  SZ_INT * 2);
  VM_Code code = *(VM_Code*)REG(0);
  m_uint local_depth = *(m_uint*)REG(SZ_INT);
  m_uint prev_stack = instr ? instr->m_val : shred->mem == shred->base ? 0 : *(m_uint*)MEM(-SZ_INT);
  m_uint push = prev_stack + local_depth;

  PUSH_MEM(shred, push);
  *(m_uint*)MEM(0)  = push;
  PUSH_MEM(shred,  SZ_INT);
  *(m_uint*)MEM(0)  = (m_uint)shred->code;
  PUSH_MEM(shred,  SZ_INT);
  *(m_uint*)MEM(0)  = shred->pc + 1;
  PUSH_MEM(shred,  SZ_INT);
  *(m_uint*)MEM(0)  = (m_uint)code->stack_depth;
  PUSH_MEM(shred,  SZ_INT);
  shred->next_pc = 0;
  shred->code = code;
}

static void shred_func_need_this(VM_Shred shred) {
  if(shred->code->need_this) {
    *(m_uint*)MEM(0) = *(m_uint*)REG(shred->code->stack_depth - SZ_INT);
    PUSH_MEM(shred,  SZ_INT);
  }
}

static void shred_func_finish(VM_Shred shred) {
  if(shred->code->need_this)
    POP_MEM(shred, SZ_INT);
  if(overflow_(shred))
    handle_overflow(shred);
}

INSTR(Instr_Exp_Func) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "func call");
#endif
  VM_Code code;
  m_uint stack_depth;
  shred_func_prepare(shred, instr);
  code = shred->code;
  stack_depth = code->stack_depth;
  if(stack_depth) {
    POP_REG(shred,  stack_depth);
    shred_func_need_this(shred);
    memcpy(shred->mem, shred->reg, stack_depth
        - (code->need_this ? SZ_INT : 0));
  }
  shred_func_finish(shred);
  return;
}

INSTR(Instr_Op_Call_Binary) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "binary call");
#endif
  Type l = (Type)instr->m_val2;
  Type r = *(Type*)instr->ptr;
  shred_func_prepare(shred, instr);
  POP_REG(shred,  l->size + SZ_INT); // cause rhs has emit_var = 1
  shred_func_need_this(shred);
  if(isa(l, &t_object) > 0)
    release(*(M_Object*)REG(0), shred);
  if(isa(r, &t_object) > 0)
    release(**(M_Object**)REG(l->size), shred);
  memcpy(MEM(0), REG(0), l->size);
  memcpy(MEM(l->size), *(m_uint**)REG(l->size), SZ_INT);
  shred_func_finish(shred);
}

INSTR(Dot_Static_Func) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "dot static func %s", ((Func)instr->m_val)->name);
#endif
  *(m_uint*)REG(-SZ_INT) = instr->m_val;
}

INSTR(Reg_Dup_Last_Vec3) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "dup last vec3");
#endif
  *(m_vec3*)REG(0) = *(m_vec3*)REG(-SZ_INT);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Reg_Dup_Last_Vec4) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "dup last vec4");
#endif
  *(m_vec4*)REG(0) = *(m_vec4*)REG(-SZ_INT);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(member_function) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "member function %p [%i] %p", *(m_uint*)instr->ptr, instr->m_val, vector_at(*(Vector*)instr->ptr,
            instr->m_val));
#endif
  POP_REG(shred,  SZ_INT);
  *(VM_Code*)REG(0) = ((Func)vector_at(*(Vector*)instr->ptr, instr->m_val))->code;
  PUSH_REG(shred,  SZ_INT);
  return;
}

INSTR(Exp_Dot_Func) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "dot member func");
#endif
  POP_REG(shred,  SZ_INT);
  M_Object obj = *(M_Object*)REG(0);
  if(!obj) Except(shred, "NullPtrException");
  *(Func*)REG(0) = (Func)vector_at(obj->vtable, instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Instr_Exp_Func_Static) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "func call static");
#endif

  f_sfun f;
  m_uint local_depth, stack_depth;
  VM_Code func;
  char retval[SZ_VEC4];

  POP_REG(shred,  SZ_INT * 2);
  func        = *(VM_Code*)REG(0);
  f           = (f_sfun)func->native_func;
  local_depth = *(m_uint*)REG(SZ_INT);
  stack_depth = func->stack_depth;
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

static void copy_member_args(VM_Shred shred, VM_Code func) { 
  m_uint stack_depth = func->stack_depth;
  if(stack_depth) {
    POP_REG(shred,  stack_depth);
    if(func->need_this) {
      *(m_uint*)MEM(0) = *(m_uint*)REG(-SZ_INT + stack_depth);
      PUSH_MEM(shred,  SZ_INT);
      stack_depth -= SZ_INT;
    }
    memcpy(shred->mem, shred->reg, stack_depth);
  }
  if(func->need_this)
    POP_MEM(shred,  SZ_INT);
}

INSTR(Instr_Exp_Func_Member) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "func call member");
#endif
  VM_Code func;
  m_uint local_depth;

  POP_REG(shred,  SZ_INT * 2);
  func = *(VM_Code*)REG(0);
  local_depth =   *(m_uint*)REG(SZ_INT);
  PUSH_MEM(shred,  local_depth);
  copy_member_args(shred, func);
  if(overflow_(shred)) {
    handle_overflow(shred);
    return;
  }
  if(func->native_func_type == NATIVE_CTOR) {
    f_xtor f = (f_xtor)func->native_func;
    f(*(M_Object*)MEM(0), shred);
  } else {
    char retval[SZ_VEC4];
    f_mfun f = (f_mfun)func->native_func;
    f((*(M_Object*)MEM(0)), retval, shred);
    dl_return_push(retval, shred, instr->m_val);
  }
  POP_MEM(shred,  local_depth);
}

INSTR(Func_Return) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "func return %p", MEM(0));
#endif
  VM_Code func;

  POP_MEM(shred,  SZ_INT * 2);
  shred->next_pc = *(m_uint*)MEM(0);
  POP_MEM(shred,  SZ_INT);
  func = *(VM_Code*)MEM(0);
  POP_MEM(shred,  SZ_INT);
  POP_MEM(shred, *(m_uint*)MEM(0));
  shred->code = func;
}

static void call_pre_constructor(VM * vm, VM_Shred shred, VM_Code pre_ctor, m_uint stack_offset) {
  *(m_uint*)REG(0) = *(m_uint*)REG(-SZ_INT); // ref dup last
  PUSH_REG(shred,  SZ_INT);
  *(VM_Code*)REG(0) = pre_ctor;
  PUSH_REG(shred,  SZ_INT);
  *(m_uint*)REG(0) = stack_offset;
  PUSH_REG(shred,  SZ_INT);
  if(pre_ctor->native_func_type != NATIVE_UNKNOWN)
    Instr_Exp_Func_Member(vm, shred, NULL);
  else
    Instr_Exp_Func(vm, shred, NULL);
}

INSTR(Reg_Push_This) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "push this %p", *(m_uint*)MEM(0));
#endif
  *(m_uint*)REG(0) = *(m_uint*)MEM(0);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Pre_Constructor) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "pre constructor");
#endif
  call_pre_constructor(vm, shred, (VM_Code)instr->m_val, instr->m_val2);
}

static void instantiate_object(VM * vm, VM_Shred shred, Type type) {
  M_Object object = new_M_Object(NULL);
  if(!object) Except(shred, "NullPtrException");
  initialize_object(object, type);
  *(M_Object*)REG(0) =  object;
  PUSH_REG(shred,  SZ_INT);
#ifdef DEBUG_INSTR
  debug_msg("instr", "instantiate object (internal)%p %s", object, type->name);
#endif
  return;
}

INSTR(Instantiate_Object) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "instantiate object %p", *(m_uint*)instr->ptr);
#endif
  instantiate_object(vm, shred, *(Type*)instr->ptr);
  vector_add(&shred->gc1, *(vtype*)REG(-SZ_INT));
}

INSTR(Alloc_Member_Word) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "alloc member word: %p[%i]", *(m_uint*)MEM(0), instr->m_val);
#endif
  M_Object obj = *(M_Object*)MEM(0);
  *(m_uint*)(obj->data + instr->m_val) = 0;
  if(*(m_uint*)instr->ptr)
    *(m_uint**)REG(0) = &*(m_uint*)(obj->data + instr->m_val);
  else
    *(m_uint*)REG(0) = *(m_uint*)(obj->data + instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Alloc_Member_Word_Float) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "alloc member float: %p[%i]", *(m_uint*)MEM(0), instr->m_val);
#endif
  M_Object obj = *(M_Object*)MEM(0);
  *(m_float*)(obj->data + instr->m_val) = 0.0;
  if(*(m_uint*)instr->ptr) {
    *(m_float**)REG(0) = &*(m_float*)(obj->data + instr->m_val);
    PUSH_REG(shred,  SZ_INT);
  } else {
    *(m_float*)REG(0) = *(m_float*)(obj->data + instr->m_val);
    PUSH_REG(shred,  SZ_FLOAT);
  }
}

INSTR(Alloc_Member_Word_Complex) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "alloc member complex: %p[%i] = '%p'", *(m_uint*)MEM(-SZ_INT), instr->m_val, *(m_uint*)MEM(0));
#endif
  M_Object obj = *(M_Object*)MEM(0);
  *(m_complex*)(obj->data + instr->m_val) = 0.0;
  if(*(m_uint*)instr->ptr) {
    *(m_complex**)REG(0) = &*(m_complex*)(obj->data + instr->m_val);
    PUSH_REG(shred, SZ_INT);
  } else {
    *(m_complex*)REG(0) = *(m_complex*)(obj->data + instr->m_val);
    PUSH_REG(shred, SZ_COMPLEX);
  }
}

INSTR(Alloc_Member_Word_Vec3) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "alloc member vec3: %p[%i] = '%p'", *(m_uint*)MEM(-SZ_INT), instr->m_val, *(m_uint*)MEM(0));
#endif
  M_Object obj = *(M_Object*)MEM(0);
  memset((obj->data + instr->m_val), 0, SZ_VEC3);
  if(*(m_uint*)instr->ptr) {
    *(m_vec3**)REG(0) = &*(m_vec3*)(obj->data + instr->m_val);
    PUSH_REG(shred,  SZ_INT);
  } else {
    *(m_vec3*)REG(0) = *(m_vec3*)(obj->data + instr->m_val);
    PUSH_REG(shred,  SZ_VEC3);
  }
}

INSTR(Alloc_Member_Word_Vec4) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "alloc member vec4: %p[%i] = '%p'", *(m_uint*)MEM(-SZ_INT), instr->m_val, *(m_uint*)MEM(0));
#endif
  M_Object obj = *(M_Object*)MEM(0);
  memset((obj->data + instr->m_val), 0, SZ_VEC4);
  if(*(m_uint*)instr->ptr) {
    *(m_vec4**)REG(0) = &*(m_vec4*)(obj->data + instr->m_val);
    PUSH_REG(shred, SZ_INT);
  } else {
    *(m_vec4*)REG(0) = *(m_vec4*)(obj->data + instr->m_val);
    PUSH_REG(shred, SZ_VEC4);
  }
}

INSTR(Dot_Static_Data) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Dot STATIC DATA: [%i] (%i) (emit:%i)", instr->m_val, instr->m_val2, *(m_uint*)instr->ptr);
#endif
  Type t = *(Type*)REG(-SZ_INT);

  POP_REG(shred,  SZ_INT);
  // take care of emit_addr ? (instr->ptr)
  if(*(m_uint*)instr->ptr)
    *(char**)REG(0) = &*(char*)(t->info->class_data + instr->m_val);
  else 
    memcpy(REG(0), t->info->class_data + instr->m_val, instr->m_val2);
  PUSH_REG(shred,  instr->m_val2);

}

INSTR(Dot_Static_Import_Data) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Dot STATIC Import DATA: %p", *(m_uint*)instr->ptr);
#endif
  if(*(m_uint*)instr->ptr)
    *(char**)REG(0) = &*(char*)(instr->m_val);
  else
    memcpy(REG(0), (char*)instr->m_val, instr->m_val2);
  PUSH_REG(shred, instr->m_val2);
}

INSTR(Exp_Dot_Data) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "dot member data '%p'[%i] (%i) (emit:%i)", *(M_Object*)REG(-SZ_INT), instr->m_val, instr->m_val2, *(m_uint*)instr->ptr);
#endif
  M_Object obj;
  m_bool ptr = *(m_uint*)instr->ptr;
  char* c;
  POP_REG(shred,  SZ_INT);
  obj  = *(M_Object*)REG(0);
  if(!obj) Except(shred, "NullPtrException");
  c = (char*)(obj->data + instr->m_val);
  memcpy(REG(0), ptr ? (char*)&c : c, ptr ? SZ_INT : instr->m_val2);
  PUSH_REG(shred, ptr ? SZ_INT : instr->m_val2);
}

INSTR(Release_Object2) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "release object %p %i", *(m_uint*)MEM(instr->m_val), instr->m_val);
#endif
  release(*(M_Object*)MEM(instr->m_val), shred);
}

/* array */
INSTR(Instr_Pre_Ctor_Array_Top) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "array alloc top");
#endif
  if(*(m_uint*)REG(-SZ_INT * 2) >= *(m_uint*)REG(-SZ_INT))
    shred->next_pc = instr->m_val;
  else
    instantiate_object(vm, shred, *(Type*)instr->ptr);
}

INSTR(Instr_Pre_Ctor_Array_Bottom) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "array alloc bottom");
#endif
  POP_REG(shred,  SZ_INT);
  M_Object obj = *(M_Object*)REG(0);
  m_uint * array = *(m_uint**)REG(-SZ_INT * 3);
  m_int i = *(m_int*)REG(-SZ_INT * 2);
  *(m_uint*)array[i] = (m_uint)obj;
  (*(m_int*)REG(-SZ_INT * 2))++;
  shred->next_pc = instr->m_val;
}

INSTR(Instr_Pre_Ctor_Array_Post) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "array alloc post");
#endif
  POP_REG(shred,  SZ_INT * 3);
  m_uint* array = *(m_uint**)REG(0);
  free(array);
}

struct ArrayAllocInfo {
  m_int capacity;
  const m_int top;
  Type type;
  m_bool is_obj;
  m_uint* objs;
  m_int* index;
};

static M_Object do_alloc_array_object(struct ArrayAllocInfo* info, m_int cap) {
  M_Object base;
  if(cap < 0) {
    fprintf(stderr, "[gwion](VM): NegativeArraySize: while allocating arrays...\n");
    REM_REF(info->type);
    return NULL;   
  }
  base = new_M_Array(info->capacity >= info->top ? 
      info->type->d.array_type->size : SZ_INT, cap, -info->capacity);
  if(!base) {
    fprintf(stderr, "[gwion](VM): OutOfMemory: while allocating arrays...\n");
    return NULL;
  }
  base->type_ref = info->type; // /13/03/17
  ADD_REF(info->type);
  return base;
}

static M_Object do_alloc_array_init(struct ArrayAllocInfo* info, m_int cap, 
    M_Object base) {
  if(info->is_obj && info->objs) {
    m_int i;
    for(i = 0; i < cap; i++) {
      info->objs[*info->index] = (m_uint)m_vector_addr(ARRAY(base), i);
      (*info->index)++;
    }
  }
  return base;
}

static M_Object do_alloc_array(VM_Shred shred, struct ArrayAllocInfo* info);
static M_Object do_alloc_array_loop(VM_Shred shred, struct ArrayAllocInfo* info,
    m_int cap, M_Object base) {
  m_int i;
  for(i = 0; i < cap; i++) {
    struct ArrayAllocInfo aai = { info->capacity + 1, info->top, info->type,
      info->is_obj, info->objs, info->index };
    M_Object next = do_alloc_array(shred, &aai);
    if(!next) {
      release(base, shred);
      return NULL;
    }
    i_vector_set(ARRAY(base), i, (m_uint)next);
  }
  return base;
}

static M_Object do_alloc_array(VM_Shred shred, struct ArrayAllocInfo* info) {
  M_Object base;
  m_int cap = *(m_int*)REG(info->capacity * SZ_INT);

  if(!(base = do_alloc_array_object(info, cap)))
    return NULL;
  if(info->capacity >= info->top)
    return do_alloc_array_init(info, cap, base);
  else
    return do_alloc_array_loop(shred, info, cap, base);
  return base;
}

INSTR(Instr_Array_Init) { // for litteral array
#ifdef DEBUG_INSTR
  debug_msg("instr", "array init");
#endif
  m_uint i;
  VM_Array_Info* info = *(VM_Array_Info**)instr->ptr;
  M_Object obj;
  POP_REG(shred, instr->m_val2 * info->length);
  obj = new_M_Array(info->type->d.array_type->size, info->length, info->depth);
  obj->type_ref = info->type;
  vector_add(&shred->gc, (vtype) obj);
  for(i = 0; i < info->length; i++)
    m_vector_set(ARRAY(obj), i, REG(instr->m_val2 * i));
  *(M_Object*)REG(0) = obj;
  PUSH_REG(shred,  SZ_INT);
}

static m_uint* init_array(VM_Shred shred, VM_Array_Info* info, m_uint* num_obj) {
  m_int curr = -info->depth;
  m_int top = - 1;
  m_int tmp;
  *num_obj = 1;
  while(curr <= top) {
    tmp = *(m_int*)REG(SZ_INT * curr);
    *num_obj *= tmp;
    curr++;
  }
  if(*num_obj > 0)
    return (m_uint*)calloc(*num_obj, sizeof(m_uint));
  return (m_uint*)1;
}

INSTR(Instr_Array_Alloc) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "array alloc");
#endif
  VM_Array_Info* info = *(VM_Array_Info**)instr->ptr;
  M_Object ref;
  m_uint num_obj = 0;
  m_int index = 0;
  struct ArrayAllocInfo aai = { -info->depth, -1, info->type,  info->is_obj, NULL, &index};
  if(info->is_obj && !info->is_ref &&
      !(aai.objs = init_array(shred, info, &num_obj)))
      goto out_of_memory;
  if(!(ref = do_alloc_array(shred, &aai)))
    goto error;
  POP_REG(shred, SZ_INT * info->depth);
  *(M_Object*)REG(0) = ref;
  PUSH_REG(shred,  SZ_INT);
  if(info->is_obj && !info->is_ref) {
    *(m_uint**)REG(0) = aai.objs;
    PUSH_REG(shred,  SZ_INT);
    *(m_uint*) REG(0) = 0;
    PUSH_REG(shred,  SZ_INT);
    *(m_uint*) REG(0) = num_obj;
    PUSH_REG(shred,  SZ_INT);
  }
  REM_REF(info->type);
  return;

out_of_memory:
  fprintf(stderr, "[Gwion](VM): OutOfMemory: while allocating arrays...\n"); // LCOV_EXCL_LINE
  goto error;                                                                  // LCOV_EXCL_LINE
error:
  fprintf(stderr, "[Gwion](VM): (note: in shred[id=%lu:%s])\n", shred->xid, shred->name);
  release(shred->me, shred);
  shred->me = NULL;
}

static void array_push(VM_Shred shred, M_Vector a, m_uint i, Kindof kind, m_bool emit_var) {
  if(emit_var)
      *(char**)REG(0) = m_vector_addr(a, i);
  else
    m_vector_get(a, i, REG(0));
  PUSH_REG(shred,  kind);
}

static void oob(M_Object obj, VM_Shred shred, m_int i) {
  fprintf(stderr,
          "[Gwion](VM): ArrayOutofBounds: in shred[id=%lu:%s], PC=[%lu], index=[%ld]\n",
          shred->xid, shred->name, shred->pc, i);
  release(obj, shred);
  release(shred->me, shred);
  shred->me = NULL;
}

#define OOB(shred, obj, i)  if(i < 0 || i >=  m_vector_size(ARRAY(obj))) { \
  oob(obj, shred, i); return; }

INSTR(Instr_Array_Access) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "array access '%p'  (emit: %i) [%i] ", *(m_uint*)REG(-SZ_INT * 2), instr->m_val, instr->m_val2);
#endif
  m_int i = 0;
  M_Object obj;
  POP_REG(shred,  SZ_INT * 2);
  if(!(obj = *(M_Object*)REG(0)))
    Except(shred, "NullPtrException");
  i = *(m_int*)REG(SZ_INT);
  OOB(shred, obj, i)
  array_push(shred, ARRAY(obj), i, instr->m_val2, instr->m_val);
}

INSTR(Instr_Array_Access_Multi) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "array access multi");
#endif
  m_int i, j;
  POP_REG(shred,  SZ_INT * (instr->m_val + 1));
  M_Object obj, *base = (M_Object*)REG(0);
  if(!(obj = *base))
    Except(shred, "NullPtrException");
  for(j = 0; j < instr->m_val - 1; j++) {
    i = *(m_int*)REG(SZ_INT * (j + 1));
    OOB(shred, obj, *(m_int*)REG(SZ_INT * (j + 1)))
    if(!(obj = (M_Object)i_vector_at(ARRAY(obj), i)))
      Except(shred, "NullPtrException");
  }
  i = *(m_int*)REG(SZ_INT * (j + 1));
  OOB(shred, obj ,*(m_int*)REG(SZ_INT * (j + 1)))
  array_push(shred, ARRAY(obj), i, instr->m_val2, *(m_uint*)instr->ptr);
}

INSTR(start_gc) {
  if(!shred->gc.ptr) //  dynamic assign
    vector_init(&shred->gc);
  vector_add(&shred->gc, (vtype)NULL); // enable scoping
}

INSTR(stop_gc) {
  M_Object o;
  while((o = (M_Object)vector_pop(&shred->gc)))
    release(o, shred);
}
