/* A Bison parser, made by GNU Bison 3.0.5.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         gwion_parse
#define yylex           gwion_lex
#define yyerror         gwion_error
#define yydebug         gwion_debug
#define yynerrs         gwion_nerrs


/* Copy the first part of user declarations.  */
#line 5 "utils/gwion.y" /* yacc.c:339  */

#include <stdio.h> // strlen in paste operation
#include <string.h> // strlen in paste operation
#include <math.h>
#include "absyn.h"
#include "hash.h"
#include "scanner.h"
#include "parser.h"
#include "lexer.h"
//ANN int gwion_parse(Scanner* arg);
#define YYMALLOC xmalloc
#define scan arg->scanner
#define CHECK_FLAG(a,b,c) if(GET_FLAG(b, c)) { gwion_error(a, "flag set twice");  ; } SET_FLAG(b, c);
#define CHECK_TEMPLATE(a, b, c, free_function) { if(c->tmpl) {\
        free_id_list(b);\
        free_function(c);\
        gwion_error(a, "double template decl");\
        YYERROR;\
      }\
      c->tmpl = new_tmpl_class(b, -1);\
    };
#define OP_SYM(a) insert_symbol(op2str(a))
ANN int get_pos(const Scanner*);
ANN void gwion_error(const Scanner*, const m_str s);
m_str op2str(const Operator op);

#line 99 "src/ast/parser.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.h".  */
#ifndef YY_GWION_INCLUDE_PARSER_H_INCLUDED
# define YY_GWION_INCLUDE_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int gwion_debug;
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
    CLASS = 309,
    STATIC = 310,
    GLOBAL = 311,
    PRIVATE = 312,
    PROTECT = 313,
    EXTENDS = 314,
    DOT = 315,
    COLONCOLON = 316,
    AND = 317,
    EQ = 318,
    GE = 319,
    GT = 320,
    LE = 321,
    LT = 322,
    MINUS = 323,
    PLUS = 324,
    NEQ = 325,
    SHIFT_LEFT = 326,
    SHIFT_RIGHT = 327,
    S_AND = 328,
    S_OR = 329,
    S_XOR = 330,
    OR = 331,
    AST_DTOR = 332,
    OPERATOR = 333,
    TYPEDEF = 334,
    RSL = 335,
    RSR = 336,
    RSAND = 337,
    RSOR = 338,
    RSXOR = 339,
    TEMPLATE = 340,
    NOELSE = 341,
    LTB = 342,
    GTB = 343,
    UNION = 344,
    ATPAREN = 345,
    TYPEOF = 346,
    CONST = 347,
    AUTO = 348,
    PASTE = 349,
    ELLIPSE = 350,
    NUM = 351,
    FLOAT = 352,
    ID = 353,
    STRING_LIT = 354,
    CHAR_LIT = 355,
    PP_COMMENT = 356,
    PP_INCLUDE = 357,
    PP_DEFINE = 358,
    PP_UNDEF = 359,
    PP_IFDEF = 360,
    PP_IFNDEF = 361,
    PP_ELSE = 362,
    PP_ENDIF = 363,
    PP_NL = 364
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
#define ATSYM 284
#define FUNCTION 285
#define DOLLAR 286
#define TILDA 287
#define QUESTION 288
#define COLON 289
#define EXCLAMATION 290
#define IF 291
#define ELSE 292
#define WHILE 293
#define DO 294
#define UNTIL 295
#define LOOP 296
#define FOR 297
#define GOTO 298
#define SWITCH 299
#define CASE 300
#define ENUM 301
#define RETURN 302
#define BREAK 303
#define CONTINUE 304
#define PLUSPLUS 305
#define MINUSMINUS 306
#define NEW 307
#define SPORK 308
#define CLASS 309
#define STATIC 310
#define GLOBAL 311
#define PRIVATE 312
#define PROTECT 313
#define EXTENDS 314
#define DOT 315
#define COLONCOLON 316
#define AND 317
#define EQ 318
#define GE 319
#define GT 320
#define LE 321
#define LT 322
#define MINUS 323
#define PLUS 324
#define NEQ 325
#define SHIFT_LEFT 326
#define SHIFT_RIGHT 327
#define S_AND 328
#define S_OR 329
#define S_XOR 330
#define OR 331
#define AST_DTOR 332
#define OPERATOR 333
#define TYPEDEF 334
#define RSL 335
#define RSR 336
#define RSAND 337
#define RSOR 338
#define RSXOR 339
#define TEMPLATE 340
#define NOELSE 341
#define LTB 342
#define GTB 343
#define UNION 344
#define ATPAREN 345
#define TYPEOF 346
#define CONST 347
#define AUTO 348
#define PASTE 349
#define ELLIPSE 350
#define NUM 351
#define FLOAT 352
#define ID 353
#define STRING_LIT 354
#define CHAR_LIT 355
#define PP_COMMENT 356
#define PP_INCLUDE 357
#define PP_DEFINE 358
#define PP_UNDEF 359
#define PP_IFDEF 360
#define PP_IFNDEF 361
#define PP_ELSE 362
#define PP_ENDIF 363
#define PP_NL 364

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 32 "utils/gwion.y" /* yacc.c:355  */

  char* sval;
  int ival;
  m_float fval;
  Symbol sym;
  Array_Sub array_sub;
  Var_Decl var_decl;
  Var_Decl_List var_decl_list;
  Type_Decl* type_decl;
  Exp   exp;
  Stmt_Fptr func_ptr;
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

