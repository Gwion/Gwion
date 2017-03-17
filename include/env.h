#ifndef __ENV
#define __ENV
#include "defs.h"
#include "vm.h"

struct Env_ {
  NameSpace curr;
  NameSpace global_nspc;
//  NameSpace user_nspc;
  m_uint    class_scope;
  Context   global_context;
  Context   context;
  Vector    contexts;
  Vector    nspc_stack;
  Vector    class_stack;
  Vector    breaks;
  Vector    conts;
  Type      class_def;
  Func      func;
  VM_Object obj;
  Map known_ctx;
};

Env new_Env();
void env_reset(Env env);
void free_Env();

#endif
