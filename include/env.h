#ifndef __ENV
#define __ENV
#include "absyn.h"

#define SCOPE(a) { ++env->class_scope;a;--env->class_scope; }
#define NSPC(a) { nspc_push_value(env->curr); SCOPE(a); nspc_pop_value(env->curr); }


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
  struct Vector_    nspc_stack;
  struct Vector_    class_stack;
  struct Vector_    breaks;
  struct Vector_    conts;
  struct Vector_    known_ctx;
};

ANEW Env new_env();
ANN void env_reset(const Env);
ANN void free_env(Env);
ANN2(1,3,4) void env_push(const Env, const Type, const Nspc, m_uint*);
ANN void env_pop(const Env, const m_uint);
ANN Map env_label(const Env);
ANN Nspc env_nspc(const Env);
ANN Type scan_type(const Env, const Type, const Type_Decl*);
ANN Type type_decl_resolve(const Env, const Type_Decl*);
ANEW ANN m_str tl2str(const Env, const Type_List); // in type_decl.c
ANN m_bool compat_func(const Func_Def, const Func_Def);
ANN m_bool type_unknown(const ID_List, const m_str);
ANN m_bool env_add_op(const Env, const struct Op_Import*);
ANN m_bool env_access(const Env env, const ae_flag flag);
ANN void env_storage(const Env env, ae_flag* flag);
#endif
