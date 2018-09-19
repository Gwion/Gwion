#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "func.h"
#include "instr.h"
#include "mpool.h"

#ifdef JIT
#include "jitter.h"
#endif

POOL_HANDLE(VM_Code, 2048)
VM_Code new_vm_code(const Vector instr, const m_uint stack_depth,
    const m_bool need_this, const m_str name) {
  VM_Code code           = mp_alloc(VM_Code);
  code->instr            = instr ?  vector_copy(instr) : NULL;
  code->stack_depth      = stack_depth;
  code->flag = need_this ? _NEED_THIS_ : 0;
  code->name             = strdup(name);
  code->native_func      = 0;
  SET_FLAG(code, NATIVE_NOT);
  INIT_OO(code, e_code_obj)
  return code;
}

ANN static void free_code_instr_gack(const Instr instr) {
  const Vector v = *(Vector*)instr->ptr;
  for(m_uint i = vector_size(v) + 1; --i;)
    REM_REF(((Type)vector_at(v, i - 1)));
  free_vector(v);
}

ANN static void free_code_instr(Vector v) {
  for(m_uint i = vector_size(v) + 1; --i;) {
#ifdef JIT
    Instr instr = (Instr)vector_at(v, i - 1);
    if(instr->execute == JitExec) {
      struct JitThread_* p = (struct JitThread_*)instr->m_val2;
      const Instr base  = *(Instr*)instr->ptr;
//      memcpy(instr, base, sizeof(struct Instr_));
      free_jit_instr(p, instr);
      instr = base;
   }
#else
    const Instr instr = (Instr)vector_at(v, i - 1);
#endif
    if(instr->execute == ArrayInit || instr->execute == ArrayAlloc) {
      ArrayInfo* info = *(ArrayInfo**)instr->ptr;
      REM_REF(info->type)
      free_array_info(info);
    } else if(instr->execute == Gack)
      free_code_instr_gack(instr);
    else if(instr->execute == BranchSwitch)
      free_map((Map)instr->m_val2);
    else if(instr->execute == SporkExp) {
//      if(instr->m_val2)
        REM_REF((Func)instr->m_val2)
    } else if(instr->execute == SporkFunc) {
        REM_REF((VM_Code)instr->m_val2)
    } else if(instr->execute == InitLoopCounter)
      free((m_int*)instr->m_val);
    else if(instr->execute == VarargIni) {
      if(instr->m_val2)
      free_vector((Vector)instr->m_val2);
    }
    free_instr(instr);
  }
  free_vector(v);
}

void free_vm_code(VM_Code a) {
  if(a->instr)
    free_code_instr(a->instr);
  free(a->name);
  mp_free(VM_Code, a);
}
