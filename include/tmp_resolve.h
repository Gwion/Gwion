#ifndef __TMPL_RESOLVE
#define __TMPL_RESOLVE
ANN Func find_template_match(const Env env, const Value value, const Exp_Call* exp);
ANN2(1, 2) Func find_func_match(const Env env, const Func up, const Exp exp);
#endif
