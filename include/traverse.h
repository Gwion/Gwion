#ifndef __TRAVERSE
#define __TRAVERSE
ANN m_bool traverse_ast(const Env, Ast *const);
ANN m_bool traverse_class_def(const Env, const Class_Def);
ANN m_bool traverse_func_def(const Env, const Func_Def);
ANN m_bool traverse_union_def(const Env, const Union_Def);
ANN m_bool traverse_enum_def(const Env, const Enum_Def);
ANN m_bool traverse_fptr_def(const Env, const Fptr_Def);
ANN m_bool traverse_type_def(const Env env, const Type_Def);
ANN m_bool traverse_exp(const Env, const Exp);

ANN m_bool scan0_ast(const Env, Ast*);
ANN m_bool scan1_ast(const Env, Ast*);
ANN m_bool scan2_ast(const Env, Ast*);
ANN m_bool check_ast(const Env, Ast*);

ANN m_bool scan1_exp(const Env, const Exp);
ANN m_bool scan2_exp(const Env, const Exp);
ANN Type   check_exp(const Env, const Exp);

ANN m_bool scan0_func_def(const Env, const Func_Def);
ANN m_bool scan1_func_def(const Env, const Func_Def);
ANN m_bool scan2_func_def(const Env, const Func_Def);
ANN m_bool check_func_def(const Env, const Func_Def);

ANN m_bool scan0_fptr_def(const Env, const Fptr_Def);
ANN m_bool scan1_fptr_def(const Env, const Fptr_Def);
ANN m_bool scan2_fptr_def(const Env, const Fptr_Def);
ANN m_bool check_fptr_def(const Env, const Fptr_Def);

ANN m_bool scan0_union_def(const Env, const Union_Def);
ANN m_bool scan1_union_def(const Env, const Union_Def);
ANN m_bool scan2_union_def(const Env, const Union_Def);
ANN m_bool check_union_def(const Env, const Union_Def);

ANN m_bool scan0_enum_def(const Env, const Enum_Def);
ANN m_bool scan1_enum_def(const Env, const Enum_Def);
// ANN m_bool scan2_enum_def(const Env, const Enum_Def);
ANN m_bool check_enum_def(const Env, const Enum_Def);

ANN m_bool scan0_type_def(const Env, const Type_Def);
ANN m_bool scan1_type_def(const Env, const Type_Def);
ANN m_bool scan2_type_def(const Env, const Type_Def);
ANN m_bool check_type_def(const Env, const Type_Def);

ANN m_bool scan0_class_def(const Env, const Class_Def);
ANN m_bool scan1_class_def(const Env, const Class_Def);
ANN m_bool scan2_class_def(const Env, const Class_Def);
ANN m_bool check_class_def(const Env, const Class_Def);

ANN Type check_exp_call1(const Env env, Exp_Call *const exp);
#endif
