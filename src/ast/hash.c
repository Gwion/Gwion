#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hash.h"
#include "defs.h"
#include "mpool.h"

Args new_args(const char* name) {
  const Args a = mp_alloc(Args);
  a->name = strdup(name);
  return a;
}

void clean_args(const Args a) {
  if(a->next)
    clean_args(a->next);
  if(a->text)
    xfree(a->text);
  a->text = NULL;
}

static void free_args(const Args a) {
  if(a->next)
    free_args(a->next);
  if(a->text)
    xfree(a->text);
  xfree(a->name);
  mp_free(Args, a);
}

static void free_entry(const Macro s) {
  if(s->next)
    free_entry(s->next);
  xfree(s->name);
  if(s->text)
    xfree(s->text);
  if(s->base)
    free_args(s->base);
  mp_free(Macro, s);
}

void hini(Hash h, const unsigned int size) {
  h->table = (Macro*)xcalloc(((size  + 3) & 0xfffffffc), sizeof(struct Macro_));
  h->size = size;
}

void hdel(const Hash h) {
  for(unsigned int i = h->size + 1; --i;) {
    const Macro s = h->table[i - 1];
    if(s) {
      free_entry(s);
      h->table[i-1] = NULL;
    }
  }
}

void hend(const Hash h) {
  xfree(h->table);
}

static unsigned int hash(const char *s0) {
  unsigned int h = 0;
  const char *s;
  for(s = s0; *s; ++s)
    h = h * 65599 + *s;
  return h;
}

static inline Macro mkentry(const char* name, const Macro next) {
  const Macro s = mp_alloc(Macro);
  s->name = strdup(name);
  s->next = next;
  return s;
}

#define haction(type, func, pre, action, post, ret) \
type h##func(const Hash h, const char* name) {      \
  const unsigned int idx = hash(name) % h->size;    \
  const Macro sym = h->table[idx];                  \
  pre                                               \
  for(Macro s = sym; s; s = s->next) {              \
    if(!strcmp(s->name, name)) { action }           \
    post                                            \
  }                                                 \
  ret;                                              \
}

haction(Macro, has,, return sym;, ,return 0)
haction(Macro, add,, return NULL;,, return h->table[idx] = mkentry(name, sym);)
haction(int, rem, Macro prev = NULL;,
      if(prev)
        prev->next = s->next;
      else
        h->table[idx] = NULL;
      s->next = NULL;
      free_entry(s);
      return 0;
,    prev = s;,return 1;)
