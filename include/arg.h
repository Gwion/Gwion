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
  char** (*config_args)(int*, char**);
  void  (*embed_libs)(Gwion);
  void  (*embed_scripts)(Gwion);
  bool               loop;
  bool               quit;
  bool               urc;
  bool               ulib;
  bool               uargs;
  enum COLOR         color;
} CliArg;

ANN void   arg_release(CliArg *);
ANN m_bool arg_parse(const Gwion, CliArg *);
ANN void   arg_compile(const Gwion, CliArg *);
#endif
