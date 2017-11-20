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
#line 7 "utils/gwion.y" /* yacc.c:339  */

#define YYERROR_VERBOSE
#include "absyn.h"
#include "scanner.h"
#define scanner arg->scanner


char *strcat(char *dest, const char *src);
void gwion_error(void* data, const char* s);
int gwion_lex(void*, void* , void*);
static int get_pos(void* data)
{
  Scanner* scan = (Scanner*)map_get(scan_map, (vtype)data);
  return scan->line;
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
#line 25 "utils/gwion.y" /* yacc.c:355  */

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
  Decl_List decl_list; // for union
  Func_Def func_def;
  Section* section;
  ID_List id_list;
  Type_List type_list; // call template
  Class_Body class_body;
  ID_List class_ext;
  Class_Def class_def;
  Ast ast;

#line 369 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (Scanner* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 385 "src/ast/parser.c" /* yacc.c:358  */

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
#define YYFINAL  146
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1408

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  107
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  75
/* YYNRULES -- Number of rules.  */
#define YYNRULES  221
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  401

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
       0,   153,   153,   154,   158,   159,   160,   164,   168,   168,
     171,   172,   176,   177,   181,   182,   186,   187,   188,   192,
     193,   197,   198,   202,   203,   207,   208,   212,   213,   217,
     218,   222,   223,   224,   228,   229,   233,   234,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     253,   254,   258,   262,   266,   270,   271,   275,   279,   281,
     283,   285,   287,   289,   291,   296,   298,   303,   304,   305,
     306,   310,   311,   315,   316,   320,   321,   325,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   357,   358,   359,   363,   364,   365,   369,
     370,   371,   372,   373,   374,   378,   379,   380,   381,   385,
     386,   389,   389,   392,   394,   396,   401,   401,   404,   405,
     406,   407,   408,   409,   414,   415,   419,   423,   424,   428,
     429,   430,   433,   434,   435,   438,   439,   444,   445,   450,
     451,   456,   457,   462,   463,   468,   469,   473,   473,   475,
     476,   480,   480,   480,   480,   483,   484,   489,   490,   494,
     495,   499,   499,   502,   503,   507,   507,   507,   510,   511,
     516,   517,   521,   521,   521,   522,   522,   523,   523,   527,
     528,   530,   532,   534,   540,   541,   545,   546,   549,   549,
     550,   550,   554,   554,   557,   558,   560,   562,   564,   569,
     570,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,   581
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
  "var_decl", "complex_exp", "polar_exp", "vec_exp",
  "conditional_expression", "logical_or_expression",
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
     355,   356,   357,   358,   359,   360,   361
};
# endif

#define YYPACT_NINF -346

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-346)))

