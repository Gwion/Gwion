#include "ctrl.h"

JIT_CTRL(VarargTop) {
  ctrl_set_pc(ctrl, instr->m_val2 + 1);
}

void jit_ctrl_import_vararg(struct Jit* jit) {
  jit_ctrl_import(jit, VarargTop, VarargTop_ctrl);
}
