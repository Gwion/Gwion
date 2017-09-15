%define api.pure full
/* %define parse.error verbose */
%parse-param { MyArg* arg }
%lex-param  { void* scanner }  
%lex-param { MyArg* arg }
/* %param { MyArg* arg } */
%name-prefix "gwion_"
%{
#define YYERROR_VERBOSE
#include "absyn.h"
#include "arg.h"
#define scanner arg->scanner


char *strcat(char *dest, const char *src);
void gwion_error(void* data, const char* s);
int gwion_lex(void*, void* , void*);
static int get_pos(void* data)
{
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  return arg->line;
}

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
  Exp   exp;
  Stmt_Ptr func_ptr;
  Stmt stmt;
  Stmt_List stmt_list;
  Arg_List arg_list;
  Decl_List decl_list; // for union
  Func_Def func_def;
  Section* section;
  ID_List id_list;
  Type_List type_list; // call template
  Class_Body class_body;
  ID_List class_ext;
  Class_Def class_def;
  Ast ast;
};

%token SEMICOLON CHUCK COMMA
  ASSIGN DIVIDE TIMES PERCENT
  L_HACK R_HACK LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE
  PLUSCHUCK MINUSCHUCK TIMESCHUCK DIVIDECHUCK MODULOCHUCK ATCHUCK UNCHUCK TRIG UNTRIG
  PERCENTPAREN SHARPPAREN
  ATSYM FUNCTION DOLLAR TILDA
  QUESTION COLON EXCLAMATION
  IF ELSE WHILE DO UNTIL LOOP FOR GOTO SWITCH CASE ENUM
  RETURN BREAK CONTINUE
  PLUSPLUS MINUSMINUS
  NEW SPORK
  CLASS STATIC PUBLIC EXTENDS DOT COLONCOLON
  AND EQ GE GT LE LT MINUS PLUS NEQ SHIFT_LEFT SHIFT_RIGHT S_AND S_OR S_XOR OR
  AST_DTOR OPERATOR FUNC_PTR
	RSL RSR RSAND RSOR RSXOR
	RAND ROR REQ RNEQ RGT RGE RLT RLE
  TEMPLATE
  NOELSE
  LTB GTB
  VARARG UNION ATPAREN TYPEOF CONST

%token<ival> NUM
%token<fval> FLOAT
%token<sval> ID STRING_LIT CHAR_LIT

%type<ival> atsym
%type<ival> op shift_op postfix_op relationnal_op equality_op
%type<ival> unary_op additive_op multiplicative_op
%type<ival> class_decl
%type<ival> static_decl
%type<ival> function_decl
%type<var_decl> var_decl
%type<var_decl_list> var_decl_list
%type<type_decl> type_decl type_decl2
%type<exp> primary_exp
%type<exp> decl_exp
%type<exp> binary_exp
%type<exp> call_paren
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
%type <exp> unary_expression
%type <exp> dur_exp
%type<polar> polar_exp
%type<c_val> complex_exp
%type<vec> vec_exp
%type<exp> postfix_exp
%type<exp> cast_exp
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
%type<stmt> union_stmt
%type<arg_list> arg_list func_args
%type<decl_list> decl_list
%type<func_def> func_def
%type<section> section
%type<class_def> class_def
%type<id_list> class_ext
%type<class_body> class_body
%type<class_body> class_body2
%type<id_list> id_list
%type<id_list> id_dot decl_template
%type<type_list> type_list
%type<type_list> template call_template
%type<section> class_section
%type<ast> ast

%start ast

%nonassoc NOELSE
%nonassoc ELSE

%precedence NEG
%left DOT

%destructor { free_type_decl($$); } <type_decl>
%%

ast
  : section { arg->ast = $$ = new_ast($1, NULL, get_pos(scanner));  }
  | section ast { arg->ast = $$ = new_ast($1, $2, get_pos(scanner)); }
  ;

section
  : stmt_list  { $$ = new_section_stmt_list($1, get_pos(scanner)); }
  | func_def   { $$ = new_section_func_def ($1, get_pos(scanner)); }
  | class_def  { $$ = new_section_class_def($1, get_pos(scanner)); }
  ;

class_def
  : decl_template class_decl CLASS id_list class_ext LBRACE class_body RBRACE
      { $$ = new_class_def( $2, $4, $5, $7, get_pos(scanner)); $$->types = $1; }
  ;

class_ext : EXTENDS id_dot { $$ = $2; } | { $$ = NULL; };

