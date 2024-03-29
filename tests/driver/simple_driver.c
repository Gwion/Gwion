#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"

static DRVRUN(simple_driver_run) {
  while (di->is_running) {
    di->run(vm);
    ++di->pos;
  }
}

static DRVINI(simple_driver_ini) { return true; }

static DRVDEL(simple_driver_del) {}

GWDRIVER(driver) {
  d->ini = simple_driver_ini;
  d->run = simple_driver_run;
  d->del = simple_driver_del;
}
