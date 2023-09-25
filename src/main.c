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
  Arg arg = {
    .thread_count = 4,
    .queue_size = 16
  };
  gwion_ini(&gwion, &arg);
  arg_release(&arg);
  afl_run(&gwion);
  return EXIT_SUCCESS;
}

#else

#ifdef GWION_CONFIG_ARGS
ANN char **gwion_config_args(int *, char **const);
#else
#define gwion_config_args NULL
#endif

#ifdef GWION_EMBED_LIBS
ANN void gwion_embed_libs(Gwion);
#else
#define gwion_embed_libs NULL
#endif

#ifdef GWION_EMBED_SCRIPTS
ANN void gwion_embed_scripts(Gwion);
#else
#define gwion_embed_scripts NULL
#endif

#ifndef GWION_HAS_URC
#define GWION_HAS_URC true
#endif

#ifndef GWION_HAS_ULIB
#define GWION_HAS_ULIB true
#endif

#ifndef GWION_HAS_UARGS
#define GWION_HAS_UARGS true
#endif

int main(int argc, char **argv) {
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  CliArg arg = {
    .arg = {.argc = argc, .argv = argv},
    .urc = GWION_HAS_URC,
    .ulib = GWION_HAS_ULIB,
    .uargs = GWION_HAS_UARGS,
    .config_args = gwion_config_args,
    .embed_libs = gwion_embed_libs,
    .embed_scripts = gwion_embed_scripts,
    .thread_count = 4,
    .queue_size = 16
  };
  const m_bool  ini = gwion_ini(&gwion, &arg);
  arg_release(&arg);
  if (ini > 0) gwion_run(&gwion);
  const bool ret = gwion.data->errored;
  gwion_end(&gwion);
  gwion.vm = NULL;
  return ret;
}
#endif
