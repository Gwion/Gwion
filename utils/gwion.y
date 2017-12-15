%define api.pure full
%parse-param { Scanner* arg }
%lex-param  { void* scan }
%name-prefix "gwion_"
%{
#include "absyn.h"
#include "scanner.h"
#define scan arg->scanner

void gwion_error(void* data, const char* s);
int gwion_lex(void*, void*);
int get_pos(void* data);
%}

%union {
  char* sval;
  int ival;
  m_float fval;
  Complex* c_val;
  Polar* polar;
  Vec* vec;
  Array_Sub array_sub;
  Var_Decl var_decl;
  Var_Decl_List var_decl_list;
  Type_Decl* type_decl;
  Exp   exp;
  Stmt_Ptr func_ptr;
  Stmt stmt;
  Stmt_List stmt_list;
  Arg_List arg_list;
  Decl_List decl_list;
  Func_Def func_def;
  Section* section;
  ID_List id_list;
  Type_List type_list;
  Class_Body class_body;
  ID_List class_ext;
  Class_Def class_def;
  Ast ast;
};

%token SEMICOLON CHUCK COMMA ASSIGN DIVIDE TIMES PERCENT L_HACK R_HACK
  LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE PLUSCHUCK MINUSCHUCK TIMESCHUCK
  DIVIDECHUCK MODULOCHUCK ATCHUCK UNCHUCK TRIG UNTRIG PERCENTPAREN SHARPPAREN
  ATSYM FUNCTION DOLLAR TILDA QUESTION COLON EXCLAMATION IF ELSE WHILE DO UNTIL
  LOOP FOR GOTO SWITCH CASE ENUM RETURN BREAK CONTINUE PLUSPLUS MINUSMINUS NEW
  SPORK CLASS STATIC PUBLIC PRIVATE EXTENDS DOT COLONCOLON AND EQ GE GT LE LT
  MINUS PLUS NEQ SHIFT_LEFT SHIFT_RIGHT S_AND S_OR S_XOR OR AST_DTOR OPERATOR
  FUNC_PTR RSL RSR RSAND RSOR RSXOR RAND ROR REQ RNEQ RGT RGE RLT RLE TEMPLATE
  NOELSE LTB GTB VARARG UNION ATPAREN TYPEOF CONST

%token<ival> NUM
%type<ival>op shift_op post_op rel_op eq_op unary_op add_op mul_op
%type<ival> atsym class_decl static_decl function_decl
%token<fval> FLOAT
%token<sval> ID STRING_LIT CHAR_LIT
%type<sval>opt_id
%type<var_decl> var_decl
%type<var_decl_list> var_decl_list
%type<type_decl> type_decl type_decl2
%type<exp> primary_exp decl_exp private_decl binary_exp call_paren
%type<exp> con_exp log_or_exp log_and_exp inc_or_exp exc_or_exp and_exp eq_exp
%type<exp> relational_exp shift_exp add_exp mul_exp unary_exp dur_exp
%type<exp> post_exp cast_exp exp
%type<array_sub> array_exp array_empty
%type<polar> polar_exp
%type<c_val> complex_exp
%type<vec> vec_exp
%type<stmt> stmt loop_stmt selection_stmt jump_stmt code_segment exp_stmt
%type<stmt> case_stmt label_stmt goto_stmt switch_stmt
%type<stmt> enum_stmt func_ptr stmt_typedef union_stmt
%type<stmt_list> stmt_list
%type<arg_list> arg_list func_args
%type<decl_list> decl_list
%type<func_def> func_def
%type<section> section
%type<class_def> class_def
%type<id_list> class_ext
%type<class_body> class_body class_body2
%type<id_list> id_list id_dot decl_template
%type<type_list> type_list template call_template
%type<section> class_section
%type<ast> ast

%start ast

%nonassoc NOELSE
%nonassoc ELSE

%precedence NEG
%left DOT

%destructor { free_stmt($$); } <stmt>
%destructor { free_class_def($$); } <class_def>
%destructor { free_class_body($$); } <class_body>
%destructor { free_type_decl($$); } <type_decl>
%destructor { free_type_list($$); } <type_list>
%%

ast
  : section { arg->ast = $$ = new_ast($1, NULL, get_pos(scan));  }
  | section ast { arg->ast = $$ = new_ast($1, $2, get_pos(scan)); }
  ;

