%define api.pure full
/* %define parse.error verbose */
%parse-param { MyArg* arg }
%lex-param  { void* scanner }  
%lex-param { MyArg* arg }
/* %param { MyArg* arg } */
%name-prefix "minimal_"
%{
#define YYERROR_VERBOSE
#include "absyn.h"
#define scanner arg->scanner


char *strcat(char *dest, const char *src);
void minimal_error(void* data, const char* s);
int minimal_lex(void*, void* , void*);
static int get_pos(void* data)
{
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  return arg->line;
}

static char* append_doc(void* data, m_str str)
{
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  vector_append(arg->doc, (vtype)str);
  return str;
}

static m_str get_doc(void* data)
{
  m_str ret;
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  ret = (m_str)vector_front(arg->doc);
  vector_remove(arg->doc, 0);
  return ret;
}
/*
static m_str get_arg_doc(void* data)
{
  m_str ret;
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  ret = (m_str)vector_back(arg->doc);
  vector_pop(arg->doc);
  return ret;
}
*/
%}

%union {
	char* sval;
	int ival;
	m_float fval;
  Complex* c_val;
  Polar* polar;
	Vec vec;
  Array_Sub array_sub;
  Var_Decl var_decl;
  Var_Decl_List var_decl_list;
  Type_Decl* type_decl;
  Expression   exp;
  Func_Ptr* func_ptr;
  Stmt* stmt;
  Stmt_List stmt_list;
  Arg_List arg_list;
  Decl_List decl_list; // for union
  Union* union_stmt; // for union
  Func_Def func_def;
  Section* section;
  ID_List id_list;
  Type_List type_list; // call template
  Class_Body class_body;
  Class_Ext class_ext;
//  Class_Ext iface_ext;
  Class_Def class_def;
  Ast ast;
};

%token SEMICOLON CHUCK COMMA
  ASSIGN DIVIDE TIMES PERCENT
  L_HACK R_HACK LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE
  PLUSCHUCK MINUSCHUCK TIMESCHUCK DIVIDECHUCK MODULOCHUCK ATCHUCK UNCHUCK TRIG UNTRIG
  PERCENTPAREN SHARPPAREN PROTECTED ABSTRACT
  ATSYM FUNCTION DOLLAR TILDA
  QUESTION COLON EXCLAMATION
  IF ELSE WHILE DO UNTIL LOOP FOR GOTO SWITCH CASE ENUM
  RETURN BREAK CONTINUE
  PLUSPLUS MINUSMINUS
  NEW SPORK SIZEOF TYPEOF
  CLASS INTERFACE STATIC PRIVATE PUBLIC EXTENDS IMPLEMENTS DOT COLONCOLON
  AND EQ GE GT LE LT MINUS PLUS NEQ SHIFT_LEFT SHIFT_RIGHT S_AND S_OR S_XOR OR
  AST_DTOR OPERATOR FUNC_PTR
	RSL RSR RSAND RSOR RSXOR
	RAND ROR REQ RNEQ RGT RGE RLT RLE
	RINC RDEC RUNINC RUNDEC
  TEMPLATE
  NOELSE ARROW_LEFT ARROW_RIGHT
  LTB GTB
  VARARG UNION ATPAREN

%token<ival> NUM
%token<fval> FLOAT
%token<sval> ID STRING_LIT CHAR_LIT DOC

