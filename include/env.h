#ifndef __ENV
#define __ENV
#include "defs.h"
#include "absyn.h"
#include "vm.h"
#include "oo.h"
#include "map_private.h"
struct Env_ {
  Nspc curr;
  Nspc global_nspc;
//  Nspc user_nspc;
  m_uint    class_scope;
  Context   context;
  Type      class_def;
  Func      func;
  Exp_Func* current; // template helper
  m_uint type_xid;
  struct Vector_    contexts;
  struct Vector_    nspc_stack;
  struct Vector_    class_stack;
  struct Vector_    breaks;
  struct Vector_    conts;
  struct Map_       known_ctx;
  m_bool do_type_xid;
};

Env new_env();
void env_reset(Env env);
void free_env();
m_bool env_push_class(Env env, Type type);
m_bool env_pop_class(Env env);
#define SCOPE(a) env->class_scope++;a;env->class_scope--;
#define NSPC(a) env->class_scope++;nspc_push_value(env->curr);a;\
nspc_pop_value(env->curr);env->class_scope--;


#endif