section
  : stmt_list  { $$ = new_section_stmt_list($1, get_pos(scan)); }
  | func_def   { $$ = new_section_func_def ($1, get_pos(scan)); }
  | class_def  { $$ = new_section_class_def($1, get_pos(scan)); }
  ;

class_def
  : decl_template class_decl CLASS id_list class_ext LBRACE class_body RBRACE
      { $$ = new_class_def( $2, $4, $5, $7, get_pos(scan)); $$->types = $1; }
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
  : class_section             { $$ = new_class_body( $1, get_pos(scan)); }
  | class_section class_body2 { $$ = prepend_class_body( $1, $2, get_pos(scan)); }
  ;

class_section
  : stmt_list    { $$ = new_section_stmt_list( $1, get_pos(scan)); }
  | func_def     { $$ = new_section_func_def( $1, get_pos(scan)); }
  | class_def    { $$ = new_section_class_def( $1, get_pos(scan)); }
  ;

id_list
  : ID                { $$ = new_id_list( $1, get_pos(scan)); }
  | ID COMMA id_list  { $$ = prepend_id_list( $1, $3, get_pos(scan)); }
  ;

id_dot
  : ID                { $$ = new_id_list( $1, get_pos(scan)); }
  | ID DOT id_dot     { $$ = prepend_id_list( $1, $3, get_pos(scan)); }
  ;

stmt_list
  : stmt { $$ = new_stmt_list($1, NULL, get_pos(scan));}
  | stmt stmt_list { $$ = new_stmt_list($1, $2, get_pos(scan));}
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
  : FUNC_PTR type_decl2 LPAREN ID RPAREN func_args { $$ = new_func_ptr_stmt(0, $4, $2, $6, get_pos(scan)); }
  | STATIC FUNC_PTR type_decl2 LPAREN ID RPAREN func_args { $$ = new_func_ptr_stmt(ae_flag_static, $5, $3, $7, get_pos(scan)); }
  ;

stmt_typedef: FUNC_PTR type_decl2 ID SEMICOLON { if($2->array->exp_list) gwion_error(&scan, ("array must be empty in typedef expression.")); $$ = new_stmt_typedef($2, $3, get_pos(scan)); };

type_decl2
  : type_decl                         { $$ = $1; }
  | type_decl array_empty             { $$ = add_type_decl_array( $1, $2, get_pos(scan)); }
  | type_decl array_exp               { $$ = add_type_decl_array( $1, $2, get_pos(scan)); }
  ;

arg_list
  : type_decl var_decl { $$ = new_arg_list($1, $2, NULL, get_pos(scan)); }
  | type_decl var_decl COMMA arg_list{ $$ = new_arg_list($1, $2, $4, get_pos(scan)); }
  ;

code_segment
  : LBRACE RBRACE { $$ = new_stmt_code( NULL, get_pos(scan)); }
  | LBRACE stmt_list RBRACE { $$ = new_stmt_code( $2, get_pos(scan)); }
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
  | stmt_typedef
  | union_stmt
  ;

opt_id
  : { $$ = NULL; }
  | ID { $$ = $1;  }
  ;

enum_stmt
  : ENUM LBRACE id_list RBRACE opt_id SEMICOLON    { $$ = new_stmt_enum($3, $5, get_pos(scan)); }
  ;

label_stmt
  : ID COLON {  $$ = new_stmt_gotolabel($1, 1, get_pos(scan)); }
  ;

goto_stmt
  : GOTO ID SEMICOLON {  $$ = new_stmt_gotolabel($2, 0, get_pos(scan)); }
  ;

case_stmt
  : CASE primary_exp COLON { $$ = new_stmt_case($2, get_pos(scan)); }
  | CASE post_exp COLON { $$ = new_stmt_case($2, get_pos(scan)); }
  ;

switch_stmt
  : SWITCH LPAREN exp RPAREN code_segment { $$ = new_stmt_switch($3, $5, get_pos(scan));}
  ;

