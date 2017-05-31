#include "vm.h"
#include "absyn.h"
#include "oo.h"

struct Func_ {
  m_str name;
  Func_Def def;
  VM_Code code;
  m_bool is_member;
  Value value_ref;
  Value up;
  Func next;
  m_str doc;
  m_uint vt_index;
  m_uint variadic_index;
  Instr variadic_start;
  m_bool is_template; // 13/03/17
  m_bool is_dtor;
  struct VM_Object_ obj;
};

Func new_func(m_str name, Func_Def def);
