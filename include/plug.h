#ifndef __PLUG
#define __PLUG

enum plug_t {
  GWPLUG_DL,
  GWPLUG_IMPORT,
  GWPLUG_MODULE,
  GWPLUG_LAST
};

typedef struct PlugInfo_ {
  struct Vector_ vec[GWPLUG_LAST];
  struct Map_ drv;
} PlugInfo;

ANN PlugInfo* new_plug(Vector);
ANN void plug_run(const Gwion gwion, const Vector);
ANN void free_plug(const Gwion gwion);

#define GWIMPORT_NAME    import
#define GWMODSTR_NAME  gwmodstr
#define GWMODINI_NAME  gwmodini
#define GWMODEND_NAME  gwmodend
#define GWDRIVER_NAME  gwmodend
#define GWMODSTR(a) m_str GWMODSTR_NAME() { return #a; }
#define GWMODINI(a)  ANN2(1) void* GWMODINI_NAME(const Gwion gwion, const Vector args)
#define GWMODEND(a)  ANN void  GWMODEND_NAME(const Gwion gwion, void* self)
#define GWDRIVER(a)  ANN void  GWDRIVER_NAME(DriverData* d)

ANN Vector split_args(const m_str str);
#endif
