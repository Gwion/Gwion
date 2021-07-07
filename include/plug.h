#ifndef __PLUG
#define __PLUG

ANN m_bool plug_ini(const struct Gwion_ *, const Vector);
ANN m_bool driver_ini(const struct Gwion_ *);
ANN void   plug_run(const struct Gwion_ *, const Map);
ANN void   free_plug(const struct Gwion_ *);
ANN void * get_module(const struct Gwion_ *, const m_str);
ANN void   set_module(const struct Gwion_ *gwion, const m_str name,
                      void *const ptr);
ANN m_bool plugin_ini(struct Gwion_ *gwion, const m_str iname);

#define GWIMPORT_NAME import
#define GWMODINI_NAME gwmodini
#define GWMODEND_NAME gwmodend
#define GWDRIVER_NAME gwdriver
#define GWDEPEND_NAME gwdepend
#define GWMODINI(a)                                                            \
  ANN2(1)                                                                      \
  void *GWMODINI_NAME(struct Gwion_ *const gwion NUSED, const Vector args NUSED)
#define GWMODEND(a)                                                            \
  ANN void GWMODEND_NAME(struct Gwion_ *const gwion NUSED, void *self NUSED)
#define GWDRIVER(a) ANN void GWDRIVER_NAME(DriverData *d)
#define GWDEPEND    ANN m_str const *GWDEPEND_NAME(void)

#endif