class_decl
  : PUBLIC  { $$ = ae_flag_public; }
  |         { $$ = ae_flag_private; }
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
  : stmt_list    { $$ = new_section_stmt_list( $1, get_pos(scanner)); }
  | func_def     { $$ = new_section_func_def( $1, get_pos(scanner)); }
  | class_def    { $$ = new_section_class_def( $1, get_pos(scanner)); }
  ;

id_list
  : ID                { $$ = new_id_list( $1, get_pos(scanner)); }
  | ID COMMA id_list  { $$ = prepend_id_list( $1, $3, get_pos(scanner)); }
  ;

id_dot
  : ID                { $$ = new_id_list( $1, get_pos(scanner)); }
  | ID DOT id_dot     { $$ = prepend_id_list( $1, $3, get_pos(scanner)); }
  ;

stmt_list
  : stmt { $$ = new_stmt_list($1, NULL, get_pos(scanner));}
  | stmt stmt_list { $$ = new_stmt_list($1, $2, get_pos(scanner));}
  ;

static_decl
  : STATIC                            { $$ = ae_flag_static;   }
  |                                   { $$ = ae_flag_instance; }
  ;

function_decl
  : FUNCTION { $$ = ae_flag_func; }
  | VARARG   { $$ = ae_flag_variadic; }
  ;

func_ptr
  : FUNC_PTR type_decl2 LPAREN ID RPAREN func_args { $$ = new_func_ptr_stmt(0, $4, $2, $6, get_pos(scanner)); }
  | STATIC FUNC_PTR type_decl2 LPAREN ID RPAREN func_args { $$ = new_func_ptr_stmt(ae_flag_static, $5, $3, $7, get_pos(scanner)); }
  ;

type_decl2
  : type_decl                         { $$ = $1; }
  | type_decl array_empty             { $$ = add_type_decl_array( $1, $2, get_pos(scanner)); }
  | type_decl array_exp               { $$ = add_type_decl_array( $1, $2, get_pos(scanner)); }
  ;

arg_list
  : type_decl var_decl { $$ = new_arg_list($1, $2, NULL, get_pos(scanner)); }
  | type_decl var_decl COMMA arg_list{ $$ = new_arg_list($1, $2, $4, get_pos(scanner)); }
  ;

code_segment
  : LBRACE RBRACE { $$ = new_stmt_code( NULL, get_pos(scanner)); }
  | LBRACE stmt_list RBRACE { $$ = new_stmt_code( $2, get_pos(scanner)); }
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
  | union_stmt
  ;

enum_stmt
  : ENUM LBRACE id_list RBRACE SEMICOLON    { $$ = new_stmt_enum($3, NULL, get_pos(scanner)); }
  | ENUM LBRACE id_list RBRACE ID SEMICOLON { $$ = new_stmt_enum($3, $5, get_pos(scanner)); }
  ;

label_stmt
  : ID COLON {  $$ = new_stmt_gotolabel($1, 1, get_pos(scanner)); }
  ;

goto_stmt
  : GOTO ID SEMICOLON {  $$ = new_stmt_gotolabel($2, 0, get_pos(scanner)); }
  ;

case_stmt
  : CASE primary_exp COLON { $$ = new_stmt_case($2, get_pos(scanner)); }
  | CASE postfix_exp COLON { $$ = new_stmt_case($2, get_pos(scanner)); }
  ;

switch_stmt
  : SWITCH LPAREN exp RPAREN code_segment { $$ = new_stmt_switch($3, $5, get_pos(scanner));}
  ;

loop_stmt
  : WHILE LPAREN exp RPAREN stmt
    { $$ = new_stmt_while( $3, $5, 0, get_pos(scanner)); }
  | DO stmt WHILE LPAREN exp RPAREN SEMICOLON
    { $$ = new_stmt_while( $5, $2, 1, get_pos(scanner)); }
  | FOR LPAREN exp_stmt exp_stmt RPAREN stmt
      { $$ = new_stmt_for( $3, $4, NULL, $6, get_pos(scanner)); }
  | FOR LPAREN exp_stmt exp_stmt exp RPAREN stmt
      { $$ = new_stmt_for( $3, $4, $5, $7, get_pos(scanner)); }
  | UNTIL LPAREN exp RPAREN stmt
      { $$ = new_stmt_until( $3, $5, 0, get_pos(scanner)); }
  | DO stmt UNTIL LPAREN exp RPAREN SEMICOLON
      { $$ = new_stmt_until( $5, $2, 1, get_pos(scanner)); }
  | LOOP LPAREN exp RPAREN stmt
      { $$ = new_stmt_loop( $3, $5, get_pos(scanner)); }
  ;

