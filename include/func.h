#include "vm.h"
#include "absyn.h"
#include "oo.h"

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
  struct VM_Object_ obj;
  m_bool is_template;
  m_bool is_dtor;
  m_bool is_member;
};

Func new_func(m_str name, Func_Def def);
