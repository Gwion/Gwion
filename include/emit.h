#ifndef EMIT_H
#define EMIT_H
#include "type.h"
#include "frame.h"

typedef struct Code_ {
  m_str name;
  m_str filename;
  m_uint stack_depth;
  struct Vector_ code;
  struct Vector_ stack_cont, stack_break, stack_return;
  Frame* frame;
  m_bool need_this;
} Code;

struct Emitter_ {
  Env       env;
  m_int	    default_case_index;
  m_str filename;
  Map       cases; // passed to instr: is a pointer
  struct Vector_    stack;
  struct Code_*     code;
};

Emitter new_emitter(Env env);
void free_emitter(Emitter emit);
VM_Code emit_code(Emitter emit);
m_bool emit_ast(Emitter emit, Ast ast, m_str filename);

Vector emit_lookup_label(Emitter emit, S_Symbol xid, int climb);
void emit_add_label(Emitter emit, S_Symbol xid, void* value);
void emit_push_label(Emitter emit);
void emit_pop_label(Emitter emit);

m_bool emit_exp_call1(Emitter emit, Func func, Type type, int pos);
#endif
