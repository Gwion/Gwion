#include "ctrl.h"
JIT_CTRL(Vararg_start) {
  ctrl_next(ctrl);
  ctrl_set_pc(ctrl, instr->m_val2 + 1);
}

JIT_CTRL(Vararg_end) { ctrl_next(ctrl); }
