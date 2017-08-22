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
#line 8 "utils/gwion.y" /* yacc.c:339  */

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


#line 90 "src/ast/parser.c" /* yacc.c:339  */

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
    EXTENDS = 312,
    DOT = 313,
    COLONCOLON = 314,
    AND = 315,
    EQ = 316,
    GE = 317,
    GT = 318,
    LE = 319,
    LT = 320,
    MINUS = 321,
    PLUS = 322,
    NEQ = 323,
    SHIFT_LEFT = 324,
    SHIFT_RIGHT = 325,
    S_AND = 326,
    S_OR = 327,
    S_XOR = 328,
    OR = 329,
    AST_DTOR = 330,
    OPERATOR = 331,
    FUNC_PTR = 332,
    RSL = 333,
    RSR = 334,
    RSAND = 335,
    RSOR = 336,
    RSXOR = 337,
    RAND = 338,
    ROR = 339,
    REQ = 340,
    RNEQ = 341,
    RGT = 342,
    RGE = 343,
    RLT = 344,
    RLE = 345,
    TEMPLATE = 346,
    NOELSE = 347,
    LTB = 348,
    GTB = 349,
    VARARG = 350,
    UNION = 351,
    ATPAREN = 352,
    TYPEOF = 353,
    NUM = 354,
    FLOAT = 355,
    ID = 356,
    STRING_LIT = 357,
    CHAR_LIT = 358,
    NEG = 359
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
#define EXTENDS 312
#define DOT 313
#define COLONCOLON 314
#define AND 315
#define EQ 316
#define GE 317
#define GT 318
#define LE 319
#define LT 320
#define MINUS 321
#define PLUS 322
#define NEQ 323
#define SHIFT_LEFT 324
#define SHIFT_RIGHT 325
#define S_AND 326
#define S_OR 327
#define S_XOR 328
#define OR 329
#define AST_DTOR 330
#define OPERATOR 331
#define FUNC_PTR 332
#define RSL 333
#define RSR 334
#define RSAND 335
#define RSOR 336
#define RSXOR 337
#define RAND 338
#define ROR 339
#define REQ 340
#define RNEQ 341
#define RGT 342
#define RGE 343
#define RLT 344
#define RLE 345
#define TEMPLATE 346
#define NOELSE 347
#define LTB 348
#define GTB 349
#define VARARG 350
#define UNION 351
#define ATPAREN 352
#define TYPEOF 353
#define NUM 354
#define FLOAT 355
#define ID 356
#define STRING_LIT 357
#define CHAR_LIT 358
#define NEG 359

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 26 "utils/gwion.y" /* yacc.c:355  */

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

#line 365 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (MyArg* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 381 "src/ast/parser.c" /* yacc.c:358  */

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
#define YYFINAL  136
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1289

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  105
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  73
/* YYNRULES -- Number of rules.  */
#define YYNRULES  211
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  366

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   359

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
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   153,   153,   154,   158,   159,   160,   164,   168,   168,
     171,   172,   176,   177,   181,   182,   186,   187,   188,   192,
     193,   197,   198,   202,   203,   207,   208,   212,   213,   217,
     218,   222,   223,   224,   228,   229,   233,   234,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     253,   254,   258,   262,   266,   267,   271,   275,   277,   279,
     281,   283,   285,   287,   292,   294,   299,   300,   301,   302,
     306,   307,   311,   312,   316,   317,   321,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   353,   354,   355,   359,   360,   361,   365,   366,
     367,   368,   372,   373,   376,   376,   379,   381,   383,   388,
     388,   391,   392,   393,   398,   399,   403,   407,   408,   412,
     413,   414,   417,   418,   419,   422,   423,   428,   429,   434,
     435,   440,   441,   446,   447,   452,   453,   457,   457,   459,
     460,   464,   464,   464,   464,   467,   468,   473,   474,   478,
     479,   483,   483,   486,   487,   491,   491,   491,   494,   495,
     500,   501,   505,   505,   505,   506,   506,   507,   507,   511,
     512,   514,   516,   518,   524,   525,   529,   530,   533,   533,
     534,   534,   538,   538,   541,   542,   544,   546,   548,   553,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     564,   565
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
  "STATIC", "PUBLIC", "EXTENDS", "DOT", "COLONCOLON", "AND", "EQ", "GE",
  "GT", "LE", "LT", "MINUS", "PLUS", "NEQ", "SHIFT_LEFT", "SHIFT_RIGHT",
  "S_AND", "S_OR", "S_XOR", "OR", "AST_DTOR", "OPERATOR", "FUNC_PTR",
  "RSL", "RSR", "RSAND", "RSOR", "RSXOR", "RAND", "ROR", "REQ", "RNEQ",
  "RGT", "RGE", "RLT", "RLE", "TEMPLATE", "NOELSE", "LTB", "GTB", "VARARG",
  "UNION", "ATPAREN", "TYPEOF", "NUM", "FLOAT", "ID", "STRING_LIT",
  "CHAR_LIT", "NEG", "$accept", "ast", "section", "class_def", "class_ext",
  "class_decl", "class_body", "class_body2", "class_section", "id_list",
  "id_dot", "stmt_list", "static_decl", "function_decl", "func_ptr",
  "type_decl2", "arg_list", "code_segment", "stmt", "enum_stmt",
  "label_stmt", "goto_stmt", "case_stmt", "switch_stmt", "loop_stmt",
  "selection_stmt", "jump_stmt", "exp_stmt", "exp", "binary_exp",
  "template", "op", "array_exp", "array_empty", "decl_exp", "func_args",
  "decl_template", "func_def", "atsym", "type_decl", "decl_list",
  "union_stmt", "var_decl_list", "var_decl", "complex_exp", "polar_exp",
  "vec_exp", "conditional_expression", "logical_or_expression",
  "logical_and_expression", "inclusive_or_expression",
  "exclusive_or_expression", "and_expression", "equality_op",
  "equality_expression", "relationnal_op", "relational_expression",
  "shift_op", "shift_expression", "additive_op", "additive_expression",
  "multiplicative_op", "multiplicative_expression", "cast_exp", "unary_op",
  "unary_expression", "dur_exp", "type_list", "call_template",
  "call_paren", "postfix_op", "postfix_exp", "primary_exp", YY_NULLPTR
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
     355,   356,   357,   358,   359
};
# endif

