#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "defs.h"
#include "map.h"
#include "map_private.h"
#include "arg.h"
#include "vm.h"
#include "driver.h"

void arg_init(Arg* arg) {
  vector_init(&arg->add);
  vector_init(&arg->rem);
  vector_init(&arg->lib);
  vector_add(&arg->lib, (vtype)GWION_ADD_DIR);
  arg->ref = &arg->add;
}

void arg_release(Arg* arg) {
  vector_release(&arg->add);
  vector_release(&arg->rem);
  vector_release(&arg->lib);
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
#ifdef GWCOV
  { "coverage",   0, NULL, 'K' },
#endif
  /*  { "status"  , 0, NULL, '%' },*/
  { NULL,       0, NULL, 0   }
};

static const char usage[] =
"usage: Gwion <options>\n"
"\toption can be any of:\n"
"GLOBAL options:  <argument>  : description\n"
"\t--help,    -?\t             : this help\n"
"\t--version  -v\t             : this help\n"
"VM     options:\n"
"\t--add,     -+\t <file>      : add file\n"
"\t--rem,     --\t <shred id>  : remove shred\n"
"\t--plugdir, -P\t <directory> : add a plugin directory\n"
"\t--quit     -q\t             : quit the vm\n"
"\t--coverage -k\t             : enter coverage mode (if enabled)\n"
"UDP    options:\n"
"\t--host     -h\t  <string>   : set host\n"
"\t--port     -p\t  <number>   : set port\n"
"\t--loop     -l\t  <0 or 1>   : loop state (0 or 1)\n"
"\t--alone    -a\t             : standalone mode. (no udp)\n"
"DRIVER options:\n"
"\t--driver   -d\t  <string>   : set the driver (one of: alsa jack soundio portaudio file dummy silent raw)\n"
"\t--sr       -s\t  <number>   : set samplerate\n"
"\t--bufnum   -n\t  <number>   : set number of buffers\n"
"\t--bufsize  -b\t  <number>   : set size   of buffers\n"
"\t--chan     -g\t  <number>   : (global) channel number\n"
"\t--in       -i\t  <number>   : number of  input channel\n"
"\t--out      -o\t  <number>   : number of output channel\n"
"\t--card     -c\t  <string>   : card identifier or output file (depending on driver)\n"
"\t--raw      -r\t  <0 or 1>   : enable raw mode (file and soundio only)\n"
"\t--format   -f\t  <string>   : soundio format (one of: S8 U8 S16 U16 S24 U24 S32 U32 F32 F64)\n"
"\t--backend  -e\t  <string>   : soundio backend (one of: jack pulse alsa core wasapi dummy)\n";

static void arg_add(Arg* arg) {
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

static void arg_drvr(DriverInfo* di, int i) {
    switch(i) {
      case 'c':
        di->card     = optarg;
        break;
      case 'g':
        di->chan       = atoi(optarg);
        di->in       = atoi(optarg);
        di->out       = atoi(optarg);
        break;
      case 'i':
        di->in       = atoi(optarg);
        break;
      case 'o':
        di->out      = atoi(optarg);
        break;
      case 's':
        di->sr      = atoi(optarg);
        break;
      case 'd':
        select_driver(di, optarg);
        break;
      case 'f':
        select_format(di, optarg);
        break;
      case 'e':
        select_backend(di, optarg);
        break;
      case 'r':
        di->raw = 1;
        break;
      case 'n':
        di->bufnum    = atoi(optarg);
        break;
      case 'b':
        di->bufsize    = atoi(optarg);
        break;
    default:
      exit(1);
  }
}

static void arg_udp(UdpIf* udp, char c) {
  switch(c) {
    case 'a':
      udp->on = 0;
      break;
    case 'h':
      udp->host = optarg;
      break;
    case 'p':
      udp->port = atoi(optarg);
      break;
  }
}

void parse_args(Arg* arg, DriverInfo* di) {
  int i, index;
  while((i = getopt_long(arg->argc, arg->argv, "?vqh:p:i:o:n:b:e:s:d:al:g:-:rc:f:P:C:K ", long_option, &index)) != -1) {
    if(strchr("ahp", i))
      arg_udp(arg->udp, i);
    else switch(i) {
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
      case 'l':
        arg->loop = atoi(optarg) > 0 ? 1 : -1;
        break;
      case 'P':
        vector_add(&arg->lib, (vtype)optarg);
        break;
#ifdef GWCOV
      case 'K':
        arg->coverage = 1;
        break;
#endif
      default:
        arg_drvr(di, i);
    }
  }
  arg_add(arg);
}
