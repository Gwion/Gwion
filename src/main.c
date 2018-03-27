#include <stdlib.h>
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
#include "compile.h"
#ifdef GWUDP
#include "udp.h"
#endif
#include "driver.h"
#include "instr.h"
#include "arg.h"
#include "repl.h"

extern void parse_args(Arg*, DriverInfo*);

volatile m_bool signaled = 0;
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
  if(!(ast = parse(name, f))) {
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
  if(GET_FLAG(code, ae_flag_recurs))
    code_optim(vm->emit);
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
#ifdef GWUDP
  Udp udp;
  UdpIf udpif = { "localhost", 8888, 1 };
#endif
  DriverInfo di = { 2, 2, 2,
  48000, 256, 3, "default:CARD=CODEC", 0, 0, D_FUNC, vm_run, 0, 0};

#ifdef GWUDP
  pthread_t thread;
#endif
  GWREPL_THREAD
  d.del = NULL;
  memset(&arg, 0, sizeof(Arg));
  arg.argc = argc;
  arg.argv = argv;
  arg.loop = -1;
  arg_init(&arg);
#ifdef GWUDP
  arg.udp = &udpif;
  udp.arg = &arg;
#endif
  parse_args(&arg, &di);

#ifdef GWUDP
  if(udpif.on)
    udp_client(&udp);
#endif
  if(arg.quit)
    goto clean;
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  vm = new_vm(arg.loop);
  if(init_bbq(vm, &di, &d) < 0)
    goto clean;
  if(!(env = type_engine_init(vm, &arg.lib)))
    goto clean;
#ifdef GWCOV
  if(arg.coverage)
    vm->emit->coverage = 1;
#endif
#ifdef GWCGRAPH
  if(arg.profile) {
    vm->emit->profile = 1;
    vm->emit->call_file = fopen("gwmon.out", "w");
  }
#endif
  srand(time(NULL));

  for(m_uint i = 0; i < vector_size(&arg.add); i++)
    compile(vm, (m_str)vector_at(&arg.add, i));

  vm->is_running = 1;
#ifdef GWUDP
  if(udpif.on) {
    udp.vm = vm;
    pthread_create(&thread, NULL, udp_process, &udp);
#ifndef __linux__
    pthread_detach(thread);
#endif
  }
#endif
  GWREPL_INI(vm, &repl_thread)
  d.run(vm, &di);
  GWREPL_END(repl_thread)

#ifdef GWUDP
  if(udpif.on)
    udp_release(&udp, thread);
#endif
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
  return 0;
}
