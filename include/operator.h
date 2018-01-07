typedef void (*f_instr)(VM*, VM_Shred, Instr);
typedef Type (*opck)(Env, void*);
typedef m_bool (*opem)(Emitter, void*);
#define OP_ANY_TYPE (Type)1
const m_str op2str(Operator op);
m_int name2op(m_str);
m_bool operator_set_func(Env env, Func f, Type lhs, Type rhs);
void free_op_map();
struct Op_Import {
  Operator op;
  Type lhs, rhs, ret;
  opck ck;
  opem em;
  uintptr_t data;
  m_bool global;
};
m_bool env_add_op(Env env, struct Op_Import* opi);
Instr get_instr(Emitter emit, struct Op_Import* opi);
Type get_return_type(Env env, struct Op_Import* opi);
