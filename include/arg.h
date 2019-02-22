#ifndef __ARG
#define __ARG
typedef struct {
  int argc;
  char** argv;
  struct Vector_ add;
  struct Vector_ rem;
  struct Vector_ lib;
  struct Vector_ mod;
  Vector ref;
  m_bool loop;
  unsigned quit : 1;
} Arg;

ANN void arg_init(Arg* arg);
ANN void arg_release(Arg* arg);
#endif
