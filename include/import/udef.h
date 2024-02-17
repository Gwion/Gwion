#ifndef __IMPORT_UNION
#define __IMPORT_UNION

ANN2(1) bool gwi_union_ini(const Gwi gwi, const m_str name);
ANN bool gwi_union_add(const Gwi gwi, const __restrict__ m_str type,
                        const __restrict__ m_str name);
ANN bool  gwi_union_end(const Gwi gwi, const ae_flag flag);
ANN void  ck_clean_udef(MemPool, ImportCK *);

#endif
