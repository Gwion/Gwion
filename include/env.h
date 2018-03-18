#ifndef __ENV
#define __ENV
#include "absyn.h"
#include "obstack.h"

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
  struct obstack obs;
};

ANEW const Env new_env();
ANN void env_reset(const Env);
ANN void free_env(Env);
ANN const m_bool env_push_class(const Env, const Type);
ANN const m_bool env_pop_class(const Env);
#define SCOPE(a) env->class_scope++;a;env->class_scope--;
#define NSPC(a) env->class_scope++;nspc_push_value(env->curr);a;\
nspc_pop_value(env->curr);env->class_scope--;

ANN const Map env_label(const Env);
ANN const Nspc env_nspc(const Env);
__attribute__((nonnull(1)))
const Class_Def env_class_def(const Env, const Class_Def);
Type scan_type(Env, Type, const Type_Decl*);
Type type_decl_resolve(Env env, const Type_Decl* td);
m_str tl2str(Env env, Type_List tl);
ANN m_bool compat_func(const Func_Def, const Func_Def);
m_bool type_unknown(ID_List, m_str);
ANN const m_bool env_add_op(const Env, const struct Op_Import*);
#endif
