#include <signal.h>
#include <getopt.h>
#include <string.h>
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

volatile m_bool signaled = 0;
VM* vm;
struct Vector_ add;
struct Vector_ rem;
struct Vector_ plug_dirs;
Vector ref;
int do_quit = 0;
m_bool udp = 1;
int port = 8888;
char* hostname = "localhost";
int loop = 0;
DriverInfo di = { 2, 2, 2,
  48000, 256, 3, "default:CARD=CODEC", 0, 0, D_FUNC, 0};

static void sig(int unused) {
  vm->is_running = 0;
  vm->wakeup();
  signaled = 1;
}

m_bool compile(VM* vm, const m_str filename) {
  m_bool ret = 1;
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
    ret = -1;
    goto clean;
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
  shred->me = new_shred(vm, shred);
  vm_add_shred(vm, shred);
clean:
  free(name);
  return ret;
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
  /*  { "status"  , 0, NULL, '%' },*/
  { NULL,       0, NULL, 0   }
};

static const char _usage[] =
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

static void parse_args_additionnal(int argc, char** argv) {
  if(optind < argc) {
    while(optind < argc) {
      m_str str = argv[optind++];
      if(!strcmp(str, "-")) {
        ref = &rem;
        str = argv[optind++];
      } else if(!strcmp(str, "+")) {
        ref = &add;
        str = argv[optind++];
      }
      vector_add(ref, (vtype)str);
    }
  }
}

static void parse_args2(int i) {
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
      loop        = atoi(optarg);
      if(loop == 0) loop = -1;
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
      vector_add(&plug_dirs, (vtype)optarg);
      break;
    default:
      exit(1);
  }
}

static void parse_args(int argc, char** argv) {
  int i, index;
  while((i = getopt_long(argc, argv, "?vqh:p:i:o:n:b:e:s:d:al:g:-:rc:f:P: ", long_option, &index)) != -1) {
    switch(i) {
      case '?':
        fprintf(stderr, usage);
        exit(0);
      case 'C':
        fprintf(stderr, "CFLAGS: %s\nLDFLAGS: %s\n", CFLAGS, LDFLAGS);
        do_quit     = 1;
        break;
      case 'q':
        do_quit     = 1;
        break;
      case 'c':
        di.card     = optarg;
        break;
      case 'h':
        hostname    = optarg;
        break;
      case 'p':
        port        = atoi(optarg);
        break;
      case 'n':
        di.bufnum    = atoi(optarg);
        break;
      case 'b':
        di.bufsize    = atoi(optarg);
        break;
      default:
        parse_args2(i);
    }
  }
  parse_args_additionnal(argc, argv);
}

static void do_udp() {
  m_uint i;
  if(server_init(hostname, port) == -1) {
    if(do_quit)
      Send("quit", 1);
    if(loop > 0)
      Send("loop 1", 1);
    else if(loop < 0)
      Send("loop 0", 1);
    for(i = 0; i < vector_size(&rem); i++) {
      m_str file = (m_str)vector_at(&rem, i);
      m_uint size = strlen(file) + 3;
      char name[size];
      memset(name, 0, size);
      strcpy(name, "- ");
      strcat(name, file);
      Send(name, 1);
    }
    for(i = 0; i < vector_size(&add); i++) {
      m_str file = (m_str)vector_at(&add, i);
      m_uint size = strlen(file) + 3;
      char name[size];
      memset(name, 0, size);
      strcpy(name, "+ ");
      strcat(name, file);
      Send(name, 1);
    }
    vector_release(&add);
    vector_release(&rem);
    vector_release(&plug_dirs);
    exit(0);
  }
}

int main(int argc, char** argv) {
  Env env = NULL;
  Driver d;
  int i;
  pthread_t udp_thread = 0;

  d.del = NULL;
  vector_init(&add);
  vector_init(&rem);
  vector_init(&plug_dirs);
  ref = &add;
  vector_add(&plug_dirs, (vtype)GWION_ADD_DIR);
  parse_args(argc, argv);

  if(udp)
    do_udp();
  if(do_quit)
    goto clean;
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  scan_map = new_map();
  if(!loop)
    loop = -1;
  if(!(vm = new_vm(loop)))
    goto clean;
  if(init_bbq(vm, &di, &d) < 0)
    goto clean;
  if(!(env = type_engine_init(vm, &plug_dirs)))
    goto clean;
  if(!(vm->emit = new_emitter(env)))
    goto clean;
  srand(time(NULL));

  for(i = 0; i < vector_size(&add); i++)
    compile(vm, (m_str)vector_at(&add, i));

  vm->is_running = 1;
  if(udp) {
    pthread_create(&udp_thread, NULL, server_thread, vm);
#ifndef __linux__
    pthread_detach(udp_thread);
#endif
  }
  d.run(vm, &di);
  if(udp)
    server_destroy(udp_thread);
clean:
  vector_release(&plug_dirs);
  vector_release(&add);
  vector_release(&rem);
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
