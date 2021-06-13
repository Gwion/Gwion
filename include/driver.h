#ifndef __DRIVER
#define __DRIVER
#include "soundinfo.h"
typedef m_bool (*f_drvini)(struct VM_ *, struct BBQ_ *);
typedef void (*f_drvrun)(struct VM_ *, struct BBQ_ *);
typedef void (*f_drvdel)(struct VM_ *, struct BBQ_ *);

typedef struct DriverData_ {
  void *   data;
  f_drvini ini;
  f_drvrun run;
  f_drvdel del;
} DriverData;

typedef void (*f_bbqset)(struct DriverData_ *);
typedef void (*f_bbqrun)(const struct VM_ *);

typedef struct BBQ_ {
  volatile uint64_t   pos;
  m_float *           in;
  m_float *           out;
  struct SoundInfo_ * si;
  f_bbqset            func;
  f_bbqrun            run;
  struct DriverData_ *driver;
  volatile bool       is_running;
} Driver;

#define DRVINI(a) ANN m_bool a(struct VM_ *vm NUSED, Driver *di NUSED)
#define DRVRUN(a) ANN void a(struct VM_ *vm NUSED, Driver *di NUSED)
#define DRVDEL(a) ANN void a(struct VM_ *vm NUSED, Driver *di NUSED)

ANN void dummy_driver(DriverData *);
ANN void driver_alloc(Driver *);
ANN Driver *new_driver(MemPool);
ANN void    free_driver(Driver *, struct VM_ *);
#endif
