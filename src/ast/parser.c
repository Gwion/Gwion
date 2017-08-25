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
#define YYFINAL  137
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1305

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  105
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  73
/* YYNRULES -- Number of rules.  */
#define YYNRULES  212
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  371

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
     367,   368,   369,   373,   374,   377,   377,   380,   382,   384,
     389,   389,   392,   393,   394,   399,   400,   404,   408,   409,
     413,   414,   415,   418,   419,   420,   423,   424,   429,   430,
     435,   436,   441,   442,   447,   448,   453,   454,   458,   458,
     460,   461,   465,   465,   465,   465,   468,   469,   474,   475,
     479,   480,   484,   484,   487,   488,   492,   492,   492,   495,
     496,   501,   502,   506,   506,   506,   507,   507,   508,   508,
     512,   513,   515,   517,   519,   525,   526,   530,   531,   534,
     534,   535,   535,   539,   539,   542,   543,   545,   547,   549,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     564,   565,   566
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

#define YYPACT_NINF -312

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-312)))

#define YYTABLE_NINF -190

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     566,  -312,  -312,  1202,   842,   902,   644,  1202,  1202,  -312,
      34,    55,   722,    71,    75,    80,   -69,    84,   215,    98,
     156,   117,   120,  -312,  -312,    21,   101,   -27,    33,  -312,
    -312,   123,    21,    21,    43,    36,   125,  1202,   130,  -312,
    -312,    -4,  -312,  -312,   143,   332,  -312,  -312,  -312,  -312,
     722,  -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,
     141,  1199,  -312,   131,  -312,    -7,  -312,    45,  -312,  -312,
    -312,  -312,  -312,    -2,    87,    77,    78,    81,     2,    42,
      62,    23,    93,   122,    27,  -312,    96,    10,  -312,   -17,
      -3,   146,  -312,   147,  -312,   148,  -312,   144,   149,   152,
    1202,  1202,    35,  1202,  1202,   782,   164,  1202,  -312,     6,
     135,    72,  -312,   171,  -312,  -312,   150,   161,   166,    21,
      36,    45,   119,   126,   167,    89,   173,   174,    72,   188,
     102,  1202,   182,    33,  -312,  -312,  -312,  -312,  -312,  -312,
    -312,  -312,  1202,  -312,  -312,  -312,  -312,  -312,  -312,  -312,
    -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,  -312,
    -312,  -312,  -312,  -312,  -312,  -312,  -312,  1202,   962,  -312,
    -312,  -312,  -312,   145,   142,   174,  -312,   193,  1202,    27,
      27,    27,    27,    27,  -312,  -312,    27,  -312,  -312,  -312,
    -312,    27,  -312,  -312,    27,  -312,  -312,    27,  -312,  -312,
    -312,    27,    21,  -312,   215,  1202,  -312,  -312,    99,    36,
    -312,  -312,   189,  -312,  -312,  -312,   190,  -312,  -312,  -312,
     192,   199,   191,   202,   204,   205,   782,  -312,   207,  -312,
    -312,   210,   209,  -312,  -312,  -312,   216,   136,  -312,    33,
     150,   166,   219,   132,  -312,   131,   138,    36,    21,   232,
     220,  -312,   223,  -312,  -312,  -312,    72,  -312,    21,  -312,
     131,    45,   206,    87,    77,    78,    81,     2,    42,    62,
      23,    93,   122,  -312,    10,  -312,   151,  1022,  -312,  1082,
    -312,   722,   722,  1202,  1202,   722,   722,  1142,   166,    72,
      15,   140,    21,  -312,  -312,  -312,    20,   166,   226,  -312,
    -312,    45,  1202,   241,   150,   194,   159,  -312,    27,  -312,
    -312,   233,  -312,   211,  -312,   234,   237,  -312,  -312,   722,
     239,  -312,  -312,  -312,   258,   249,    45,  -312,   250,    45,
    -312,   219,  -312,  -312,  -312,  -312,    33,   248,   219,  -312,
    -312,   722,   262,   263,  -312,   722,  -312,   219,  -312,  -312,
     264,  -312,  -312,   410,   166,  -312,  -312,  -312,  -312,  -312,
      21,  -312,   253,  -312,   488,  -312,  -312,  -312,  -312,  -312,
    -312
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     115,    71,   175,     0,     0,     0,     0,     0,     0,   178,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   176,   177,     0,     0,     0,     0,   174,
     173,     0,     0,     0,     0,     0,     0,     0,     0,   201,
     202,   200,   203,   204,     0,   115,     6,     4,    48,    41,
      23,    46,    42,    43,    45,    44,    39,    40,    47,    38,
       0,    72,   205,   206,    74,    11,     5,     0,    49,   207,
     208,   209,   108,   136,   138,   140,   142,   144,   146,   150,
     156,   160,   164,   169,     0,   171,   180,   185,   195,     0,
     200,     0,   212,     0,   105,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   200,   189,
     195,     0,    66,     0,    68,    69,   120,   182,   179,     0,
       0,     0,    21,     0,     0,     0,     0,    31,     0,   187,
       0,     0,     0,     0,   121,    52,   122,     1,     3,    24,
      70,    77,     0,   101,    81,    82,    83,    84,    85,    80,
      78,    86,    87,    79,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,     0,     0,   107,
      27,    10,    28,     0,    26,   130,   109,   128,     0,     0,
       0,     0,     0,     0,   148,   149,     0,   155,   153,   154,
     152,     0,   158,   159,     0,   163,   162,     0,   167,   166,
     168,     0,     0,   181,     0,     0,   193,   194,     0,     0,
     190,   196,     0,   199,   210,   211,   102,    37,   134,   133,
       0,     0,     0,     0,     0,     0,     0,    53,     0,    55,
      54,    19,     0,    67,   183,   184,     0,     0,   111,     0,
     120,     0,     0,     0,    33,    32,     0,     0,     0,     0,
       0,   135,     0,    73,    75,   106,     0,    25,     0,   131,
     132,     0,     0,   139,   141,   143,   145,   147,   151,   157,
     161,   165,   170,   172,   186,   198,     0,     0,   197,     0,
     103,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,   123,   119,     0,     0,     0,   116,
     188,     0,   125,     0,   120,     9,     0,   129,     0,    76,
     191,     0,   104,    64,    57,     0,     0,    61,    63,     0,
       0,    56,    20,    50,     0,     0,     0,   113,     0,     0,
     118,     0,   110,   126,   127,   124,     0,     0,     0,   137,
     192,     0,     0,     0,    59,     0,    51,     0,   112,   114,
      34,    29,     8,   115,     0,    65,    58,    62,    60,    30,
       0,    18,     0,    12,   115,    16,    17,   117,    35,     7,
      15
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -312,   222,  -312,  -311,  -312,  -312,  -312,   -89,  -312,  -118,
    -126,    -6,  -312,  -312,  -312,  -102,   -84,  -116,    -8,  -312,
    -312,  -312,  -312,  -312,  -312,  -312,  -312,   -86,     8,  -312,
    -312,  -312,   -58,   -93,   104,  -295,  -312,  -306,  -213,   -24,
     -23,  -312,  -107,   -49,  -312,  -312,  -312,   -26,  -312,   105,
     103,   100,   106,  -312,   107,  -312,   108,  -312,    95,  -312,
     109,  -312,    92,    97,  -312,   212,  -312,   -99,  -312,  -312,
    -312,   -12,   275
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    44,    45,    46,   337,   173,   362,   363,   364,   232,
     123,    47,   258,   174,    48,   126,   328,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
     210,   167,    62,    63,    64,   297,    65,    66,   136,    67,
     250,    68,   176,   177,    69,    70,    71,    72,    73,    74,
      75,    76,    77,   186,    78,   191,    79,   194,    80,   197,
      81,   201,    82,    83,    84,    85,    86,   130,   212,   278,
     213,    87,    88
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      97,   117,   235,   121,   102,   169,   109,   252,   125,   127,
     246,    91,    93,    95,   238,    98,    99,   236,   323,   226,
     205,   237,  -189,   170,   205,   134,   134,   294,   113,   211,
     135,   178,   106,   327,   245,     2,   351,     3,    28,     4,
     229,     5,   361,   354,   139,   132,   100,   366,    28,   171,
     119,   211,   359,   361,     7,     8,   206,   207,   366,   234,
     206,   207,     9,   184,   208,   121,   120,   101,   208,   244,
     185,    38,   179,   222,   116,   223,   120,    23,    24,    25,
      26,    38,   260,   103,   116,    28,    28,   104,   172,   195,
     196,   335,   105,    29,    30,   127,   107,  -120,  -120,   209,
     198,   199,   200,   209,   187,   188,   189,   190,   220,   221,
     276,   224,   225,   293,   111,   228,   324,   259,    38,    38,
     114,   116,   116,   115,    37,   295,    39,    40,   108,    42,
      43,   192,   193,   118,   122,   124,   128,   129,   305,   249,
     287,   131,   133,   137,   140,   168,   175,   180,   300,   181,
     253,   182,   183,   202,   307,   204,   306,   214,   280,   112,
     215,   217,   218,   216,     2,   219,     3,   227,     4,   230,
       5,   322,   321,   231,   233,   205,    95,   239,   273,   134,
     241,   330,     6,     7,     8,   243,   262,   169,     5,   240,
     242,     9,   274,   247,   332,   251,   248,   257,   261,   256,
     275,   277,   169,   283,   279,   281,    23,    24,    25,    26,
     352,    89,   282,    95,   284,   289,   211,   285,   286,   348,
     288,    28,    29,    30,   301,     3,   290,     4,   291,     5,
     292,   296,   299,   298,   127,   302,   304,   303,   367,   331,
     308,   325,     7,     8,   334,   309,   340,   342,   341,    35,
     343,   336,   345,    37,    38,    39,    40,    90,    42,    43,
     338,   346,   347,   349,   353,   356,   357,   138,   326,   360,
     369,   254,   329,   313,   314,   370,   368,   317,   318,   333,
     350,   265,   339,   264,   263,   311,   269,    95,   266,   271,
     267,   315,   316,   110,   268,   320,   203,     0,   272,     0,
       0,     0,     0,   270,     0,     0,     0,     0,     0,     0,
     249,   344,    37,     0,    39,    40,   108,    42,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    -2,   355,     0,     1,   329,   358,     0,     0,
       2,     0,     3,     0,     4,     0,     5,   365,     6,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   365,     7,
       8,     0,     0,     0,     0,     0,     0,     9,    10,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,    29,    30,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     1,     0,     0,     0,     0,     2,     0,
       3,     0,     4,    34,     5,    35,     6,   -13,    36,    37,
      38,    39,    40,    41,    42,    43,     0,     7,     8,     0,
       0,     0,     0,     0,     0,     9,    10,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    28,    29,    30,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    33,     0,     0,
       0,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,    34,     5,    35,     6,   -14,    36,    37,    38,    39,
      40,    41,    42,    43,     0,     7,     8,     0,     0,     0,
       0,     0,     0,     9,    10,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    28,    29,    30,     0,     0,     0,     0,
       0,     0,     0,    31,    32,    33,     0,     0,     0,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,    34,
       5,    35,     6,     0,    36,    37,    38,    39,    40,    41,
      42,    43,     0,     7,     8,     0,     0,     0,     0,     0,
       0,     9,    10,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    29,    30,     0,     0,     0,     0,     0,     0,
       0,    31,    32,    33,     0,     0,     0,     1,     0,     0,
       0,     0,     2,     0,     3,     0,     4,    34,     5,    35,
       6,    96,    36,    37,    38,    39,    40,    41,    42,    43,
       0,     7,     8,     0,     0,     0,     0,     0,     0,     9,
      10,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      29,    30,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    33,     0,     0,     0,     1,     0,     0,     0,     0,
       2,     0,     3,     0,     4,     0,     5,    35,     6,     0,
      36,    37,    38,    39,    40,    41,    42,    43,     0,     7,
       8,     0,     0,     0,     0,     0,     0,     9,    10,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,     0,     0,
       0,     0,     0,     0,     0,     1,     0,    28,    29,    30,
       2,     0,     3,     0,     4,     0,     5,     0,     0,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,    35,     0,     9,    36,    37,
      38,    39,    40,    41,    42,    43,     0,     0,     0,     0,
       0,     0,    23,    24,    25,    26,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,    29,    30,
       2,     0,     3,     0,     4,    92,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,    35,     0,     9,     0,    37,
      38,    39,    40,    90,    42,    43,     0,     0,     0,     0,
       0,     0,    23,    24,    25,    26,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,    29,    30,
       2,     0,     3,     0,     4,     0,     5,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,    35,     0,     9,     0,    37,
      38,    39,    40,    90,    42,    43,     0,     0,     0,     0,
       0,     0,    23,    24,    25,    26,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,    29,    30,
       2,     0,     3,     0,     4,     0,     5,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,    35,     0,     9,     0,    37,
      38,    39,    40,    90,    42,    43,     0,     0,     0,     0,
       0,     0,    23,    24,    25,    26,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,    29,    30,
       2,     0,     3,     0,     4,   310,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,    35,     0,     9,     0,    37,
      38,    39,    40,    90,    42,    43,     0,     0,     0,     0,
       0,     0,    23,    24,    25,    26,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,    29,    30,
       2,     0,     3,     0,     4,     0,     5,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,    35,     0,     9,     0,    37,
      38,    39,    40,    90,    42,    43,     0,     0,     0,     0,
       0,     0,    23,    24,    25,    26,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,    29,    30,
       2,     0,     3,     0,     4,   319,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,    35,     0,     9,     0,    37,
      38,    39,    40,    90,    42,    43,     0,     0,     0,     0,
       0,     0,    23,    24,    25,    26,     0,    89,     0,     0,
       0,     0,     0,   141,   142,   143,     0,    28,    29,    30,
       2,     0,     3,     0,     4,     0,     5,   144,   145,   146,
     147,   148,   149,   150,   151,   152,     0,     0,     0,     7,
       8,     0,     0,     0,     0,    35,     0,     9,     0,    37,
      38,    39,    40,    90,    42,    43,     0,     0,     0,     0,
       0,     0,    23,    24,    25,    26,     0,    89,     0,     0,
     153,     0,     0,     0,     0,     0,     0,    28,    29,    30,
       0,     0,     0,     0,     0,     0,     0,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
       0,     0,     0,     0,     0,    35,     0,     0,     0,    37,
      38,    39,    40,    90,    42,    43
};

