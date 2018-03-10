#include "stdlib.h"
#include "string.h"
#include "defs.h"
#include "symbol.h"
#define SIZE 65347  /* should be prime */

struct S_Symbol_ {
  m_str name;
  S_Symbol next;
};

static S_Symbol hashtable[SIZE];

__attribute__((nonnull(1)))
static S_Symbol mksymbol(const m_str name, const S_Symbol next) {
  S_Symbol s = malloc(sizeof(*s));
  s->name = strdup(name);
  s->next = next;
  return s;
}

ANN static void free_symbol(S_Symbol s) {
  if(s->next)
    free_symbol(s->next);
  free(s->name);
  free(s);
}

void free_symbols(void) {
  LOOP_OPTIM
  for(unsigned int i = SIZE + 1; --i;) {
    S_Symbol s = hashtable[i - 1];
    if(s)
      free_symbol(s);
  }
}

ANN static unsigned int hash(const char *s0) {
  unsigned int h = 0;
  const char *s;
  for(s = s0; *s; s++)
    h = h * 65599 + *s;
  return h;
}

ANN S_Symbol insert_symbol(const m_str name) {
  unsigned int index = hash(name) % SIZE;
  S_Symbol sym, syms = hashtable[index];

  LOOP_OPTIM
  for(sym = syms; sym; sym = sym->next)
    if(!strcmp(sym->name, (m_str)name))
      return sym;
  return hashtable[index] = mksymbol(name, syms);
}

ANN m_str s_name(const S_Symbol sym) {
  return sym->name;
}

