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
  pthread_kill(pthread_self(), SIGTERM);
  pthread_exit(NULL);
#endif
}

#ifdef __AFL_HAVE_MANUAL_CONTROL

ANN static void gwion_reset(const Gwion gwion) {
  pop_global(gwion);
  push_global(gwion, "[user]");
}

#define BUFSIZE 64

static void afl_run(const Gwion gwion) {
  char buf[BUFSIZE];
  struct GwText_ text = { .mp=gwion->mp };
  while (__AFL_LOOP(5)) {
    ssize_t sz;
    memset(buf, 0, BUFSIZE);
    while((sz = read(0, buf, BUFSIZE)) > 0) {
      buf[sz] = '\0';
      text_add(&text, buf);
    }
    if(compile_string(gwion, "afl", text.str))
      gwion_run(gwion);
    text_reset(&text);
    gwion_reset(gwion);
  }
    text_release(&text);
}
#define gwion_run(a) afl_run(a)
#endif

int main(int argc, char** argv) {
  Arg arg = { .arg={.argc=argc, .argv=argv}, .loop=-1 };
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  struct Gwion_ gwion = {};
  const m_bool ini = gwion_ini(&gwion, &arg);
  arg_release(&arg);
  if(ini > 0)
    gwion_run(&gwion);
  gwion_end(&gwion);
  THREAD_RETURN(EXIT_SUCCESS);
}
