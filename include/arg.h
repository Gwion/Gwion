#ifndef __ARG
#define __ARG

typedef struct Arg_ {
  struct CArg    arg;
  struct Map_    mod;
  struct Vector_ add;
  struct Vector_ lib;
  struct Vector_ config;
  struct SoundInfo_ *si;
  m_bool loop;
} Arg;

ANN void arg_release(Arg*);
ANN m_bool arg_parse(const Gwion, Arg*);
ANN void arg_compile(const Gwion, Arg*);
#endif
