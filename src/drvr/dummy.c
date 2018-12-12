#include <stdlib.h>
#include <unistd.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "driver.h"

static void dummy_run(VM* vm, DriverInfo* di) {
  while(vm->is_running) {
    di->run(vm);
    ++vm->bbq->pos;
  }
}

static void silent_run(VM* vm, DriverInfo* di) {
  const uint timer = 1000000 / vm->bbq->sr;
  while(vm->is_running) {
    di->run(vm);
    ++vm->bbq->pos;
    usleep(timer);
  }
}

static m_bool dummy_ini(VM* vm __attribute__((unused)), DriverInfo* di __attribute__((unused))) {
  return GW_OK;
}

void silent_driver(Driver* d) {
  d->ini = dummy_ini;
  d->run = silent_run;
}

void dummy_driver(Driver* d) {
  d->ini = dummy_ini;
  d->run = dummy_run;
}
