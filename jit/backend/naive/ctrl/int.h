#include "ctrl.h"
JIT_CTRL(int_divide)   { ctrl_set_ex(ctrl); }
JIT_CTRL(int_modulo)   { ctrl_set_ex(ctrl); }
JIT_CTRL(int_r_divide) { ctrl_set_ex(ctrl); }
JIT_CTRL(int_r_modulo) { ctrl_set_ex(ctrl); }

void jit_ctrl_import_int(struct Jit* jit) {
  jit_ctrl_import(jit, int_divide, int_divide_ctrl);
  jit_ctrl_import(jit, int_modulo, int_modulo_ctrl);
  jit_ctrl_import(jit, int_r_divide, int_r_divide_ctrl);
  jit_ctrl_import(jit, int_r_modulo, int_r_modulo_ctrl);
}
