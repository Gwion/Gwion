#include "absyn.h"

struct Variadic_Info {
  Instr instr;
  Value value;
};

struct Func_ {
  m_str name;
  Func_Def def;
  VM_Code code;
  Value value_ref;
  Value up;
  Func next;
  m_uint vt_index;
  ae_flag flag;
  struct Variadic_Info* variadic;
  struct VM_Object_ obj;
};

Func new_func(m_str name, Func_Def def);
