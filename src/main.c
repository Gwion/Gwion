#include <signal.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "gwion.h"

static struct Gwion_ gwion;

static void sig(int unused __attribute__((unused))) {
  gwion.vm->is_running = 0;
}

int main(int argc, char** argv) {
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  if(gwion_ini(&gwion, argc, argv) > 0)
    gwion_run(&gwion);
  gwion_end(&gwion);
  return 0;
}
