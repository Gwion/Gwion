#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "emit.h"

// should be in vm.h
#define VAL (*(m_uint*)(byte + SZ_INT))
#define FVAL (*(m_float*)(byte + SZ_INT))
#define VAL2 (*(m_uint*)(byte + SZ_INT*2))
/*
ANN static void set_recurs(m_bit *byte, m_bit **reg) {
  *(m_bit*)byte = eRegPushImm;
  VAL = (*(m_uint*)(*reg-SZ_INT) = );
  VAL2 = -SZ_INT;
// copy from vm_code
}
*/

ANN static void set_func(m_bit *byte, m_bit **reg) {
  *(m_bit*)byte = eRegSetImm;
  VAL = (*(m_uint*)(*reg-SZ_INT) = (m_uint)((Func)VAL)->code);
  VAL2 = -SZ_INT;
// copy from vm_code
}

ANN static void set_ctor(m_bit *byte, m_bit *reg) {
  *(m_bit*)byte = eRegSetImm;
  VAL = (*(m_uint*)(reg+SZ_INT) = (m_uint)((Type)VAL)->nspc->pre_ctor);
  VAL2 = SZ_INT;
}

ANN void emit_mod_recurs(const Emitter emit) {
  const Instr instr = vector_size(&emit->code->instr) ?
    (Instr)vector_back(&emit->code->instr) : emit_add_instr(emit, RegPushImm);
  instr->opcode = ePushStaticCode;
  instr->m_val = 0;
}

ANN void emit_mod_func(const Emitter emit, const Func f) {
  const Instr instr = emit_add_instr(emit, PushStaticCode);
  instr->m_val = (m_uint)f;
  instr->m_val2 = (m_uint)set_func;
}

ANN void emit_mod_ctor(const Emitter emit, const Type t) {
  const Instr instr = emit_add_instr(emit, PushStaticCode);
  instr->m_val = (m_uint)t;
  instr->m_val2 = (m_uint)set_ctor;
}
