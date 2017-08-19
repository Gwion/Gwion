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

S_Symbol insert_symbol(const m_str);
m_str s_name(S_Symbol);
void free_symbols();
#endif
