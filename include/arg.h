#ifdef GWUDP
typedef struct {
  m_str  host;
  int    port;
  m_bool on;
} UdpIf;
#endif
typedef struct {
  int argc;
  char** argv;
  m_int loop;
  m_uint quit;
  struct Vector_ add;
  struct Vector_ rem;
  struct Vector_ lib;
  Vector ref;
#ifdef GWUDP
  UdpIf* udp;
#endif
#ifdef GWCOV
  m_bool coverage;
#endif
#ifdef GWREPL
  m_bool repl;
#endif
} Arg;

ANN void arg_init(Arg* arg);
ANN void arg_release(Arg* arg);
