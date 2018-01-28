#ifndef EMIT_H
#define EMIT_H

typedef struct {
  m_uint curr_offset;
  struct Vector_ stack;
} Frame;

typedef struct Code_ {
  m_str  name;
  m_str  filename;
  m_uint stack_depth;
  struct Vector_ code;
  struct Vector_ stack_cont, stack_break, stack_return;
  Frame* frame;
  m_bool need_this;
} Code;

struct Emitter_ {
  Env    env;
  m_int	 default_case_index;
  m_str  filename;
  Map    cases; // passed to instr: is a pointer
  Code*  code;
  struct Vector_    stack;
#ifdef GWCOV
  m_bool coverage;
  FILE* cov_file;
#endif
};

Emitter new_emitter(Env env);
void free_emitter(Emitter emit);
VM_Code emit_code(Emitter emit);
m_bool emit_ast(Emitter emit, Ast ast, m_str filename);
m_bool emit_exp_call1(Emitter emit, Func func, Type type, int pos);
Instr emitter_add_instr(Emitter emit, f_instr f);
Code* emit_class_code(Emitter emit, m_str name);
m_bool emit_array_extend(Emitter emit, Type t, Exp e);
m_bool emit_class_finish(Emitter emit, Nspc nspc);
m_bool emit_ext_ctor(Emitter emit, VM_Code code);
#endif
