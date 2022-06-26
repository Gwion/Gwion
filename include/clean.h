#ifndef __CLEAN
#define __CLEAN

void ast_cleaner(const Gwion, Ast);
void class_def_cleaner(const Gwion, Class_Def);
void func_def_cleaner(const Gwion, Func_Def);
#ifdef __CLEAN_IMPL
typedef struct {
  Gwion    gwion;
  uint16_t scope;
} Clean;

ANN static void clean_array_sub(Clean *a, Array_Sub b);
ANN static void clean_type_list(Clean *a, Type_List b);
ANN static void clean_tmpl(Clean *a, Tmpl *b);
ANN static void clean_range(Clean *a, Range *b);
ANN static void clean_type_decl(Clean *a, Type_Decl *b);
ANN static void clean_prim(Clean *a, Exp_Primary *b);
ANN static void clean_var_decl(Clean *a, Var_Decl *b);
ANN static void clean_exp_decl(Clean *a, Exp_Decl *b);
ANN static void clean_exp_binary(Clean *a, Exp_Binary *b);
ANN static void clean_exp_unary(Clean *a, Exp_Unary *b);
ANN static void clean_exp_cast(Clean *a, Exp_Cast *b);
ANN static void clean_exp_post(Clean *a, Exp_Postfix *b);
ANN static void clean_exp_call(Clean *a, Exp_Call *b);
ANN static void clean_exp_array(Clean *a, Exp_Array *b);
ANN static void clean_exp_slice(Clean *a, Exp_Slice *b);
ANN static void clean_exp_if(Clean *a, Exp_If *b);
ANN static void clean_exp_dot(Clean *a, Exp_Dot *b);
ANN static void clean_exp_lambda(Clean *a, Exp_Lambda *b);
ANN static void clean_exp(Clean *a, Exp b);
ANN static void clean_stmt_exp(Clean *a, Stmt_Exp b);
ANN static void clean_stmt_for(Clean *a, Stmt_For b);
ANN static void clean_stmt_each(Clean *a, Stmt_Each b);
ANN static void clean_stmt_loop(Clean *a, Stmt_Loop b);
ANN static void clean_stmt_if(Clean *a, Stmt_If b);
ANN static void clean_stmt_code(Clean *a, Stmt_Code b);
ANN static void clean_stmt_return(Clean *a, Stmt_Exp b);
ANN static void clean_case_list(Clean *a, Stmt_List b);
ANN static void clean_stmt_match(Clean *a, Stmt_Match b);
ANN static void clean_stmt_case(Clean *a, Stmt_Match b);
ANN static void clean_stmt(Clean *a, Stmt b);
ANN static void clean_arg_list(Clean *a, Arg_List b);
ANN static void clean_stmt_list(Clean *a, Stmt_List b);
ANN static void clean_func_base(Clean *a, Func_Base *b);
ANN static void clean_func_def(Clean *a, Func_Def b);
ANN static void clean_class_def(Clean *a, Class_Def b);
ANN static void clean_enum_def(Clean *a, Enum_Def b);
ANN static void clean_union_def(Clean *a, Union_Def b);
ANN static void clean_fptr_def(Clean *a, Fptr_Def b);
ANN static void clean_type_def(Clean *a, Type_Def b);
ANN static void clean_section(Clean *a, Section *b);
ANN static void clean_ast(Clean *a, Ast b);

#endif

#endif
