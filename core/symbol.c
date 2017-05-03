/*----------------------------------------------------------------------------

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  U.S.A.
-----------------------------------------------------------------------------*/

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
#include "symbol.h"

struct S_Symbol_ {
  c_str name;
  S_Symbol next;
};

static S_Symbol mksymbol( c_constr name, S_Symbol next )
{
  S_Symbol s = calloc(1, sizeof(*s));
  s->name = calloc(1, strlen(name) + 1);
  strcpy(s->name, (c_str)name);
  s->next = next;
  return s;
}

#define SIZE 65347  /* should be prime */

static S_Symbol hashtable[SIZE];

static void free_Symbol(S_Symbol s)
{
  free(s->name);
  free(s);
}
void free_Symbols()
{
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

static unsigned int hash(const char *s0)
{
  unsigned int h = 0;
  const char *s;
  for(s = s0; *s; s++)
    h = h * 65599 + *s;
  return h;
}

static int streq(c_constr a, c_constr b)
{
  return !strcmp((char*)a, (char*)b);
}

S_Symbol insert_symbol(c_constr name)
{
  S_Symbol syms = NULL, sym;
  int index;
  if(!name)
    return NULL;
  index = hash(name) % SIZE;
  syms = hashtable[index];
  for(sym = syms; sym; sym = sym->next)
    if (streq(sym->name, name))
      return sym;
  sym = mksymbol(name, syms);
  hashtable[index] = sym;
  return sym;
}

c_str S_name( S_Symbol sym )
{
  return sym->name;
}
