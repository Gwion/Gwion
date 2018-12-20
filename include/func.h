#ifndef __FUNC
#define __FUNC
struct Func_ {
  m_str name;
  Func_Def def;
  struct VM_Code_* code;
  Value value_ref;
  Func next;
  struct Instr_* variadic;
  size_t vt_index;
  ae_flag flag;
  HAS_OBJ
};

ANEW ANN Func new_func(const m_str name, const Func_Def def);
ANN2(1,2) Symbol func_symbol(const m_str, const m_str, const m_str, const m_uint);
#endif
