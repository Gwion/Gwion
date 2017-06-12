#ifndef EMIT_H
#define EMIT_H
#include "type.h"
#include "frame.h"

struct Emitter_ {
  Env       env;
  Vector    stack;
  Vector    funcs;
  Context   context;
  Nspc nspc;
  Func      func;
  Map       cases;
  m_int	    default_case_index;
  m_str filename;
  struct Code_*     code;
  struct VM_Object_ obj;
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