#define YYTABLE_NINF -199

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     596,  -346,  -346,  1303,   937,   998,   675,  1303,  1303,  -346,
      11,    30,   754,    32,    36,    39,   -42,    57,    53,    62,
     815,    79,    83,  -346,  -346,    -7,    56,    45,   162,     5,
    -346,  -346,    85,    -7,    -7,    15,    19,   114,  1303,   119,
     -27,  -346,  -346,     4,  -346,  -346,   136,   359,  -346,  -346,
    -346,  -346,   754,  -346,  -346,  -346,  -346,  -346,  -346,  -346,
    -346,  -346,   134,   189,  -346,   126,  -346,  -346,    10,  -346,
      38,  -346,  -346,  -346,  -346,  -346,     2,    91,    80,    89,
      92,   -19,    -8,    34,    27,   161,   143,    75,  -346,   121,
      20,  -346,    72,    -3,   171,  -346,   170,  -346,   169,  -346,
     172,   175,   177,  1303,  1303,    24,  1303,  1303,   876,   188,
    1303,  -346,    40,   168,   100,  -346,   202,  -346,  -346,   190,
     204,   205,    -7,    19,    38,   140,    19,    38,   163,   156,
     211,   122,   214,   213,   100,   224,   137,  1303,   222,     5,
       5,   226,   190,  -346,  -346,  -346,  -346,  -346,  -346,  -346,
    -346,  1303,  -346,  -346,  -346,  -346,  -346,  -346,  -346,  -346,
    -346,  -346,  -346,  -346,  -346,  -346,  -346,  -346,  -346,  -346,
    -346,  -346,  -346,  -346,  -346,  -346,  1303,  1059,  -346,  -346,
    -346,  -346,   191,   187,   213,  -346,   239,  1303,    75,    75,
      75,    75,    75,  -346,  -346,    75,  -346,  -346,  -346,  -346,
      75,  -346,  -346,    75,  -346,  -346,    75,  -346,  -346,  -346,
      75,    -7,  -346,    53,  1303,  -346,  -346,   144,    19,  -346,
    -346,   234,  -346,  -346,  -346,   235,  -346,  -346,  -346,   240,
     241,   236,   243,   244,   246,   876,  -346,   247,  -346,  -346,
     258,   249,  -346,  -346,  -346,   255,   157,  -346,    19,    38,
     186,  -346,     5,   190,   205,   270,   180,  -346,   126,   193,
      19,    -7,   282,   269,  -346,   276,   230,     5,  -346,  -346,
    -346,  -346,   100,  -346,    -7,  -346,   126,    38,   256,    91,
      80,    89,    92,   -19,    -8,    34,    27,   161,   143,  -346,
      20,  -346,   200,  1120,  -346,  1181,  -346,   754,   754,  1303,
    1303,   754,   754,  1242,   205,   100,   195,   196,    -7,   206,
    -346,    -7,  -346,  -346,  -346,    18,   205,   287,  -346,  -346,
      38,  1303,   195,   190,   190,   290,   248,   207,  -346,    75,
    -346,  -346,   292,  -346,   274,  -346,   294,   300,  -346,  -346,
     754,   301,  -346,  -346,  -346,   312,   303,    38,    -7,    38,
    -346,   304,    38,  -346,   270,  -346,  -346,   315,  -346,  -346,
     190,     5,   305,   270,  -346,  -346,   754,   316,   317,  -346,
     754,  -346,   270,  -346,    38,  -346,  -346,   318,  -346,  -346,
    -346,  -346,   438,   205,  -346,  -346,  -346,  -346,  -346,  -346,
      -7,  -346,   308,  -346,   517,  -346,  -346,  -346,  -346,  -346,
    -346
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     121,    72,   184,     0,     0,     0,     0,     0,     0,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   185,   186,     0,     0,     0,     0,     0,
     183,   182,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   210,   211,   209,   212,   213,     0,   121,     6,     4,
      48,    41,    23,    46,    42,    43,    45,    44,    39,    40,
      47,    38,     0,    73,   214,   215,    75,   114,    11,     5,
       0,    49,   216,   217,   218,   109,   145,   147,   149,   151,
     153,   155,   159,   165,   169,   173,   178,     0,   180,   189,
     194,   204,     0,   209,     0,   221,     0,   106,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   209,   198,   204,     0,    67,     0,    69,    70,   126,
     191,   188,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,    31,     0,   196,     0,     0,     0,     0,
       0,     0,   126,   127,    53,   128,     1,     3,    24,    71,
      78,     0,   102,    82,    83,    84,    85,    86,    81,    79,
      87,    88,    80,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,     0,     0,   108,    27,
      10,    28,     0,    26,   139,   110,   137,     0,     0,     0,
       0,     0,     0,   157,   158,     0,   164,   162,   163,   161,
       0,   167,   168,     0,   172,   171,     0,   176,   175,   177,
       0,     0,   190,     0,     0,   202,   203,     0,     0,   199,
     205,     0,   208,   219,   220,   103,    37,   143,   142,     0,
       0,     0,     0,     0,     0,     0,    54,     0,    56,    55,
      19,     0,    68,   192,   193,     0,     0,   112,     0,     0,
       0,   115,     0,   126,     0,     0,     0,    33,    32,     0,
       0,     0,     0,     0,   144,     0,     0,     0,   129,    74,
      76,   107,     0,    25,     0,   140,   141,     0,     0,   148,
     150,   152,   154,   156,   160,   166,   170,   174,   179,   181,
     195,   207,     0,     0,   206,     0,   104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
     117,     0,    22,   130,   125,     0,     0,     0,   122,   197,
       0,   134,    50,   126,   126,     0,     9,     0,   138,     0,
      77,   200,     0,   105,    65,    58,     0,     0,    62,    64,
       0,     0,    57,    20,    51,     0,     0,     0,     0,     0,
     119,     0,     0,   124,     0,   111,   135,     0,   132,   131,
     126,     0,     0,     0,   146,   201,     0,     0,     0,    60,
       0,    52,     0,   113,     0,   116,   120,    34,    29,   136,
     133,     8,   121,     0,    66,    59,    63,    61,    30,   118,
       0,    18,     0,    12,   121,    16,    17,   123,    35,     7,
      15
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -346,   275,  -346,  -345,  -346,  -346,  -346,   -68,  -346,  -113,
    -120,    -6,  -346,  -346,  -346,  -107,   -63,  -119,    -5,     6,
    -346,  -346,  -346,  -346,  -346,  -346,  -346,  -346,   -86,     9,
    -346,  -346,  -346,   -60,  -109,   153,  -346,  -327,  -346,  -341,
    -124,   -24,    12,  -346,  -116,   -21,  -346,  -346,  -346,     3,
    -346,   146,   147,   148,   149,  -346,   145,  -346,   150,  -346,
     139,  -346,   138,  -346,   141,   132,  -346,   257,  -346,   -98,
    -346,  -346,  -346,   -12,   325
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    46,    47,    48,   362,   182,   392,   393,   394,   241,
     129,    49,   274,   183,    50,   132,   351,    51,    52,   345,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,   219,   176,    64,    65,    66,    67,   316,    68,    69,
     145,    70,   263,    71,   185,   186,    72,    73,    74,    75,
      76,    77,    78,    79,    80,   195,    81,   200,    82,   203,
      83,   206,    84,   210,    85,    86,    87,    88,    89,   136,
     221,   294,   222,    90,    91
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     100,   120,   244,   124,   127,   178,   112,   105,   247,   131,
     133,   251,    94,    96,    98,   245,   101,   102,   268,   265,
     266,   259,   235,   103,   258,   246,   143,   378,   250,   116,
     220,   350,  -198,   143,   214,   187,   383,   391,   144,   140,
     179,   396,   104,   193,   106,   388,   148,   138,   107,   391,
     194,   108,   220,   396,   214,   196,   197,   198,   199,    29,
     243,   109,   231,     3,   232,     4,   180,     5,   124,   110,
     215,   216,   141,   257,   238,   276,   142,   188,   114,   217,
       7,     8,   117,     2,    29,     3,   118,     4,   121,     5,
     215,   216,    39,    40,   204,   205,   119,   130,   133,   217,
    -126,   249,     7,     8,   201,   202,   181,  -126,   128,   134,
       9,    29,   229,   230,   218,   233,   234,    39,    40,   237,
     292,   119,   135,   122,   275,    23,    24,    25,    26,   313,
     137,   139,   312,   310,   218,   314,   146,   149,    29,   123,
     177,   184,    30,    31,    39,    40,   262,   325,   119,   303,
     309,    38,   189,   190,    41,    42,   111,    44,    45,   326,
     269,   328,   319,   191,   192,   296,   123,   327,   207,   208,
     209,    39,    40,    38,   211,   119,    41,    42,   111,    44,
      45,   213,   223,   224,   225,   342,    98,   289,   227,   226,
     228,   236,   343,   150,   151,   152,   278,   353,   178,   358,
     359,   290,   239,   240,   355,   242,    29,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   178,   125,   214,   143,
     253,     6,   252,    98,   254,   255,   256,     5,    29,   260,
     220,   373,   261,   375,   248,   264,   380,   320,   267,    39,
      40,   381,   273,   119,   277,   272,   293,   291,   299,   295,
     133,   162,   308,   297,   298,   300,   126,   301,   389,   302,
     304,    39,    40,   305,   397,   119,   306,   307,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   311,   315,   317,   347,   321,   322,   349,   318,   323,
     329,   352,   334,   335,   324,   330,   338,   339,   344,   346,
     354,   348,   332,   360,    98,   365,   361,   367,   336,   337,
     363,   366,   341,   368,   370,   371,   372,   376,   379,   385,
     386,   382,   147,   390,   374,   399,   400,   398,   357,   270,
     262,   377,   364,   356,   279,   369,   280,   283,   281,   285,
     282,   286,   288,   113,   212,   284,     0,   287,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    -2,
       0,   384,     1,     0,     0,   387,   352,     2,     0,     3,
       0,     4,     0,     5,     0,     6,   395,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,   395,     0,
       0,     0,     0,     0,     9,    10,     0,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,     0,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    30,    31,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,     0,     0,
       0,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,    35,     5,    36,     6,   -13,    37,    38,    39,    40,
      41,    42,    43,    44,    45,     7,     8,     0,     0,     0,
       0,     0,     0,     9,    10,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,     0,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     0,     0,     0,
       0,     0,     0,     0,    32,    33,    34,     0,     0,     0,
       1,     0,     0,     0,     0,     2,     0,     3,     0,     4,
      35,     5,    36,     6,   -14,    37,    38,    39,    40,    41,
      42,    43,    44,    45,     7,     8,     0,     0,     0,     0,
       0,     0,     9,    10,     0,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,     0,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     0,     0,     0,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,    35,
       5,    36,     6,     0,    37,    38,    39,    40,    41,    42,
      43,    44,    45,     7,     8,     0,     0,     0,     0,     0,
       0,     9,    10,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,     0,    28,     0,     0,     0,     0,     0,     0,
       0,     0,    29,    30,    31,     0,     0,     0,     0,     0,
       0,     0,    32,    33,    34,     0,     0,     0,     1,     0,
       0,     0,     0,     2,     0,     3,     0,     4,    35,     5,
      36,     6,    99,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     7,     8,     0,     0,     0,     0,     0,     0,
       9,    10,     0,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,     0,    28,     0,     0,     0,     0,     0,     0,     0,
       0,    29,    30,    31,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,     0,     0,     1,     0,     0,
       0,     0,     2,     0,     3,     0,     4,     0,     5,    36,
       6,     0,    37,    38,    39,    40,    41,    42,    43,    44,
      45,     7,     8,     0,     0,     0,     0,     0,     0,     9,
      10,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
       0,    28,     0,     0,     0,     0,     0,     0,   115,     0,
      29,    30,    31,     2,     0,     3,     0,     4,     0,     5,
       0,     0,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,    36,     0,
       9,    37,    38,    39,    40,    41,    42,    43,    44,    45,
       0,     0,     0,     0,     0,    23,    24,    25,    26,     0,
      92,     0,    28,     0,     0,     0,     0,     0,     0,     1,
       0,    29,    30,    31,     2,     0,     3,     0,     4,     0,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,    36,
       0,     9,     0,    38,    39,    40,    41,    42,    93,    44,
      45,     0,     0,     0,     0,     0,    23,    24,    25,    26,
       0,    92,     0,    28,     0,     0,     0,     0,     0,     0,
       0,     0,    29,    30,    31,     2,     0,     3,     0,     4,
      95,     5,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
      36,     0,     9,     0,    38,    39,    40,    41,    42,    93,
      44,    45,     0,     0,     0,     0,     0,    23,    24,    25,
      26,     0,    92,     0,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     2,     0,     3,     0,
       4,     0,     5,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,    36,     0,     9,     0,    38,    39,    40,    41,    42,
      93,    44,    45,     0,     0,     0,     0,     0,    23,    24,
      25,    26,     0,    92,     0,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     2,     0,     3,
       0,     4,     0,     5,   271,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,    36,     0,     9,     0,    38,    39,    40,    41,
      42,    93,    44,    45,     0,     0,     0,     0,     0,    23,
      24,    25,    26,     0,    92,     0,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    30,    31,     2,     0,
       3,     0,     4,   331,     5,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,    36,     0,     9,     0,    38,    39,    40,
      41,    42,    93,    44,    45,     0,     0,     0,     0,     0,
      23,    24,    25,    26,     0,    92,     0,    28,     0,     0,
       0,     0,     0,     0,     0,     0,    29,    30,    31,     2,
       0,     3,     0,     4,     0,     5,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
       0,     0,     0,     0,    36,     0,     9,     0,    38,    39,
      40,    41,    42,    93,    44,    45,     0,     0,     0,     0,
       0,    23,    24,    25,    26,     0,    92,     0,    28,     0,
       0,     0,     0,     0,     0,     0,     0,    29,    30,    31,
       2,     0,     3,     0,     4,   340,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,    36,     0,     9,     0,    38,
      39,    40,    41,    42,    93,    44,    45,     0,     0,     0,
       0,     0,    23,    24,    25,    26,     0,    92,     0,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    29,    30,
      31,     2,     0,     3,     0,     4,     0,     5,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,    36,     0,     9,     0,
      38,    39,    40,    41,    42,    93,    44,    45,     0,     0,
       0,     0,     0,    23,    24,    25,    26,     0,    92,     0,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    29,
      30,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,     0,     0,
       0,    38,    39,    40,    41,    42,    93,    44,    45
};

