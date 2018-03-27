#include <setjmp.h>
typedef struct {
  void* scanner;
  Ast ast;
  unsigned int line;
  unsigned int pos;
  FILE* file;
  m_str filename;
  jmp_buf jmp;
} Scanner;
