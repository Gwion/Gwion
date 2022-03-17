#ifndef __ARG
#define __ARG

enum COLOR {
  COLOR_NEVER,
  COLOR_AUTO,
  COLOR_ALWAYS,
} __attribute__((packed));

typedef struct CliArg_ {
  struct CArg        arg;
  struct Map_        mod;
  struct Vector_     add;
  struct Vector_     lib;
  struct Vector_     config;
  struct SoundInfo_ *si;
  bool               loop;
  bool               quit;
  enum COLOR         color;
} CliArg;

ANN void   arg_release(CliArg *);
ANN m_bool arg_parse(const Gwion, CliArg *);
ANN void   arg_compile(const Gwion, CliArg *);
#endif
