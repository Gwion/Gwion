#include "ctrl.h"
JIT_CTRL(Instr_Array_Alloc) { ctrl_set_ex(ctrl); }
JIT_CTRL(Instr_Array_Access) { ctrl_set_ex(ctrl); }
JIT_CTRL(Instr_Array_Access_Multi) { ctrl_set_ex(ctrl); }

JIT_CTRL(Instr_Pre_Ctor_Array_Top) {
  ctrl_set_pc(ctrl, instr->m_val); // conditionnal
  ctrl_next(ctrl);
}

JIT_CTRL(Instr_Pre_Ctor_Array_Bottom) {
  ctrl_curr(ctrl); // ???
  ctrl_next(ctrl);
  ctrl_set_pc(ctrl, instr->m_val);
}