static const yytype_int16 yycheck[] =
{
       6,    25,   118,    27,    12,    63,    18,   133,    32,    33,
     128,     3,     4,     5,   121,     7,     8,   119,     3,   105,
      14,   120,    12,    30,    14,    29,    29,   240,    20,    87,
      34,    33,   101,    13,   127,     8,   331,    10,    65,    12,
      34,    14,   353,   338,    50,    37,    12,   353,    65,    56,
      77,   109,   347,   364,    27,    28,    50,    51,   364,   117,
      50,    51,    35,    61,    58,    89,    93,    12,    58,   127,
      68,    98,    74,    38,   101,    40,    93,    50,    51,    52,
      53,    98,   175,    12,   101,    65,    65,    12,    95,    66,
      67,   304,    12,    66,    67,   119,    12,   101,   101,    93,
       7,     8,     9,    93,    62,    63,    64,    65,   100,   101,
     209,   103,   104,   239,    16,   107,   101,   175,    98,    98,
       3,   101,   101,     3,    97,   241,    99,   100,   101,   102,
     103,    69,    70,    32,   101,    12,    93,   101,   256,   131,
     226,    16,    12,     0,     3,    14,   101,    60,   247,    72,
     142,    73,    71,    31,   261,    59,   258,    11,   216,     3,
      13,    17,    13,    15,     8,    13,    10,     3,    12,    34,
      14,   289,   288,   101,     3,    14,   168,    58,   202,    29,
      13,   297,    16,    27,    28,    12,   178,   245,    14,    63,
     101,    35,   204,     5,   301,    13,    94,    55,     5,    54,
     101,    12,   260,    12,    14,    13,    50,    51,    52,    53,
     336,    55,    13,   205,    12,     5,   274,    13,    13,   326,
      13,    65,    66,    67,   248,    10,    17,    12,    12,    14,
      94,    12,    94,   101,   258,     3,    13,    17,   354,    13,
      34,   101,    27,    28,     3,    94,    13,    13,    37,    93,
      13,    57,    13,    97,    98,    99,   100,   101,   102,   103,
     101,     3,    13,    13,    16,     3,     3,    45,   292,     5,
      17,   167,   296,   281,   282,   364,   360,   285,   286,   302,
     329,   181,   308,   180,   179,   277,   191,   279,   182,   197,
     183,   283,   284,    18,   186,   287,    84,    -1,   201,    -1,
      -1,    -1,    -1,   194,    -1,    -1,    -1,    -1,    -1,    -1,
     302,   319,    97,    -1,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     0,   341,    -1,     3,   360,   345,    -1,    -1,
       8,    -1,    10,    -1,    12,    -1,    14,   353,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   364,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    12,    91,    14,    93,    16,    17,    96,    97,
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
      -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    91,
      14,    93,    16,    -1,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
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
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    -1,    14,    93,    16,    -1,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    65,    66,    67,
       8,    -1,    10,    -1,    12,    -1,    14,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    93,    -1,    35,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    93,    -1,    35,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
       8,    -1,    10,    -1,    12,    -1,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    93,    -1,    35,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
       8,    -1,    10,    -1,    12,    -1,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    93,    -1,    35,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    93,    -1,    35,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
       8,    -1,    10,    -1,    12,    -1,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    93,    -1,    35,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    93,    -1,    35,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,     4,     5,     6,    -1,    65,    66,    67,
       8,    -1,    10,    -1,    12,    -1,    14,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    93,    -1,    35,    -1,    97,
      98,    99,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,   103
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
      93,   144,   101,   115,    12,   144,   120,   144,    93,   101,
     172,    16,   133,    12,    29,    34,   143,     0,   106,   116,
       3,     4,     5,     6,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    61,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,   136,    14,   137,
      30,    56,    95,   110,   118,   101,   147,   148,    33,    74,
      60,    72,    73,    71,    61,    68,   158,    62,    63,    64,
      65,   160,    69,    70,   162,    66,    67,   164,     7,     8,
       9,   166,    31,   170,    59,    14,    50,    51,    58,    93,
     135,   137,   173,   175,    11,    13,    15,    17,    13,    13,
     133,   133,    38,    40,   133,   133,   132,     3,   133,    34,
      34,   101,   114,     3,   137,   122,   120,   172,   147,    58,
      63,    13,   101,    12,   137,   138,   114,     5,    94,   133,
     145,    13,   115,   133,   139,    15,    54,    55,   117,   137,
     138,     5,   133,   154,   155,   156,   157,   159,   161,   163,
     165,   167,   168,   144,   176,   101,   172,    12,   174,    14,
     137,    13,    13,    12,    12,    13,    13,   132,    13,     5,
      17,    12,    94,   115,   143,   122,    12,   140,   101,    94,
     172,   144,     3,    17,    13,   114,   120,   147,    34,    94,
      13,   133,    15,   123,   123,   133,   133,   123,   123,    13,
     133,   122,   114,     3,   101,   101,   144,    13,   121,   144,
     122,    13,   147,   145,     3,   143,    57,   109,   101,   152,
      13,    37,    13,    13,   123,    13,     3,    13,   147,    13,
     148,   140,   115,    16,   140,   123,     3,     3,   123,   140,
       5,   108,   111,   112,   113,   116,   142,   122,   121,    17,
     112
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
     139,   139,   139,   140,   140,   141,   141,   142,   142,   142,
     143,   143,   144,   144,   144,   145,   145,   146,   147,   147,
     148,   148,   148,   149,   150,   151,   152,   152,   153,   153,
     154,   154,   155,   155,   156,   156,   157,   157,   158,   158,
     159,   159,   160,   160,   160,   160,   161,   161,   162,   162,
     163,   163,   164,   164,   165,   165,   166,   166,   166,   167,
     167,   168,   168,   169,   169,   169,   169,   169,   169,   169,
     170,   170,   170,   170,   170,   171,   171,   172,   172,   173,
     173,   174,   174,   175,   175,   176,   176,   176,   176,   176,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177
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
       5,     3,     6,     2,     3,     0,     4,     7,     5,     4,
       0,     1,     2,     4,     5,     2,     3,     5,     1,     3,
       1,     2,     2,     3,     3,     3,     1,     5,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     1,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     2,
       1,     2,     2,     3,     3,     1,     3,     1,     3,     0,
       1,     2,     3,     1,     1,     1,     2,     3,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     2
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
#line 1692 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 144: /* type_decl  */
#line 149 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1698 "src/ast/parser.c" /* yacc.c:1257  */
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
#line 1966 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 154 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(scanner)); }
#line 1972 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 158 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(scanner)); }
#line 1978 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 159 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(scanner)); }
#line 1984 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 160 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(scanner)); }
#line 1990 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 165 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].id_list), (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->types = (yyvsp[-7].id_list); }
#line 1996 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 168 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[0].id_list); }
#line 2002 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 168 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2008 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 171 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_public; }
#line 2014 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 172 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_private; }
#line 2020 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 176 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2026 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 177 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2032 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 181 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body( (yyvsp[0].section), get_pos(scanner)); }
#line 2038 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 182 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body( (yyvsp[-1].section), (yyvsp[0].class_body), get_pos(scanner)); }
#line 2044 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 186 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list( (yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2050 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 187 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def( (yyvsp[0].func_def), get_pos(scanner)); }
#line 2056 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 188 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def( (yyvsp[0].class_def), get_pos(scanner)); }
#line 2062 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 192 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2068 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 193 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2074 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 197 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2080 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 198 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2086 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 202 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(scanner));}
#line 2092 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 203 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(scanner));}
#line 2098 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 207 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_static;   }
#line 2104 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 208 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_instance; }
#line 2110 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 212 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_func; }
#line 2116 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 213 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2122 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 217 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-2].sval), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(scanner)); }
#line 2128 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 218 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_flag_static, (yyvsp[-2].sval), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(scanner)); }
#line 2134 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 222 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2140 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 223 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2146 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 224 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2152 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 228 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(scanner)); }
#line 2158 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 229 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(scanner)); }
#line 2164 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 233 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( NULL, get_pos(scanner)); }
#line 2170 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 234 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( (yyvsp[-1].stmt_list), get_pos(scanner)); }
#line 2176 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 253 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-2].id_list), NULL, get_pos(scanner)); }
#line 2182 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 254 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sval), get_pos(scanner)); }
#line 2188 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 258 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 1, get_pos(scanner)); }
#line 2194 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 262 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 0, get_pos(scanner)); }
#line 2200 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 266 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2206 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 267 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2212 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 271 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner));}
#line 2218 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 276 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2224 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 278 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2230 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2236 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2242 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2248 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 286 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2254 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 288 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop( (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2260 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 293 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(scanner)); }
#line 2266 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 295 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2272 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 299 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( NULL, get_pos(scanner)); }
#line 2278 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 300 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2284 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 301 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_break(get_pos(scanner)); }
#line 2290 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 302 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_continue(get_pos(scanner)); }
#line 2296 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 306 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_expression((yyvsp[-1].exp),   get_pos(scanner)); }
#line 2302 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 307 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_expression(NULL, get_pos(scanner)); }
#line 2308 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 311 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2314 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 312 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_expression((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2320 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 316 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2326 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 317 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2332 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 321 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2338 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 325 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2344 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 326 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2350 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 327 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; /* LCOV_EXCL_LINE */ }
#line 2356 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 328 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2362 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 329 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2368 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 330 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2374 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 331 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2380 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 332 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2386 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 333 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2392 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 334 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2398 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 335 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2404 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 336 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2410 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 337 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2416 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 338 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2422 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 339 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2428 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 340 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2434 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 341 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2440 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 342 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2446 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 343 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2452 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 344 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2458 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 345 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2464 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 346 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2470 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 347 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2476 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 348 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2482 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 349 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2488 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 353 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( (yyvsp[-1].exp), get_pos(scanner) ); }
#line 2494 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 354 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[0].array_sub), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2500 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 355 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( new_array_sub( NULL, get_pos(scanner)), (yyvsp[-3].exp), get_pos(scanner)); }
#line 2506 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 359 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( NULL, get_pos(scanner)); }
#line 2512 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 360 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-2].array_sub), NULL, get_pos(scanner)); }
#line 2518 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 361 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-1].array_sub), (yyvsp[0].array_sub)->exp_list, get_pos(scanner)); }
#line 2524 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 366 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); }
#line 2530 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 367 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); (yyval.exp)->d.exp_decl.types = (yyvsp[-3].type_list); }
#line 2536 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 368 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); }
#line 2542 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 369 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); (yyval.exp)->d.exp_decl.types = (yyvsp[-3].type_list); }
#line 2548 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 373 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2554 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 374 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2560 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 377 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2566 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 377 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2572 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 381 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->types = (yyvsp[-6].id_list); if((yyvsp[-6].id_list)) SET_FLAG((yyval.func_def), ae_flag_template);}
#line 2578 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 383 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_func | ae_flag_static | ae_flag_op , (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2584 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 385 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_func | ae_flag_instance | ae_flag_dtor, new_type_decl(new_id_list("void", get_pos(scanner)), 0, 
      get_pos(scanner)), "dtor", NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2591 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 389 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2597 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 389 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 2603 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 392 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), (yyvsp[0].ival), get_pos(scanner)); }