%type<ival> op
%type<ival> unary_operator
%type<ival> arrow_operator
%type<ival> class_decl
%type<ival> static_decl
%type<ival> function_decl
%type<var_decl> var_decl
%type<var_decl_list> var_decl_list
%type<type_decl> type_decl type_decl2
%type<exp> primary_exp
%type<exp> decl_exp
%type<exp> binary_exp
%type <array_sub> array_exp
%type <array_sub> array_empty
%type <exp> conditional_expression
%type <exp> logical_or_expression
%type <exp> logical_and_expression
%type <exp> inclusive_or_expression
%type <exp> exclusive_or_expression
%type <exp> and_expression
%type <exp> equality_expression
%type <exp> relational_expression
%type <exp> shift_expression
%type <exp> additive_expression
%type <exp> multiplicative_expression
%type <exp> tilda_expression
%type <exp> unary_expression
%type <exp> dur_exp
%type<polar> polar_exp
%type<c_val> complex_exp
%type<vec> vec_exp
%type<exp> postfix_exp
%type<exp> cast_exp
%type<exp> arrow_expression
%type<exp> exp
%type<stmt> stmt
%type<stmt> loop_stmt
%type<stmt> selection_stmt
%type<stmt> jump_stmt
%type<stmt> code_segment
%type<stmt> exp_stmt
%type<stmt> case_stmt
%type<stmt_list> stmt_list
%type<stmt> label_stmt
%type<stmt> goto_stmt
%type<stmt> switch_stmt
%type<stmt> enum_stmt
%type<stmt> func_ptr
%type<arg_list> arg_list
%type<decl_list> decl_list
%type<union_stmt> union
%type<func_def> func_def
%type<section> section
%type<class_def> class_def
%type<class_ext> class_ext
//%type<class_ext> iface_ext
%type<class_body> class_body
%type<class_body> class_body2
%type<id_list> id_list
%type<id_list> id_dot
%type<type_list> type_list
%type<type_list> template
%type<section> class_section
%type<ast> ast

%start ast

%nonassoc NOELSE
%nonassoc ELSE


%left DOT

%%

ast
  : section { arg->ast = $$ = new_Ast($1, NULL, get_pos(scanner));  }
  | section ast { arg->ast = $$ = new_Ast($1, $2, get_pos(scanner)); }
  ;

section
  : stmt_list  { $$ = new_section_Stmt_List($1, get_pos(scanner)); }
  | func_def   { $$ = new_section_Func_Def ($1, get_pos(scanner)); }
  | class_def  { $$ = new_section_Class_Def($1, get_pos(scanner)); }
  ;

class_def
  : class_decl CLASS id_list LBRACE class_body RBRACE
      { $$ = new_class_def( $1, $3, NULL, $5, get_pos(scanner)); $$->doc = get_doc(scanner); }
  | class_decl CLASS id_list class_ext LBRACE class_body RBRACE
      { $$ = new_class_def( $1, $3, $4, $6, get_pos(scanner)); $$->doc = get_doc(scanner);}
//  | class_decl INTERFACE id_list LBRACE class_body RBRACE
//      { $$ = new_iface_def( $1, $3, NULL, $5, get_pos(scanner)); $$->doc = get_doc(scanner);}
//  | class_decl INTERFACE id_list iface_ext LBRACE class_body RBRACE
//      { $$ = new_iface_def( $1, $3, $4, $6, get_pos(scanner)); $$->doc = get_doc(scanner);}
  ;

class_ext
//  : IMPLEMENTS id_list                { $$ = new_class_ext( NULL, $2, get_pos(scanner)); }
//  | IMPLEMENTS id_list EXTENDS id_dot { $$ = new_class_ext( $4, $2, get_pos(scanner)); }
  : EXTENDS id_dot                    { $$ = new_class_ext( $2, NULL, get_pos(scanner)); }
//  | EXTENDS id_dot IMPLEMENTS id_list { $$ = new_class_ext( $2, $4, get_pos(scanner)); }
  ;

class_decl
  : PUBLIC  { $$ = ae_key_public; }
  | PRIVATE { $$ = ae_key_private; }
  |         { $$ = ae_key_private; }
  ;

class_body
  : class_body2 { $$ = $1; }
  |             { $$ = NULL; }
  ;

class_body2
  : class_section             { $$ = new_class_body( $1, get_pos(scanner)); }
  | class_section class_body2 { $$ = prepend_class_body( $1, $2, get_pos(scanner)); }
  ;

class_section
  : stmt_list    { $$ = new_section_Stmt_List( $1, get_pos(scanner)); }
  | func_def  { $$ = new_section_Func_Def( $1, get_pos(scanner)); }
  | class_def     { $$ = new_section_Class_Def( $1, get_pos(scanner)); }
  ;

