#include <stdio.h>
#include "map.h"

typedef struct {
  void* scanner;
  Ast ast;
  unsigned int line;
  unsigned int pos;
  unsigned int comment_depth;
  FILE* file;
  m_str filename;
} Scanner;
