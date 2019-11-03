#ifndef __IMPORT_CLASS
#define __IMPORT_CLASS
ANN2(1,2) Type gwi_class_ini(const Gwi gwi, const m_str, const m_str parent);
ANN2(1) void gwi_class_xtor(const Gwi gwi, const f_xtor ctor, const f_xtor dtor);
ANN m_int gwi_class_end(const Gwi gwi);
#endif
