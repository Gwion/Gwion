#include <errno.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "soundinfo.h"
#include "vm.h"
#include "gwion.h"
#include "pass.h"
#include "compile.h"
#include "cmdapp.h"

#define GWIONRC ".gwionrc"

enum {
  CONFIG,
  PLUGIN,
  LOAD_PLUGIN,
  MODULE,
  LOOP,
  PASS,
  STDIN,
  COLOR,
  // sound options
  DRIVER,
  SRATE,
  NINPUT,
  NOUTPUT,
  // pp options
  DEFINE,
  UNDEF,
  INCLUDE,
  DEBUG,
  DUMP,
  CDOC,
  THREAD,
  QUEUE,
  NOPTIONS
};

/* use before MemPool allocation */
ANN static inline void config_end(const Vector config) {
  for (m_uint i = 0; i < vector_size(config); ++i) {
    const Vector v = (Vector)vector_at(config, i);
    for (m_uint j = 0; j < vector_size(v); ++j) xfree((m_str)vector_at(v, j));
    vector_release(v);
    xfree(v);
  }
}

ANN static m_str plug_dir(void) {
  const m_str  home     = getenv("HOME");
  const size_t sz       = strlen(home);
  const m_str  pdir     = "/.gwplug";
  m_str        plug_dir = (m_str)xmalloc(sz + strlen(pdir) + 1);
  strcpy(plug_dir, home);
  strcpy(plug_dir + sz, pdir);
  return plug_dir;
}

enum arg_type {
  ARG_FILE,
  ARG_STDIN,
  ARG_LOAD_PLUGIN,
  ARG_DEFINE,
  ARG_UNDEF,
  ARG_INCLUDE,
  ARG_DEBUG,
  ARG_DUMP,
  ARG_CDOC,
};

ANN static void arg_init(CliArg *arg) {
  map_init(&arg->mod);
  vector_init(&arg->add);
  vector_init(&arg->lib);
  vector_init(&arg->config);
  if(arg->ulib)vector_add(&arg->lib, (vtype)plug_dir());
  arg->color = COLOR_AUTO;
}

ANN void arg_release(CliArg *arg) {
  map_release(&arg->mod);
  vector_release(&arg->add);
  xfree((m_str)vector_front(&arg->lib));
  vector_release(&arg->lib);
  config_end(&arg->config);
  vector_release(&arg->config);
}

static inline bool str2bool(const char *str) {
  if (!str || !strcmp(str, "true")) return true;
  if (!strcmp(str, "false")) return false;
  char *rem = NULL;
  long opt = strtol(str, &rem, 10);
  if(rem || errno == EINVAL) {
    gw_err("invalid argument for boolean option, setting to `false`\n");
    return false;
  }
  return !!opt;
}

ANN static inline void get_debug(const Gwion gwion, const char *dbg) {
  const bool is_dbg = str2bool(dbg);
  gwion_set_debug(gwion, is_dbg);
}

ANN static inline void get_dump(const Gwion gwion, const char *dbg) {
  const bool is_dbg = str2bool(dbg);
  gwion_set_dump(gwion, is_dbg);
}

ANN static inline void get_cdoc(const Gwion gwion, const char *cdoc) {
  const bool is_cdoc = str2bool(cdoc);
  gwion_set_cdoc(gwion, is_cdoc);
}

ANN static char* gw_args(const char* base, Vector args) {
  m_str d = strdup(base);
  char *name = strsep(&d, ":");
  if (d) vector_init(args);
  while (d) vector_add(args, (vtype)strdup(strsep(&d, ":")));
  xfree(d);
  return name;
}

ANN static inline void load_plugin(const Gwion gwion, const char *plug_name) {
  char c[1024];
  sprintf(c, "#import %s\n", plug_name);
  const bool cdoc = gwion->data->cdoc;
  gwion->data->cdoc = true;
  compile_string(gwion, "<command-line>", c, NULL);
  gwion->data->cdoc = cdoc;
}

