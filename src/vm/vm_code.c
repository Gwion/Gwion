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
#include "import.h"

ANN /*static*/ void free_code_instr(const Vector v, const Gwion gwion) {
  for(m_uint i = vector_size(v) + 1; --i;) {
    const Instr instr = (Instr)vector_at(v, i - 1);
    const f_freearg f = (f_freearg)(map_get(&gwion->data->freearg, instr->opcode) ?:
       map_get(&gwion->data->freearg, (vtype)instr->execute));
    if(f)
      f(instr, gwion);
    mp_free(gwion->mp, Instr, instr);
  }
//  free_vector(gwion->mp, v);
}

ANN static void _free_code_instr(const Vector v, const Gwion gwion) {
  free_code_instr(v, gwion);
  free_vector(gwion->mp, v);
}
ANN static void free_vm_code(VM_Code a, Gwion gwion) {
  if(a->memoize)
    memoize_end(gwion->mp, a->memoize);
  if(!GET_FLAG(a, builtin))
    _free_code_instr(a->instr, gwion);
  free_mstr(gwion->mp, a->name);
  mp_free(gwion->mp , VM_Code, a);
}

VM_Code new_vm_code(MemPool p, const Vector instr, const m_uint stack_depth,
    const ae_flag flag, const m_str name) {
  VM_Code code           = mp_calloc(p, VM_Code);
  code->instr            = instr ?  vector_copy(p, instr) : NULL;
  code->name             = mstrdup(p, name);
  code->stack_depth      = stack_depth;
  code->flag = flag;
  code->ref = new_refcount(p, free_vm_code);
  return code;
}

