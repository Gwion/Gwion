typedef void (*f_instr)(VM* vm, VM_Shred shred, Instr instr);

const m_str op2str(Operator op);
m_bool add_binary_op(Env env, Operator op, Type lhs, Type rhs, Type ret, f_instr f, m_bool global);
Type get_return_type(Env env, Operator op, Type rhs, Type lhs);
m_int name2op(m_str);
m_bool operator_set_func(Env env, Func f, Type lhs, Type rhs);
Map new_Operator_Map();
void free_Operator_Map();
