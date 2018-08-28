#include "ctrl.h"

JIT_CTRL(String_Assign)         { ctrl_set_ex(ctrl); }
JIT_CTRL(Int_String_Assign)     { ctrl_set_ex(ctrl); }
JIT_CTRL(Float_String_Assign)   { ctrl_set_ex(ctrl); }
JIT_CTRL(Complex_String_Assign) { ctrl_set_ex(ctrl); }
JIT_CTRL(Polar_String_Assign)   { ctrl_set_ex(ctrl); }
JIT_CTRL(Vec3_String_Assign)    { ctrl_set_ex(ctrl); }
JIT_CTRL(Vec4_String_Assign)    { ctrl_set_ex(ctrl); }
JIT_CTRL(Object_String_Assign)  { ctrl_set_ex(ctrl); }
JIT_CTRL(Int_String)            { ctrl_set_ex(ctrl); }
JIT_CTRL(Float_String)          { ctrl_set_ex(ctrl); }
JIT_CTRL(Complex_String)        { ctrl_set_ex(ctrl); }
JIT_CTRL(Polar_String)          { ctrl_set_ex(ctrl); }
JIT_CTRL(Vec3_String)           { ctrl_set_ex(ctrl); }
JIT_CTRL(Vec4_String)           { ctrl_set_ex(ctrl); }
JIT_CTRL(Object_String)         { ctrl_set_ex(ctrl); }
JIT_CTRL(String_Plus)           { ctrl_set_ex(ctrl); }
JIT_CTRL(Int_String_Plus)       { ctrl_set_ex(ctrl); }
JIT_CTRL(Float_String_Plus)     { ctrl_set_ex(ctrl); }
JIT_CTRL(Complex_String_Plus)   { ctrl_set_ex(ctrl); }
JIT_CTRL(Polar_String_Plus)     { ctrl_set_ex(ctrl); }
JIT_CTRL(Vec3_String_Plus)      { ctrl_set_ex(ctrl); }
JIT_CTRL(Vec4_String_Plus)      { ctrl_set_ex(ctrl); }
JIT_CTRL(Object_String_Plus)    { ctrl_set_ex(ctrl); }

void jit_ctrl_import_string(struct Jit* jit) {
  jit_ctrl_import(jit, String_Assign, String_Assign_ctrl);
  jit_ctrl_import(jit, Int_String_Assign, Int_String_Assign_ctrl);
  jit_ctrl_import(jit, Float_String_Assign, Float_String_Assign_ctrl);
  jit_ctrl_import(jit, Complex_String_Assign, Complex_String_Assign_ctrl);
  jit_ctrl_import(jit, Polar_String_Assign, Polar_String_Assign_ctrl);
  jit_ctrl_import(jit, Vec3_String_Assign, Vec3_String_Assign_ctrl);
  jit_ctrl_import(jit, Vec4_String_Assign, Vec4_String_Assign_ctrl);
  jit_ctrl_import(jit, Object_String_Assign, Object_String_Assign_ctrl);
  jit_ctrl_import(jit, Int_String, Int_String_ctrl);
  jit_ctrl_import(jit, Float_String, Float_String_ctrl);
  jit_ctrl_import(jit, Complex_String, Complex_String_ctrl);
  jit_ctrl_import(jit, Polar_String, Polar_String_ctrl);
  jit_ctrl_import(jit, Vec3_String, Vec3_String_ctrl);
  jit_ctrl_import(jit, Vec4_String, Vec4_String_ctrl);
  jit_ctrl_import(jit, Object_String, Object_String_ctrl);
  jit_ctrl_import(jit, String_Plus, String_Plus_ctrl);
  jit_ctrl_import(jit, Int_String_Plus, Int_String_Plus_ctrl);
  jit_ctrl_import(jit, Float_String_Plus, Float_String_Plus_ctrl);
  jit_ctrl_import(jit, Complex_String_Plus, Complex_String_Plus_ctrl);
  jit_ctrl_import(jit, Polar_String_Plus, Polar_String_Plus_ctrl);
  jit_ctrl_import(jit, Vec3_String_Plus, Vec3_String_Plus_ctrl);
  jit_ctrl_import(jit, Vec4_String_Plus, Vec4_String_Plus_ctrl);
  jit_ctrl_import(jit, Object_String_Plus, Object_String_Plus_ctrl);
}

