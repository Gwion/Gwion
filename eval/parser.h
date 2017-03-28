/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_MINIMAL_EVAL_PARSER_H_INCLUDED
# define YY_MINIMAL_EVAL_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int minimal_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SEMICOLON = 258,
    CHUCK = 259,
    COMMA = 260,
    ASSIGN = 261,
    DIVIDE = 262,
    TIMES = 263,
    PERCENT = 264,
    L_HACK = 265,
    R_HACK = 266,
    LPAREN = 267,
    RPAREN = 268,
    LBRACK = 269,
    RBRACK = 270,
    LBRACE = 271,
    RBRACE = 272,
    PLUSCHUCK = 273,
    MINUSCHUCK = 274,
    TIMESCHUCK = 275,
    DIVIDECHUCK = 276,
    MODULOCHUCK = 277,
    ATCHUCK = 278,
    UNCHUCK = 279,
    TRIG = 280,
    UNTRIG = 281,
    PERCENTPAREN = 282,
    SHARPPAREN = 283,
    PROTECTED = 284,
    ATSYM = 285,
    FUNCTION = 286,
    DOLLAR = 287,
    TILDA = 288,
    QUESTION = 289,
    COLON = 290,
    EXCLAMATION = 291,
    IF = 292,
    ELSE = 293,
    WHILE = 294,
    DO = 295,
    UNTIL = 296,
    LOOP = 297,
    FOR = 298,
    GOTO = 299,
    SWITCH = 300,
    CASE = 301,
    ENUM = 302,
    RETURN = 303,
    BREAK = 304,
    CONTINUE = 305,
    PLUSPLUS = 306,
    MINUSMINUS = 307,
    NEW = 308,
    SPORK = 309,
    SIZEOF = 310,
    TYPEOF = 311,
    CLASS = 312,
    INTERFACE = 313,
    STATIC = 314,
    PRIVATE = 315,
    PUBLIC = 316,
    EXTENDS = 317,
    IMPLEMENTS = 318,
    DOT = 319,
    COLONCOLON = 320,
    AND = 321,
    EQ = 322,
    GE = 323,
    GT = 324,
    LE = 325,
    LT = 326,
    MINUS = 327,
    PLUS = 328,
    NEQ = 329,
    SHIFT_LEFT = 330,
    SHIFT_RIGHT = 331,
    S_AND = 332,
    S_OR = 333,
    S_XOR = 334,
    OR = 335,
    AST_DTOR = 336,
    OPERATOR = 337,
    FUNC_PTR = 338,
    RSL = 339,
    RSR = 340,
    RSAND = 341,
    RSOR = 342,
    RSXOR = 343,
    RAND = 344,
    ROR = 345,
    REQ = 346,
    RNEQ = 347,
    RGT = 348,
    RGE = 349,
    RLT = 350,
    RLE = 351,
    RINC = 352,
    RDEC = 353,
    RUNINC = 354,
    RUNDEC = 355,
    TEMPLATE = 356,
    NOELSE = 357,
    ARROW_LEFT = 358,
    ARROW_RIGHT = 359,
    LTB = 360,
    GTB = 361,
    VARARG = 362,
    UNION = 363,
    ATPAREN = 364,
    NUM = 365,
    FLOAT = 366,
    ID = 367,
    STRING_LIT = 368,
    CHAR_LIT = 369,
    DOC = 370
  };
#endif
/* Tokens.  */
#define SEMICOLON 258
#define CHUCK 259
#define COMMA 260
#define ASSIGN 261
#define DIVIDE 262
#define TIMES 263
#define PERCENT 264
#define L_HACK 265
#define R_HACK 266
#define LPAREN 267
#define RPAREN 268
#define LBRACK 269
#define RBRACK 270
#define LBRACE 271
#define RBRACE 272
#define PLUSCHUCK 273
#define MINUSCHUCK 274
#define TIMESCHUCK 275
#define DIVIDECHUCK 276
#define MODULOCHUCK 277
#define ATCHUCK 278
#define UNCHUCK 279
#define TRIG 280
#define UNTRIG 281
#define PERCENTPAREN 282
#define SHARPPAREN 283
#define PROTECTED 284
#define ATSYM 285
#define FUNCTION 286
#define DOLLAR 287
#define TILDA 288
#define QUESTION 289
#define COLON 290
#define EXCLAMATION 291
#define IF 292
#define ELSE 293
#define WHILE 294
#define DO 295
#define UNTIL 296
#define LOOP 297
#define FOR 298
#define GOTO 299
#define SWITCH 300
#define CASE 301
#define ENUM 302
#define RETURN 303
#define BREAK 304
#define CONTINUE 305
#define PLUSPLUS 306
#define MINUSMINUS 307
#define NEW 308
#define SPORK 309
#define SIZEOF 310
#define TYPEOF 311
#define CLASS 312
#define INTERFACE 313
#define STATIC 314
#define PRIVATE 315
#define PUBLIC 316
#define EXTENDS 317
#define IMPLEMENTS 318
#define DOT 319
#define COLONCOLON 320
#define AND 321
#define EQ 322
#define GE 323
#define GT 324
#define LE 325
#define LT 326
#define MINUS 327
#define PLUS 328
#define NEQ 329
#define SHIFT_LEFT 330
#define SHIFT_RIGHT 331
#define S_AND 332
#define S_OR 333
#define S_XOR 334
#define OR 335
#define AST_DTOR 336
#define OPERATOR 337
#define FUNC_PTR 338
#define RSL 339
#define RSR 340
#define RSAND 341
#define RSOR 342
#define RSXOR 343
#define RAND 344
#define ROR 345
#define REQ 346
#define RNEQ 347
#define RGT 348
#define RGE 349
#define RLT 350
#define RLE 351
#define RINC 352
#define RDEC 353
#define RUNINC 354
#define RUNDEC 355
#define TEMPLATE 356
#define NOELSE 357
#define ARROW_LEFT 358
#define ARROW_RIGHT 359
#define LTB 360
#define GTB 361
#define VARARG 362
#define UNION 363
#define ATPAREN 364
#define NUM 365
#define FLOAT 366
#define ID 367
#define STRING_LIT 368
#define CHAR_LIT 369
#define DOC 370

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 50 "eval/gwion.y" /* yacc.c:1909  */

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

#line 313 "eval/parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int minimal_parse (MyArg* arg);

#endif /* !YY_MINIMAL_EVAL_PARSER_H_INCLUDED  */
