#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "func.h"
#include "instr.h"
#include "mpool.h"

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
    const Instr instr = (Instr)vector_at(v, i - 1);
    if(instr->execute == Instr_Array_Init ||
        instr->execute == Instr_Array_Alloc) {
      VM_Array_Info* info = *(VM_Array_Info**)instr->ptr;
      if(!instr->m_val)
        REM_REF(info->type)
      free_array_info(info);
    }
    else if(instr->execute == Gack)
      free_code_instr_gack(instr);
    else if(instr->execute == Branch_Switch)
      free_map(*(Map*)instr->ptr);
    else if(instr->execute == Spork && instr->m_val2)
        REM_REF(((Func)instr->m_val2))
    else if(instr->execute == Init_Loop_Counter)
      free((m_int*)instr->m_val);
    free_instr(instr);
  }
  free_vector(v);
}

void free_vm_code(VM_Code a) {
  if(!strcmp(a->name, "[dtor]")) { // dtor from release. free only EOC
    free_instr((void*)vector_back(a->instr));
    free_vector(a->instr);
  } else if(a->instr)
    free_code_instr(a->instr);
  free(a->name);
  mp_free(VM_Code, a);
}
