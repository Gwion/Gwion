#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "soundinfo.h"
#include "vm.h"
#include "gwion.h"
#include "arg.h"
#include "pass.h"
#include "compile.h"
#include "cmdapp.h"

#define GWIONRC ".gwionrc"

enum {
  CONFIG,
  PLUGIN,
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
  NOPTIONS
};

/* use before MemPool allocation */
ANN static inline void config_end(const Vector config) {
  for (m_uint i = 0; i < vector_size(config); ++i) {
    const Vector v = (Vector)vector_at(config, i);
    for (m_uint i = 1; i < vector_size(v); ++i) xfree((m_str)vector_at(v, i));
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
  ARG_DEFINE,
  ARG_UNDEF,
  ARG_INCLUDE,
  ARG_DEBUG,
  ARG_DUMP,
  ARG_CDOC,
};

ANN static void arg_init(Arg *arg) {
  map_init(&arg->mod);
  vector_init(&arg->add);
  vector_init(&arg->lib);
  vector_init(&arg->config);
  vector_add(&arg->lib, (vtype)plug_dir());
  arg->color = COLOR_AUTO;
}

ANN void arg_release(Arg *arg) {
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
  return atoi(str) ? true : false;
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

ANN void arg_compile(const Gwion gwion, Arg *arg) {
  const Vector v = &arg->add;
  for (m_uint i = 0; i < vector_size(v); i++) {
    switch (vector_at(v, i)) {
    case ARG_FILE:
      compile_filename(gwion, (m_str)VPTR(v, ++i));
      break;
    case ARG_STDIN:
      compile_file(gwion, "stdin", stdin);
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
  cmdapp_set(app, 'C', "config", CMDOPT_TAKESARG, NULL, "parse a config file",
             &opt[CONFIG]);
  cmdapp_set(app, 'p', "plugdir", CMDOPT_TAKESARG, NULL,
             "add ARG to the plugin search path", &opt[PLUGIN]);
  cmdapp_set(app, 'm', "module", CMDOPT_TAKESARG, NULL,
             "activate module (and arguments)", &opt[MODULE]);
  cmdapp_set(app, 'l', "loop", CMDOPT_TAKESARG, NULL, "set loop mode",
             &opt[LOOP]);
  cmdapp_set(app, 'g', "passes", CMDOPT_TAKESARG, NULL, "set pass order",
             &opt[PASS]);
  cmdapp_set(app, '\0', "stdin", 0, NULL, "read from stdin", &opt[STDIN]);
  cmdapp_set(app, 'c', "color", CMDOPT_TAKESARG, NULL,
             "set colored output (never/auto/always, defaults to auto)",
             &opt[COLOR]);
  // sound options
  cmdapp_set(app, 'd', "driver", CMDOPT_TAKESARG, NULL,
             "set driver (and arguments)", &opt[DRIVER]);
  cmdapp_set(app, 's', "samplerate", CMDOPT_TAKESARG, NULL,
             "set the samplerate", &opt[SRATE]);
  cmdapp_set(app, 'i', "input", CMDOPT_TAKESARG, NULL,
             "number of input channel", &opt[NINPUT]);
  cmdapp_set(app, 'o', "output", CMDOPT_TAKESARG, NULL,
             "number of output channel", &opt[NOUTPUT]);
  cmdapp_set(app, 'D', "define", CMDOPT_TAKESARG, NULL, "define a macro",
             &opt[DEFINE]);
  cmdapp_set(app, 'U', "undef", CMDOPT_TAKESARG, NULL, "undefine a macro",
             &opt[UNDEF]);
  cmdapp_set(app, 'I', "include", CMDOPT_TAKESARG, NULL,
             "add ARG to include path", &opt[INCLUDE]);
  cmdapp_set(app, 'G', "debug", CMDOPT_MAYTAKEARG, NULL, "set/unset debug mode",
             &opt[DEBUG]);
  cmdapp_set(app, 'B', "dump", CMDOPT_MAYTAKEARG, NULL, "set/unset bytecode dump",
             &opt[DUMP]);
  cmdapp_set(app, 'H', "cdoc", CMDOPT_MAYTAKEARG, NULL, "set/unset cdoc mode",
             &opt[CDOC]);
}

static inline void add2arg(Arg *const arg, const char *data,
                           const enum arg_type type) {
  vector_add(&arg->add, type);
  vector_add(&arg->add, (vtype)data);
}

ANN static void split_line(const m_str line, const Vector v) {
  m_str d = strdup(line), c = d;
  while (d) {
    const m_str  str = strsep(&d, " ");
    const size_t sz  = strlen(str);
    const m_bool arg = (str[sz - 1] == '\n');
    vector_add(v, (vtype)strndup(str, arg ? sz - 1 : sz));
  }
  xfree(d);
  xfree(c);
}

ANN static Vector get_config(const char *name) {
  char * line = NULL;
  size_t len  = 0;
  FILE * f    = fopen(name, "r");
  CHECK_OO(f);
  const Vector v = (Vector)xmalloc(sizeof(struct Vector_));
  vector_init(v);
  vector_add(v, (vtype)name);
  while (getline(&line, &len, f) != -1) {
    if (line[0] != '#') split_line(line, v);
  }
  free(line);
  fclose(f);
  return v;
}

struct ArgInternal {
  const Gwion gwion;
  Arg *       arg;
};

ANN m_bool _arg_parse(struct ArgInternal *arg);

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
  Arg *               _arg    = arg_int->arg;
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
    }
  }
}

#ifndef GWION_VERSION
#define GWION_VERSION "N.A."
#endif

ANN m_bool _arg_parse(struct ArgInternal *arg) {
  cmdapp_t            app;
  const cmdapp_info_t info = {
      .program         = "gwion",
      .synopses        = NULL, // so it's automatic
      .version         = GWION_VERSION,
      .author          = "Jérémie Astor",
      .year            = 2016,
      .description     = "Strongly timed musical programming language.",
      .help_des_offset = 28,
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
  return GW_OK;
}

ANN static void config_default(struct ArgInternal *arg) {
  char *home = getenv("HOME");
  char  c[strlen(home) + strlen(GWIONRC) + 2];
  sprintf(c, "%s/%s", home, GWIONRC);
  config_parse(arg, c);
}

ANN m_bool arg_parse(const Gwion gwion, Arg *a) {
  struct ArgInternal arg = {.gwion = gwion, .arg = a};
  arg_init(a);
#ifdef __FUZZING
  return;
#endif
  config_default(&arg);
  return _arg_parse(&arg);
}
