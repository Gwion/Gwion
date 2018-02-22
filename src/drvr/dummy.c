#include <stdlib.h>
#include <unistd.h>
#include "defs.h"
#include "vm.h"
#include "driver.h"

static void dummy_run(VM* vm, DriverInfo* di) {
  sp_data* sp = vm->sp;
  while(vm->is_running) {
    di->run(vm);
    sp->pos++;
  }
}

static void silent_run(VM* vm, DriverInfo* di) {
  sp_data* sp = vm->sp;
  m_uint timer = (vm->sp->sr / 100000);
  while(vm->is_running) {
    di->run(vm);
    sp->pos++;
    usleep(timer);
  }
}

static m_bool dummy_ini(VM* vm, DriverInfo* di) {
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
