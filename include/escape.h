#ifndef __ESCAPE
#define __ESCAPE
char *     escape_table(MemPool);
ANN char   str2char(const Emitter, const m_str, const loc_t);
ANN m_bool escape_str(const Emitter, m_str, const loc_t);
#endif