#define YYPACT_NINF -317

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-317)))

#define YYTABLE_NINF -189

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     550,  -317,  -317,  1126,    27,   826,   628,  1126,  1126,  -317,
      11,    44,   706,    49,    54,    56,   -50,    64,    60,    65,
     199,    88,    96,  -317,  -317,     2,    70,    36,     6,  -317,
    -317,   102,     2,     2,    26,    21,   115,  1126,   121,  -317,
    -317,    -5,  -317,  -317,   140,   316,  -317,  -317,  -317,  -317,
     706,  -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,
     145,  1183,  -317,   132,  -317,    -9,  -317,    52,  -317,  -317,
    -317,  -317,  -317,     1,    94,    83,    91,    95,   -30,    80,
      35,    69,   143,   134,  1186,  -317,   108,    13,  -317,     2,
      -3,   157,  -317,   156,  -317,   158,  -317,   154,   159,   163,
    1126,  1126,    10,  1126,  1126,   766,   176,  1126,  -317,    39,
     146,    81,  -317,   178,  -317,  -317,   155,   169,   171,     2,
      52,   130,   126,   177,    90,   181,   180,    81,   191,   103,
    1126,   185,     6,  -317,  -317,  -317,  -317,  -317,  -317,  -317,
    -317,  1126,  -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,
    -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,  -317,
    -317,  -317,  -317,  -317,  -317,  -317,  1126,   886,  -317,  -317,
    -317,  -317,   149,   144,   180,  -317,   195,  1126,  1186,  1186,
    1186,  1186,  1186,  -317,  -317,  1186,  -317,  -317,  -317,  -317,
    1186,  -317,  -317,  1186,  -317,  -317,  1186,  -317,  -317,  -317,
    1186,     2,  -317,    60,  1126,  -317,  -317,   105,    21,  -317,
    -317,   192,  -317,  -317,  -317,   194,  -317,  -317,  -317,   197,
     201,   204,   205,   206,   207,   766,  -317,   208,  -317,  -317,
     213,   211,  -317,  -317,  -317,   212,  -317,     6,   155,   171,
     217,   122,  -317,   132,   131,    21,     2,   227,   214,  -317,
     222,  -317,  -317,  -317,    81,  -317,     2,  -317,   132,    52,
     202,    94,    83,    91,    95,   -30,    80,    35,    69,   143,
     134,  -317,    13,  -317,   147,   946,  -317,  1006,  -317,   706,
     706,  1126,  1126,   706,   706,  1066,   171,    81,    15,   136,
    -317,  -317,  -317,    20,   171,   225,  -317,  -317,    52,  1126,
     236,   155,   183,   141,  -317,  1186,  -317,  -317,   230,  -317,
     209,  -317,   231,   232,  -317,  -317,   706,   234,  -317,  -317,
    -317,   245,   240,  -317,   242,    52,  -317,   217,  -317,  -317,
    -317,  -317,     6,   241,   217,  -317,  -317,   706,   253,   255,
    -317,   706,  -317,   217,  -317,   254,  -317,  -317,   394,   171,
    -317,  -317,  -317,  -317,  -317,     2,  -317,   243,  -317,   472,
    -317,  -317,  -317,  -317,  -317,  -317
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     114,    71,   174,     0,     0,     0,     0,     0,     0,   177,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   175,   176,     0,     0,     0,     0,   173,
     172,     0,     0,     0,     0,     0,     0,     0,     0,   200,
     201,   199,   202,   203,     0,   114,     6,     4,    48,    41,
      23,    46,    42,    43,    45,    44,    39,    40,    47,    38,
       0,    72,   204,   205,    74,    11,     5,     0,    49,   206,
     207,   208,   108,   135,   137,   139,   141,   143,   145,   149,
     155,   159,   163,   168,     0,   170,   179,   184,   194,     0,
     199,     0,   211,     0,   105,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   199,   188,
     194,     0,    66,     0,    68,    69,   119,   181,   178,     0,
       0,    21,     0,     0,     0,     0,    31,     0,   186,     0,
       0,     0,     0,   120,    52,   121,     1,     3,    24,    70,
      77,     0,   101,    81,    82,    83,    84,    85,    80,    78,
      86,    87,    79,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,     0,     0,   107,    27,
      10,    28,     0,    26,   129,   109,   127,     0,     0,     0,
       0,     0,     0,   147,   148,     0,   154,   152,   153,   151,
       0,   157,   158,     0,   162,   161,     0,   166,   165,   167,
       0,     0,   180,     0,     0,   192,   193,     0,     0,   189,
     195,     0,   198,   209,   210,   102,    37,   133,   132,     0,
       0,     0,     0,     0,     0,     0,    53,     0,    55,    54,
      19,     0,    67,   182,   183,     0,   111,     0,   119,     0,
       0,     0,    33,    32,     0,     0,     0,     0,     0,   134,
       0,    73,    75,   106,     0,    25,     0,   130,   131,     0,
       0,   138,   140,   142,   144,   146,   150,   156,   160,   164,
     169,   171,   185,   197,     0,     0,   196,     0,   103,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,   122,   118,     0,     0,     0,   115,   187,     0,   124,
       0,   119,     9,     0,   128,     0,    76,   190,     0,   104,
      64,    57,     0,     0,    61,    63,     0,     0,    56,    20,
      50,     0,     0,   112,     0,     0,   117,     0,   110,   125,
     126,   123,     0,     0,     0,   136,   191,     0,     0,     0,
      59,     0,    51,     0,   113,    34,    29,     8,   114,     0,
      65,    58,    62,    60,    30,     0,    18,     0,    12,   114,
      16,    17,   116,    35,     7,    15
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -317,   216,  -317,  -316,  -317,  -317,  -317,   -97,  -317,  -113,
    -127,    -6,  -317,  -317,  -317,  -109,   -92,  -116,    -8,  -317,
    -317,  -317,  -317,  -317,  -317,  -317,  -317,   -86,     8,  -317,
    -317,  -317,   -57,   -90,   101,  -285,  -317,  -302,  -218,   -24,
     -31,  -317,  -103,   -55,  -317,  -317,  -317,   -32,  -317,    99,
     100,    98,    93,  -317,   104,  -317,    97,  -317,   113,  -317,
      87,  -317,    85,    84,  -317,   203,  -317,  -186,  -317,  -317,
    -317,   -11,   270
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    44,    45,    46,   333,   172,   357,   358,   359,   231,
     122,    47,   256,   173,    48,   125,   324,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
     209,   166,    62,    63,    64,   294,    65,    66,   135,    67,
     248,    68,   175,   176,    69,    70,    71,    72,    73,    74,
      75,    76,    77,   185,    78,   190,    79,   193,    80,   196,
      81,   200,    82,    83,    84,    85,    86,   129,   211,   276,
     212,    87,    88
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      97,   117,   234,   120,   102,   250,   168,   109,   124,   126,
     235,    91,    93,    95,   244,    98,    99,   236,   320,   225,
     291,   169,   274,   100,   133,  -188,   133,   204,   113,   134,
     210,   183,   356,   323,   177,     2,   243,     3,   184,     4,
      92,     5,   346,   356,   138,   131,   361,   170,   221,   349,
     222,   106,   210,   204,     7,     8,   101,   361,   354,   297,
     233,   103,     9,   205,   206,   120,   104,    28,   105,   242,
       3,   207,     4,   228,     5,   178,   107,    23,    24,    25,
      26,   111,    89,   331,   258,    28,   171,     7,     8,   205,
     206,   114,    28,    29,    30,   126,  -119,   207,  -119,   115,
      38,    28,   118,   116,   191,   192,   208,   121,   219,   220,
     290,   223,   224,   119,   123,   227,   321,   257,    38,   127,
      35,   116,   128,   292,    37,    38,    39,    40,    90,    42,
      43,   130,   208,   132,    38,   194,   195,   116,   247,   285,
     136,   302,   186,   187,   188,   189,   167,   303,   139,   251,
     197,   198,   199,   174,   179,   180,   304,    37,   278,    39,
      40,   108,    42,    43,   181,   201,   182,   203,   213,   214,
     318,   216,   217,   215,   319,    95,   218,   271,   326,   226,
     229,   232,   230,   204,   133,   260,   168,     6,   237,   238,
     239,   240,   272,   241,     5,   328,   245,   246,   249,   255,
     259,   168,   112,   254,   275,   347,   273,     2,   277,     3,
     279,     4,    95,     5,   280,   210,   281,   282,   287,   283,
     284,   286,   298,   295,   289,   296,     7,     8,   288,   293,
     299,   300,   126,   362,     9,   301,   305,   322,   327,   330,
     332,   306,   334,   336,   338,   339,   337,   341,   342,    23,
      24,    25,    26,   343,    89,   344,   351,   348,   352,   355,
     364,   137,   365,   363,    28,    29,    30,   252,   329,   325,
     345,   310,   311,   335,   264,   314,   315,   261,   263,   262,
     268,   269,   266,   308,   270,    95,   265,   202,   110,   312,
     313,     0,    35,   317,     0,     0,    37,    38,    39,    40,
      90,    42,    43,   267,     0,     0,     0,   247,   340,     0,
       0,     0,     0,     0,     0,     0,    -2,     0,     0,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,   350,
       5,   325,     6,   353,     0,     0,     0,     0,     0,     0,
       0,     0,   360,     7,     8,     0,     0,     0,     0,     0,
       0,     9,    10,   360,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    29,    30,     0,     0,     0,     0,     0,     0,
       0,    31,    32,    33,     0,     0,     0,     1,     0,     0,
       0,     0,     2,     0,     3,     0,     4,    34,     5,    35,
       6,   -13,    36,    37,    38,    39,    40,    41,    42,    43,
       0,     7,     8,     0,     0,     0,     0,     0,     0,     9,
      10,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      29,    30,     0,     0,     0,     0,     0,     0,     0,    31,
      32,    33,     0,     0,     0,     1,     0,     0,     0,     0,
       2,     0,     3,     0,     4,    34,     5,    35,     6,   -14,
      36,    37,    38,    39,    40,    41,    42,    43,     0,     7,
       8,     0,     0,     0,     0,     0,     0,     9,    10,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,    29,    30,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     1,     0,     0,     0,     0,     2,     0,
       3,     0,     4,    34,     5,    35,     6,     0,    36,    37,
      38,    39,    40,    41,    42,    43,     0,     7,     8,     0,
       0,     0,     0,     0,     0,     9,    10,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    28,    29,    30,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    33,     0,     0,
       0,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,    34,     5,    35,     6,    96,    36,    37,    38,    39,
      40,    41,    42,    43,     0,     7,     8,     0,     0,     0,
       0,     0,     0,     9,    10,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    28,    29,    30,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    33,     0,     0,     0,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,     0,
       5,    35,     6,     0,    36,    37,    38,    39,    40,    41,
      42,    43,     0,     7,     8,     0,     0,     0,     0,     0,
       0,     9,    10,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,     0,     0,     0,     0,     0,     0,     0,     1,
       0,    28,    29,    30,     2,     0,     3,     0,     4,     0,
       5,     0,     0,    33,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,    35,
       0,     9,    36,    37,    38,    39,    40,    41,    42,    43,
       0,     0,     0,     0,     0,     0,    23,    24,    25,    26,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    29,    30,     2,     0,     3,     0,     4,     0,
       5,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,    35,
       0,     9,     0,    37,    38,    39,    40,    90,    42,    43,
       0,     0,     0,     0,     0,     0,    23,    24,    25,    26,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    29,    30,     2,     0,     3,     0,     4,     0,
       5,   253,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,    35,
       0,     9,     0,    37,    38,    39,    40,    90,    42,    43,
       0,     0,     0,     0,     0,     0,    23,    24,    25,    26,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    29,    30,     2,     0,     3,     0,     4,   307,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,    35,
       0,     9,     0,    37,    38,    39,    40,    90,    42,    43,
       0,     0,     0,     0,     0,     0,    23,    24,    25,    26,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    29,    30,     2,     0,     3,     0,     4,     0,
       5,   309,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,    35,
       0,     9,     0,    37,    38,    39,    40,    90,    42,    43,
       0,     0,     0,     0,     0,     0,    23,    24,    25,    26,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    29,    30,     2,     0,     3,     0,     4,   316,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,    35,
       0,     9,     0,    37,    38,    39,    40,    90,    42,    43,
       0,     0,     0,     0,     0,     0,    23,    24,    25,    26,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    29,    30,     2,     0,     3,     0,     4,     0,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,    35,
       0,     9,     0,    37,    38,    39,    40,    90,    42,    43,
       0,     0,     0,     0,     0,     0,    23,    24,    25,    26,
       0,    89,     0,     0,     0,     0,     0,   140,   141,   142,
       0,    28,    29,    30,     2,     0,     3,     0,     4,     0,
       5,   143,   144,   145,   146,   147,   148,   149,   150,   151,
       0,     0,     0,     7,     8,     0,     0,     0,     0,    35,
       0,     9,     0,    37,    38,    39,    40,    90,    42,    43,
       0,     0,     0,     0,     0,     0,    23,    24,    25,    26,
       0,     0,     0,     0,   152,     0,     0,     0,     0,     0,
       0,     0,    29,    30,     0,     0,     0,     0,     0,     0,
       0,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,    39,    40,   108,    42,    43
};

