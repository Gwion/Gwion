#include "ctrl.h"
JIT_CTRL(EventWait) { ctrl_next(ctrl); }

void jit_ctrl_import_event(struct Jit* jit) {
  jit_ctrl_import(jit, EventWait, EventWait_ctrl);
}

