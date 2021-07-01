#include <time.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "driver.h"

ANN Driver *new_driver(MemPool p) {
  Driver *di     = (Driver *)mp_calloc(p, BBQ);
  di->func       = dummy_driver;
  di->driver     = (DriverData *)mp_calloc(p, DriverData);
  di->is_running = 1;
  return di;
}

ANN void free_driver(Driver *d, VM *vm) {
  if (d->in) xfree(d->in);
  if (d->out) xfree(d->out);
  mp_free(vm->gwion->mp, SoundInfo, d->si);
  if (d->driver->del) d->driver->del(vm, d);
  mp_free(vm->gwion->mp, DriverData, d->driver);
  mp_free(vm->gwion->mp, BBQ, d);
}

ANN void driver_alloc(Driver *d) {
  d->out = (m_float *)xcalloc(d->si->out, SZ_FLOAT);
  d->in  = (m_float *)xcalloc(d->si->in, SZ_FLOAT);
}

static DRVRUN(dummy_run) {
  while (di->is_running) {
    di->run(vm);
    next_bbq_pos(vm);
  }
}

static DRVINI(dummy_ini) { return GW_OK; }

void dummy_driver(DriverData *dd) {
  dd->ini = dummy_ini;
  dd->run = dummy_run;
}