ANN void arg_compile(const Gwion gwion, CliArg *arg) {
  const Vector v = &arg->add;
  for (m_uint i = 0; i < vector_size(v); i++) {
    switch (vector_at(v, i)) {
    case ARG_FILE:
        // TODO: make arguments
      {
      struct Vector_ args = {};
      char *name = gw_args((m_str)VPTR(v, ++i), &args);
      compile_filename(gwion, name, args.ptr ? &args : NULL);
      xfree(name);
        }
      break;
    case ARG_STDIN:
      compile_file(gwion, "stdin", stdin, NULL);
      break;
    case ARG_LOAD_PLUGIN:
      load_plugin(gwion, (m_str)VPTR(v, ++i));
      break;
    case ARG_DEFINE:
      pparg_add(gwion->ppa, (m_str)VPTR(v, ++i));
      break;
    case ARG_UNDEF:
      pparg_rem(gwion->ppa, (m_str)VPTR(v, ++i));
      break;
    case ARG_INCLUDE:
      pparg_inc(gwion->ppa, (m_str)VPTR(v, ++i));
      break;
    case ARG_DEBUG:
      get_debug(gwion, (m_str)VPTR(v, ++i));
      break;
    case ARG_DUMP:
      get_debug(gwion, (m_str)VPTR(v, ++i));
      break;
    }
  }
}

ANN2(1) static inline void arg_set_pass(const Gwion gwion, const char *str) {
  const Vector v = split_args(gwion->mp, (const m_str)str);
  pass_set(gwion, v);
  for (m_uint i = 0; i < vector_size(v); ++i)
    free_mstr(gwion->mp, (m_str)vector_at(v, i));
  free_vector(gwion->mp, v);
}

ANN2(1) static void module_arg(const Map map, const char *str) {
  m_str val = strchr(str, '=');
  if (val) {
    *val = '\0';
    ++val;
  }
  map_set(map, (vtype)str, (vtype)val);
}

static void setup_options(cmdapp_t *app, cmdopt_t *opt) {
  cmdapp_set(app, 'C', "config", CMDOPT_TAKESARG, NULL, "parse a config file", "{Y}path{0}", 
             &opt[CONFIG]);
  cmdapp_set(app, 'p', "plugdir", CMDOPT_TAKESARG, NULL,
             "add ARG to the plugin search path", "directory", &opt[PLUGIN]);
  cmdapp_set(app, 'm', "module", CMDOPT_TAKESARG, NULL,
             "activate module", "arguments", &opt[MODULE]);
  cmdapp_set(app, 'l', "loop", CMDOPT_TAKESARG, NULL, "set loop mode", "boolean",
             &opt[LOOP]);
  cmdapp_set(app, 'g', "passes", CMDOPT_TAKESARG, NULL, "comma separted list of passes", "{B}passes{0}",
             &opt[PASS]);
  cmdapp_set(app, '\0', "stdin", 0, NULL, "read from stdin", NULL, &opt[STDIN]);
  cmdapp_set(app, 'c', "color", CMDOPT_TAKESARG, NULL,
             "when to use {R}c{G}o{B}l{Y}o{M}r{C}s{0} (defaults to auto)", "{+}never{-}/auto/always",
             &opt[COLOR]);
  // sound options
  cmdapp_set(app, 'd', "driver", CMDOPT_TAKESARG, NULL,
             "set driver (and arguments)", "arguments", &opt[DRIVER]);
  cmdapp_set(app, 's', "samplerate", CMDOPT_TAKESARG, NULL,
             "set the samplerate", "integer", &opt[SRATE]);
  cmdapp_set(app, 'i', "input", CMDOPT_TAKESARG, NULL,
             "number of input channel", "integer", &opt[NINPUT]);
  cmdapp_set(app, 'o', "output", CMDOPT_TAKESARG, NULL,
             "number of output channel", "integer", &opt[NOUTPUT]);
  cmdapp_set(app, 'P', "plugin", CMDOPT_TAKESARG, NULL, "(force-)load a plugin", "name",
             &opt[LOAD_PLUGIN]);
  cmdapp_set(app, 'D', "define", CMDOPT_TAKESARG, NULL, "define a macro", "macro",
             &opt[DEFINE]);
  cmdapp_set(app, 'U', "undef", CMDOPT_TAKESARG, NULL, "undefine a macro", "macro",
             &opt[UNDEF]);
  cmdapp_set(app, 'I', "include", CMDOPT_TAKESARG, NULL,
             "add ARG to include path", "directory", &opt[INCLUDE]);
  cmdapp_set(app, 'G', "debug", CMDOPT_MAYTAKEARG, NULL, "set/unset debug mode","bool",
             &opt[DEBUG]);
  cmdapp_set(app, 'B', "dump", CMDOPT_MAYTAKEARG, NULL, "set/unset bytecode dump", "bool", 
             &opt[DUMP]);
  cmdapp_set(app, 'H', "cdoc", CMDOPT_MAYTAKEARG, NULL, "set/unset cdoc mode", "bool",
             &opt[CDOC]);
  cmdapp_set(app, 't', "thread_count", CMDOPT_TAKESARG, NULL, "set number of threads", "integer",
             &opt[THREAD]);
  cmdapp_set(app, 'q', "queue_size", CMDOPT_TAKESARG, NULL, "set threadpool queue_size", "integer",
             &opt[QUEUE]);
}

