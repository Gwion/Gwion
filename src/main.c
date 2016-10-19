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
static VM* vm = NULL;
void sig(int unused) {
	ssp_is_running = 0;
	vm->wakeup();
}
static int do_quit = 0;

static const struct option long_option[] = {
  { "add"    , 0, NULL, '+' },
  { "remove"    , 0, NULL, '-' },
  { "quit"   , 0, NULL, 'q' },
  { "driver" , 1, NULL, 'd' },
  { "backend" , 1, NULL, 'b' },
  { "sr"     , 1, NULL, 's' },
  { "name"    , 1, NULL, 'n' },
  { "raw"    , 0, NULL, 'r' },
  { "host"   , 1, NULL, 'h' },
  { "port"   , 1, NULL, 'p' },
  { "rate"   , 1, NULL, 'r' },
  { "buf"    , 1, NULL, 'b' },
  { "alone"  , 1, NULL, 'a' },
  { "in"     , 1, NULL, 'i' },
  { "out"    , 1, NULL, 'o' },
//  { "bufsize"    , 1, NULL, 'b' },
//  { "bufnum"    , 1, NULL, 'n' },
  { "loop"   , 1, NULL, 'l' },
/*  { "status" , 0, NULL, '%' },*/
  { NULL     , 0, NULL, 0   }
};

typedef (*Driver_Func)();
int main(int argc, char** argv)
{
  int ret, i, index;
  m_str filename;
	Driver_Func d_func = D_FUNC;
  Vector add = new_Vector();
  Vector rem = new_Vector();
  Vector ref = add;

	int samplerate = 48000;
	m_str id = "default:CARD=CODEC";
	m_str name = NULL;
	m_bool raw = 0;
	m_uint in = 2;
	m_uint out = 2;
	int port = 8888;
	char* hostname = "localhost";
	int loop = -1;
	DriverInfo di;
	di.in  = 2;
	di.out = 2;
	di.chan = 2;
	di.sr = 48000;
	di.bufsize = 256;
	di.bufnum = 3;
	di.card = "default:CARD=CODEC";
  while((i = getopt_long(argc, argv, "+-qh:p:i:o:n:b:s:d:al:r ", long_option, &index)) != -1)
  {
    switch(i)
    {
      case '+':
        ref         = add;
        break;
      case '-':
        ref         = rem;
        break;
			case 'd':
				if(!strcmp("file", optarg))
				{
					d_func = sndfile_driver;
					di.card = "/tmp/gwion";
				}
				else if(!strcmp("dummy", optarg))
					d_func = dummy_driver;
				else if(!strcmp("silent", optarg))
					d_func = silent_driver;
#ifdef HAVE_ALSA
				else if(!strcmp("alsa", optarg))
				{
					d_func = alsa_driver;
					di.card = "default";
				}
#endif
#ifdef HAVE_JACK
				else if(!strcmp("jack", optarg))
					d_func = jack_driver;
#endif
#ifdef HAVE_SOUNDIO
				else if(!strcmp("soundio", optarg))
					d_func = sio_driver;
#endif
#ifdef HAVE_PORTAUDIO
				else if(!strcmp("portaudio", optarg))
					d_func = pa_driver;
#endif
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
      case 'i':
        //di.in       = atoi(optarg);
        break;
      case 'o':
        //di.out      = atoi(optarg);
        break;
      case 's':
        samplerate       = atoi(optarg);
        break;
      case 'l':
        loop        = atoi(optarg);
        if(loop == 0) loop = -1;
        break;
/*
      case 'b':
				if(!strcmp("dummy", optarg))
        	backend = SoundIoBackendDummy;
				else if(!strcmp("alsa", optarg))
        	backend = SoundIoBackendAlsa;
				else if(!strcmp("jack", optarg))
        	backend = SoundIoBackendJack;
				else if(!strcmp("pulse", optarg))
        	backend = SoundIoBackendPulseAudio;
				else if(!strcmp("core", optarg))
        	backend = SoundIoBackendCoreAudio;
				else if(!strcmp("Wasapi", optarg))
        	backend = SoundIoBackendWasapi;
        else
          printf("unknown driver. back to default\n");
				break;
*/
      case 'n':
        name = optarg;
        break;
      case 'r':
        di.raw = 1;
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
#ifdef HAVE_SPA
	if(di.raw && d_func == sndfile_driver)
		d_func = raw_driver;
#endif
	pthread_t udp_thread;
  if(udp)
  {
    if(server_init(hostname, port) == -1)
    {
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
	vm = new_VM();
  shreduler_set_loop(vm->shreduler, loop < 0 ? 0 : 1);
  ssp_is_running = 1;
  vm->bbq = calloc(1, sizeof(struct BBQ_));
	sp_createn(&vm->bbq->sp, 2);

	Driver* d = d_func(vm);
	CHECK_OO(d->ini(vm, &di))
	vm->bbq->sp->nchan = di.out;
	vm->bbq->in   = calloc(vm->bbq->sp->nchan, sizeof(SPFLOAT));
  vm->bbq->sp->sr = samplerate;

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
		free_Emitter(emit);
		free_VM(vm);
		return 1;
  }
  vm->emit = emit;
  srand(time(NULL));


  for(i = 0; i < vector_size(add); i++)
    compile(vm, (m_str)vector_at(add, i));
  free_Vector(add);
  free_Vector(rem);
	if(udp)
		pthread_create(&udp_thread, NULL, &server_thread, vm);
	d->run(vm, &di);
  if(udp)
    server_destroy(udp_thread);

	// clean
	free_Driver(d, vm);
  free_Map(scan_map);
/*  free_Env(env);*/
/*  free_Emitter(emit);*/
/*  free_VM(vm);*/
  return 0;
}
