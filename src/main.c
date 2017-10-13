#include <signal.h>
#include <getopt.h>
#include <string.h>
#include <pthread.h>
#include "err_msg.h"
#include "defs.h"
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

volatile m_bool signaled = 0;
VM* vm;
static m_bool udp = 1;

static DriverInfo di = { 2, 2, 2,
  48000, 256, 3, "default:CARD=CODEC", 0, 0, D_FUNC, vm_run, 0};

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
  vm->emit->code->filename = strdup(name);
  code = emit_code(vm->emit);
  free_ast(ast);
  shred = new_vm_shred(code);
  shred->args = args;
  vm_add_shred(vm, shred);
  free(name);
  return -1;
}

static const struct option long_option[] = {
  { "add",      0, NULL, '+' },
  { "rem",      0, NULL, 'z' },
  { "pludir",   0, NULL, 'P' },
  { "quit",     0, NULL, 'q' },
  { "driver",   1, NULL, 'd' },
  { "card",     1, NULL, 'c' },
  { "backend",  1, NULL, 'b' },
  { "sr",       1, NULL, 's' },
  { "name",     1, NULL, 'n' },
  { "raw",      0, NULL, 'r' },
  { "host",     1, NULL, 'h' },
  { "port",     1, NULL, 'p' },
  { "rate",     1, NULL, 'r' },
  { "alone",    1, NULL, 'a' },
  { "in",       1, NULL, 'i' },
  { "out",      1, NULL, 'o' },
  { "bufsize",  1, NULL, 'b' },
  { "bufnum",   1, NULL, 'n' },
  { "loop",     1, NULL, 'l' },
  { "format",   1, NULL, 'l' },
  { "help",     0, NULL, '?' },
  { "version",  0, NULL, 'v' },
  { "config",   0, NULL, 'C' },
  /*  { "status"  , 0, NULL, '%' },*/
  { NULL,       0, NULL, 0   }
};

static const char usage[] =
"usage: Gwion <options>\n"
"\toption can be any of:\n"
"GLOBAL options:  <argument>  : description\n"
"\t--help,   -?\t             : this help\n"
"\t--version -v\t             : this help\n"
"VM     options:\n"
"\t--add,    -+\t <file>      : add file\n"
"\t--rem,    --\t <shred id>  : remove shred\n"
"\t--plugdir,-P\t <directory> : add a plugin directory\n"
"\t--quit    -q\t             : quit the vm\n"
"UDP    options:\n"
"\t--host    -h\t  <string>   : set host\n"
"\t--port    -p\t  <number>   : set port\n"
"\t--loop    -l\t  <0 or 1>   : loop state (0 or 1)\n"
"\t--alone   -a\t             : standalone mode. (no udp)\n"
"DRIVER options:\n"
"\t--driver  -d\t  <string>   : set the driver (one of: alsa jack soundio portaudio file dummy silent raw)\n"
"\t--sr      -s\t  <number>   : set samplerate\n"
"\t--bufnum  -n\t  <number>   : set number of buffers\n"
"\t--bufsize -b\t  <number>   : set size   of buffers\n"
"\t--chan    -g\t  <number>   : (global) channel number\n"
"\t--in      -i\t  <number>   : number of  input channel\n"
"\t--out     -o\t  <number>   : number of output channel\n"
"\t--card    -c\t  <string>   : card identifier or output file (depending on driver)\n"
"\t--raw     -r\t  <0 or 1>   : enable raw mode (file and soundio only)\n"
"\t--format  -f\t  <string>   : soundio format (one of: S8 U8 S16 U16 S24 U24 S32 U32 F32 F64)\n"
"\t--backend -e\t  <string>   : soundio backend (one of: jack pulse alsa core wasapi dummy)\n";

static void parse_args_additionnal(Arg* arg) {
  if(optind < arg->argc) {
    while(optind < arg->argc) {
      m_str str = arg->argv[optind++];
      if(!strcmp(str, "-")) {
        arg->ref = &arg->rem;
        str = arg->argv[optind++];
      } else if(!strcmp(str, "+")) {
        arg->ref = &arg->add;
        str = arg->argv[optind++];
      }
      vector_add(arg->ref, (vtype)str);
    }
  }
}

static void parse_args2(Arg* arg, int i) {
    switch(i) {
    case 'g':
      di.chan       = atoi(optarg);
      di.in       = atoi(optarg);
      di.out       = atoi(optarg);
      break;
    case 'i':
      di.in       = atoi(optarg);
      break;
    case 'o':
      di.out      = atoi(optarg);
      break;
    case 's':
      di.sr      = atoi(optarg);
      break;
    case 'l':
      arg->loop        = atoi(optarg);
      if(arg->loop == 0) arg->loop = -1;
      break;
    case 'd':
      select_driver(&di, optarg);
      break;
    case 'f':
      select_format(&di, optarg);
      break;
    case 'e':
      select_backend(&di, optarg);
      break;
    case 'r':
      di.raw = 1;
      break;
    case 'a':
      udp = 0;
      break;
    case 'P':
      vector_add(&arg->lib, (vtype)optarg);
      break;
    default:
      exit(1);
  }
}

static void parse_args(Arg* arg) {
  int i, index;
  while((i = getopt_long(arg->argc, arg->argv, "?vqh:p:i:o:n:b:e:s:d:al:g:-:rc:f:P:C ", long_option, &index)) != -1) {
    switch(i) {
      case '?':
        fprintf(stderr, usage);
        exit(0);
      case 'C':
        fprintf(stderr, "CFLAGS: %s\nLDFLAGS: %s\n", CFLAGS, LDFLAGS);
        arg->quit = 1;
        break;
      case 'q':
        arg->quit  = 1;
        break;
      case 'c':
        di.card     = optarg;
        break;
      case 'h':
        arg->host = optarg;
        break;
      case 'p':
        arg->port        = atoi(optarg);
        break;
      case 'n':
        di.bufnum    = atoi(optarg);
        break;
      case 'b':
        di.bufsize    = atoi(optarg);
        break;
      default:
        parse_args2(arg, i);
    }
  }
  parse_args_additionnal(arg);
}

int main(int argc, char** argv) {
  Env env = NULL;
  Driver d;
  Arg arg = { argc, argv, "localhost", 8888 };
  int i;
  pthread_t thread = 0;

  d.del = NULL;
  arg_init(&arg);
  parse_args(&arg);

  if(udp)
    udp_client(&arg);
  if(arg.quit)
    goto clean;
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  scan_map = new_map();
  if(!arg.loop)
    arg.loop = -1;
  if(!(vm = new_vm(arg.loop)))
    goto clean;
  if(init_bbq(vm, &di, &d) < 0)
    goto clean;
  if(!(env = type_engine_init(vm, &arg.lib)))
    goto clean;
  if(!(vm->emit = new_emitter(env)))
    goto clean;
  srand(time(NULL));

  for(i = 0; i < vector_size(&arg.add); i++)
    compile(vm, (m_str)vector_at(&arg.add, i));

  vm->is_running = 1;
  if(udp) {
    pthread_create(&thread, NULL, udp_thread, vm);
#ifndef __linux__
    pthread_detach(thread);
#endif
  }
  d.run(vm, &di);
  if(udp)
    udp_release(thread);
clean:
  arg_release(&arg);
  d.del(vm);
  if(scan_map)
    free_map(scan_map);
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
