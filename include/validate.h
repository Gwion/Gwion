#pragma once

ANN bool exp_validation(Env env, Exp* b);
ANN bool func_def_validation(Env env, Func_Def b);
ANN bool union_def_validation(Env env, Union_Def b);
ANN bool enum_def_validation(Env env, Enum_Def b);
ANN bool fptr_def_validation(Env env, Fptr_Def b);
ANN bool type_def_validation(Env env, Type_Def b);
ANN bool class_def_validation(Env env, Class_Def b);
ANN bool ast_validation(Env env, Ast *ast);
