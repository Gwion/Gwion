#ifndef EMIT_H
#define EMIT_H
#include "type.h"
#include "frame.h"

struct Emitter_ {
  Env       env;
  Vector    stack;
  Vector    spork;
  Vector    funcs;
  Context   context;
  NameSpace nspc;
  Func      func;
  Map       cases;
  m_int	    default_case_index;
  struct Code_*     code;
  struct VM_Object_ obj;
};

Emitter new_Emitter (Env env);
void free_Emitter(Emitter emit);
VM_Code emit_to_code(Emitter emit);
m_bool emit_Ast(Emitter emit, Ast ast, m_str filename);

Vector emit_lookup_label(Emitter emit, S_Symbol xid, int climb);
void emit_add_label(Emitter emit, S_Symbol xid, void* value);
void emit_push_label(Emitter emit);
void emit_pop_label(Emitter emit);

m_bool emit_Func_Call1(Emitter emit, Func func, Type type, int pos);
#endif
