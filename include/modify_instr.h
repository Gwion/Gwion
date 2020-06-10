#ifndef __MODIFY_INSTR
#define __MODIFY_INSTR
typedef void (*instr_modifier)(m_bit *bytecode, m_bit *reg);

ANN void emit_mod_recurs(const Emitter);
ANN void emit_mod_func(const Emitter, const Func);
ANN void emit_mod_ctor(const Emitter, const Type);
#endif
