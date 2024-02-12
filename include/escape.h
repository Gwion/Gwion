#ifndef __ESCAPE
#define __ESCAPE
char *escape_table(MemPool);
ANN bool str2char(const Emitter, const m_str, char *, const loc_t);
ANN bool escape_str(const Emitter, m_str, const loc_t);
#endif