selection_stmt
  : IF LPAREN exp RPAREN stmt %prec NOELSE
      { $$ = new_stmt_if( $3, $5, NULL, get_pos(scanner)); }
  | IF LPAREN exp RPAREN stmt ELSE stmt
      { $$ = new_stmt_if( $3, $5, $7, get_pos(scanner)); }
  ;

jump_stmt
  : RETURN SEMICOLON     { $$ = new_stmt_return( NULL, get_pos(scanner)); }
  | RETURN exp SEMICOLON { $$ = new_stmt_return( $2, get_pos(scanner)); }
  | BREAK SEMICOLON      { $$ = new_stmt_break(get_pos(scanner)); }
  | CONTINUE SEMICOLON   { $$ = new_stmt_continue(get_pos(scanner)); }
  ;

exp_stmt
  : exp SEMICOLON { $$ = new_stmt_expression($1,   get_pos(scanner)); }
  | SEMICOLON     { $$ = new_stmt_expression(NULL, get_pos(scanner)); }
  ;

exp
  : binary_exp            { $$ = $1; }
  | binary_exp COMMA exp  { $$ = prepend_expression($1, $3, get_pos(scanner)); }
  ;

binary_exp
  : decl_exp      { $$ = $1; }
  | binary_exp op decl_exp     { $$ = new_exp_binary($1, $2, $3, get_pos(scanner)); }
  ;

template
  : LTB type_list GTB { $$ = $2; }
  ;

op
  : CHUCK       { $$ = op_chuck; }
  | UNCHUCK     { $$ = op_unchuck; }
  | EQ          { $$ = op_eq; /* LCOV_EXCL_LINE */ }
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
  | ASSIGN      { $$ = op_assign; }
  ;

array_exp
  : LBRACK exp RBRACK           { $$ = new_array_sub( $2, get_pos(scanner) ); }
  | LBRACK exp RBRACK array_exp { $$ = prepend_array_sub( $4, $2); }
  | LBRACK exp RBRACK LBRACK RBRACK { $$ = prepend_array_sub( new_array_sub( NULL, get_pos(scanner)), $2); }
  ;

array_empty
  : LBRACK RBRACK             { $$ = new_array_sub(NULL, get_pos(scanner)); }
  | array_empty LBRACK RBRACK { $$ = prepend_array_sub($1, NULL); }
  | array_empty array_exp     { $$ = prepend_array_sub($1, $2->exp_list); }
  ;

decl_exp
  : conditional_expression
  | type_decl  var_decl_list { $$= new_exp_decl($1, $2, 0, get_pos(scanner)); }
  | LTB type_list GTB type_decl  var_decl_list { $$= new_exp_decl($4, $5, 0, get_pos(scanner)); $$->d.exp_decl.types = $2; }
  | STATIC type_decl var_decl_list { $$= new_exp_decl($2, $3, 1, get_pos(scanner)); }
  | STATIC LTB type_list GTB type_decl var_decl_list { $$= new_exp_decl($5, $6, 1, get_pos(scanner)); $$->d.exp_decl.types = $3; }
  ;

func_args
  : LPAREN RPAREN          { $$ = NULL; }
  | LPAREN arg_list RPAREN { $$ = $2; }
  ;

decl_template: { $$ = NULL; } | TEMPLATE LTB id_list GTB { $$ = $3; };

func_def
  : decl_template function_decl static_decl type_decl2 ID func_args code_segment
    { $$ = new_func_def($2 | $3, $4, $5, $6, $7, get_pos(scanner)); $$->types = $1; if($1) SET_FLAG($$, ae_flag_template);}
  | OPERATOR type_decl ID func_args code_segment
    { $$ = new_func_def(ae_flag_func | ae_flag_static | ae_flag_op , $2, $3, $4, $5, get_pos(scanner)); }
  | AST_DTOR LPAREN RPAREN code_segment
    { $$ = new_func_def(ae_flag_func | ae_flag_instance | ae_flag_dtor, new_type_decl(new_id_list("void", get_pos(scanner)), 0, 
      get_pos(scanner)), "dtor", NULL, $4, get_pos(scanner)); }
  ;

atsym: { $$ = 0; } | ATSYM { $$ = 1; };

