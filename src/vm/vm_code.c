#include <stdlib.h>
#include <string.h>
#include "absyn.h"
#include "type.h"
#include "func.h"
#include "instr.h"

VM_Code new_vm_code(Vector instr, m_uint stack_depth, m_bool need_this,
    m_str name, m_str filename) {
  VM_Code code           = malloc(sizeof(struct VM_Code_));
  code->instr            = instr ?  vector_copy(instr) : NULL;
  code->stack_depth      = stack_depth;
  code->need_this        = need_this;
  code->name             = strdup(name);
  code->filename         = strdup(filename);
  code->native_func      = 0;
  code->native_func_type = NATIVE_UNKNOWN;
  INIT_OO(code, e_code_obj)
  return code;
}

static void free_code_instr_gack(Instr instr) {
  m_uint j;
  Vector v = *(Vector*)instr->ptr;
  for(j = vector_size(v) + 1; --j;)
    REM_REF(((Type)vector_at(v, j - 1)));
  free_vector(v);
}

static void free_code_instr(Vector v) {
  m_uint i;
  for(i = vector_size(v) + 1; --i;) {
    Instr instr = (Instr)vector_at(v, i - 1);
    if(instr->execute == Instr_Array_Init ||
        instr->execute == Instr_Array_Alloc) {
      VM_Array_Info* info = *(VM_Array_Info**)instr->ptr;
      if(!instr->m_val)
        REM_REF(info->type)
      free(info);
    }
    else if(instr->execute == Gack)
      free_code_instr_gack(instr);
    else if(instr->execute == Branch_Switch)
      free_map(*(Map*)instr->ptr);
    else if(instr->execute == Spork && instr->m_val2)
        REM_REF(((Func)instr->m_val2))
    else if(instr->execute == Init_Loop_Counter)
      free((m_int*)instr->m_val);
    free(instr);
  }
  free_vector(v);
}

void free_vm_code(VM_Code a) {
  if(!strcmp(a->name, "[dtor]")) { // dtor from release. free only EOC
    free((void*)vector_back(a->instr));
    free_vector(a->instr);
  } else if(a->instr)
    free_code_instr(a->instr);
  free(a->name);
  free(a->filename);
  free(a);
}