#line 382 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (Scanner* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 398 "src/ast/parser.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  214
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1602

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  110
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  73
/* YYNRULES -- Number of rules.  */
#define YYNRULES  238
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  417

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   364

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   108,   108,   109,   113,   114,   115,   119,   121,   122,
     123,   124,   125,   127,   127,   129,   129,   132,   133,   137,
     138,   142,   143,   147,   148,   152,   154,   155,   156,   157,
     161,   163,   164,   165,   166,   169,   170,   171,   175,   176,
     180,   181,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   203,   204,   211,   211,   214,
     215,   216,   217,   218,   222,   226,   230,   231,   235,   238,
     238,   241,   243,   245,   247,   249,   251,   253,   255,   260,
     262,   267,   268,   269,   270,   274,   275,   279,   280,   284,
     285,   288,   288,   290,   290,   290,   291,   291,   292,   292,
     293,   293,   294,   294,   295,   295,   295,   296,   296,   297,
     301,   302,   303,   307,   308,   309,   313,   314,   315,   317,
     319,   321,   326,   327,   331,   332,   335,   338,   340,   342,
     344,   346,   348,   360,   360,   360,   360,   360,   360,   362,
     363,   365,   367,   372,   372,   375,   376,   377,   379,   381,
     382,   386,   387,   391,   396,   398,   400,   402,   407,   408,
     412,   413,   414,   417,   417,   420,   420,   423,   423,   426,
     426,   429,   429,   432,   432,   435,   435,   437,   437,   440,
     440,   441,   441,   443,   443,   447,   448,   451,   451,   454,
     454,   456,   456,   459,   459,   459,   461,   461,   464,   464,
     467,   467,   467,   468,   468,   469,   469,   469,   472,   472,
     474,   483,   486,   486,   490,   491,   494,   494,   496,   496,
     498,   498,   500,   503,   505,   509,   510,   511,   514,   515,
     516,   517,   518,   519,   520,   521,   522,   523,   524
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEMICOLON", "CHUCK", "COMMA", "ASSIGN",
  "DIVIDE", "TIMES", "PERCENT", "L_HACK", "R_HACK", "LPAREN", "RPAREN",
  "LBRACK", "RBRACK", "LBRACE", "RBRACE", "PLUSCHUCK", "MINUSCHUCK",
  "TIMESCHUCK", "DIVIDECHUCK", "MODULOCHUCK", "ATCHUCK", "UNCHUCK", "TRIG",
  "UNTRIG", "PERCENTPAREN", "SHARPPAREN", "ATSYM", "FUNCTION", "DOLLAR",
  "TILDA", "QUESTION", "COLON", "EXCLAMATION", "IF", "ELSE", "WHILE", "DO",
  "UNTIL", "LOOP", "FOR", "GOTO", "SWITCH", "CASE", "ENUM", "RETURN",
  "BREAK", "CONTINUE", "PLUSPLUS", "MINUSMINUS", "NEW", "SPORK", "CLASS",
  "STATIC", "GLOBAL", "PRIVATE", "PROTECT", "EXTENDS", "DOT", "COLONCOLON",
  "AND", "EQ", "GE", "GT", "LE", "LT", "MINUS", "PLUS", "NEQ",
  "SHIFT_LEFT", "SHIFT_RIGHT", "S_AND", "S_OR", "S_XOR", "OR", "AST_DTOR",
  "OPERATOR", "TYPEDEF", "RSL", "RSR", "RSAND", "RSOR", "RSXOR",
  "TEMPLATE", "NOELSE", "LTB", "GTB", "UNION", "ATPAREN", "TYPEOF",
  "CONST", "AUTO", "PASTE", "ELLIPSE", "NUM", "FLOAT", "ID", "STRING_LIT",
  "CHAR_LIT", "PP_COMMENT", "PP_INCLUDE", "PP_DEFINE", "PP_UNDEF",
  "PP_IFDEF", "PP_IFNDEF", "PP_ELSE", "PP_ENDIF", "PP_NL", "$accept",
  "ast", "section", "class_def", "class_ext", "class_body", "class_body2",
  "id_list", "id_dot", "stmt_list", "func_ptr", "stmt_type", "type_decl2",
  "arg_list", "code_segment", "stmt", "id", "opt_id", "enum_stmt",
  "label_stmt", "goto_stmt", "case_stmt", "switch_stmt", "auto",
  "loop_stmt", "selection_stmt", "jump_stmt", "exp_stmt", "exp",
  "binary_exp", "template", "op", "array_exp", "array_empty", "decl_exp",
  "func_args", "arg_type", "decl_template", "func_def_base", "op_op",
  "func_def", "atsym", "type_decl", "decl_list", "union_stmt",
  "var_decl_list", "var_decl", "con_exp", "log_or_exp", "log_and_exp",
  "inc_or_exp", "exc_or_exp", "and_exp", "eq_op", "eq_exp", "rel_op",
  "relational_exp", "shift_op", "shift_exp", "add_op", "add_exp", "mul_op",
  "mul_exp", "cast_exp", "unary_op", "unary_exp", "dur_exp", "type_list",
  "call_paren", "post_op", "post_exp", "vec_type", "primary_exp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364
};
# endif

#define YYPACT_NINF -324

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-324)))

