typedef struct {
  int argc;
  char** argv;
  m_str  host;
  int    port;
  m_uint quit;
  m_uint loop;
  struct Vector_ add;
  struct Vector_ rem;
  struct Vector_ lib;
  Vector ref;
} Arg;

void arg_init(Arg* arg);
void arg_release(Arg* arg);
