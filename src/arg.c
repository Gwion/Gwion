#include <getopt.h>
#include "gwion_util.h"
#include "oo.h"
#include "vm.h"
#include "driver.h"
#include "arg.h"

ANN static void arg_init(Arg* arg) {
  vector_init(&arg->add);
  vector_init(&arg->lib);
  vector_init(&arg->mod);
  vector_add(&arg->lib, (vtype)GWPLUG_DIR);
}

ANN void arg_release(Arg* arg) {
  vector_release(&arg->add);
  vector_release(&arg->lib);
  vector_release(&arg->mod);
}

static const struct option long_option[] = {
  { "pludir",   0, NULL, 'p' },
  { "driver",   1, NULL, 'd' },
  { "sr",       1, NULL, 's' },
  { "in",       1, NULL, 'i' },
  { "out",      1, NULL, 'o' },
  { "loop",     1, NULL, 'l' },
  { "help",     0, NULL, 'h' },
  { "config",   0, NULL, 'c' },
  { "module",   0, NULL, 'm' },
  { NULL,       0, NULL, 0   }
};

static const char usage[] =
"usage: Gwion <options>\n"
"\toption can be any of:\n"
"GLOBAL options:  <argument>  : description\n"
"\t--help,      -h\t             : this help\n"
"\t--plugdir,   -P\t <directory> : add a plugin directory\n"
"\t--driver     -d\t  <string>   : set the driver\n"
"\t--module     -m\t  <string>   : module setting\n"
"\t--sr         -s\t  <number>   : set samplerate\n"
;

ANN static inline void arg_add(Arg* arg) {
  while(optind < arg->argc)
    vector_add(&arg->add, (vtype)arg->argv[optind++]);
}

ANN static void arg_drvr(DriverInfo* di, const int i) {
  switch(i) {
    case 'i':
      di->in  = (m_uint)strtol(optarg, NULL, 10);
      break;
    case 'o':
      di->out = (m_uint)strtol(optarg, NULL, 10);
      break;
    case 's':
      di->sr  = (uint)strtol(optarg, NULL, 10);
      break;
    case 'd':
      di->arg = optarg;
      break;
  }
}

ANN void arg_parse(Arg* arg, DriverInfo* di) {
  int i, idx;
  arg_init(arg);
  while((i = getopt_long(arg->argc, arg->argv, "hl:i:o:s:d:m:p:c",
      long_option, &idx)) != -1) {
    switch(i) {
      case 'h':
        gw_err(usage);
        break;
      case 'c':
        gw_err("CFLAGS: %s\nLDFLAGS: %s\n", CFLAGS, LDFLAGS);
        break;
      case 'l':
        arg->loop = strtol(optarg, NULL, 10) > 0 ? 1 : -1;
        break;
      case 'p':
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
}
