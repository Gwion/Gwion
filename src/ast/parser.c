/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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
#define YYBISON_VERSION "3.0.4"

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

#include "absyn.h"
#include "scanner.h"
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
int gwion_error(Scanner*, const char*);
int gwion_lex(void*, Scanner*);
ANN int get_pos(const Scanner*);
m_str op2str(const Operator op);

#line 93 "src/ast/parser.c" /* yacc.c:339  */

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
    PUBLIC = 311,
    PRIVATE = 312,
    EXTENDS = 313,
    DOT = 314,
    COLONCOLON = 315,
    AND = 316,
    EQ = 317,
    GE = 318,
    GT = 319,
    LE = 320,
    LT = 321,
    MINUS = 322,
    PLUS = 323,
    NEQ = 324,
    SHIFT_LEFT = 325,
    SHIFT_RIGHT = 326,
    S_AND = 327,
    S_OR = 328,
    S_XOR = 329,
    OR = 330,
    AST_DTOR = 331,
    OPERATOR = 332,
    TYPEDEF = 333,
    RSL = 334,
    RSR = 335,
    RSAND = 336,
    RSOR = 337,
    RSXOR = 338,
    TEMPLATE = 339,
    NOELSE = 340,
    LTB = 341,
    GTB = 342,
    VARARG = 343,
    UNION = 344,
    ATPAREN = 345,
    TYPEOF = 346,
    CONST = 347,
    AUTO = 348,
    AUTO_PTR = 349,
    INLINE = 350,
    NUM = 351,
    FLOAT = 352,
    ID = 353,
    STRING_LIT = 354,
    CHAR_LIT = 355
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
#define PUBLIC 311
#define PRIVATE 312
#define EXTENDS 313
#define DOT 314
#define COLONCOLON 315
#define AND 316
#define EQ 317
#define GE 318
#define GT 319
#define LE 320
#define LT 321
#define MINUS 322
#define PLUS 323
#define NEQ 324
#define SHIFT_LEFT 325
#define SHIFT_RIGHT 326
#define S_AND 327
#define S_OR 328
#define S_XOR 329
#define OR 330
#define AST_DTOR 331
#define OPERATOR 332
#define TYPEDEF 333
#define RSL 334
#define RSR 335
#define RSAND 336
#define RSOR 337
#define RSXOR 338
#define TEMPLATE 339
#define NOELSE 340
#define LTB 341
#define GTB 342
#define VARARG 343
#define UNION 344
#define ATPAREN 345
#define TYPEOF 346
#define CONST 347
#define AUTO 348
#define AUTO_PTR 349
#define INLINE 350
#define NUM 351
#define FLOAT 352
#define ID 353
#define STRING_LIT 354
#define CHAR_LIT 355

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 26 "utils/gwion.y" /* yacc.c:355  */

  char* sval;
  int ival;
  m_float fval;
  Symbol sym;
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

#line 358 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (Scanner* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 374 "src/ast/parser.c" /* yacc.c:358  */

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
#define YYFINAL  194
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1859

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  101
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  73
/* YYNRULES -- Number of rules.  */
#define YYNRULES  218
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  395

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   355

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
      95,    96,    97,    98,    99,   100
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   102,   102,   103,   107,   108,   109,   113,   115,   116,
     119,   119,   121,   121,   124,   125,   129,   130,   134,   135,
     139,   140,   144,   145,   149,   150,   154,   155,   161,   165,
     166,   167,   171,   172,   176,   177,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   196,
     198,   198,   201,   202,   206,   210,   214,   215,   219,   223,
     225,   227,   229,   231,   233,   235,   237,   239,   244,   246,
     251,   252,   253,   254,   258,   259,   263,   264,   268,   269,
     272,   272,   274,   274,   274,   275,   275,   276,   276,   277,
     277,   278,   278,   279,   279,   279,   280,   280,   281,   285,
     286,   287,   291,   292,   293,   297,   298,   299,   301,   306,
     307,   310,   313,   315,   317,   319,   332,   332,   332,   332,
     332,   332,   334,   335,   337,   339,   344,   344,   347,   348,
     349,   351,   353,   354,   358,   359,   363,   364,   366,   371,
     372,   376,   377,   378,   381,   381,   384,   384,   387,   387,
     390,   390,   393,   393,   396,   396,   399,   399,   401,   401,
     404,   404,   405,   405,   407,   407,   411,   412,   415,   415,
     418,   418,   420,   420,   423,   423,   423,   425,   425,   428,
     428,   431,   431,   431,   432,   432,   433,   433,   436,   436,
     438,   447,   450,   450,   454,   455,   458,   458,   460,   460,
     462,   462,   464,   467,   469,   473,   474,   475,   478,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488
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
  "STATIC", "PUBLIC", "PRIVATE", "EXTENDS", "DOT", "COLONCOLON", "AND",
  "EQ", "GE", "GT", "LE", "LT", "MINUS", "PLUS", "NEQ", "SHIFT_LEFT",
  "SHIFT_RIGHT", "S_AND", "S_OR", "S_XOR", "OR", "AST_DTOR", "OPERATOR",
  "TYPEDEF", "RSL", "RSR", "RSAND", "RSOR", "RSXOR", "TEMPLATE", "NOELSE",
  "LTB", "GTB", "VARARG", "UNION", "ATPAREN", "TYPEOF", "CONST", "AUTO",
  "AUTO_PTR", "INLINE", "NUM", "FLOAT", "ID", "STRING_LIT", "CHAR_LIT",
  "$accept", "ast", "section", "class_def", "class_ext", "class_body",
  "class_body2", "id_list", "id_dot", "stmt_list", "static_decl",
  "function_decl", "func_ptr", "stmt_typedef", "type_decl2", "arg_list",
  "code_segment", "stmt", "id", "opt_id", "enum_stmt", "label_stmt",
  "goto_stmt", "case_stmt", "switch_stmt", "loop_stmt", "selection_stmt",
  "jump_stmt", "exp_stmt", "exp", "binary_exp", "template", "op",
  "array_exp", "array_empty", "decl_exp", "func_args", "decl_template",
  "func_def_base", "op_op", "func_def", "atsym", "type_decl", "decl_list",
  "union_stmt", "var_decl_list", "var_decl", "con_exp", "log_or_exp",
  "log_and_exp", "inc_or_exp", "exc_or_exp", "and_exp", "eq_op", "eq_exp",
  "rel_op", "relational_exp", "shift_op", "shift_exp", "add_op", "add_exp",
  "mul_op", "mul_exp", "cast_exp", "unary_op", "unary_exp", "dur_exp",
  "type_list", "call_paren", "post_op", "post_exp", "vec_type",
  "primary_exp", YY_NULLPTR
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
     355
};
# endif

