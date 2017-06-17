//#include "vm.h"
#include "absyn.h"
//#include "oo.h"

struct Func_ {
  m_str name;
  Func_Def def;
  VM_Code code;
  Value value_ref;
  Value up;
  Func next;
  m_str doc;
  m_uint vt_index;
  Instr variadic_start;
  ae_flag flag;
  struct VM_Object_ obj;
};

Func new_func(m_str name, Func_Def def);
