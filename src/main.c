#include <stdlib.h>
#include <stdio.h>
//#include <stdio_ext.h>
#include <signal.h>
#include <getopt.h>
#include <string.h>
#include <pthread.h>
#include "defs.h"
#include "err_msg.h"
#include "map.h"
#include "vm.h"
#include "absyn.h"
#include "type.h"
#include "emit.h"
#include "hash.h"
#include "scanner.h"
#include "compile.h"
#include "driver.h"
#include "instr.h"
#include "arg.h"
#include "repl.h"

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

VM* vm;

static void sig(int unused __attribute__((unused))) {
  vm->is_running = 0;
}

m_uint compile(VM* vm, const m_str filename) {
  VM_Shred shred;
  VM_Code  code;
  Ast      ast;
  FILE* f;
  Vector args = NULL;
  m_str _name, name, d = strdup(filename);
  _name = strsep(&d, ":");
  if(d)
    args = new_vector();
  while(d)
    vector_add(args, (vtype)strdup(strsep(&d, ":")));
  free(d);
  name = realpath(_name, NULL);
  free(_name);
  if(!name) {
    err_msg(COMPILE_, 0, "error while opening file '%s'", filename);
    return 0;
  }
  if(!(f = fopen(name, "r")))
    free(name);
  if(!(ast = parse(vm->scan, name, f))) {
    free(name);
    fclose(f);
    return 0;
  }
  if(type_engine_check_prog(vm->emit->env, ast, name) < 0 ||
    emit_ast(vm->emit, ast, name) < 0) {
    fclose(f);
    return 0;
  }
  emitter_add_instr(vm->emit, EOC);
  vm->emit->code->name = strdup(name);
  code = emit_code(vm->emit);
  free_ast(ast);
  shred = new_vm_shred(code);
  shred->args = args;
  vm_add_shred(vm, shred);
  free(name);
  fclose(f);
  return shred->xid;
}

int main(int argc, char** argv) {
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
  vm = new_vm(arg.loop);
  vm->emit = new_emitter();
  di.func(&d);
  if(d.ini(vm, &di) < 0 || !(vm->bbq = new_bbq(&di)))
    goto clean;
  if(!(env = type_engine_init(vm, &pi[1])))
    goto clean;
#ifdef GWCOV
  if(arg.coverage)
    vm->emit->coverage = 1;
#endif
  srand(time(NULL));
  for(m_uint i = 0; i < vector_size(&arg.add); i++)
    compile(vm, (m_str)vector_at(&arg.add, i));
  vm->is_running = 1;
  GWREPL_INI(vm)
  VMBENCH_INI
  d.run(vm, &di);
  VMBENCH_END
  GWREPL_END()
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
