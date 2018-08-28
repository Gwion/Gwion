#include "ctrl.h"
JIT_CTRL(Event_Wait) { ctrl_next(ctrl); }

void jit_ctrl_import_event(struct Jit* jit) {
  jit_ctrl_import(jit, Event_Wait, Event_Wait_ctrl);
}

