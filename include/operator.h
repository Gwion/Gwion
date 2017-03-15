#include "vm.h"

typedef void (*f_instr)(VM* vm, VM_Shred shred, Instr instr);

m_str op2str(Operator op);
m_bool add_binary_op(Env env, Operator op, Type lhs, Type rhs, Type ret, f_instr f, m_bool global, m_bool is_new);
Type get_return_type(Env env, Operator op, Type rhs, Type lhs);
//Operator name2op(m_str);
m_int name2op(m_str);
//m_bool operator_set_func(Env env, Func f);
m_bool operator_set_func(Env env, Func f, Type lhs, Type rhs);
//m_bool operator_set_func(Env env, Func f, Type lhs, Expression rhs);
Map new_Operator_Map();
void free_Operator_Map();