#define YYPACT_NINF -267

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-267)))

#define YYTABLE_NINF -127

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     445,  -267,  -267,  1554,  1092,  1158,   543,  -267,  -267,  -267,
    -267,    31,    39,   610,    45,    52,    54,   -30,    59,    69,
      58,   752,    73,    96,  -267,  -267,    89,    76,   -30,   959,
      34,   893,   -30,  -267,  -267,    74,  1705,    89,    19,    89,
    -267,    99,  -267,   108,    89,   291,  -267,  -267,  -267,  -267,
    -267,   124,   445,  -267,  -267,    72,  -267,  -267,  -267,   610,
       2,  -267,  -267,  -267,  -267,  -267,  -267,  -267,  -267,  -267,
     135,   318,  -267,   138,  -267,   221,  -267,  -267,   -30,  -267,
    -267,    -3,    93,    87,    88,    98,    -7,   128,    66,    83,
     122,   147,  1620,  -267,   123,    63,  1554,  -267,  1554,  1554,
      -5,   173,  1654,  -267,   175,  -267,   181,  -267,  1026,   182,
    1554,  1554,     9,  1554,  1554,   677,   198,  1554,  -267,    75,
     172,   -30,  -267,   205,  -267,  -267,  -267,   180,   213,   196,
     171,   227,  1224,    89,  -267,  -267,  -267,  -267,    34,  1224,
    -267,  -267,   291,  -267,  -267,   170,   177,   226,  -267,  -267,
    -267,  -267,  -267,  -267,  -267,  -267,  -267,  -267,  -267,  -267,
    -267,  -267,  -267,  -267,  -267,  -267,  -267,  -267,  -267,  -267,
    -267,  -267,  -267,  -267,  -267,  -267,  -267,  -267,  -267,    89,
    -267,  -267,  -267,  -267,  -267,     3,   -30,   235,   154,  1554,
     -30,  -267,   291,  -267,  -267,  -267,  -267,    89,  -267,  -267,
    -267,  -267,  -267,  1554,  1554,  1290,  -267,  -267,  -267,   213,
    -267,   238,  1554,  1654,  1654,  1654,  1654,  1654,  -267,  -267,
    1654,  1654,  1654,  1654,  1654,    89,    89,  -267,    69,  1554,
     -30,    89,   232,  -267,  -267,   236,  -267,  -267,   231,  -267,
     237,   241,   245,   249,   251,   252,   -30,   -30,   818,  -267,
     254,  -267,  -267,   255,  -267,  -267,   138,  -267,    89,   246,
     -30,   256,   180,   -30,   196,   259,   -30,   270,   189,    89,
     -25,   276,   264,   269,   -30,  -267,  -267,  -267,  -267,   138,
     -30,   250,    93,    87,    88,    98,    -7,   128,    66,    83,
     122,   147,  -267,   259,    63,  -267,   199,  1356,  -267,  -267,
    1422,  -267,   610,   610,  1554,  1554,   610,   610,   253,   260,
    1488,   196,   -30,  -267,   445,  -267,  -267,  -267,  -267,    25,
     196,   272,  -267,  -267,  -267,   -30,   180,  1554,   -30,   180,
     259,  -267,  1654,   196,  -267,  -267,   275,  -267,   262,  -267,
     277,   280,  -267,  -267,  1554,  1554,   610,   284,  -267,  -267,
     286,   445,   266,  -267,  -267,   287,   -30,  -267,   259,   240,
    -267,  -267,   298,  -267,   196,  -267,  -267,  -267,   610,   305,
     307,   874,  1776,  -267,   610,  -267,  -267,  -267,  -267,   306,
    -267,   180,  -267,  -267,  -267,  -267,  -267,   610,   610,  -267,
      89,  -267,  -267,  -267,  -267
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    75,   183,     0,     0,     0,     0,   206,   205,    24,
     186,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,   185,     0,     0,     0,     0,
       0,     0,     0,   182,   181,     0,     0,     0,     0,     0,
      25,     0,   207,     0,     0,     0,   209,   210,    49,   211,
     212,     0,     2,     6,     4,    23,    46,    47,    39,    20,
     208,    44,    40,    41,    43,    42,    37,    38,    45,    36,
       0,    76,   213,   214,    78,     0,   122,     5,     0,    48,
     105,   144,   146,   148,   150,   152,   154,   158,   164,   168,
     172,   177,     0,   179,   188,   192,     0,   200,     0,     0,
     208,     0,     0,   218,     0,   102,     0,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   208,    80,
     200,     0,    70,     0,    72,    73,   190,   126,    29,   187,
      11,    16,     0,     0,    27,   107,   137,     8,     0,     0,
      53,   108,     0,   113,   138,     0,    18,     0,    82,    98,
     175,   174,   176,    86,    87,    88,    89,    90,    85,    83,
      91,    92,   198,   199,    84,   163,   161,   162,   160,   171,
     170,   166,   167,    93,    94,    95,    96,    97,   116,     0,
     119,   117,   121,   120,   118,     0,     0,   194,     0,     0,
       0,   133,     0,   114,     1,     3,    22,     0,    21,   127,
      54,   128,    74,     0,     0,     0,   104,     9,   115,   141,
     106,   139,     0,     0,     0,     0,     0,     0,   156,   157,
       0,     0,     0,     0,     0,     0,     0,   189,     0,     0,
       0,     0,     0,   201,   204,     0,   216,   217,    99,    35,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
       0,    57,    56,     0,    71,    31,    30,   191,     0,     0,
       0,     0,   126,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    77,    79,   103,   142,   143,
       0,     0,   147,   149,   151,   153,   155,   159,   165,   169,
     173,   178,   180,     0,   193,   203,     0,     0,   202,   215,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    10,    13,    17,   129,    19,   125,     0,
       0,     0,    28,   111,   195,     0,   126,   134,    50,   126,
       0,   140,     0,     0,    81,   196,     0,   101,    68,    59,
       0,     0,    65,    67,     0,     0,     0,     0,    58,    51,
       0,    14,     0,    12,   109,     0,     0,   123,     0,     0,
     130,   135,     0,   132,     0,   145,   124,   197,     0,     0,
       0,     0,     0,    61,     0,    52,    15,     7,   110,    32,
      26,   126,   136,   112,    69,    60,    66,     0,     0,    62,
       0,   131,    63,    64,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -267,   261,  -266,   -19,  -267,  -267,   -34,  -117,  -181,    48,
    -267,  -267,   289,  -267,   -23,   -70,  -122,   -11,     0,    -1,
      -2,  -267,  -267,  -267,  -267,  -267,  -267,  -267,  -107,   142,
    -172,  -267,   290,   -61,  -106,    -4,  -260,    15,   -10,  -267,
    -267,  -242,   -21,     6,    32,    49,   -26,    17,  -267,   121,
     136,   140,   137,  -267,   141,   323,   144,   324,   145,   325,
     143,   326,   149,   146,     1,   -52,  -267,  -189,  -267,   331,
      -9,  -267,   354
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    51,    52,    53,   259,   352,   353,   130,   145,    54,
     197,    55,    56,    57,   187,   355,    58,    59,   100,   350,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,   232,   204,    72,    73,    74,   320,    75,    76,   179,
      77,   201,    78,   272,    79,   210,   211,    80,    81,    82,
      83,    84,    85,   220,    86,   221,    87,   222,    88,   223,
      89,   224,    90,    91,   102,    93,    94,   188,   298,   234,
      95,    96,    97
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    92,   112,   126,   253,   128,    60,   257,   248,   273,
     119,   137,   206,    60,   185,   266,   128,   116,   128,   118,
     316,   143,   256,   191,   199,   135,   127,   141,   131,   140,
     212,   199,   146,   333,   233,   193,   200,   127,   354,   127,
     227,   325,   296,   110,   127,   138,   142,   242,   351,   243,
     227,   111,    60,    92,   109,   218,   207,   113,   233,    60,
     142,   136,   219,   144,   114,   208,   115,   255,    48,   268,
     364,   117,   213,    48,   121,   -80,   124,   229,   209,     3,
     324,     4,   317,     5,   360,   351,   147,   363,    28,   229,
      30,    32,   118,  -126,   135,   141,     7,     8,   380,   125,
    -126,    48,   118,   279,   141,   186,   140,   198,   129,   251,
     261,    39,   128,   162,   163,   189,    43,    44,    38,   207,
     190,   131,   230,    48,   194,   162,   163,   196,   141,   150,
     151,   152,   208,   127,   230,   135,   171,   172,   202,   391,
     144,   310,   318,   315,   359,   101,   104,   106,   278,   231,
     169,   170,   205,   138,   214,    32,   265,   142,   128,    42,
     215,   231,   216,   123,   144,    46,    47,    48,    49,    50,
     217,   136,   371,   372,   274,    39,   128,   301,   225,   127,
      43,    44,   143,   228,   236,   267,   131,    48,   237,   348,
     146,   165,   166,   167,   168,   206,   238,   127,   357,   239,
     276,   249,   292,   293,   128,   128,   252,   142,   254,   199,
     128,   366,     6,   118,   118,   118,   118,   118,   206,   294,
     118,   118,   118,   118,   118,   127,   127,     5,   118,   258,
     295,   127,   260,   233,   262,   313,   263,   128,   235,   264,
     269,   270,   383,   280,   297,   300,   308,   309,   128,   299,
     302,     9,   240,   241,   303,   244,   245,   304,   127,   250,
     131,   305,   314,   146,   306,   307,   321,   311,   266,   127,
     326,   319,   312,   322,   330,    28,   323,    30,   192,   327,
     209,   328,   329,   377,   332,   358,   334,   344,   367,   375,
     369,   338,   339,   370,   345,   342,   343,   374,   356,   368,
     378,   382,    60,    60,   381,    38,    60,    60,   385,    40,
     386,   390,   349,   195,    60,    92,    45,   376,   134,   127,
     394,     9,   148,   203,   149,   146,   178,   362,   349,   331,
     379,   271,   118,   361,   282,   373,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   275,    60,   106,   192,   365,
     283,    60,    92,   285,   281,   284,   209,   384,   286,   180,
     181,   182,   183,   389,   287,   289,   288,   184,    60,   356,
     291,   106,   290,   120,    60,    38,   392,   393,     0,    40,
     164,     0,     0,     0,     0,     0,    45,    60,    60,     0,
     127,     0,     0,     0,     0,     0,     0,   173,   174,   175,
     176,   177,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   336,
       0,     0,   106,     0,     0,     0,   340,   341,     1,     0,
       0,     0,   347,     2,     0,     3,     0,     4,     0,     5,
       0,     6,     0,     0,     0,     0,     0,     0,     0,   271,
       0,     0,     7,     8,     0,     9,     0,     0,     0,     0,
      10,    11,     0,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,     0,     0,     0,     0,     0,
       0,    32,    33,    34,     0,     0,     0,     0,     0,     0,
       0,    35,    36,    37,     0,     0,     0,     0,     0,    38,
       0,    39,     0,    40,    41,    42,    43,    44,     0,     0,
      45,    46,    47,    48,    49,    50,     1,     0,     0,     0,
       0,     2,     0,     3,     0,     4,     0,     5,     0,     6,
     107,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,    10,    11,
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    29,     0,
     108,     0,     0,     0,     0,     0,     0,     0,     0,    32,
      33,    34,     0,     1,     0,     0,     0,     0,     2,     0,
       3,    37,     4,     0,     5,     0,     6,     0,     0,    39,
       0,     0,    41,    42,    43,    44,     0,     7,     8,    46,
      47,    48,    49,    50,     0,    10,    11,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    29,     0,   108,     0,     0,
       0,     0,     0,     0,     0,     0,    32,    33,    34,     0,
       1,     0,     0,     0,     0,     2,     0,     3,    37,     4,
       0,     5,     0,     0,     0,     0,    39,     0,     0,    41,
      42,    43,    44,     0,     7,     8,    46,    47,    48,    49,
      50,     0,    10,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24,    25,    26,
      27,     0,    98,     0,    99,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   122,     0,     0,     0,     0,
       2,     0,     3,    39,     4,     0,     5,    42,    43,    44,
     246,   247,     0,    46,    47,    48,    49,    50,     0,     7,
       8,     0,     0,     0,     0,     0,     0,    10,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,    25,    26,    27,     0,    98,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,    32,    33,
      34,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,     0,     5,     0,     0,     0,     0,     0,    39,     0,
       0,     0,    42,    43,    44,     7,     8,     0,    46,    47,
      48,    49,    50,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,    25,
      26,    27,     0,    98,     0,    99,     0,     0,   148,     0,
     149,     0,     0,     0,    32,    33,    34,   387,     0,     0,
       0,     0,   153,   154,   155,   156,   157,   158,   159,   160,
     161,     2,     0,     3,    39,     4,     0,     5,    42,    43,
      44,     0,     0,     0,    46,    47,    48,    49,    50,     0,
       7,     8,     0,     9,     0,     0,     0,     0,    10,     0,
       0,     0,     0,     0,     0,     0,   164,     0,     0,    20,
       0,     0,     0,    24,    25,    26,    27,     0,   139,     0,
      31,     0,     0,   173,   174,   175,   176,   177,     0,    32,
      33,    34,     0,     0,     0,     0,     0,     2,     0,     3,
       0,     4,     0,     5,     0,     0,     0,    38,     0,    39,
       0,    40,    41,    42,    43,    44,     7,     8,    45,    46,
      47,    48,    49,    50,    10,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
      25,    26,    27,     0,    29,     0,   132,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,     0,     0,
       0,     0,     0,     0,     2,     0,     3,   133,     4,     0,
       5,     0,     0,     0,     0,    39,     0,     0,    41,    42,
      43,    44,     0,     7,     8,    46,    47,    48,    49,    50,
       0,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,     0,     0,     0,    24,    25,    26,    27,
       0,   139,     0,   108,     0,     0,     0,     0,     0,     0,
       0,     0,    32,    33,    34,     0,     0,     0,     0,     0,
       2,     0,     3,     0,     4,   103,     5,     0,     0,     0,
       0,     0,    39,     0,     0,    41,    42,    43,    44,     7,
       8,     0,    46,    47,    48,    49,    50,    10,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,    25,    26,    27,     0,    98,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,    32,    33,
      34,     0,     0,     0,     0,     0,     2,     0,     3,     0,
       4,     0,     5,   105,     0,     0,     0,     0,    39,     0,
       0,     0,    42,    43,    44,     7,     8,     0,    46,    47,
      48,    49,    50,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,    25,
      26,    27,     0,    98,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,    32,    33,    34,     0,     0,     0,
       0,     0,     2,     0,     3,     0,     4,     0,     5,     0,
       0,     0,     0,     0,    39,     0,     0,     0,    42,    43,
      44,     7,     8,     0,    46,    47,    48,    49,    50,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    25,    26,    27,     0,   139,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
      32,    33,    34,     0,     0,     0,     0,     0,     2,     0,
       3,     0,     4,     0,     5,   277,     0,     0,     0,     0,
      39,     0,     0,    41,    42,    43,    44,     7,     8,     0,
      46,    47,    48,    49,    50,    10,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    25,    26,    27,     0,    98,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     2,     0,     3,     0,     4,   335,
       5,     0,     0,     0,     0,     0,    39,     0,     0,     0,
      42,    43,    44,     7,     8,     0,    46,    47,    48,    49,
      50,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    98,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,    32,    33,    34,     0,     0,     0,     0,     0,
       2,     0,     3,     0,     4,     0,     5,   337,     0,     0,
       0,     0,    39,     0,     0,     0,    42,    43,    44,     7,
       8,     0,    46,    47,    48,    49,    50,    10,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,    25,    26,    27,     0,    98,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,    32,    33,
      34,     0,     0,     0,     0,     0,     2,     0,     3,     0,
       4,   346,     5,     0,     0,     0,     0,     0,    39,     0,
       0,     0,    42,    43,    44,     7,     8,     0,    46,    47,
      48,    49,    50,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,    25,
      26,    27,     0,    98,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,    32,    33,    34,     0,     0,     0,
       0,     0,     2,     0,     3,     0,     4,     0,     5,     0,
       0,     0,     0,     0,    39,     0,     0,     0,    42,    43,
      44,     7,     8,     0,    46,    47,    48,    49,    50,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    25,    26,    27,     0,    98,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
      32,    33,    34,     0,     0,     0,     0,     0,     2,     0,
       3,     0,     4,     0,     5,     0,     0,     0,     0,     0,
      39,     0,     0,     0,    42,    43,    44,     7,     8,     0,
      46,    47,    48,    49,    50,    10,     0,     0,     0,     0,
       0,     0,     2,     0,     3,     0,     4,     0,     5,     0,
      24,    25,    26,    27,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,    33,    34,    10,
       0,     0,     0,     0,     0,     0,     0,   226,     0,     0,
       0,     0,     0,     0,    24,    25,    26,    27,     0,   148,
      42,   149,   150,   151,   152,     0,    46,    47,    48,    49,
      50,    33,    34,   153,   154,   155,   156,   157,   158,   159,
     160,   161,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    42,     0,     0,     0,     0,     0,
      46,    47,    48,    49,    50,   162,   163,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   164,   165,   166,
     167,   168,   169,   170,     0,   171,   172,     0,     0,     0,
     148,     0,   149,     0,   173,   174,   175,   176,   177,   388,
       0,     0,     0,     0,   153,   154,   155,   156,   157,   158,
     159,   160,   161,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   164,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   173,   174,   175,   176,   177
};

