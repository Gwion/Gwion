#ifndef __IMPORT_FUNC
#define __IMPORT_FUNC

ANN bool gwi_func_ini(const Gwi gwi, const m_str type,
                       const m_str name);
ANN bool gwi_func_arg(const Gwi gwi, const m_str t,
                       const m_str n);
ANN bool gwi_func_end(const Gwi gwi, const f_xfun addr, const ae_flag flag);
ANN bool gwi_fptr_ini(const Gwi gwi, const m_str type,
                       const m_str name);
ANN Type  gwi_fptr_end(const Gwi gwi, const ae_flag flag);
ANN bool gwi_func_arg(const Gwi gwi, const m_str t,
                       const m_str n);
ANN void  ck_clean_fdef(MemPool, ImportCK *);
#endif
