#ifndef __IMPORT_OPER
#define __IMPORT_OPER

ANN2(1)
m_int gwi_oper_ini(const Gwi gwi, const m_str l, const m_str r, const m_str t);
ANN m_int gwi_oper_add(const Gwi gwi, const opck);
ANN m_int gwi_oper_emi(const Gwi gwi, const opem);
ANN2(1) m_int gwi_oper_end(const Gwi gwi, const m_str op, const f_instr f);
ANN m_int gwi_oper_cond(const Gwi, const m_str, const f_instr, const f_instr);
ANN void  gwi_oper_eff(const Gwi gwi, const m_str effect);
#define _EQUALITY_OPER(sz, sign)                                               \
  POP_REG(shred, sz * 2 - SZ_INT);                                             \
  *(m_uint *)REG(-SZ_INT) =                                                    \
      sign memcmp(shred->reg + SZ_INT, shred->reg + SZ_INT + sz, sz);

#define EQUALITY_OPER(name, sz)                                                \
  static INSTR(name##_eq) { _EQUALITY_OPER(sz, !) }                            \
  static INSTR(name##_ne) { _EQUALITY_OPER(sz, !!) }

#endif
