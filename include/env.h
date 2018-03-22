#ifndef __ENV
#define __ENV
#include "absyn.h"
#ifdef OBSTACK
#include "obstack.h"
#endif
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
  struct Vector_    known_ctx;
#ifdef OBSTACK
  struct obstack obs;
#endif
};

ANEW Env new_env();
ANN void env_reset(const Env);
ANN void free_env(Env);
ANN m_bool env_push_class(const Env, const Type);
ANN m_bool env_pop_class(const Env);
#define SCOPE(a) env->class_scope++;a;env->class_scope--;
#define NSPC(a) env->class_scope++;nspc_push_value(env->curr);a;\
nspc_pop_value(env->curr);env->class_scope--;

ANN Map env_label(const Env);
ANN Nspc env_nspc(const Env);
__attribute__((nonnull(1)))
Class_Def env_class_def(const Env, const Class_Def);
ANN Type scan_type(const Env, const Type, const Type_Decl*);
ANN Type type_decl_resolve(const Env, const Type_Decl*);
ANEW ANN m_str tl2str(const Env, const Type_List); // in type_decl.c
ANN m_bool compat_func(const Func_Def, const Func_Def);
ANN m_bool type_unknown(const ID_List, const m_str);
ANN m_bool env_add_op(const Env, const struct Op_Import*);
#endif
