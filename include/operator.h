typedef void (*f_instr)(VM* vm, VM_Shred shred, Instr instr);

const m_str op2str(Operator op);
Type get_return_type(Env env, Operator op, Type rhs, Type lhs);
m_int name2op(m_str);
m_bool operator_set_func(Env env, Func f, Type lhs, Type rhs);
void free_op_map();
struct Op_Import {
  Operator op;
  Type lhs, rhs, ret;
  f_instr f;
  m_bool global;
};
m_bool env_add_op(Env env, struct Op_Import* opi);
Instr get_instr(Emitter emit, struct Op_Import* opi);
