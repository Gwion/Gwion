#include <signal.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "arg.h"

static void sig(int unused NUSED) {
#ifdef BUILD_ON_WINDOWS
  exit(EXIT_FAILURE);
#else
  pthread_exit(NULL);
#endif
}

int main(int argc, char** argv) {
  Arg arg = { .argc=argc, .argv=argv, .loop=-1 };
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  struct Gwion_ gwion = { .plug=NULL };
  const m_bool ini = gwion_ini(&gwion, &arg);
  arg_release(&arg);
  if(ini > 0)
    gwion_run(&gwion);
  gwion_end(&gwion);
  THREAD_RETURN(EXIT_SUCCESS);
}