static const yytype_int16 yycheck[] =
{
       0,     0,    13,    26,   121,    26,     6,   129,   115,   190,
      19,    30,    73,    13,    37,    12,    37,    17,    39,    19,
     262,    31,   128,    44,    29,    29,    26,    31,    28,    31,
      33,    29,    32,   293,    95,    45,    34,    37,    13,    39,
      92,    66,   231,    12,    44,    30,    31,    38,   314,    40,
     102,    12,    52,    52,     6,    62,    75,    12,   119,    59,
      45,    29,    69,    31,    12,    75,    12,   128,    98,   186,
     330,    12,    75,    98,    16,    12,     3,    14,    78,    10,
     269,    12,   263,    14,   326,   351,    12,   329,    54,    14,
      56,    66,    92,    98,    98,    99,    27,    28,   358,     3,
      98,    98,   102,   209,   108,    86,   108,    59,    32,    34,
     133,    86,   133,    50,    51,    16,    91,    92,    84,   138,
      12,   121,    59,    98,     0,    50,    51,    55,   132,     7,
       8,     9,   142,   133,    59,   139,    70,    71,     3,   381,
     108,   248,   264,   260,   325,     3,     4,     5,   209,    86,
      67,    68,    14,   138,    61,    66,   179,   142,   179,    90,
      73,    86,    74,    21,   132,    96,    97,    98,    99,   100,
      72,   139,   344,   345,   197,    86,   197,   238,    31,   179,
      91,    92,   192,    60,    11,   185,   186,    98,    13,   311,
     190,    63,    64,    65,    66,   256,    15,   197,   320,    17,
     204,     3,   225,   226,   225,   226,    34,   192,     3,    29,
     231,   333,    16,   213,   214,   215,   216,   217,   279,   228,
     220,   221,   222,   223,   224,   225,   226,    14,   228,    58,
     230,   231,     5,   294,    64,   258,    59,   258,    96,    13,
       5,    87,   364,     5,    12,    14,   246,   247,   269,    13,
      13,    30,   110,   111,    13,   113,   114,    12,   258,   117,
     260,    12,    16,   263,    13,    13,   266,    13,    12,   269,
     270,    12,    17,     3,   274,    54,    87,    56,    57,     3,
     280,    17,    13,    17,    34,    13,    87,    34,    13,     3,
      13,   302,   303,    13,    34,   306,   307,    13,   319,    37,
      13,     3,   302,   303,    64,    84,   306,   307,     3,    88,
       3,     5,   312,    52,   314,   314,    95,   351,    29,   319,
     390,    30,     4,     5,     6,   325,    36,   328,   328,   280,
     356,   189,   332,   327,   213,   346,    18,    19,    20,    21,
      22,    23,    24,    25,    26,   203,   346,   205,    57,   332,
     214,   351,   351,   216,   212,   215,   356,   368,   217,    36,
      36,    36,    36,   374,   220,   222,   221,    36,   368,   390,
     224,   229,   223,    19,   374,    84,   387,   388,    -1,    88,
      62,    -1,    -1,    -1,    -1,    -1,    95,   387,   388,    -1,
     390,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   297,
      -1,    -1,   300,    -1,    -1,    -1,   304,   305,     3,    -1,
      -1,    -1,   310,     8,    -1,    10,    -1,    12,    -1,    14,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   327,
      -1,    -1,    27,    28,    -1,    30,    -1,    -1,    -1,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    86,    -1,    88,    89,    90,    91,    92,    -1,    -1,
      95,    96,    97,    98,    99,   100,     3,    -1,    -1,    -1,
      -1,     8,    -1,    10,    -1,    12,    -1,    14,    -1,    16,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      67,    68,    -1,     3,    -1,    -1,    -1,    -1,     8,    -1,
      10,    78,    12,    -1,    14,    -1,    16,    -1,    -1,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    27,    28,    96,
      97,    98,    99,   100,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    -1,
       3,    -1,    -1,    -1,    -1,     8,    -1,    10,    78,    12,
      -1,    14,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    27,    28,    96,    97,    98,    99,
     100,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,
      53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    86,    12,    -1,    14,    90,    91,    92,
      93,    94,    -1,    96,    97,    98,    99,   100,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    90,    91,    92,    27,    28,    -1,    96,    97,
      98,    99,   100,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,     4,    -1,
       6,    -1,    -1,    -1,    66,    67,    68,    13,    -1,    -1,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     8,    -1,    10,    86,    12,    -1,    14,    90,    91,
      92,    -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,
      27,    28,    -1,    30,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,
      57,    -1,    -1,    79,    80,    81,    82,    83,    -1,    66,
      67,    68,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    -1,    -1,    84,    -1,    86,
      -1,    88,    89,    90,    91,    92,    27,    28,    95,    96,
      97,    98,    99,   100,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,     8,    -1,    10,    78,    12,    -1,
      14,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,
      91,    92,    -1,    27,    28,    96,    97,    98,    99,   100,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    89,    90,    91,    92,    27,
      28,    -1,    96,    97,    98,    99,   100,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    -1,    14,    15,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    90,    91,    92,    27,    28,    -1,    96,    97,
      98,    99,   100,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    91,
      92,    27,    28,    -1,    96,    97,    98,    99,   100,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    12,    -1,    14,    15,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    89,    90,    91,    92,    27,    28,    -1,
      96,    97,    98,    99,   100,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      90,    91,    92,    27,    28,    -1,    96,    97,    98,    99,
     100,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    -1,    14,    15,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    90,    91,    92,    27,
      28,    -1,    96,    97,    98,    99,   100,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    90,    91,    92,    27,    28,    -1,    96,    97,
      98,    99,   100,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    91,
      92,    27,    28,    -1,    96,    97,    98,    99,   100,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    90,    91,    92,    27,    28,    -1,
      96,    97,    98,    99,   100,    35,    -1,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    67,    68,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,     4,
      90,     6,     7,     8,     9,    -1,    96,    97,    98,    99,
     100,    67,    68,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,    50,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
       4,    -1,     6,    -1,    79,    80,    81,    82,    83,    13,
      -1,    -1,    -1,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    30,
      35,    36,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    66,    67,    68,    76,    77,    78,    84,    86,
      88,    89,    90,    91,    92,    95,    96,    97,    98,    99,
     100,   102,   103,   104,   110,   112,   113,   114,   117,   118,
     119,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   134,   135,   136,   138,   139,   141,   143,   145,
     148,   149,   150,   151,   152,   153,   155,   157,   159,   161,
     163,   164,   165,   166,   167,   171,   172,   173,    55,    57,
     119,   130,   165,    13,   130,    15,   130,    17,    57,   110,
      12,    12,   118,    12,    12,    12,   119,    12,   119,   171,
     173,    16,     3,   130,     3,     3,   115,   119,   143,    32,
     108,   119,    57,    78,   113,   136,   145,   104,   138,    55,
     121,   136,   138,   139,   145,   109,   119,    12,     4,     6,
       7,     8,     9,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    50,    51,    62,    63,    64,    65,    66,    67,
      68,    70,    71,    79,    80,    81,    82,    83,   133,   140,
     156,   158,   160,   162,   170,   115,    86,   115,   168,    16,
      12,   143,    57,   139,     0,   102,    55,   111,   110,    29,
      34,   142,     3,     5,   133,    14,   134,   104,   139,   119,
     146,   147,    33,    75,    61,    73,    74,    72,    62,    69,
     154,   156,   158,   160,   162,    31,    77,   166,    60,    14,
      59,    86,   132,   134,   170,   130,    11,    13,    15,    17,
     130,   130,    38,    40,   130,   130,    93,    94,   129,     3,
     130,    34,    34,   108,     3,   134,   135,   117,    58,   105,
       5,   115,    64,    59,    13,   115,    12,   119,   108,     5,
      87,   130,   144,   109,   115,   130,   136,    15,   134,   135,
       5,   130,   150,   151,   152,   153,   155,   157,   159,   161,
     163,   164,   115,   115,   171,   119,   168,    12,   169,    13,
      14,   134,    13,    13,    12,    12,    13,    13,   119,   119,
     129,    13,    17,   115,    16,   108,   142,   109,   117,    12,
     137,   119,     3,    87,   168,    66,   119,     3,    17,    13,
     119,   146,    34,   137,    87,    13,   130,    15,   118,   118,
     130,   130,   118,   118,    34,    34,    13,   130,   117,   119,
     120,   103,   106,   107,    13,   116,   143,   117,    13,   109,
     142,   144,   120,   142,   137,   148,   117,    13,    37,    13,
      13,   131,   131,   118,    13,     3,   107,    17,    13,   147,
     137,    64,     3,   117,   118,     3,     3,    13,    13,   118,
       5,   142,   118,   118,   116
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   101,   102,   102,   103,   103,   103,   104,   104,   104,
     105,   105,   106,   106,   107,   107,   108,   108,   109,   109,
     110,   110,   111,   111,   112,   112,   113,   113,   114,   115,
     115,   115,   116,   116,   117,   117,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   119,
     120,   120,   121,   121,   122,   123,   124,   124,   125,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   127,   127,
     128,   128,   128,   128,   129,   129,   130,   130,   131,   131,
     132,   132,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   134,
     134,   134,   135,   135,   135,   136,   136,   136,   136,   137,
     137,   138,   139,   139,   139,   139,   140,   140,   140,   140,
     140,   140,   141,   141,   141,   141,   142,   142,   143,   143,
     143,   143,   143,   143,   144,   144,   145,   145,   145,   146,
     146,   147,   147,   147,   148,   148,   149,   149,   150,   150,
     151,   151,   152,   152,   153,   153,   154,   154,   155,   155,
     156,   156,   156,   156,   157,   157,   158,   158,   159,   159,
     160,   160,   161,   161,   162,   162,   162,   163,   163,   164,
     164,   165,   165,   165,   165,   165,   165,   165,   166,   166,
     166,   166,   167,   167,   168,   168,   169,   169,   170,   170,
     171,   171,   171,   171,   171,   172,   172,   172,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     6,     2,     2,
       2,     0,     1,     0,     1,     2,     1,     3,     1,     3,
       1,     2,     1,     0,     1,     1,     6,     2,     4,     1,
       2,     2,     2,     4,     2,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     6,     2,     2,     3,     3,     3,     5,     5,
       7,     6,     7,     8,     8,     5,     7,     5,     5,     7,
       2,     3,     2,     2,     2,     1,     1,     3,     1,     3,
       0,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       4,     5,     2,     3,     2,     1,     2,     2,     2,     2,
       3,     4,     6,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     5,     5,     4,     0,     1,     2,     4,
       5,     7,     5,     2,     2,     3,     6,     2,     2,     1,
       3,     1,     2,     2,     1,     5,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     3,     1,     1,     1,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     2,     1,     2,
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
          case 113: /* func_ptr  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1798 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 114: /* stmt_typedef  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1804 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 117: /* code_segment  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1810 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 118: /* stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1816 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 121: /* enum_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1822 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 122: /* label_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1828 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 123: /* goto_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1834 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 124: /* case_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1840 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 125: /* switch_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1846 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 126: /* loop_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1852 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 127: /* selection_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1858 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 128: /* jump_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1864 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 129: /* exp_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1870 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 130: /* exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1876 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 131: /* binary_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1882 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 136: /* decl_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1888 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 145: /* union_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1894 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 148: /* con_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1900 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 149: /* log_or_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1906 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 150: /* log_and_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1912 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 151: /* inc_or_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1918 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 152: /* exc_or_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1924 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 153: /* and_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1930 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 155: /* eq_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1936 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 157: /* relational_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1942 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 159: /* shift_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1948 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 161: /* add_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1954 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 163: /* mul_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1960 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 164: /* cast_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1966 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 166: /* unary_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1972 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 167: /* dur_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1978 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 169: /* call_paren  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1984 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 171: /* post_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1990 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 173: /* primary_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1996 "src/ast/parser.c" /* yacc.c:1257  */
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
#line 102 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL);  }
#line 2264 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 103 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast)); }
#line 2270 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 107 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list)); }
#line 2276 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 108 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def)); }
#line 2282 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 109 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def)); }
#line 2288 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 114 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def(0, (yyvsp[-4].id_list), (yyvsp[-3].type_decl), (yyvsp[-1].class_body)); }
#line 2294 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 115 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_global); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2300 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 117 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].class_def), free_class_def); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2306 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 119 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2312 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 119 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = NULL; }
#line 2318 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 121 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2324 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 124 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body((yyvsp[0].section), NULL); }
#line 2330 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 125 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body((yyvsp[-1].section), (yyvsp[0].class_body)); }
#line 2336 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 129 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2342 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 130 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2348 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 134 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2354 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 135 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2360 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 139 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL);}
#line 2366 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 140 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list));}
#line 2372 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 144 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_static;   }
#line 2378 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 145 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2384 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 149 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2390 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 150 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2396 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 154 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-2].sym), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(arg)); }
#line 2402 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 156 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_ptr), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2408 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 162 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_typedef((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2414 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 166 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub)); }
#line 2420 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 167 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub)); }
#line 2426 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 171 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL); }
#line 2432 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 172 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list)); }
#line 2438 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 176 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code(NULL, get_pos(arg)); }
#line 2444 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 177 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code((yyvsp[-1].stmt_list), get_pos(arg)); }
#line 2450 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 196 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = insert_symbol((yyvsp[0].sval)); }
#line 2456 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 198 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = NULL; }
#line 2462 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 201 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sym), get_pos(arg)); }
#line 2468 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 202 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2474 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 206 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 1, get_pos(arg)); }
#line 2480 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 210 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 0, get_pos(arg)); }
#line 2486 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 214 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_case, (yyvsp[-1].exp), get_pos(arg)); }
#line 2492 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 215 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_case, (yyvsp[-1].exp), get_pos(arg)); }
#line 2498 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 219 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg));}
#line 2504 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 224 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_while, (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2510 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 226 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_while, (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2516 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 228 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2522 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 230 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2528 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 232 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2534 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 234 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); (yyval.stmt)->d.stmt_auto.is_ptr = 1;}
#line 2540 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 236 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_until, (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2546 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 238 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_until, (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2552 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 240 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2558 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 245 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(arg)); }
#line 2564 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 247 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(arg)); }
#line 2570 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 251 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_return, NULL, get_pos(arg)); }
#line 2576 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 252 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_return, (yyvsp[-1].exp), get_pos(arg)); }
#line 2582 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 253 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt(ae_stmt_break, get_pos(arg)); }
#line 2588 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 254 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt(ae_stmt_continue, get_pos(arg)); }
#line 2594 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 258 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_exp, (yyvsp[-1].exp),   get_pos(arg)); }
#line 2600 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 259 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_exp, NULL, get_pos(arg)); }
#line 2606 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 264 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_exp((yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 2612 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 269 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2618 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 272 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 2624 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 272 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2630 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 274 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2636 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 274 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2642 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 274 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2648 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 275 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2654 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 275 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2660 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 276 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2666 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 276 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2672 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 277 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2678 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 277 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2684 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 278 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2690 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 278 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2696 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2702 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2708 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2714 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2720 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2726 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2732 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 285 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub((yyvsp[-1].exp), get_pos(arg)); }
#line 2738 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 286 "utils/gwion.y" /* yacc.c:1646  */
    { if((yyvsp[-2].exp)->next){ gwion_error(arg, "invalid format for array init [...][...]..."); free_exp((yyvsp[-2].exp)); free_array_sub((yyvsp[0].array_sub)); YYERROR; } (yyval.array_sub) = prepend_array_sub((yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2744 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 287 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [...][]..."); free_exp((yyvsp[-3].exp)); YYERROR; }
#line 2750 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 291 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(arg)); }
#line 2756 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 292 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2762 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 293 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [][...]"); free_array_sub((yyvsp[-1].array_sub)); free_array_sub((yyvsp[0].array_sub)); YYERROR; }
#line 2768 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 298 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2774 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 300 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_static); (yyval.exp) = (yyvsp[0].exp); }
#line 2780 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 302 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_private); (yyval.exp) = (yyvsp[0].exp); }
#line 2786 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 306 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2792 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 307 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2798 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 310 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2804 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 314 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sym), (yyvsp[-1].arg_list), (yyvsp[0].stmt)); }
#line 2810 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 316 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_private); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2816 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 318 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_inline); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2822 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 320 "utils/gwion.y" /* yacc.c:1646  */
    { //CHECK_TEMPLATE(arg, $1, $2, free_func_def);

      if((yyvsp[0].func_def)->tmpl) {
        free_id_list((yyvsp[-1].id_list));
        free_func_def((yyvsp[0].func_def));
        gwion_error(arg, "double template decl");
        YYERROR;
      }
      (yyvsp[0].func_def)->tmpl = new_tmpl_list((yyvsp[-1].id_list), -1);
      (yyval.func_def) = (yyvsp[0].func_def); SET_FLAG((yyval.func_def), ae_flag_template);
    }
#line 2838 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 336 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_op , (yyvsp[-2].type_decl), OP_SYM((yyvsp[-3].ival)), (yyvsp[-1].arg_list), (yyvsp[0].stmt)); }
#line 2844 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 338 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_op | ae_flag_unary, (yyvsp[-2].type_decl), OP_SYM((yyvsp[-4].ival)), (yyvsp[-1].arg_list), (yyvsp[0].stmt)); }
#line 2850 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 340 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_dtor, new_type_decl(new_id_list(insert_symbol("void"), get_pos(arg)), 0,
      get_pos(arg)), insert_symbol("dtor"), NULL, (yyvsp[0].stmt)); }
#line 2857 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 344 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2863 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 344 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_ref; }
#line 2869 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 347 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)), (yyvsp[0].ival), get_pos(arg)); }
#line 2875 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 348 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2881 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 349 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)),
      (yyvsp[0].ival), get_pos(arg)); (yyval.type_decl)->types = (yyvsp[-3].type_list); }
