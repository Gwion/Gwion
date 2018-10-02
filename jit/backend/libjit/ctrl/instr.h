#include "ctrl.h"

JIT_CTRL(FuncUsr) {
  ctrl_next(ctrl);
}

JIT_CTRL(FuncOp) {
  ctrl_next(ctrl);
}

JIT_CTRL(PreCtor) {
//ctrl_skip(ctrl);
  if(GET_FLAG((VM_Code)instr->m_val, NATIVE_NOT))
    ctrl_next(ctrl);
}

JIT_CTRL(FuncStatic) {
  ctrl_next(ctrl);
}

JIT_CTRL(FuncMember) {
  ctrl_next(ctrl);
}

JIT_CTRL(FuncPtr) {
  ctrl_next(ctrl);
}

JIT_CTRL(AutoLoopStart) { ctrl_next(ctrl); }
JIT_CTRL(AutoLoopEnd) {
  ctrl_set_pc(ctrl, *(m_uint*)instr->ptr + 1);
  ctrl_next(ctrl);
}

JIT_CTRL(BranchSwitch) {
  const Map m = (Map)instr->m_val2;
  const m_uint size = map_size(m);
  for(m_uint i = 0; i < size; ++i)
    ctrl_set_pc(ctrl, map_at(m, i));
}

JIT_CTRL(BranchEqInt) {
  ctrl_set_pc(ctrl, instr->m_val);
}
JIT_CTRL(BranchNeqInt) {
  ctrl_set_pc(ctrl, instr->m_val);
}
JIT_CTRL(BranchEqFloat) {
  ctrl_set_pc(ctrl, instr->m_val);
}
JIT_CTRL(BranchNeqFloat) {
  ctrl_set_pc(ctrl, instr->m_val);
}
JIT_CTRL(Goto) {
  ctrl_set_pc(ctrl, instr->m_val);
}


void jit_ctrl_import_instr(struct Jit* jit) {
  jit_ctrl_import(jit, FuncUsr, FuncUsr_ctrl);
  jit_ctrl_import(jit, FuncOp, FuncOp_ctrl);
  jit_ctrl_import(jit, PreCtor, PreCtor_ctrl);
  jit_ctrl_import(jit, FuncStatic, FuncStatic_ctrl);
  jit_ctrl_import(jit, FuncMember, FuncMember_ctrl);
  jit_ctrl_import(jit, FuncPtr, FuncPtr_ctrl);
//  jit_ctrl_import(jit, AutoLoopStart, AutoLoopStart_ctrl);
  jit_ctrl_import(jit, AutoLoopEnd, AutoLoopEnd_ctrl);
  jit_ctrl_import(jit, BranchSwitch, BranchSwitch_ctrl);
  jit_ctrl_import(jit, BranchEqInt, BranchEqInt_ctrl);
  jit_ctrl_import(jit, BranchNeqInt, BranchNeqInt_ctrl);
  jit_ctrl_import(jit, BranchEqFloat, BranchEqFloat_ctrl);
  jit_ctrl_import(jit, BranchNeqFloat, BranchNeqFloat_ctrl);
  jit_ctrl_import(jit, Goto, Goto_ctrl);
}

