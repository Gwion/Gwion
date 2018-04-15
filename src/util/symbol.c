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

ANN static void free_symbol(const Symbol s) {
  if(s->next)
    free_symbol(s->next);
  free(s->name);
  mp_free(Symbol, s);
}

void free_symbols(void) {
  LOOP_OPTIM
  for(unsigned int i = SIZE + 1; --i;) {
    const Symbol s = hashtable[i - 1];
    if(s)
      free_symbol(s);
  }
}

__attribute__((hot,pure))
ANN static unsigned int hash(const char *s0) {
  unsigned int h = 0;
  const char *s;
  for(s = s0; *s; ++s)
    h = h * 65599 + *s;
  return h;
}

__attribute__((hot))
ANN2(1) static Symbol mksymbol(const m_str name, const Symbol next) {
  const Symbol s = mp_alloc(Symbol);
  s->name = strdup(name);
  s->next = next;
  return s;
}

__attribute__((hot))
ANN Symbol insert_symbol(const m_str name) {
  const unsigned int index = hash(name) % SIZE;
  const Symbol syms = hashtable[index];
  LOOP_OPTIM
  for(Symbol sym = syms; sym; sym = sym->next)
    if(!strcmp(sym->name, name))
      return sym;
  return hashtable[index] = mksymbol(name, syms);
}

m_str s_name(const Symbol s) { return s->name; }
