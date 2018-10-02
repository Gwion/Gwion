#ifndef __SYMBOL_H__
#define __SYMBOL_H__

typedef struct Symbol_ * Symbol;

ANN Symbol insert_symbol(const m_str) __attribute__((pure));
ANN m_str s_name(const Symbol) __attribute__((pure));
void free_symbols(void);

ANN unsigned int levenstein(const char* restrict s1, const size_t l1,
    const char* restrict s2, const size_t l2);
m_str did_you_mean(const Symbol s);
#endif
