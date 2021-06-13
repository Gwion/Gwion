#ifndef __IMPORT_FUNC
#define __IMPORT_FUNC

ANN m_int gwi_func_ini(const Gwi gwi, const __restrict__ m_str type,
                       const __restrict__ m_str name);
ANN m_int gwi_func_arg(const Gwi gwi, const __restrict__ m_str t,
                       const __restrict__ m_str n);
ANN m_int gwi_func_end(const Gwi gwi, const f_xfun addr, const ae_flag flag);
ANN m_int gwi_fptr_ini(const Gwi gwi, const m_str __restrict__ type,
                       const __restrict__ m_str name);
ANN Type  gwi_fptr_end(const Gwi gwi, const ae_flag flag);
ANN m_int gwi_func_arg(const Gwi gwi, const restrict m_str t,
                       const restrict m_str n);
ANN void  ck_clean_fdef(MemPool, ImportCK *);
#endif
