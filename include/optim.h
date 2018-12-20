#ifndef __OPTIM
#define __OPTIM
#ifdef OPTIMIZE
#define OPTIMIZE_DEFAULT default: break;
#define OPTIMIZE_CONST(a) CHECK_BO(optimize_const(a))
ANN m_bool optimize_const(const Exp_Binary*);
ANN2(1) void constprop_prim(const Exp_Primary* p, m_uint* ptr);
#define OPTIMIZE_PRIM_CONST(a, b)      \
else if(GET_FLAG(v, ae_flag_constprop)) \
  constprop_prim(a,b);
#else
#define OPTIMIZE_DEFAULT
#define OPTIMIZE_CONST(a)
#define OPTIMIZE_PRIM_CONST(a, b)
#endif
m_bool constant_folding(const Exp_Binary* bin);
#endif
