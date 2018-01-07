#define OP_ANY_TYPE (Type)1

typedef void (*f_instr)(VM*, VM_Shred, Instr);
typedef Type (*opck)(Env, void*);
typedef m_bool (*opem)(Emitter, void*);

struct Op_Import {
  Operator op;
  Type lhs, rhs, ret;
  opck ck;
  opem em;
  uintptr_t data;
  m_bool global;
};

m_bool env_add_op(Env env, struct Op_Import* opi);
Type   op_check(Env env, struct Op_Import* opi);
m_bool op_emit(Emitter emit, struct Op_Import* opi);
const m_str op2str(Operator op);
m_int name2op(m_str);
m_bool operator_set_func(Env env, Func f, Type lhs, Type rhs);
void free_op_map();