#define YYTABLE_NINF -144

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     408,  -324,  -324,  1327,  1021,  1072,   506,  -324,  -324,    50,
    -324,  -324,    16,    26,   604,    45,    47,    66,   -73,    78,
      97,    89,   729,   107,   112,  -324,  -324,    50,    87,   -73,
     854,   854,   854,   854,   -73,  -324,  -324,   106,  1482,    50,
      36,    50,   -73,  -324,   117,    50,  -324,  -324,    37,  -324,
    -324,   136,   408,  -324,  -324,  -324,  -324,  -324,   604,    64,
    -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,   135,
     835,  -324,   126,  -324,   308,  -324,  -324,   -73,  -324,  -324,
       7,    93,    84,    85,    90,   -33,    86,    41,    58,    88,
     130,  1378,  -324,   103,    19,  1327,  -324,  1327,  1327,  1327,
    1327,   -18,   154,  1430,  -324,   153,  -324,   152,  -324,   928,
     928,   928,   928,   151,   -73,   140,   156,  1327,  1327,    10,
    1327,  1327,   678,   168,  1327,  -324,    70,   138,   -73,  -324,
     170,  -324,  -324,  -324,   159,   119,   176,  -324,  -324,  -324,
    -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,
    -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,
    -324,  -324,  -324,  -324,  -324,   120,   122,   171,  -324,  -324,
    -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,
    -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,
    -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,  -324,    50,
    -324,  -324,  -324,  -324,  -324,    -4,   -73,   181,   100,  -324,
     173,   -73,  -324,   -73,  -324,  -324,  -324,  -324,  -324,  -324,
    -324,  1327,  1327,  1123,  -324,   308,   308,   308,   308,  -324,
    -324,   156,  -324,   185,  1327,  1430,  1430,  1430,  1430,  1430,
    -324,  -324,  1430,  1430,  1430,  1430,  1430,    50,    50,  -324,
      97,  1327,   -73,    50,   180,  -324,  -324,   187,  -324,  -324,
     184,  -324,   189,  -324,   126,   190,   191,   196,   197,   199,
     201,   186,   -73,   780,  -324,   204,  -324,  -324,   193,  -324,
    -324,    50,   202,   -73,   140,   -73,   159,   189,   -73,   216,
     114,    50,   -52,  1327,   208,  -324,  -324,  -324,  -324,  -324,
     126,   -73,   198,    93,    84,    85,    90,   -33,    86,    41,
      58,    88,   130,  -324,   189,    19,  -324,   141,  1174,  -324,
    -324,  1225,  -324,    50,    -3,   604,   604,  1327,  1327,   604,
     604,  -324,   200,  1276,   159,   -73,  -324,   408,  -324,  -324,
    -324,  -324,   210,   217,  -324,  -324,  -324,   -73,   140,   230,
     223,   140,  -324,  1430,   228,  -324,  -324,   236,  -324,  -324,
     -73,  -324,   241,   159,   221,  -324,   249,   250,  -324,  -324,
    1327,   604,   258,  -324,   256,   408,   267,  -324,   159,   189,
     222,  -324,  1327,   -73,  -324,  -324,   159,  -324,   268,  -324,
    -324,   604,   283,   287,  1518,  -324,   604,  -324,  -324,  -324,
    -324,    -3,   140,  -324,   297,  -324,    50,  -324,  -324,  -324,
     604,  -324,  -324,  -324,  -324,  -324,  -324
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    86,   202,     0,     0,     0,     0,   226,   225,     0,
     207,   205,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   203,   204,     0,     0,     0,
       0,     0,     0,     0,     0,   201,   200,     0,     0,     0,
       0,     0,    57,   227,     0,     0,   229,   230,    55,   231,
     232,     0,     2,     6,     4,    52,    53,    45,    23,   228,
      50,    46,    47,    49,    48,    43,    44,    51,    42,     0,
      87,   233,   234,    89,     0,   139,     5,     0,    54,   116,
     163,   165,   167,   169,   171,   173,   177,   183,   187,   191,
     196,     0,   198,   208,   212,     0,   220,     0,     0,     0,
       0,   228,     0,     0,   238,     0,   113,     0,    40,     0,
       0,     0,     0,     0,     0,   143,    35,     0,     0,     0,
       0,     0,     0,     0,     0,   228,    91,   220,     0,    81,
       0,    83,    84,   210,   206,    14,    19,     8,    26,    31,
      60,   118,   128,   154,     9,    27,    32,    61,   119,   129,
     155,    10,    28,    33,    62,   120,   130,   156,    11,    29,
      34,    63,   121,   131,   157,     0,    21,     0,    93,   109,
     194,   193,   195,    97,    98,    99,   100,   101,    96,    94,
     102,   103,   218,   219,    95,   182,   180,   181,   179,   190,
     189,   185,   186,   104,   105,   106,   107,   108,   133,     0,
     136,   134,   138,   137,   135,     0,     0,   214,     0,    58,
       0,     0,   150,     0,     1,     3,    24,   144,    64,   145,
      85,     0,     0,     0,   115,     0,     0,     0,     0,    12,
     132,   160,   117,   158,     0,     0,     0,     0,     0,     0,
     175,   176,     0,     0,     0,     0,     0,     0,     0,   209,
       0,     0,     0,     0,     0,   221,   224,     0,   236,   237,
     110,    41,     0,    37,    36,     0,     0,     0,     0,     0,
       0,    70,     0,     0,    65,     0,    67,    66,     0,    82,
     211,     0,     0,     0,   143,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    88,    90,   114,   161,
     162,     0,     0,   166,   168,   170,   172,   174,   178,   184,
     188,   192,   197,   199,     0,   213,   223,     0,     0,   222,
     235,     0,   111,   122,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,    57,    13,    16,    20,   146,
      22,   142,     0,     0,    30,   126,   215,     0,   143,     0,
       0,   143,   159,     0,     0,    92,   216,     0,   112,   123,
       0,   124,     0,     0,    79,    71,     0,     0,    76,    78,
       0,     0,     0,    68,     0,    17,     0,    15,     0,     0,
       0,   147,   151,    57,   149,   164,     0,   217,    38,   125,
     127,     0,     0,     0,     0,    73,     0,    59,    18,     7,
     140,     0,   143,   152,     0,   141,     0,    80,    72,    77,
       0,    74,    25,   148,   153,    39,    75
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -324,   251,  -314,    42,  -324,  -324,   -66,  -124,  -198,    13,
     195,   246,    17,  -104,  -112,   -12,     0,  -323,   318,  -324,
    -324,  -324,  -324,  -324,  -324,  -324,  -324,  -117,    59,   -62,
    -324,   273,   -40,  -109,    35,  -271,   -89,  -324,    69,  -324,
    -324,  -263,     8,   -63,   369,    14,   -44,   -32,  -324,    92,
      96,    83,    95,  -324,   102,   284,    82,   290,    91,   298,
      98,   301,    99,   108,     3,   -67,  -324,  -202,  -324,   305,
     -19,  -324,   325
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    51,    52,    53,   282,   376,   377,   135,   165,    54,
      55,    56,   207,   359,    57,    58,   101,   210,    60,    61,
      62,    63,    64,   272,    65,    66,    67,    68,    69,    70,
     254,   222,    71,    72,    73,   324,   363,    74,    75,   199,
      76,   219,    77,   350,    78,   232,   233,    79,    80,    81,
      82,    83,    84,   242,    85,   243,    86,   244,    87,   245,
      88,   246,    89,    90,   103,    92,    93,   208,   319,   256,
      94,    95,    96
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      59,   126,   119,    91,   278,   273,    59,   264,   288,   115,
     361,   217,   374,   294,    59,   347,   342,   116,   123,   113,
     125,   339,   280,   375,   249,    48,   114,   115,   117,   136,
     240,   -91,   224,   251,   166,   116,   249,   241,   118,   115,
     234,   115,   209,   354,   133,   115,    48,   116,   267,   116,
     268,   317,    59,   212,   255,    91,   205,   120,    59,   121,
     404,   375,   102,   105,   107,   141,   148,   155,   162,   182,
     183,   216,   137,   144,   151,   158,   263,   231,   122,   252,
    -143,   130,   290,   235,   251,   381,   255,   340,   384,   346,
     124,   125,   362,   217,    48,   170,   171,   172,   218,   142,
     149,   156,   163,   125,   276,   128,   253,     3,   401,     4,
     131,     5,   191,   192,   262,   132,   229,    34,   167,   134,
     182,   183,   300,   206,     7,     8,   189,   190,   136,   211,
     252,   213,   141,   148,   155,   162,   214,    41,   220,   413,
     223,    44,    45,   230,   141,   148,   155,   162,    48,   380,
     185,   186,   187,   188,   257,   236,   333,   253,   237,   338,
     238,   247,  -143,   239,   250,   258,   259,   260,   261,   217,
       5,   274,   277,   279,   341,     6,   265,   266,   281,   269,
     270,   283,   285,   275,   286,   284,   291,    43,   292,   293,
     301,   299,   318,    46,    47,    48,    49,    50,   321,   115,
     320,   323,   345,   325,   326,   289,   136,   116,   327,   328,
     335,   166,   329,   295,   330,   331,   287,   334,   337,   344,
     322,   351,   373,   378,   224,   138,   145,   152,   159,   355,
     379,   315,   353,   382,   370,   125,   125,   125,   125,   125,
     383,   386,   125,   125,   125,   125,   125,   115,   115,   387,
     125,   390,   316,   115,   389,   116,   116,   297,   391,   397,
     224,   116,   392,   393,   313,   314,   400,   137,   144,   151,
     158,   396,   332,   406,   405,   255,   139,   146,   153,   160,
     296,   115,   107,   136,   399,   166,   408,   402,   343,   116,
     409,   115,   348,   302,   142,   149,   156,   163,   336,   116,
     414,   231,   415,   215,   138,   145,   152,   159,   394,   398,
     107,   198,   412,   364,   365,   352,   388,   368,   369,   403,
     305,   385,   200,   115,   308,    59,    59,   303,   201,    59,
      59,   360,   304,   306,   309,   209,   202,    59,     9,   203,
      91,   307,   310,   204,   311,   127,     0,   166,   140,   147,
     154,   161,   349,   125,   312,   139,   146,   153,   160,   395,
     231,     0,    29,   225,   226,   227,   228,     0,     0,     0,
       0,    59,     0,     0,     0,    59,     0,   357,    91,   407,
     107,     0,     0,   209,   411,     0,   366,   367,     0,     0,
       0,    59,   372,    40,     0,     0,    59,     0,   416,   143,
     150,   157,   164,     0,     0,     0,   115,     0,     0,     0,
      59,     1,     0,     0,   360,     0,     2,     0,     3,     0,
       4,     0,     5,     0,     6,     0,     0,   140,   147,   154,
     161,     0,     0,     0,     0,     7,     8,     0,     9,     0,
      10,   349,     0,    11,    12,     0,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,     0,     0,     0,
       0,     0,     0,     0,     0,    34,    35,    36,   143,   150,
     157,   164,     0,     0,     0,    37,    38,    39,     0,     0,
       0,     0,     0,    40,     0,    41,     0,    42,    43,    44,
      45,     0,     0,     0,    46,    47,    48,    49,    50,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,     0,
       5,     0,     6,   108,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,    10,     0,
       0,    11,    12,     0,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
       0,   109,   110,   111,   112,     0,     0,     0,     0,     0,
       0,     0,     0,    34,    35,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
       0,     0,     0,    41,     0,    42,    43,    44,    45,     0,
       0,     0,    46,    47,    48,    49,    50,     1,     0,     0,
       0,     0,     2,     0,     3,     0,     4,     0,     5,     0,
       6,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,    10,     0,     0,    11,
      12,     0,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,     0,   109,
     110,   111,   112,     0,     0,     0,     0,     0,     0,     0,
       0,    34,    35,    36,     0,     0,     0,     0,     0,     0,
       0,     1,     0,    39,     0,     0,     2,     0,     3,     0,
       4,    41,     5,    42,    43,    44,    45,     0,     0,     0,
      46,    47,    48,    49,    50,     7,     8,     0,     0,     0,
      10,     0,     0,    11,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    25,    26,
      27,    28,   129,    97,    98,    99,   100,     2,     0,     3,
       0,     4,     0,     5,     0,    34,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,    10,     0,     0,    11,    41,     0,     0,    43,    44,
      45,   271,     0,     0,    46,    47,    48,    49,    50,    25,
      26,    27,    28,     1,    97,    98,    99,   100,     2,     0,
       3,     0,     4,     0,     5,     0,    34,    35,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,    10,     0,     0,    11,    41,     0,     0,    43,
      44,    45,     0,     0,     0,    46,    47,    48,    49,    50,
      25,    26,    27,    28,     0,    97,    98,    99,   100,   168,
     221,   169,     0,     0,     0,     0,     0,    34,    35,    36,
       0,     0,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     2,     0,     3,     0,     4,    41,     5,     0,
      43,    44,    45,     0,     0,     0,    46,    47,    48,    49,
      50,     7,     8,     0,     9,     0,    10,     0,     0,    11,
       0,     0,     0,     0,     0,     0,     0,     0,   184,     0,
      21,     0,     0,     0,    25,    26,    27,    28,    29,    30,
      31,    32,    33,     0,     0,   193,   194,   195,   196,   197,
       0,    34,    35,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    39,     0,     0,     2,     0,     3,    40,
       4,    41,     5,    42,    43,    44,    45,     0,     0,     0,
      46,    47,    48,    49,    50,     7,     8,     0,     0,     0,
      10,     0,     0,    11,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,    25,    26,
      27,    28,     0,   109,   110,   111,   112,     0,     0,     0,
       0,     0,     0,     0,     0,    34,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,    41,     0,    42,    43,    44,
      45,     0,     0,     0,    46,    47,    48,    49,    50,     2,
       0,     3,     0,     4,   104,     5,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
       0,     0,     0,    10,     0,     0,    11,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    25,    26,    27,    28,     0,    97,    98,    99,   100,
       2,     0,     3,     0,     4,     0,     5,   106,    34,    35,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,    10,     0,     0,    11,    41,     0,
       0,    43,    44,    45,     0,     0,     0,    46,    47,    48,
      49,    50,    25,    26,    27,    28,     0,    97,    98,    99,
     100,     2,     0,     3,     0,     4,     0,     5,   298,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,    10,     0,     0,    11,    41,
       0,     0,    43,    44,    45,     0,     0,     0,    46,    47,
      48,    49,    50,    25,    26,    27,    28,     0,    97,    98,
      99,   100,     2,     0,     3,     0,     4,   356,     5,     0,
      34,    35,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,    10,     0,     0,    11,
      41,     0,     0,    43,    44,    45,     0,     0,     0,    46,
      47,    48,    49,    50,    25,    26,    27,    28,     0,    97,
      98,    99,   100,     2,     0,     3,     0,     4,     0,     5,
     358,    34,    35,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,    10,     0,     0,
      11,    41,     0,     0,    43,    44,    45,     0,     0,     0,
      46,    47,    48,    49,    50,    25,    26,    27,    28,     0,
      97,    98,    99,   100,     2,     0,     3,     0,     4,   371,
       5,     0,    34,    35,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,    10,     0,
       0,    11,    41,     0,     0,    43,    44,    45,     0,     0,
       0,    46,    47,    48,    49,    50,    25,    26,    27,    28,
       0,    97,    98,    99,   100,     2,     0,     3,     0,     4,
       0,     5,     0,    34,    35,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,    10,
       0,     0,    11,    41,     0,     0,    43,    44,    45,     0,
       0,     0,    46,    47,    48,    49,    50,    25,    26,    27,
      28,     0,    97,    98,    99,   100,     2,     0,     3,     0,
       4,     0,     5,     0,    34,    35,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
      10,     0,     0,    11,    41,     0,     0,    43,    44,    45,
       0,     0,     0,    46,    47,    48,    49,    50,    25,    26,
      27,    28,     0,     0,     0,     0,     0,     0,     2,     0,
       3,     0,     4,     0,     5,     0,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,   248,     7,     8,     0,
       0,     0,    10,     0,     0,    11,     0,     0,    43,     0,
       0,     0,     0,     0,    46,    47,    48,    49,    50,     0,
      25,    26,    27,    28,     0,     0,   168,     0,   169,   170,
     171,   172,     0,     0,     0,     0,     0,     0,    35,    36,
     173,   174,   175,   176,   177,   178,   179,   180,   181,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,     0,   168,     0,   169,     0,    46,    47,    48,    49,
      50,   410,   182,   183,     0,     0,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   184,   185,   186,   187,   188,
     189,   190,     0,   191,   192,     0,     0,     0,     0,     0,
       0,     0,   193,   194,   195,   196,   197,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   184,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   193,   194,
     195,   196,   197
};

