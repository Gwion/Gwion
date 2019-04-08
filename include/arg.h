#ifndef __ARG
#define __ARG

typedef struct Arg_ {
  int argc;
  char** argv;
  struct Vector_ add;
  struct Vector_ lib;
  struct Vector_ mod;
  struct Vector_ config;
  struct SoundInfo_ *si;
  m_bool loop;
  m_bool memoize;
} Arg;

ANN void arg_release(Arg*);
ANN void arg_parse(Arg*);
#endif
