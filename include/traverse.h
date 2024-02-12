#ifndef __TRAVERSE
#define __TRAVERSE
ANN bool traverse_ast(const Env, Ast *const);
ANN bool traverse_class_def(const Env, const Class_Def);
ANN bool traverse_func_def(const Env, const Func_Def);
ANN bool traverse_union_def(const Env, const Union_Def);
ANN bool traverse_enum_def(const Env, const Enum_Def);
ANN bool traverse_fptr_def(const Env, const Fptr_Def);
ANN bool traverse_type_def(const Env env, const Type_Def);
ANN bool traverse_exp(const Env, Exp*);

ANN bool scan0_ast(const Env, Ast*);
ANN bool scan1_ast(const Env, Ast*);
ANN bool scan2_ast(const Env, Ast*);
ANN bool check_ast(const Env, Ast*);

ANN bool scan1_exp(const Env, Exp*);
ANN bool scan2_exp(const Env, Exp*);
ANN Type   check_exp(const Env, Exp*);

ANN bool scan0_func_def(const Env, const Func_Def);
ANN bool scan1_func_def(const Env, const Func_Def);
ANN bool scan2_func_def(const Env, const Func_Def);
ANN bool check_func_def(const Env, const Func_Def);

ANN bool scan0_fptr_def(const Env, const Fptr_Def);
ANN bool scan1_fptr_def(const Env, const Fptr_Def);
ANN bool scan2_fptr_def(const Env, const Fptr_Def);
ANN bool check_fptr_def(const Env, const Fptr_Def);

ANN bool scan0_union_def(const Env, const Union_Def);
ANN bool scan1_union_def(const Env, const Union_Def);
ANN bool scan2_union_def(const Env, const Union_Def);
ANN bool check_union_def(const Env, const Union_Def);

ANN bool scan0_enum_def(const Env, const Enum_Def);
ANN bool scan1_enum_def(const Env, const Enum_Def);
// ANN bool scan2_enum_def(const Env, const Enum_Def);
ANN bool check_enum_def(const Env, const Enum_Def);

ANN bool scan0_type_def(const Env, const Type_Def);
ANN bool scan1_type_def(const Env, const Type_Def);
ANN bool scan2_type_def(const Env, const Type_Def);
ANN bool check_type_def(const Env, const Type_Def);

ANN bool scan0_class_def(const Env, const Class_Def);
ANN bool scan1_class_def(const Env, const Class_Def);
ANN bool scan2_class_def(const Env, const Class_Def);
ANN bool check_class_def(const Env, const Class_Def);

ANN Type check_exp_call1(const Env env, Exp_Call *const exp);
ANN bool scan0_prim_def(const Env env, const Prim_Def pdef);
#endif
