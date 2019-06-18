#include <signal.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "gwion.h"
#include "arg.h"

#ifdef __linux__
#include<libintl.h>
#include<locale.h>
#endif

//static jmp_buf jmp;
//static struct Gwion_ gwion;

static void sig(int unused NUSED) {
#ifdef BUILD_ON_WINDOWS
  exit(EXIT_FAILURE);
#else
  pthread_exit(NULL);
#endif
}

int main(int argc, char** argv) {
//  setlocale(LC_ALL, NULL);
//puts(
// bindtextdomain ("bison-runtime", "/usr/share/locale")
//);
//  bindtextdomain ("bison-runtime", NULL);
//puts(
  bindtextdomain ("gwion_ast", "ast/po");
  bindtextdomain ("gwion_util", "util/po");
  bindtextdomain ("gwion", "po");
//);
  Arg arg = { .argc=argc, .argv=argv, .loop=-1 };
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  struct Gwion_ gwion = { .plug=NULL };
  const m_bool ini = gwion_ini(&gwion, &arg);
  arg_release(&arg);
  if(/*setjmp(jmp) == 0 && */ini > 0)
    gwion_run(&gwion);
  gwion_end(&gwion);
  THREAD_RETURN(EXIT_SUCCESS);
  return EXIT_SUCCESS;
}
