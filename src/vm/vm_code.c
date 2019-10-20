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
#include "gwion.h"
#include "value.h"
#include "operator.h"
#include "import.h"

ANN void free_code_instr(const Vector v, const Gwion gwion) {
  for(m_uint i = vector_size(v) + 1; --i;) {
    const Instr instr = (Instr)vector_at(v, i - 1);
    const f_freearg f = (f_freearg)(map_get(&gwion->data->freearg, instr->opcode) ?:
       map_get(&gwion->data->freearg, (vtype)instr->execute));
    if(f)
      f(instr, gwion);
    mp_free(gwion->mp, Instr, instr);
  }
}

ANN static void _free_code_instr(const Vector v, const Gwion gwion) {
  free_code_instr(v, gwion);
  free_vector(gwion->mp, v);
}

ANN static void free_vm_code(VM_Code a, Gwion gwion) {
  if(a->memoize)
    memoize_end(gwion->mp, a->memoize);
  if(!GET_FLAG(a, builtin)) {
    _mp_free(gwion->mp, vector_size(a->instr) * SZ_INT, a->bytecode);
    _free_code_instr(a->instr, gwion);
  }
  free_mstr(gwion->mp, a->name);
  mp_free(gwion->mp , VM_Code, a);
}

ANN static m_bit* tobytecode(MemPool p, const VM_Code code) {
  const Vector v = code->instr;
  const m_uint sz = vector_size(v);
  m_bit *ptr = _mp_malloc(p, sz * BYTECODE_SZ);
  for(m_uint i= 0; i < sz; ++i) {
    const Instr instr = (Instr)vector_at(v, i);
    if(instr->opcode == ePushStaticCode && !instr->m_val) {
      instr->opcode = eRegPushImm;
      instr->m_val = (m_uint)code;
    }
    if(instr->opcode < eGack)
      memcpy(ptr + i*BYTECODE_SZ, instr, BYTECODE_SZ);
    else {
      *(m_bit*)(ptr + (i*BYTECODE_SZ)) = instr->opcode;
      *(Instr*)(ptr + (i*BYTECODE_SZ) + SZ_INT) = instr;
      *(f_instr*)(ptr + (i*BYTECODE_SZ) + SZ_INT*2) = instr->execute;
    }
    *(unsigned short*)(ptr + (i*BYTECODE_SZ) + 2) = i;
  }
  return ptr;
}


VM_Code new_vm_code(MemPool p, const Vector instr, const m_uint stack_depth,
    const ae_flag flag, const m_str name) {
  VM_Code code           = mp_calloc(p, VM_Code);
  if(instr) {
    code->instr            = vector_copy(p, instr);
    code->bytecode = tobytecode(p, code);
  }
  code->name             = mstrdup(p, name);
  code->stack_depth      = stack_depth;
  code->flag = flag;
  code->ref = new_refcount(p, free_vm_code);
  return code;
}

