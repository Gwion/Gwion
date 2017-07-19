#include "absyn.h"

struct Func_ {
  m_str name;
  Func_Def def;
  VM_Code code;
  Value value_ref;
  Value up;
  Func next;
  m_uint vt_index;
  ae_flag flag;
  Instr variadic;
  struct VM_Object_ obj;
};

Func new_func(m_str name, Func_Def def);
