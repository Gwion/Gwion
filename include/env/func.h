#ifndef __FUNC
#define __FUNC

enum fflag {
  fflag_none  = 1 << 0,
  fflag_pure  = 1 << 1,
  fflag_ftmpl = 1 << 2,
  fflag_tmpl  = 1 << 3,
  fflag_valid = 1 << 4,
} __attribute__((packed));

struct Func_ {
  m_str name;
  Func_Def def;
  struct VM_Code_* code;
  Value value_ref;
  Func next;
  size_t vt_index;
  struct Map_ upvalues;
  uint16_t ref;
  ae_flag flag;
  enum fflag fflag;
};

REF_FUNC(Func, func)
FLAG_FUNC(Func, f)

ANEW ANN Func new_func(MemPool, const m_str, const Func_Def);
ANN2(1,2) Symbol func_symbol(const Env, const m_str, const m_str, const m_str, const m_uint);
ANN m_bool check_lambda(const Env, const Type, Exp_Lambda*);
ANN void builtin_func(const MemPool mp, const Func f, void* func_ptr);
#endif
