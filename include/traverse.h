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
#endif
