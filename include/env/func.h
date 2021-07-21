#ifndef __FUNC
#define __FUNC

enum fflag {
  fflag_none   = 1 << 0,
  fflag_pure   = 1 << 1,
  fflag_ftmpl  = 1 << 2,
  fflag_tmpl   = 1 << 3,
  fflag_valid  = 1 << 4,
  fflag_emit   = 1 << 5,
  fflag_return = 1 << 6,
  fflag_recurs = 1 << 7,
} __attribute__((packed));

struct Func_ {
  Func_Def         def;
  struct VM_Code_ *code;
  Value            value_ref;
  Func             next;
  m_str            name;
  struct Map_      upvalues;
  float            inline_mult;
  uint16_t         weight;
  uint16_t         memoize;
  uint16_t         ref;
  uint16_t         vt_index;
  ae_flag          flag;
  enum fflag       fflag;
};

REF_FUNC(Func, func)
FLAG_FUNC(Func, f)

ANEW ANN Func new_func(MemPool, const m_str, const Func_Def);
ANN2(1, 2)
Symbol     func_symbol(const Env, const m_str, const m_str, const m_str,
                       const m_uint);
ANN m_bool check_lambda(const Env, const Type, Exp_Lambda *);
ANN Type   check_op_call(const Env env, Exp_Call *const exp);
ANN void   builtin_func(const MemPool mp, const Func f, void *func_ptr);
#endif
