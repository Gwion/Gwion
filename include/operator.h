typedef struct {
  Type lhs, rhs, ret;
  f_instr instr;
  Func func;
//  m_str doc;
} M_Operator;

m_str op2str(Operator op);
m_bool add_binary_op(Env env, Operator op, Type lhs, Type rhs, Type ret, f_instr f, m_bool global);
Type get_return_type(Env env, Operator op, Type rhs, Type lhs);
Operator name2op(m_str);
m_bool operator_set_doc(m_str str);
m_bool operator_set_func(Env env, Func f, Type lhs, Type rhs);
Map new_Operator_Map();
void free_Operator_Map();
