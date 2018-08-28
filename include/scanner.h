#include <setjmp.h>
typedef struct Scanner_ {
  void* scanner;
  Ast ast;
  unsigned int line;
  unsigned int pos;
#ifndef TINY_MODE
  struct Vector_ filename;
  struct Hash_ macros;
  struct Macro_* entry;
  struct pp_info def;
  size_t arg_len, arg_cap;
  int npar;
#ifdef LINT_MODE
  unsigned lint : 1;
#endif
#else
  m_str filename;
#endif
  jmp_buf jmp;
} Scanner;

ANN Ast parse(struct Scanner_*, const m_str, FILE*);
ANEW Scanner* new_scanner(const m_uint size);
ANN void free_scanner(Scanner* scan);
ANN void scanner_post(Scanner* scan);
int clear_buffer(Vector, void*, const m_bool);
