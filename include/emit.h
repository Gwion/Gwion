#ifndef EMIT_H
#define EMIT_H

typedef struct Frame_ {
  m_uint curr_offset;
  struct Vector_ stack;
} Frame;

typedef struct Code_ {
  m_str  name;
  m_uint stack_depth;
  struct Vector_ instr;
  struct Vector_ stack_cont, stack_break, stack_return;
  Frame* frame;
  ae_flag flag;
} Code;

struct Emitter_ {
  Env    env;
  Code*  code;
  struct Vector_    stack;
  m_str  filename;
  Map    cases; // passed to instr: is a pointer
  m_int	 default_case_index;
};

ANEW ANN Emitter new_emitter(void/*const Env*/);
ANN void free_emitter(Emitter);
ANEW ANN VM_Code emit_code(const Emitter);
ANN m_bool emit_ast(const Emitter emit, Ast ast, m_str filename);
ANN m_bool emit_exp_call1(const Emitter, const Func);
ANN2(1) Instr emitter_add_instr(const Emitter, const f_instr) __attribute__((returns_nonnull));
ANN Code* emit_class_code(const Emitter, const m_str);
ANN m_bool emit_array_extend(const Emitter, const Type, const Exp);
ANN void emit_class_finish(const Emitter, const Nspc);
ANN void emit_ext_ctor(const Emitter, const VM_Code);
ANN void emit_union_offset(Decl_List, const m_uint);
ANN2(1,2) m_bool emit_instantiate_object(const Emitter, const Type, const Array_Sub, const m_bool);
#endif
