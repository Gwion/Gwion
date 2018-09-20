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

#include <string.h> // strlen in paste operation
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

#line 97 "src/ast/parser.c" /* yacc.c:339  */

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
#line 30 "utils/gwion.y" /* yacc.c:355  */

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

#line 380 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (Scanner* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 396 "src/ast/parser.c" /* yacc.c:358  */

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
#define YYFINAL  222
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1650

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  110
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  74
/* YYNRULES -- Number of rules.  */
#define YYNRULES  247
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  425

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
       0,   106,   106,   107,   111,   112,   113,   117,   119,   120,
     121,   122,   123,   125,   125,   127,   127,   130,   131,   135,
     136,   140,   141,   145,   146,   150,   152,   153,   154,   155,
     159,   161,   162,   163,   164,   167,   168,   169,   173,   174,
     178,   179,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   212,   213,   220,   220,   223,
     224,   225,   226,   227,   231,   235,   239,   240,   244,   247,
     247,   250,   252,   254,   256,   258,   260,   262,   264,   269,
     271,   276,   277,   278,   279,   283,   284,   288,   289,   293,
     294,   297,   297,   299,   299,   299,   300,   300,   301,   301,
     302,   302,   303,   303,   304,   304,   304,   305,   305,   306,
     310,   311,   312,   316,   317,   318,   322,   323,   324,   326,
     328,   330,   335,   336,   340,   341,   344,   347,   349,   351,
     353,   355,   357,   369,   369,   369,   369,   369,   369,   371,
     372,   374,   376,   381,   381,   384,   385,   386,   388,   390,
     391,   395,   396,   400,   401,   403,   405,   407,   412,   413,
     417,   418,   419,   422,   422,   425,   425,   428,   428,   431,
     431,   434,   434,   437,   437,   440,   440,   442,   442,   445,
     445,   446,   446,   448,   448,   452,   453,   456,   456,   459,
     459,   461,   461,   464,   464,   464,   466,   466,   469,   469,
     472,   472,   472,   473,   473,   474,   474,   477,   477,   479,
     488,   491,   491,   495,   496,   499,   499,   501,   501,   503,
     503,   505,   508,   510,   514,   515,   516,   519,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529
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
  "arg_list", "code_segment", "stmt_pp", "stmt", "id", "opt_id",
  "enum_stmt", "label_stmt", "goto_stmt", "case_stmt", "switch_stmt",
  "auto", "loop_stmt", "selection_stmt", "jump_stmt", "exp_stmt", "exp",
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

#define YYPACT_NINF -336

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-336)))

