#define RET_NSPC(exp)       \
++env->class_scope;         \
nspc_push_value(env->curr); \
const m_bool ret = exp;     \
nspc_pop_value(env->curr);  \
--env->class_scope;         \
return ret;

//typedef m_bool (*_exp_func)(const void*, const union exp_data *);

#define DECL_EXP_FUNC(prefix)                                                                     \
static const _exp_func exp_func[] = {                                                             \
  (_exp_func)prefix##_exp_decl,    (_exp_func)prefix##_exp_binary, (_exp_func)prefix##_exp_unary, \
  (_exp_func)prefix##_exp_primary, (_exp_func)prefix##_exp_cast,   (_exp_func)prefix##_exp_post,  \
  (_exp_func)prefix##_exp_call,    (_exp_func)prefix##_exp_array,  (_exp_func)prefix##_exp_if,    \
  (_exp_func)prefix##_exp_dot,     (_exp_func)prefix##_exp_dur                                    \
};

#define HANDLE_EXP_FUNC(prefix, type, ret)                                                        \
DECL_EXP_FUNC(prefix)                                                                             \
ANN static inline type prefix##_exp(const Env env, Exp exp) { GWDEBUG_EXE                         \
  do CHECK_BB(exp_func[exp->exp_type](env, &exp->d))                                              \
  while((exp = exp->next));                                                                       \
  return ret;                                                                                     \
}



#define describe_stmt_func(prefix, name, type, exp) \
ANN static m_bool prefix##_stmt_##name(const Env env, const type stmt) { GWDEBUG_EXE \
  RET_NSPC(exp) \
}