loop_stmt
  : WHILE LPAREN exp RPAREN stmt
    { $$ = new_stmt_while( $3, $5, 0, get_pos(scan)); }
  | DO stmt WHILE LPAREN exp RPAREN SEMICOLON
    { $$ = new_stmt_while( $5, $2, 1, get_pos(scan)); }
  | FOR LPAREN exp_stmt exp_stmt RPAREN stmt
      { $$ = new_stmt_for( $3, $4, NULL, $6, get_pos(scan)); }
  | FOR LPAREN exp_stmt exp_stmt exp RPAREN stmt
      { $$ = new_stmt_for( $3, $4, $5, $7, get_pos(scan)); }
  | UNTIL LPAREN exp RPAREN stmt
      { $$ = new_stmt_until( $3, $5, 0, get_pos(scan)); }
  | DO stmt UNTIL LPAREN exp RPAREN SEMICOLON
      { $$ = new_stmt_until( $5, $2, 1, get_pos(scan)); }
  | LOOP LPAREN exp RPAREN stmt
      { $$ = new_stmt_loop( $3, $5, get_pos(scan)); }
  ;

selection_stmt
  : IF LPAREN exp RPAREN stmt %prec NOELSE
      { $$ = new_stmt_if( $3, $5, NULL, get_pos(scan)); }
  | IF LPAREN exp RPAREN stmt ELSE stmt
      { $$ = new_stmt_if( $3, $5, $7, get_pos(scan)); }
  ;

jump_stmt
  : RETURN SEMICOLON     { $$ = new_stmt_return( NULL, get_pos(scan)); }
  | RETURN exp SEMICOLON { $$ = new_stmt_return( $2, get_pos(scan)); }
  | BREAK SEMICOLON      { $$ = new_stmt_break(get_pos(scan)); }
  | CONTINUE SEMICOLON   { $$ = new_stmt_continue(get_pos(scan)); }
  ;

exp_stmt
  : exp SEMICOLON { $$ = new_stmt_exp($1,   get_pos(scan)); }
  | SEMICOLON     { $$ = new_stmt_exp(NULL, get_pos(scan)); }
  ;

exp
  : binary_exp            { $$ = $1; }
  | binary_exp COMMA exp  { $$ = prepend_exp($1, $3, get_pos(scan)); }
  ;

binary_exp
  : decl_exp      { $$ = $1; }
  | binary_exp op decl_exp     { $$ = new_exp_binary($1, $2, $3, get_pos(scan)); }
  ;

template
  : LTB type_list GTB { $$ = $2; }
  ;

op: CHUCK { $$ = op_chuck; } | UNCHUCK { $$ = op_unchuck; } | EQ { $$ = op_eq; }
  | ATCHUCK     { $$ = op_at_chuck; } | PLUSCHUCK   { $$ = op_plus_chuck; }
  | MINUSCHUCK  { $$ = op_minus_chuck; } | TIMESCHUCK  { $$ = op_times_chuck; }
  | DIVIDECHUCK { $$ = op_divide_chuck; } | MODULOCHUCK { $$ = op_modulo_chuck; }
  | TRIG { $$ = op_trig; } | UNTRIG { $$ = op_untrig; }
  | RSL { $$ = op_rsl; } | RSR { $$ = op_rsr; } | RSAND { $$ = op_rsand; }
  | RSOR { $$ = op_rsor; } | RSXOR { $$ = op_rsxor; } | RAND { $$ = op_rand; }
  | ROR { $$ = op_ror; } | REQ { $$ = op_req; } | RNEQ { $$ = op_rneq; }
  | RGT { $$ = op_rgt; } | RGE { $$ = op_rge; } | RLT { $$ = op_rlt; }
  | RLE { $$ = op_rle; } | ASSIGN { $$ = op_assign; }
  ;

array_exp
  : LBRACK exp RBRACK           { $$ = new_array_sub( $2, get_pos(scan) ); }
  | LBRACK exp RBRACK array_exp { if($2->next){ yyerror(&scan, "invalid format for array init [...][...]..."); free_exp($2); free_array_sub($4); YYERROR; } $$ = prepend_array_sub( $4, $2); }
  | LBRACK exp RBRACK LBRACK RBRACK { yyerror(&scan, "partially empty array init [...][]..."); free_exp($2); YYERROR; }
  ;

array_empty
  : LBRACK RBRACK             { $$ = new_array_sub(NULL, get_pos(scan)); }
  | array_empty LBRACK RBRACK { $$ = prepend_array_sub($1, NULL); }
  | array_empty array_exp     { yyerror(&scan, "partially empty array init [][...]"); free_array_sub($1); free_array_sub($2); YYERROR; }
  ;

