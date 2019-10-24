#ifndef __IMPORT_OPER
#define __IMPORT_OPER

ANN2(1) m_int gwi_oper_ini(const Gwi gwi, const m_str l, const m_str r, const m_str t);
ANN m_int gwi_oper_add(const Gwi gwi, const opck);
ANN m_int gwi_oper_emi(const Gwi gwi, const opem);
ANN2(1) m_int gwi_oper_end(const Gwi gwi, const m_str op, const f_instr f);
ANN m_int gwi_oper_cond(const Gwi, const m_str,  const f_instr, const f_instr);

#endif
