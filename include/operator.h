#define OP_ANY_TYPE (Type)1
typedef Type (*opck)(const Env, void*);
typedef m_bool (*opem)(const Emitter, void*);

struct Op_Import {
  Operator op;
  Type lhs, rhs, ret;
  opck ck;
  opem em;
  uintptr_t data;
};

struct Implicit {
  void* e;
  Type  t;
};
ANN m_bool add_op(const Nspc , const struct Op_Import*);
ANN Type   op_check(const Env, struct Op_Import*);
ANN m_bool op_emit(const Emitter, const struct Op_Import*);
ANN m_int name2op(const m_str);
ANN m_bool operator_set_func(const struct Op_Import*);
ANN void free_op_map(Map map);
ANN m_str get_type_name(const m_str, const m_uint);
ANN m_bool env_add_op(const Env, const struct Op_Import*);