#define YYTABLE_NINF -154

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     413,  -336,  -336,  1375,  1069,  1120,   520,  -336,  -336,   -35,
    -336,     4,    18,   627,    34,    62,    71,   -50,    82,    31,
      88,   785,   114,   115,  -336,  -336,   -35,    92,   -50,   910,
     910,   910,   910,   -50,  -336,  -336,   108,  1530,   -35,    46,
     -35,   118,  -336,   124,   -35,  -336,  -336,    50,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,   143,
     413,  -336,  -336,  -336,  -336,  -336,  -336,   627,     5,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,   146,   891,
    -336,   136,  -336,   126,  -336,  -336,   -50,  -336,  -336,   -21,
      90,    80,    93,    96,   -39,    99,    36,    42,    57,   139,
    1426,  -336,   110,   181,  1375,  -336,  1375,  1375,  1375,  1375,
       8,   161,  1478,  -336,   160,  -336,   159,  -336,   976,   976,
     976,   976,   158,   -50,   148,   164,  1375,  1375,   -11,  1375,
    1375,   734,   176,  1375,  -336,    75,   155,   -50,  -336,   187,
    -336,  -336,  -336,   175,   133,   189,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,   131,   137,   190,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,   -35,  -336,
    -336,  -336,  -336,  -336,     3,   -50,   195,   119,  1375,   -50,
    -336,   -50,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  1375,
    1375,  1171,  -336,   126,   126,   126,   126,  -336,  -336,   164,
    -336,   201,  1375,  1478,  1478,  1478,  1478,  1478,  -336,  -336,
    1478,  1478,  1478,  1478,  1478,   -35,   -35,  -336,    31,  1375,
     -50,   -35,   198,  -336,  -336,   199,  -336,  -336,   202,  -336,
     206,  -336,   136,   200,   207,   210,   211,   212,   213,   205,
     -50,   836,  -336,   222,  -336,  -336,   219,  -336,  -336,   -35,
     221,   -50,   148,   -50,   175,   206,   -50,   236,   152,   -35,
      24,   259,   231,   250,  -336,  -336,  -336,  -336,  -336,   136,
     -50,   232,    90,    80,    93,    96,   -39,    99,    36,    42,
      57,   139,  -336,   206,   181,  -336,   179,  1222,  -336,  -336,
    1273,  -336,   -35,    -3,   627,   627,  1375,  1375,   627,   627,
    -336,   239,  1324,   175,   -50,  -336,   413,  -336,  -336,  -336,
    -336,   265,   268,  -336,  -336,  -336,   -50,   148,  1375,   -50,
     148,  -336,  1478,   273,  -336,  -336,   274,  -336,  -336,   -50,
    -336,   275,   175,   255,  -336,   282,   284,  -336,  -336,  1375,
     627,   289,  -336,  -336,   301,   413,   290,  -336,   175,   206,
     241,  -336,  -336,   306,  -336,  -336,   175,  -336,   307,  -336,
    -336,   627,   308,   310,  1566,  -336,   627,  -336,  -336,  -336,
    -336,    -3,   148,  -336,  -336,   -35,  -336,  -336,  -336,   627,
    -336,  -336,  -336,  -336,  -336
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    96,   212,     0,     0,     0,     0,   235,   234,     0,
     215,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   213,   214,     0,     0,     0,     0,
       0,     0,     0,     0,   211,   210,     0,     0,     0,     0,
       0,     0,   236,     0,     0,   238,   239,    65,   240,   241,
      42,    43,    44,    45,    46,    47,    48,    49,    50,     0,
       2,     6,     4,    61,    62,    54,    64,    23,   237,    59,
      55,    56,    58,    57,    52,    53,    60,    51,     0,    97,
     242,   243,    99,     0,   149,     5,     0,    63,   126,   173,
     175,   177,   179,   181,   183,   187,   193,   197,   201,   206,
       0,   208,   217,   221,     0,   229,     0,     0,     0,     0,
     237,     0,     0,   247,     0,   123,     0,    40,     0,     0,
       0,     0,     0,     0,   153,    35,     0,     0,     0,     0,
       0,     0,     0,     0,   237,   101,   229,     0,    91,     0,
      93,    94,   219,   216,    14,    19,     8,    26,    31,    70,
     128,   138,   164,     9,    27,    32,    71,   129,   139,   165,
      10,    28,    33,    72,   130,   140,   166,    11,    29,    34,
      73,   131,   141,   167,     0,    21,     0,   103,   119,   204,
     203,   205,   107,   108,   109,   110,   111,   106,   104,   112,
     113,   227,   228,   105,   192,   190,   191,   189,   200,   199,
     195,   196,   114,   115,   116,   117,   118,   143,     0,   146,
     144,   148,   147,   145,     0,     0,   223,     0,     0,     0,
     160,     0,     1,     3,    24,   154,    74,   155,    95,     0,
       0,     0,   125,     0,     0,     0,     0,    12,   142,   170,
     127,   168,     0,     0,     0,     0,     0,     0,   185,   186,
       0,     0,     0,     0,     0,     0,     0,   218,     0,     0,
       0,     0,     0,   230,   233,     0,   245,   246,   120,    41,
       0,    37,    36,     0,     0,     0,     0,     0,     0,    80,
       0,     0,    75,     0,    77,    76,     0,    92,   220,     0,
       0,     0,   153,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    98,   100,   124,   171,   172,
       0,     0,   176,   178,   180,   182,   184,   188,   194,   198,
     202,   207,   209,     0,   222,   232,     0,     0,   231,   244,
       0,   121,   132,     0,     0,     0,     0,     0,     0,     0,
      79,     0,     0,     0,    67,    13,    16,    20,   156,    22,
     152,     0,     0,    30,   136,   224,     0,   153,   161,    67,
     153,   169,     0,     0,   102,   225,     0,   122,   133,     0,
     134,     0,     0,    89,    81,     0,     0,    86,    88,     0,
       0,     0,    78,    68,     0,    17,     0,    15,     0,     0,
       0,   157,   162,     0,   159,   174,     0,   226,    38,   135,
     137,     0,     0,     0,     0,    83,     0,    69,    18,     7,
     150,     0,   153,   163,   151,     0,    90,    82,    87,     0,
      84,    25,   158,    39,    85
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -336,   256,  -335,    49,  -336,  -336,   -70,  -130,  -205,    15,
      67,   109,    16,   -94,  -139,  -336,   -12,     0,   -34,   245,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -128,    72,
     -55,  -336,   291,   -30,  -120,    39,  -287,   -82,  -336,    84,
    -336,  -336,  -270,     9,   -25,   366,    20,   -33,   -22,  -336,
      94,    98,   100,    97,  -336,   102,   313,   101,   315,   103,
     316,    95,   318,   104,   106,     2,   -28,  -336,  -238,  -336,
     321,     1,  -336,   342
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    59,    60,    61,   290,   386,   387,   144,   174,    62,
      63,    64,   216,   368,    65,    66,    67,   110,   384,    69,
      70,    71,    72,    73,   280,    74,    75,    76,    77,    78,
      79,   262,   230,    80,    81,    82,   333,   372,    83,    84,
     208,    85,   227,    86,   302,    87,   240,   241,    88,    89,
      90,    91,    92,    93,   250,    94,   251,    95,   252,    96,
     253,    97,   254,    98,    99,   112,   101,   102,   217,   328,
     264,   103,   104,   105
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      68,   128,   100,   281,   288,   272,    68,   286,   351,   124,
     370,   385,   242,    68,   303,   296,   126,   132,   125,   134,
     135,   122,   348,   326,   248,   123,   124,   275,   145,   276,
     127,   249,    33,   175,   225,   125,   363,   225,   124,   226,
     124,     3,   142,     4,   124,     5,   129,   125,    47,   125,
     385,   232,    40,   220,   214,   243,    43,    44,     7,     8,
      68,   355,   100,    47,   179,   180,   181,    68,   150,   157,
     164,   171,   257,   263,   130,   111,   114,   116,   146,   153,
     160,   167,   224,   131,   257,   298,   239,   391,   349,   259,
     394,   356,   371,   139,   133,   271,   147,   154,   161,   168,
     134,    47,   411,  -153,   137,   263,  -153,   200,   201,   284,
     198,   199,   134,   151,   158,   165,   172,   140,   141,   309,
     176,    42,    47,   270,   143,   191,   192,    45,    46,    47,
      48,    49,   237,   215,   218,   260,   219,   145,   148,   155,
     162,   169,   422,   222,   221,   150,   157,   164,   171,   228,
     231,   390,   244,   342,   245,   350,     9,   150,   157,   164,
     171,   347,   261,   194,   195,   196,   197,   238,   246,   247,
     255,   258,   266,   267,   268,   269,   265,   225,     5,   282,
      28,   233,   234,   235,   236,   147,   154,   161,   168,   285,
     287,     6,   289,  -101,   291,   259,   292,   293,   273,   274,
     299,   277,   278,   294,   382,   283,   310,   300,   124,   308,
     327,    39,   329,   334,   297,   145,   330,   125,   332,   175,
     335,   304,   336,   337,   295,   338,   339,   148,   155,   162,
     169,   191,   192,   400,   340,   343,   344,   346,   331,   353,
     354,   260,   232,   134,   134,   134,   134,   134,   359,   410,
     134,   134,   134,   134,   134,   124,   124,   414,   134,   324,
     325,   124,   358,   360,   125,   125,   362,   364,   261,   306,
     125,   322,   323,   379,   149,   156,   163,   170,   388,   232,
     341,   389,   146,   153,   160,   167,   396,   397,   399,   124,
     301,   145,   401,   175,   263,   402,   352,   403,   125,   124,
     357,   305,   406,   116,   407,   345,   412,   409,   125,   413,
     239,   417,   415,   418,   311,   408,   223,   151,   158,   165,
     172,   423,   373,   374,   404,   393,   377,   378,   207,   421,
     361,   116,   124,   392,    68,    68,   398,   312,    68,    68,
     395,   369,   313,   315,   383,   314,    68,   319,   100,   316,
     209,   317,   210,   211,   318,   212,   175,   320,   213,   383,
     321,   136,   134,   149,   156,   163,   170,     0,   405,   239,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,    68,     0,   100,     0,   416,
       0,     0,     0,     0,   420,   152,   159,   166,   173,   366,
       0,    68,   116,     0,     0,     0,    68,   424,   375,   376,
       0,     0,     0,     0,   381,   124,     1,     0,     0,    68,
       0,     2,     0,     3,   369,     4,     0,     5,     0,     6,
     301,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     9,     0,     0,     0,     0,    10,    11,
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,     0,     0,     0,     0,     0,     0,     0,     0,
      33,    34,    35,     0,   152,   159,   166,   173,     0,     0,
      36,    37,    38,     0,     0,     0,     0,     0,    39,     0,
      40,     0,    41,    42,    43,    44,     0,     0,     0,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,     1,     0,     0,     0,     0,     2,     0,
       3,     0,     4,     0,     5,     0,     6,   117,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    11,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,   118,   119,   120,   121,     0,
       0,     0,     0,     0,     0,     0,     0,    33,    34,    35,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    40,     0,    41,
      42,    43,    44,     0,     0,     0,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
       1,     0,     0,     0,     0,     2,     0,     3,     0,     4,
       0,     5,     0,     6,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
       0,     0,    10,    11,     0,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,   118,   119,   120,   121,     0,     0,     0,     0,
       0,     0,     0,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    40,     0,    41,    42,    43,    44,
       0,     0,     0,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,     1,     0,     0,
       0,     0,     2,     0,     3,     0,     4,     0,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    25,    26,    27,   138,   106,
     107,   108,   109,     2,     0,     3,     0,     4,     0,     5,
       0,    33,    34,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,     0,     0,
      10,    40,     0,     0,    42,    43,    44,   279,     0,     0,
      45,    46,    47,    48,    49,    24,    25,    26,    27,     1,
     106,   107,   108,   109,     2,     0,     3,     0,     4,     0,
       5,     0,    33,    34,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,    40,     0,     0,    42,    43,    44,     0,     0,
       0,    45,    46,    47,    48,    49,    24,    25,    26,    27,
       0,   106,   107,   108,   109,   177,   229,   178,     0,     0,
       0,     0,     0,    33,    34,    35,     0,     0,     0,   182,
     183,   184,   185,   186,   187,   188,   189,   190,     2,     0,
       3,     0,     4,    40,     5,     0,    42,    43,    44,     0,
       0,     0,    45,    46,    47,    48,    49,     7,     8,     0,
       9,     0,     0,     0,     0,    10,     0,     0,     0,     0,
       0,     0,     0,     0,   193,     0,    20,     0,     0,     0,
      24,    25,    26,    27,    28,    29,    30,    31,    32,     0,
       0,   202,   203,   204,   205,   206,     0,    33,    34,    35,
       0,     0,     0,     0,     2,     0,     3,     0,     4,    38,
       5,     0,     0,     0,     0,    39,     0,    40,     0,    41,
      42,    43,    44,     7,     8,     0,    45,    46,    47,    48,
      49,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,     0,     0,     0,    24,    25,    26,    27,
       0,   118,   119,   120,   121,     0,     0,     0,     0,     0,
       0,     0,     0,    33,    34,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    40,     0,    41,    42,    43,    44,     0,
       0,     0,    45,    46,    47,    48,    49,     2,     0,     3,
       0,     4,   113,     5,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,    10,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
      25,    26,    27,     0,   106,   107,   108,   109,     2,     0,
       3,     0,     4,     0,     5,   115,    33,    34,    35,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    40,     0,     0,    42,
      43,    44,     0,     0,     0,    45,    46,    47,    48,    49,
      24,    25,    26,    27,     0,   106,   107,   108,   109,     2,
       0,     3,     0,     4,     0,     5,   307,    33,    34,    35,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
       0,     0,     0,     0,     0,     0,    10,    40,     0,     0,
      42,    43,    44,     0,     0,     0,    45,    46,    47,    48,
      49,    24,    25,    26,    27,     0,   106,   107,   108,   109,
       2,     0,     3,     0,     4,   365,     5,     0,    33,    34,
      35,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,     0,     0,    10,    40,     0,
       0,    42,    43,    44,     0,     0,     0,    45,    46,    47,
      48,    49,    24,    25,    26,    27,     0,   106,   107,   108,
     109,     2,     0,     3,     0,     4,     0,     5,   367,    33,
      34,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,    10,    40,
       0,     0,    42,    43,    44,     0,     0,     0,    45,    46,
      47,    48,    49,    24,    25,    26,    27,     0,   106,   107,
     108,   109,     2,     0,     3,     0,     4,   380,     5,     0,
      33,    34,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      40,     0,     0,    42,    43,    44,     0,     0,     0,    45,
      46,    47,    48,    49,    24,    25,    26,    27,     0,   106,
     107,   108,   109,     2,     0,     3,     0,     4,     0,     5,
       0,    33,    34,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,     0,     0,
      10,    40,     0,     0,    42,    43,    44,     0,     0,     0,
      45,    46,    47,    48,    49,    24,    25,    26,    27,     0,
     106,   107,   108,   109,     2,     0,     3,     0,     4,     0,
       5,     0,    33,    34,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,    40,     0,     0,    42,    43,    44,     0,     0,
       0,    45,    46,    47,    48,    49,    24,    25,    26,    27,
       0,     0,     0,     0,     0,     0,     2,     0,     3,     0,
       4,     0,     5,     0,    34,    35,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     7,     8,     0,     0,     0,
       0,     0,     0,    10,     0,     0,    42,     0,     0,     0,
       0,     0,    45,    46,    47,    48,    49,     0,    24,    25,
      26,    27,     0,     0,   177,     0,   178,   179,   180,   181,
       0,     0,     0,     0,     0,     0,    34,    35,   182,   183,
     184,   185,   186,   187,   188,   189,   190,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    42,     0,
     177,     0,   178,     0,    45,    46,    47,    48,    49,   419,
     191,   192,     0,     0,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   193,   194,   195,   196,   197,   198,   199,
       0,   200,   201,     0,     0,     0,     0,     0,     0,     0,
     202,   203,   204,   205,   206,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   193,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   202,   203,   204,   205,
     206
};

