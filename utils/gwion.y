%define api.pure full
%parse-param { Scanner* arg }
%lex-param  { void* scan }
%name-prefix "gwion_"
%{
#include "absyn.h"
#include "scanner.h"
#define scan arg->scanner
#define CHECK_FLAG(a,b,c) if(GET_FLAG(b, c)) { gwion_error(a, "flag set twice");  ; } SET_FLAG(b, c);
#define CHECK_TEMPLATE(a, b, c, free_function) { if(c->tmpl) {\
        free_id_list(b);\
        free_function(c);\
        gwion_error(a, "double template decl");\
        YYERROR;\
      }\
      c->tmpl = new_tmpl_list(b, 1);\
    };
#define OP_SYM(a) insert_symbol(op2str(a))
int gwion_error(Scanner*, const char*);
int gwion_lex(void*, Scanner*);
int get_pos(Scanner*);

m_str op2str(Operator op);
%}

%union {
  char* sval;
  int ival;
  m_float fval;
  S_Symbol sym;
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
  TYPEDEF RSL RSR RSAND RSOR RSXOR TEMPLATE
  NOELSE LTB GTB VARARG UNION ATPAREN TYPEOF CONST AUTO AUTO_PTR

%token<ival> NUM
%type<ival>op shift_op post_op rel_op eq_op unary_op add_op mul_op op_op
%type<ival> atsym static_decl function_decl
%token<fval> FLOAT
%token<sval> ID STRING_LIT CHAR_LIT
%type<sym>id opt_id
%type<var_decl> var_decl
%type<var_decl_list> var_decl_list
%type<type_decl> type_decl type_decl2 class_ext
%type<exp> primary_exp decl_exp binary_exp call_paren
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
%type<func_def> func_def func_def_base
%type<section> section
%type<class_def> class_def
%type<class_body> class_body class_body2
%type<id_list> id_list id_dot decl_template
%type<type_list> type_list template
%type<ast> ast

%start ast

%nonassoc NOELSE
%nonassoc ELSE

%destructor { free_stmt($$); } <stmt>
%destructor { free_class_def($$); } <class_def>
%destructor { free_class_body($$); } <class_body>
%destructor { free_type_decl($$); } <type_decl>
%destructor { free_type_list($$); } <type_list>
%destructor { free_exp($$); } <exp>

%expect 48
%%

ast
  : section { arg->ast = $$ = new_ast($1, NULL, get_pos(arg));  }
  | section ast { arg->ast = $$ = new_ast($1, $2, get_pos(arg)); }
  ;

section
  : stmt_list  { $$ = new_section_stmt_list($1, get_pos(arg)); }
  | func_def   { $$ = new_section_func_def ($1, get_pos(arg)); }
  | class_def  { $$ = new_section_class_def($1, get_pos(arg)); }
  ;

class_def
  : CLASS id_list class_ext LBRACE class_body RBRACE
      { $$ = new_class_def(0, $2, $3, $5, get_pos(arg)); }
  | PUBLIC class_def { CHECK_FLAG(arg, $2, ae_flag_global); $$ = $2; }
  | decl_template class_def
    { CHECK_TEMPLATE(arg, $1, $2, free_class_def); $$ = $2; }

class_ext : EXTENDS type_decl2 { $$ = $2; } | { $$ = NULL; };

class_body : class_body2 | { $$ = NULL; };

class_body2
  : section             { $$ = new_class_body($1, NULL, get_pos(arg)); }
  | section class_body2 { $$ = new_class_body($1, $2, get_pos(arg)); }
  ;

id_list
  : id                { $$ = new_id_list($1, get_pos(arg)); }
  | id COMMA id_list  { $$ = prepend_id_list($1, $3, get_pos(arg)); }
  ;

id_dot
  : id                { $$ = new_id_list($1, get_pos(arg)); }
  | id DOT id_dot     { $$ = prepend_id_list($1, $3, get_pos(arg)); }
  ;

stmt_list
  : stmt { $$ = new_stmt_list($1, NULL, get_pos(arg));}
  | stmt stmt_list { $$ = new_stmt_list($1, $2, get_pos(arg));}
  ;

static_decl
  : STATIC                            { $$ = ae_flag_static;   }
  |                                   { $$ = 0; }
  ;

function_decl
  : FUNCTION { $$ = 0; }
  | VARARG   { $$ = ae_flag_variadic; }
  ;

func_ptr
  : TYPEDEF type_decl2 LPAREN id RPAREN func_args { $$ = new_func_ptr_stmt(0, $4, $2, $6, get_pos(arg)); }
  | STATIC func_ptr
    { CHECK_FLAG(arg, (&$2->d.stmt_ptr), ae_flag_static); $$ = $2; }
  ;

stmt_typedef
  :
  TYPEDEF type_decl2 id SEMICOLON
  { $$ = new_stmt_typedef($2, $3, get_pos(arg)); };

type_decl2
  : type_decl
  | type_decl array_empty             { $$ = add_type_decl_array($1, $2, get_pos(arg)); }
  | type_decl array_exp               { $$ = add_type_decl_array($1, $2, get_pos(arg)); }
  ;

arg_list
  : type_decl var_decl { $$ = new_arg_list($1, $2, NULL, get_pos(arg)); }
  | type_decl var_decl COMMA arg_list{ $$ = new_arg_list($1, $2, $4, get_pos(arg)); }
  ;

code_segment
  : LBRACE RBRACE { $$ = new_stmt_code(NULL, get_pos(arg)); }
  | LBRACE stmt_list RBRACE { $$ = new_stmt_code($2, get_pos(arg)); }
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

id: ID { $$ = insert_symbol($1); }

opt_id: { $$ = NULL; } | id;

enum_stmt
  : ENUM LBRACE id_list RBRACE opt_id SEMICOLON    { $$ = new_stmt_enum($3, $5, get_pos(arg)); }
  | PRIVATE enum_stmt { CHECK_FLAG(arg, (&$2->d.stmt_enum), ae_flag_private); $$ = $2; }
  ;

label_stmt
  : id COLON {  $$ = new_stmt_gotolabel($1, 1, get_pos(arg)); }
  ;

goto_stmt
  : GOTO id SEMICOLON {  $$ = new_stmt_gotolabel($2, 0, get_pos(arg)); }
  ;

case_stmt
  : CASE primary_exp COLON { $$ = new_stmt_case($2, get_pos(arg)); }
  | CASE post_exp COLON { $$ = new_stmt_case($2, get_pos(arg)); }
  ;

switch_stmt
  : SWITCH LPAREN exp RPAREN code_segment { $$ = new_stmt_switch($3, $5, get_pos(arg));}
  ;

loop_stmt
  : WHILE LPAREN exp RPAREN stmt
    { $$ = new_stmt_while($3, $5, 0, get_pos(arg)); }
  | DO stmt WHILE LPAREN exp RPAREN SEMICOLON
    { $$ = new_stmt_while($5, $2, 1, get_pos(arg)); }
  | FOR LPAREN exp_stmt exp_stmt RPAREN stmt
      { $$ = new_stmt_for($3, $4, NULL, $6, get_pos(arg)); }
  | FOR LPAREN exp_stmt exp_stmt exp RPAREN stmt
      { $$ = new_stmt_for($3, $4, $5, $7, get_pos(arg)); }
  | FOR LPAREN AUTO id COLON binary_exp RPAREN stmt
      { $$ = new_stmt_auto($4, $6, $8, get_pos(arg)); }
  | FOR LPAREN AUTO_PTR id COLON binary_exp RPAREN stmt
      { $$ = new_stmt_auto($4, $6, $8, get_pos(arg)); $$->d.stmt_auto.is_ptr = 1;}
  | UNTIL LPAREN exp RPAREN stmt
      { $$ = new_stmt_until($3, $5, 0, get_pos(arg)); }
  | DO stmt UNTIL LPAREN exp RPAREN SEMICOLON
      { $$ = new_stmt_until($5, $2, 1, get_pos(arg)); }
  | LOOP LPAREN exp RPAREN stmt
      { $$ = new_stmt_loop($3, $5, get_pos(arg)); }
  ;

selection_stmt
  : IF LPAREN exp RPAREN stmt %prec NOELSE
      { $$ = new_stmt_if($3, $5, NULL, get_pos(arg)); }
  | IF LPAREN exp RPAREN stmt ELSE stmt
      { $$ = new_stmt_if($3, $5, $7, get_pos(arg)); }
  ;

jump_stmt
  : RETURN SEMICOLON     { $$ = new_stmt_return(NULL, get_pos(arg)); }
  | RETURN exp SEMICOLON { $$ = new_stmt_return($2, get_pos(arg)); }
  | BREAK SEMICOLON      { $$ = new_stmt_break(get_pos(arg)); }
  | CONTINUE SEMICOLON   { $$ = new_stmt_continue(get_pos(arg)); }
  ;

exp_stmt
  : exp SEMICOLON { $$ = new_stmt_exp($1,   get_pos(arg)); }
  | SEMICOLON     { $$ = new_stmt_exp(NULL, get_pos(arg)); }
  ;

exp
  : binary_exp
  | binary_exp COMMA exp  { $$ = prepend_exp($1, $3, get_pos(arg)); }
  ;

binary_exp
  : decl_exp
  | binary_exp op decl_exp     { $$ = new_exp_binary($1, $2, $3, get_pos(arg)); }
  ;

template: { $$ = NULL; } | LTB type_list GTB { $$ = $2; };

op: CHUCK { $$ = op_chuck; } | UNCHUCK { $$ = op_unchuck; } | EQ { $$ = op_eq; }
  | ATCHUCK     { $$ = op_at_chuck; } | PLUSCHUCK   { $$ = op_plus_chuck; }
  | MINUSCHUCK  { $$ = op_minus_chuck; } | TIMESCHUCK  { $$ = op_times_chuck; }
  | DIVIDECHUCK { $$ = op_divide_chuck; } | MODULOCHUCK { $$ = op_modulo_chuck; }
  | TRIG { $$ = op_trig; } | UNTRIG { $$ = op_untrig; }
  | RSL { $$ = op_rsl; } | RSR { $$ = op_rsr; } | RSAND { $$ = op_rsand; }
  | RSOR { $$ = op_rsor; } | RSXOR { $$ = op_rsxor; }
  | ASSIGN { $$ = op_assign; }
  ;

array_exp
  : LBRACK exp RBRACK           { $$ = new_array_sub($2, get_pos(arg)); }
  | LBRACK exp RBRACK array_exp { if($2->next){ gwion_error(arg, "invalid format for array init [...][...]..."); free_exp($2); free_array_sub($4); YYERROR; } $$ = prepend_array_sub($4, $2); }
  | LBRACK exp RBRACK LBRACK RBRACK { gwion_error(arg, "partially empty array init [...][]..."); free_exp($2); YYERROR; }
  ;

array_empty
  : LBRACK RBRACK             { $$ = new_array_sub(NULL, get_pos(arg)); }
  | array_empty LBRACK RBRACK { $$ = prepend_array_sub($1, NULL); }
  | array_empty array_exp     { gwion_error(arg, "partially empty array init [][...]"); free_array_sub($1); free_array_sub($2); YYERROR; }
  ;

decl_exp
  : con_exp
  | type_decl var_decl_list { $$= new_exp_decl($1, $2, get_pos(arg)); }
  | STATIC decl_exp
    { CHECK_FLAG(arg, $2->d.exp_decl.type, ae_flag_static); $$ = $2; }
  | PRIVATE decl_exp
    { CHECK_FLAG(arg, $2->d.exp_decl.type, ae_flag_private); $$ = $2; }
  ;

func_args
  : LPAREN RPAREN          { $$ = NULL; }
  | LPAREN arg_list RPAREN { $$ = $2; }
  ;

decl_template: TEMPLATE LTB id_list GTB { $$ = $3; };

func_def_base
  : function_decl static_decl type_decl2 id func_args code_segment
    { $$ = new_func_def($1 | $2, $3, $4, $5, $6, get_pos(arg)); }
  | PRIVATE func_def_base
    { CHECK_FLAG(arg, $2, ae_flag_private); $$ = $2; }
  | decl_template func_def_base
    { //CHECK_TEMPLATE(arg, $1, $2, free_func_def);

      if($2->tmpl) {
        free_id_list($1);
        free_func_def($2);
        gwion_error(arg, "double template decl");
        YYERROR;
      }
      $2->tmpl = new_tmpl_list($1, 1);
      $$ = $2; SET_FLAG($$, ae_flag_template);
    };

op_op: op | shift_op | post_op | rel_op | mul_op | add_op;
func_def
  : func_def_base
  |  OPERATOR op_op type_decl2 func_args code_segment
    { $$ = new_func_def(ae_flag_op , $3, OP_SYM($2), $4, $5, get_pos(arg)); }
  |  unary_op OPERATOR type_decl2 func_args code_segment
    { $$ = new_func_def(ae_flag_op | ae_flag_unary, $3, OP_SYM($1), $4, $5, get_pos(arg)); }
  | AST_DTOR LPAREN RPAREN code_segment
    { $$ = new_func_def(ae_flag_dtor, new_type_decl(new_id_list(insert_symbol("void"), get_pos(arg)), 0,
      get_pos(arg)), insert_symbol("dtor"), NULL, $4, get_pos(arg)); }
  ;

atsym: { $$ = 0; } | ATSYM { $$ = 1; };

type_decl
  : id atsym  { $$ = new_type_decl(new_id_list($1, get_pos(arg)), $2, get_pos(arg)); }
  | LT id_dot GT atsym { $$ = new_type_decl($2, $4, get_pos(arg)); }
  | LTB type_list GTB id atsym  { $$ = new_type_decl(new_id_list($4, get_pos(arg)),
      $5, get_pos(arg)); $$->types = $2; }
  | LTB type_list GTB LT id_dot GT atsym { $$ = new_type_decl($5, $7, get_pos(arg));
      $$->types = $2; }
  | TYPEOF LPAREN id_dot RPAREN atsym { $$ = new_type_decl2($3, $5, get_pos(arg)); }
  | CONST type_decl { CHECK_FLAG(arg, $2, ae_flag_const); $$ = $2; }
  ;

decl_list
  : exp SEMICOLON { $$ = new_decl_list($1, NULL); }
  | exp SEMICOLON decl_list { $$ = new_decl_list($1, $3); }
  ;

union_stmt
  : UNION LBRACE decl_list RBRACE opt_id SEMICOLON { $$ = new_stmt_union($3, get_pos(arg));$$->d.stmt_union.xid = $5; }
  | STATIC union_stmt
    { CHECK_FLAG(arg, (&$2->d.stmt_union), ae_flag_static); $$ = $2; }
  | PRIVATE union_stmt
    { CHECK_FLAG(arg, (&$2->d.stmt_union), ae_flag_private); $$ = $2; }
  ;

var_decl_list
  : var_decl  { $$ = new_var_decl_list($1, NULL, get_pos(arg)); }
  | var_decl  COMMA var_decl_list { $$ = new_var_decl_list($1, $3, get_pos(arg)); }
  ;

var_decl
  : id              { $$ = new_var_decl($1, NULL, get_pos(arg)); }
  | id array_exp    { $$ = new_var_decl($1,   $2, get_pos(arg)); }
  | id array_empty  { $$ = new_var_decl($1,   $2, get_pos(arg)); }
  ;

complex_exp: SHARPPAREN   exp RPAREN { $$ = new_complex($2, get_pos(arg)); };
polar_exp:   PERCENTPAREN exp RPAREN { $$ = new_polar(  $2, get_pos(arg)); };
vec_exp:     ATPAREN      exp RPAREN { $$ = new_vec(    $2, get_pos(arg)); };

con_exp: log_or_exp | log_or_exp QUESTION exp COLON con_exp
      { $$ = new_exp_if($1, $3, $5, get_pos(arg)); };

log_or_exp: log_and_exp | log_or_exp OR log_and_exp
      { $$ = new_exp_binary($1, op_or, $3, get_pos(arg)); };

log_and_exp: inc_or_exp | log_and_exp AND inc_or_exp
      { $$ = new_exp_binary($1, op_and, $3, get_pos(arg)); };

inc_or_exp: exc_or_exp | inc_or_exp S_OR exc_or_exp
      { $$ = new_exp_binary($1, op_s_or, $3, get_pos(arg)); };

exc_or_exp: and_exp | exc_or_exp S_XOR and_exp
      { $$ = new_exp_binary($1, op_s_xor, $3, get_pos(arg)); };

and_exp: eq_exp | and_exp S_AND eq_exp
      { $$ = new_exp_binary($1, op_s_and, $3, get_pos(arg)); };

eq_op : EQ { $$ = op_eq; } | NEQ { $$ = op_neq; };

eq_exp: relational_exp | eq_exp eq_op relational_exp
    { $$ = new_exp_binary($1, $2, $3, get_pos(arg)); };

rel_op: LT { $$ = op_lt; } | GT { $$ = op_gt; } |
    LE { $$ = op_le; } | GE { $$ = op_ge; };

relational_exp: shift_exp | relational_exp rel_op shift_exp
    { $$ = new_exp_binary($1, $2, $3, get_pos(arg)); };

shift_op
  : SHIFT_LEFT  { $$ = op_shift_left;  }
  | SHIFT_RIGHT { $$ = op_shift_right; }
  ;

shift_exp: add_exp | shift_exp shift_op  add_exp
    { $$ = new_exp_binary($1, $2, $3, get_pos(arg)); };

add_op: PLUS { $$ = op_plus; } | MINUS { $$ = op_minus; };

add_exp: mul_exp | add_exp add_op mul_exp
    { $$ = new_exp_binary($1, $2, $3, get_pos(arg)); };

mul_op: TIMES { $$ = op_times; } | DIVIDE { $$ = op_divide; } | PERCENT { $$ = op_percent; };

mul_exp: cast_exp | mul_exp mul_op cast_exp
    { $$ = new_exp_binary($1, $2, $3, get_pos(arg)); };

cast_exp: unary_exp | cast_exp DOLLAR type_decl2
    { $$ = new_exp_cast($3, $1, get_pos(arg)); };

unary_op : PLUS { $$ = op_plus; } | MINUS { $$ = op_minus; } | TIMES { $$ = op_times; }
         | PLUSPLUS { $$ = op_plusplus; } | MINUSMINUS { $$ = op_minusminus; }
  | EXCLAMATION { $$ = op_exclamation; } | SPORK TILDA { $$ = op_spork; }
  ;

unary_exp : dur_exp | unary_op unary_exp
      { $$ = new_exp_unary($1, $2, get_pos(arg)); }
  | NEW type_decl2
    {
      if($2->array && !$2->array->exp_list) {
        free_type_decl($2);
        gwion_error(arg, "can't use empty '[]' in 'new' expression");
        YYERROR;
      }
      $$ = new_exp_unary2(op_new, $2, get_pos(arg));
    }
  | SPORK TILDA code_segment
        { $$ = new_exp_unary3(op_spork, $3, get_pos(arg)); };

dur_exp: post_exp | dur_exp COLONCOLON post_exp
    { $$ = new_exp_dur($1, $3, get_pos(arg)); };

type_list
  : type_decl2 { $$ = new_type_list($1, NULL, get_pos(arg)); }
  | type_decl2 COMMA type_list { $$ = new_type_list($1, $3, get_pos(arg)); }
  ;

call_paren : LPAREN RPAREN { $$ = NULL; } | LPAREN exp RPAREN { $$ = $2; } ;

post_op : PLUSPLUS { $$ = op_plusplus; } | MINUSMINUS { $$ = op_minusminus; };

post_exp: primary_exp | post_exp array_exp
    { $$ = new_array($1, $2, get_pos(arg)); }
  | post_exp template call_paren
    { $$ = new_exp_call($1, $3, get_pos(arg));
      if($2)$$->d.exp_func.tmpl = new_tmpl_call($2); }
  | post_exp DOT id
    { $$ = new_exp_dot($1, $3, get_pos(arg)); }
  | post_exp post_op
    { $$ = new_exp_post($1, $2, get_pos(arg)); }
  ;

primary_exp
  : id                { $$ = new_exp_prim_id(     $1, get_pos(arg)); }
  | NUM               { $$ = new_exp_prim_int(    $1, get_pos(arg)); }
  | FLOAT             { $$ = new_exp_prim_float(  $1, get_pos(arg)); }
  | STRING_LIT        { $$ = new_exp_prim_string( $1, get_pos(arg)); }
  | CHAR_LIT          { $$ = new_exp_prim_char(   $1, get_pos(arg)); }
  | array_exp         { $$ = new_exp_prim_array(  $1, get_pos(arg)); }
  | array_empty       { $$ = new_exp_prim_array(  $1, get_pos(arg)); }
  | complex_exp       { $$ = new_exp_prim_complex($1, get_pos(arg)); }
  | polar_exp         { $$ = new_exp_prim_polar(  $1, get_pos(arg)); }
  | vec_exp           { $$ = new_exp_prim_vec(    $1, get_pos(arg)); }
  | L_HACK exp R_HACK { $$ = new_exp_prim_hack(   $2, get_pos(arg)); }
  | LPAREN exp RPAREN { $$ =                       $2;                    }
  | LPAREN RPAREN     { $$ = new_exp_prim_nil(        get_pos(arg)); }
  ;
%%
