#ifndef __PARSE
#define __PARSE

#include "gwion.h"
#define insert_symbol(a) insert_symbol(env->gwion->st, (a))

#undef ERR_B
#define ERR_B(a, b, ...) { env_err(env, (a), (b), ## __VA_ARGS__); return GW_ERROR; }
#undef ERR_O
#define ERR_O(a, b, ...) { env_err(env, (a), (b), ## __VA_ARGS__); return NULL; }

#define RET_NSPC(exp)       \
++env->scope->depth;        \
nspc_push_value(env->gwion->p, env->curr); \
const m_bool ret = exp;     \
nspc_pop_value(env->gwion->p, env->curr);  \
--env->scope->depth;        \
return ret;

#define SET_ACCESS(a,b)       \
if(GET_FLAG(a, private))      \
  SET_FLAG(b, private);       \
else if(GET_FLAG(a, protect)) \
  SET_FLAG(b, protect);

typedef m_bool (*_exp_func)(const void*, const void*);
static inline m_bool dummy_func(const void*a NUSED,
  const void*b NUSED) { return 1; }
#define DECL_EXP_FUNC(prefix)                                                                     \
static const _exp_func exp_func[] = {                                                             \
  (_exp_func)prefix##_exp_decl,    (_exp_func)prefix##_exp_binary, (_exp_func)prefix##_exp_unary, \
  (_exp_func)prefix##_exp_primary, (_exp_func)prefix##_exp_cast,   (_exp_func)prefix##_exp_post,  \
  (_exp_func)prefix##_exp_call,    (_exp_func)prefix##_exp_array,  (_exp_func)prefix##_exp_if,    \
  (_exp_func)prefix##_exp_dot,     (_exp_func)prefix##_exp_lambda, (_exp_func)prefix##_exp_typeof \
};

#define DECL_SECTION_FUNC(prefix)                                                                 \
static const _exp_func section_func[] = {                                                         \
  (_exp_func)prefix##_stmt_list, (_exp_func)prefix##_func_def, (_exp_func)prefix##_class_def      \
};                                                                                                \
ANN static inline m_bool prefix##_section(const void* a, /* const */ Section* section) { GWDEBUG_EXE    \
  void* d = &section->d.stmt_list;                            \
  return section_func[section->section_type](a, *(void**)d);                            \
}

#define HANDLE_EXP_FUNC(prefix, type, ret)                                                        \
DECL_EXP_FUNC(prefix)                                                                             \
ANN type prefix##_exp(const Env env, Exp exp) { GWDEBUG_EXE                         \
  do CHECK_BB(exp_func[exp->exp_type](env, &exp->d))                                              \
  while((exp = exp->next));                                                                       \
  return ret;                                                                                     \
}
ANN m_bool scan1_exp(const Env, Exp);
ANN m_bool scan2_exp(const Env, Exp);

#define describe_stmt_func(prefix, name, type, prolog, exp) \
ANN static m_bool prefix##_stmt_##name(const Env env, const type stmt) { GWDEBUG_EXE \
  RET_NSPC(exp) \
}

ANN m_uint union_push(const Env, const Stmt_Union);
ANN void union_pop(const Env, const Stmt_Union, const m_uint);
ANN m_bool check_stmt(const Env env, const Stmt stmt);
#endif
