#ifndef __MEMOIZE
#define __MEMOIZE

typedef struct Memoize_ * Memoize;
Memoize memoize_ini(MemPool, const Func, const enum Kind);
void memoize_end(MemPool, Memoize);
INSTR(MemoizeCall);
INSTR(MemoizeStore);
#endif