decl_exp
  : con_exp
  | type_decl var_decl_list { $$= new_exp_decl($1, $2, get_pos(scan)); }
  | LTB type_list GTB type_decl  var_decl_list { $$= new_exp_decl($4, $5, get_pos(scan)); $$->d.exp_decl.types = $2; }
  | STATIC type_decl var_decl_list { $$= new_exp_decl($2, $3, get_pos(scan)); SET_FLAG($$->d.exp_decl.type, ae_flag_static); }
  | STATIC LTB type_list GTB type_decl var_decl_list { $$= new_exp_decl($5, $6, get_pos(scan)); SET_FLAG($$->d.exp_decl.type, ae_flag_static); $$->d.exp_decl.types = $3; }
  | private_decl
  ;

private_decl
  : PRIVATE type_decl var_decl_list { $$= new_exp_decl($2, $3, get_pos(scan)); SET_FLAG($$->d.exp_decl.type, ae_flag_private); }
  | PRIVATE LTB type_list GTB type_decl  var_decl_list { $$= new_exp_decl($5, $6, get_pos(scan)); SET_FLAG($$->d.exp_decl.type, ae_flag_private); $$->d.exp_decl.types = $3; }
  | PRIVATE STATIC type_decl var_decl_list { $$= new_exp_decl($3, $4, get_pos(scan)); SET_FLAG($$->d.exp_decl.type, ae_flag_private | ae_flag_static); }
  | PRIVATE STATIC LTB type_list GTB type_decl var_decl_list { $$= new_exp_decl($6, $7, get_pos(scan));  SET_FLAG($$->d.exp_decl.type, ae_flag_private | ae_flag_static); $$->d.exp_decl.types = $4; }
  ;

func_args
  : LPAREN RPAREN          { $$ = NULL; }
  | LPAREN arg_list RPAREN { $$ = $2; }
  ;

decl_template: { $$ = NULL; } | TEMPLATE LTB id_list GTB { $$ = $3; };

func_def
  : decl_template function_decl static_decl type_decl2 ID func_args code_segment
    { $$ = new_func_def($2 | $3, $4, $5, $6, $7, get_pos(scan)); $$->types = $1; if($1) SET_FLAG($$, ae_flag_template);}
  | PRIVATE decl_template function_decl static_decl type_decl2 ID func_args code_segment
    { $$ = new_func_def($3 | $4, $5, $6, $7, $8, get_pos(scan)); $$->types = $2; if($2) SET_FLAG($$, ae_flag_template); SET_FLAG($$, ae_flag_private); }
  | OPERATOR type_decl2 ID func_args code_segment
    { $$ = new_func_def(ae_flag_func | ae_flag_static | ae_flag_op , $2, $3, $4, $5, get_pos(scan)); }
  | AST_DTOR LPAREN RPAREN code_segment
    { $$ = new_func_def(ae_flag_func | ae_flag_instance | ae_flag_dtor, new_type_decl(new_id_list("void", get_pos(scan)), 0,
      get_pos(scan)), "dtor", NULL, $4, get_pos(scan)); }
  ;

atsym: { $$ = 0; } | ATSYM { $$ = 1; };

type_decl
  : ID atsym  { $$ = new_type_decl(new_id_list($1, get_pos(scan)), $2, get_pos(scan)); }
  | CONST ID atsym  { $$ = new_type_decl(new_id_list($2, get_pos(scan)), $3, get_pos(scan)); SET_FLAG($$, ae_flag_const); }
  | LT id_dot GT atsym { $$ = new_type_decl($2, $4, get_pos(scan)); }
  | CONST LT id_dot GT atsym { $$ = new_type_decl($3, $5, get_pos(scan)); SET_FLAG($$, ae_flag_const); }
  | TYPEOF LPAREN id_dot RPAREN atsym { $$ = new_type_decl2($3, $5, get_pos(scan)); }
  | CONST TYPEOF LPAREN id_dot RPAREN atsym { $$ = new_type_decl2($4, $6, get_pos(scan)); SET_FLAG($$, ae_flag_const); }
  ;


decl_list
  : exp SEMICOLON { $$ = new_decl_list($1, NULL); }
  | exp SEMICOLON decl_list { $$ = new_decl_list($1, $3); }
  ;

