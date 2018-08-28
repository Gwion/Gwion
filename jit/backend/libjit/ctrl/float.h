#include "ctrl.h"
JIT_CTRL(Time_Advance) { ctrl_next(ctrl); }

void jit_ctrl_import_float(struct Jit* jit) {
  jit_ctrl_import(jit, Time_Advance, Time_Advance_ctrl);
}

