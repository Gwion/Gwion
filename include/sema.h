#ifndef __SEMA
ANN bool sema_pass(Env a, Ast *b);
ANN bool sema_variadic_class(Env, Class_Def);
ANN bool sema_variadic_func(Env, Func_Def);
#endif 