static const yytype_int16 yycheck[] =
{
       0,    13,     0,   131,   143,   125,     6,   137,   295,     9,
      13,   346,    33,    13,   219,    12,    12,    17,     9,    19,
      19,     6,   292,   261,    63,     9,    26,    38,    28,    40,
      12,    70,    67,    33,    29,    26,   323,    29,    38,    34,
      40,    10,    26,    12,    44,    14,    12,    38,    98,    40,
     385,    81,    87,    44,    38,    76,    91,    92,    27,    28,
      60,   299,    60,    98,     7,     8,     9,    67,    29,    30,
      31,    32,   100,   103,    12,     3,     4,     5,    29,    30,
      31,    32,    67,    12,   112,   215,    86,   357,   293,    14,
     360,    67,    95,    21,    12,   125,    29,    30,    31,    32,
     100,    98,   389,    98,    16,   135,    98,    71,    72,    34,
      68,    69,   112,    29,    30,    31,    32,     3,     3,   239,
      12,    90,    98,   123,    32,    50,    51,    96,    97,    98,
      99,   100,    83,    87,    16,    60,    12,   137,    29,    30,
      31,    32,   412,     0,    94,   106,   107,   108,   109,     3,
      14,   356,    62,   281,    74,   294,    30,   118,   119,   120,
     121,   291,    87,    64,    65,    66,    67,    83,    75,    73,
      31,    61,    11,    13,    15,    17,   104,    29,    14,     3,
      54,    55,    56,    57,    58,   118,   119,   120,   121,    34,
       3,    16,    59,    12,     5,    14,    65,    60,   126,   127,
       5,   129,   130,    13,   343,   133,     5,    88,   208,   239,
      12,    85,    13,    13,   214,   215,    14,   208,    12,   219,
      13,   221,    12,    12,   208,    13,    13,   118,   119,   120,
     121,    50,    51,   372,    29,    13,    17,    16,   268,     3,
      88,    60,   272,   243,   244,   245,   246,   247,    17,   388,
     250,   251,   252,   253,   254,   255,   256,   396,   258,   258,
     260,   261,     3,    13,   255,   256,    34,    88,    87,   230,
     261,   255,   256,    34,    29,    30,    31,    32,    13,   309,
     280,    13,   233,   234,   235,   236,    13,    13,    13,   289,
     218,   291,    37,   293,   324,    13,   296,    13,   289,   299,
     300,   229,    13,   231,     3,   289,    65,    17,   299,     3,
     310,     3,     5,     3,   242,   385,    60,   233,   234,   235,
     236,   415,   334,   335,   379,   359,   338,   339,    37,   411,
     310,   259,   332,   358,   334,   335,   369,   243,   338,   339,
     362,   332,   244,   246,   344,   245,   346,   252,   346,   247,
      37,   250,    37,    37,   251,    37,   356,   253,    37,   359,
     254,    19,   362,   118,   119,   120,   121,    -1,   380,   369,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     380,    -1,    -1,    -1,    -1,   385,    -1,   385,    -1,   401,
      -1,    -1,    -1,    -1,   406,    29,    30,    31,    32,   327,
      -1,   401,   330,    -1,    -1,    -1,   406,   419,   336,   337,
      -1,    -1,    -1,    -1,   342,   415,     3,    -1,    -1,   419,
      -1,     8,    -1,    10,   415,    12,    -1,    14,    -1,    16,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    30,    -1,    -1,    -1,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    -1,   118,   119,   120,   121,    -1,    -1,
      77,    78,    79,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      87,    -1,    89,    90,    91,    92,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,     3,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    16,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    56,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    89,
      90,    91,    92,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
       3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,
      -1,    14,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    55,    56,    57,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    89,    90,    91,    92,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,     3,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,     3,    55,
      56,    57,    58,     8,    -1,    10,    -1,    12,    -1,    14,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    87,    -1,    -1,    90,    91,    92,    93,    -1,    -1,
      96,    97,    98,    99,   100,    50,    51,    52,    53,     3,
      55,    56,    57,    58,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    87,    -1,    -1,    90,    91,    92,    -1,    -1,
      -1,    96,    97,    98,    99,   100,    50,    51,    52,    53,
      -1,    55,    56,    57,    58,     4,     5,     6,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    -1,    -1,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     8,    -1,
      10,    -1,    12,    87,    14,    -1,    90,    91,    92,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    27,    28,    -1,
      30,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    46,    -1,    -1,    -1,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    -1,
      -1,    80,    81,    82,    83,    84,    -1,    67,    68,    69,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    79,
      14,    -1,    -1,    -1,    -1,    85,    -1,    87,    -1,    89,
      90,    91,    92,    27,    28,    -1,    96,    97,    98,    99,
     100,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    56,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    89,    90,    91,    92,    -1,
      -1,    -1,    96,    97,    98,    99,   100,     8,    -1,    10,
      -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    56,    57,    58,     8,    -1,
      10,    -1,    12,    -1,    14,    15,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    87,    -1,    -1,    90,
      91,    92,    -1,    -1,    -1,    96,    97,    98,    99,   100,
      50,    51,    52,    53,    -1,    55,    56,    57,    58,     8,
      -1,    10,    -1,    12,    -1,    14,    15,    67,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    87,    -1,    -1,
      90,    91,    92,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    50,    51,    52,    53,    -1,    55,    56,    57,    58,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    87,    -1,
      -1,    90,    91,    92,    -1,    -1,    -1,    96,    97,    98,
      99,   100,    50,    51,    52,    53,    -1,    55,    56,    57,
      58,     8,    -1,    10,    -1,    12,    -1,    14,    15,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    87,
      -1,    -1,    90,    91,    92,    -1,    -1,    -1,    96,    97,
      98,    99,   100,    50,    51,    52,    53,    -1,    55,    56,
      57,    58,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      87,    -1,    -1,    90,    91,    92,    -1,    -1,    -1,    96,
      97,    98,    99,   100,    50,    51,    52,    53,    -1,    55,
      56,    57,    58,     8,    -1,    10,    -1,    12,    -1,    14,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    87,    -1,    -1,    90,    91,    92,    -1,    -1,    -1,
      96,    97,    98,    99,   100,    50,    51,    52,    53,    -1,
      55,    56,    57,    58,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    87,    -1,    -1,    90,    91,    92,    -1,    -1,
      -1,    96,    97,    98,    99,   100,    50,    51,    52,    53,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,    50,    51,
      52,    53,    -1,    -1,     4,    -1,     6,     7,     8,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
       4,    -1,     6,    -1,    96,    97,    98,    99,   100,    13,
      50,    51,    -1,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,
      84
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    30,
      35,    36,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    67,    68,    69,    77,    78,    79,    85,
      87,    89,    90,    91,    92,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   111,
     112,   113,   119,   120,   121,   124,   125,   126,   127,   129,
     130,   131,   132,   133,   135,   136,   137,   138,   139,   140,
     143,   144,   145,   148,   149,   151,   153,   155,   158,   159,
     160,   161,   162,   163,   165,   167,   169,   171,   173,   174,
     175,   176,   177,   181,   182,   183,    55,    56,    57,    58,
     127,   139,   175,    13,   139,    15,   139,    17,    55,    56,
      57,    58,   119,   122,   127,   153,    12,    12,   126,    12,
      12,    12,   127,    12,   127,   181,   183,    16,     3,   139,
       3,     3,   122,    32,   117,   127,   113,   120,   121,   129,
     145,   149,   155,   113,   120,   121,   129,   145,   149,   155,
     113,   120,   121,   129,   145,   149,   155,   113,   120,   121,
     129,   145,   149,   155,   118,   127,    12,     4,     6,     7,
       8,     9,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    50,    51,    63,    64,    65,    66,    67,    68,    69,
      71,    72,    80,    81,    82,    83,    84,   142,   150,   166,
     168,   170,   172,   180,   122,    87,   122,   178,    16,    12,
     153,    94,     0,   111,   119,    29,    34,   152,     3,     5,
     142,    14,   143,    55,    56,    57,    58,   113,   149,   127,
     156,   157,    33,    76,    62,    74,    75,    73,    63,    70,
     164,   166,   168,   170,   172,    31,    78,   176,    61,    14,
      60,    87,   141,   143,   180,   139,    11,    13,    15,    17,
     127,   143,   144,   139,   139,    38,    40,   139,   139,    93,
     134,   138,     3,   139,    34,    34,   117,     3,   124,    59,
     114,     5,    65,    60,    13,   122,    12,   127,   117,     5,
      88,   139,   154,   118,   127,   139,   145,    15,   143,   144,
       5,   139,   160,   161,   162,   163,   165,   167,   169,   171,
     173,   174,   122,   122,   181,   127,   178,    12,   179,    13,
      14,   143,    12,   146,    13,    13,    12,    12,    13,    13,
      29,   127,   138,    13,    17,   122,    16,   117,   152,   118,
     124,   146,   127,     3,    88,   178,    67,   127,     3,    17,
      13,   156,    34,   146,    88,    13,   139,    15,   123,   153,
      13,    95,   147,   126,   126,   139,   139,   126,   126,    34,
      13,   139,   124,   127,   128,   112,   115,   116,    13,    13,
     118,   152,   154,   128,   152,   158,    13,    13,   157,    13,
     124,    37,    13,    13,   140,   126,    13,     3,   116,    17,
     124,   146,    65,     3,   124,     5,   126,     3,     3,    13,
     126,   147,   152,   123,   126
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   110,   111,   111,   112,   112,   112,   113,   113,   113,
     113,   113,   113,   114,   114,   115,   115,   116,   116,   117,
     117,   118,   118,   119,   119,   120,   120,   120,   120,   120,
     121,   121,   121,   121,   121,   122,   122,   122,   123,   123,
     124,   124,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   127,   127,   128,   128,   129,
     129,   129,   129,   129,   130,   131,   132,   132,   133,   134,
     134,   135,   135,   135,   135,   135,   135,   135,   135,   136,
     136,   137,   137,   137,   137,   138,   138,   139,   139,   140,
     140,   141,   141,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     143,   143,   143,   144,   144,   144,   145,   145,   145,   145,
     145,   145,   146,   146,   147,   147,   148,   149,   149,   149,
     149,   149,   149,   150,   150,   150,   150,   150,   150,   151,
     151,   151,   151,   152,   152,   153,   153,   153,   153,   153,
     153,   154,   154,   155,   155,   155,   155,   155,   156,   156,
     157,   157,   157,   158,   158,   159,   159,   160,   160,   161,
     161,   162,   162,   163,   163,   164,   164,   165,   165,   166,
     166,   166,   166,   167,   167,   168,   168,   169,   169,   170,
     170,   171,   171,   172,   172,   172,   173,   173,   174,   174,
     175,   175,   175,   175,   175,   175,   175,   176,   176,   176,
     176,   177,   177,   178,   178,   179,   179,   180,   180,   181,
     181,   181,   181,   181,   182,   182,   182,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     6,     2,     2,
       2,     2,     2,     2,     0,     1,     0,     1,     2,     1,
       3,     1,     3,     1,     2,     7,     2,     2,     2,     2,
       4,     2,     2,     2,     2,     1,     2,     2,     2,     4,
       2,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
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
       2,     2,     3,     6,     2,     2,     2,     2,     1,     3,
       1,     2,     2,     1,     5,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     3,     1,     1,     1,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     2,     1,     2,     2,
       3,     1,     3,     1,     3,     2,     3,     1,     1,     1,
       2,     3,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     2
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
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1801 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 121: /* stmt_type  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1807 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 124: /* code_segment  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1813 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 125: /* stmt_pp  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1819 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 126: /* stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1825 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 129: /* enum_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1831 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 130: /* label_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1837 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 131: /* goto_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1843 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 132: /* case_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1849 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 133: /* switch_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1855 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 135: /* loop_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1861 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 136: /* selection_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1867 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 137: /* jump_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1873 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 138: /* exp_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1879 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 139: /* exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1885 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 140: /* binary_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1891 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 145: /* decl_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1897 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 155: /* union_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1903 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 158: /* con_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1909 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 159: /* log_or_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1915 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 160: /* log_and_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1921 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 161: /* inc_or_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1927 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 162: /* exc_or_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1933 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 163: /* and_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1939 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 165: /* eq_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1945 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 167: /* relational_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1951 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 169: /* shift_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1957 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 171: /* add_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1963 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 173: /* mul_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1969 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 174: /* cast_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1975 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 176: /* unary_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1981 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 177: /* dur_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1987 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 179: /* call_paren  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1993 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 181: /* post_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1999 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 183: /* primary_exp  */
#line 102 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 2005 "src/ast/parser.c" /* yacc.c:1258  */
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
#line 106 "utils/gwion.y" /* yacc.c:1648  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL);  }
#line 2273 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 3:
#line 107 "utils/gwion.y" /* yacc.c:1648  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast)); }
#line 2279 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 4:
#line 111 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list)); }
#line 2285 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 5:
#line 112 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def)); }
#line 2291 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 6:
#line 113 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def)); }
#line 2297 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 7:
#line 118 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_def) = new_class_def(0, (yyvsp[-4].id_list), (yyvsp[-3].type_decl), (yyvsp[-1].class_body)); }
#line 2303 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 8:
#line 119 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_static);  (yyval.class_def) = (yyvsp[0].class_def); }
#line 2309 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 9:
#line 120 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_global);  (yyval.class_def) = (yyvsp[0].class_def); }
#line 2315 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 10:
#line 121 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_private); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2321 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 11:
#line 122 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_protect); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2327 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 12:
#line 123 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].class_def), free_class_def); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2333 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 13:
#line 125 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2339 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 14:
#line 125 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = NULL; }
#line 2345 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 16:
#line 127 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = NULL; }
#line 2351 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 17:
#line 130 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = new_class_body((yyvsp[0].section), NULL); }
#line 2357 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 18:
#line 131 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = new_class_body((yyvsp[-1].section), (yyvsp[0].class_body)); }
#line 2363 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 19:
#line 135 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2369 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 20:
#line 136 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2375 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 21:
#line 140 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2381 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 22:
#line 141 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2387 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 23:
#line 145 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL);}
#line 2393 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 24:
#line 146 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list));}
#line 2399 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 25:
#line 151 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_fptr((yyvsp[-3].sym), (yyvsp[-5].type_decl), (yyvsp[-1].arg_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2405 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 26:
#line 152 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_fptr.td), ae_flag_static);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 2411 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 27:
#line 153 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_fptr.td), ae_flag_global);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 2417 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 28:
#line 154 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_fptr.td), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2423 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 29:
#line 155 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_fptr.td), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2429 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 30:
#line 160 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_type((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2435 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 31:
#line 161 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_type.td), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2441 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 32:
#line 162 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_type.td), ae_flag_global); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2447 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 33:
#line 163 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_type.td), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2453 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 34:
#line 164 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, ((yyvsp[0].stmt)->d.stmt_type.td), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2459 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 36:
#line 168 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub)); }
#line 2465 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 37:
#line 169 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub)); }
#line 2471 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 38:
#line 173 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL); }
#line 2477 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 39:
#line 174 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list)); }
#line 2483 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 40:
#line 178 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_code(NULL, get_pos(arg)); }
#line 2489 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 41:
#line 179 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_code((yyvsp[-1].stmt_list), get_pos(arg)); }
#line 2495 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 42:
#line 183 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_pp(ae_pp_comment, (yyvsp[0].sval)); }
#line 2501 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 43:
#line 184 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_pp(ae_pp_include, (yyvsp[0].sval)); }
#line 2507 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 44:
#line 185 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_pp(ae_pp_define,  (yyvsp[0].sval)); }
#line 2513 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 45:
#line 186 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_pp(ae_pp_undef,   (yyvsp[0].sval)); }
#line 2519 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 46:
#line 187 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_pp(ae_pp_ifdef,   (yyvsp[0].sval)); }
#line 2525 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 47:
#line 188 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_pp(ae_pp_ifndef,  (yyvsp[0].sval)); }
#line 2531 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 48:
#line 189 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_pp(ae_pp_else,    (yyvsp[0].sval)); }
#line 2537 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 49:
#line 190 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_pp(ae_pp_endif,   (yyvsp[0].sval)); }
#line 2543 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 50:
#line 191 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_pp(ae_pp_nl,      (yyvsp[0].sval)); }
#line 2549 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 65:
#line 212 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.sym) = insert_symbol((yyvsp[0].sval)); }
#line 2555 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 66:
#line 213 "utils/gwion.y" /* yacc.c:1648  */
    {
    char c[strlen(s_name((yyvsp[0].sym))) + strlen((yyvsp[-2].sval))];
    sprintf(c, "%s%s", (yyvsp[-2].sval), s_name((yyvsp[0].sym)));
    (yyval.sym) = insert_symbol(c);
  }
#line 2565 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 67:
#line 220 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.sym) = NULL; }
#line 2571 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 69:
#line 223 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sym), get_pos(arg)); }
#line 2577 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 70:
#line 224 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_static);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 2583 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 71:
#line 225 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_global);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 2589 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 72:
#line 226 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2595 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 73:
#line 227 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2601 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 74:
#line 231 "utils/gwion.y" /* yacc.c:1648  */
    {  (yyval.stmt) = new_stmt_jump((yyvsp[-1].sym), 1, get_pos(arg)); }
#line 2607 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 75:
#line 235 "utils/gwion.y" /* yacc.c:1648  */
    {  (yyval.stmt) = new_stmt_jump((yyvsp[-1].sym), 0, get_pos(arg)); }
#line 2613 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 76:
#line 239 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_case, (yyvsp[-1].exp), get_pos(arg)); }
#line 2619 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 77:
#line 240 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_case, (yyvsp[-1].exp), get_pos(arg)); }
#line 2625 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 78:
#line 244 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg));}
#line 2631 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 79:
#line 247 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 1; }
#line 2637 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 80:
#line 247 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 2643 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 81:
#line 251 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_while, (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2649 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 82:
#line 253 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_while, (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2655 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 83:
#line 255 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2661 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 84:
#line 257 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2667 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 85:
#line 259 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), (yyvsp[-5].ival), get_pos(arg)); }
#line 2673 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 86:
#line 261 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_until, (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2679 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 87:
#line 263 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_until, (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2685 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 88:
#line 265 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_loop((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2691 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 89:
#line 270 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(arg)); }
#line 2697 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 90:
#line 272 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(arg)); }
#line 2703 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 91:
#line 276 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_return, NULL, get_pos(arg)); }
#line 2709 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 92:
#line 277 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_return, (yyvsp[-1].exp), get_pos(arg)); }
#line 2715 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 93:
#line 278 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt(ae_stmt_break, get_pos(arg)); }
#line 2721 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 94:
#line 279 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt(ae_stmt_continue, get_pos(arg)); }
#line 2727 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 95:
#line 283 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_exp, (yyvsp[-1].exp),   get_pos(arg)); }
#line 2733 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 96:
#line 284 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_exp, NULL, get_pos(arg)); }
#line 2739 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 98:
#line 289 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = prepend_exp((yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 2745 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 100:
#line 294 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2751 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 101:
#line 297 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = NULL; }
#line 2757 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 102:
#line 297 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2763 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 103:
#line 299 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_chuck; }
#line 2769 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 104:
#line 299 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_unchuck; }
#line 2775 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 105:
#line 299 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_eq; }
#line 2781 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 106:
#line 300 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_ref; }
#line 2787 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 107:
#line 300 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_radd; }
#line 2793 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 108:
#line 301 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsub; }
#line 2799 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 109:
#line 301 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rmul; }
#line 2805 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 110:
#line 302 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rdiv; }
#line 2811 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 111:
#line 302 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rmod; }
#line 2817 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 112:
#line 303 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_trig; }
#line 2823 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 113:
#line 303 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_untrig; }
#line 2829 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 114:
#line 304 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsl; }
#line 2835 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 115:
#line 304 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsr; }
#line 2841 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 116:
#line 304 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsand; }
#line 2847 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 117:
#line 305 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsor; }
#line 2853 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 118:
#line 305 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsxor; }
#line 2859 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 119:
#line 306 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_assign; }
#line 2865 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 120:
#line 310 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = new_array_sub((yyvsp[-1].exp), get_pos(arg)); }
#line 2871 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 121:
#line 311 "utils/gwion.y" /* yacc.c:1648  */
    { if((yyvsp[-2].exp)->next){ gwion_error(arg, "invalid format for array init [...][...]..."); free_exp((yyvsp[-2].exp)); free_array_sub((yyvsp[0].array_sub)); YYERROR; } (yyval.array_sub) = prepend_array_sub((yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2877 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 122:
#line 312 "utils/gwion.y" /* yacc.c:1648  */
    { gwion_error(arg, "partially empty array init [...][]..."); free_exp((yyvsp[-3].exp)); YYERROR; }
#line 2883 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 123:
#line 316 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(arg)); }
#line 2889 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 124:
#line 317 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2895 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 125:
#line 318 "utils/gwion.y" /* yacc.c:1648  */
    { gwion_error(arg, "partially empty array init [][...]"); free_array_sub((yyvsp[-1].array_sub)); free_array_sub((yyvsp[0].array_sub)); YYERROR; }
#line 2901 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 127:
#line 323 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2907 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 128:
#line 325 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_static);  (yyval.exp) = (yyvsp[0].exp); }
#line 2913 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 129:
#line 327 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_global);  (yyval.exp) = (yyvsp[0].exp); }
#line 2919 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 130:
#line 329 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_private); (yyval.exp) = (yyvsp[0].exp); }
#line 2925 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 131:
#line 331 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_protect); (yyval.exp) = (yyvsp[0].exp); }
#line 2931 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 132:
#line 335 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = NULL; }
#line 2937 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 133:
#line 336 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = (yyvsp[0].arg_list); }
#line 2943 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 134:
#line 340 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 2949 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 135:
#line 341 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2955 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 136:
#line 344 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2961 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 137:
#line 348 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-4].type_decl), (yyvsp[-3].sym), (yyvsp[-2].arg_list), (yyvsp[0].stmt), (yyvsp[-1].ival)); }
#line 2967 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 138:
#line 350 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_static); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2973 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 139:
#line 352 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_global); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2979 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 140:
#line 354 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_private); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2985 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 141:
#line 356 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_protect); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2991 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 142:
#line 358 "utils/gwion.y" /* yacc.c:1648  */
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
#line 3006 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 150:
#line 373 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-3].type_decl), OP_SYM((yyvsp[-4].ival)), (yyvsp[-2].arg_list), (yyvsp[0].stmt), ae_flag_op); }
#line 3012 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 151:
#line 375 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-3].type_decl), OP_SYM((yyvsp[-5].ival)), (yyvsp[-2].arg_list), (yyvsp[0].stmt), ae_flag_op | ae_flag_unary); }
#line 3018 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 152:
#line 377 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def(new_type_decl(new_id_list(insert_symbol("void"), get_pos(arg)), 0,
      get_pos(arg)), insert_symbol("dtor"), NULL, (yyvsp[0].stmt), ae_flag_dtor); }
#line 3025 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 153:
#line 381 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 3031 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 154:
#line 381 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_flag_ref; }
#line 3037 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 155:
#line 384 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)), (yyvsp[0].ival), get_pos(arg)); }
#line 3043 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 156:
#line 385 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 3049 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 157:
#line 386 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)),
      (yyvsp[0].ival), get_pos(arg)); (yyval.type_decl)->types = (yyvsp[-3].type_list); }
