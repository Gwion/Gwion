#include <string.h>
#include "gwion_util.h"
#include "arg.h"
#include "soundinfo.h"
#define GWIONRC ".gwionrc"

ANN static inline void config_end(MemPool p, const Vector config) {
  for(m_uint i = 0; i < vector_size(config); ++i) {
    const Vector v = (Vector)vector_at(config, i);
    free_mstr(p, (m_str)vector_front(v));
    free_vector(p, v);
  }
}

ANN static m_str plug_dir(MemPool p) {
  const m_str home = getenv("HOME");
  const size_t sz = strlen(home);
  const m_str pdir = "/.gwplug";
  m_str plug_dir = (m_str)_mp_malloc(p, sz + strlen(pdir) + 1);
  strcpy(plug_dir, home);
  strcpy(plug_dir + sz, pdir);
  return plug_dir;
}

ANN static void arg_init(MemPool p, Arg* arg) {
  vector_init(&arg->add);
  vector_init(&arg->lib);
  vector_init(&arg->mod);
  vector_init(&arg->config);
  vector_add(&arg->lib, (vtype)plug_dir(p));
}

ANN void arg_release(MemPool p, Arg* arg) {
  vector_release(&arg->add);
  free_mstr(p, (m_str)vector_front(&arg->lib));
  vector_release(&arg->lib);
  vector_release(&arg->mod);
  config_end(p, &arg->config);
  vector_release(&arg->config);
}

static const char usage[] =
"usage: Gwion <options>\n"
"  -h              : this help\n"
"  -k              : show compilation flags\n"
"  -c     <file>   : load config\n"
"  -p     <path>   : add a plugin directory\n"
"  -s    <number>  : set samplerate\n"
"  -i    <number>  : set input channel number\n"
"  -o    <number>  : set output channel number\n"
"  -d    <number>  : set driver (and arguments)\n"
"  -z    <number>  : set memoization limit\n"
"  -m   <mod:args> : load module (and arguments)\n";

ANN static void config_parse(MemPool p, Arg* arg, const m_str name);

#define CASE(a,b) case a: (b) ; break;
#define get_arg(a) (a[i][2] == '\0' ? arg->argv[++i] : a[i] + 2)
#define ARG2INT(a) strtol(get_arg(a), NULL, 10)

ANN void _arg_parse(MemPool p, Arg* arg) {
  for(int i = 1; i < arg->argc; ++i) {
    if(arg->argv[i][0] == '-') {
      switch(arg->argv[i][1]) {
        CASE('h', gw_err(usage))
        CASE('k', gw_err("CFLAGS: %s\nLDFLAGS: %s\n", CFLAGS, LDFLAGS))
        CASE('c', config_parse(p, arg, get_arg(arg->argv)))
        CASE('p', vector_add(&arg->lib, (vtype)get_arg(arg->argv)))
        CASE('m', vector_add(&arg->mod, (vtype)get_arg(arg->argv)))
        CASE('l', arg->loop = (m_bool)ARG2INT(arg->argv) > 0 ? 1 : -1)
        CASE('z', arg->memoize = (uint32_t)ARG2INT(arg->argv))
        CASE('i', arg->si->in  = (uint8_t)ARG2INT(arg->argv))
        CASE('o', arg->si->out = (uint8_t)ARG2INT(arg->argv))
        CASE('s', arg->si->sr = (uint32_t)ARG2INT(arg->argv))
        CASE('d', arg->si->arg = get_arg(arg->argv))
      }
    } else
      vector_add(&arg->add, (vtype)arg->argv[i]);
  }
}

ANN static void split_line(MemPool p, const m_str line, const Vector v) {
  m_str d = mstrdup(p, line);
  while(d) {
    const m_str str = strsep(&d, " ");
    const size_t sz = strlen(str);
    if(str[sz-1] == '\n')
      str[sz-1] = '\0';
    vector_add(v, (vtype)str);
  }
}

ANN static Vector get_config(MemPool p, const m_str name) {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  FILE *f = fopen(name, "r");
  CHECK_OO(f)
  const Vector v = new_vector(p);
  vector_add(v, (vtype)name);
  while((nread = getline(&line, &len, f)) != -1) {
    if(line[0] != '#')
      split_line(p, line, v);
  }
  free(line);
  fclose(f);
  return v;
}

ANN static void config_parse(MemPool p, Arg* arg, const m_str name) {
  const Vector v = get_config(p, name);
  if(v) {
    int argc = arg->argc;
    char** argv = arg->argv;
    arg->argc = vector_size(v);
    arg->argv =  (m_str*)(v->ptr + OFFSET);
    _arg_parse(p, arg);
    arg->argc = argc;
    arg->argv = argv;
    vector_add(&arg->config, (vtype)v);
  }
}

ANN static void config_default(MemPool p, Arg* arg) {
  char* home = getenv("HOME");
  char c[strlen(home) + strlen(GWIONRC) + 2];
  sprintf(c, "%s/%s", home, GWIONRC);
  config_parse(p, arg, c);
}

ANN void arg_parse(MemPool p, Arg* a) {
  arg_init(p, a);
  config_default(p, a);
  _arg_parse(p, a);
}
