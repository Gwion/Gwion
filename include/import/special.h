#ifndef __IMPORT_SPECIAL
#define __IMPORT_SPECIAL

#include "pass.h"

#define FREEARG(a) ANN void a(Instr instr  NUSED, void *gwion NUSED)
typedef void (*f_freearg)(Instr, void*);
ANN void gwi_register_freearg(const Gwi, const f_instr, const f_freearg);
ANN void gwi_register_pass(const Gwi, const m_str, const compilation_pass[2]);
ANN void gwi_reserve(const Gwi, const m_str);
typedef struct SpecialId_* SpecialId;
ANN void gwi_specialid(const Gwi gwi, const m_str id, const SpecialId);
ANN void gwi_set_loc(const Gwi, const m_str, const uint);

// TODO: move me
ANN Type_Decl*    str2decl(const Gwi, const m_str);
ANN Var_Decl      str2var(const Gwion, const m_str, const loc_t);
ANN Var_Decl_List str2varlist(const Gwion, const m_str, const loc_t);
ANN Type str2type(const Gwi, const m_str);

#define gwi_str2var(gwi, path) str2var(gwi->gwion, path, gwi->loc);
#define gwi_str2varlist(gwi, path) str2varlist(gwi->gwion, path, gwi->loc);
#endif
