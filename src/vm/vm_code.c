#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "func.h"
#include "instr.h"
#include "object.h"
#include "array.h"
#include "memoize.h"

ANN static void free_code_instr_gack(const Instr instr) {
  const Vector v = (Vector)instr->m_val2;
  for(m_uint i = vector_size(v) + 1; --i;)
    REM_REF(((Type)vector_at(v, i - 1)));
  free_vector(v);
}

ANN static void free_array_info(ArrayInfo* info) {
  REM_REF((Type)vector_back(&info->type));
  vector_release(&info->type);
  mp_free(ArrayInfo, info);
}

ANN static void free_code_instr(const Vector v) {
  for(m_uint i = vector_size(v) + 1; --i;) {
    const Instr instr = (Instr)vector_at(v, i - 1);
    if(instr->opcode == eSporkIni || instr->opcode == eForkIni)
      REM_REF((VM_Code)instr->m_val)
    else if(instr->execute == ArrayAlloc)
      free_array_info((ArrayInfo*)instr->m_val);
    else if(instr->opcode == (m_uint)Gack)
      free_code_instr_gack(instr);
    else if(instr->execute == BranchSwitch)
      free_map((Map)instr->m_val2);
    else if(instr->execute == DotTmpl) {
      struct dottmpl_ *dt = (struct dottmpl_*)instr->m_val;
      free_type_list(dt->tl);
      mp_free(dottmpl, dt);
    }
    else if(instr->execute == SwitchIni) {
      free_vector((Vector)instr->m_val);
      free_map((Map)instr->m_val2);
    } else if(instr->opcode == eInitLoopCounter)
      free((m_int*)instr->m_val);
    else if(instr->execute == VarargIni) {
      if(instr->m_val2)
      free_vector((Vector)instr->m_val2);
    }
    mp_free(Instr, instr);
  }
  free_vector(v);
}

ANN static void free_vm_code(VM_Code a) {
#ifndef NOMEMOIZE
  if(a->memoize)
    memoize_end(a->memoize);
#endif
  if(!GET_FLAG(a, builtin))
    free_code_instr(a->instr);
  free(a->name);
  mp_free(VM_Code, a);
}

VM_Code new_vm_code(const Vector instr, const m_uint stack_depth,
    const ae_flag flag, const m_str name) {
  VM_Code code           = mp_alloc(VM_Code);
  code->instr            = instr ?  vector_copy(instr) : NULL;
  code->name             = strdup(name);
  code->stack_depth      = stack_depth;
  code->flag = flag;
  INIT_OO(code, free_vm_code)
  return code;
}

