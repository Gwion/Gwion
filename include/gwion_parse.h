#pragma once
ANN Ast gwion_parse_ast(const Gwion gwion, const char *data);
ANN Func_Def gwion_parse_func_def(const Gwion gwion, const char *data, const loc_t loc);
ANN StmtList *gwion_parse_stmt_list(const Gwion gwion, const char *data, const loc_t loc);
ANN Exp *gwion_parse_expression(const Gwion gwion, const char *data, const loc_t loc);
ANN Type_Decl *gwion_parse_type_decl(const Gwion gwion, const char *data, const loc_t loc);

