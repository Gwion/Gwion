#ifndef EMIT_H
#define EMIT_H

typedef struct {
  m_uint curr_offset;
  struct Vector_ stack;
} Frame;

typedef struct Code_ {
  m_str  name;
  m_uint stack_depth;
  struct Vector_ code;
  struct Vector_ stack_cont, stack_break, stack_return;
  Frame* frame;
  ae_flag flag;
} Code;

struct Emitter_ {
  Env    env;
  m_int	 default_case_index;
  m_str  filename;
  Map    cases; // passed to instr: is a pointer
  Code*  code;
  struct Vector_    stack;
  struct Vector_    codes;
#ifdef GWCOV
  FILE* cov_file;
#endif
#ifdef GWCGRAPH
  FILE* call_file;
#endif
#ifdef GWCOV
  m_bool coverage;
#endif
#ifdef GWCGRAPH
  m_bool profile;
#endif
};

Emitter new_emitter(Env env);
void free_emitter(Emitter emit);
VM_Code emit_code(const Emitter emit)__attribute__ ((nonnull, warn_unused_result));
m_bool emit_ast(const Emitter emit, Ast ast, m_str filename) ANN;
m_bool emit_exp_call1(const Emitter emit, const Func func, const Type type) ANN;
Instr emitter_add_instr(const Emitter emit, const f_instr f) __attribute__((nonnull(1)));
Code* emit_class_code(const Emitter emit, const m_str name) ANN;
m_bool emit_array_extend(const Emitter emit, const Type t, const Exp e) ANN;
m_bool emit_class_finish(const Emitter emit, const Nspc nspc) ANN;
m_bool emit_ext_ctor(const Emitter emit, const VM_Code code) ANN;
void emit_union_offset(Decl_List l, const m_uint o) ANN;
m_bool emit_instantiate_object(const Emitter emit, const Type type, const Array_Sub array, const m_bool is_ref)
__attribute__((nonnull(1,2)));
#endif