#line 2888 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 351 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg));
      (yyval.type_decl)->types = (yyvsp[-5].type_list); }
#line 2895 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 353 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2901 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 354 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].type_decl), ae_flag_const); (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2907 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 358 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2913 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 359 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2919 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 363 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(arg));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sym); }
#line 2925 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 365 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2931 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 367 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2937 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 371 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL); }
#line 2943 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 372 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list)); }
#line 2949 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 376 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sym), NULL, get_pos(arg)); }
#line 2955 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 377 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2961 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 378 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2967 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 382 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_if((yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 2973 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 385 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(arg)); }
#line 2979 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 388 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(arg)); }
#line 2985 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 391 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(arg)); }
#line 2991 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 394 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(arg)); }
#line 2997 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 397 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(arg)); }
#line 3003 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 399 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 3009 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 399 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_neq; }
#line 3015 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 402 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3021 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 404 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_lt; }
#line 3027 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 404 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_gt; }
#line 3033 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 405 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_le; }
#line 3039 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 405 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ge; }
#line 3045 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 408 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3051 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 411 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 3057 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 412 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 3063 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 416 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3069 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 418 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3075 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 418 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3081 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 421 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3087 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 423 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3093 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 423 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide; }
#line 3099 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 423 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_percent; }
#line 3105 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 426 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3111 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 429 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_cast((yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(arg)); }
#line 3117 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 431 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3123 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 431 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3129 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 431 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3135 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 432 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3141 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 432 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3147 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 433 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3153 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 433 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3159 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 437 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary((yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3165 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 439 "utils/gwion.y" /* yacc.c:1646  */
    {
      if((yyvsp[0].type_decl)->array && !(yyvsp[0].type_decl)->array->exp) {
        free_type_decl((yyvsp[0].type_decl));
        gwion_error(arg, "can't use empty '[]' in 'new' expression");
        YYERROR;
      }
      (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), get_pos(arg));
    }
