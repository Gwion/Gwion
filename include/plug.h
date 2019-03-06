#ifndef __PLUG
#define __PLUG

enum plug_t {
  GWPLUG_DL,
  GWPLUG_IMPORT,
  GWPLUG_MODULE,
  GWPLUG_LAST
};

typedef struct {
  struct Vector_ vec[GWPLUG_LAST];
  struct Map_ drv;
} PlugInfo;
void plug_discover(PlugInfo*, Vector);
void plug_end(const Gwion gwion);

#define GWIMPORT_NAME    import
#define GWMODSTR_NAME  gwmodstr
#define GWMODINI_NAME  gwmodini
#define GWMODEND_NAME  gwmodend
#define GWDRIVER_NAME  gwmodend
#define GWMODSTR(a) m_str GWMODSTR_NAME() { return #a; }
#define GWMODINI(a)  ANN void* GWMODINI_NAME(const Gwion gwion, const Vector args)
#define GWMODEND(a)  ANN void  GWMODEND_NAME(const Gwion gwion, void* self)
#define GWDRIVER(a)  ANN void  GWDRIVER_NAME(Driver* d)

ANN void plug_ini(const Gwion gwion, const Vector);
ANN Vector split_args(const m_str str);
#endif
