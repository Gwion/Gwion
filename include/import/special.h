#ifndef __IMPORT_SPECIAL
#define __IMPORT_SPECIAL

#define FREEARG(a) ANN void a(Instr instr  NUSED, void *gwion NUSED)
typedef void (*f_freearg)(Instr, void*);
ANN void register_freearg(const Gwi, const f_instr, const f_freearg);
ANN void gwi_reserve(const Gwi, const m_str);
typedef struct SpecialId_* SpecialId;
ANN void gwi_specialid(const Gwi gwi, const m_str id, const SpecialId);
ANN void gwi_set_loc(const Gwi, const m_str, const uint);

ANN Type_Decl* str2decl(const Env env, const m_str s, m_uint *depth, const loc_t pos);

#endif
