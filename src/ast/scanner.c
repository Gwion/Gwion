#include "absyn.h"
#include "hash.h"
#include "scanner.h"
#include "parser.h"
#include "lexer.h"

ANEW Scanner* new_scanner(const m_uint size) {
  Scanner* scan = (Scanner*)xcalloc(1, sizeof(Scanner));
  gwion_lex_init(&scan->scanner);
  gwion_set_extra(scan, scan->scanner);
#ifndef TINY_MODE
#ifdef LINT_MODE
  scan->lint = 0;
#endif
  hini(&scan->macros, size);
  vector_init(&scan->filename);
#endif
  return scan;
}

ANN void free_scanner(Scanner* scan) {
#ifndef TINY_MODE
  vector_release(&scan->filename);
  hend(&scan->macros);
#endif
  gwion_lex_destroy(scan->scanner);
  xfree(scan);
}

static inline void scanner_pre(Scanner* scan, const m_str filename, FILE* f) {
  scan->line = 1;
  scan->pos  = 1;
#ifndef TINY_MODE
  scan->def.idx = 0;
  scan->npar = 0;
  vector_add(&scan->filename, (vtype)NULL);
  vector_add(&scan->filename, (vtype)filename);
#else
  scan->filename = filename;
#endif
  gwion_set_in(f, scan->scanner);
}

#ifndef TINY_MODE
void scanner_post(Scanner* scan) {
  m_uint size = vector_size(&scan->filename);
  while(size > 2)
    size = clear_buffer(&scan->filename, scan, size > 6);
  scan->entry = NULL;
  vector_clear(&scan->filename);
  hdel(&scan->macros);
}
#else
#define scanner_post(a)
#endif

Ast parse(Scanner* scan, const m_str filename, FILE* f) {
  scanner_pre(scan, filename, f);
  if(setjmp(scan->jmp) || gwion_parse(scan))
    scan->ast = NULL;
  scanner_post(scan);
  return scan->ast;
}
