#ifndef __OPERATOR
#define __OPERATOR
#define OP_ANY_TYPE (Type)1
typedef Type (*opck)(const Env, void*);
typedef m_bool (*opem)(const Emitter, void*);

struct Op_Import {
  Type lhs, rhs, ret;
  opck ck;
  opem em;
  uintptr_t data;
  Operator op;
  m_bool mut;
};

struct Implicit {
  void* e;
  Type  t;
};
ANN m_bool add_op(const Gwion gwion, const Nspc , const struct Op_Import*);
ANN Type   op_check(const Env, struct Op_Import*);
ANN m_bool op_emit(const Emitter, const struct Op_Import*);
ANN m_bool operator_set_func(const struct Op_Import*);
ANN void free_op_map(Map map, struct Gwion_* gwion);
ANN m_bool env_add_op(const Env, const struct Op_Import*);
#endif