static const yytype_int16 yycheck[] =
{
       0,    20,    14,     0,   128,   122,     6,   116,    12,     9,
      13,    29,   335,   211,    14,    67,   287,     9,    18,     6,
      20,   284,   134,   337,    91,    98,     9,    27,    12,    29,
      63,    12,    72,    14,    34,    27,   103,    70,    12,    39,
      33,    41,    42,   314,    27,    45,    98,    39,    38,    41,
      40,   253,    52,    45,    94,    52,    39,    12,    58,    12,
     383,   375,     3,     4,     5,    30,    31,    32,    33,    50,
      51,    58,    30,    31,    32,    33,   116,    77,    12,    60,
      98,    22,   206,    76,    14,   348,   126,   285,   351,   291,
      12,    91,    95,    29,    98,     7,     8,     9,    34,    30,
      31,    32,    33,   103,    34,    16,    87,    10,   379,    12,
       3,    14,    71,    72,   114,     3,    74,    67,    12,    32,
      50,    51,   231,    87,    27,    28,    68,    69,   128,    12,
      60,    94,    97,    98,    99,   100,     0,    87,     3,   402,
      14,    91,    92,    74,   109,   110,   111,   112,    98,   347,
      64,    65,    66,    67,    95,    62,   273,    87,    74,   283,
      75,    31,    98,    73,    61,    11,    13,    15,    17,    29,
      14,     3,    34,     3,   286,    16,   117,   118,    59,   120,
     121,     5,    60,   124,    13,    65,     5,    90,    88,    16,
       5,   231,    12,    96,    97,    98,    99,   100,    14,   199,
      13,    12,    88,    13,    13,   205,   206,   199,    12,    12,
      17,   211,    13,   213,    13,    29,   199,    13,    16,     3,
     260,    13,   334,    13,   264,    30,    31,    32,    33,    88,
      13,   250,    34,     3,    34,   235,   236,   237,   238,   239,
      17,    13,   242,   243,   244,   245,   246,   247,   248,    13,
     250,   363,   252,   253,    13,   247,   248,   222,    37,     3,
     300,   253,    13,    13,   247,   248,   378,   225,   226,   227,
     228,    13,   272,     5,   386,   315,    30,    31,    32,    33,
     221,   281,   223,   283,    17,   285,     3,    65,   288,   281,
       3,   291,   292,   234,   225,   226,   227,   228,   281,   291,
       3,   301,   406,    52,   109,   110,   111,   112,   370,   375,
     251,    38,   401,   325,   326,   301,   360,   329,   330,   382,
     237,   353,    38,   323,   242,   325,   326,   235,    38,   329,
     330,   323,   236,   238,   243,   335,    38,   337,    30,    38,
     337,   239,   244,    38,   245,    20,    -1,   347,    30,    31,
      32,    33,   293,   353,   246,   109,   110,   111,   112,   371,
     360,    -1,    54,    55,    56,    57,    58,    -1,    -1,    -1,
      -1,   371,    -1,    -1,    -1,   375,    -1,   318,   375,   391,
     321,    -1,    -1,   383,   396,    -1,   327,   328,    -1,    -1,
      -1,   391,   333,    85,    -1,    -1,   396,    -1,   410,    30,
      31,    32,    33,    -1,    -1,    -1,   406,    -1,    -1,    -1,
     410,     3,    -1,    -1,   406,    -1,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    16,    -1,    -1,   109,   110,   111,
     112,    -1,    -1,    -1,    -1,    27,    28,    -1,    30,    -1,
      32,   382,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,   109,   110,
     111,   112,    -1,    -1,    -1,    77,    78,    79,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    87,    -1,    89,    90,    91,
      92,    -1,    -1,    -1,    96,    97,    98,    99,   100,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    32,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    56,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    89,    90,    91,    92,    -1,
      -1,    -1,    96,    97,    98,    99,   100,     3,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    32,    -1,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    55,
      56,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    79,    -1,    -1,     8,    -1,    10,    -1,
      12,    87,    14,    89,    90,    91,    92,    -1,    -1,    -1,
      96,    97,    98,    99,   100,    27,    28,    -1,    -1,    -1,
      32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,     3,    55,    56,    57,    58,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    67,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    32,    -1,    -1,    35,    87,    -1,    -1,    90,    91,
      92,    93,    -1,    -1,    96,    97,    98,    99,   100,    50,
      51,    52,    53,     3,    55,    56,    57,    58,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    32,    -1,    -1,    35,    87,    -1,    -1,    90,
      91,    92,    -1,    -1,    -1,    96,    97,    98,    99,   100,
      50,    51,    52,    53,    -1,    55,    56,    57,    58,     4,
       5,     6,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      -1,    -1,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     8,    -1,    10,    -1,    12,    87,    14,    -1,
      90,    91,    92,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    27,    28,    -1,    30,    -1,    32,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      46,    -1,    -1,    -1,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    -1,    -1,    80,    81,    82,    83,    84,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    -1,     8,    -1,    10,    85,
      12,    87,    14,    89,    90,    91,    92,    -1,    -1,    -1,
      96,    97,    98,    99,   100,    27,    28,    -1,    -1,    -1,
      32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    56,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    89,    90,    91,
      92,    -1,    -1,    -1,    96,    97,    98,    99,   100,     8,
      -1,    10,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    51,    52,    53,    -1,    55,    56,    57,    58,
       8,    -1,    10,    -1,    12,    -1,    14,    15,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    32,    -1,    -1,    35,    87,    -1,
      -1,    90,    91,    92,    -1,    -1,    -1,    96,    97,    98,
      99,   100,    50,    51,    52,    53,    -1,    55,    56,    57,
      58,     8,    -1,    10,    -1,    12,    -1,    14,    15,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    32,    -1,    -1,    35,    87,
      -1,    -1,    90,    91,    92,    -1,    -1,    -1,    96,    97,
      98,    99,   100,    50,    51,    52,    53,    -1,    55,    56,
      57,    58,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    32,    -1,    -1,    35,
      87,    -1,    -1,    90,    91,    92,    -1,    -1,    -1,    96,
      97,    98,    99,   100,    50,    51,    52,    53,    -1,    55,
      56,    57,    58,     8,    -1,    10,    -1,    12,    -1,    14,
      15,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    32,    -1,    -1,
      35,    87,    -1,    -1,    90,    91,    92,    -1,    -1,    -1,
      96,    97,    98,    99,   100,    50,    51,    52,    53,    -1,
      55,    56,    57,    58,     8,    -1,    10,    -1,    12,    13,
      14,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    32,    -1,
      -1,    35,    87,    -1,    -1,    90,    91,    92,    -1,    -1,
      -1,    96,    97,    98,    99,   100,    50,    51,    52,    53,
      -1,    55,    56,    57,    58,     8,    -1,    10,    -1,    12,
      -1,    14,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    32,
      -1,    -1,    35,    87,    -1,    -1,    90,    91,    92,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    50,    51,    52,
      53,    -1,    55,    56,    57,    58,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    67,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      32,    -1,    -1,    35,    87,    -1,    -1,    90,    91,    92,
      -1,    -1,    -1,    96,    97,    98,    99,   100,    50,    51,
      52,    53,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    27,    28,    -1,
      -1,    -1,    32,    -1,    -1,    35,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,
      50,    51,    52,    53,    -1,    -1,     4,    -1,     6,     7,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,     4,    -1,     6,    -1,    96,    97,    98,    99,
     100,    13,    50,    51,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,
      82,    83,    84
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    30,
      32,    35,    36,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    67,    68,    69,    77,    78,    79,
      85,    87,    89,    90,    91,    92,    96,    97,    98,    99,
     100,   111,   112,   113,   119,   120,   121,   124,   125,   126,
     128,   129,   130,   131,   132,   134,   135,   136,   137,   138,
     139,   142,   143,   144,   147,   148,   150,   152,   154,   157,
     158,   159,   160,   161,   162,   164,   166,   168,   170,   172,
     173,   174,   175,   176,   180,   181,   182,    55,    56,    57,
      58,   126,   138,   174,    13,   138,    15,   138,    17,    55,
      56,    57,    58,   119,   122,   126,   152,    12,    12,   125,
      12,    12,    12,   126,    12,   126,   180,   182,    16,     3,
     138,     3,     3,   122,    32,   117,   126,   113,   120,   121,
     128,   144,   148,   154,   113,   120,   121,   128,   144,   148,
     154,   113,   120,   121,   128,   144,   148,   154,   113,   120,
     121,   128,   144,   148,   154,   118,   126,    12,     4,     6,
       7,     8,     9,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    50,    51,    63,    64,    65,    66,    67,    68,
      69,    71,    72,    80,    81,    82,    83,    84,   141,   149,
     165,   167,   169,   171,   179,   122,    87,   122,   177,   126,
     127,    12,   152,    94,     0,   111,   119,    29,    34,   151,
       3,     5,   141,    14,   142,    55,    56,    57,    58,   113,
     148,   126,   155,   156,    33,    76,    62,    74,    75,    73,
      63,    70,   163,   165,   167,   169,   171,    31,    78,   175,
      61,    14,    60,    87,   140,   142,   179,   138,    11,    13,
      15,    17,   126,   142,   143,   138,   138,    38,    40,   138,
     138,    93,   133,   137,     3,   138,    34,    34,   117,     3,
     124,    59,   114,     5,    65,    60,    13,   122,    12,   126,
     117,     5,    88,    16,   118,   126,   138,   144,    15,   142,
     143,     5,   138,   159,   160,   161,   162,   164,   166,   168,
     170,   172,   173,   122,   122,   180,   126,   177,    12,   178,
      13,    14,   142,    12,   145,    13,    13,    12,    12,    13,
      13,    29,   126,   137,    13,    17,   122,    16,   117,   151,
     118,   124,   145,   126,     3,    88,   177,    67,   126,   138,
     153,    13,   155,    34,   145,    88,    13,   138,    15,   123,
     152,    13,    95,   146,   125,   125,   138,   138,   125,   125,
      34,    13,   138,   124,   127,   112,   115,   116,    13,    13,
     118,   151,     3,    17,   151,   157,    13,    13,   156,    13,
     124,    37,    13,    13,   139,   125,    13,     3,   116,    17,
     124,   145,    65,   153,   127,   124,     5,   125,     3,     3,
      13,   125,   146,   151,     3,   123,   125
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   110,   111,   111,   112,   112,   112,   113,   113,   113,
     113,   113,   113,   114,   114,   115,   115,   116,   116,   117,
     117,   118,   118,   119,   119,   120,   120,   120,   120,   120,
     121,   121,   121,   121,   121,   122,   122,   122,   123,   123,
     124,   124,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   126,   126,   127,   127,   128,
     128,   128,   128,   128,   129,   130,   131,   131,   132,   133,
     133,   134,   134,   134,   134,   134,   134,   134,   134,   135,
     135,   136,   136,   136,   136,   137,   137,   138,   138,   139,
     139,   140,   140,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     142,   142,   142,   143,   143,   143,   144,   144,   144,   144,
     144,   144,   145,   145,   146,   146,   147,   148,   148,   148,
     148,   148,   148,   149,   149,   149,   149,   149,   149,   150,
     150,   150,   150,   151,   151,   152,   152,   152,   152,   152,
     152,   153,   153,   154,   154,   154,   154,   154,   155,   155,
     156,   156,   156,   157,   157,   158,   158,   159,   159,   160,
     160,   161,   161,   162,   162,   163,   163,   164,   164,   165,
     165,   165,   165,   166,   166,   167,   167,   168,   168,   169,
     169,   170,   170,   171,   171,   171,   172,   172,   173,   173,
     174,   174,   174,   174,   174,   174,   174,   174,   175,   175,
     175,   175,   176,   176,   177,   177,   178,   178,   179,   179,
     180,   180,   180,   180,   180,   181,   181,   181,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     6,     2,     2,
       2,     2,     2,     2,     0,     1,     0,     1,     2,     1,
       3,     1,     3,     1,     2,     7,     2,     2,     2,     2,
       4,     2,     2,     2,     2,     1,     2,     2,     2,     4,
       2,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     1,     6,
       2,     2,     2,     2,     2,     3,     3,     3,     5,     2,
       1,     5,     7,     6,     7,     8,     5,     7,     5,     5,
       7,     2,     3,     2,     2,     2,     1,     1,     3,     1,
       3,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     5,     2,     3,     2,     1,     2,     2,     2,
       2,     2,     1,     2,     1,     2,     4,     6,     2,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       6,     6,     4,     0,     1,     2,     4,     5,     7,     5,
       2,     2,     3,     7,     2,     2,     2,     2,     1,     3,
       1,     2,     2,     1,     5,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     3,     1,     1,     1,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     2,
       2,     3,     1,     3,     1,     3,     2,     3,     1,     1,
       1,     2,     3,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (arg, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, arg); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, Scanner* arg)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (arg);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, Scanner* arg)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, arg);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, Scanner* arg)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              , arg);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, arg); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, Scanner* arg)
{
  YYUSE (yyvaluep);
  YYUSE (arg);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 120: /* func_ptr  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1787 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 121: /* stmt_type  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1793 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 124: /* code_segment  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1799 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 125: /* stmt  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1805 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 128: /* enum_stmt  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1811 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 129: /* label_stmt  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1817 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 130: /* goto_stmt  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1823 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 131: /* case_stmt  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1829 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 132: /* switch_stmt  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1835 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 134: /* loop_stmt  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1841 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 135: /* selection_stmt  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1847 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 136: /* jump_stmt  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1853 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 137: /* exp_stmt  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1859 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 138: /* exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1865 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 139: /* binary_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1871 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 144: /* decl_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1877 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 154: /* union_stmt  */
#line 103 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1883 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 157: /* con_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1889 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 158: /* log_or_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1895 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 159: /* log_and_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1901 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 160: /* inc_or_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1907 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 161: /* exc_or_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1913 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 162: /* and_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1919 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 164: /* eq_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1925 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 166: /* relational_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1931 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 168: /* shift_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1937 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 170: /* add_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1943 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 172: /* mul_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1949 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 173: /* cast_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1955 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 175: /* unary_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1961 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 176: /* dur_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1967 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 178: /* call_paren  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1973 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 180: /* post_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1979 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 182: /* primary_exp  */
#line 104 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1985 "src/ast/parser.c" /* yacc.c:1258  */
        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (Scanner* arg)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, scan);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 108 "utils/gwion.y" /* yacc.c:1648  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL);  }
#line 2253 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 3:
#line 109 "utils/gwion.y" /* yacc.c:1648  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast)); }
#line 2259 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 4:
#line 113 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list)); }
#line 2265 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 5:
#line 114 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def)); }
#line 2271 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 6:
#line 115 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def)); }
#line 2277 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 7:
#line 120 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_def) = new_class_def(0, (yyvsp[-4].id_list), (yyvsp[-3].type_decl), (yyvsp[-1].class_body)); }
#line 2283 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 8:
#line 121 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_static);  (yyval.class_def) = (yyvsp[0].class_def); }
#line 2289 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 9:
#line 122 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_global);  (yyval.class_def) = (yyvsp[0].class_def); }
#line 2295 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 10:
#line 123 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_private); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2301 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 11:
#line 124 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_protect); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2307 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 12:
#line 125 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].class_def), free_class_def); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2313 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 13:
#line 127 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2319 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 14:
#line 127 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = NULL; }
#line 2325 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 16:
#line 129 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = NULL; }
#line 2331 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 17:
#line 132 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = new_class_body((yyvsp[0].section), NULL); }
#line 2337 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 18:
#line 133 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = new_class_body((yyvsp[-1].section), (yyvsp[0].class_body)); }
#line 2343 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 19:
#line 137 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2349 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 20:
#line 138 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2355 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 21:
#line 142 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2361 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 22:
#line 143 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2367 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 23:
#line 147 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL);}
#line 2373 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 24:
#line 148 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list));}
#line 2379 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 25:
#line 153 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_fptr((yyvsp[-3].sym), (yyvsp[-5].type_decl), (yyvsp[-1].arg_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2385 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 26:
#line 154 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_fptr.td), ae_flag_static);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 2391 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 27:
#line 155 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_fptr.td), ae_flag_global);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 2397 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 28:
#line 156 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_fptr.td), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2403 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 29:
#line 157 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_fptr.td), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2409 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 30:
#line 162 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_type((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2415 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 31:
#line 163 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_type.td), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2421 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 32:
#line 164 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_type.td), ae_flag_global); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2427 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 33:
#line 165 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_type.td), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2433 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 34:
#line 166 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_type.td), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2439 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 36:
#line 170 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub)); }
#line 2445 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 37:
#line 171 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub)); }
#line 2451 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 38:
#line 175 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL); }
#line 2457 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 39:
#line 176 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list)); }
#line 2463 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 40:
#line 180 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_code(NULL, get_pos(arg)); }
#line 2469 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 41:
#line 181 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_code((yyvsp[-1].stmt_list), get_pos(arg)); }
#line 2475 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 55:
#line 203 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.sym) = insert_symbol((yyvsp[0].sval)); }
#line 2481 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 56:
#line 204 "utils/gwion.y" /* yacc.c:1648  */
    {
    char c[strlen(s_name((yyvsp[0].sym))) + strlen((yyvsp[-2].sval))];
    sprintf(c, "%s%s", (yyvsp[-2].sval), s_name((yyvsp[0].sym)));
    (yyval.sym) = insert_symbol(c);
  }
