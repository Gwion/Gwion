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

VM_Code new_vm_code(const Vector instr, const m_uint stack_depth,
    const m_bool need_this, const m_str name) {
  VM_Code code           = mp_alloc(VM_Code);
  code->instr            = instr ?  vector_copy(instr) : NULL;
  code->name             = strdup(name);
  code->stack_depth      = stack_depth;
  code->native_func      = 0;
  SET_FLAG(code, NATIVE_NOT | (uint)(need_this ? _NEED_THIS_ : 0));
  INIT_OO(code, e_code_obj)
  return code;
}

ANN static void free_code_instr_gack(const Instr instr) {
  const Vector v = *(Vector*)instr->ptr;
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
    if(instr->execute == SporkExp)
      REM_REF((Func)instr->m_val2)
    else if(instr->execute == SporkFunc)
      REM_REF((VM_Code)instr->m_val2)
    else if(instr->execute == ArrayAlloc)
      free_array_info(*(ArrayInfo**)instr->ptr);
    else if(instr->execute == Gack)
      free_code_instr_gack(instr);
    else if(instr->execute == BranchSwitch)
      free_map((Map)instr->m_val2);
    else if(instr->execute == InitLoopCounter)
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
  if(a->memoize)
    memoize_end(a->memoize);
  if(a->instr)
    free_code_instr(a->instr);
  free(a->name);
  mp_free(VM_Code, a);
}
