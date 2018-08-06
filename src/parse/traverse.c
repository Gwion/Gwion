#include "absyn.h"
#include "traverse.h"

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

ANN m_bool scan1_stmt_union(const Env env, const Stmt_Union def);
ANN m_bool scan2_stmt_union(const Env env, const Stmt_Union def);
ANN m_bool check_stmt_union(const Env env, const Stmt_Union def);

ANN m_bool scan0_stmt_enum(const Env env, const Stmt_Enum def);
ANN m_bool scan1_stmt_enum(const Env env, const Stmt_Enum def);
ANN m_bool scan2_stmt_enum(const Env env, const Stmt_Enum def);
ANN m_bool check_stmt_enum(const Env env, const Stmt_Enum def);

ANN m_bool scan1_class_def(const Env env, const Class_Def def);
ANN m_bool scan2_class_def(const Env env, const Class_Def def);
ANN m_bool check_class_def(const Env env, const Class_Def def);

ANN m_bool traverse_ast(const Env env, const Ast ast) {
  if(scan0_ast(env, ast) < 0 ||
     scan1_ast(env, ast) < 0 ||
     scan2_ast(env, ast) < 0 ||
     check_ast(env, ast) < 0)
    return -1;
  return 1;
}

ANN m_bool traverse_decl(const Env env, const Exp_Decl* decl) {
  if(scan1_exp_decl(env, decl) < 0 ||
     scan2_exp_decl(env, decl) < 0 ||
     check_exp_decl(env, decl) ? 0:1)
    return -1;
  return 1;
}

ANN m_bool traverse_func_def(const Env env, const Func_Def def) {
  if(scan1_func_def(env, def) < 0 ||
     scan2_func_def(env, def) < 0 ||
     check_func_def(env, def) < 0)
    return -1;
  return 1;
}

ANN m_bool traverse_stmt_union(const Env env, const Stmt_Union def) {
  if(scan1_stmt_union(env, def) < 0 ||
     scan2_stmt_union(env, def) < 0 ||
     check_stmt_union(env, def) < 0)
    return -1;
  return 1;
}

ANN m_bool traverse_stmt_enum(const Env env, const Stmt_Enum def) {
  if(scan0_stmt_enum(env, def) < 0 ||
     scan1_stmt_enum(env, def) < 0 ||
     scan2_stmt_enum(env, def) < 0 ||
     check_stmt_enum(env, def) < 0)
    return -1;
  return 1;
}

ANN m_bool traverse_stmt_fptr(const Env env, const Stmt_Fptr def) {
  if(scan0_stmt_fptr(env, def) < 0 ||
     scan1_stmt_fptr(env, def) < 0 ||
     scan2_stmt_fptr(env, def) < 0 )// ||
//     check_stmt_fptr(env, def) < 0)
    return -1;
  return 1;
}

ANN m_bool traverse_class_def(const Env env, const Class_Def def) {
  if(scan1_class_def(env, def) < 0 ||
     scan2_class_def(env, def) < 0 ||
     check_class_def(env, def) < 0)
    return -1;
  return 1;
}