static inline void add2arg(CliArg *const arg, const char *data,
                           const enum arg_type type) {
  vector_add(&arg->add, type);
  vector_add(&arg->add, (vtype)data);
}

ANN static void split_line(const m_str line, const Vector v) {
  m_str d = strdup(line), c = d;
  while (d) {
    const m_str  str = strsep(&d, " ");
    const size_t sz  = strlen(str);
    const bool arg = (str[sz - 1] == '\n');
    vector_add(v, (vtype)strndup(str, arg ? sz - 1 : sz));
  }
  xfree(d);
  xfree(c);
}

ANN static Vector get_config(const char *name) {
  char * line = NULL;
  size_t len  = 0;
  FILE * f    = fopen(name, "r");
  CHECK_O(f);
  const Vector v = (Vector)xmalloc(sizeof(struct Vector_));
  vector_init(v);
  while (getline(&line, &len, f) != -1) {
    if (line[0] != '#') split_line(line, v);
  }
  free(line);
  fclose(f);
  return v;
}

struct ArgInternal {
  const Gwion gwion;
  CliArg     *arg;
};

ANN bool _arg_parse(struct ArgInternal *arg);

ANN static void config_parse(struct ArgInternal *arg, const char *name) {
  const Vector v = get_config(name);
  if (v) {
    struct CArg ca     = arg->arg->arg;
    arg->arg->arg.argc = vector_size(v);
    arg->arg->arg.argv = (m_str *)(v->ptr + OFFSET);
    _arg_parse(arg);
    arg->arg->arg = ca;
    vector_add(&arg->arg->config, (vtype)v);
  }
}

#define ARG2INT(a) strtol(a, NULL, 10)

