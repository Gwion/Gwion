#include <stdlib.h>
#include <unistd.h>
#include "defs.h"
#include "vm.h"
#include "driver.h"

//void no_wakeup(){}
static void dummy_run(VM* vm, DriverInfo* di) {
  sp_data* sp = vm->sp;
  while(vm->is_running) {
    vm_run(vm);
    sp->pos++;
    GWION_CTL
  }
}

static void silent_run(VM* vm, DriverInfo* di) {
  sp_data* sp = vm->sp;
  m_uint timer = (vm->sp->sr / 100000);
  while(vm->is_running) {
    vm_run(vm);
    sp->pos++;
    usleep(timer);
    GWION_CTL
  }
}

static m_bool dummy_ini(VM* vm, DriverInfo* di) {
  return 1;
}
static void dummy_del(VM* vm) {}

void silent_driver(Driver* d, VM* vm) { 
  d->ini = dummy_ini;
  d->run = silent_run;
  d->del = dummy_del;
  vm->wakeup = no_wakeup;
}

void dummy_driver(Driver* d, VM* vm) {
  d->ini = dummy_ini;
  d->run = dummy_run;
  d->del = dummy_del;
  vm->wakeup = no_wakeup;
}