type_decl
  : ID atsym  { $$ = new_type_decl(new_id_list($1, get_pos(scanner)), $2, get_pos(scanner)); }
  | CONST ID atsym  { $$ = new_type_decl(new_id_list($2, get_pos(scanner)), $3, get_pos(scanner)); SET_FLAG($$, ae_flag_const); }
  | LT id_dot GT atsym { $$ = new_type_decl($2, $4, get_pos(scanner)); }
  | CONST LT id_dot GT atsym { $$ = new_type_decl($3, $5, get_pos(scanner)); SET_FLAG($$, ae_flag_const); }
  | TYPEOF LPAREN id_dot RPAREN atsym { $$ = new_type_decl2($3, $5, get_pos(scanner)); }
  | CONST TYPEOF LPAREN id_dot RPAREN atsym { $$ = new_type_decl2($4, $6, get_pos(scanner)); SET_FLAG($$, ae_flag_const); }
  ;


decl_list
  : exp SEMICOLON { $$ = new_decl_list($1, NULL); }
  | exp SEMICOLON decl_list { $$ = new_decl_list($1, $3); }
  ;

union_stmt
  : UNION LBRACE decl_list RBRACE SEMICOLON { $$ = new_stmt_union($3, get_pos(scanner)); }
  ;

var_decl_list
  : var_decl  { $$ = new_var_decl_list($1, NULL, get_pos(scanner)); }
  | var_decl  COMMA var_decl_list { $$ = new_var_decl_list($1, $3, get_pos(scanner)); }
  ;

var_decl
  : ID              { $$ = new_var_decl($1, NULL, get_pos(scanner)); }
  | ID array_exp    { $$ = new_var_decl($1,   $2, get_pos(scanner)); }
  | ID array_empty  { $$ = new_var_decl($1,   $2, get_pos(scanner)); }
  ;

complex_exp: SHARPPAREN   exp RPAREN { $$ = new_complex( $2, get_pos(scanner)); };
polar_exp:   PERCENTPAREN exp RPAREN { $$ = new_polar(   $2, get_pos(scanner)); };
vec_exp:     ATPAREN      exp RPAREN { $$ = new_vec(     $2, get_pos(scanner)); };

conditional_expression
  : logical_or_expression
  | logical_or_expression QUESTION exp COLON conditional_expression
      { $$ = new_exp_if( $1, $3, $5, get_pos(scanner)); }
  ;

logical_or_expression
  : logical_and_expression            { $$ = $1; }
  | logical_or_expression OR logical_and_expression
      { $$ = new_exp_binary( $1, op_or, $3, get_pos(scanner)); }
  ;

logical_and_expression
  : inclusive_or_expression           { $$ = $1; }
  | logical_and_expression AND inclusive_or_expression
      { $$ = new_exp_binary( $1, op_and, $3, get_pos(scanner)); }
  ;

inclusive_or_expression
  : exclusive_or_expression           { $$ = $1; }
  | inclusive_or_expression S_OR exclusive_or_expression
      { $$ = new_exp_binary( $1, op_s_or, $3, get_pos(scanner)); }
  ;

exclusive_or_expression
  : and_expression                    { $$ = $1; }
  | exclusive_or_expression S_XOR and_expression
      { $$ = new_exp_binary( $1, op_s_xor, $3, get_pos(scanner)); }
  ;

and_expression
  : equality_expression               { $$ = $1; }
  | and_expression S_AND equality_expression
      { $$ = new_exp_binary( $1, op_s_and, $3, get_pos(scanner)); }
  ;

equality_op : EQ { $$ = op_eq; } | NEQ { $$ = op_neq; };
equality_expression
  : relational_expression             { $$ = $1; }
  | equality_expression equality_op relational_expression
    { $$ = new_exp_binary( $1, $2, $3, get_pos(scanner)); }
  ;

relationnal_op: LT { $$ = op_lt; } | GT { $$ = op_gt; } | LE { $$ = op_le; } | GE { $$ = op_ge; };

relational_expression
  : shift_expression                  { $$ = $1; }
  | relational_expression relationnal_op shift_expression
    { $$ = new_exp_binary( $1, $2, $3, get_pos(scanner)); }
  ;

shift_op
  : SHIFT_LEFT  { $$ = op_shift_left;  }
  | SHIFT_RIGHT { $$ = op_shift_right; }
  ;

shift_expression
  : additive_expression               { $$ = $1; }
  | shift_expression shift_op  additive_expression
    { $$ = new_exp_binary( $1, $2, $3, get_pos(scanner)); }
  ;

