#include "ctrl.h"
JIT_CTRL(VarargTop) {
  ctrl_next(ctrl);
  ctrl_set_pc(ctrl, instr->m_val2 + 1);
}

JIT_CTRL(VarargEnd) { ctrl_next(ctrl); }

void jit_ctrl_import_vararg(struct Jit* jit) {
  jit_ctrl_import(jit, VarargTop, VarargTop_ctrl);
  jit_ctrl_import(jit, VarargEnd, VarargEnd_ctrl);
}

