#include "ctrl.h"
JIT_CTRL(ArrayAlloc) {
  ctrl_set_ex(ctrl);
  ctrl_next(ctrl);
  ctrl_curr(ctrl);
}

JIT_CTRL(ArrayAppend)      { ctrl_set_ex(ctrl); }
JIT_CTRL(ArrayAccess)      { ctrl_set_ex(ctrl); }
JIT_CTRL(ArrayAccessMulti) { ctrl_set_ex(ctrl); }

JIT_CTRL(ArrayTop) {
  ctrl_set_pc(ctrl, instr->m_val);
  ctrl_next(ctrl);
  ctrl_curr(ctrl);
}

JIT_CTRL(ArrayBottom) {
  ctrl_set_pc(ctrl, instr->m_val);
  ctrl_next(ctrl);
  ctrl_curr(ctrl);
}
JIT_CTRL(ArrayPost) {
//  ctrl_set_pc(ctrl, instr->m_val);
  ctrl_next(ctrl);
  ctrl_curr(ctrl);
}

void jit_ctrl_import_array(struct Jit* jit) {
  jit_ctrl_import(jit, ArrayAlloc, ArrayAlloc_ctrl);
  jit_ctrl_import(jit, ArrayPost, ArrayPost_ctrl);
//  jit_ctrl_import(jit, ArrayAppend, ArrayAppend_ctrl);
//  jit_ctrl_import(jit, ArrayAccess, ArrayAccess_ctrl);
//  jit_ctrl_import(jit, ArrayAccessMulti, ArrayAccessMulti_ctrl);
  jit_ctrl_import(jit, ArrayTop, ArrayTop_ctrl);
  jit_ctrl_import(jit, ArrayBottom, ArrayBottom_ctrl);
}

