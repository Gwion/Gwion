#ifndef __ARG
#define __ARG
typedef struct {
  int argc;
  char** argv;
  m_uint quit;
  struct Vector_ add;
  struct Vector_ rem;
  struct Vector_ lib;
  Vector ref;
  m_bool loop;
} Arg;

ANN void arg_init(Arg* arg);
ANN void arg_release(Arg* arg);
#endif
