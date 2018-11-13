struct Func_ {
  m_str name;
  Func_Def def;
  struct VM_Code_* code;
  Value value_ref;
  Func next;
  m_uint vt_index;
  struct Instr_* variadic;
  ae_flag flag;
  HAS_OBJ
};

ANEW ANN Func new_func(const m_str name, const Func_Def def);
ANN void free_func_simple(Func);
