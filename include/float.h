
m_bool emit_Float_Assign(Instr instr);
m_bool emit_Add_Float_Assign(Instr instr);
m_bool emit_Sub_Float_Assign(Instr instr);
m_bool emit_Mul_Float_Assign(Instr instr);
m_bool emit_Div_Float_Assign(Instr instr);
m_bool emit_Mod_Float_Assign(Instr instr);
//m_bool import_float(Env env)

void Float_Assign( VM * vm, VM_Shred shred, Instr instr );