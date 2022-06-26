#ifndef __FUNC
#define __FUNC

enum fflag {
  fflag_none   = 1 << 0,
  fflag_pure   = 1 << 1,
  fflag_ftmpl  = 1 << 2,
  fflag_tmpl   = 1 << 3,
  fflag_valid  = 1 << 4,
  fflag_emit   = 1 << 5,
  fflag_fptr   = 1 << 6,
} __attribute__((packed));

struct Func_ {
  Func_Def         def;
  struct VM_Code_ *code;
  Value            value_ref;
  Func             next;
  m_str            name;
  float            inline_mult;
  uint16_t         weight;  // used to mark gack use in scan1
  uint16_t         memoize; // used to mark return in scan1
  uint16_t         ref;
//  uint16_t         vt_index;
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

ANN static inline Value upvalues_lookup(const Upvalues *upvalues, const Symbol sym) {
  const Value v = (Value)scope_lookup1(upvalues->values, (m_uint)sym);
  if(v) return v;
  return upvalues->parent ? upvalues_lookup(upvalues->parent, sym) : NULL;
}

ANN static inline m_uint captures_sz(const Capture_List captures) {
  const Capture *cap = mp_vector_at(captures, Capture, (captures->len - 1));
  return cap->temp->from->offset + cap->temp->type->size;
}
#endif
