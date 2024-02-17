#ifndef __IMPORT_ENUM
#define __IMPORT_ENUM

__attribute__ ((visibility ("default")))
ANN bool gwi_enum_ini(const Gwi gwi, const m_str type);
ANN bool gwi_enum_add(const Gwi gwi, const m_str name, const m_uint value);
ANN bool gwi_enum_end(const Gwi gwi);

ANN void ck_clean_edef(MemPool, ImportCK *);
#endif