#line 3056 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 158:
#line 388 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg));
      (yyval.type_decl)->types = (yyvsp[-5].type_list); }
#line 3063 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 159:
#line 390 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 3069 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 160:
#line 391 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].type_decl), ae_flag_const); (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 3075 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 161:
#line 395 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 3081 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 162:
#line 396 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 3087 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 163:
#line 400 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(arg));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sym); }
#line 3093 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 164:
#line 402 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 3099 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 165:
#line 404 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_global);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 3105 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 166:
#line 406 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 3111 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 167:
#line 408 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 3117 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 168:
#line 412 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL); }
#line 3123 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 169:
#line 413 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list)); }
#line 3129 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 170:
#line 417 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sym), NULL, get_pos(arg)); }
#line 3135 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 171:
#line 418 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 3141 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 172:
#line 419 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 3147 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 174:
#line 423 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_if((yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3153 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 176:
#line 426 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(arg)); }
#line 3159 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 178:
#line 429 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(arg)); }
#line 3165 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 180:
#line 432 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_sor, (yyvsp[0].exp), get_pos(arg)); }
#line 3171 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 182:
#line 435 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_sxor, (yyvsp[0].exp), get_pos(arg)); }
#line 3177 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 184:
#line 438 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_sand, (yyvsp[0].exp), get_pos(arg)); }
#line 3183 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 185:
#line 440 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_eq; }
#line 3189 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 186:
#line 440 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_ne; }
#line 3195 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 188:
#line 443 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3201 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 189:
#line 445 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_lt; }
#line 3207 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 190:
#line 445 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_gt; }
#line 3213 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 191:
#line 446 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_le; }
#line 3219 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 192:
#line 446 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_ge; }
#line 3225 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 194:
#line 449 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3231 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 195:
#line 452 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_shl;  }
#line 3237 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 196:
#line 453 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_shr; }
#line 3243 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 198:
#line 457 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3249 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 199:
#line 459 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_add; }
#line 3255 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 200:
#line 459 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_sub; }
#line 3261 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 202:
#line 462 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3267 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 203:
#line 464 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_mul; }
#line 3273 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 204:
#line 464 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_div; }
#line 3279 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 205:
#line 464 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_mod; }
#line 3285 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 207:
#line 467 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3291 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 209:
#line 470 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_cast((yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(arg)); }
#line 3297 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 210:
#line 472 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_add; }
#line 3303 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 211:
#line 472 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_sub; }
#line 3309 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 212:
#line 472 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_mul; }
#line 3315 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 213:
#line 473 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_inc; }
#line 3321 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 214:
#line 473 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_dec; }
#line 3327 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 215:
#line 474 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_not; }
#line 3333 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 216:
#line 474 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_spork; }
#line 3339 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 218:
#line 478 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_unary((yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3345 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 219:
#line 480 "utils/gwion.y" /* yacc.c:1648  */
    {
      if((yyvsp[0].type_decl)->array && !(yyvsp[0].type_decl)->array->exp) {
        free_type_decl((yyvsp[0].type_decl));
        gwion_error(arg, "can't use empty '[]' in 'new' expression");
        YYERROR;
      }
      (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), get_pos(arg));
    }
