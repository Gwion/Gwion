#ifndef __MEMOIZE
#define __MEMOIZE

#ifndef NOMEMOIZE
#define MEMOIZE_CALL  const Instr memoize = !GET_FLAG(f, pure) ? NULL : emit_add_instr(emit, MemoizeCall);
#define MEMOIZE_SET(a) if(memoize)memoize->m_val = a + 2;
#define MEMOIZE_STORE if(GET_FLAG(emit->env->func, pure)) emit_add_instr(emit, MemoizeStore);
#define MEMOIZE_INI   if(GET_FLAG(func, pure)) func->code->memoize = memoize_ini(func, kindof(func->def->ret_type->size, !func->def->ret_type->size));
typedef struct Memoize_ * Memoize;
Memoize memoize_ini(const Func f, const enum Kind);
void memoize_end(Memoize m);
INSTR(MemoizeCall);
INSTR(MemoizeStore);
#else
#define MEMOIZE_CALL
#define MEMOIZE_STORE
#define MEMOIZE_INI
#define MEMOIZE_SET(a)
#endif
#endif
