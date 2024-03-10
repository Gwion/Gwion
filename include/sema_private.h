typedef struct {
  char *filename;
  MemPool mp;
  SymTable *st;
  MP_Vector *tmpls;
  Stmt_List *stmt_list;
  struct PPArg_ *ppa;
  bool error;
  bool func;
  bool scope;
  bool handling;
  bool in_variadic;
} Sema;

#ifdef __SEMA_IMPLEMENTATION__
ANN static bool sema_array_sub(Sema *a, Array_Sub b);
ANN static bool sema_tmpl(Sema *a, Tmpl *b);
ANN static bool sema_range(Sema *a, Range *b);
ANN static bool sema_type_decl(Sema *a, Type_Decl *b);
ANN static bool sema_prim_id(Sema *a, Symbol *b);
ANN static bool sema_prim_num(Sema *a, m_uint *b);
ANN static bool sema_prim_float(Sema *a, m_float *b);
ANN static bool sema_prim_str(Sema *a, m_str *b);
ANN static bool sema_prim_array(Sema *a, Array_Sub *b);
ANN static bool sema_prim_range(Sema *a, Range* *b);
ANN static bool sema_prim_dict(Sema *a, Exp* *b);
ANN static bool sema_prim_hack(Sema *a, Exp* *b);
ANN static bool sema_prim_interp(Sema *a, Exp* *b);
ANN static bool sema_prim_char(Sema *a, m_str *b);
ANN static bool sema_prim_nil(Sema *a, bool *b);
ANN static bool sema_prim_perform(Sema *a, Symbol *b);
ANN static bool sema_prim(Sema *a, Exp_Primary *b);
ANN static bool sema_variable(Sema *a, Variable *b);
ANN static bool sema_exp_decl(Sema *a, Exp_Decl *b);
ANN static bool sema_exp_binary(Sema *a, Exp_Binary *b);
ANN static bool sema_exp_unary(Sema *a, Exp_Unary *b);
ANN static bool sema_exp_cast(Sema *a, Exp_Cast *b);
ANN static bool sema_exp_post(Sema *a, Exp_Postfix *b);
ANN static bool sema_exp_call(Sema *a, Exp_Call *b);
ANN static bool sema_exp_array(Sema *a, Exp_Array *b);
ANN static bool sema_exp_slice(Sema *a, Exp_Slice *b);
ANN static bool sema_exp_if(Sema *a, Exp_If *b);
ANN static bool sema_exp_dot(Sema *a, Exp_Dot *b);
ANN static bool sema_exp_lambda(Sema *a, Exp_Lambda *b);
ANN static bool sema_exp_td(Sema *a, Type_Decl *b);
ANN static bool sema_exp(Sema *a, Exp* b);
ANN static bool sema_stmt_exp(Sema *a, Stmt_Exp b);
ANN static bool sema_stmt_while(Sema *a, Stmt_Flow b);
ANN static bool sema_stmt_until(Sema *a, Stmt_Flow b);
ANN static bool sema_stmt_for(Sema *a, Stmt_For b);
ANN static bool sema_stmt_each(Sema *a, Stmt_Each b);
ANN static bool sema_stmt_loop(Sema *a, Stmt_Loop b);
ANN static bool sema_stmt_if(Sema *a, Stmt_If b);
ANN static bool sema_stmt_code(Sema *a, Stmt_Code b);
ANN static bool sema_stmt_break(Sema *a, Stmt_Index b);
ANN static bool sema_stmt_continue(Sema *a, Stmt_Index b);
ANN static bool sema_stmt_return(Sema *a, Stmt_Exp b);
ANN static bool sema_case_list(Sema *a, Stmt_List b);
ANN static bool sema_stmt_match(Sema *a, Stmt_Match b);
ANN static bool sema_stmt_case(Sema *a, Stmt_Match b);
ANN static bool sema_stmt_index(Sema *a, Stmt_Index b);
ANN static bool sema_stmt_pp(Sema *a, Stmt_PP b);
ANN static bool sema_stmt_retry(Sema *a, Stmt_Exp b);
ANN static bool sema_stmt_try(Sema *a, Stmt_Try b);
ANN static bool sema_stmt_defer(Sema *a, Stmt_Defer b);
ANN static bool sema_stmt(Sema *a, Stmt* b, bool in_list);
ANN static bool sema_stmt_list(Sema *a, Stmt_List *b);
ANN static bool sema_func_def(Sema *a, Func_Def b);
ANN static bool sema_class_def(Sema *a, Class_Def b);
ANN static bool sema_trait_def(Sema *a, Trait_Def b);
ANN static bool sema_enum_def(Sema *a, Enum_Def b);
ANN static bool sema_union_def(Sema *a, Union_Def b);
ANN static bool sema_fptr_def(Sema *a, Fptr_Def b);
ANN static bool sema_type_def(Sema *a, Type_Def b);
ANN static bool sema_extend_def(Sema *a, Extend_Def b);
ANN static bool sema_section(Sema *a, Section *b);
ANN static bool sema_ast(Sema *a, Ast *b);
#endif
