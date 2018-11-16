#include <stdlib.h>
#include <stdio.h>
//#include <stdio_ext.h>
#include <signal.h>
#include <getopt.h>
#include <string.h>
#include <pthread.h>
#include "gwion_util.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "emit.h"
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

#include "plug.h"
#include <dlfcn.h>
#include <dirent.h>

extern void parse_args(Arg*, DriverInfo*);

VM* some_global_vm_soon_to_disapear;

static void sig(int unused __attribute__((unused))) {
  some_global_vm_soon_to_disapear->is_running = 0;
}

int main(int argc, char** argv) {
  VM* vm = NULL;
  Env env = NULL;
  Driver d;
  Arg arg;
  DriverInfo di = { 2, 2, 2,
  48000, 256, 3, "default:CARD=CODEC", 0, 0, D_FUNC, vm_run, 0, 0};

  d.del = NULL;
  memset(&arg, 0, sizeof(Arg));
  arg.argc = argc;
  arg.argv = argv;
  arg.loop = -1;
  arg_init(&arg);
//__fsetlocking(stdout, FSETLOCKING_BYCALLER);
//__fsetlocking(stderr, FSETLOCKING_BYCALLER);

  parse_args(&arg, &di);

  if(arg.quit)
    goto clean;
  signal(SIGINT, sig);
  signal(SIGTERM, sig);

//  init_symbols();
  PlugInfo pi;
  plug_ini(pi, &arg.lib);
  vm = some_global_vm_soon_to_disapear = new_vm((m_bool)arg.loop); // change type in struct to m_bool
  vm->emit = new_emitter();
  di.func(&d);
  if(d.ini(vm, &di) < 0 || !(vm->bbq = new_bbq(&di)))
    goto clean;
  if(!(env = type_engine_init(vm, &pi[1])))
    goto clean;
  srand((uint)time(NULL));
  for(m_uint i = 0; i < vector_size(&arg.add); i++)
    compile_filename(vm, (m_str)vector_at(&arg.add, i));
  vm->is_running = 1;
  VMBENCH_INI
  d.run(vm, &di);
  VMBENCH_END
clean:
  arg_release(&arg);
  if(d.del)
    d.del(vm, &di);
#ifndef __linux__
  sleep(1);
#endif
  if(!vm->emit && env)
    free(env);
  free_vm(vm);
  free_symbols();
  plug_end(pi);
  return 0;
}