//iface_ext
//  : EXTENDS id_list   { $$ = new_class_ext( NULL, $2, get_pos(scanner)); }
//  ;

id_list
  : ID                { $$ = new_id_list( $1, get_pos(scanner)); }
  | ID COMMA id_list  { $$ = prepend_id_list( $1, $3, get_pos(scanner)); }
  ;

id_dot
  : ID                { $$ = new_id_list( $1, get_pos(scanner)); }
  | ID DOT id_dot     { $$ = prepend_id_list( $1, $3, get_pos(scanner)); }
  ;

stmt_list
  : stmt { $$ = new_Stmt_List($1, NULL, get_pos(scanner));}
  | stmt stmt_list { $$ = new_Stmt_List($1, $2, get_pos(scanner));}
  ;

static_decl
  : STATIC                            { $$ = ae_key_static;   }
  | ABSTRACT                          { $$ = ae_key_abstract; }
  |                                   { $$ = ae_key_instance; }
  ;

function_decl
  : FUNCTION { $$ = ae_key_func; }
  | VARARG   { $$ = ae_key_variadic; }
  ;

func_ptr
//  : FUNC_PTR static_decl type_decl LPAREN ID RPAREN LPAREN arg_list RPAREN
// { $$ = new_Func_Ptr_Stmt($2, $5, $3, $8, get_pos(scanner)); }
//  | FUNC_PTR static_decl type_decl LPAREN ID RPAREN LPAREN RPAREN
// { $$ = new_Func_Ptr_Stmt($2, $5, $3, NULL, get_pos(scanner)); }

  : FUNC_PTR type_decl LPAREN ID RPAREN LPAREN RPAREN { $$ = new_Func_Ptr_Stmt(0, $4, $2, NULL, get_pos(scanner)); }
  | STATIC FUNC_PTR type_decl LPAREN ID RPAREN LPAREN RPAREN { $$ = new_Func_Ptr_Stmt(ae_key_static, $5, $3, NULL, get_pos(scanner)); }
  | FUNC_PTR type_decl LPAREN ID RPAREN LPAREN arg_list RPAREN { $$ = new_Func_Ptr_Stmt(0, $4, $2, $7, get_pos(scanner)); }
  | STATIC FUNC_PTR type_decl LPAREN ID RPAREN LPAREN arg_list RPAREN { $$ = new_Func_Ptr_Stmt(ae_key_static, $5, $3, $8, get_pos(scanner)); }
  ;

type_decl2
  : type_decl                         { $$ = $1; }
  | type_decl array_empty             { $$ = add_type_decl_array( $1, $2, get_pos(scanner)); }
  ;

