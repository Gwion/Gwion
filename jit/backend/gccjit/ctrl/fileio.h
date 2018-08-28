#include "ctrl.h"
JIT_CTRL(int_to_file)    { ctrl_set_ex(ctrl); }
JIT_CTRL(float_to_file)  { ctrl_set_ex(ctrl); }
JIT_CTRL(string_to_file) { ctrl_set_ex(ctrl); }
JIT_CTRL(object_to_file) { ctrl_set_ex(ctrl); }
JIT_CTRL(polar_to_file)  { ctrl_set_ex(ctrl); }
JIT_CTRL(vec3_to_file)   { ctrl_set_ex(ctrl); }
JIT_CTRL(vec4_to_file)   { ctrl_set_ex(ctrl); }
JIT_CTRL(file_to_int)    { ctrl_set_ex(ctrl); }
JIT_CTRL(file_to_float)  { ctrl_set_ex(ctrl); }
JIT_CTRL(file_to_string) { ctrl_set_ex(ctrl); }

void jit_ctrl_import_fileio(struct Jit* jit) {
  jit_ctrl_import(jit, int_to_file, int_to_file_ctrl);
  jit_ctrl_import(jit, float_to_file, float_to_file_ctrl);
  jit_ctrl_import(jit, string_to_file, string_to_file_ctrl);
  jit_ctrl_import(jit, object_to_file, object_to_file_ctrl);
  jit_ctrl_import(jit, polar_to_file, polar_to_file_ctrl);
  jit_ctrl_import(jit, vec3_to_file, vec3_to_file_ctrl);
  jit_ctrl_import(jit, vec4_to_file, vec4_to_file_ctrl);
  jit_ctrl_import(jit, file_to_int, file_to_int_ctrl);
  jit_ctrl_import(jit, file_to_float, file_to_float_ctrl);
  jit_ctrl_import(jit, file_to_string, file_to_string_ctrl);
}

