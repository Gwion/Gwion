#ifndef __FUNC
#define __FUNC
struct Func_ {
  m_str name;
  Func_Def def;
  struct VM_Code_* code;
  Value value_ref;
  Func next;
  size_t vt_index;
  HAS_OBJ
  ae_flag flag;
};

ANEW ANN Func new_func(MemPool, const m_str, const Func_Def);
ANN2(1,2) Symbol func_symbol(const Env, const m_str, const m_str, const m_str, const m_uint);
ANN m_bool check_lambda(const Env, const Type, Exp_Lambda*);
#endif
