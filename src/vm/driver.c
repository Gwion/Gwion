#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "driver.h"

ANN Driver* new_driver(void) {
  Driver* di = (Driver*)mp_alloc(BBQ);
  di->func = dummy_driver;
  di->run = vm_run;
  di->driver = (DriverData*)mp_alloc(DriverData);
  di->is_running = 1;
  return di;
}

ANN void free_driver(Driver *d, VM *vm) {
  if(d->in)
    xfree(d->in);
  if(d->out)
    xfree(d->out);
  mp_free(SoundInfo, d->si);
  if(d->driver->del)
    d->driver->del(vm, d);
  mp_free(DriverData, d->driver);
  mp_free(BBQ, d);
}

ANN void driver_alloc(Driver *d) {
  d->out = (m_float*)xcalloc(d->si->out, SZ_FLOAT);
  d->in  = (m_float*)xcalloc(d->si->in, SZ_FLOAT);
}

static DRVRUN(dummy_run) {
  while(di->is_running) {
    di->run(vm);
    ++di->pos;
  }
}

static DRVINI(dummy_ini) {
  return GW_OK;
}

void dummy_driver(DriverData* dd) {
  dd->ini = dummy_ini;
  dd->run = dummy_run;
}