union_stmt
  : UNION LBRACE decl_list RBRACE opt_id SEMICOLON { $$ = new_stmt_union($3, get_pos(scan));$$->d.stmt_union.xid = $5 ? insert_symbol($5) : NULL; }
  | STATIC UNION LBRACE decl_list RBRACE opt_id SEMICOLON { $$ = new_stmt_union($4, get_pos(scan));$$->d.stmt_union.xid = $6 ? insert_symbol($6) : NULL; $$->d.stmt_union.flag |= ae_flag_static; }
  | PRIVATE UNION LBRACE decl_list RBRACE opt_id SEMICOLON { $$ = new_stmt_union($4, get_pos(scan));$$->d.stmt_union.xid = $6 ? insert_symbol($6) : NULL; $$->d.stmt_union.flag |= ae_flag_private; }
  | PRIVATE STATIC UNION LBRACE decl_list RBRACE opt_id SEMICOLON { $$ = new_stmt_union($5, get_pos(scan));$$->d.stmt_union.xid = $7 ? insert_symbol($7) : NULL; $$->d.stmt_union.flag |= ae_flag_private | ae_flag_static; }
  ;

var_decl_list
  : var_decl  { $$ = new_var_decl_list($1, NULL, get_pos(scan)); }
  | var_decl  COMMA var_decl_list { $$ = new_var_decl_list($1, $3, get_pos(scan)); }
  ;

var_decl
  : ID              { $$ = new_var_decl($1, NULL, get_pos(scan)); }
  | ID array_exp    { $$ = new_var_decl($1,   $2, get_pos(scan)); }
  | ID array_empty  { $$ = new_var_decl($1,   $2, get_pos(scan)); }
  ;

complex_exp: SHARPPAREN   exp RPAREN { $$ = new_complex( $2, get_pos(scan)); };
           polar_exp:   PERCENTPAREN exp RPAREN { $$ = new_polar(   $2, get_pos(scan)); };
vec_exp:     ATPAREN      exp RPAREN { $$ = new_vec(     $2, get_pos(scan)); };

con_exp
  : log_or_exp
  | log_or_exp QUESTION exp COLON con_exp
      { $$ = new_exp_if( $1, $3, $5, get_pos(scan)); }
  ;

log_or_exp
  : log_and_exp            { $$ = $1; }
  | log_or_exp OR log_and_exp
      { $$ = new_exp_binary( $1, op_or, $3, get_pos(scan)); }
  ;

log_and_exp
  : inc_or_exp           { $$ = $1; }
  | log_and_exp AND inc_or_exp
      { $$ = new_exp_binary( $1, op_and, $3, get_pos(scan)); }
  ;

inc_or_exp
  : exc_or_exp           { $$ = $1; }
  | inc_or_exp S_OR exc_or_exp
      { $$ = new_exp_binary( $1, op_s_or, $3, get_pos(scan)); }
  ;

exc_or_exp
  : and_exp                    { $$ = $1; }
  | exc_or_exp S_XOR and_exp
      { $$ = new_exp_binary( $1, op_s_xor, $3, get_pos(scan)); }
  ;

and_exp
  : eq_exp               { $$ = $1; }
  | and_exp S_AND eq_exp
      { $$ = new_exp_binary( $1, op_s_and, $3, get_pos(scan)); }
  ;

eq_op : EQ { $$ = op_eq; } | NEQ { $$ = op_neq; };
            eq_exp
  : relational_exp             { $$ = $1; }
  | eq_exp eq_op relational_exp
    { $$ = new_exp_binary( $1, $2, $3, get_pos(scan)); }
  ;

rel_op: LT { $$ = op_lt; } | GT { $$ = op_gt; } | LE { $$ = op_le; } | GE { $$ = op_ge; };

relational_exp
  : shift_exp                  { $$ = $1; }
  | relational_exp rel_op shift_exp
    { $$ = new_exp_binary( $1, $2, $3, get_pos(scan)); }
  ;

shift_op
  : SHIFT_LEFT  { $$ = op_shift_left;  }
  | SHIFT_RIGHT { $$ = op_shift_right; }
  ;

shift_exp
  : add_exp               { $$ = $1; }
  | shift_exp shift_op  add_exp
    { $$ = new_exp_binary( $1, $2, $3, get_pos(scan)); }
  ;

