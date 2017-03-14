#ifndef EMIT_H
#define EMIT_H
#include "type.h"
#include "frame.h"

m_bool get_instr(Emitter emit, Operator op, Type rhs, Type lhs);

typedef struct {
  m_str name;
  m_str filename;
  m_uint stack_depth;
  m_bool need_this;
  Vector code;
  Vector stack_cont, stack_break, stack_return;
  Frame* frame;
} Code;

Code* new_Code();
void free_Code(Code* code);
struct Emitter_ {
  Env       env;
  Vector    stack;
  Vector    spork;
  Vector    funcs;
  Vector    array; // a vector to hold empty array types.
  Code*     code;
  Context   context;
  NameSpace nspc;
  Func      func;
  VM_Object obj;
//  Scope     labels;
  Map       cases;
  m_int			default_case_index;
};


Emitter new_Emitter (Env env);
void free_Emitter(Emitter emit);
VM_Code emit_to_code(Emitter emit);
m_bool emit_Ast(Emitter emit, Ast ast, m_str filename);

/* label */
Vector emit_lookup_label(Emitter emit, S_Symbol xid, int climb);
void emit_add_label(Emitter emit, S_Symbol xid, void* value);
void emit_push_label(Emitter emit);
void emit_pop_label(Emitter emit);

m_bool emit_Func_Call1(Emitter emit, Func func, Type type, int pos);

Instr add_instr(Emitter emit, f_instr f);
#endif