static const yytype_int16 yycheck[] =
{
       6,    25,   118,    27,    12,   132,    63,    18,    32,    33,
     119,     3,     4,     5,   127,     7,     8,   120,     3,   105,
     238,    30,   208,    12,    29,    12,    29,    14,    20,    34,
      87,    61,   348,    13,    33,     8,   126,    10,    68,    12,
      13,    14,   327,   359,    50,    37,   348,    56,    38,   334,
      40,   101,   109,    14,    27,    28,    12,   359,   343,   245,
     117,    12,    35,    50,    51,    89,    12,    65,    12,   126,
      10,    58,    12,    34,    14,    74,    12,    50,    51,    52,
      53,    16,    55,   301,   174,    65,    95,    27,    28,    50,
      51,     3,    65,    66,    67,   119,   101,    58,   101,     3,
      98,    65,    32,   101,    69,    70,    93,   101,   100,   101,
     237,   103,   104,    77,    12,   107,   101,   174,    98,    93,
      93,   101,   101,   239,    97,    98,    99,   100,   101,   102,
     103,    16,    93,    12,    98,    66,    67,   101,   130,   225,
       0,   254,    62,    63,    64,    65,    14,   256,     3,   141,
       7,     8,     9,   101,    60,    72,   259,    97,   215,    99,
     100,   101,   102,   103,    73,    31,    71,    59,    11,    13,
     286,    17,    13,    15,   287,   167,    13,   201,   294,     3,
      34,     3,   101,    14,    29,   177,   243,    16,    58,    63,
      13,   101,   203,    12,    14,   298,     5,    94,    13,    55,
       5,   258,     3,    54,    12,   332,   101,     8,    14,    10,
      13,    12,   204,    14,    13,   272,    12,    12,     5,    13,
      13,    13,   246,   101,    12,    94,    27,    28,    17,    12,
       3,    17,   256,   349,    35,    13,    34,   101,    13,     3,
      57,    94,   101,    13,    13,    13,    37,    13,     3,    50,
      51,    52,    53,    13,    55,    13,     3,    16,     3,     5,
      17,    45,   359,   355,    65,    66,    67,   166,   299,   293,
     325,   279,   280,   305,   181,   283,   284,   178,   180,   179,
     193,   196,   185,   275,   200,   277,   182,    84,    18,   281,
     282,    -1,    93,   285,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,   190,    -1,    -1,    -1,   299,   316,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     0,    -1,    -1,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,   337,
      14,   355,    16,   341,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   348,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,   359,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    12,    91,    14,    93,
      16,    17,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    91,    14,    93,    16,    17,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    12,    91,    14,    93,    16,    -1,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    91,    14,    93,    16,    17,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    -1,
      14,    93,    16,    -1,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    93,
      -1,    35,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    93,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    93,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    93,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    93,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    93,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    93,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,     4,     5,     6,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    93,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    99,   100,   101,   102,   103
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    35,
      36,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    55,    65,    66,
      67,    75,    76,    77,    91,    93,    96,    97,    98,    99,
     100,   101,   102,   103,   106,   107,   108,   116,   119,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   137,   138,   139,   141,   142,   144,   146,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   159,   161,
     163,   165,   167,   168,   169,   170,   171,   176,   177,    55,
     101,   133,    13,   133,    15,   133,    17,   116,   133,   133,
      12,    12,   123,    12,    12,    12,   101,    12,   101,   176,
     177,    16,     3,   133,     3,     3,   101,   144,    32,    77,
     144,   101,   115,    12,   144,   120,   144,    93,   101,   172,
      16,   133,    12,    29,    34,   143,     0,   106,   116,     3,
       4,     5,     6,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    61,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,   136,    14,   137,    30,
      56,    95,   110,   118,   101,   147,   148,    33,    74,    60,
      72,    73,    71,    61,    68,   158,    62,    63,    64,    65,
     160,    69,    70,   162,    66,    67,   164,     7,     8,     9,
     166,    31,   170,    59,    14,    50,    51,    58,    93,   135,
     137,   173,   175,    11,    13,    15,    17,    13,    13,   133,
     133,    38,    40,   133,   133,   132,     3,   133,    34,    34,
     101,   114,     3,   137,   122,   120,   147,    58,    63,    13,
     101,    12,   137,   138,   114,     5,    94,   133,   145,    13,
     115,   133,   139,    15,    54,    55,   117,   137,   138,     5,
     133,   154,   155,   156,   157,   159,   161,   163,   165,   167,
     168,   144,   176,   101,   172,    12,   174,    14,   137,    13,
      13,    12,    12,    13,    13,   132,    13,     5,    17,    12,
     115,   143,   122,    12,   140,   101,    94,   172,   144,     3,
      17,    13,   114,   120,   147,    34,    94,    13,   133,    15,
     123,   123,   133,   133,   123,   123,    13,   133,   122,   114,
       3,   101,   101,    13,   121,   144,   122,    13,   147,   145,
       3,   143,    57,   109,   101,   152,    13,    37,    13,    13,
     123,    13,     3,    13,    13,   148,   140,   115,    16,   140,
     123,     3,     3,   123,   140,     5,   108,   111,   112,   113,
     116,   142,   122,   121,    17,   112
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   105,   106,   106,   107,   107,   107,   108,   109,   109,
     110,   110,   111,   111,   112,   112,   113,   113,   113,   114,
     114,   115,   115,   116,   116,   117,   117,   118,   118,   119,
     119,   120,   120,   120,   121,   121,   122,   122,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     124,   124,   125,   126,   127,   127,   128,   129,   129,   129,
     129,   129,   129,   129,   130,   130,   131,   131,   131,   131,
     132,   132,   133,   133,   134,   134,   135,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   137,   137,   137,   138,   138,   138,   139,   139,
     139,   139,   140,   140,   141,   141,   142,   142,   142,   143,
     143,   144,   144,   144,   145,   145,   146,   147,   147,   148,
     148,   148,   149,   150,   151,   152,   152,   153,   153,   154,
     154,   155,   155,   156,   156,   157,   157,   158,   158,   159,
     159,   160,   160,   160,   160,   161,   161,   162,   162,   163,
     163,   164,   164,   165,   165,   166,   166,   166,   167,   167,
     168,   168,   169,   169,   169,   169,   169,   169,   169,   170,
     170,   170,   170,   170,   171,   171,   172,   172,   173,   173,
     174,   174,   175,   175,   176,   176,   176,   176,   176,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     8,     2,     0,
       1,     0,     1,     0,     1,     2,     1,     1,     1,     1,
       3,     1,     3,     1,     2,     1,     0,     1,     1,     6,
       7,     1,     2,     2,     2,     4,     2,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     6,     2,     3,     3,     3,     5,     5,     7,     6,
       7,     5,     7,     5,     5,     7,     2,     3,     2,     2,
       2,     1,     1,     3,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     4,     5,     2,     3,     2,     1,     2,
       5,     3,     2,     3,     0,     4,     7,     5,     4,     0,
       1,     2,     4,     5,     2,     3,     5,     1,     3,     1,
       2,     2,     3,     3,     3,     1,     5,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     2,     3,     3,     1,     3,     1,     3,     0,     1,
       2,     3,     1,     1,     1,     2,     3,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     2
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, MyArg* arg)
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, MyArg* arg)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, MyArg* arg)
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, MyArg* arg)
{
  YYUSE (yyvaluep);
  YYUSE (arg);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 120: /* type_decl2  */
#line 149 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1685 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 144: /* type_decl  */
#line 149 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1691 "src/ast/parser.c" /* yacc.c:1257  */
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
yyparse (MyArg* arg)
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
      yychar = yylex (&yylval, scanner, arg);
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
#line 153 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL, get_pos(scanner));  }
#line 1959 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 154 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(scanner)); }
#line 1965 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 158 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(scanner)); }
#line 1971 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 159 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(scanner)); }
#line 1977 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 160 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(scanner)); }
#line 1983 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 165 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].id_list), (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->types = (yyvsp[-7].id_list); }
#line 1989 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 168 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[0].id_list); }
#line 1995 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 168 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2001 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 171 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_public; }
#line 2007 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 172 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_private; }
#line 2013 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 176 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2019 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 177 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2025 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 181 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body( (yyvsp[0].section), get_pos(scanner)); }
#line 2031 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 182 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body( (yyvsp[-1].section), (yyvsp[0].class_body), get_pos(scanner)); }
#line 2037 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 186 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list( (yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2043 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 187 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def( (yyvsp[0].func_def), get_pos(scanner)); }
#line 2049 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 188 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def( (yyvsp[0].class_def), get_pos(scanner)); }
#line 2055 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 192 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2061 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 193 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2067 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 197 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2073 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 198 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2079 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 202 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(scanner));}
#line 2085 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 203 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(scanner));}
#line 2091 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 207 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_static;   }
#line 2097 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 208 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_instance; }
#line 2103 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 212 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_func; }
#line 2109 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 213 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2115 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 217 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-2].sval), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(scanner)); }
#line 2121 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 218 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_flag_static, (yyvsp[-2].sval), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(scanner)); }
#line 2127 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 222 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2133 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 223 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2139 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 224 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2145 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 228 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(scanner)); }
#line 2151 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 229 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(scanner)); }
#line 2157 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 233 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( NULL, get_pos(scanner)); }
#line 2163 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 234 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( (yyvsp[-1].stmt_list), get_pos(scanner)); }
#line 2169 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 253 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-2].id_list), NULL, get_pos(scanner)); }
#line 2175 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 254 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sval), get_pos(scanner)); }
#line 2181 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 258 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 1, get_pos(scanner)); }
#line 2187 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 262 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 0, get_pos(scanner)); }
#line 2193 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 266 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2199 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 267 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2205 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 271 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner));}
#line 2211 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 276 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2217 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 278 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2223 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2229 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2235 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2241 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 286 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2247 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 288 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop( (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2253 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 293 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(scanner)); }
#line 2259 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 295 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2265 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 299 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( NULL, get_pos(scanner)); }
#line 2271 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 300 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2277 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 301 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_break(get_pos(scanner)); }
#line 2283 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 302 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_continue(get_pos(scanner)); }
#line 2289 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 306 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_expression((yyvsp[-1].exp),   get_pos(scanner)); }
#line 2295 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 307 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_expression(NULL, get_pos(scanner)); }
#line 2301 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 311 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2307 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 312 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_expression((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2313 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 316 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2319 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 317 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2325 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 321 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2331 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 325 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2337 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 326 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2343 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 327 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; /* LCOV_EXCL_LINE */ }
#line 2349 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 328 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2355 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 329 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2361 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 330 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2367 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 331 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2373 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 332 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2379 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 333 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2385 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 334 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2391 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 335 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2397 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 336 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2403 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 337 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2409 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 338 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2415 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 339 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2421 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 340 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2427 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 341 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2433 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 342 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2439 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 343 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2445 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 344 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2451 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 345 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2457 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 346 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2463 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 347 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2469 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 348 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2475 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 349 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2481 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 353 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( (yyvsp[-1].exp), get_pos(scanner) ); }
#line 2487 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 354 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[0].array_sub), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2493 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 355 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( new_array_sub( NULL, get_pos(scanner)), (yyvsp[-3].exp), get_pos(scanner)); }
#line 2499 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 359 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( NULL, get_pos(scanner)); }
#line 2505 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 360 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-2].array_sub), NULL, get_pos(scanner)); }
#line 2511 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 361 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-1].array_sub), (yyvsp[0].array_sub)->exp_list, get_pos(scanner)); }
#line 2517 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 366 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); }
#line 2523 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 367 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); (yyval.exp)->d.exp_decl.types = (yyvsp[-3].type_list); }
#line 2529 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 368 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); }
#line 2535 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 372 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2541 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 373 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2547 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 376 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2553 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 376 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2559 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 380 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->types = (yyvsp[-6].id_list); if((yyvsp[-6].id_list)) SET_FLAG((yyval.func_def), ae_flag_template);}
#line 2565 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 382 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_func | ae_flag_static | ae_flag_op , (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2571 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 384 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_func | ae_flag_instance | ae_flag_dtor, new_type_decl(new_id_list("void", get_pos(scanner)), 0, 
      get_pos(scanner)), "dtor", NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2578 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 388 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2584 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 388 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 2590 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 391 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), (yyvsp[0].ival), get_pos(scanner)); }
