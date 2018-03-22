const m_bool traverse_ast(const Env, const Ast);
const m_bool traverse_class_def(const Env, const Class_Def);
const m_bool traverse_func_def(const Env, const Func_Def);
const m_bool traverse_stmt_union(const Env, const Stmt_Union);
const m_bool traverse_stmt_enum(const Env, const Stmt_Enum);
const m_bool traverse_stmt_fptr(const Env, const Stmt_Ptr );
const m_bool traverse_decl(const Env, const Exp_Decl*);

ANN Class_Def template_class(const Env, const Class_Def, Type_List);
ANN m_bool template_push_types(const Env, ID_List, Type_List);
ANN m_bool template_match(ID_List, Type_List);