arg_list
  : type_decl var_decl { $$ = new_Arg_List($1, $2, NULL, get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
  | type_decl var_decl COMMA arg_list{ $$ = new_Arg_List($1, $2, $4, get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
  ;

code_segment
  : LBRACE RBRACE { $$ = new_stmt_from_code( NULL, get_pos(scanner)); }
  | LBRACE stmt_list RBRACE { $$ = new_stmt_from_code( $2, get_pos(scanner)); }
  ;

stmt
  : exp_stmt
  | loop_stmt
  | selection_stmt
  | code_segment
  | label_stmt
  | goto_stmt
  | switch_stmt
  | case_stmt
  | enum_stmt
  | jump_stmt
  | func_ptr
  | union { $$ = new_stmt_from_Union($1, 0); }
  ;

enum_stmt
  : ENUM LBRACE id_list RBRACE SEMICOLON    { $$ = new_stmt_from_enum($3, NULL, get_pos(scanner)); }
  | ENUM LBRACE id_list RBRACE ID SEMICOLON { $$ = new_stmt_from_enum($3, $5, get_pos(scanner)); }
  ;

label_stmt
  : ID COLON {  $$ = new_stmt_from_gotolabel($1, 1, get_pos(scanner)); }
  ;

goto_stmt
  : GOTO ID SEMICOLON {  $$ = new_stmt_from_gotolabel($2, 0, get_pos(scanner)); }
  ;

case_stmt
  : CASE primary_exp COLON { $$ = new_stmt_from_case($2, get_pos(scanner)); }
  | CASE postfix_exp COLON { $$ = new_stmt_from_case($2, get_pos(scanner)); }
  ;

switch_stmt
  : SWITCH LPAREN exp RPAREN code_segment { $$ = new_stmt_from_switch($3, $5, get_pos(scanner));}
  ;

loop_stmt
  : WHILE LPAREN exp RPAREN stmt
    { $$ = new_stmt_from_while( $3, $5, 0, get_pos(scanner)); }
  | DO stmt WHILE LPAREN exp RPAREN SEMICOLON
    { $$ = new_stmt_from_while( $5, $2, 1, get_pos(scanner)); }
  | FOR LPAREN exp_stmt exp_stmt RPAREN stmt
      { $$ = new_stmt_from_for( $3, $4, NULL, $6, get_pos(scanner)); }
  | FOR LPAREN exp_stmt exp_stmt exp RPAREN stmt
      { $$ = new_stmt_from_for( $3, $4, $5, $7, get_pos(scanner)); }
  | UNTIL LPAREN exp RPAREN stmt
      { $$ = new_stmt_from_until( $3, $5, 0, get_pos(scanner)); }
  | DO stmt UNTIL LPAREN exp RPAREN SEMICOLON
      { $$ = new_stmt_from_until( $5, $2, 1, get_pos(scanner)); }
  | LOOP LPAREN exp RPAREN stmt
      { $$ = new_stmt_from_loop( $3, $5, get_pos(scanner)); }
  ;

selection_stmt
  : IF LPAREN exp RPAREN stmt %prec NOELSE
      { $$ = new_stmt_from_if( $3, $5, NULL, get_pos(scanner)); }
  | IF LPAREN exp RPAREN stmt ELSE stmt
      { $$ = new_stmt_from_if( $3, $5, $7, get_pos(scanner)); }
  ;

jump_stmt
  : RETURN SEMICOLON     { $$ = new_stmt_from_return( NULL, get_pos(scanner)); }
  | RETURN exp SEMICOLON { $$ = new_stmt_from_return( $2, get_pos(scanner)); }
  | BREAK SEMICOLON      { $$ = new_stmt_from_break(get_pos(scanner)); }
  | CONTINUE SEMICOLON   { $$ = new_stmt_from_continue(get_pos(scanner)); }
  ;

exp_stmt
  : exp SEMICOLON { $$ = new_Stmt_from_Expression($1,   get_pos(scanner)); }
  | SEMICOLON     { $$ = new_Stmt_from_Expression(NULL, get_pos(scanner)); }
  | DOC           { $$ = new_Stmt_from_Expression(NULL, get_pos(scanner)); append_doc(scanner, $1); }
  ;

exp
  : binary_exp            { $$ = $1; }
  | binary_exp COMMA exp  { $$ = prepend_Expression($1, $3, get_pos(scanner)); }
  //| exp COMMA unary_expression { $$ = prepend_Expression($1, $3, get_pos(scanner)); }
  ;
arrow_operator
        : ARROW_LEFT                        { $$ = ae_op_arrow_left; }
        | ARROW_RIGHT                       { $$ = ae_op_arrow_right; }
        ;
binary_exp
  : arrow_expression      { $$ = $1; }
  | binary_exp op arrow_expression     { $$ = new_Binary_Expression($1, $2, $3, get_pos(scanner)); }
  ;

arrow_expression
  : decl_exp                   { $$ = $1; }
  | arrow_expression arrow_operator decl_exp
    { $$ = new_Binary_Expression( $1, $2, $3, get_pos(scanner)); }

template
  : LTB type_list GTB { $$ = $2; }
  ;


op
  : CHUCK       { $$ = op_chuck; }
  | UNCHUCK     { $$ = op_unchuck; }
  | ATCHUCK     { $$ = op_at_chuck; }
  | PLUSCHUCK   { $$ = op_plus_chuck; }
  | MINUSCHUCK  { $$ = op_minus_chuck; }
  | TIMESCHUCK  { $$ = op_times_chuck; }
  | DIVIDECHUCK { $$ = op_divide_chuck; }
  | MODULOCHUCK { $$ = op_modulo_chuck; }
  | TRIG        { $$ = op_trig; }
  | UNTRIG      { $$ = op_untrig; }
  | RSL         { $$ = op_rsl; }
  | RSR         { $$ = op_rsr; }
  | RSAND       { $$ = op_rsand; }
  | RSOR        { $$ = op_rsor; }
  | RSXOR       { $$ = op_rsxor; }
  | RAND        { $$ = op_rand; }
  | ROR         { $$ = op_ror; }
  | REQ         { $$ = op_req; }
  | RNEQ        { $$ = op_rneq; }
  | RGT         { $$ = op_rgt; }
  | RGE         { $$ = op_rge; }
  | RLT         { $$ = op_rlt; }
  | RLE         { $$ = op_rle; }
  | RINC         { $$ = op_rinc; }
  | RDEC         { $$ = op_rdec; }
  | RUNINC         { $$ = op_runinc; }
  | RUNDEC         { $$ = op_rundec; }
  | EQ          { $$ = op_eq; }
  | ASSIGN      { $$ = op_assign; }
  ;

array_exp
  : LBRACK exp RBRACK          { $$ = new_array_sub( $2, get_pos(scanner) ); }
  | LBRACK exp RBRACK array_exp
      { $$ = prepend_array_sub( $4, $2, get_pos(scanner)); }
  ;

array_empty
  : LBRACK RBRACK                     { $$ = new_array_sub( NULL, get_pos(scanner)); }
  | array_empty LBRACK RBRACK         { $$ = prepend_array_sub( $1, NULL, get_pos(scanner)); }
  ;

decl_exp
  : conditional_expression
  | type_decl var_decl_list { $$= new_Decl_Expression($1, $2, 0, get_pos(scanner)); }
  | STATIC type_decl var_decl_list { $$= new_Decl_Expression($2, $3, 1, get_pos(scanner)); }
  | PRIVATE type_decl var_decl_list { $$= new_Decl_Expression($2, $3, 2, get_pos(scanner)); }
  | STATIC PRIVATE type_decl var_decl_list { $$= new_Decl_Expression($3, $4, 3, get_pos(scanner)); }
  | PROTECTED type_decl var_decl_list { $$= new_Decl_Expression($2, $3, 4, get_pos(scanner)); }
  | STATIC PROTECTED type_decl var_decl_list { $$= new_Decl_Expression($3, $4, 5, get_pos(scanner)); }
  ;
func_def
  : function_decl static_decl type_decl2 ID LPAREN RPAREN  code_segment
    { $$ = new_Func_Def($1, $2, $3, $4, NULL, $7, get_pos(scanner)); $$->type_decl->doc = get_doc(scanner); }
  | function_decl static_decl type_decl2 ID LPAREN RPAREN SEMICOLON
    { $$ = new_Func_Def($1, $2, $3, $4, NULL, NULL, get_pos(scanner)); $$->type_decl->doc = get_doc(scanner); }
  | function_decl static_decl type_decl2 ID LPAREN arg_list RPAREN  code_segment
    { $$ = new_Func_Def($1, $2, $3, $4, $6, $8, get_pos(scanner)); $$->type_decl->doc = get_doc(scanner); }
  | AST_DTOR LPAREN RPAREN code_segment
    { $$ = new_Func_Def(ae_key_func, ae_key_instance, new_Type_Decl(new_id_list("void", get_pos(scanner)), 0, get_pos(scanner)), "dtor", NULL, $4, get_pos(scanner)); $$->spec = ae_func_spec_dtor; $$->type_decl->doc = get_doc(scanner);}
  | OPERATOR type_decl ID LPAREN RPAREN code_segment
    { $$ = new_Func_Def(ae_key_func, ae_key_static, $2, $3, NULL, $6, get_pos(scanner)); $$->spec = ae_func_spec_op; $$->type_decl->doc = get_doc(scanner); }
  | OPERATOR type_decl ID LPAREN arg_list RPAREN code_segment
    { $$ = new_Func_Def(ae_key_func, ae_key_static, $2, $3, $5, $7, get_pos(scanner)); $$->spec = ae_func_spec_op; $$->type_decl->doc = get_doc(scanner); }
  | TEMPLATE LTB id_list GTB function_decl static_decl type_decl2 ID LPAREN RPAREN  code_segment
    { $$ = new_Func_Def($5, $6, $7, $8, NULL, $11, get_pos(scanner)); $$->type_decl->doc = get_doc(scanner); $$->types = $3; }
  | TEMPLATE LTB id_list GTB function_decl static_decl type_decl2 ID LPAREN arg_list RPAREN  code_segment
    { $$ = new_Func_Def($5, $6, $7, $8, $10, $12, get_pos(scanner)); $$->type_decl->doc = get_doc(scanner); $$->types = $3; }
  ;

type_decl
  : ID    { $$ = new_Type_Decl(new_id_list($1, get_pos(scanner)), 0, get_pos(scanner)); }
  | ID  ATSYM  { $$ = new_Type_Decl(new_id_list($1, get_pos(scanner)), 1, get_pos(scanner)); }
  | LT id_dot GT { $$ = new_Type_Decl($2,  0, get_pos(scanner)); }
  | LT id_dot GT ATSYM { $$ = new_Type_Decl($2,  1, get_pos(scanner)); }
  ;


decl_list
  : decl_exp SEMICOLON { $$ = new_Decl_List($1->d.exp_decl, NULL); }
  | decl_exp SEMICOLON decl_list { $$ = new_Decl_List($1->d.exp_decl, $3); }
  | ID SEMICOLON { $$ = NULL; }
  | ID SEMICOLON decl_list { $$ = new_Decl_List(NULL, $3); }
  ;

union
  : UNION LBRACE decl_list RBRACE SEMICOLON { $$ = new_Union($3); }
  ;

var_decl_list
  : var_decl  { $$ = new_Var_Decl_List($1, NULL, get_pos(scanner)); $$->doc = get_doc(scanner); }
  | var_decl  COMMA var_decl_list { $$ = new_Var_Decl_List($1, $3, get_pos(scanner)); $$->doc = get_doc(scanner); }
  ;

var_decl
  : ID              { $$ = new_Var_Decl($1, NULL, get_pos(scanner)); }
  | ID array_exp    { $$ = new_Var_Decl($1,   $2, get_pos(scanner)); }
  | ID array_empty  { $$ = new_Var_Decl($1,   $2, get_pos(scanner)); }
  ;

complex_exp
  : SHARPPAREN exp RPAREN
    { $$ = new_complex( $2, get_pos(scanner)); }
  ;

polar_exp
  : PERCENTPAREN exp RPAREN
    { $$ = new_polar($2, get_pos(scanner)); }
  ;
vec_exp // ge: added 1.3.5.3
	: ATPAREN exp RPAREN { $$ = new_Vec($2, get_pos(scanner)); }

conditional_expression
  : logical_or_expression
  | logical_or_expression QUESTION exp COLON conditional_expression
      { $$ = new_If_Expression( $1, $3, $5, get_pos(scanner)); }
  ;

logical_or_expression
  : logical_and_expression            { $$ = $1; }
  | logical_or_expression OR logical_and_expression
      { $$ = new_Binary_Expression( $1, op_or, $3, get_pos(scanner)); }
  ;

logical_and_expression
  : inclusive_or_expression           { $$ = $1; }
  | logical_and_expression AND inclusive_or_expression
      { $$ = new_Binary_Expression( $1, op_and, $3, get_pos(scanner)); }
  ;

inclusive_or_expression
  : exclusive_or_expression           { $$ = $1; }
  | inclusive_or_expression S_OR exclusive_or_expression
      { $$ = new_Binary_Expression( $1, op_s_or, $3, get_pos(scanner)); }
  ;

exclusive_or_expression
  : and_expression                    { $$ = $1; }
  | exclusive_or_expression S_XOR and_expression
      { $$ = new_Binary_Expression( $1, op_s_xor, $3, get_pos(scanner)); }
  ;

and_expression
  : equality_expression               { $$ = $1; }
  | and_expression S_AND equality_expression
      { $$ = new_Binary_Expression( $1, op_s_and, $3, get_pos(scanner)); }
  ;

equality_expression
  : relational_expression             { $$ = $1; }
  | equality_expression EQ relational_expression
    { $$ = new_Binary_Expression( $1, op_eq, $3, get_pos(scanner)); }
  | equality_expression NEQ relational_expression
    { $$ = new_Binary_Expression( $1, op_neq, $3, get_pos(scanner)); }
  ;

relational_expression
  : shift_expression                  { $$ = $1; }
  | relational_expression LT shift_expression
    { $$ = new_Binary_Expression( $1, op_lt, $3, get_pos(scanner)); }
  | relational_expression GT shift_expression
    { $$ = new_Binary_Expression( $1, op_gt, $3, get_pos(scanner)); }
  | relational_expression LE shift_expression
    { $$ = new_Binary_Expression( $1, op_le, $3, get_pos(scanner)); }
  | relational_expression GE shift_expression
    { $$ = new_Binary_Expression( $1, op_ge, $3, get_pos(scanner)); }
  ;

shift_expression
  : additive_expression               { $$ = $1; }
  | shift_expression SHIFT_LEFT additive_expression
    { $$ = new_Binary_Expression( $1, op_shift_left, $3, get_pos(scanner)); }
  | shift_expression SHIFT_RIGHT additive_expression
    { $$ = new_Binary_Expression( $1, op_shift_right, $3, get_pos(scanner)); }
  ;

additive_expression
  : multiplicative_expression          { $$ = $1; }
  | additive_expression PLUS multiplicative_expression
    { $$ = new_Binary_Expression( $1, op_plus, $3, get_pos(scanner)); }
  | additive_expression MINUS multiplicative_expression
    { $$ = new_Binary_Expression( $1, op_minus, $3, get_pos(scanner)); }
  ;

multiplicative_expression
  : tilda_expression { $$ = $1; }
  | multiplicative_expression TIMES tilda_expression
      { $$ = new_Binary_Expression( $1, op_times, $3, get_pos(scanner)); }
  | multiplicative_expression DIVIDE tilda_expression
      { $$ = new_Binary_Expression( $1, op_divide, $3, get_pos(scanner)); }
  | multiplicative_expression PERCENT tilda_expression
      { $$ = new_Binary_Expression( $1, op_percent, $3, get_pos(scanner)); }
  ;

tilda_expression
  : cast_exp  { $$ = $1; }
  | tilda_expression TILDA cast_exp
      { $$ = new_Binary_Expression( $1, op_tilda, $3, get_pos(scanner)); }
  ;

cast_exp
  : unary_expression
  | cast_exp DOLLAR type_decl
      { $$ = new_Cast_Expression( $3, $1, get_pos(scanner)); }
  ;


unary_expression
  : dur_exp { $$ = $1; }
  | PLUSPLUS unary_expression
      { $$ = new_exp_from_unary( op_plusplus, $2, get_pos(scanner)); }
  | MINUSMINUS unary_expression
      { $$ = new_exp_from_unary( op_minusminus, $2, get_pos(scanner)); }
  | unary_operator unary_expression
      { $$ = new_exp_from_unary( $1, $2, get_pos(scanner)); }
  | TYPEOF unary_expression
      { $$ = new_exp_from_unary( op_typeof, $2, get_pos(scanner)); }
  | SIZEOF unary_expression
      { $$ = new_exp_from_unary( op_sizeof, $2, get_pos(scanner)); }
  | NEW type_decl
      { $$ = new_exp_from_unary2(op_new, $2, NULL, get_pos(scanner)); }
  | NEW type_decl array_exp
      { $$ = new_exp_from_unary2(op_new, $2, $3, get_pos(scanner)); }
  | SPORK TILDA code_segment
        { $$ = new_exp_from_unary3( op_spork, $3, get_pos(scanner)); }
  ;

unary_operator
  : PLUS                              { $$ = op_plus; }
  | MINUS                             { $$ = op_minus; }
  | TILDA                             { $$ = op_tilda; }
  | EXCLAMATION                       { $$ = op_exclamation; }
  | TIMES                             { $$ = op_times; }
  | SPORK TILDA                       { $$ = op_spork; }
  // | S_AND                             { $$ = ae_op_s_and; }
  ;

dur_exp
  : postfix_exp
  | dur_exp COLONCOLON postfix_exp { $$ = new_Exp_Dur( $1, $3, get_pos(scanner)); }
  ;

type_list
  : ID { $$ = new_type_list(new_id_list($1, get_pos(scanner)), NULL, get_pos(scanner)); }
  | ID COMMA type_list{ $$ = new_type_list(new_id_list($1, get_pos(scanner)), $3, get_pos(scanner)); }
  ;

postfix_exp
  : primary_exp
  | postfix_exp array_exp
    { $$ = new_Array( $1, $2, get_pos(scanner)); }
  | postfix_exp LPAREN RPAREN
    { $$ = new_Func_Call( $1, NULL, get_pos(scanner)); }
  | postfix_exp LPAREN exp RPAREN
    { $$ = new_Func_Call( $1, $3, get_pos(scanner)); }
  | postfix_exp DOT ID
    { $$ = new_exp_from_member_dot( $1, $3, get_pos(scanner)); }
  | postfix_exp PLUSPLUS
    { $$ = new_Postfix_Expression( $1, op_plusplus, get_pos(scanner)); }
  | postfix_exp MINUSMINUS
    { $$ = new_Postfix_Expression( $1, op_minusminus, get_pos(scanner)); }
  | postfix_exp template LPAREN RPAREN
    { $$ = new_Func_Call( $1, NULL, get_pos(scanner)); $$->d.exp_func->types = $2; }
  | postfix_exp template LPAREN exp RPAREN
    { $$ = new_Func_Call( $1, $4, get_pos(scanner)); $$->d.exp_func->types = $2; }  ;
  ;
primary_exp
  : ID                { $$ = new_Primary_Expression_from_ID(     $1, get_pos(scanner)); }
  | NUM               { $$ = new_Primary_Expression_from_int(    $1, get_pos(scanner)); }
  | FLOAT             { $$ = new_Primary_Expression_from_float(  $1, get_pos(scanner)); }
  | STRING_LIT        { $$ = new_Primary_Expression_from_string( $1, get_pos(scanner)); }
  | CHAR_LIT          { $$ = new_exp_from_char(                  $1, get_pos(scanner)); }
  | array_exp         { $$ = new_exp_from_array_lit(             $1, get_pos(scanner)); }
  | array_empty         { $$ = new_exp_from_array_lit(           $1, get_pos(scanner)); }
  | complex_exp       { $$ = new_exp_from_complex(               $1, get_pos(scanner)); }
  | polar_exp         { $$ = new_exp_from_polar(                 $1, get_pos(scanner)); }
  | vec_exp           { $$ = new_exp_from_vec(                   $1, get_pos(scanner)); }
  | L_HACK exp R_HACK { $$ = new_Hack_Expression(                $2, get_pos(scanner)); }
  | LPAREN exp RPAREN { $$ = $2; }
  | LPAREN RPAREN     { $$ = new_Primary_Expression_from_nil(       get_pos(scanner)); }
  ;
%%
