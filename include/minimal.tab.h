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

#ifndef YY_MINIMAL_MINIMAL_TAB_H_INCLUDED
# define YY_MINIMAL_MINIMAL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int minimaldebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
enum yytokentype {
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
  PERCENTPAREN = 280,
  POUNDPAREN = 281,
  PROTECTED = 282,
  ABSTRACT = 283,
  ATSYM = 284,
  FUNCTION = 285,
  DOLLAR = 286,
  TILDA = 287,
  QUESTION = 288,
  COLON = 289,
  EXCLAMATION = 290,
  IF = 291,
  ELSE = 292,
  WHILE = 293,
  DO = 294,
  UNTIL = 295,
  LOOP = 296,
  FOR = 297,
  GOTO = 298,
  SWITCH = 299,
  CASE = 300,
  ENUM = 301,
  RETURN = 302,
  BREAK = 303,
  CONTINUE = 304,
  PLUSPLUS = 305,
  MINUSMINUS = 306,
  NEW = 307,
  SPORK = 308,
  SIZEOF = 309,
  TYPEOF = 310,
  CLASS = 311,
  INTERFACE = 312,
  STATIC = 313,
  PRIVATE = 314,
  PUBLIC = 315,
  EXTENDS = 316,
  IMPLEMENTS = 317,
  DOT = 318,
  COLONCOLON = 319,
  AND = 320,
  EQ = 321,
  GE = 322,
  GT = 323,
  LE = 324,
  LT = 325,
  MINUS = 326,
  PLUS = 327,
  NEQ = 328,
  NOR = 329,
  SHIFT_LEFT = 330,
  SHIFT_RIGHT = 331,
  S_AND = 332,
  S_OR = 333,
  S_XOR = 334,
  OR = 335,
  NUM = 336,
  FLOAT = 337,
  ID = 338,
  STRING_LIT = 339,
  CHAR_LIT = 340
};
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE {
#line 18 "minimal.y" /* yacc.c:1909  */

  char* sval;
  int ival;
  double fval;
  Complex* complex;
  Polar* polar;
  Array_Sub array_sub;
  Var_Decl var_decl;
  Var_Decl_List var_decl_list;
  Type_Decl* type_decl;
  Expression   exp;
  Stmt* stmt;
  Stmt_List stmt_list;
  Arg_List arg_list;
  Func_Def func_def;
  Section* section;
  ID_List id_list;
  Class_Body class_body;
  Class_Ext class_ext;
  Class_Ext iface_ext;
  Class_Def class_def;
  Ast ast;

#line 164 "minimal.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int minimalparse (MyArg* arg);

#endif /* !YY_MINIMAL_MINIMAL_TAB_H_INCLUDED  */
