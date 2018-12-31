#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "nspc.h"
#include "value.h"
#include "instr.h"
#include "emit.h"

ANEW Emitter new_emitter(void) {
  Emitter emit = (Emitter)xcalloc(1, sizeof(struct Emitter_));
  vector_init(&emit->stack);
  return emit;
}

ANN void free_emitter(Emitter a) {
  vector_release(&a->stack);
  xfree(a);
}

__attribute__((returns_nonnull))
ANN2(1) Instr emit_add_instr(const Emitter emit, const f_instr f) {
  const Instr instr = mp_alloc(Instr);
  instr->execute = f;
  vector_add(&emit->code->instr, (vtype)instr);
  return instr;
}