static const yytype_int16 yycheck[] =
{
       6,    25,   121,    27,    28,    65,    18,    12,   124,    33,
      34,   127,     3,     4,     5,   122,     7,     8,   142,   139,
     140,   134,   108,    12,   133,   123,    29,   354,   126,    20,
      90,    13,    12,    29,    14,    33,   363,   382,    34,    66,
      30,   382,    12,    62,    12,   372,    52,    38,    12,   394,
      69,    12,   112,   394,    14,    63,    64,    65,    66,    66,
     120,   103,    38,    10,    40,    12,    56,    14,    92,    12,
      50,    51,    99,   133,    34,   184,   103,    75,    16,    59,
      27,    28,     3,     8,    66,    10,     3,    12,    32,    14,
      50,    51,    99,   100,    67,    68,   103,    12,   122,    59,
     103,   125,    27,    28,    70,    71,    96,   103,   103,    94,
      35,    66,   103,   104,    94,   106,   107,    99,   100,   110,
     218,   103,   103,    78,   184,    50,    51,    52,    53,   253,
      16,    12,   252,   249,    94,   254,     0,     3,    66,    94,
      14,   103,    67,    68,    99,   100,   137,   267,   103,   235,
     248,    98,    61,    73,   101,   102,   103,   104,   105,   272,
     151,   277,   260,    74,    72,   225,    94,   274,     7,     8,
       9,    99,   100,    98,    31,   103,   101,   102,   103,   104,
     105,    60,    11,    13,    15,   304,   177,   211,    13,    17,
      13,     3,   305,     4,     5,     6,   187,   316,   258,   323,
     324,   213,    34,   103,   320,     3,    66,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   276,    55,    14,    29,
      64,    16,    59,   214,    13,   103,    12,    14,    66,     5,
     290,   347,    95,   349,    94,    13,   360,   261,    12,    99,
     100,   361,    55,   103,     5,    54,    12,   103,    12,    14,
     274,    62,    95,    13,    13,    12,    94,    13,   374,    13,
      13,    99,   100,     5,   383,   103,    17,    12,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    95,    12,   103,   308,     3,    17,   311,    95,    13,
      34,   315,   297,   298,    64,    95,   301,   302,   103,   103,
      13,    95,   293,    13,   295,    13,    58,    13,   299,   300,
     103,    37,   303,    13,    13,     3,    13,    13,     3,     3,
       3,    16,    47,     5,   348,    17,   394,   390,   322,   176,
     321,   352,   329,   321,   188,   340,   189,   192,   190,   200,
     191,   203,   210,    18,    87,   195,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
      -1,   366,     3,    -1,    -1,   370,   390,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    16,   382,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,   394,    -1,
      -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    77,    78,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    92,    14,    94,    16,    17,    97,    98,    99,   100,
     101,   102,   103,   104,   105,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    77,    78,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,
      92,    14,    94,    16,    17,    97,    98,    99,   100,   101,
     102,   103,   104,   105,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    77,    78,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    92,
      14,    94,    16,    -1,    97,    98,    99,   100,   101,   102,
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
      -1,    -1,    -1,    78,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    12,    -1,    14,    94,
      16,    -1,    97,    98,    99,   100,   101,   102,   103,   104,
     105,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      66,    67,    68,     8,    -1,    10,    -1,    12,    -1,    14,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    94,    -1,
      35,    97,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    66,    67,    68,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    94,
      -1,    35,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,     8,    -1,    10,    -1,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      94,    -1,    35,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,
      53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,     8,    -1,    10,    -1,
      12,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,     8,
      -1,    10,    -1,    12,    -1,    14,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    94,    -1,    35,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,
      -1,    50,    51,    52,    53,    -1,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    94,    -1,    35,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,     8,    -1,    10,    -1,    12,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    94,    -1,    35,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105
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
     180,   181,    55,   103,   136,    13,   136,    15,   136,    17,
     118,   136,   136,    12,    12,   125,    12,    12,    12,   103,
      12,   103,   180,   181,    16,     3,   136,     3,     3,   103,
     148,    32,    78,    94,   148,    55,    94,   148,   103,   117,
      12,   148,   122,   148,    94,   103,   176,    16,   136,    12,
      66,    99,   103,    29,    34,   147,     0,   108,   118,     3,
       4,     5,     6,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    62,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,   139,    14,   140,    30,
      56,    96,   112,   120,   103,   151,   152,    33,    75,    61,
      73,    74,    72,    62,    69,   162,    63,    64,    65,    66,
     164,    70,    71,   166,    67,    68,   168,     7,     8,     9,
     170,    31,   174,    60,    14,    50,    51,    59,    94,   138,
     140,   177,   179,    11,    13,    15,    17,    13,    13,   136,
     136,    38,    40,   136,   136,   135,     3,   136,    34,    34,
     103,   116,     3,   140,   124,   122,   176,   151,    94,   148,
     176,   151,    59,    64,    13,   103,    12,   140,   141,   116,
       5,    95,   136,   149,    13,   117,   117,    12,   147,   136,
     142,    15,    54,    55,   119,   140,   141,     5,   136,   158,
     159,   160,   161,   163,   165,   167,   169,   171,   172,   148,
     180,   103,   176,    12,   178,    14,   140,    13,    13,    12,
      12,    13,    13,   135,    13,     5,    17,    12,    95,   176,
     151,    95,   117,   147,   124,    12,   144,   103,    95,   176,
     148,     3,    17,    13,    64,   117,   116,   122,   151,    34,
      95,    13,   136,    15,   125,   125,   136,   136,   125,   125,
      13,   136,   124,   116,   103,   126,   103,   148,    95,   148,
      13,   123,   148,   124,    13,   151,   149,   126,   147,   147,
      13,    58,   111,   103,   156,    13,    37,    13,    13,   125,
      13,     3,    13,   151,   148,   151,    13,   152,   144,     3,
     147,   117,    16,   144,   125,     3,     3,   125,   144,   151,
       5,   110,   113,   114,   115,   118,   146,   124,   123,    17,
     114
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
     144,   145,   145,   146,   146,   146,   147,   147,   148,   148,
     148,   148,   148,   148,   149,   149,   150,   151,   151,   152,
     152,   152,   153,   154,   155,   156,   156,   157,   157,   158,
     158,   159,   159,   160,   160,   161,   161,   162,   162,   163,
     163,   164,   164,   164,   164,   165,   165,   166,   166,   167,
     167,   168,   168,   169,   169,   170,   170,   170,   171,   171,
     172,   172,   173,   173,   173,   173,   173,   173,   173,   174,
     174,   174,   174,   174,   175,   175,   176,   176,   177,   177,
     178,   178,   179,   179,   180,   180,   180,   180,   180,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181
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
       3,     0,     4,     7,     5,     4,     0,     1,     2,     3,
       4,     5,     5,     6,     2,     3,     6,     1,     3,     1,
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
          case 122: /* type_decl2  */
#line 149 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1730 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 148: /* type_decl  */
#line 149 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1736 "src/ast/parser.c" /* yacc.c:1257  */
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
#line 2004 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 154 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(scanner)); }
#line 2010 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 158 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2016 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 159 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(scanner)); }
#line 2022 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 160 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(scanner)); }
#line 2028 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 165 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].id_list), (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->types = (yyvsp[-7].id_list); }
#line 2034 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 168 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[0].id_list); }
#line 2040 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 168 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2046 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 171 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_public; }
#line 2052 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 172 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_private; }
#line 2058 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 176 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2064 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 177 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2070 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 181 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body( (yyvsp[0].section), get_pos(scanner)); }
#line 2076 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 182 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body( (yyvsp[-1].section), (yyvsp[0].class_body), get_pos(scanner)); }
#line 2082 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 186 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list( (yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2088 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 187 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def( (yyvsp[0].func_def), get_pos(scanner)); }
#line 2094 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 188 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def( (yyvsp[0].class_def), get_pos(scanner)); }
#line 2100 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 192 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2106 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 193 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2112 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 197 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2118 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 198 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2124 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 202 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(scanner));}
#line 2130 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 203 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(scanner));}
#line 2136 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 207 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_static;   }
#line 2142 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 208 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_instance; }
#line 2148 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 212 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_func; }
#line 2154 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 213 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2160 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 217 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-2].sval), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(scanner)); }
#line 2166 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 218 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_flag_static, (yyvsp[-2].sval), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(scanner)); }
#line 2172 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 222 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2178 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 223 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2184 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 224 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2190 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 228 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(scanner)); }
#line 2196 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 229 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(scanner)); }
#line 2202 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 233 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( NULL, get_pos(scanner)); }
#line 2208 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 234 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( (yyvsp[-1].stmt_list), get_pos(scanner)); }
#line 2214 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 253 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sval) = NULL; }
#line 2220 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 254 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sval) = (yyvsp[0].sval);  }
#line 2226 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 258 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sval), get_pos(scanner)); }
#line 2232 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 262 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 1, get_pos(scanner)); }
#line 2238 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 266 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 0, get_pos(scanner)); }
#line 2244 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 270 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2250 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 271 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2256 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 275 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner));}
#line 2262 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2268 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2274 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2280 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 286 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2286 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 288 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2292 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 290 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2298 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 292 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop( (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2304 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 297 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(scanner)); }
#line 2310 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 299 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2316 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 303 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( NULL, get_pos(scanner)); }
#line 2322 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 304 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2328 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 305 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_break(get_pos(scanner)); }
#line 2334 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 306 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_continue(get_pos(scanner)); }
#line 2340 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 310 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_expression((yyvsp[-1].exp),   get_pos(scanner)); }
#line 2346 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 311 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_expression(NULL, get_pos(scanner)); }
#line 2352 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 315 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2358 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 316 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_expression((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2364 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 320 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2370 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 321 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2376 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 325 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2382 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 329 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2388 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 330 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2394 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 331 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; /* LCOV_EXCL_LINE */ }
#line 2400 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 332 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2406 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 333 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2412 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 334 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2418 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 335 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2424 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 336 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2430 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 337 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2436 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 338 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2442 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 339 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2448 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 340 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2454 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 341 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2460 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 342 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2466 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 343 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2472 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 344 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2478 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 345 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2484 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 346 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2490 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 347 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2496 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 348 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2502 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 349 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2508 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 350 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2514 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 351 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2520 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 352 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2526 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 353 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2532 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 357 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( (yyvsp[-1].exp), get_pos(scanner) ); }
#line 2538 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 358 "utils/gwion.y" /* yacc.c:1646  */
    { if((yyvsp[-2].exp)->next){ yyerror(&scanner, "invalid format for array init [...][...]...");YYERROR; } (yyval.array_sub) = prepend_array_sub( (yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2544 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 359 "utils/gwion.y" /* yacc.c:1646  */
    { yyerror(&scanner, "partially empty array init [...][]..."); YYERROR; }
#line 2550 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 363 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(scanner)); }
#line 2556 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 364 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2562 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 365 "utils/gwion.y" /* yacc.c:1646  */
    { yyerror(&scanner, "partially empty array init [][...]"); YYERROR; }
#line 2568 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 370 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); }
#line 2574 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 371 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); (yyval.exp)->d.exp_decl.types = (yyvsp[-3].type_list); }
#line 2580 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 372 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); }
#line 2586 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 373 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); (yyval.exp)->d.exp_decl.types = (yyvsp[-3].type_list); }
#line 2592 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 378 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); SET_FLAG((yyval.exp)->d.exp_decl.type, ae_flag_private); }
#line 2598 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 379 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); (yyval.exp)->d.exp_decl.types = (yyvsp[-3].type_list); }
#line 2604 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 380 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); }
#line 2610 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 381 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); (yyval.exp)->d.exp_decl.types = (yyvsp[-3].type_list); }
#line 2616 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 385 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2622 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 386 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2628 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 389 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2634 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 389 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2640 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 393 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->types = (yyvsp[-6].id_list); if((yyvsp[-6].id_list)) SET_FLAG((yyval.func_def), ae_flag_template);}
#line 2646 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 395 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_func | ae_flag_static | ae_flag_op , (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2652 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 397 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_func | ae_flag_instance | ae_flag_dtor, new_type_decl(new_id_list("void", get_pos(scanner)), 0,
      get_pos(scanner)), "dtor", NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2659 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 401 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2665 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 401 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 2671 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 404 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), (yyvsp[0].ival), get_pos(scanner)); }
