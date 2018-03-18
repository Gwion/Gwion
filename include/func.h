#include "absyn.h"

struct Func_ {
  m_str name;
  Func_Def def;
  VM_Code code;
  Value value_ref;
  Func next;
  m_uint vt_index;
  ae_flag flag;
  Instr variadic;
  struct VM_Object_ obj;
};

ANEW ANN Func new_func(const m_str name, const Func_Def def);
Func new_func_simple();
ANN Func get_func(const Env, const Func_Def);
ANN void free_func_simple(Func);
