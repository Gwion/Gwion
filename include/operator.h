#ifndef __OPERATOR
#define __OPERATOR
#define OP_ANY_TYPE (Type)1

#define ERR_N(a, b, ...) { env_err(env, (a), (b), ## __VA_ARGS__); return env->gwion->type[et_error]; }
#define DECL_ON(decl, f, exp) decl f exp; { if(!f)    return env->gwion->type[et_error]; }
#define DECL_BN(decl, f, exp) decl f exp; { if(f < 0)    return env->gwion->type[et_error]; }
#define DECL_NN(decl, f, exp) decl f exp; { if(f == env->gwion->type[et_error]) return env->gwion->type[et_error]; }
#define CHECK_ON(f) { if(!f)    return env->gwion->type[et_error]; }
#define CHECK_NB(f) { if(f == env->gwion->type[et_error]) return GW_ERROR; }
#define CHECK_NO(f) { if(f == env->gwion->type[et_error]) return NULL; }
#define CHECK_BN(f) { if(f < 0) return env->gwion->type[et_error]; }
#define CHECK_NN(f) { if(f == env->gwion->type[et_error]) return env->gwion->type[et_error]; }

typedef Type   (*opck)(const Env, void*, m_bool*);
typedef m_bool (*opem)(const Emitter, void*);

struct Op_Func {
  opck ck;
  opem em;
};

enum op_type {
  op_implicit,
  op_exp,
  op_dot,
  op_array,
  op_binary,
  op_cast,
  op_postfix,
  op_unary,
  op_scan
};

struct Op_Import {
  Type lhs, rhs, ret;
  const struct Op_Func *func;
  uintptr_t data;
  loc_t pos;
  Symbol op;
  enum op_type op_type;
};

struct Implicit {
  Exp e;
  Type  t;
  loc_t pos;
};

struct TemplateScan {
  const Type  t;
  const Type_Decl  *td;
};

ANN m_bool add_op(const Gwion gwion, const struct Op_Import*);
ANN Type   op_check(const Env, struct Op_Import*);
ANN m_bool op_emit(const Emitter, const struct Op_Import*);
ANN m_bool operator_set_func(const struct Op_Import*);
ANN void free_op_map(Map map, struct Gwion_* gwion);


ANN void operator_suspend(const Nspc, struct Op_Import*);
ANN static inline void operator_resume(struct Op_Import *opi) {
  *(uintptr_t*)opi->ret = opi->data;
}

ANN static inline void set_decl_ref(const Exp e) {
  if(e->exp_type == ae_exp_decl) {
    SET_FLAG(e->d.exp_decl.td, late);
    SET_FLAG(e->d.exp_decl.list->self->value, late);
  }
}


ANN void func_operator(const Func_Def fdef, struct Op_Import *opi);
#endif
