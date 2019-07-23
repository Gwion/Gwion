#ifndef __TRAVERSE
#define __TRAVERSE
ANN m_bool traverse_ast(const Env, const Ast);
ANN m_bool traverse_class_def(const Env, const Class_Def);
ANN m_bool traverse_func_def(const Env, const Func_Def);
ANN m_bool traverse_stmt_union(const Env, const Stmt_Union);
ANN m_bool traverse_enum_def(const Env, const Enum_Def);
ANN m_bool traverse_stmt_fptr(const Env, const Stmt_Fptr );
ANN m_bool traverse_stmt_type(const Env env, const Stmt_Type def);
ANN m_bool traverse_decl(const Env, const Exp_Decl*);

ANN m_bool scan0_ast(const Env, Ast);
ANN m_bool scan1_ast(const Env, Ast);
ANN m_bool scan2_ast(const Env, Ast);
ANN m_bool check_ast(const Env, Ast);

ANN m_bool scan1_exp_decl(const Env, const Exp_Decl*);
ANN m_bool scan2_exp_decl(const Env, const Exp_Decl*);
ANN Type   check_exp_decl(const Env, const Exp_Decl*);

ANN m_bool scan1_func_def(const Env, const Func_Def);
ANN m_bool scan2_func_def(const Env, const Func_Def);
ANN m_bool check_func_def(const Env, const Func_Def);

ANN m_bool scan0_stmt_fptr(const Env, const Stmt_Fptr);
ANN m_bool scan1_stmt_fptr(const Env, const Stmt_Fptr);
ANN m_bool scan2_stmt_fptr(const Env, const Stmt_Fptr);
//ANN m_bool check_stmt_fptr(const Env, const Stmt_Fptr);

ANN m_bool scan0_stmt_union(const Env, const Stmt_Union);
ANN m_bool scan1_stmt_union(const Env, const Stmt_Union);
ANN m_bool scan2_stmt_union(const Env, const Stmt_Union);
ANN m_bool check_stmt_union(const Env, const Stmt_Union);

ANN m_bool scan0_enum_def(const Env, const Enum_Def);
ANN m_bool scan1_enum_def(const Env, const Enum_Def);
//ANN m_bool scan2_enum_def(const Env, const Enum_Def);
ANN m_bool check_enum_def(const Env, const Enum_Def);

ANN m_bool scan0_stmt_type(const Env, const Stmt_Type);
ANN m_bool scan1_stmt_type(const Env, const Stmt_Type);
ANN m_bool scan2_stmt_type(const Env, const Stmt_Type);
ANN m_bool check_stmt_type(const Env, const Stmt_Type);

ANN m_bool scan0_class_def(const Env, const Class_Def);
ANN m_bool scan1_class_def(const Env, const Class_Def);
ANN m_bool scan2_class_def(const Env, const Class_Def);
ANN m_bool check_class_def(const Env, const Class_Def);
#endif
