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
#define scan arg->scanner

void gwion_error(void* data, const char* s);
int gwion_lex(void*, void*);
int get_pos(void* data);

#line 82 "src/ast/parser.c" /* yacc.c:339  */

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
    FUNC_PTR = 333,
    RSL = 334,
    RSR = 335,
    RSAND = 336,
    RSOR = 337,
    RSXOR = 338,
    RAND = 339,
    ROR = 340,
    REQ = 341,
    RNEQ = 342,
    RGT = 343,
    RGE = 344,
    RLT = 345,
    RLE = 346,
    TEMPLATE = 347,
    NOELSE = 348,
    LTB = 349,
    GTB = 350,
    VARARG = 351,
    UNION = 352,
    ATPAREN = 353,
    TYPEOF = 354,
    CONST = 355,
    NUM = 356,
    FLOAT = 357,
    ID = 358,
    STRING_LIT = 359,
    CHAR_LIT = 360,
    NEG = 361
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
#define FUNC_PTR 333
#define RSL 334
#define RSR 335
#define RSAND 336
#define RSOR 337
#define RSXOR 338
#define RAND 339
#define ROR 340
#define REQ 341
#define RNEQ 342
#define RGT 343
#define RGE 344
#define RLT 345
#define RLE 346
#define TEMPLATE 347
#define NOELSE 348
#define LTB 349
#define GTB 350
#define VARARG 351
#define UNION 352
#define ATPAREN 353
#define TYPEOF 354
#define CONST 355
#define NUM 356
#define FLOAT 357
#define ID 358
#define STRING_LIT 359
#define CHAR_LIT 360
#define NEG 361

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 15 "utils/gwion.y" /* yacc.c:355  */

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

#line 361 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (Scanner* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 377 "src/ast/parser.c" /* yacc.c:358  */

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
#define YYFINAL  151
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1426

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  107
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  75
/* YYNRULES -- Number of rules.  */
#define YYNRULES  225
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  429

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   361

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
     105,   106
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   101,   101,   102,   106,   107,   108,   112,   116,   116,
     119,   120,   124,   125,   129,   130,   134,   135,   136,   140,
     141,   145,   146,   150,   151,   155,   156,   160,   161,   165,
     166,   170,   171,   172,   176,   177,   181,   182,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     201,   202,   206,   210,   214,   218,   219,   223,   227,   229,
     231,   233,   235,   237,   239,   244,   246,   251,   252,   253,
     254,   258,   259,   263,   264,   268,   269,   273,   276,   276,
     276,   277,   277,   278,   278,   279,   279,   280,   280,   281,
     281,   281,   282,   282,   282,   283,   283,   283,   284,   284,
     284,   285,   285,   289,   290,   291,   295,   296,   297,   301,
     302,   303,   304,   305,   306,   310,   311,   312,   313,   317,
     318,   321,   321,   324,   326,   328,   330,   335,   335,   338,
     339,   340,   341,   342,   343,   348,   349,   353,   354,   355,
     356,   360,   361,   365,   366,   367,   370,   371,   372,   375,
     376,   381,   382,   387,   388,   393,   394,   399,   400,   405,
     406,   410,   410,   412,   413,   417,   417,   417,   417,   420,
     421,   426,   427,   431,   432,   436,   436,   439,   440,   444,
     444,   444,   447,   448,   453,   454,   458,   458,   458,   459,
     459,   460,   460,   464,   465,   467,   469,   471,   477,   478,
     482,   483,   486,   486,   487,   487,   489,   489,   492,   493,
     495,   497,   499,   504,   505,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516
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
  "FUNC_PTR", "RSL", "RSR", "RSAND", "RSOR", "RSXOR", "RAND", "ROR", "REQ",
  "RNEQ", "RGT", "RGE", "RLT", "RLE", "TEMPLATE", "NOELSE", "LTB", "GTB",
  "VARARG", "UNION", "ATPAREN", "TYPEOF", "CONST", "NUM", "FLOAT", "ID",
  "STRING_LIT", "CHAR_LIT", "NEG", "$accept", "ast", "section",
  "class_def", "class_ext", "class_decl", "class_body", "class_body2",
  "class_section", "id_list", "id_dot", "stmt_list", "static_decl",
  "function_decl", "func_ptr", "type_decl2", "arg_list", "code_segment",
  "stmt", "opt_id", "enum_stmt", "label_stmt", "goto_stmt", "case_stmt",
  "switch_stmt", "loop_stmt", "selection_stmt", "jump_stmt", "exp_stmt",
  "exp", "binary_exp", "template", "op", "array_exp", "array_empty",
  "decl_exp", "private_decl", "func_args", "decl_template", "func_def",
  "atsym", "type_decl", "decl_list", "union_stmt", "var_decl_list",
  "var_decl", "complex_exp", "polar_exp", "vec_exp", "con_exp",
  "log_or_exp", "log_and_exp", "inc_or_exp", "exc_or_exp", "and_exp",
  "eq_op", "eq_exp", "rel_op", "relational_exp", "shift_op", "shift_exp",
  "add_op", "add_exp", "mul_op", "mul_exp", "cast_exp", "unary_op",
  "unary_exp", "dur_exp", "type_list", "call_template", "call_paren",
  "post_op", "post_exp", "primary_exp", YY_NULLPTR
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
     355,   356,   357,   358,   359,   360,   361
};
# endif

#define YYPACT_NINF -361

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-361)))

