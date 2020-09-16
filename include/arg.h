#ifndef __ARG
#define __ARG

typedef struct Arg_ {
  struct CArg    arg;
  struct Vector_ add;
  struct Vector_ lib;
  struct Vector_ mod;
  struct Vector_ config;
  struct SoundInfo_ *si;
  m_bool loop;
  m_bool stdin;
} Arg;

ANN void arg_release(Arg*);
ANN m_bool arg_parse(const Gwion, Arg*);
#endif
