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
  const Env env = gwion->env;
  __AFL_INIT();
  while (__AFL_LOOP(128)) {
    const Nspc nspc = env->global_nspc;
    env->curr = env->global_nspc = new_nspc(gwion->mp, "[afl]");
    env->global_nspc->parent = nspc;
    FILE *f = fdopen(0, "r");
    if (compile_file(gwion, "afl", f)) gwion_run(gwion);
    free_nspc(env->global_nspc, env->gwion);
    env->curr = env->global_nspc = nspc;
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

#ifdef GWION_CONFIG_ARGS
ANN char **config_args(int *, char **const);
#endif
#ifdef GWION_EMBED
ANN void gwion_embed(const Gwion);
#endif

int main(int argc, char **argv) {
#ifndef GWION_CONFIG_ARGS
  CliArg arg = {.arg = {.argc = argc, .argv = argv}, .loop = false};
#else
  char **config_argv = config_args(&argc, argv);
  CliArg arg = {.arg = {.argc = argc, .argv = config_argv}, .loop = false};
#endif
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  const m_bool  ini   = gwion_ini(&gwion, &arg);
#ifdef GWION_EMBED
  gwion_embed(&gwion);
#endif
  if(ini > 0) arg_compile(&gwion, &arg);
  arg_release(&arg);
#ifdef GWION_CONFIG_ARGS
  free(config_argv);
#endif
  if (ini > 0) gwion_run(&gwion);
  gwion_end(&gwion);
  gwion.vm = NULL;
  exit(EXIT_SUCCESS);
}

#endif
