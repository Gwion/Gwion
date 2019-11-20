#ifndef __PLUG
#define __PLUG

enum plug_t {
  GWPLUG_DL,
  GWPLUG_IMPORT,
  GWPLUG_MODULE,
  GWPLUG_LAST
};

typedef struct PlugInfo_ {
  struct Map_ drv;
  struct Vector_ vec[GWPLUG_LAST];
} PlugInfo;

ANN PlugInfo* new_pluginfo(MemPool, Vector);
ANN void plug_run(const struct Gwion_*, const Vector);
ANN void free_plug(const struct Gwion_*);
ANN void* get_module(const struct Gwion_*, const m_str);
#define GWIMPORT_NAME    import
#define GWMODSTR_NAME  gwmodstr
#define GWMODINI_NAME  gwmodini
#define GWMODEND_NAME  gwmodend
#define GWDRIVER_NAME  gwmodend
#define GWMODSTR(a) m_str GWMODSTR_NAME() { return #a; }
#define GWMODINI(a)  ANN2(1) void* GWMODINI_NAME(const struct Gwion_ *gwion NUSED, const Vector args NUSED)
#define GWMODEND(a)  ANN void  GWMODEND_NAME(const struct Gwion_ *gwion NUSED, void* self NUSED)
#define GWDRIVER(a)  ANN void  GWDRIVER_NAME(DriverData* d)

ANN Vector split_args(MemPool, const m_str str);
#endif
