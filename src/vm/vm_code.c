#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "array.h"
#include "memoize.h"
#include "gwion.h"
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

ANN void free_vmcode(VM_Code a, Gwion gwion) {
  if(a->memoize)
    memoize_end(gwion->mp, a->memoize);
  if(!a->builtin) {
    _mp_free(gwion->mp, vector_size(a->instr) * BYTECODE_SZ, a->bytecode);
    _free_code_instr(a->instr, gwion);
  }
  if(a->closure)
    free_closure(a->closure, gwion);
  free_mstr(gwion->mp, a->name);
  mp_free(gwion->mp , VM_Code, a);
}

static inline uint isgoto(const unsigned opcode) {
  return opcode == eGoto ||
      opcode == eBranchEqInt || opcode == eBranchNeqInt ||
      opcode == eBranchEqFloat || opcode == eBranchNeqFloat;
}

static inline void setpc(const m_bit *data, const m_uint i) {
  *(unsigned*)(data+1) = i + 1;
}

ANN static m_bit* tobytecode(MemPool p, const VM_Code code) {
  const Vector v = code->instr;
  const m_uint sz = vector_size(v);
  m_bit *ptr = _mp_malloc(p, sz * BYTECODE_SZ);
  struct Vector_ nop;
  vector_init(&nop);
  for(m_uint i= 0; i < sz; ++i) {
    m_bit *const data = ptr + i*BYTECODE_SZ;
    const Instr instr = (Instr)vector_at(v, i);
    if(instr->opcode < eOP_MAX) {
      if(instr->opcode == eRegMove) {
        m_int move = (m_int)instr->m_val;
        m_uint j = 0;
        Instr next;
        while((next = (Instr)vector_at(v, i + j +1)) && next->opcode == eRegMove) {
          ++j; vector_add(&nop, i+j);
          move += (m_int)next->m_val;
          next->opcode = eNoOp;
        }
        if((instr->m_val = move)) {
          memcpy(data, instr, BYTECODE_SZ);
          setpc(data, i);
        } else {
          vector_add(&nop, i);
          instr->opcode = eNoOp;
        }
        i += j;
        continue;
      }
      if(instr->opcode != eNoOp)
        memcpy(data, instr, BYTECODE_SZ);
      else
        vector_add(&nop, i);
    } else {
      *(m_bit*)(data) = instr->opcode;
      *(Instr*)(data + SZ_INT) = instr;
      *(f_instr*)(data + SZ_INT*2) = instr->execute;
    }
    setpc(data, i);
  }
  if(!vector_size(&nop)) {
    vector_release(&nop);
    return ptr;
  }
  m_bit *const final = _mp_malloc(p, sz * BYTECODE_SZ); // could use smaller size
  for(m_uint i= 0, j = 0; i < sz; ++i) {
    const Instr instr = (Instr)vector_at(v, i);
    unsigned opcode = instr->opcode;
    if(opcode != eNoOp) {
      m_bit *const base = ptr   + i*BYTECODE_SZ,
            *const data = final + j*BYTECODE_SZ;
//        if(!isgoto(opcode))
      memcpy(data, base, BYTECODE_SZ);
      if(isgoto(opcode)) {
        m_bit pc = 0;
        for(m_uint k = 0; k < vector_size(&nop); ++k) {
          if(instr->m_val <= vector_at(&nop, k))
            break;
          ++pc;
        }
        *(m_uint*)(data + SZ_INT) = instr->m_val > pc ? instr->m_val - pc : 0;
      }
      setpc(data, j);
      ++j;
    }
  }
  vector_release(&nop);
  mp_free2(p, sz*BYTECODE_SZ, ptr);
  return final;
}

VM_Code new_vmcode(MemPool p, const Vector instr, const m_uint stack_depth,
      const int builtin, const m_str name) {
  VM_Code code           = mp_calloc(p, VM_Code);
  code->name             = mstrdup(p, name);
  if(instr) {
    code->instr            = vector_copy(p, instr);
    code->bytecode = tobytecode(p, code);
  }
  code->stack_depth      = stack_depth;
  code->builtin = builtin;
  code->ref = 1;
  return code;
}

