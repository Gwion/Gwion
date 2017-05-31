//-----------------------------------------------------------------------------
// file: symbol.c
// desc: symbols in the syntax, adapted from Tiger compiler by Appel Appel
// adapted from
// adapted from
//   author: Ge Wang (ge@ccrma.stanford.edu | gewang@cs.princeton.edu)
//   file: chuck_symbol.cpp
// adapted from: Andrew Appel (appel@cs.princeton.edu)
// date: Autumn 2002
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "symbol.h"

#define SIZE 65347  /* should be prime */

struct S_Symbol_ {
  m_str name;
  S_Symbol next;
};

static S_Symbol hashtable[SIZE];

static S_Symbol mksymbol(const m_str name, S_Symbol next) {
  S_Symbol s = calloc(1, sizeof(*s));
  s->name = calloc(1, strlen(name) + 1);
  strcpy(s->name, (m_str)name);
  s->next = next;
  return s;
}

static void free_Symbol(S_Symbol s) {
  free(s->name);
  free(s);
}
void free_Symbols() {
  int i;
  for(i = 0; i < SIZE; i++) {
    S_Symbol s = hashtable[i];
    if(s) {
      S_Symbol tmp, next = s;
      while(next) {
        tmp = next;
        next = next->next;
        free_Symbol(tmp);
      }
    }
  }
}

static unsigned int hash(const char *s0) {
  unsigned int h = 0;
  const char *s;
  for(s = s0; *s; s++)
    h = h * 65599 + *s;
  return h;
}

S_Symbol insert_symbol(const m_str name) {
  S_Symbol syms = NULL, sym;
  int index;
  index = hash(name) % SIZE;
  syms = hashtable[index];
  for(sym = syms; sym; sym = sym->next)
    if(!strcmp(sym->name, (m_str)name))
      return sym;
  sym = mksymbol(name, syms);
  hashtable[index] = sym;
  return sym;
}

m_str S_name(S_Symbol sym) {
  return sym->name;
}
