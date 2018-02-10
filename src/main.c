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
#include "udp.h"
#include "driver.h"
#include "instr.h"
#include "arg.h"

extern void parse_args(Arg*, DriverInfo*);

volatile m_bool signaled = 0;
VM* vm;

static void sig(int unused) {
  vm->is_running = 0;
  vm->wakeup();
  signaled = 1;
}

m_bool compile(VM* vm, const m_str filename) {
  VM_Shred shred;
  VM_Code  code;
  Ast      ast;
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
    return -1;
  }
  if(!(ast = parse(name))) {
    free(name);
    return -1;
  }
  CHECK_BB(type_engine_check_prog(vm->emit->env, ast, name))
  CHECK_BB(emit_ast(vm->emit, ast, name))
  emitter_add_instr(vm->emit, EOC);
  vm->emit->code->name = strdup(name);
  code = emit_code(vm->emit);
  free_ast(ast);
  shred = new_vm_shred(code);
  shred->args = args;
  vm_add_shred(vm, shred);
  free(name);
  return -1;
}

int main(int argc, char** argv) {
  Env env = NULL;
  Driver d;
  Arg arg = { argc, argv , -1 };
  Udp udp;
  UdpIf udpif = { "localhost", 8888, 1 };
  DriverInfo di = { 2, 2, 2,
  48000, 256, 3, "default:CARD=CODEC", 0, 0, D_FUNC, vm_run, 0};
  int i;

  pthread_t thread = 0;

  d.del = NULL;
  arg_init(&arg);
  arg.udp = &udpif;
  udp.arg = &arg;
  parse_args(&arg, &di);

  if(udpif.on)
    udp_client(&udp);
  if(arg.quit)
    goto clean;
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  if(!(vm = new_vm(arg.loop)))
    goto clean;
  if(init_bbq(vm, &di, &d) < 0)
    goto clean;
  if(!(env = type_engine_init(vm, &arg.lib)))
    goto clean;
#ifdef GWCOV
  if(arg.coverage)
    vm->emit->coverage = 1;
#endif
  srand(time(NULL));

  for(i = 0; i < vector_size(&arg.add); i++)
    compile(vm, (m_str)vector_at(&arg.add, i));

  udp.vm = vm;
  vm->is_running = 1;
  if(udpif.on) {
    pthread_create(&thread, NULL, udp_thread, &udp);
#ifndef __linux__
    pthread_detach(thread);
#endif
  }
  d.run(vm, &di);
  if(udpif.on)
    udp_release(&udp, thread);
clean:
  arg_release(&arg);
  if(d.del)
    d.del(vm);
#ifndef __linux__
  sleep(1);
#endif
  if(vm && !signaled) {
    if(!vm->emit && env)
      free(env);
    free_vm(vm);
  }
  return 0;
 }
