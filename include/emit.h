#ifndef EMIT_H
#define EMIT_H

typedef struct Frame_ {
  size_t curr_offset;
  struct Vector_ stack;
  struct Vector_ defer;
  struct Map_    handlers;
  m_uint try_top;
} Frame;

typedef struct Code_ {
  Frame* frame;
  size_t stack_depth;
  struct Vector_ instr;
  struct Vector_  stack_cont;
  struct Vector_ stack_break;
  struct Vector_ stack_return;
  m_str  name;
} Code;

struct EmitterInfo_ {
  struct Vector_ pure;
  char *escape;
  VM_Code (*emit_code)(const Emitter);
  VM_Code code;
  uint memoize;
  uint unroll;
  uint line;
  bool debug;
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
ANN m_bool emit_func_def(const Emitter emit, const Func_Def fdef);
ANN m_bool emit_exp_call1(const Emitter, const Func);
ANN2(1) Instr emit_add_instr(const Emitter, const f_instr) __attribute__((returns_nonnull));
ANN Code* emit_class_code(const Emitter, const m_str);
ANN m_bool emit_array_extend(const Emitter, const Type, const Exp);
ANN void emit_class_finish(const Emitter, const Nspc);
ANN2(1,2) m_bool emit_instantiate_object(const Emitter, const Type, const Array_Sub, const m_bool);
ANN m_uint emit_code_offset(const Emitter emit);
ANN m_uint emit_local(const Emitter emit, const Type t);
ANN m_bool emit_exp_spork(const Emitter, const Exp_Unary*);
ANN m_bool emit_exp(const Emitter, const Exp);
ANN static inline void emit_gc(const Emitter emit, const m_int offset) {
  const Instr gc = emit_add_instr(emit, GcAdd);
  gc->m_val = offset;
}


ANN Instr emit_object_addref(const Emitter emit, const m_int size, const m_bool emit_var);
ANN Instr emit_struct_addref(const Emitter emit, const Type t, const m_int size, const m_bool emit_var);
ANN static inline Instr emit_compound_addref(const Emitter emit, const Type t, const m_int size, const m_bool emit_var) {
  return !tflag(t, tflag_struct) ? emit_object_addref(emit, size, emit_var) :
                                                emit_struct_addref(emit, t, size, emit_var);
}
#endif
