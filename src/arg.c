#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "soundinfo.h"
#include "vm.h"
#include "gwion.h"
#include "arg.h"
#include "pass.h"

#define GWIONRC ".gwionrc"

/* use before MemPool allocation */
ANN static inline void config_end(const Vector config) {
  for(m_uint i = 0; i < vector_size(config); ++i) {
    const Vector v = (Vector)vector_at(config, i);
    for(m_uint i = 1; i < vector_size(v); ++i)
      xfree((m_str)vector_at(v, i));
    vector_release(v);
    xfree(v);
  }
}

ANN static m_str plug_dir(void) {
  const m_str home = getenv("HOME");
  const size_t sz = strlen(home);
  const m_str pdir = "/.gwplug";
  m_str plug_dir = (m_str)xmalloc(sz + strlen(pdir) + 1);
  strcpy(plug_dir, home);
  strcpy(plug_dir + sz, pdir);
  return plug_dir;
}

ANN static void arg_init(Arg* arg) {
  vector_init(&arg->add);
  vector_init(&arg->lib);
  vector_init(&arg->mod);
  vector_init(&arg->config);
  vector_add(&arg->lib, (vtype)plug_dir());
}

ANN void arg_release(Arg* arg) {
  vector_release(&arg->add);
  xfree((m_str)vector_front(&arg->lib));
  vector_release(&arg->lib);
  vector_release(&arg->mod);
  config_end(&arg->config);
  vector_release(&arg->config);
}

static const char usage[] =
"usage: Gwion <options>\n"
"  -h              : this help\n"
"  -c     <file>   : load config\n"
"  -p     <path>   : add a plugin directory\n"
"  -s    <number>  : set samplerate\n"
"  -i    <number>  : set input channel number\n"
"  -o    <number>  : set output channel number\n"
"  -d    <number>  : set driver (and arguments)\n"
"  -l    <number>  : set loop mode\n"
"  -z    <number>  : set memoization limit\n"
"  -m   <mod:args> : load module (and arguments)\n"
"  -g   <mod:args> : set Gwion compiler passes order\n";

ANN static void config_parse(const Gwion, Arg*, const m_str);

#define ARG2INT(a) strtol(a, NULL, 10)

ANN2(1) static inline void arg_set_pass(const Gwion gwion, const m_str str) {
  const Vector v = split_args(gwion->mp, str);
  pass_set(gwion, v);
  for(m_uint i = 0; i < vector_size(v); ++i)
    free_mstr(gwion->mp, (m_str)vector_at(v, i));
  free_vector(gwion->mp, v);
}

ANN static inline m_str _get_arg(Arg* arg, int *i) {
  const char key = arg->argv[*i][1];
  const m_str str = (arg->argv[*i][2] == '\0' ? arg->argv[++(*i)] : arg->argv[*i] + 2);
  if(!str)
    gw_err(_("option '-%c' needs arguments\n"), key);
  return str;
}

ANN m_bool _arg_parse(const Gwion gwion, Arg* arg) {
  for(int i = 1; i < arg->argc; ++i) {
    if(arg->argv[i][0] == '-') {
      m_str tmp;
      switch(arg->argv[i][1]) {
        case 'h':
          gw_err(usage);
          break;
        case 'c':
          CHECK_OB((tmp = _get_arg(arg, &i)))
          config_parse(gwion, arg, tmp);
          break;
        case 'p':
          CHECK_OB((tmp = _get_arg(arg, &i)))
          vector_add(&arg->lib, (vtype)tmp);
          break;
        case 'm':
          CHECK_OB((tmp = _get_arg(arg, &i)))
          vector_add(&arg->mod, (vtype)tmp);
          break;
        case 'l':
          CHECK_OB((tmp = _get_arg(arg, &i)))
          arg->loop = (m_bool)ARG2INT(tmp) > 0 ? 1 : -1;
          break;
        case 'z':
          CHECK_OB((tmp = _get_arg(arg, &i)))
          arg->memoize = (uint32_t)ARG2INT(tmp);
          break;
        case 'i':
          CHECK_OB((tmp = _get_arg(arg, &i)))
          arg->si->in = (uint8_t)ARG2INT(tmp);
          break;
        case 'o':
          CHECK_OB((tmp = _get_arg(arg, &i)))
          arg->si->out = (uint8_t)ARG2INT(tmp);
          break;
        case 's':
          CHECK_OB((tmp = _get_arg(arg, &i)))
          arg->si->sr = (uint32_t)ARG2INT(tmp);
          break;
        case 'd':
          CHECK_OB((tmp = _get_arg(arg, &i)))
          arg->si->arg = tmp;
          break;
        case 'g':
          CHECK_OB((tmp = _get_arg(arg, &i)))
          arg_set_pass(gwion, tmp);
          break;
        default:
          gw_err(_("invalid arguments"));
          return GW_ERROR;
      }
    } else
      vector_add(&arg->add, (vtype)arg->argv[i]);
  }
  return GW_OK;
}

ANN static void split_line(const m_str line, const Vector v) {
  m_str d = strdup(line), c = d;
  while(d) {
    const m_str str = strsep(&d, " ");
    const size_t sz = strlen(str);
    const m_bool arg = (str[sz-1] == '\n');
    vector_add(v, (vtype)strndup(str, arg ? sz - 1 : sz));
  }
  free(d);
  free(c);
}

ANN static Vector get_config(const m_str name) {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  FILE *f = fopen(name, "r");
  CHECK_OO(f)
  const Vector v = (Vector)xmalloc(sizeof(struct Vector_));
  vector_init(v);
  vector_add(v, (vtype)name);
  while((nread = getline(&line, &len, f)) != -1) {
    if(line[0] != '#')
      split_line(line, v);
  }
  free(line);
  fclose(f);
  return v;
}

ANN static void config_parse(const Gwion gwion, Arg* arg, const m_str name) {
  const Vector v = get_config(name);
  if(v) {
    int argc = arg->argc;
    char** argv = arg->argv;
    arg->argc = vector_size(v);
    arg->argv =  (m_str*)(v->ptr + OFFSET);
    _arg_parse(gwion, arg);
    arg->argc = argc;
    arg->argv = argv;
    vector_add(&arg->config, (vtype)v);
  }
}

ANN static void config_default(const Gwion gwion , Arg* arg) {
  char* home = getenv("HOME");
  char c[strlen(home) + strlen(GWIONRC) + 2];
  sprintf(c, "%s/%s", home, GWIONRC);
  config_parse(gwion, arg, c);
}

ANN m_bool arg_parse(const Gwion gwion, Arg* a) {
  arg_init(a);
  config_default(gwion, a);
  return _arg_parse(gwion, a);
}
