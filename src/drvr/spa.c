#include <stdlib.h>
#include <string.h>
#include <soundpipe.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "driver.h"

static m_bool spa_ini(VM* vm __attribute__((unused)), DriverInfo* di) {
  di->data = (sp_audio*)xmalloc(sizeof(sp_audio));
  return GW_OK;
}

static void spa_run(VM* vm, DriverInfo* di) {
  char tmp[strlen(di->card) + 5];
  sp_audio* spa = di->data;
  sprintf(tmp, "%s.spa", di->card);
  if(spa_open(vm->sp, spa, tmp, SPA_WRITE) == SP_NOT_OK) {
    gw_err("Error: could not open file %s.\n", di->card);
    return;
  }
  while(vm->is_running) {
    di->run(vm);
    spa_write_buf(vm->sp, spa, sp->out, sp->nchan);
    ++sp->pos;
  }
}

static void spa_del(VM* vm __attribute__((unused)), DriverInfo* di) {
  sp_audio* spa = (sp_audio*)di->data;
  spa_close(spa);
  free(spa);
}

void spa_driver(Driver* d) {
  d->ini = spa_ini;
  d->run = spa_run;
  d->del = spa_del;
}
