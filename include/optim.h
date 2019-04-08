#ifndef __OPTIM
#define __OPTIM
#ifdef OPTIMIZE
#define OPTIMIZE_CONST(a) CHECK_BO(optimize_const(a))
ANN m_bool optimize_const(MemPool mp, const Exp_Binary*);
//ANN2(1) void constprop_prim(const Exp_Primary* p, m_uint* ptr);
#else
#define OPTIMIZE_CONST(a)
#endif
m_bool constant_folding(MemPool mp, const Exp_Binary* bin);
#endif
