#include "ctrl.h"
JIT_CTRL(ugen_connect)    { ctrl_set_ex(ctrl); }
JIT_CTRL(ugen_disconnect) { ctrl_set_ex(ctrl); }
JIT_CTRL(trig_connect)    { ctrl_set_ex(ctrl); }
JIT_CTRL(trig_disconnect) { ctrl_set_ex(ctrl); }

void jit_ctrl_import_ugen(struct Jit* jit) {
  jit_ctrl_import(jit, ugen_connect, ugen_connect_ctrl);
  jit_ctrl_import(jit, ugen_disconnect, ugen_disconnect_ctrl);
  jit_ctrl_import(jit, trig_connect, trig_connect_ctrl);
  jit_ctrl_import(jit, trig_disconnect, trig_disconnect_ctrl);
}

