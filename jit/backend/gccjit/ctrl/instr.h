#include "ctrl.h"
JIT_CTRL(RegPushCode)   { ctrl_set_ex(ctrl); }
JIT_CTRL(DotFunc)  { ctrl_set_ex(ctrl); }
JIT_CTRL(DotData)  { ctrl_set_ex(ctrl); }

JIT_CTRL(FuncUsr) {
//  ctrl_set_nf(ctrl);
  ctrl_set_ex(ctrl);
  ctrl_next(ctrl);
}
JIT_CTRL(FuncOp) {
//  ctrl_set_nf(ctrl);
  ctrl_set_ex(ctrl);
  ctrl_next(ctrl);
}

JIT_CTRL(PreCtor) {
  //  ctrl_set_nf(ctrl);
//  if(GET_FLAG((VM_Code)instr->m_val, NATIVE_NOT)) {
    ctrl_next(ctrl);
    ctrl_set_ex(ctrl);
//  }
}

JIT_CTRL(FuncStatic) {
  ctrl_set_ex(ctrl);
//  ctrl_set_nf(ctrl);
  ctrl_next(ctrl);
}

JIT_CTRL(FuncMember) {
  ctrl_set_ex(ctrl);
//  ctrl_set_nf(ctrl);
  ctrl_next(ctrl);
}

JIT_CTRL(FuncPtr) {
  ctrl_set_ex(ctrl);
//  ctrl_set_nf(ctrl);
  ctrl_next(ctrl);
}

JIT_CTRL(AutoLoopStart) { ctrl_set_ex(ctrl); ctrl_next(ctrl); }
JIT_CTRL(AutoLoopEnd) {
  ctrl_set_pc(ctrl, instr->m_val2 + 1);
 ctrl_next(ctrl);
}

JIT_CTRL(BranchSwitch) {
  ctrl_set_pc(ctrl, ctrl_idx(ctrl)); // watch me
  ctrl_set_pc(ctrl, instr->m_val);
  const Map m = *(Map*)instr->ptr;
   const m_uint size = map_size(m);
  for(m_uint i = 0; i < size; ++i)
    ctrl_set_pc(ctrl, map_at(m, i));
}

JIT_CTRL(BranchEqInt) {
  ctrl_next(ctrl);
  ctrl_set_pc(ctrl, instr->m_val);
}
JIT_CTRL(BranchNeqInt) {
  ctrl_next(ctrl);
  ctrl_set_pc(ctrl, instr->m_val);
}
JIT_CTRL(BranchEqFloat) {
  ctrl_next(ctrl);
  ctrl_set_pc(ctrl, instr->m_val);
}
JIT_CTRL(BranchNeqFloat) {
  ctrl_next(ctrl);
  ctrl_set_pc(ctrl, instr->m_val);
}
JIT_CTRL(Goto) {
  ctrl_next(ctrl);
  ctrl_set_pc(ctrl, instr->m_val);
}

void jit_ctrl_import_instr(struct Jit* jit) {
  jit_ctrl_import(jit, RegPushCode, RegPushCode_ctrl);
  jit_ctrl_import(jit, DotFunc, DotFunc_ctrl);
  jit_ctrl_import(jit, DotData, DotData_ctrl);
  jit_ctrl_import(jit, FuncUsr, FuncUsr_ctrl);
  jit_ctrl_import(jit, FuncOp, FuncOp_ctrl);
  jit_ctrl_import(jit, PreCtor, PreCtor_ctrl);
  jit_ctrl_import(jit, FuncStatic, FuncStatic_ctrl);
  jit_ctrl_import(jit, FuncMember, FuncMember_ctrl);
  jit_ctrl_import(jit, FuncPtr, FuncPtr_ctrl);
  jit_ctrl_import(jit, AutoLoopStart, AutoLoopStart_ctrl);
  jit_ctrl_import(jit, AutoLoopEnd, AutoLoopEnd_ctrl);
  jit_ctrl_import(jit, BranchSwitch, BranchSwitch_ctrl);
  jit_ctrl_import(jit, BranchEqInt, BranchEqInt_ctrl);
  jit_ctrl_import(jit, BranchNeqInt, BranchNeqInt_ctrl);
  jit_ctrl_import(jit, BranchEqFloat, BranchEqFloat_ctrl);
  jit_ctrl_import(jit, BranchNeqFloat, BranchNeqFloat_ctrl);
  jit_ctrl_import(jit, Goto, Goto_ctrl);
}

