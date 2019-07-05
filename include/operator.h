#ifndef __OPERATOR
#define __OPERATOR
#define OP_ANY_TYPE (Type)1

#define ERR_N(a, b, ...) { env_err(env, (a), (b), ## __VA_ARGS__); return t_null; }

typedef Type (*opck)(const Env, void*, m_bool*);
typedef m_bool (*opem)(const Emitter, void*);

struct Op_Import {
  Type lhs, rhs, ret;
  opck ck;
  opem em;
  uintptr_t data;
  loc_t pos;
  Operator op;
};

struct Implicit {
  void* e;
  Type  t;
};
ANN m_bool add_op(const Gwion gwion, const struct Op_Import*);
ANN Type   op_check(const Env, struct Op_Import*);
ANN m_bool op_emit(const Emitter, const struct Op_Import*);
ANN m_bool operator_set_func(const struct Op_Import*);
ANN void free_op_map(Map map, struct Gwion_* gwion);
#endif
