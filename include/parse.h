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
nspc_push_value(env->gwion->mp, env->curr); \
const m_bool ret = exp;     \
nspc_pop_value(env->gwion->mp, env->curr);  \
--env->scope->depth;        \
return ret;

#define SET_ACCESS(a,b)       \
if(GET_FLAG(a, private))      \
  SET_FLAG(b, private);       \
else if(GET_FLAG(a, protect)) \
  SET_FLAG(b, protect);

#define HANDLE_SECTION_FUNC(prefix, type, Arg)                                           \
DECL_SECTION_FUNC(prefix, type, Arg)                                                     \
ANN static inline type prefix##_section(const Arg a, /* const */ Section* section) {     \
  void* d = &section->d.stmt_list;                                                       \
  return prefix##_section_func[section->section_type](a, *(void**)d);                    \
}

#define HANDLE_EXP_FUNC(prefix, type, Arg)                  \
DECL_EXP_FUNC(prefix, type, Arg)                            \
ANN type prefix##_exp(const Arg arg, Exp exp) {             \
  do CHECK_BB(prefix##_exp_func[exp->exp_type](arg, &exp->d)) \
  while((exp = exp->next));                                 \
  return GW_OK;                                             \
}
ANN m_bool scan1_exp(const Env, Exp);
ANN m_bool scan2_exp(const Env, Exp);

#define describe_stmt_func(prefix, name, type, prolog, exp) \
ANN static m_bool prefix##_stmt_##name(const Env env, const type stmt) { \
  RET_NSPC(exp) \
}

ANN m_uint union_push(const Env, const Union_Def);
ANN void union_pop(const Env, const Union_Def, const m_uint);
ANN void union_flag(const Union_Def, const ae_flag);
ANN m_bool check_stmt(const Env env, const Stmt stmt);

typedef m_bool (*_exp_func)(const void*, const void*);
ANN m_bool scanx_body(const Env env, const Class_Def cdef, const _exp_func f, void* data);
static inline ANN m_bool env_body(const Env env, const Class_Def cdef, const _exp_func f) {
  return scanx_body(env, cdef, f, env);
}
#define env_body(a,b,c) env_body(a,b,(_exp_func)c)

ANN m_bool scanx_parent(const Type t, const _exp_func f, void *d);
#define scanx_parent(a,b,c) scanx_parent(a, (_exp_func)b, c)


ANN m_bool scanx_cdef(const Env, void *,const Class_Def,
  const _exp_func f_cdef, const _exp_func f_union);

#define xxx_cdef(prefix)                                                  \
static inline m_bool prefix##_cdef(const Env env, const Class_Def cdef) { \
  return scanx_cdef(env, env, cdef,                                       \
      (_exp_func)prefix##_class_def, (_exp_func)prefix##_union_def);      \
}

#define xxx_cdef_flag(prefix)                                             \
static inline m_bool prefix##_cdef(const Env env, const Class_Def cdef) { \
  SET_FLAG(cdef, prefix);                                                 \
  return scanx_cdef(env, env, cdef,                                       \
      (_exp_func)prefix##_class_def, (_exp_func)prefix##_union_def);      \
}
xxx_cdef_flag(scan0)
xxx_cdef_flag(scan1)
xxx_cdef_flag(scan2)
xxx_cdef_flag(check)
xxx_cdef(traverse)

ANN m_bool scanx_fdef(const Env, void *, const Func_Def, const _exp_func);

__attribute__((returns_nonnull))
ANN Type get_type(const Type t);
ANN m_bool check_subscripts(const Env, const Array_Sub, const m_bool is_decl);
ANN m_bool check_implicit(const Env env, const Exp e, const Type t);
#endif
