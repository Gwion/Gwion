#include "ctrl.h"
JIT_CTRL(Reg_Push_Code) { ctrl_set_ex(ctrl); }
JIT_CTRL(Exp_Dot_Func) { ctrl_set_ex(ctrl); }
JIT_CTRL(Exp_Dot_Data) { ctrl_set_ex(ctrl); }

JIT_CTRL(Instr_Exp_Func) {
  ctrl_set_ex(ctrl);
  ctrl_next(ctrl);
}
JIT_CTRL(Call_Binary) {
  ctrl_set_ex(ctrl);
  ctrl_next(ctrl);
}

JIT_CTRL(Pre_Constructor) {
  ctrl_next(ctrl);
  ctrl_set_ex(ctrl);
}

JIT_CTRL(Func_Static) {
  ctrl_set_ex(ctrl);
  ctrl_next(ctrl);
}

JIT_CTRL(Func_Member) {
  ctrl_set_ex(ctrl);
  ctrl_next(ctrl);
}

JIT_CTRL(AutoLoopStart) { ctrl_set_ex(ctrl); ctrl_next(ctrl); }
JIT_CTRL(AutoLoopEnd) { ctrl_set_pc(ctrl, instr->m_val2); ctrl_next(ctrl); }

JIT_CTRL(Branch_Switch) {
  ctrl_set_pc(ctrl, ctrl_idx(ctrl)); // watch me
  ctrl_set_pc(ctrl, instr->m_val);
  const Map m = *(Map*)instr->ptr;
  const m_uint size = map_size(m);
  for(m_uint i = 0; i < size; ++i)
    ctrl_set_pc(ctrl, map_at(m, i));
}

JIT_CTRL(Branch_Eq_Int) {
  ctrl_next(ctrl);
  ctrl_set_pc(ctrl, instr->m_val);
}
JIT_CTRL(Branch_Neq_Int) {
  ctrl_next(ctrl);
  ctrl_set_pc(ctrl, instr->m_val);
}
JIT_CTRL(Branch_Eq_Float) {
  ctrl_next(ctrl);
  ctrl_set_pc(ctrl, instr->m_val);
}
JIT_CTRL(Branch_Neq_Float) {
  ctrl_next(ctrl);
  ctrl_set_pc(ctrl, instr->m_val);
}
JIT_CTRL(Goto) {
  ctrl_next(ctrl);
  ctrl_set_pc(ctrl, instr->m_val);
}