#line 2491 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 57:
#line 211 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.sym) = NULL; }
#line 2497 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 59:
#line 214 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sym), get_pos(arg)); }
#line 2503 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 60:
#line 215 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_static);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 2509 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 61:
#line 216 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_global);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 2515 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 62:
#line 217 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2521 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 63:
#line 218 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2527 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 64:
#line 222 "utils/gwion.y" /* yacc.c:1648  */
    {  (yyval.stmt) = new_stmt_jump((yyvsp[-1].sym), 1, get_pos(arg)); }
#line 2533 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 65:
#line 226 "utils/gwion.y" /* yacc.c:1648  */
    {  (yyval.stmt) = new_stmt_jump((yyvsp[-1].sym), 0, get_pos(arg)); }
#line 2539 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 66:
#line 230 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_case, (yyvsp[-1].exp), get_pos(arg)); }
#line 2545 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 67:
#line 231 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_case, (yyvsp[-1].exp), get_pos(arg)); }
#line 2551 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 68:
#line 235 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg));}
#line 2557 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 69:
#line 238 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 1; }
#line 2563 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 70:
#line 238 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 2569 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 71:
#line 242 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_while, (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2575 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 72:
#line 244 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_while, (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2581 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 73:
#line 246 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2587 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 74:
#line 248 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2593 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 75:
#line 250 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), (yyvsp[-5].ival), get_pos(arg)); }
#line 2599 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 76:
#line 252 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_until, (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2605 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 77:
#line 254 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_until, (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2611 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 78:
#line 256 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_loop((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2617 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 79:
#line 261 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(arg)); }
#line 2623 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 80:
#line 263 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(arg)); }
#line 2629 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 81:
#line 267 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_return, NULL, get_pos(arg)); }
#line 2635 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 82:
#line 268 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_return, (yyvsp[-1].exp), get_pos(arg)); }
#line 2641 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 83:
#line 269 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt(ae_stmt_break, get_pos(arg)); }
#line 2647 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 84:
#line 270 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt(ae_stmt_continue, get_pos(arg)); }
#line 2653 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 85:
#line 274 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_exp, (yyvsp[-1].exp),   get_pos(arg)); }
#line 2659 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 86:
#line 275 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_exp, NULL, get_pos(arg)); }
#line 2665 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 88:
#line 280 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = prepend_exp((yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 2671 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 90:
#line 285 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2677 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 91:
#line 288 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = NULL; }
#line 2683 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 92:
#line 288 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2689 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 93:
#line 290 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_chuck; }
#line 2695 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 94:
#line 290 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_unchuck; }
#line 2701 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 95:
#line 290 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_eq; }
#line 2707 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 96:
#line 291 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_ref; }
#line 2713 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 97:
#line 291 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_radd; }
#line 2719 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 98:
#line 292 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsub; }
#line 2725 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 99:
#line 292 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rmul; }
#line 2731 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 100:
#line 293 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rdiv; }
#line 2737 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 101:
#line 293 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rmod; }
#line 2743 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 102:
#line 294 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_trig; }
#line 2749 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 103:
#line 294 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_untrig; }
#line 2755 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 104:
#line 295 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsl; }
#line 2761 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 105:
#line 295 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsr; }
#line 2767 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 106:
#line 295 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsand; }
#line 2773 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 107:
#line 296 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsor; }
#line 2779 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 108:
#line 296 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsxor; }
#line 2785 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 109:
#line 297 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_assign; }
#line 2791 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 110:
#line 301 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = new_array_sub((yyvsp[-1].exp), get_pos(arg)); }
#line 2797 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 111:
#line 302 "utils/gwion.y" /* yacc.c:1648  */
    { if((yyvsp[-2].exp)->next){ gwion_error(arg, "invalid format for array init [...][...]..."); free_exp((yyvsp[-2].exp)); free_array_sub((yyvsp[0].array_sub)); YYERROR; } (yyval.array_sub) = prepend_array_sub((yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2803 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 112:
#line 303 "utils/gwion.y" /* yacc.c:1648  */
    { gwion_error(arg, "partially empty array init [...][]..."); free_exp((yyvsp[-3].exp)); YYERROR; }
#line 2809 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 113:
#line 307 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(arg)); }
#line 2815 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 114:
#line 308 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2821 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 115:
#line 309 "utils/gwion.y" /* yacc.c:1648  */
    { gwion_error(arg, "partially empty array init [][...]"); free_array_sub((yyvsp[-1].array_sub)); free_array_sub((yyvsp[0].array_sub)); YYERROR; }
#line 2827 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 117:
#line 314 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2833 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 118:
#line 316 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_static);  (yyval.exp) = (yyvsp[0].exp); }
#line 2839 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 119:
#line 318 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_global);  (yyval.exp) = (yyvsp[0].exp); }
#line 2845 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 120:
#line 320 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_private); (yyval.exp) = (yyvsp[0].exp); }
#line 2851 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 121:
#line 322 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_protect); (yyval.exp) = (yyvsp[0].exp); }
#line 2857 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 122:
#line 326 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = NULL; }
#line 2863 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 123:
#line 327 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = (yyvsp[0].arg_list); }
#line 2869 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 124:
#line 331 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 2875 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 125:
#line 332 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2881 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 126:
#line 335 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2887 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 127:
#line 339 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-4].type_decl), (yyvsp[-3].sym), (yyvsp[-2].arg_list), (yyvsp[0].stmt), (yyvsp[-1].ival)); }
#line 2893 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 128:
#line 341 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_static); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2899 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 129:
#line 343 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_global); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2905 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 130:
#line 345 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_private); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2911 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 131:
#line 347 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_protect); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2917 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 132:
#line 349 "utils/gwion.y" /* yacc.c:1648  */
    {
      if((yyvsp[0].func_def)->tmpl) {
        free_id_list((yyvsp[-1].id_list));
        free_func_def((yyvsp[0].func_def));
        gwion_error(arg, "double template decl");
        YYERROR;
      }
      (yyvsp[0].func_def)->tmpl = new_tmpl_list((yyvsp[-1].id_list), -1);
      (yyval.func_def) = (yyvsp[0].func_def); SET_FLAG((yyval.func_def), ae_flag_template);
    }
#line 2932 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 140:
#line 364 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-3].type_decl), OP_SYM((yyvsp[-4].ival)), (yyvsp[-2].arg_list), (yyvsp[0].stmt), ae_flag_op); }
#line 2938 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 141:
#line 366 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-3].type_decl), OP_SYM((yyvsp[-5].ival)), (yyvsp[-2].arg_list), (yyvsp[0].stmt), ae_flag_op | ae_flag_unary); }
#line 2944 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 142:
#line 368 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def(new_type_decl(new_id_list(insert_symbol("void"), get_pos(arg)), 0,
      get_pos(arg)), insert_symbol("dtor"), NULL, (yyvsp[0].stmt), ae_flag_dtor); }
