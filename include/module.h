#ifndef __MODULE
#define __MODULE
#define GWMODNAME_NAME "gwmodname"
#define GWMODINI_NAME  "gwmodini"
#define GWMODEND_NAME  "gwmodend"
#define GWMODNAME_FUNC gwmodname
#define GWMODINI_FUNC  gwmodini
#define GWMODEND_FUNC  gwmodend
#define GWMODNAME(a) m_str GWMODNAME_FUNC() { return a; }
#define GWMODINI(a)  ANN void* GWMODINI_FUNC(const Gwion gwion, const Vector args)
#define GWMODEND(a)  ANN void  GWMODEND_FUNC(const Gwion gwion, void* self)

//typedef void* (*f_gwmodname)(void);
typedef void* (*f_gwmodini)(const Gwion, const Vector);
typedef void* (*f_gwmodend)(const Gwion, void*);
void module_ini(const Gwion gwion, Vector v, Vector);
#endif