static void myproc(void *data, cmdopt_t *option, const char *arg) {
  struct ArgInternal *arg_int = data;
  CliArg *            _arg    = arg_int->arg;
  if (arg) {
    if (!_arg->arg.idx)
      _arg->arg.idx++;
    else
      add2arg(_arg, arg, ARG_FILE);
  } else {
    switch (option->shorto) {
    case 'p':
      vector_add(&_arg->lib, (vtype)option->value);
      break;
    case 'm':
      module_arg(&_arg->mod, option->value);
      break;
    case 'C':
      config_parse(arg_int, option->value);
      break;
    case 'l':
      _arg->loop = ARG2INT(option->value) > 0 ? true : false;
      break;
    case 'g':
      arg_set_pass(arg_int->gwion, option->value);
      break;
    case '\0':
      vector_add(&_arg->add, (vtype)ARG_STDIN);
      break;
    case 'P':
      vector_add(&_arg->add, (vtype)ARG_LOAD_PLUGIN);
      vector_add(&_arg->add, (vtype)option->value);
      break;
    case 'c':
      if (!strcmp(option->value, "never"))
        _arg->color = COLOR_NEVER;
      else if (!strcmp(option->value, "auto"))
        _arg->color = COLOR_AUTO;
      else if (!strcmp(option->value, "always"))
        _arg->color = COLOR_ALWAYS;
      // ignore error silently
      break;
      // sound options
    case 's':
      _arg->si->sr = (uint32_t)ARG2INT(option->value);
      break;
    case 'd':
      _arg->si->arg = (m_str)option->value;
      break;
    case 'i':
      _arg->si->in = (uint8_t)ARG2INT(option->value);
      break;
    case 'o':
      _arg->si->out = (uint8_t)ARG2INT(option->value);
      break;
      // pp options
    case 'D':
      add2arg(_arg, option->value, ARG_DEFINE);
      break;
    case 'U':
      add2arg(_arg, option->value, ARG_UNDEF);
      break;
    case 'I':
      add2arg(_arg, option->value, ARG_INCLUDE);
      break;
      // debug
    case 'G':
      add2arg(_arg, option->value, ARG_DEBUG);
      break;
    case 'B':
      get_dump(arg_int->gwion, option->value);
      break;
    case 'H':
      get_cdoc(arg_int->gwion, option->value);
      break;
    case 't':
      _arg->thread_count = (uint32_t)ARG2INT(option->value);
      break;
    case 'q':
      _arg->queue_size = (uint32_t)ARG2INT(option->value);
      break;
    }
  }
}

#ifndef GWION_VERSION
#define GWION_VERSION "N.A."
#endif

ANN bool _arg_parse(struct ArgInternal *arg) {
  cmdapp_t            app;
  const cmdapp_info_t info = {
      .program         = "gwion",
      .synopses        = NULL, // so it's automatic
      .version         = GWION_VERSION,
      .author          = "Jérémie Astor",
      .year            = 2016,
      .description     = "Strongly timed musical programming language.",
      .help_des_offset = 32,
      .ver_extra =
          "License GPLv3+: GNU GPL version 3 or later "
          "<https://gnu.org/licenses/gpl.html>\n"
          "This is free software: you are free to change and redistribute it.\n"
          "There is NO WARRANTY, to the extent permitted by law.\n"};
  struct CArg *ca = &arg->arg->arg;
  cmdapp_init(&app, ca->argc, ca->argv, CMDAPP_MODE_SHORTARG, &info);
  cmdapp_enable_procedure(&app, myproc, arg);
  cmdopt_t opt[NOPTIONS];
  setup_options(&app, opt);
  if (cmdapp_run(&app) == EXIT_SUCCESS && cmdapp_should_exit(&app))
    arg->arg->quit = 1;
  cmdapp_destroy(&app);
  return true;
}

ANN static void config_default(struct ArgInternal *arg) {
  char *home = getenv("HOME");
  char  c[strlen(home) + strlen(GWIONRC) + 2];
  sprintf(c, "%s/%s", home, GWIONRC);
  config_parse(arg, c);
}

ANN bool arg_parse(const Gwion gwion, CliArg *a) {
  if(!a->uargs) a->arg.argc = 1;
  if(a->config_args) a->arg.argv = a->config_args(&a->arg.argc, a->arg.argv);
  struct ArgInternal arg = {.gwion = gwion, .arg = a};
  arg_init(a);
#ifdef __FUZZING
  return;
#endif
  if(a->urc) config_default(&arg);
  return _arg_parse(&arg);
}
