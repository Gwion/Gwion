#ifndef EMIT_H
#define EMIT_H

typedef struct Frame_ {
  size_t curr_offset;
  struct Vector_ stack;
} Frame;

typedef struct Code_ {
  m_str  name;
  size_t stack_depth;
  struct Vector_ instr;
  struct Vector_ stack_cont, stack_break, stack_return;
  Frame* frame;
  ae_flag flag;
} Code;

struct EmitterInfo_ {
  struct Vector_ pure;
  char *escape;
  f_instr finalyzer;
  VM_Code (*emit_code)(const Emitter);
  VM_Code code;
  m_bool memoize;
};

struct Emitter_ {
  Env    env;
  Code*  code;
  struct Gwion_ *gwion;
  struct EmitterInfo_ *info;
  struct Vector_ stack;
};

ANEW ANN Emitter new_emitter(MemPool);
ANN void free_emitter(MemPool, Emitter);
ANN m_bool emit_ast(const Env env, Ast ast);
ANN Instr emit_exp_call1(const Emitter, const Func);
ANN2(1) Instr emit_add_instr(const Emitter, const f_instr) __attribute__((returns_nonnull));
ANN Code* emit_class_code(const Emitter, const m_str);
ANN m_bool emit_array_extend(const Emitter, const Type, const Exp);
ANN void emit_class_finish(const Emitter, const Nspc);
ANN void emit_union_offset(Decl_List, const m_uint);
ANN2(1,2) m_bool emit_instantiate_object(const Emitter, const Type, const Array_Sub, const m_bool);
ANN m_uint emit_code_offset(const Emitter emit);
ANN m_uint emit_local(const Emitter emit, const Type t);
ANN Instr emit_exp_spork(const Emitter, const Exp_Unary*);
ANN m_bool emit_exp(const Emitter, const Exp);
ANN static inline void emit_except(const Emitter emit, const Type t) {
  emit_add_instr(emit, !GET_FLAG(t, nonnull) ? GWOP_EXCEPT : SetObj);
}
#endif
