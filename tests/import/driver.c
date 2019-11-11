#include <stdlib.h>
#include <unistd.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "driver.h"
#include "gwion.h"
#include "plug.h"

static DRVRUN(driver_test_run) {
  while(di->is_running) {
    di->run(vm);
    ++di->pos;
  }
}

static DRVINI(driver_test_ini) {
  return GW_OK;
}

static DRVDEL(driver_test_del) {}

GWMODSTR(driver_test);

GWDRIVER(driver_test) {
  d->ini = driver_test_ini;
  d->run = driver_test_run;
  d->del = driver_test_del;
}
