#include <stdlib.h>
#include "vm.h"
#include "bbq.h"
#include "driver.h"

static sp_audio spa;
static m_bool raw_ini(VM* vm, DriverInfo* di)
{
  char tmp[104];
  sprintf(tmp, "%s.raw", di->card);
  if(spa_open(vm->bbq->sp, &spa, tmp, SPA_WRITE) == SP_NOT_OK) {
    fprintf(stderr, "Error: could not open file %s.\n", di->card);
    return -1;
  }
  return 1;
}

static void raw_run(VM* vm, DriverInfo* di)
{
  while(vm->is_running) {
    vm_run(vm);
    spa_write_buf(vm->bbq->sp, &spa, vm->bbq->sp->out, vm->bbq->sp->nchan);
    vm->bbq->sp->pos++;
  }
}

static void raw_del(VM* vm)
{
  spa_close(&spa);
}

Driver* raw_driver(VM* vm)
{
  Driver* d = malloc(sizeof(Driver));
  d->ini = raw_ini;
  d->run = raw_run;
  d->del = raw_del;
  vm->wakeup = no_wakeup;
  return d;
}
