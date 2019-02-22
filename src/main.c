#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include <string.h> // arg memset
#include <pthread.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "compile.h"
#include "driver.h"
#include "arg.h"
#include "engine.h"

#ifdef VMBENCH
#include <time.h>
#include <bsd/sys/time.h>
#define VMBENCH_INI                             \
  struct timespec ini, end, ret;                \
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ini);
#define VMBENCH_END                                      \
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);          \
  timespecsub(&end, &ini, &ret);                         \
  printf("timespec %lu.%09lu\n", ret.tv_sec, ret.tv_nsec);
#else
#define VMBENCH_INI
#define VMBENCH_END
#endif

#include "gwion.h"
#include "plug.h"
#include "module.h"

extern void parse_args(Arg*, DriverInfo*);
static VM* some_static_vm;

static void sig(int unused __attribute__((unused))) {
  some_static_vm->is_running = 0;
}

int main(int argc, char** argv) {
  Driver d = { };
  Arg arg = { .argc = argc, .argv=argv, .loop=-1, .quit=0};
  DriverInfo di = { 2, 2, 2,
  48000, 256, 3, "default:CARD=CODEC", 0, 0, D_FUNC, vm_run, 0, 0};

  arg_init(&arg);

//#define STD_BUFSZ 10000
//char buf[STD_BUFSZ];
//setvbuf(stdout, buf, _IOFBF, STD_BUFSZ);
//char buf2[STD_BUFSZ];
//setvbuf(stderr, buf2, _IOFBF, STD_BUFSZ);

  parse_args(&arg, &di);
struct Gwion_ gwion;

  if(arg.quit)
    goto clean;
  signal(SIGINT, sig);
  signal(SIGTERM, sig);

//  init_symbols();
  PlugInfo pi;
//  gwion.pi = pi;
  plug_ini(pi, &arg.lib);


  gwion_init(&gwion);
  some_static_vm = gwion.vm;
  di.func(&d);
  shreduler_set_loop(gwion.vm->shreduler, arg.loop);
  if(d.ini(gwion.vm, &di) < 0 || !(gwion.vm->bbq = new_bbq(&di)))
    goto clean;
  if(type_engine_init(gwion.vm, &pi[GWPLUG_IMPORT]) < 0)
    goto clean;
  module_ini(&gwion, &pi[GWPLUG_MODULE], &arg.mod);
  srand((uint)time(NULL));
  for(m_uint i = 0; i < vector_size(&arg.add); i++)
    compile_filename(&gwion, (m_str)vector_at(&arg.add, i));
  gwion.vm->is_running = 1;
  VMBENCH_INI
  d.run(gwion.vm, &di);
  VMBENCH_END
clean:
  arg_release(&arg);
  if(d.del)
    d.del(gwion.vm, &di);
  plug_end(&gwion, pi);
  gwion_release(&gwion);
  return 0;
}
