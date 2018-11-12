typedef struct Memoize_ * Memoize;
//m_bool memoize_get(VM_Shred shred);
Memoize memoize_ini(const Func f, const enum Kind);
void memoize_end(Memoize m);
INSTR(MemoizeCall);
INSTR(MemoizeStore);
