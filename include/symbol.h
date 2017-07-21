//-----------------------------------------------------------------------------
// file: chuck_symbol.h
// desc: symbols in the syntax, adapted from Tiger compiler by Appel Appel
//
// author: Ge Wang (ge@ccrma.stanford.edu | gewang@cs.princeton.edu)
// adapted from: Andrew Appel (appel@cs.princeton.edu)
// date: Autumn 2002
//-----------------------------------------------------------------------------
#ifndef __SYMBOL_H__
#define __SYMBOL_H__

typedef struct S_Symbol_ * S_Symbol;
struct S_Symbol_ {
  m_str name;
  S_Symbol next;
};

S_Symbol insert_symbol(const m_str);
//static inline m_str s_name(S_Symbol sym) { return sym->name; }
#define s_name(sym) (sym->name)
void free_Symbols();
#endif