#define YYTABLE_NINF -203

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     614,  -361,  -361,  1321,   955,  1016,   693,  1321,  1321,  -361,
      26,    28,   772,    36,    43,    54,   -33,    61,    79,    62,
     833,    82,    84,  -361,  -361,    56,    66,   226,   156,    -2,
    -361,  -361,   100,    56,    56,    22,    20,   109,  1321,   116,
     -32,  -361,  -361,    -4,  -361,  -361,   135,   377,  -361,  -361,
    -361,  -361,   772,  -361,  -361,  -361,  -361,  -361,  -361,  -361,
    -361,  -361,   140,  1318,  -361,   130,  -361,  -361,     6,  -361,
      42,  -361,  -361,  -361,  -361,  -361,    17,    86,    75,    80,
      77,   -20,   106,   -18,    52,   131,   126,    29,  -361,   102,
      91,  -361,   121,   142,    -8,   149,  -361,   150,  -361,   158,
    -361,   164,   147,   161,   166,  1321,  1321,    23,  1321,  1321,
     894,   183,  1321,  -361,   144,   154,    87,  -361,   186,  -361,
    -361,   173,   179,   190,    56,    20,   191,    42,   177,    20,
     193,    15,    42,   151,   153,   201,   122,   214,   213,    87,
     224,   136,  1321,   219,    -2,    -2,   221,   173,  -361,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,  1321,  -361,  -361,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,
    -361,  1321,  1077,  -361,  -361,  -361,  -361,   180,   194,   213,
    -361,   230,  1321,    29,    29,    29,    29,    29,  -361,  -361,
      29,  -361,  -361,  -361,  -361,    29,  -361,  -361,    29,  -361,
    -361,    29,  -361,  -361,  -361,    29,    56,  -361,    79,  1321,
    -361,  -361,   143,    20,  -361,  -361,   240,  -361,   185,  -361,
    -361,   243,  -361,  -361,  -361,   252,   253,   257,   258,   260,
     262,   894,  -361,   265,  -361,  -361,   276,   266,  -361,  -361,
    -361,   270,   195,  1321,  -361,    20,   273,    42,   196,  1321,
     194,  -361,    -2,   173,   190,   274,   192,  -361,   130,   198,
      20,    56,   293,   281,  -361,   287,   244,    -2,  -361,  -361,
    -361,  -361,    87,  -361,    56,  -361,   130,    42,   275,    86,
      75,    80,    77,   -20,   106,   -18,    52,   131,   126,  -361,
      91,  -361,   215,  1138,  -361,  1199,  -361,   772,   772,  1321,
    1321,   772,   772,  1260,   190,    87,   208,   210,    56,   298,
     222,  1321,  -361,    56,   299,    56,  -361,  -361,  -361,    10,
     190,   308,  -361,  -361,    42,  1321,   208,   173,   173,   311,
     269,   225,  -361,    29,  -361,  -361,   318,  -361,   295,  -361,
     320,   321,  -361,  -361,   772,   322,  -361,  -361,  -361,   333,
     324,    42,   208,    56,   323,    42,   208,   235,  -361,   328,
      42,  -361,   274,  -361,  -361,   339,  -361,  -361,   173,    -2,
     327,   274,  -361,  -361,   772,   342,   343,  -361,   772,  -361,
     274,  -361,   344,    42,   208,  -361,   345,   274,  -361,   346,
    -361,  -361,  -361,  -361,   456,   190,  -361,  -361,  -361,  -361,
    -361,  -361,  -361,   347,  -361,   190,    56,  -361,   335,  -361,
     535,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     121,    72,   188,     0,     0,     0,     0,     0,     0,   191,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,   190,     0,     0,     0,   121,     0,
     187,   186,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   214,   215,   213,   216,   217,     0,   121,     6,     4,
      48,    41,    23,    46,    42,    43,    45,    44,    39,    40,
      47,    38,     0,    73,   218,   219,    75,   114,    11,     5,
       0,    49,   220,   221,   222,   109,   149,   151,   153,   155,
     157,   159,   163,   169,   173,   177,   182,     0,   184,   193,
     198,   208,     0,     0,   213,     0,   225,     0,   106,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   213,   202,   208,     0,    67,     0,    69,
      70,   127,   195,   192,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,    31,     0,
     200,     0,     0,     0,     0,     0,     0,   127,   128,    53,
     129,     1,     3,    24,    71,    78,     0,   102,    82,    83,
      84,    85,    86,    81,    79,    87,    88,    80,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,     0,     0,   108,    27,    10,    28,     0,    26,   143,
     110,   141,     0,     0,     0,     0,     0,     0,   161,   162,
       0,   168,   166,   167,   165,     0,   171,   172,     0,   176,
     175,     0,   180,   179,   181,     0,     0,   194,     0,     0,
     206,   207,     0,     0,   203,   209,     0,   212,     0,   223,
     224,   103,    37,   147,   146,     0,     0,     0,     0,     0,
       0,     0,    54,     0,    56,    55,    19,     0,    68,   196,
     197,     0,     0,     0,   112,     0,     0,     0,     0,     0,
      26,   115,     0,   127,     0,     0,     0,    33,    32,     0,
       0,     0,     0,     0,   148,     0,     0,     0,   130,    74,
      76,   107,     0,    25,     0,   144,   145,     0,     0,   152,
     154,   156,   158,   160,   164,   170,   174,   178,   183,   185,
     199,   211,     0,     0,   210,     0,   104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,   117,     0,     0,     0,    22,   131,   126,     0,
       0,     0,   122,   201,     0,   135,    50,   127,   127,     0,
       9,     0,   142,     0,    77,   204,     0,   105,    65,    58,
       0,     0,    62,    64,     0,     0,    57,    20,    51,     0,
       0,     0,    50,     0,     0,     0,    50,     0,   119,     0,
       0,   125,     0,   111,   136,     0,   133,   132,   127,     0,
       0,     0,   150,   205,     0,     0,     0,    60,     0,    52,
       0,   113,     0,     0,    50,   116,     0,     0,   120,    34,
      29,   137,   134,     8,   121,     0,    66,    59,    63,    61,
      30,   138,   118,     0,   139,     0,     0,    18,     0,    12,
     121,    16,    17,   123,   140,   124,    35,     7,    15
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -361,   306,  -361,  -360,  -361,  -361,  -361,   -66,  -361,  -115,
    -125,    -6,    95,   227,  -361,  -109,   -60,  -118,    -5,  -308,
    -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,   -88,     9,
    -361,  -361,  -361,   -63,  -105,   176,  -361,  -307,   331,  -332,
    -139,   -24,  -227,  -361,  -121,   -10,  -361,  -361,  -361,    18,
    -361,   169,   170,   168,   171,  -361,   172,  -361,   165,  -361,
     163,  -361,   162,  -361,   155,   157,  -361,   284,  -361,   -94,
    -361,  -361,  -361,     0,   355
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    46,    47,    48,   380,   187,   418,   419,   420,   247,
     134,    49,   284,   188,    50,   137,   369,    51,    52,   359,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,   224,   181,    64,    65,    66,    67,   330,    68,    69,
     150,    70,   273,    71,   190,   191,    72,    73,    74,    75,
      76,    77,    78,    79,    80,   200,    81,   205,    82,   208,
      83,   211,    84,   215,    85,    86,    87,    88,    89,   141,
     226,   304,   227,    90,    91
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     102,   122,   183,   127,   132,   250,   254,   107,   278,   136,
     138,   261,    95,    97,    99,   251,   103,   104,   114,   275,
     276,   148,   241,   368,   269,   148,   319,   225,   375,   118,
     149,   252,   324,   268,   145,   258,   184,     2,   105,     3,
     106,     4,   198,     5,   417,   184,   153,   143,   108,   199,
     192,   225,   206,   207,   392,   109,     7,     8,   396,   249,
     417,   237,   185,   238,     9,   400,   110,   146,   127,   132,
     111,   147,   422,   112,   405,   267,    29,   132,   116,    23,
      24,    25,    26,   410,   286,   119,   413,   120,   422,     3,
     415,     4,   193,     5,   364,  -127,    30,    31,   123,  -127,
     138,   133,   186,  -202,   257,   219,     7,     8,   374,    39,
      40,   186,   135,   121,   235,   236,   139,   239,   240,   209,
     210,   243,    29,   140,   327,   142,   285,    38,   144,   302,
      41,    42,   113,    44,    45,   151,   322,   326,   212,   213,
     214,   220,   221,   154,   182,   189,   328,   194,   195,   197,
     222,   272,   339,   313,   196,    39,    40,   216,   219,   121,
     229,   320,   218,   230,   232,   279,   342,   340,   306,   201,
     202,   203,   204,   231,   233,   341,   333,    38,   244,   234,
      41,    42,   113,    44,    45,   223,   242,    29,   245,   248,
     246,    99,   299,   219,   220,   221,   356,   228,   376,   377,
     357,   288,   148,   222,   257,   183,     6,   253,    29,   259,
     262,   128,   371,   373,   264,   125,   367,   263,   300,   128,
      39,    40,    29,   183,   121,   265,   266,     5,    99,   270,
      29,   271,   274,   277,   282,   287,   129,   225,   223,   402,
     391,    39,    40,    29,   395,   121,   301,   334,    35,   283,
     129,    29,   303,   130,   403,    39,    40,   305,   129,   121,
     138,   130,   272,    39,    40,   307,   308,   121,   272,   309,
     310,   255,   412,   311,   256,   312,    39,    40,   314,   255,
     121,   315,   317,   316,    39,    40,   329,   423,   121,   321,
     318,   323,    29,   332,   361,   331,   335,   425,   336,   365,
     337,   138,   348,   349,   124,   370,   352,   353,   338,   343,
     344,   358,   346,   360,    99,   362,   366,   363,   350,   351,
     125,   372,   355,   126,   378,    39,    40,   379,   381,   121,
     272,   383,   384,   385,   386,   388,   389,   390,   397,   393,
     394,   398,   401,   404,   272,   407,   408,   411,   414,   387,
     424,   416,   427,   152,   428,   325,   426,   280,   260,   131,
     399,   382,   289,   291,   290,   294,   297,   292,   295,   293,
     296,   217,   298,   115,     0,     0,     0,    -2,     0,   406,
       1,     0,     0,   409,     0,     2,     0,     3,     0,     4,
       0,     5,   370,     6,     0,     0,     0,     0,   421,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
       0,     0,     9,    10,   421,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,     0,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     0,     0,     0,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,    35,
       5,    36,     6,   -13,    37,    38,    39,    40,    41,    42,
      43,    44,    45,     7,     8,     0,     0,     0,     0,     0,
       0,     9,    10,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,     0,    28,     0,     0,     0,     0,     0,     0,
       0,     0,    29,    30,    31,     0,     0,     0,     0,     0,
       0,     0,    32,    33,    34,     0,     0,     0,     1,     0,
       0,     0,     0,     2,     0,     3,     0,     4,    35,     5,
      36,     6,   -14,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     7,     8,     0,     0,     0,     0,     0,     0,
       9,    10,     0,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,     0,    28,     0,     0,     0,     0,     0,     0,     0,
       0,    29,    30,    31,     0,     0,     0,     0,     0,     0,
       0,    32,    33,    34,     0,     0,     0,     1,     0,     0,
       0,     0,     2,     0,     3,     0,     4,    35,     5,    36,
       6,     0,    37,    38,    39,    40,    41,    42,    43,    44,
      45,     7,     8,     0,     0,     0,     0,     0,     0,     9,
      10,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
       0,    28,     0,     0,     0,     0,     0,     0,     0,     0,
      29,    30,    31,     0,     0,     0,     0,     0,     0,     0,
      32,    33,    34,     0,     0,     0,     1,     0,     0,     0,
       0,     2,     0,     3,     0,     4,    35,     5,    36,     6,
     100,    37,    38,    39,    40,    41,    42,    43,    44,    45,
       7,     8,     0,     0,     0,     0,     0,     0,     9,    10,
       0,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,     0,
     101,     0,     0,     0,     0,     0,     0,     0,     0,    29,
      30,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    34,     0,     0,     0,     1,     0,     0,     0,     0,
       2,     0,     3,     0,     4,     0,     5,    36,     6,     0,
      37,    38,    39,    40,    41,    42,    43,    44,    45,     7,
       8,     0,     0,     0,     0,     0,     0,     9,    10,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,     0,   101,
       0,     0,     0,     0,     0,     0,   117,     0,    29,    30,
      31,     2,     0,     3,     0,     4,     0,     5,     0,     0,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,    36,     0,     9,    37,
      38,    39,    40,    41,    42,    43,    44,    45,     0,     0,
       0,     0,     0,    23,    24,    25,    26,     0,    92,     0,
      93,     0,     0,     0,     0,     0,     0,     1,     0,    29,
      30,    31,     2,     0,     3,     0,     4,     0,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,    36,     0,     9,
       0,    38,    39,    40,    41,    42,    94,    44,    45,     0,
       0,     0,     0,     0,    23,    24,    25,    26,     0,    92,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
      29,    30,    31,     2,     0,     3,     0,     4,    96,     5,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,    36,     0,
       9,     0,    38,    39,    40,    41,    42,    94,    44,    45,
       0,     0,     0,     0,     0,    23,    24,    25,    26,     0,
      92,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    29,    30,    31,     2,     0,     3,     0,     4,     0,
       5,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,    36,
       0,     9,     0,    38,    39,    40,    41,    42,    94,    44,
      45,     0,     0,     0,     0,     0,    23,    24,    25,    26,
       0,    92,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    29,    30,    31,     2,     0,     3,     0,     4,
       0,     5,   281,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
      36,     0,     9,     0,    38,    39,    40,    41,    42,    94,
      44,    45,     0,     0,     0,     0,     0,    23,    24,    25,
      26,     0,    92,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     2,     0,     3,     0,
       4,   345,     5,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,    36,     0,     9,     0,    38,    39,    40,    41,    42,
      94,    44,    45,     0,     0,     0,     0,     0,    23,    24,
      25,    26,     0,    92,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     2,     0,     3,
       0,     4,     0,     5,   347,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,    36,     0,     9,     0,    38,    39,    40,    41,
      42,    94,    44,    45,     0,     0,     0,     0,     0,    23,
      24,    25,    26,     0,    92,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    30,    31,     2,     0,
       3,     0,     4,   354,     5,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,    36,     0,     9,     0,    38,    39,    40,
      41,    42,    94,    44,    45,     0,     0,     0,     0,     0,
      23,    24,    25,    26,     0,    92,     0,    93,     0,     0,
       0,     0,   155,   156,   157,     0,    29,    30,    31,     2,
       0,     3,     0,     4,     0,     5,   158,   159,   160,   161,
     162,   163,   164,   165,   166,     0,     0,     0,     7,     8,
       0,     0,     0,     0,    36,     0,     9,     0,    38,    39,
      40,    41,    42,    94,    44,    45,     0,     0,     0,     0,
       0,    23,    24,    25,    26,     0,    92,     0,    93,     0,
     167,     0,     0,     0,     0,     0,     0,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
       0,     0,     0,     0,     0,    36,     0,     0,     0,    38,
      39,    40,    41,    42,    94,    44,    45
};

