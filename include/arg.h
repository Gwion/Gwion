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
} Arg;

void arg_init(Arg* arg);
void arg_release(Arg* arg);
