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

ANN void bbq_alloc(struct BBQ_ *bbq) {
  bbq->out = (m_float*)xcalloc(bbq->si->out, SZ_FLOAT);
  bbq->in  = (m_float*)xcalloc(bbq->si->in, SZ_FLOAT);
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

void dummy_driver(DriverData* d) {
  d->ini = dummy_ini;
  d->run = dummy_run;
}
