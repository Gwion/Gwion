#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "emit.h"
#include "object.h"
#include "shreduler_private.h"

ANN void comptime_ini(const Emitter emit, const m_str name) {
  emit_push_code(emit, name); // new code {
}

ANN2(1) void comptime_end(const Emitter emit, const size_t size, void *data) {
  if (size) {
    emit_regmove(emit, -size);
    const Instr instr = emit_add_instr(emit, _staticmemcpy_);
    instr->m_val  = (m_uint)data;
    instr->m_val2 = size;
  }
  const VM *vm = emit->gwion->vm;
  const VM_Code code = finalyze(emit, EOC); // } new code
  const VM_Shred shred = vm->cleaner_shred;
  shred->code = code;
  shred->info->me->ref++;
  shredule(vm->shreduler, shred, 0);
  const bool loop = vm->shreduler->loop;
  vm_run(vm);
  shred->code = NULL;
  vm->bbq->is_running = true;
  vm->shreduler->loop = loop;
}