#line 2596 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 392 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scanner)); }
#line 2602 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 393 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scanner)); }
#line 2608 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 398 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2614 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 399 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2620 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 403 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-2].decl_list), get_pos(scanner)); }
#line 2626 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 407 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(scanner)); }
#line 2632 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 408 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(scanner)); }
#line 2638 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 412 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sval), NULL, get_pos(scanner)); }
#line 2644 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 413 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2650 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 414 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2656 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 417 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2662 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 418 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar(   (yyvsp[-1].exp), get_pos(scanner)); }
#line 2668 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 419 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec(     (yyvsp[-1].exp), get_pos(scanner)); }
#line 2674 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 424 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_if( (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2680 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 428 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2686 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 430 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2692 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 434 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2698 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 436 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2704 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 440 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2710 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 442 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2716 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 446 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2722 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 448 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(scanner)); }
#line 2728 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 452 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2734 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 454 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2740 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 457 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2746 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 457 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_neq; }
#line 2752 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 459 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2758 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 461 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2764 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 464 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_lt; }
#line 2770 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 464 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_gt; }
#line 2776 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 464 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_le; }
#line 2782 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 464 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ge; }
#line 2788 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 467 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2794 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 469 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2800 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 473 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 2806 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 474 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 2812 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 478 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2818 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 480 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2824 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 483 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2830 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 483 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 2836 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 486 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2842 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 488 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_plus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2848 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 491 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 2854 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 491 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide; }
#line 2860 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 491 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_percent; }
#line 2866 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 494 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2872 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 496 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2878 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 502 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_cast( (yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2884 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 505 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2890 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 505 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 2896 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 505 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 2902 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 506 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 2908 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 506 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 2914 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 507 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 2920 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 507 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 2926 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 511 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2932 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 513 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary( (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2938 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 515 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(scanner)); }
#line 2944 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 517 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary2(op_new, (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2950 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 519 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary3( op_spork, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2956 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 525 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2962 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 529 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sval), get_pos(scanner)), NULL, get_pos(scanner)); }
#line 2968 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 530 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sval), get_pos(scanner)), (yyvsp[0].type_list), get_pos(scanner)); }
#line 2974 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 533 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 2980 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 533 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[0].type_list);}
#line 2986 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 534 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 2992 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 534 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 2998 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 538 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3004 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 538 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3010 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 543 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array( (yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3016 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 545 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_call( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); (yyval.exp)->d.exp_func.types = (yyvsp[-1].type_list); }
#line 3022 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 547 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dot( (yyvsp[-2].exp), (yyvsp[0].sval), get_pos(scanner)); }
#line 3028 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 549 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_postfix( (yyvsp[-1].exp), (yyvsp[0].ival), get_pos(scanner)); }
#line 3034 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 553 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_ID(      (yyvsp[0].sval), get_pos(scanner)); }
#line 3040 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 554 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_int(     (yyvsp[0].ival), get_pos(scanner)); }
#line 3046 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 555 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_float(   (yyvsp[0].fval), get_pos(scanner)); }
#line 3052 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 556 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_string(  (yyvsp[0].sval), get_pos(scanner)); }
#line 3058 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 557 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_char(    (yyvsp[0].sval), get_pos(scanner)); }
#line 3064 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 558 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3070 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 559 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3076 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 560 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_complex( (yyvsp[0].c_val), get_pos(scanner)); }
#line 3082 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 561 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_polar(   (yyvsp[0].polar), get_pos(scanner)); }
#line 3088 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 562 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_vec(     (yyvsp[0].vec), get_pos(scanner)); }
#line 3094 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 563 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_hack(    (yyvsp[-1].exp), get_pos(scanner)); }
#line 3100 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 564 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) =                       (yyvsp[-1].exp);                    }
#line 3106 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 565 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_nil(         get_pos(scanner)); }
#line 3112 "src/ast/parser.c" /* yacc.c:1646  */
    break;


#line 3116 "src/ast/parser.c" /* yacc.c:1646  */
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
#line 567 "utils/gwion.y" /* yacc.c:1906  */