#line 2677 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 405 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), (yyvsp[0].ival), get_pos(scanner)); SET_FLAG((yyval.type_decl), ae_flag_const); }
#line 2683 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 406 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scanner)); }
#line 2689 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 407 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scanner)); SET_FLAG((yyval.type_decl), ae_flag_const); }
#line 2695 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 408 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scanner)); }
#line 2701 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 409 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scanner)); SET_FLAG((yyval.type_decl), ae_flag_const); }
#line 2707 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 414 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2713 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 415 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2719 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 419 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(scanner));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sval) ? insert_symbol((yyvsp[-1].sval)) : NULL; }
#line 2725 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 423 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(scanner)); }
#line 2731 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 424 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(scanner)); }
#line 2737 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 428 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sval), NULL, get_pos(scanner)); }
#line 2743 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 429 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2749 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 430 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2755 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 433 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2761 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 434 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar(   (yyvsp[-1].exp), get_pos(scanner)); }
#line 2767 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 435 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec(     (yyvsp[-1].exp), get_pos(scanner)); }
#line 2773 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 440 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_if( (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2779 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 444 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2785 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 446 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2791 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 450 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2797 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 452 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2803 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 456 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2809 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 458 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2815 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 462 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2821 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 464 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(scanner)); }
#line 2827 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 468 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2833 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 470 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2839 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 473 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2845 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 473 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_neq; }
#line 2851 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 475 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2857 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 477 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2863 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 480 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_lt; }
#line 2869 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 480 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_gt; }
#line 2875 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 480 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_le; }
#line 2881 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 480 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ge; }
#line 2887 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 483 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2893 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 485 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2899 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 489 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 2905 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 490 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 2911 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 494 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2917 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 496 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2923 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 499 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2929 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 499 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 2935 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 502 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2941 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 504 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2947 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 507 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 2953 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 507 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide; }
#line 2959 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 507 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_percent; }
#line 2965 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 510 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2971 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 512 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2977 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 518 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_cast( (yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2983 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 521 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2989 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 521 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 2995 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 521 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3001 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 522 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3007 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 522 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3013 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 523 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3019 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 523 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3025 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 527 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3031 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 529 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary( (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 3037 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 531 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(scanner)); }
#line 3043 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 533 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary2(op_new, (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3049 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 535 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary3( op_spork, (yyvsp[0].stmt), get_pos(scanner)); }
#line 3055 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 541 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 3061 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 545 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sval), get_pos(scanner)), NULL, get_pos(scanner)); }
#line 3067 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 546 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sval), get_pos(scanner)), (yyvsp[0].type_list), get_pos(scanner)); }
#line 3073 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 549 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 3079 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 549 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[0].type_list);}
#line 3085 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 550 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 3091 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 550 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3097 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 554 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3103 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 554 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3109 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 559 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array( (yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3115 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 561 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_call( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); (yyval.exp)->d.exp_func.types = (yyvsp[-1].type_list); }
#line 3121 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 563 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dot( (yyvsp[-2].exp), (yyvsp[0].sval), get_pos(scanner)); }
#line 3127 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 565 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_postfix( (yyvsp[-1].exp), (yyvsp[0].ival), get_pos(scanner)); }
#line 3133 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 569 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_ID(      (yyvsp[0].sval), get_pos(scanner)); }
#line 3139 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 570 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_int(     (yyvsp[0].ival), get_pos(scanner)); }
#line 3145 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 571 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_float(   (yyvsp[0].fval), get_pos(scanner)); }
#line 3151 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 572 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_string(  (yyvsp[0].sval), get_pos(scanner)); }
#line 3157 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 573 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_char(    (yyvsp[0].sval), get_pos(scanner)); }
#line 3163 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 574 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3169 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 575 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3175 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 576 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_complex( (yyvsp[0].c_val), get_pos(scanner)); }
#line 3181 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 217:
#line 577 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_polar(   (yyvsp[0].polar), get_pos(scanner)); }
#line 3187 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 218:
#line 578 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_vec(     (yyvsp[0].vec), get_pos(scanner)); }
#line 3193 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 219:
#line 579 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_hack(    (yyvsp[-1].exp), get_pos(scanner)); }
#line 3199 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 220:
#line 580 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) =                       (yyvsp[-1].exp);                    }
#line 3205 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 221:
#line 581 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_nil(         get_pos(scanner)); }
#line 3211 "src/ast/parser.c" /* yacc.c:1646  */
    break;


#line 3215 "src/ast/parser.c" /* yacc.c:1646  */
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
#line 583 "utils/gwion.y" /* yacc.c:1906  */

