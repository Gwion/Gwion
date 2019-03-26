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

static DRVRUN(dummy_run) {
  while(di->is_running) {
    di->run(vm);
    ++di->pos;
  }
}

static DRVINI(dummy_ini) {
  return GW_OK;
}

GWMODSTR(dummy2);

GWDRIVER(dummy2) {
  d->ini = dummy_ini;
  d->run = dummy_run;
}
