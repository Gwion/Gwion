#ifndef __IMPORT_TYPE
#define __IMPORT_TYPE

ANN2(1, 2)
ANEW Type gwi_mk_type(const Gwi, const m_str, const m_uint, const m_str);
ANN void  gwi_add_type(const Gwi gwi, Type type);
ANN void  gwi_set_global_type(const Gwi gwi, const Type type,
                              const type_enum te);

#endif
