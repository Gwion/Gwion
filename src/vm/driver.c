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
#include "sound.h"

ANN void bbq_alloc(DriverInfo* di) {
  struct BBQ_* bbq   = (struct BBQ_*)di;
  bbq->out = (m_float*)xcalloc(di->si->out, SZ_FLOAT);
  bbq->in  = (m_float*)xcalloc(di->si->in, SZ_FLOAT);
}

static void dummy_run(VM* vm, DriverInfo* di) {
  while(di->is_running) {
    di->run(vm);
    ++di->pos;
  }
}

static m_bool dummy_ini(VM* vm __attribute__((unused)), DriverInfo* di __attribute__((unused))) {
  return GW_OK;
}

void dummy_driver(Driver* d) {
  d->ini = dummy_ini;
  d->run = dummy_run;
}
