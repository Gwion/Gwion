#ifndef __ARG
#define __ARG
typedef struct {
  int argc;
  char** argv;
  struct Vector_ add;
  struct Vector_ rem;
  struct Vector_ lib;
  struct Vector_ mod;
  m_bool loop;
} Arg;

ANN void arg_release(Arg* arg);
ANN uint parse_args(Arg*, DriverInfo*);
#endif
