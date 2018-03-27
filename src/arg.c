#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "defs.h"
#include "map.h"
#include "err_msg.h"
#include "map_private.h"
#include "arg.h"
#include "vm.h"
#include "driver.h"

ANN void arg_init(Arg* arg) {
  vector_init(&arg->add);
  vector_init(&arg->rem);
  vector_init(&arg->lib);
  vector_add(&arg->lib, (vtype)GWPLUG_DIR);
  arg->ref = &arg->add;
}

ANN void arg_release(Arg* arg) {
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
  { "coverage", 0, NULL, 'K' },
#endif
#ifdef GWCGRAPH
  { "profile",  0, NULL, 'G' },
#endif
#ifdef GWREPL
  { "repl",  0, NULL, 'R' },
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
"\t--profile  -G\t             : enter profile mode (if enabled)\n"
"\t--coverage -k\t             : enter coverage mode (if enabled)\n"
"\t--repl     -R\t             : enter repl  mode (if enabled)\n"
"UDP    options (if enabled):\n"
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

ANN static void arg_add(Arg* arg) {
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

ANN static void arg_drvr(DriverInfo* di, const int i) {
    switch(i) {
      case 'c':
        di->card     = optarg;
        break;
      case 'g':
        di->chan     = strtol(optarg, NULL, 10);
        di->in       = strtol(optarg, NULL, 10);
        di->out      = strtol(optarg, NULL, 10);
        break;
      case 'i':
        di->in       = strtol(optarg, NULL, 10);
        break;
      case 'o':
        di->out      = strtol(optarg, NULL, 10);
        break;
      case 's':
        di->sr       = strtol(optarg, NULL, 10);
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
        di->bufnum    = strtol(optarg, NULL, 10);
        break;
      case 'b':
        di->bufsize    = strtol(optarg, NULL, 10);
        break;
    default:
      gw_err("Unknown argument '%c'\n", i);
  }
}

#ifdef GWUDP
ANN static void arg_udp(UdpIf* udp, const char c) {
  switch(c) {
    case 'a':
      udp->on = 0;
      break;
    case 'h':
      udp->host = optarg;
      break;
    case 'p':
      udp->port = strtol(optarg, NULL, 10);
      break;
  }
}
#endif

ANN void parse_args(Arg* arg, DriverInfo* di) {
  int i, index;
  while((i = getopt_long(arg->argc, arg->argv, "?vqh:p:i:o:n:b:e:s:d:al:g:-:rc:f:P:CKGR ", long_option, &index)) != -1) {
#ifdef GWUDP
    if(strchr("ahp", i))
      arg_udp(arg->udp, i);
    else
#endif
    switch(i) {
      case '?':
        gw_err(usage);
        exit(0);
      case 'C':
        gw_err("CFLAGS: %s\nLDFLAGS: %s\n", CFLAGS, LDFLAGS);
        exit(1);
        break;
      case 'q':
        arg->quit  = 1;
        break;
      case 'l':
        arg->loop = strtol(optarg, NULL, 10) > 0 ? 1 : -1;
        break;
      case 'P':
        vector_add(&arg->lib, (vtype)optarg);
        break;
#ifdef GWCOV
      case 'K':
        arg->coverage = 1;
        break;
#endif
#ifdef GWCGRAPH
      case 'G':
        arg->profile = 1;
        break;
#endif
#ifdef GWREPL
      case 'R':
        arg->repl = 1;
        break;
#endif
      default:
        arg_drvr(di, i);
    }
  }
  arg_add(arg);
}
