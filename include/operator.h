#define OP_ANY_TYPE (Type)1

typedef void (*f_instr)(const VM*, const VM_Shred, const Instr);
typedef Type (*opck)(Env, void*);
typedef m_bool (*opem)(Emitter, void*);

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
const m_bool env_add_op(Env env, const struct Op_Import* opi);
const Type   op_check(Env env, struct Op_Import* opi);
const m_bool op_emit(Emitter emit, const struct Op_Import* opi);
const m_str op2str(const Operator op);
const m_int name2op(const m_str);
const m_bool operator_set_func(const Env env, const struct Op_Import* opi);
void free_op_map(Map map);
const m_str get_type_name(const m_str str, const m_uint index);
