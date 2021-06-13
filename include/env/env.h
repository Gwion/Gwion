#ifndef __ENV
#define __ENV

#define SCOPE(a)                                                               \
  {                                                                            \
    ++env->scope;                                                              \
    a;                                                                         \
    --env->scope;                                                              \
  }
#define NSPC(a)                                                                \
  {                                                                            \
    nspc_push_value(env->curr);                                                \
    SCOPE(a);                                                                  \
    nspc_pop_value(env->curr);                                                 \
  }

struct Env_Scope_ {
  struct Vector_ nspc_stack;
  struct Vector_ class_stack;
  struct Vector_ breaks;
  struct Vector_ conts;
  struct Vector_ known_ctx;
  struct Match_ *match;
  struct Vector_ effects;
  uint16_t       depth;
  bool           in_try;
  bool           in_loop;
};

typedef struct Env_ {
  m_str              name;
  Nspc               curr;
  Type               class_def;
  Func               func;
  struct Context_ *  context;
  Nspc               global_nspc;
  struct Gwion_ *    gwion;
  struct Env_Scope_ *scope;
} * Env;

ANEW Env new_env(MemPool);
ANN void env_reset(const Env);
ANN void free_env(Env);
ANN2(1, 3) m_uint env_push(const Env, const Type, const Nspc);
ANN static inline m_uint env_push_global(const Env env) {
  return env_push(env, NULL, env->global_nspc);
}
ANN void   env_pop(const Env, const m_uint);
ANN Type   scan_type(const Env, const Type, Type_Decl *);
ANN Value  mk_class(const Env env, const Type base, const loc_t);
ANN m_bool compat_func(const __restrict__ Func_Def,
                       const __restrict__ Func_Def);
ANN Type   known_type(const Env env, Type_Decl *);
ANN Type   prim_ref(const Env env, const Type t, const Type_Decl *td);
ANN m_bool env_access(const Env env, const ae_flag flag, const loc_t pos);
ANN m_bool env_storage(const Env env, ae_flag flag, const loc_t pos);
ANN void   env_add_type(const Env, const Type, const loc_t);
ANN Type   find_type(const Env, Type_Decl *);
ANN m_bool already_defined(const Env env, const Symbol s, const loc_t pos);
ANN m_bool traverse_func_template(const Env, const Func_Def);
ANN void   env_err(const Env, const loc_t pos, const m_str fmt, ...);
ANN void   env_warn(const Env, const loc_t pos, const m_str fmt, ...);
ANN void   env_error_footer(const Env env);
ANN Value  global_string(const Env env, const m_str str);
ANN void   release_ctx(struct Env_Scope_ *a, struct Gwion_ *gwion);

struct ScopeEffect {
  Symbol sym;
  loc_t  pos;
};

ANN void env_add_effect(const Env a, const Symbol effect, const loc_t pos);
ANN void call_add_effect(const Env env, const Func func, const loc_t pos);
ANN bool check_effect_overload(const Vector base, const Func override);
#endif
