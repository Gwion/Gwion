#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "gwion_parse.h"

ANN Ast gwion_parse_ast(const Gwion gwion, const char *data) {
  FILE *file = fmemopen((void*)data, strlen(data), "r");
  struct AstGetter_ arg = {gwion->env->name, file, gwion->st, .ppa = gwion->ppa};
  return parse(&arg);
}


ANN static Ast gwion_parse_ast1(const Gwion gwion, const char *data, const loc_t loc) {
  DECL_O(const Ast, ast, = gwion_parse_ast(gwion, data));
  if(ast->len != 1) {
    gwlog_error("more than one section", NULL, gwion->env->name, loc, 0);
    free_ast(gwion->mp, ast);
    return NULL;
  }
  return ast;
}

ANN Func_Def gwion_parse_func_def(const Gwion gwion, const char *data, const loc_t loc) {
  DECL_O(const Ast, ast, = gwion_parse_ast1(gwion, data, loc));
  const Section section = sectionlist_at(ast, 0);
  if(section.section_type != ae_section_func) {
    gwlog_error("invalid section type", NULL, gwion->env->name, loc, 0);
    free_ast(gwion->mp, ast);
    return NULL;
  }
  Func_Def fdef = section.d.func_def;
  free_sectionlist(gwion->mp, ast);
  return fdef;
}

ANN StmtList* gwion_parse_stmt_list(const Gwion gwion, const char *data, const loc_t loc) {
  DECL_O(const Ast, ast, = gwion_parse_ast1(gwion, data, loc));
  const Section section = sectionlist_at(ast, 0);
  if(section.section_type != ae_section_stmt) {
    gwlog_error("invalid section type", NULL, gwion->env->name, loc, 0);
    free_ast(gwion->mp, ast);
    return NULL;
  }
  StmtList* stmts = section.d.stmt_list;
  free_sectionlist(gwion->mp, ast);
  return stmts;
}

ANN Exp *gwion_parse_expression(const Gwion gwion, const char *data, const loc_t loc) {
  char* str = NULL;
  gw_asprintf(gwion->mp, &str, "%s;", data);
  StmtList *stmts = gwion_parse_stmt_list(gwion, str, loc);
  free_mstr(gwion->mp, str);
  if(!stmts) return NULL;
  if(stmts->len != 1) {
    gwlog_error("invalid number of statements", NULL, gwion->env->name, loc, 0);
    free_stmt_list(gwion->mp, stmts);
    return NULL;
  }
  Stmt *stmt = stmtlist_ptr_at(stmts, 0);
  if(stmt->stmt_type != ae_stmt_exp) {
    gwlog_error("invalid statement type", NULL, gwion->env->name, loc, 0);
    free_stmt_list(gwion->mp, stmts);
    return NULL;
  }
  Exp *exp = stmt->d.stmt_exp.val;
  free_stmtlist(gwion->mp, stmts);
  return exp;
}

ANN Type_Decl *gwion_parse_type_decl(const Gwion gwion, const char *data, const loc_t loc) {
  char* str = NULL;
  gw_asprintf(gwion->mp, &str, "var %s A;", data);
  StmtList *stmts = gwion_parse_stmt_list(gwion, str, loc);
  free_mstr(gwion->mp, str);
  if(!stmts) return NULL;
  if(stmts->len != 1) {
    gwlog_error("invalid number of statements", NULL, gwion->env->name, loc, 0);
    free_stmt_list(gwion->mp, stmts);
    return NULL;
  }
  Stmt *stmt = stmtlist_ptr_at(stmts, 0);
  if(stmt->stmt_type != ae_stmt_exp) {
    gwlog_error("invalid statement type", NULL, gwion->env->name, loc, 0);
    free_stmt_list(gwion->mp, stmts);
    return NULL;
  }
  Exp *exp = stmt->d.stmt_exp.val;
  free_stmtlist(gwion->mp, stmts);
  if(exp->exp_type != ae_exp_decl) {
    gwlog_error("invalid expression type", NULL, gwion->env->name, loc, 0);
    free_exp(gwion->mp, exp);
    return NULL;
  }
  Type_Decl *td = cpy_type_decl(gwion->mp, exp->d.exp_decl.var.td);
  free_exp(gwion->mp, exp);
  return td;
}
