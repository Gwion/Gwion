typedef struct {
  m_str  host;
  int    port;
  m_bool on;
// VM* vm
} UdpIf;

typedef struct {
  int argc;
  char** argv;
  m_int loop;
  m_uint quit;
  struct Vector_ add;
  struct Vector_ rem;
  struct Vector_ lib;
  Vector ref;
  UdpIf* udp;
#ifdef GWCOV
  m_bool coverage;
#endif
#ifdef GWCGRAPH
  m_bool profile;
#endif
#ifdef GWREPL
  m_bool repl;
#endif
} Arg;

void arg_init(Arg* arg);
void arg_release(Arg* arg);
