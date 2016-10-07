#include <signal.h>
#include <getopt.h>
#include "err_msg.h"
#include "type.h"
#include "dl.h"
#include "lang.h"
#include "alsa.h"

#include "emit.h"
#include "vm.h"
#include "compile.h"
#include "udp.h"
#include "bbq.h"

#include "driver.h"
#include "shreduler.h"

//	#include <mcheck.h>
#include <string.h>

m_bool ssp_is_running;
m_bool udp = 1;

void sig(int unused) {
	ssp_is_running = 0;
}
static int do_quit = 0;

static const struct option long_option[] = {
  { "add"    , 0, NULL, '+' },
  { "remove"    , 0, NULL, '-' },
  { "quit"   , 0, NULL, 'q' },
  { "driver" , 1, NULL, 'd' },
  { "host"   , 1, NULL, 'h' },
  { "port"   , 1, NULL, 'p' },
  { "rate"   , 1, NULL, 'r' },
  { "buf"    , 1, NULL, 'b' },
  { "alone"  , 1, NULL, 'a' },
  { "in"     , 1, NULL, 'i' },
  { "out"    , 1, NULL, 'o' },
  { "bufsize"    , 1, NULL, 'b' },
  { "bufnum"    , 1, NULL, 'n' },
  { "samplerate"    , 1, NULL, 's' },
  { "loop"   , 1, NULL, 'l' },
/*  { "status" , 0, NULL, '%' },*/
  { NULL     , 0, NULL, 0   }
};

typedef Driver* (*f_driver)();
f_driver d_func = alsa_driver;
//f_driver d_func = pa_driver;
int main(int argc, char** argv)
{
/*  mcheck(NULL);*/
/*  mtrace();*/
/*  mtrace();*/
  int ret, i, index;
  m_str filename;
  DriverInfo di;
	Driver* d;
  Vector add = new_Vector();
  Vector rem = new_Vector();
  Vector ref = add;

  di.in  = 2;
  di.out = 2;
  di.sr  = 48000;
  di.buf_num   = 3;
  di.buf_size  = 64;
/*  muntrace();*/
int port = 8888;
char* hostname = "localhost";
int loop = -1;
  while((i = getopt_long(argc, argv, "+-qh:p:i:o:n:b:s:d:al: ", long_option, &index)) != -1)
  {
    switch(i)
    {
      case '+':
        ref         = add;
        break;
      case '-':
        ref         = rem;
        break;
      case 'q':
        do_quit     = 1;
        break;
      case 'h':
        hostname    = optarg;
        break;
      case 'p':
        port        = atoi(optarg);
        break;
      case 'i':
        di.in       = atoi(optarg);
        break;
      case 'o':
        di.out      = atoi(optarg);
        break;
      case 'n':
        di.buf_num  = atoi(optarg);
        break;
      case 'b':
        di.buf_size = atoi(optarg);
        break;
      case 's':
        di.sr       = atoi(optarg);
        break;
      case 'l':
        loop        = atoi(optarg);
        if(loop == 0) loop = -1;
        break;
      case 'd':
				if(!strcmp("dummy", optarg))
        	d_func = dummy_driver;
				else if(!strcmp("alsa", optarg))
        	d_func = alsa_driver;
				else if(!strcmp("dummy", optarg))
        	d_func = dummy_driver;
				else if(!strcmp("silent", optarg))
        	d_func = silent_driver;
				else if(!strcmp("jackd", optarg))
{
        printf("jackd driver not implemented yet\n");  
  return 1;
}
        else
          printf("unknown driver. back to default\n");
				break;
      case 'a':
        udp = 0;
        break;
      default:
        return 1;
    }
  }
  if(optind < argc)
  {
    while (optind < argc)
      vector_append(ref, argv[optind++]);
  }
	pthread_t udp_thread;
  if(udp)
  {
    if(server_init(hostname, port) == -1)
    {
      // enter client mode
      if(do_quit)
        Send("quit", 1);
      if(loop > 0)
        Send("loop 1", 1);
      else if(loop < 0)
        Send("loop 0", 1);
      for(i = 0; i < vector_size(rem); i++)
      {
        printf("- %s\n", vector_at(add, i));
        m_str file = vector_at(add, i);
        char name[256];
        memset(name, 0, 256);
        strcat(name, "- ");
        strcat(name, file);
        Send(name, 1);
        printf("send: %s\n", name);
      }
      for(i = 0; i < vector_size(add); i++)
      {
        printf("+ %s\n", vector_at(add, i));
        m_str file = vector_at(add, i);
        char name[256];
        memset(name, 0, 256);
        strcat(name, "+ ");
        strcat(name, file);
        Send(name, 1);
        printf("send: %s\n", name);
      }
      free_Vector(add);
      free_Vector(rem);
      exit(0);
    }
  }
  if(do_quit)
    return 0;
	signal(SIGINT, sig);
	signal(SIGTERM, sig);
	signal(SIGKILL, sig);
  scan_map = new_Map();
  VM* vm = new_VM();
  shreduler_set_loop(vm->shreduler, loop < 0 ? 0 : 1);
  ssp_is_running = 1;
  vm->bbq = calloc(1, sizeof(struct BBQ_));
	sp_createn(&vm->bbq->sp, di.out);
  vm->bbq->sp->sr = di.sr;
  d = d_func();
  d->ini(vm, &di);
	d->del(vm, 0);
  Env env = type_engine_init(vm);

  if(!env)
  {
    err_msg(COMPILE_, 0, "problem initing environment");
		free_VM(vm);
		return 1;
  }

  Emitter emit = new_Emitter(env);
  if(!emit)
  {
    err_msg(COMPILE_, 0, "problem initing emitter");
		free_VM(vm);
		free_Emitter(emit);
		return 1;
  }
  vm->emit = emit;
  srand(time(NULL));


  for(i = 0; i < vector_size(add); i++)
    compile(vm, (m_str)vector_at(add, i));
  free_Vector(add);
  free_Vector(rem);
printf("here\n");
	if(udp)
		pthread_create(&udp_thread, NULL, &server_thread, vm);
printf("here pre run\n");
  d->run(vm, &di);
printf("here\n");
  if(udp)
    server_destroy(udp_thread);

	// clean
/*	free_Driver(d, vm);*/
/*  free_Map(scan_map);*/
/*  free_Env(env);*/
/*  free_Emitter(emit);*/
/*  free_VM(vm);*/
  return 0;
}
