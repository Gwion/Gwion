#ifndef __PLUG
#define __PLUG

struct DriverData_;
struct Gwi_;
typedef m_bool (*gwplugin_t) (struct Gwi_*);
typedef void*  (*gwmodini_t) (struct Gwion_ const* gwion, const Vector args);
typedef void*  (*gwmodend_t) (struct Gwion_ const* gwion, void *self);
typedef void   (*gwdriver_t) (struct DriverData_ *);
typedef m_str* (*gwdepend_t) (void);

typedef struct Plug_ {
  void *dl;
  void *self;
  Nspc nspc;
  gwplugin_t plugin;
  gwdriver_t driver;
  gwmodini_t modini;
  gwmodend_t modend;
  gwdepend_t depend;
} * Plug;

ANN static inline Plug new_plug(MemPool p) {
  return (Plug)mp_calloc(p, Plug);
}

ANN m_bool plug_ini(const struct Gwion_ *, const Vector);
ANN m_bool driver_ini(const struct Gwion_ *);
ANN m_bool plug_run(const struct Gwion_ *, const Map);
ANN void   free_plug(const Gwion);
ANN void * get_module(const struct Gwion_ *, const m_str);
ANN m_bool   set_module(const struct Gwion_ *gwion, const m_str name,
                      void *const ptr);
ANN m_bool plugin_ini(struct Gwion_ *gwion, const m_str iname, const loc_t);


#define GWIMPORT_NAME(a) gwimport_##a
#define GWMODINI_NAME(a) gwmodini_##a
#define GWMODEND_NAME(a) gwmodend_##a
#define GWDRIVER_NAME(a) gwdriver_##a
#define GWDEPEND_NAME(a) gwdepend_##a

#define GWMODINI(a)                                                             \
  ANN2(1)                                                                        \
  void *gwmodini_##a(struct Gwion_ *const gwion NUSED, const Vector args NUSED)
#define GWMODEND(a)                                                              \
  ANN void gwmodend_##a(struct Gwion_ *const gwion NUSED, void *self NUSED)
#define GWDRIVER(a) ANN void gwdriver_##a(DriverData *d)
#define GWDEPEND(a) ANN m_str const *gwdepend_##a(void)

#endif
