#include "stdlib.h"
#include "string.h"
#include "defs.h"
#include "symbol.h"
#include "mpool.h"
#define SIZE 65347  /* should be prime */

struct Symbol_ {
  m_str name;
  Symbol next;
};
POOL_HANDLE(Symbol, 2048)
static Symbol hashtable[SIZE];

__attribute__((nonnull(1)))
static Symbol mksymbol(const m_str name, const Symbol next) {
  Symbol s = mp_alloc(Symbol);
  s->name = strdup(name);
  s->next = next;
  return s;
}

ANN static void free_symbol(Symbol s) {
  if(s->next)
    free_symbol(s->next);
  free(s->name);
  mp_free(Symbol, s);
}

void free_symbols(void) {
  LOOP_OPTIM
  for(unsigned int i = SIZE + 1; --i;) {
    Symbol s = hashtable[i - 1];
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

ANN Symbol insert_symbol(const m_str name) {
  unsigned int index = hash(name) % SIZE;
  Symbol sym, syms = hashtable[index];

  LOOP_OPTIM
  for(sym = syms; sym; sym = sym->next)
    if(!strcmp(sym->name, (m_str)name))
      return sym;
  return hashtable[index] = mksymbol(name, syms);
}

ANN m_str s_name(const Symbol sym) {
  return sym->name;
}

