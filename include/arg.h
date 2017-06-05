#include <stdio.h>
#include "map.h"

typedef struct {
  void* scanner;
  Vector doc;
  char doc_str[4096];
  Ast ast;
  unsigned int line;
  unsigned int pos;
  FILE* file;
  m_str filename;
} MyArg;
