m_bool traverse_ast(Env env, Ast ast);
m_bool traverse_decl(Env env, Exp_Decl* decl);
m_bool traverse_def(Env env, Func_Def def);

Class_Def template_class(Env env, Class_Def def, Type_List call);
m_bool template_push_types(Env env, ID_List base, Type_List call);