add_op: PLUS { $$ = op_plus; } | MINUS { $$ = op_minus; };

add_exp
  : mul_exp          { $$ = $1; }
  | add_exp add_op mul_exp
    { $$ = new_exp_binary( $1, $2, $3, get_pos(scan)); }
  ;

mul_op: TIMES { $$ = op_times; } | DIVIDE { $$ = op_divide; } | PERCENT { $$ = op_percent; };

mul_exp
  : cast_exp { $$ = $1; }
  | mul_exp mul_op cast_exp
      { $$ = new_exp_binary( $1, $2, $3, get_pos(scan)); }
  ;

cast_exp
  : unary_exp
  | cast_exp DOLLAR type_decl
      { $$ = new_exp_cast( $3, $1, get_pos(scan)); }
  ;

unary_op : PLUS { $$ = op_plus; } | MINUS %prec NEG { $$ = op_minus; } | TIMES { $$ = op_times; }
         | PLUSPLUS { $$ = op_plusplus; } | MINUSMINUS { $$ = op_minusminus; }
  | EXCLAMATION { $$ = op_exclamation; } | SPORK TILDA { $$ = op_spork; }
  ;

unary_exp
  : dur_exp { $$ = $1; }
  | unary_op unary_exp
      { $$ = new_exp_unary( $1, $2, get_pos(scan)); }
  | NEW type_decl
      { $$ = new_exp_unary2(op_new, $2, NULL, get_pos(scan)); }
  | NEW type_decl array_exp
      { $$ = new_exp_unary2(op_new, $2, $3, get_pos(scan)); }
  | SPORK TILDA code_segment
        { $$ = new_exp_unary3( op_spork, $3, get_pos(scan)); }
  ;


dur_exp
  : post_exp
  | dur_exp COLONCOLON post_exp { $$ = new_exp_dur( $1, $3, get_pos(scan)); }
  ;

type_list
  : ID { $$ = new_type_list(new_id_list($1, get_pos(scan)), NULL, get_pos(scan)); }
  | ID COMMA type_list{ $$ = new_type_list(new_id_list($1, get_pos(scan)), $3, get_pos(scan)); }
  ;

call_template : { $$ = NULL; } | template { $$ = $1;} ;
              call_paren : LPAREN RPAREN { $$ = NULL; } | LPAREN exp RPAREN { $$ = $2; } ;

post_op : PLUSPLUS { $$ = op_plusplus; } | MINUSMINUS { $$ = op_minusminus; };

post_exp
  : primary_exp
  | post_exp array_exp
    { $$ = new_array( $1, $2, get_pos(scan)); }
  | post_exp call_template call_paren
    { $$ = new_exp_call( $1, $3, get_pos(scan)); $$->d.exp_func.types = $2; }  ;
  | post_exp DOT ID
    { $$ = new_exp_dot( $1, $3, get_pos(scan)); }
  | post_exp post_op
    { $$ = new_exp_post( $1, $2, get_pos(scan)); }
  ;

primary_exp
  : ID                { $$ = new_exp_prim_ID(      $1, get_pos(scan)); }
  | NUM               { $$ = new_exp_prim_int(     $1, get_pos(scan)); }
  | FLOAT             { $$ = new_exp_prim_float(   $1, get_pos(scan)); }
  | STRING_LIT        { $$ = new_exp_prim_string(  $1, get_pos(scan)); }
  | CHAR_LIT          { $$ = new_exp_prim_char(    $1, get_pos(scan)); }
  | array_exp         { $$ = new_exp_prim_array(   $1, get_pos(scan)); }
  | array_empty       { $$ = new_exp_prim_array(   $1, get_pos(scan)); }
  | complex_exp       { $$ = new_exp_prim_complex( $1, get_pos(scan)); }
  | polar_exp         { $$ = new_exp_prim_polar(   $1, get_pos(scan)); }
  | vec_exp           { $$ = new_exp_prim_vec(     $1, get_pos(scan)); }
  | L_HACK exp R_HACK { $$ = new_exp_prim_hack(    $2, get_pos(scan)); }
  | LPAREN exp RPAREN { $$ =                       $2;                    }
  | LPAREN RPAREN     { $$ = new_exp_prim_nil(         get_pos(scan)); }
  ;
%%
