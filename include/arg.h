#ifndef __ARG
#define __ARG

typedef struct Arg_ {
  int argc;
  char** argv;
  struct Vector_ add;
  struct Vector_ lib;
  struct Vector_ mod;
  m_bool loop;
} Arg;

ANN void arg_release(Arg*);
ANN void arg_parse(Arg*, DriverInfo*);
#endif