additive_op: PLUS { $$ = op_plus; } | MINUS { $$ = op_minus; };

additive_expression
  : multiplicative_expression          { $$ = $1; }
  | additive_expression additive_op multiplicative_expression
    { $$ = new_exp_binary( $1, $2, $3, get_pos(scanner)); }
  ;

multiplicative_op: TIMES { $$ = op_times; } | DIVIDE { $$ = op_divide; } | PERCENT { $$ = op_percent; };

multiplicative_expression
  : cast_exp { $$ = $1; }
  | multiplicative_expression multiplicative_op cast_exp
      { $$ = new_exp_binary( $1, $2, $3, get_pos(scanner)); }
  ;

cast_exp
  : unary_expression
  | cast_exp DOLLAR type_decl
      { $$ = new_exp_cast( $3, $1, get_pos(scanner)); }
  ;

unary_op : PLUS { $$ = op_plus; } | MINUS %prec NEG { $$ = op_minus; } | TIMES { $$ = op_times; }
  | PLUSPLUS { $$ = op_plusplus; } | MINUSMINUS { $$ = op_minusminus; }
  | EXCLAMATION { $$ = op_exclamation; } | SPORK TILDA { $$ = op_spork; }
  ;

unary_expression
  : dur_exp { $$ = $1; }
  | unary_op unary_expression
      { $$ = new_exp_unary( $1, $2, get_pos(scanner)); }
  | NEW type_decl
      { $$ = new_exp_unary2(op_new, $2, NULL, get_pos(scanner)); }
  | NEW type_decl array_exp
      { $$ = new_exp_unary2(op_new, $2, $3, get_pos(scanner)); }
  | SPORK TILDA code_segment
        { $$ = new_exp_unary3( op_spork, $3, get_pos(scanner)); }
  ;


dur_exp
  : postfix_exp
  | dur_exp COLONCOLON postfix_exp { $$ = new_exp_dur( $1, $3, get_pos(scanner)); }
  ;

type_list
  : ID { $$ = new_type_list(new_id_list($1, get_pos(scanner)), NULL, get_pos(scanner)); }
  | ID COMMA type_list{ $$ = new_type_list(new_id_list($1, get_pos(scanner)), $3, get_pos(scanner)); }
  ;

call_template : { $$ = NULL; } | template { $$ = $1;} ;
call_paren : LPAREN RPAREN { $$ = NULL; } | LPAREN exp RPAREN { $$ = $2; } ;



postfix_op : PLUSPLUS { $$ = op_plusplus; } | MINUSMINUS { $$ = op_minusminus; };

postfix_exp
  : primary_exp
  | postfix_exp array_exp
    { $$ = new_array( $1, $2, get_pos(scanner)); }
  | postfix_exp call_template call_paren
    { $$ = new_exp_call( $1, $3, get_pos(scanner)); $$->d.exp_func.types = $2; }  ;
  | postfix_exp DOT ID
    { $$ = new_exp_dot( $1, $3, get_pos(scanner)); }
  | postfix_exp postfix_op
    { $$ = new_exp_postfix( $1, $2, get_pos(scanner)); }
  ;

primary_exp
  : ID                { $$ = new_exp_prim_ID(      $1, get_pos(scanner)); }
  | NUM               { $$ = new_exp_prim_int(     $1, get_pos(scanner)); }
  | FLOAT             { $$ = new_exp_prim_float(   $1, get_pos(scanner)); }
  | STRING_LIT        { $$ = new_exp_prim_string(  $1, get_pos(scanner)); }
  | CHAR_LIT          { $$ = new_exp_prim_char(    $1, get_pos(scanner)); }
  | array_exp         { $$ = new_exp_prim_array(   $1, get_pos(scanner)); }
  | array_empty       { $$ = new_exp_prim_array(   $1, get_pos(scanner)); }
  | complex_exp       { $$ = new_exp_prim_complex( $1, get_pos(scanner)); }
  | polar_exp         { $$ = new_exp_prim_polar(   $1, get_pos(scanner)); }
  | vec_exp           { $$ = new_exp_prim_vec(     $1, get_pos(scanner)); }
  | L_HACK exp R_HACK { $$ = new_exp_prim_hack(    $2, get_pos(scanner)); }
  | LPAREN exp RPAREN { $$ =                       $2;                    }
  | LPAREN RPAREN     { $$ = new_exp_prim_nil(         get_pos(scanner)); }
  ;
%%
