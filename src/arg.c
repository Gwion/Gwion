#include <getopt.h>
#include <string.h>
#include "gwion_util.h"
#include "oo.h"
#include "vm.h"
#include "driver.h"
#include "arg.h"

ANN static void arg_init(Arg* arg) {
  vector_init(&arg->add);
  vector_init(&arg->rem);
  vector_init(&arg->lib);
  vector_init(&arg->mod);
  vector_add(&arg->lib, (vtype)GWPLUG_DIR);
}

ANN void arg_release(Arg* arg) {
  vector_release(&arg->add);
  vector_release(&arg->rem);
  vector_release(&arg->lib);
  vector_release(&arg->mod);
}

static const struct option long_option[] = {
  { "pludir",   0, NULL, 'P' },
  { "driver",   1, NULL, 'd' },
  { "sr",       1, NULL, 's' },
  { "in",       1, NULL, 'i' },
  { "out",      1, NULL, 'o' },
  { "loop",     1, NULL, 'l' },
  { "help",     0, NULL, '?' },
  { "version",  0, NULL, 'v' },
  { "config",   0, NULL, 'C' },
  { "module",   0, NULL, 'm' },
  { NULL,       0, NULL, 0   }
};

static const char usage[] =
"usage: Gwion <options>\n"
"\toption can be any of:\n"
"GLOBAL options:  <argument>  : description\n"
"\t--help,      -?\t             : this help\n"
"\t--version    -v\t             : this help\n"
"VM     options:\n"
"\t--plugdir,   -P\t <directory> : add a plugin directory\n"
"DRIVER options:\n"
"\t--driver     -d\t  <string>   : set the driver (one of: alsa jack soundio portaudio file dummy silent raw)\n"
"\t--sr         -s\t  <number>   : set samplerate\n"
;

ANN static void arg_add(Arg* arg) {
  Vector ref = &arg->add;
  while(optind < arg->argc) {
    m_str str = arg->argv[optind++];
    if(!strcmp(str, "-")) {
      ref = &arg->rem;
      str = arg->argv[optind++];
    } else if(!strcmp(str, "+")) {
      ref = &arg->add;
      str = arg->argv[optind++];
    }
    vector_add(ref, (vtype)str);
  }
}

ANN static void arg_drvr(DriverInfo* di, const int i) {
    switch(i) {
      case 'i':
        di->in       = (m_uint)strtol(optarg, NULL, 10);
        break;
      case 'o':
        di->out      = (m_uint)strtol(optarg, NULL, 10);
        break;
      case 's':
        di->sr       = (uint)strtol(optarg, NULL, 10);
        break;
      case 'd':
        di->arg = optarg;
        break;
    default:
      gw_err("Unknown argument '%c'\n", i);
  }
}

ANN uint parse_args(Arg* arg, DriverInfo* di) {
  uint quit = 0;
  int i, index;
  arg_init(arg);
  while((i = getopt_long(arg->argc, arg->argv, "?vl:i:o:s:d:m:P:C ",
      long_option, &index)) != -1) {
    switch(i) {
      case '?':
        gw_err(usage);
        exit(0);
      case 'C':
        gw_err("CFLAGS: %s\nLDFLAGS: %s\n", CFLAGS, LDFLAGS);
        exit(1);
        break;
      case 'l':
        arg->loop = strtol(optarg, NULL, 10) > 0 ? 1 : -1;
        break;
      case 'P':
        vector_add(&arg->lib, (vtype)optarg);
        break;
      case 'm':
        vector_add(&arg->mod, (vtype)optarg);
        break;
      default:
        arg_drvr(di, i);
    }
  }
  arg_add(arg);
  return quit;
}
