#include <signal.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "arg.h"

static struct Gwion_ gwion = {};

static void sig(int unused NUSED) {
  if(gwion.vm)
    gwion.vm->bbq->is_running = false;
  else
    exit(EXIT_FAILURE);
}

#ifdef __AFL_HAVE_MANUAL_CONTROL

#include "compile.h"

static void afl_run(const Gwion gwion) {
  __AFL_INIT();
  while (__AFL_LOOP(128)) {
    push_global(gwion, "[afl]");
    FILE *f = fdopen(0, "r");
    if (compile_file(gwion, "afl", f)) gwion_run(gwion);
    pop_global(gwion);
  }
}

int main(int argc, char **argv) {
  Arg arg = {};
  gwion_ini(&gwion, &arg);
  arg_release(&arg);
  afl_run(&gwion);
  return EXIT_SUCCESS;
}

#else

int main(int argc, char **argv) {
  Arg arg = {.arg = {.argc = argc, .argv = argv}, .loop = false};
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  struct Gwion_ gwion = {};
  const m_bool  ini   = gwion_ini(&gwion, &arg);
  arg_release(&arg);
  if (ini > 0) gwion_run(&gwion);
  gwion_end(&gwion);
#ifndef BUILD_ON_WINDOWS
  pthread_exit(NULL);
#endif
  return EXIT_SUCCESS;
}

#endif
