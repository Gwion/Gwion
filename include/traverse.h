#ifndef __TRAVERSE
#define __TRAVERSE
m_bool traverse_ast(const Env, const Ast);
m_bool traverse_class_def(const Env, const Class_Def);
m_bool traverse_template(const Env, const Class_Def);
m_bool traverse_func_def(const Env, const Func_Def);
m_bool traverse_stmt_union(const Env, const Stmt_Union);
m_bool traverse_stmt_enum(const Env, const Stmt_Enum);
m_bool traverse_stmt_fptr(const Env, const Stmt_Fptr );
m_bool traverse_decl(const Env, const Exp_Decl*);

ANN m_bool scan0_ast(const Env env, Ast ast);
ANN m_bool scan1_ast(const Env env, Ast ast);
ANN m_bool scan2_ast(const Env env, Ast ast);
ANN m_bool check_ast(const Env env, Ast ast);

ANN m_bool scan1_exp_decl(const Env env, const Exp_Decl* decl);
ANN m_bool scan2_exp_decl(const Env env, const Exp_Decl* decl);
ANN Type   check_exp_decl(const Env env, const Exp_Decl* decl);

ANN m_bool scan1_func_def(const Env env, const Func_Def def);
ANN m_bool scan2_func_def(const Env env, const Func_Def def);
ANN m_bool check_func_def(const Env env, const Func_Def def);

ANN m_bool scan0_stmt_fptr(const Env env, const Stmt_Fptr def);
ANN m_bool scan1_stmt_fptr(const Env env, const Stmt_Fptr def);
ANN m_bool scan2_stmt_fptr(const Env env, const Stmt_Fptr def);
//ANN m_bool check_stmt_fptr(const Env env, const Stmt_Fptr def);

ANN m_bool scan0_stmt_union(const Env env, const Stmt_Union def);
ANN m_bool scan1_stmt_union(const Env env, const Stmt_Union def);
ANN m_bool scan2_stmt_union(const Env env, const Stmt_Union def);
ANN m_bool check_stmt_union(const Env env, const Stmt_Union def);

ANN m_bool scan0_stmt_enum(const Env env, const Stmt_Enum def);
ANN m_bool scan1_stmt_enum(const Env env, const Stmt_Enum def);
//ANN m_bool scan2_stmt_enum(const Env env, const Stmt_Enum def);
ANN m_bool check_stmt_enum(const Env env, const Stmt_Enum def);

ANN m_bool scan0_stmt_type(const Env env, const Stmt_Type stmt);
ANN m_bool scan1_class_def(const Env env, const Class_Def def);
ANN m_bool scan2_class_def(const Env env, const Class_Def def);
ANN m_bool check_class_def(const Env env, const Class_Def def);
#endif