#line 2609 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 393 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scanner)); }
#line 2615 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 394 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scanner)); }
#line 2621 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 399 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2627 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 400 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2633 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 404 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-2].decl_list), get_pos(scanner)); }
#line 2639 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 408 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(scanner)); }
#line 2645 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 409 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(scanner)); }
#line 2651 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 413 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sval), NULL, get_pos(scanner)); }
#line 2657 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 414 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2663 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 415 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2669 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 418 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2675 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 419 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar(   (yyvsp[-1].exp), get_pos(scanner)); }
#line 2681 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 420 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec(     (yyvsp[-1].exp), get_pos(scanner)); }
#line 2687 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 425 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_if( (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2693 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 429 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2699 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 431 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2705 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 435 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2711 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 437 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2717 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 441 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2723 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 443 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2729 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 447 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2735 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 449 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(scanner)); }
#line 2741 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 453 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2747 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 455 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2753 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 458 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2759 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 458 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_neq; }
#line 2765 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 460 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2771 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 462 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2777 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 465 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_lt; }
#line 2783 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 465 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_gt; }
#line 2789 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 465 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_le; }
#line 2795 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 465 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ge; }
#line 2801 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 468 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2807 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 470 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2813 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 474 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 2819 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 475 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 2825 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 479 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2831 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 481 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2837 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 484 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2843 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 484 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 2849 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 487 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2855 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 489 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_plus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2861 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 492 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 2867 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 492 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide; }
#line 2873 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 492 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_percent; }
#line 2879 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 495 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2885 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 497 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2891 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 503 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_cast( (yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2897 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 506 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2903 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 506 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 2909 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 506 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 2915 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 507 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 2921 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 507 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 2927 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 508 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 2933 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 508 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 2939 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 512 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2945 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 514 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary( (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2951 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 516 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(scanner)); }
#line 2957 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 518 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary2(op_new, (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2963 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 520 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary3( op_spork, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2969 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 526 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2975 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 530 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sval), get_pos(scanner)), NULL, get_pos(scanner)); }
#line 2981 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 531 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sval), get_pos(scanner)), (yyvsp[0].type_list), get_pos(scanner)); }
#line 2987 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 534 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 2993 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 534 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[0].type_list);}
#line 2999 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 535 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 3005 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 535 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3011 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 539 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3017 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 539 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3023 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 544 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array( (yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3029 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 546 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_call( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); (yyval.exp)->d.exp_func.types = (yyvsp[-1].type_list); }
#line 3035 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 548 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dot( (yyvsp[-2].exp), (yyvsp[0].sval), get_pos(scanner)); }
#line 3041 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 550 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_postfix( (yyvsp[-1].exp), (yyvsp[0].ival), get_pos(scanner)); }
#line 3047 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 554 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_ID(      (yyvsp[0].sval), get_pos(scanner)); }
#line 3053 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 555 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_int(     (yyvsp[0].ival), get_pos(scanner)); }
#line 3059 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 556 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_float(   (yyvsp[0].fval), get_pos(scanner)); }
#line 3065 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 557 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_string(  (yyvsp[0].sval), get_pos(scanner)); }
#line 3071 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 558 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_char(    (yyvsp[0].sval), get_pos(scanner)); }
#line 3077 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 559 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3083 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 560 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3089 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 561 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_complex( (yyvsp[0].c_val), get_pos(scanner)); }
#line 3095 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 562 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_polar(   (yyvsp[0].polar), get_pos(scanner)); }
#line 3101 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 563 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_vec(     (yyvsp[0].vec), get_pos(scanner)); }
#line 3107 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 564 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_hack(    (yyvsp[-1].exp), get_pos(scanner)); }
#line 3113 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 565 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) =                       (yyvsp[-1].exp);                    }
#line 3119 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 566 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_nil(         get_pos(scanner)); }
#line 3125 "src/ast/parser.c" /* yacc.c:1646  */
    break;


#line 3129 "src/ast/parser.c" /* yacc.c:1646  */
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
#line 568 "utils/gwion.y" /* yacc.c:1906  */

