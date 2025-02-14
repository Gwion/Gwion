#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "emit.h"

static ANEW ANN VM_Code emit_code(const Emitter emit) {
  Code *const   c          = emit->code;
  const bool    has_values = m_vector_size(&c->live_values);
  const VM_Code code       = new_vmcode(emit->gwion->mp, &c->instr,
                                  has_values ? &c->live_values : NULL, c->name,
                                  c->stack_depth, false, emit->info->dump);
  if (has_values) c->live_values.ptr = NULL;
  return code;
}

ANEW Emitter new_emitter(MemPool p) {
  Emitter emit = (Emitter)mp_calloc(p, Emitter);
  vector_init(&emit->stack);
  emit->info = (struct EmitterInfo_ *)mp_calloc(p, EmitterInfo);
  emit->info->emit_code = emit_code;
  return emit;
}

ANN void free_emitter(MemPool p, Emitter a) {
  vector_release(&a->stack);
  mp_free(p, EmitterInfo, a->info);
  mp_free(p, Emitter, a);
}

__attribute__((returns_nonnull)) ANN2(1) Instr
new_instr(const MemPool mp, const f_instr f) {
  const Instr instr = mp_calloc(mp, Instr);
  if ((m_uint)f < 255)
    instr->opcode = (m_uint)f;
  else {
    instr->opcode  = eOP_MAX;
    instr->execute = f;
  }
  return instr;
}
__attribute__((returns_nonnull)) ANN2(1) Instr
emit_add_instr(const Emitter emit, const f_instr f) {
  const Instr instr = new_instr(emit->gwion->mp, f);
  vector_add(&emit->code->instr, (vtype)instr);
  return instr;
}

ANN2(1) void emit_fast_except(const Emitter emit, const ValueFrom *vf, const loc_t loc) {
  const Instr instr = emit_add_instr(emit, fast_except);
  if(!emit->info->debug && vf) {
    struct FastExceptInfo *info = mp_malloc2(emit->gwion->mp, sizeof(struct FastExceptInfo));
    info->file = emit->env->name;
    info->loc = loc;
    info->file2 = vf->filename;
    info->loc2 = vf->loc;
    instr->m_val2 = (m_uint)info;
  }
}
