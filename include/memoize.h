#ifndef __MEMOIZE
#define __MEMOIZE

typedef struct Memoize_ *Memoize;
Memoize                  memoize_ini(const Emitter, const Func);
void                     memoize_end(MemPool, const Memoize);
INSTR(MemoizeIni);
INSTR(MemoizeStore);
#endif