#line 2951 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 143:
#line 372 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 2957 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 144:
#line 372 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_flag_ref; }
#line 2963 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 145:
#line 375 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)), (yyvsp[0].ival), get_pos(arg)); }
#line 2969 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 146:
#line 376 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2975 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 147:
#line 377 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)),
      (yyvsp[0].ival), get_pos(arg)); (yyval.type_decl)->types = (yyvsp[-3].type_list); }
#line 2982 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 148:
#line 379 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg));
      (yyval.type_decl)->types = (yyvsp[-5].type_list); }
#line 2989 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 149:
#line 381 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2995 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 150:
#line 382 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].type_decl), ae_flag_const); (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 3001 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 151:
#line 386 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 3007 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 152:
#line 387 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 3013 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 153:
#line 391 "utils/gwion.y" /* yacc.c:1648  */
    {
      (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(arg));
      (yyval.stmt)->d.stmt_union.type_xid = (yyvsp[-5].sym);
      (yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sym);
    }
#line 3023 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 154:
#line 397 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 3029 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 155:
#line 399 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_global);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 3035 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 156:
#line 401 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 3041 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 157:
#line 403 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 3047 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 158:
#line 407 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL); }
#line 3053 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 159:
#line 408 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list)); }
#line 3059 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 160:
#line 412 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sym), NULL, get_pos(arg)); }
#line 3065 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 161:
#line 413 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 3071 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 162:
#line 414 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 3077 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 164:
#line 418 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_if((yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3083 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 166:
#line 421 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(arg)); }
#line 3089 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 168:
#line 424 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(arg)); }
#line 3095 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 170:
#line 427 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_sor, (yyvsp[0].exp), get_pos(arg)); }
#line 3101 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 172:
#line 430 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_sxor, (yyvsp[0].exp), get_pos(arg)); }
#line 3107 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 174:
#line 433 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_sand, (yyvsp[0].exp), get_pos(arg)); }
#line 3113 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 175:
#line 435 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_eq; }
#line 3119 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 176:
#line 435 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_ne; }
#line 3125 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 178:
#line 438 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3131 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 179:
#line 440 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_lt; }
#line 3137 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 180:
#line 440 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_gt; }
#line 3143 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 181:
#line 441 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_le; }
#line 3149 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 182:
#line 441 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_ge; }
#line 3155 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 184:
#line 444 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3161 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 185:
#line 447 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_shl;  }
#line 3167 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 186:
#line 448 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_shr; }
#line 3173 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 188:
#line 452 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3179 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 189:
#line 454 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_add; }
#line 3185 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 190:
#line 454 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_sub; }
#line 3191 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 192:
#line 457 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3197 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 193:
#line 459 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_mul; }
#line 3203 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 194:
#line 459 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_div; }
#line 3209 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 195:
#line 459 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_mod; }
#line 3215 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 197:
#line 462 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3221 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 199:
#line 465 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_cast((yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(arg)); }
#line 3227 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 200:
#line 467 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_add; }
#line 3233 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 201:
#line 467 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_sub; }
#line 3239 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 202:
#line 467 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_mul; }
#line 3245 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 203:
#line 468 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_inc; }
#line 3251 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 204:
#line 468 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_dec; }
#line 3257 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 205:
#line 469 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_not; }
#line 3263 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 206:
#line 469 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_spork; }
#line 3269 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 207:
#line 469 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_cmp; }
#line 3275 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 209:
#line 473 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_unary((yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3281 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 210:
#line 475 "utils/gwion.y" /* yacc.c:1648  */
    {
      if((yyvsp[0].type_decl)->array && !(yyvsp[0].type_decl)->array->exp) {
        free_type_decl((yyvsp[0].type_decl));
        gwion_error(arg, "can't use empty '[]' in 'new' expression");
        YYERROR;
      }
      (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), get_pos(arg));
    }
