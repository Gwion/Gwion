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

typedef m_bool (*_exp_func)(const void*, const void*);
static inline m_bool dummy_func(const void*a NUSED,
  const void*b NUSED) { return 1; }

#define DECL_PRIM_FUNC(prefix)                                              \
static const _exp_func prim_func[] = {                                      \
  (_exp_func)prefix##__primary_id,      (_exp_func)prefix##__primary_num,   \
  (_exp_func)prefix##__primary_float,   (_exp_func)prefix##__primary_str,   \
  (_exp_func)prefix##__primary_array,   (_exp_func)prefix##__primary_hack,  \
  (_exp_func)prefix##__primary_complex, (_exp_func)prefix##__primary_polar, \
  (_exp_func)prefix##__primary_vec,     (_exp_func)prefix##__primary_tuple, \
  (_exp_func)prefix##__primary_unpack,  (_exp_func)prefix##__primary_char,  \
  (_exp_func)prefix##__primary_nil                                          \
}

#define DECL_EXP_FUNC(prefix)                                                                     \
static const _exp_func exp_func[] = {                                                             \
  (_exp_func)prefix##_exp_decl,    (_exp_func)prefix##_exp_binary, (_exp_func)prefix##_exp_unary, \
  (_exp_func)prefix##_exp_primary, (_exp_func)prefix##_exp_cast,   (_exp_func)prefix##_exp_post,  \
  (_exp_func)prefix##_exp_call,    (_exp_func)prefix##_exp_array,  (_exp_func)prefix##_exp_if,    \
  (_exp_func)prefix##_exp_dot,     (_exp_func)prefix##_exp_lambda, (_exp_func)prefix##_exp_typeof \
};

#define DECL_STMT_FUNC(prefix)                                          \
static const _exp_func stmt_func[] = {                                  \
  (_exp_func)prefix##__stmt_exp,    (_exp_func)prefix##__stmt_while,    \
  (_exp_func)prefix##__stmt_until,  (_exp_func)prefix##__stmt_for,      \
  (_exp_func)prefix##__stmt_auto,   (_exp_func)prefix##__stmt_loop,     \
  (_exp_func)prefix##__stmt_if,     (_exp_func)prefix##__stmt_code,     \
  (_exp_func)prefix##__stmt_break,  (_exp_func)prefix##__stmt_continue, \
  (_exp_func)prefix##__stmt_return, (_exp_func)prefix##__stmt_match,    \
  (_exp_func)prefix##__stmt_jump,                                       \
};

#define DECL_SECTION_FUNC(prefix)                                                                 \
static const _exp_func section_func[] = {                                                         \
  (_exp_func)prefix##_stmt_list, (_exp_func)prefix##_func_def, (_exp_func)prefix##_class_def,     \
  (_exp_func)prefix##_enum_def,  (_exp_func)prefix##_union_def,                                   \
  (_exp_func)prefix##_fptr_def, (_exp_func)prefix##_type_def                                      \
};                                                                                                \
ANN static inline m_bool prefix##_section(const void* a, /* const */ Section* section) {                                                  \
  void* d = &section->d.stmt_list;                           \
  return section_func[section->section_type](a, *(void**)d);                            \
}

#define HANDLE_EXP_FUNC(prefix, type, ret)                                                        \
DECL_EXP_FUNC(prefix)                                                                             \
ANN type prefix##_exp(const Env env, Exp exp) {                       \
  do CHECK_BB(exp_func[exp->exp_type](env, &exp->d))                                              \
  while((exp = exp->next));                                                                       \
  return ret;                                                                                     \
}
ANN m_bool scan1_exp(const Env, Exp);
ANN m_bool scan2_exp(const Env, Exp);

#define describe_stmt_func(prefix, name, type, prolog, exp) \
ANN static m_bool prefix##_stmt_##name(const Env env, const type stmt) { \
  RET_NSPC(exp) \
}

ANN m_uint union_push(const Env, const Union_Def);
ANN void union_pop(const Env, const Union_Def, const m_uint);
ANN m_bool check_stmt(const Env env, const Stmt stmt);

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
      (_exp_func)prefix##_class_def, (_exp_func)prefix##_union_def);     \
}
xxx_cdef(scan1)
xxx_cdef(scan2)
xxx_cdef(check)
xxx_cdef(traverse)

ANN m_bool scanx_fdef(const Env, void *, const Func_Def, const _exp_func);

__attribute__((returns_nonnull))
ANN Type get_type(const Type t);
ANN m_bool check_subscripts(const Env, const Array_Sub);
#endif