#line 3358 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 220:
#line 489 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_unary3(op_spork, (yyvsp[0].stmt), get_pos(arg)); }
#line 3364 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 222:
#line 492 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_dur((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3370 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 223:
#line 495 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = new_type_list((yyvsp[0].type_decl), NULL); }
#line 3376 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 224:
#line 496 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = new_type_list((yyvsp[-2].type_decl), (yyvsp[0].type_list)); }
#line 3382 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 225:
#line 499 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = NULL; }
#line 3388 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 226:
#line 499 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3394 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 227:
#line 501 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_inc; }
#line 3400 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 228:
#line 501 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_dec; }
#line 3406 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 230:
#line 504 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_array((yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(arg)); }
#line 3412 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 231:
#line 506 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_call((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg));
      if((yyvsp[-1].type_list))(yyval.exp)->d.exp_func.tmpl = new_tmpl_call((yyvsp[-1].type_list)); }
#line 3419 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 232:
#line 509 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_dot((yyvsp[-2].exp), (yyvsp[0].sym), get_pos(arg)); }
#line 3425 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 233:
#line 511 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_post((yyvsp[-1].exp), (yyvsp[0].ival), get_pos(arg)); }
#line 3431 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 234:
#line 514 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_complex; }
#line 3437 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 235:
#line 515 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_polar;   }
#line 3443 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 236:
#line 516 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_vec;     }
#line 3449 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 237:
#line 519 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_id(     (yyvsp[0].sym), get_pos(arg)); }
#line 3455 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 238:
#line 520 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_int(    (yyvsp[0].ival), get_pos(arg)); }
#line 3461 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 239:
#line 521 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_float(  (yyvsp[0].fval), get_pos(arg)); }
#line 3467 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 240:
#line 522 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_string( (yyvsp[0].sval), get_pos(arg)); }
#line 3473 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 241:
#line 523 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_char(   (yyvsp[0].sval), get_pos(arg)); }
#line 3479 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 242:
#line 524 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3485 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 243:
#line 525 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3491 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 244:
#line 526 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_vec((yyvsp[-2].ival), (yyvsp[-1].exp), get_pos(arg)); }
#line 3497 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 245:
#line 527 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_hack(   (yyvsp[-1].exp), get_pos(arg)); }
#line 3503 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 246:
#line 528 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) =                      (yyvsp[-1].exp);                }
#line 3509 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 247:
#line 529 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_nil(        get_pos(arg)); }
#line 3515 "src/ast/parser.c" /* yacc.c:1648  */
    break;


#line 3519 "src/ast/parser.c" /* yacc.c:1648  */
      default: break;
    }
  /* User semantic actions somemul alter yychar, and that requires
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
#line 531 "utils/gwion.y" /* yacc.c:1907  */