static const yytype_int16 yycheck[] =
{
       6,    25,    65,    27,    28,   123,   127,    12,   147,    33,
      34,   132,     3,     4,     5,   124,     7,     8,    18,   144,
     145,    29,   110,    13,   139,    29,   253,    90,   336,    20,
      34,   125,   259,   138,    66,   129,    30,     8,    12,    10,
      12,    12,    62,    14,   404,    30,    52,    38,    12,    69,
      33,   114,    70,    71,   362,    12,    27,    28,   366,   122,
     420,    38,    56,    40,    35,   372,    12,    99,    92,    93,
     103,   103,   404,    12,   381,   138,    66,   101,    16,    50,
      51,    52,    53,   390,   189,     3,   394,     3,   420,    10,
     397,    12,    75,    14,   321,   103,    67,    68,    32,   103,
     124,   103,    96,    12,   128,    14,    27,    28,   335,    99,
     100,    96,    12,   103,   105,   106,    94,   108,   109,    67,
      68,   112,    66,   103,   263,    16,   189,    98,    12,   223,
     101,   102,   103,   104,   105,     0,   257,   262,     7,     8,
       9,    50,    51,     3,    14,   103,   264,    61,    73,    72,
      59,   142,   277,   241,    74,    99,   100,    31,    14,   103,
      11,   255,    60,    13,    17,   156,   287,   282,   231,    63,
      64,    65,    66,    15,    13,   284,   270,    98,    34,    13,
     101,   102,   103,   104,   105,    94,     3,    66,    34,     3,
     103,   182,   216,    14,    50,    51,   314,    55,   337,   338,
     315,   192,    29,    59,   228,   268,    16,    16,    66,    16,
      59,    55,   330,   334,    13,    94,   325,    64,   218,    55,
      99,   100,    66,   286,   103,   103,    12,    14,   219,     5,
      66,    95,    13,    12,    54,     5,    94,   300,    94,   378,
     361,    99,   100,    66,   365,   103,   103,   271,    92,    55,
      94,    66,    12,    97,   379,    99,   100,    14,    94,   103,
     284,    97,   253,    99,   100,    13,    13,   103,   259,    12,
      12,    94,   393,    13,    97,    13,    99,   100,    13,    94,
     103,     5,    12,    17,    99,   100,    12,   405,   103,    16,
      95,    95,    66,    95,   318,   103,     3,   415,    17,   323,
      13,   325,   307,   308,    78,   329,   311,   312,    64,    34,
      95,   103,   303,   103,   305,    17,    17,    95,   309,   310,
      94,    13,   313,    97,    13,    99,   100,    58,   103,   103,
     321,    13,    37,    13,    13,    13,     3,    13,   103,   363,
      17,    13,     3,    16,   335,     3,     3,     3,     3,   354,
       3,     5,    17,    47,   420,   260,   416,   181,   131,    28,
     370,   343,   193,   195,   194,   200,   211,   196,   205,   197,
     208,    87,   215,    18,    -1,    -1,    -1,     0,    -1,   384,
       3,    -1,    -1,   388,    -1,     8,    -1,    10,    -1,    12,
      -1,    14,   416,    16,    -1,    -1,    -1,    -1,   404,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    36,   420,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    77,    78,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    92,
      14,    94,    16,    17,    97,    98,    99,   100,   101,   102,
     103,   104,   105,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    77,    78,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    12,    92,    14,
      94,    16,    17,    97,    98,    99,   100,   101,   102,   103,
     104,   105,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    77,    78,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    12,    92,    14,    94,
      16,    -1,    97,    98,    99,   100,   101,   102,   103,   104,
     105,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    77,    78,    -1,    -1,    -1,     3,    -1,    -1,    -1,
      -1,     8,    -1,    10,    -1,    12,    92,    14,    94,    16,
      17,    97,    98,    99,   100,   101,   102,   103,   104,   105,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    -1,    14,    94,    16,    -1,
      97,    98,    99,   100,   101,   102,   103,   104,   105,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    66,    67,
      68,     8,    -1,    10,    -1,    12,    -1,    14,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    94,    -1,    35,    97,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    66,
      67,    68,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    94,    -1,    35,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,     8,    -1,    10,    -1,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    94,    -1,
      35,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,     8,    -1,    10,    -1,    12,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    94,
      -1,    35,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,     8,    -1,    10,    -1,    12,
      -1,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      94,    -1,    35,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,
      53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,     8,    -1,    10,    -1,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    94,    -1,    35,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,     8,    -1,    10,
      -1,    12,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    94,    -1,    35,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,     8,    -1,
      10,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    94,    -1,    35,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,
      50,    51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,
      -1,    -1,     4,     5,     6,    -1,    66,    67,    68,     8,
      -1,    10,    -1,    12,    -1,    14,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    94,    -1,    35,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,
      -1,    50,    51,    52,    53,    -1,    55,    -1,    57,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    35,
      36,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    55,    57,    66,
      67,    68,    76,    77,    78,    92,    94,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   108,   109,   110,   118,
     121,   124,   125,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   140,   141,   142,   143,   145,   146,
     148,   150,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   163,   165,   167,   169,   171,   172,   173,   174,   175,
     180,   181,    55,    57,   103,   136,    13,   136,    15,   136,
      17,    57,   118,   136,   136,    12,    12,   125,    12,    12,
      12,   103,    12,   103,   180,   181,    16,     3,   136,     3,
       3,   103,   148,    32,    78,    94,    97,   148,    55,    94,
      97,   145,   148,   103,   117,    12,   148,   122,   148,    94,
     103,   176,    16,   136,    12,    66,    99,   103,    29,    34,
     147,     0,   108,   118,     3,     4,     5,     6,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    62,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,   139,    14,   140,    30,    56,    96,   112,   120,   103,
     151,   152,    33,    75,    61,    73,    74,    72,    62,    69,
     162,    63,    64,    65,    66,   164,    70,    71,   166,    67,
      68,   168,     7,     8,     9,   170,    31,   174,    60,    14,
      50,    51,    59,    94,   138,   140,   177,   179,    55,    11,
      13,    15,    17,    13,    13,   136,   136,    38,    40,   136,
     136,   135,     3,   136,    34,    34,   103,   116,     3,   140,
     124,   122,   176,    16,   151,    94,    97,   148,   176,    16,
     120,   151,    59,    64,    13,   103,    12,   140,   141,   116,
       5,    95,   136,   149,    13,   117,   117,    12,   147,   136,
     142,    15,    54,    55,   119,   140,   141,     5,   136,   158,
     159,   160,   161,   163,   165,   167,   169,   171,   172,   148,
     180,   103,   176,    12,   178,    14,   140,    13,    13,    12,
      12,    13,    13,   135,    13,     5,    17,    12,    95,   149,
     176,    16,   151,    95,   149,   119,   117,   147,   124,    12,
     144,   103,    95,   176,   148,     3,    17,    13,    64,   117,
     116,   122,   151,    34,    95,    13,   136,    15,   125,   125,
     136,   136,   125,   125,    13,   136,   124,   116,   103,   126,
     103,   148,    17,    95,   149,   148,    17,   122,    13,   123,
     148,   124,    13,   151,   149,   126,   147,   147,    13,    58,
     111,   103,   156,    13,    37,    13,    13,   125,    13,     3,
      13,   151,   126,   148,    17,   151,   126,   103,    13,   152,
     144,     3,   147,   117,    16,   144,   125,     3,     3,   125,
     144,     3,   151,   126,     3,   144,     5,   110,   113,   114,
     115,   118,   146,   124,     3,   124,   123,    17,   114
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   107,   108,   108,   109,   109,   109,   110,   111,   111,
     112,   112,   113,   113,   114,   114,   115,   115,   115,   116,
     116,   117,   117,   118,   118,   119,   119,   120,   120,   121,
     121,   122,   122,   122,   123,   123,   124,   124,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     126,   126,   127,   128,   129,   130,   130,   131,   132,   132,
     132,   132,   132,   132,   132,   133,   133,   134,   134,   134,
     134,   135,   135,   136,   136,   137,   137,   138,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   140,   140,   140,   141,   141,   141,   142,
     142,   142,   142,   142,   142,   143,   143,   143,   143,   144,
     144,   145,   145,   146,   146,   146,   146,   147,   147,   148,
     148,   148,   148,   148,   148,   149,   149,   150,   150,   150,
     150,   151,   151,   152,   152,   152,   153,   154,   155,   156,
     156,   157,   157,   158,   158,   159,   159,   160,   160,   161,
     161,   162,   162,   163,   163,   164,   164,   164,   164,   165,
     165,   166,   166,   167,   167,   168,   168,   169,   169,   170,
     170,   170,   171,   171,   172,   172,   173,   173,   173,   173,
     173,   173,   173,   174,   174,   174,   174,   174,   175,   175,
     176,   176,   177,   177,   178,   178,   179,   179,   180,   180,
     180,   180,   180,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     8,     2,     0,
       1,     0,     1,     0,     1,     2,     1,     1,     1,     1,
       3,     1,     3,     1,     2,     1,     0,     1,     1,     6,
       7,     1,     2,     2,     2,     4,     2,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     6,     2,     3,     3,     3,     5,     5,     7,
       6,     7,     5,     7,     5,     5,     7,     2,     3,     2,
       2,     2,     1,     1,     3,     1,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     5,     2,     3,     2,     1,
       2,     5,     3,     6,     1,     3,     6,     4,     7,     2,
       3,     0,     4,     7,     8,     5,     4,     0,     1,     2,
       3,     4,     5,     5,     6,     2,     3,     6,     7,     7,
       8,     1,     3,     1,     2,     2,     3,     3,     3,     1,
       5,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     2,     3,     3,     1,     3,
       1,     3,     0,     1,     2,     3,     1,     1,     1,     2,
       3,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     2
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
          case 110: /* class_def  */
#line 94 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_def(((*yyvaluep).class_def)); }
#line 1729 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 113: /* class_body  */
#line 95 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1735 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 114: /* class_body2  */
#line 95 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1741 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 121: /* func_ptr  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1747 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 122: /* type_decl2  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1753 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 124: /* code_segment  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1759 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 125: /* stmt  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1765 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 127: /* enum_stmt  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1771 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 128: /* label_stmt  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1777 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 129: /* goto_stmt  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1783 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 130: /* case_stmt  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1789 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 131: /* switch_stmt  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1795 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 132: /* loop_stmt  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1801 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 133: /* selection_stmt  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1807 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 134: /* jump_stmt  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1813 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 135: /* exp_stmt  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1819 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 138: /* template  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 1825 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 148: /* type_decl  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1831 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 150: /* union_stmt  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1837 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 176: /* type_list  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 1843 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 177: /* call_template  */
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 1849 "src/ast/parser.c" /* yacc.c:1257  */
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
#line 101 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL, get_pos(scan));  }
#line 2117 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 102 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(scan)); }
#line 2123 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 106 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(scan)); }
#line 2129 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 107 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(scan)); }
#line 2135 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 108 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(scan)); }
#line 2141 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 113 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].id_list), (yyvsp[-1].class_body), get_pos(scan)); (yyval.class_def)->types = (yyvsp[-7].id_list); }
#line 2147 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 116 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[0].id_list); }
#line 2153 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 116 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2159 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 119 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_public; }
#line 2165 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 120 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_private; }
#line 2171 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 124 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2177 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 125 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2183 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 129 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body( (yyvsp[0].section), get_pos(scan)); }
#line 2189 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 130 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body( (yyvsp[-1].section), (yyvsp[0].class_body), get_pos(scan)); }
#line 2195 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 134 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list( (yyvsp[0].stmt_list), get_pos(scan)); }
#line 2201 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 135 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def( (yyvsp[0].func_def), get_pos(scan)); }
#line 2207 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 136 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def( (yyvsp[0].class_def), get_pos(scan)); }
#line 2213 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 140 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scan)); }
#line 2219 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 141 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scan)); }
#line 2225 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 145 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scan)); }
#line 2231 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 146 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scan)); }
#line 2237 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 150 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(scan));}
#line 2243 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 151 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(scan));}
#line 2249 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 155 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_static;   }
#line 2255 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 156 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_instance; }
#line 2261 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 160 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_func; }
#line 2267 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 161 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2273 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 165 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-2].sval), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(scan)); }
#line 2279 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 166 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_flag_static, (yyvsp[-2].sval), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(scan)); }
#line 2285 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 170 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2291 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 171 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scan)); }
#line 2297 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 172 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scan)); }
#line 2303 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 176 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(scan)); }
#line 2309 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 177 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(scan)); }
#line 2315 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 181 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( NULL, get_pos(scan)); }
#line 2321 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 182 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( (yyvsp[-1].stmt_list), get_pos(scan)); }
#line 2327 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 201 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sval) = NULL; }
#line 2333 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 202 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sval) = (yyvsp[0].sval);  }
#line 2339 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 206 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sval), get_pos(scan)); }
#line 2345 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 210 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 1, get_pos(scan)); }
#line 2351 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 214 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 0, get_pos(scan)); }
#line 2357 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 218 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scan)); }
#line 2363 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 219 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scan)); }
#line 2369 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 223 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scan));}
#line 2375 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 228 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scan)); }
#line 2381 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 230 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scan)); }
#line 2387 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 232 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(scan)); }
#line 2393 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 234 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scan)); }
#line 2399 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 236 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scan)); }
#line 2405 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 238 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scan)); }
#line 2411 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 240 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop( (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scan)); }
#line 2417 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 245 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(scan)); }
#line 2423 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 247 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(scan)); }
#line 2429 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 251 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( NULL, get_pos(scan)); }
#line 2435 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 252 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( (yyvsp[-1].exp), get_pos(scan)); }
#line 2441 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 253 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_break(get_pos(scan)); }
#line 2447 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 254 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_continue(get_pos(scan)); }
#line 2453 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 258 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp((yyvsp[-1].exp),   get_pos(scan)); }
#line 2459 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 259 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp(NULL, get_pos(scan)); }
#line 2465 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 263 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2471 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 264 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_exp((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scan)); }
#line 2477 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 268 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2483 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 269 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scan)); }
#line 2489 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 273 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2495 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 276 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2501 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 276 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2507 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 276 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2513 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 277 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2519 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 277 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2525 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 278 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2531 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 278 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2537 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2543 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2549 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2555 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2561 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2567 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2573 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2579 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2585 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2591 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2597 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2603 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2609 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2615 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2621 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2627 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2633 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 285 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2639 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 285 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2645 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 289 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( (yyvsp[-1].exp), get_pos(scan) ); }
#line 2651 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 290 "utils/gwion.y" /* yacc.c:1646  */
    { if((yyvsp[-2].exp)->next){ yyerror(&scan, "invalid format for array init [...][...]..."); free_exp((yyvsp[-2].exp)); free_array_sub((yyvsp[0].array_sub)); YYERROR; } (yyval.array_sub) = prepend_array_sub( (yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2657 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 291 "utils/gwion.y" /* yacc.c:1646  */
    { yyerror(&scan, "partially empty array init [...][]..."); free_exp((yyvsp[-3].exp)); YYERROR; }
#line 2663 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 295 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(scan)); }
#line 2669 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 296 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2675 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 297 "utils/gwion.y" /* yacc.c:1646  */
    { yyerror(&scan, "partially empty array init [][...]"); free_array_sub((yyvsp[-1].array_sub)); free_array_sub((yyvsp[0].array_sub)); YYERROR; }
#line 2681 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 302 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(scan)); }
#line 2687 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 303 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(scan)); (yyval.exp)->d.exp_decl.types = (yyvsp[-3].type_list); }
#line 2693 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 304 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(scan)); SET_FLAG((yyval.exp)->d.exp_decl.type, ae_flag_static); }
#line 2699 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 305 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(scan)); SET_FLAG((yyval.exp)->d.exp_decl.type, ae_flag_static); (yyval.exp)->d.exp_decl.types = (yyvsp[-3].type_list); }
#line 2705 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 310 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(scan)); SET_FLAG((yyval.exp)->d.exp_decl.type, ae_flag_private); }
#line 2711 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 311 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(scan)); SET_FLAG((yyval.exp)->d.exp_decl.type, ae_flag_private); (yyval.exp)->d.exp_decl.types = (yyvsp[-3].type_list); }
#line 2717 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 312 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(scan)); SET_FLAG((yyval.exp)->d.exp_decl.type, ae_flag_private | ae_flag_static); }
#line 2723 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 313 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(scan));  SET_FLAG((yyval.exp)->d.exp_decl.type, ae_flag_private | ae_flag_static); (yyval.exp)->d.exp_decl.types = (yyvsp[-3].type_list); }
#line 2729 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 317 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2735 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 318 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2741 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 321 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2747 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 321 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2753 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 325 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scan)); (yyval.func_def)->types = (yyvsp[-6].id_list); if((yyvsp[-6].id_list)) SET_FLAG((yyval.func_def), ae_flag_template);}
#line 2759 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 327 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scan)); (yyval.func_def)->types = (yyvsp[-6].id_list); if((yyvsp[-6].id_list)) SET_FLAG((yyval.func_def), ae_flag_template); SET_FLAG((yyval.func_def), ae_flag_private); }
#line 2765 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 329 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_func | ae_flag_static | ae_flag_op , (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scan)); }
#line 2771 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 331 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_func | ae_flag_instance | ae_flag_dtor, new_type_decl(new_id_list("void", get_pos(scan)), 0,
      get_pos(scan)), "dtor", NULL, (yyvsp[0].stmt), get_pos(scan)); }
#line 2778 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 335 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2784 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 335 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 2790 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 338 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sval), get_pos(scan)), (yyvsp[0].ival), get_pos(scan)); }
#line 2796 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 339 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sval), get_pos(scan)), (yyvsp[0].ival), get_pos(scan)); SET_FLAG((yyval.type_decl), ae_flag_const); }
#line 2802 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 340 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scan)); }
#line 2808 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 341 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scan)); SET_FLAG((yyval.type_decl), ae_flag_const); }
#line 2814 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 342 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scan)); }
#line 2820 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 343 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scan)); SET_FLAG((yyval.type_decl), ae_flag_const); }
#line 2826 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 348 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2832 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 349 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2838 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 353 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(scan));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sval) ? insert_symbol((yyvsp[-1].sval)) : NULL; }
#line 2844 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 354 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(scan));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sval) ? insert_symbol((yyvsp[-1].sval)) : NULL; (yyval.stmt)->d.stmt_union.flag |= ae_flag_static; }
#line 2850 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 355 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(scan));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sval) ? insert_symbol((yyvsp[-1].sval)) : NULL; (yyval.stmt)->d.stmt_union.flag |= ae_flag_private; }
#line 2856 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 356 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(scan));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sval) ? insert_symbol((yyvsp[-1].sval)) : NULL; (yyval.stmt)->d.stmt_union.flag |= ae_flag_private | ae_flag_static; }
#line 2862 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 360 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(scan)); }
#line 2868 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 361 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(scan)); }
#line 2874 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 365 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sval), NULL, get_pos(scan)); }
#line 2880 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 366 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scan)); }
#line 2886 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 367 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scan)); }
#line 2892 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 370 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex( (yyvsp[-1].exp), get_pos(scan)); }
#line 2898 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 371 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar(   (yyvsp[-1].exp), get_pos(scan)); }
#line 2904 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 372 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec(     (yyvsp[-1].exp), get_pos(scan)); }
#line 2910 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 377 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_if( (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scan)); }
#line 2916 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 381 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2922 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 383 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(scan)); }
#line 2928 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 387 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2934 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 389 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(scan)); }
#line 2940 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 393 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2946 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 395 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(scan)); }
#line 2952 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 399 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2958 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 401 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(scan)); }
#line 2964 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 405 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2970 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 407 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(scan)); }
#line 2976 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 410 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2982 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 410 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_neq; }
#line 2988 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 412 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2994 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 414 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scan)); }
#line 3000 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 417 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_lt; }
#line 3006 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 417 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_gt; }
#line 3012 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 417 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_le; }
#line 3018 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 417 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ge; }
#line 3024 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 420 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3030 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 422 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scan)); }
#line 3036 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 426 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 3042 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 427 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 3048 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 431 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3054 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 433 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scan)); }
#line 3060 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 436 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3066 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 436 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3072 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 439 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3078 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 441 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scan)); }
#line 3084 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 444 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3090 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 444 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide; }
#line 3096 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 444 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_percent; }
#line 3102 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 447 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3108 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 449 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scan)); }
#line 3114 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 455 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_cast( (yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(scan)); }
#line 3120 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 458 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3126 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 458 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3132 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 458 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3138 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 459 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3144 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 459 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3150 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 460 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3156 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 460 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3162 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 464 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3168 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 466 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary( (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scan)); }
#line 3174 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 468 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(scan)); }
#line 3180 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 470 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary2(op_new, (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scan)); }
#line 3186 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 472 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary3( op_spork, (yyvsp[0].stmt), get_pos(scan)); }
#line 3192 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 478 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scan)); }
#line 3198 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 482 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sval), get_pos(scan)), NULL, get_pos(scan)); }
#line 3204 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 483 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sval), get_pos(scan)), (yyvsp[0].type_list), get_pos(scan)); }
#line 3210 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 486 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 3216 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 486 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[0].type_list);}
#line 3222 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 487 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 3228 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 487 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3234 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 489 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3240 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 489 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3246 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 494 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array( (yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(scan)); }
#line 3252 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 496 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_call( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scan)); (yyval.exp)->d.exp_func.types = (yyvsp[-1].type_list); }
#line 3258 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 498 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dot( (yyvsp[-2].exp), (yyvsp[0].sval), get_pos(scan)); }
#line 3264 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 500 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_post( (yyvsp[-1].exp), (yyvsp[0].ival), get_pos(scan)); }
#line 3270 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 504 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_ID(      (yyvsp[0].sval), get_pos(scan)); }
#line 3276 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 505 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_int(     (yyvsp[0].ival), get_pos(scan)); }
#line 3282 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 506 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_float(   (yyvsp[0].fval), get_pos(scan)); }
#line 3288 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 507 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_string(  (yyvsp[0].sval), get_pos(scan)); }
#line 3294 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 217:
#line 508 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_char(    (yyvsp[0].sval), get_pos(scan)); }
#line 3300 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 218:
#line 509 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(   (yyvsp[0].array_sub), get_pos(scan)); }
#line 3306 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 219:
#line 510 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(   (yyvsp[0].array_sub), get_pos(scan)); }
#line 3312 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 220:
#line 511 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_complex( (yyvsp[0].c_val), get_pos(scan)); }
#line 3318 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 221:
#line 512 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_polar(   (yyvsp[0].polar), get_pos(scan)); }
#line 3324 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 222:
#line 513 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_vec(     (yyvsp[0].vec), get_pos(scan)); }
#line 3330 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 223:
#line 514 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_hack(    (yyvsp[-1].exp), get_pos(scan)); }
#line 3336 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 224:
#line 515 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) =                       (yyvsp[-1].exp);                    }
#line 3342 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 225:
#line 516 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_nil(         get_pos(scan)); }
#line 3348 "src/ast/parser.c" /* yacc.c:1646  */
    break;


#line 3352 "src/ast/parser.c" /* yacc.c:1646  */
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
#line 518 "utils/gwion.y" /* yacc.c:1906  */

