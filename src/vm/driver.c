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

ANN struct BBQ_* new_bbq(DriverInfo* di) {
  struct BBQ_* bbq   = (struct BBQ_*)xcalloc(1, sizeof(struct BBQ_));
  bbq->out = (m_float*)xcalloc(di->out, SZ_FLOAT);
  bbq->in  = (m_float*)xcalloc(di->in, SZ_FLOAT);
  bbq->n_in = (uint8_t)di->in;
  bbq->sr = di->sr;
  bbq->n_out = (uint8_t)di->out;
  return bbq;
}

static void dummy_run(VM* vm, DriverInfo* di) {
  while(vm->is_running) {
    di->run(vm);
    ++vm->bbq->pos;
  }
}

static m_bool dummy_ini(VM* vm __attribute__((unused)), DriverInfo* di __attribute__((unused))) {
  return GW_OK;
}

void dummy_driver(Driver* d) {
  d->ini = dummy_ini;
  d->run = dummy_run;
}
