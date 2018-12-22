#ifndef __ENV
#define __ENV

#define SCOPE(a) { ++env->scope; a ;--env->scope; }
#define NSPC(a) { nspc_push_value(env->curr); SCOPE(a); nspc_pop_value(env->curr); }

struct Switch_ {
  struct Vector_ exp;
  Vector vec;
  Map cases;
  size_t default_case_index;
};

typedef struct Env_       * Env;
struct Env_ {
  m_str name;
  Nspc curr;
  Nspc global_nspc;
  struct Context_ *context;
  Type      class_def;
  Func      func;
  struct Gwion_ *gwion;
  struct Vector_    nspc_stack;
  struct Vector_    class_stack;
  struct Vector_    breaks;
  struct Vector_    conts;
  struct Vector_    known_ctx;
  struct Switch_* sw;
  size_t scope;
  size_t type_xid;
};

ANEW Env new_env();
ANN void env_reset(const Env);
ANN void free_env(Env);
ANN2(1,3) m_uint env_push(const Env, const Type, const Nspc);
ANN void env_pop(const Env, const m_uint);
ANN Map env_label(const Env);
ANN Nspc env_nspc(const Env);
ANN Type scan_type(const Env, const Type, const Type_Decl*);
ANN Type type_decl_resolve(const Env, const Type_Decl*);
ANEW ANN m_str tl2str(const Env, const Type_List); // in type_decl.c
ANN m_bool compat_func(const __restrict__ Func_Def, const __restrict__ Func_Def);
ANN Type known_type(const Env env, const Type_Decl*, const m_str);
ANN m_bool env_access(const Env env, const ae_flag flag);
ANN void env_storage(const Env env, ae_flag* flag);
ANN2(1,2) void env_add_value(const Env, const m_str, const Type, 	const m_bool, void*);
ANN void env_add_type(const Env, const Type);
ANN Type find_type(const Env, ID_List);
ANN m_bool already_defined(const Env env, const Symbol s, const uint pos);
ANN m_bool type_engine_check_prog(const Env, const Ast);
ANN Func get_func(const Env, const Func_Def);
ANN m_bool traverse_func_template(const Env env, const Func_Def def, const Type_List types);
#endif
