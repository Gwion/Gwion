#include "absyn.h"

m_bool scan0_Ast(Env env, Ast ast);
m_bool scan1_ast(Env env, Ast ast);
m_bool scan2_ast(Env env, Ast ast);
m_bool check_ast(Env env, Ast ast);

m_bool scan1_exp_decl(Env env, Exp_Decl* decl);
m_bool scan2_exp_decl(Env env, Exp_Decl* decl);
m_bool check_exp_decl(Env env, Exp_Decl* decl);

m_bool scan1_func_def(Env env, Func_Def def);
m_bool scan2_func_def(Env env, Func_Def def);
m_bool check_func_def(Env env, Func_Def def);

m_bool scan0_stmt_fptr(Env env, Func_Def def);
m_bool scan1_stmt_fptr(Env env, Func_Def def);
m_bool scan2_stmt_fptr(Env env, Func_Def def);
m_bool check_stmt_fptr(Env env, Func_Def def);

m_bool scan1_stmt_union(Env env, Stmt_Union def);
m_bool scan2_stmt_union(Env env, Stmt_Union def);
m_bool check_stmt_union(Env env, Stmt_Union def);

m_bool scan1_stmt_enum(Env env, Stmt_Enum def);
m_bool scan2_stmt_enum(Env env, Stmt_Enum def);
m_bool check_stmt_enum(Env env, Stmt_Enum def);

m_bool scan1_class_def(Env env, Class_Def def);
m_bool scan2_class_def(Env env, Class_Def def);
m_bool check_class_def(Env env, Class_Def def);

m_bool traverse_ast(Env env, Ast ast) {
  if(scan0_Ast(env, ast) < 0 ||
     scan1_ast(env, ast) < 0 ||
     scan2_ast(env, ast) < 0 ||
     check_ast(env, ast) < 0)
    return -1;
  return 1;
}

m_bool traverse_decl(Env env, Exp_Decl* decl) {
  if(scan1_exp_decl(env, decl) < 0 ||
     scan2_exp_decl(env, decl) < 0 ||
     check_exp_decl(env, decl) ? 0:1)
    return -1;
  return 1;
}

m_bool traverse_func_def(Env env, Func_Def def) {
  if(scan1_func_def(env, def) < 0 ||
     scan2_func_def(env, def) < 0 ||
     check_func_def(env, def) < 0)
    return -1;
  return 1;
}

m_bool traverse_stmt_union(Env env, Stmt_Union def) {
  if(scan1_stmt_union(env, def) < 0 ||
     scan2_stmt_union(env, def) < 0 ||
     check_stmt_union(env, def) < 0)
    return -1;
  return 1;
}

m_bool traverse_stmt_enum(Env env, Stmt_Enum def) {
  if(scan1_stmt_enum(env, def) < 0 ||
     scan2_stmt_enum(env, def) < 0 ||
     check_stmt_enum(env, def) < 0)
    return -1;
  return 1;
}

m_bool traverse_stmt_fptr(Env env, Func_Def def) {
  if(scan0_stmt_fptr(env, def) < 0 ||
     scan1_stmt_fptr(env, def) < 0 ||
     scan2_stmt_fptr(env, def) < 0 ||
     check_stmt_fptr(env, def) < 0)
    return -1;
  return 1;
}
m_bool traverse_class_def(Env env, Class_Def def) {
  if(scan1_class_def(env, def) < 0 ||
     scan2_class_def(env, def) < 0 ||
     check_class_def(env, def) < 0)
    return -1;
  return 1;
}
