#include <stdlib.h>
#include <unistd.h>
#include "defs.h"
#include "vm.h"
#include "driver.h"

static void dummy_run(VM* vm, DriverInfo* di) {
  sp_data* sp = vm->sp;
  while(vm->is_running) {
    di->run(vm);
    ++sp->pos;
  }
}

static void silent_run(VM* vm, DriverInfo* di) {
  sp_data* sp = vm->sp;
  const m_uint timer = 1000000 / sp->sr;
  while(vm->is_running) {
    di->run(vm);
    ++sp->pos;
    usleep(timer);
  }
}

static m_bool dummy_ini(VM* vm __attribute__((unused)), DriverInfo* di __attribute__((unused))) {
  return 1;
}

void silent_driver(Driver* d) {
  d->ini = dummy_ini;
  d->run = silent_run;
}

void dummy_driver(Driver* d) {
  d->ini = dummy_ini;
  d->run = dummy_run;
}
