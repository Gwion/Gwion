#ifndef __ENV
#define __ENV
#include "defs.h"
#include "absyn.h"
#include "vm.h"
#include "oo.h"
struct Env_ {
  Nspc curr;
  Nspc global_nspc;
//  Nspc user_nspc;
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
  Map known_ctx;
  Exp_Func* current; // template helper
  struct VM_Object_ obj;
};

Env new_env();
void env_reset(Env env);
void free_env();

#endif
