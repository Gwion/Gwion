#ifndef __SYMBOL_H__
#define __SYMBOL_H__

typedef struct Symbol_ * Symbol;

ANN Symbol insert_symbol(const m_str) __attribute__((pure));
ANN m_str s_name(const Symbol) __attribute__((pure));
void free_symbols(void);
#endif
