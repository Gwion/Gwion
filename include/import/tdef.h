#ifndef __IMPORT_TYPEDEF
#define __IMPORT_TYPEDEF

ANN m_int gwi_typedef_ini(const Gwi gwi, const restrict m_str type,
                          const restrict m_str name);
ANN Type  gwi_typedef_end(const Gwi gwi, const ae_flag flag);
ANN void  ck_clean_tdef(MemPool, ImportCK *);

ANN Type mk_primitive(const Env env, const m_str name, const m_uint size);
//ANN Type gwi_primitive(const Gwi gwi, const m_str name, const m_uint size, const ae_flag flag);
ANN m_bool gwi_primitive(const Gwi gwi, const m_str name, const m_uint size, const ae_flag flag);
#endif
