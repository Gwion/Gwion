#include <stdlib.h>
#include <unistd.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "vm.h"
#include "driver.h"
#include "gwion.h"
#include "plug.h"

static void dummy_run(VM* vm, struct BBQ_* di) {
  while(vm->bbq->is_running) {
    di->run(vm);
    ++vm->bbq->pos;
  }
}

static m_bool dummy_ini(VM* vm __attribute__((unused)), struct BBQ_* di __attribute__((unused))) {
  return GW_OK;
}
GWMODSTR(dummy2);
GWDRIVER(dummy2) {
  d->ini = dummy_ini;
  d->run = dummy_run;
}
