#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "gwion.h"
#include "type.h"
#include "nspc.h"
#include "value.h"
#include "instr.h"
#include "emit.h"
#include "escape.h"

ANEW Emitter new_emitter(MemPool p) {
  Emitter emit = (Emitter)mp_calloc(p, Emitter);
  vector_init(&emit->stack);
  vector_init(&emit->pure);
  vector_init(&emit->variadic);
  emit->escape = escape_table(p);
  return emit;
}

ANN void free_emitter(MemPool p, Emitter a) {
  vector_release(&a->stack);
  vector_release(&a->pure);
  vector_release(&a->variadic);
  mp_free2(p, 256, a->escape);
  mp_free(p, Emitter, a);
}

__attribute__((returns_nonnull))
ANN2(1) Instr emit_add_instr(const Emitter emit, const f_instr f) {
  const Instr instr = mp_calloc(emit->gwion->mp, Instr);
  if((m_uint)f < 255)
    instr->opcode = (m_uint)f;
  else {
    instr->opcode = (m_uint)OP_MAX;
    instr->execute = f;
  }
  vector_add(&emit->code->instr, (vtype)instr);
  return instr;
}
