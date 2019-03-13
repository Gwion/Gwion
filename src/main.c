#include <signal.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "gwion.h"
#include "arg.h"

static jmp_buf jmp;
static struct Gwion_ gwion;

static void sig(int unused __attribute__((unused))) {
  gwion.vm->bbq->is_running = 0;
  longjmp(jmp, 1);
}

int main(int argc, char** argv) {
  Arg arg = { .argc=argc, .argv=argv, .loop=-1 };
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  const m_bool ini = gwion_ini(&gwion, &arg);
  arg_release(&arg);
  if(setjmp(jmp) == 0 && ini > 0)
    gwion_run(&gwion);
  gwion_end(&gwion);
  return EXIT_SUCCESS;
}
