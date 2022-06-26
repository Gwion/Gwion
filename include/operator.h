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
#define CHECK_NB(f)                                                            \
  do {                                                                         \
    if (f == env->gwion->type[et_error]) return GW_ERROR;                      \
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
typedef m_bool (*opem)(const Emitter, void *);

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
  loc_t  pos;
};

struct Implicit {
  Exp   e;
  Type  t;
  loc_t pos;
};

struct TemplateScan {
  const Type       t;
  const Type_Decl *td;
};

ANN void   op_cpy(const Env env, const struct Op_Import *opi);
ANN m_bool add_op(const Gwion gwion, const struct Op_Import *);
ANN void* op_get(const Env env, struct Op_Import *opi);
ANN Type   op_check(const Env, struct Op_Import *);
ANN m_bool op_emit(const Emitter, const struct Op_Import *);
ANN m_bool operator_set_func(const struct Op_Import *);
ANN void   free_op_map(Map map, struct Gwion_ *gwion);
ANN void   free_op_tmpl(Vector v, struct Gwion_ *gwion);

ANN void               operator_suspend(const Nspc, struct Op_Import *);
ANN static inline void operator_resume(struct Op_Import *opi) {
  assert(opi->ret);
  *(uintptr_t *)opi->ret = opi->data;
}

ANN static inline void set_decl_ref(const Exp e) {
  if (e->exp_type == ae_exp_decl)
    SET_FLAG(e->d.exp_decl.vd.value, late);
}

ANN void func_operator(const Func_Def fdef, struct Op_Import *opi);
#endif
