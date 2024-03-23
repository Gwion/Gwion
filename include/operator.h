#ifndef __OPERATOR
#define __OPERATOR
#define OP_ANY_TYPE (Type)1

#define ERR_N(a, b, ...)                                                       \
  do {                                                                         \
    env_err(env, (a), (b), ##__VA_ARGS__);                                     \
    return env->gwion->type[et_error];                                         \
  } while (0)
#define DECL_ON(decl, f, exp)                                                  \
  decl f exp;                                                                  \
  do {                                                                         \
    if (!f) return env->gwion->type[et_error];                                 \
  } while (0)
#define DECL_BN(decl, f, exp)                                                  \
  decl f exp;                                                                  \
  do {                                                                         \
    if (f < 0) return env->gwion->type[et_error];                              \
  } while (0)
#define DECL_NN(decl, f, exp)                                                  \
  decl f exp;                                                                  \
  do {                                                                         \
    if (f == env->gwion->type[et_error]) return env->gwion->type[et_error];    \
  } while (0)
#define CHECK_ON(f)                                                            \
  do {                                                                         \
    if (!f) return env->gwion->type[et_error];                                 \
  } while (0)
#define CHECK_NO(f)                                                            \
  do {                                                                         \
    if (f == env->gwion->type[et_error]) return NULL;                          \
  } while (0)
#define CHECK_BN(f)                                                            \
  do {                                                                         \
    if (f < 0) return env->gwion->type[et_error];                              \
  } while (0)
#define CHECK_NN(f)                                                            \
  do {                                                                         \
    if (f == env->gwion->type[et_error]) return env->gwion->type[et_error];    \
  } while (0)

typedef Type (*opck)(const Env, void *);
typedef bool (*opem)(const Emitter, void *);

struct Op_Func {
  opck           ck;
  opem           em;
  struct Vector_ effect;
};

struct Op_Import {
  Type                  lhs;
  Type                  rhs;
  Type                  ret;
  const struct Op_Func *func;
  uintptr_t             data;
  // used to return nspc in op_check.
  // could be put in a union with `ret`
  Nspc   nspc;
  Symbol op;
  loc_t  loc;
};

struct Implicit {
  Exp*   e;
  Type  t;
  loc_t loc;
};

struct TemplateScan {
  const Type       t;
  const Type_Decl *td;
};

ANN void  op_cpy(const Env env, const struct Op_Import *opi);
ANN bool  add_op(const Gwion gwion, const struct Op_Import *);
ANN void* op_get(const Env env, struct Op_Import *opi);
ANN Type  op_check(const Env, struct Op_Import *);
ANN bool  op_emit(const Emitter, const struct Op_Import *);
ANN bool  operator_set_func(const struct Op_Import *);
ANN void  free_op_map(Map map, struct Gwion_ *gwion);
ANN void  free_op_tmpl(Vector v, struct Gwion_ *gwion);

ANN void               operator_suspend(const Nspc, struct Op_Import *);
ANN static inline void operator_resume(struct Op_Import *opi) {
  assert(opi->ret);
  *(uintptr_t *)opi->ret = opi->data;
}

ANN void func_operator(const Func_Def fdef, struct Op_Import *opi);
ANN bool add_op_func_check(const Env env, const Type t, const struct Op_Func *opfunc, const m_uint idx);
#endif