#line 3178 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 448 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary3(op_spork, (yyvsp[0].stmt), get_pos(arg)); }
#line 3184 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 451 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3190 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 454 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list((yyvsp[0].type_decl), NULL); }
#line 3196 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 455 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list((yyvsp[-2].type_decl), (yyvsp[0].type_list)); }
#line 3202 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 458 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 3208 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 458 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3214 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 460 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3220 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 460 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3226 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 463 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array((yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(arg)); }
#line 3232 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 465 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_call((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg));
      if((yyvsp[-1].type_list))(yyval.exp)->d.exp_func.tmpl = new_tmpl_call((yyvsp[-1].type_list)); }
#line 3239 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 468 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dot((yyvsp[-2].exp), (yyvsp[0].sym), get_pos(arg)); }
#line 3245 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 470 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_post((yyvsp[-1].exp), (yyvsp[0].ival), get_pos(arg)); }
#line 3251 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 473 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_primary_complex; }
#line 3257 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 474 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_primary_polar;   }
#line 3263 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 475 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_primary_vec;     }
#line 3269 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 478 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_id(     (yyvsp[0].sym), get_pos(arg)); }
#line 3275 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 479 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_int(    (yyvsp[0].ival), get_pos(arg)); }
#line 3281 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 480 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_float(  (yyvsp[0].fval), get_pos(arg)); }
#line 3287 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 481 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_string( (yyvsp[0].sval), get_pos(arg)); }
#line 3293 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 482 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_char(   (yyvsp[0].sval), get_pos(arg)); }
#line 3299 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 483 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3305 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 484 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3311 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 485 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_vec((yyvsp[-2].ival), (yyvsp[-1].exp), get_pos(arg)); }
#line 3317 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 486 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_hack(   (yyvsp[-1].exp), get_pos(arg)); }
#line 3323 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 217:
#line 487 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) =                      (yyvsp[-1].exp);                }
#line 3329 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 218:
#line 488 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_nil(        get_pos(arg)); }
#line 3335 "src/ast/parser.c" /* yacc.c:1646  */
    break;


#line 3339 "src/ast/parser.c" /* yacc.c:1646  */
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
#line 490 "utils/gwion.y" /* yacc.c:1906  */