#line 3294 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 211:
#line 484 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_unary3(op_spork, (yyvsp[0].stmt), get_pos(arg)); }
#line 3300 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 213:
#line 487 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_dur((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3306 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 214:
#line 490 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = new_type_list((yyvsp[0].type_decl), NULL); }
#line 3312 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 215:
#line 491 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = new_type_list((yyvsp[-2].type_decl), (yyvsp[0].type_list)); }
#line 3318 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 216:
#line 494 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = NULL; }
#line 3324 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 217:
#line 494 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3330 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 218:
#line 496 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_inc; }
#line 3336 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 219:
#line 496 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_dec; }
#line 3342 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 221:
#line 499 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_array((yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(arg)); }
#line 3348 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 222:
#line 501 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_call((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg));
      if((yyvsp[-1].type_list))(yyval.exp)->d.exp_func.tmpl = new_tmpl_call((yyvsp[-1].type_list)); }
#line 3355 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 223:
#line 504 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_dot((yyvsp[-2].exp), (yyvsp[0].sym), get_pos(arg)); }
#line 3361 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 224:
#line 506 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_post((yyvsp[-1].exp), (yyvsp[0].ival), get_pos(arg)); }
#line 3367 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 225:
#line 509 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_complex; }
#line 3373 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 226:
#line 510 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_polar;   }
#line 3379 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 227:
#line 511 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_vec;     }
#line 3385 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 228:
#line 514 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_id(     (yyvsp[0].sym), get_pos(arg)); }
#line 3391 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 229:
#line 515 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_int(    (yyvsp[0].ival), get_pos(arg)); }
#line 3397 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 230:
#line 516 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_float(  (yyvsp[0].fval), get_pos(arg)); }
#line 3403 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 231:
#line 517 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_string( (yyvsp[0].sval), get_pos(arg)); }
#line 3409 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 232:
#line 518 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_char(   (yyvsp[0].sval), get_pos(arg)); }
#line 3415 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 233:
#line 519 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3421 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 234:
#line 520 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3427 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 235:
#line 521 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_vec((yyvsp[-2].ival), (yyvsp[-1].exp), get_pos(arg)); }
#line 3433 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 236:
#line 522 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_hack(   (yyvsp[-1].exp), get_pos(arg)); }
#line 3439 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 237:
#line 523 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) =                      (yyvsp[-1].exp);                }
#line 3445 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 238:
#line 524 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_nil(        get_pos(arg)); }
#line 3451 "src/ast/parser.c" /* yacc.c:1648  */
    break;


#line 3455 "src/ast/parser.c" /* yacc.c:1648  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (arg, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (arg, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, arg);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, arg);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (arg, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, arg);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, arg);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 526 "utils/gwion.y" /* yacc.c:1907  */

