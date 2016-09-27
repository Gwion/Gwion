#include <stdlib.h>
#include <unistd.h>
#include "defs.h"
#include "vm.h"
#include "bbq.h"
#include "driver.h"
extern int ssp_is_running;

static void dummy_run(VM* vm, DriverInfo* di)
{
  vm->bbq->in = calloc(vm->bbq->sp->nchan, sizeof(SPFLOAT));
  while(ssp_is_running)
  {
    vm_run(vm);
    vm->bbq->sp->pos++;
  }
}

static void silent_run(VM* vm, DriverInfo* di)
{
  m_uint timer = (vm->bbq->sp->sr / 100000);
  vm->bbq->in = calloc(vm->bbq->sp->nchan, sizeof(SPFLOAT));
  while(ssp_is_running)
  {
    vm_run(vm);
    vm->bbq->sp->pos++;
    usleep(timer);
  }
}

static m_bool dummy_ini(VM* vm, DriverInfo* di){}
static void dummy_del(VM* vm, int finish){}

Driver* silent_driver()
{
  Driver* d = malloc(sizeof(Driver));
  d->ini = dummy_ini;
  d->run = silent_run;
  d->del = dummy_del;
  return d;
}

Driver* dummy_driver()
{
  Driver* d = malloc(sizeof(Driver));
  d->ini = dummy_ini;
  d->run = dummy_run;
  d->del = dummy_del;
  return d;
}

