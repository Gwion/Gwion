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
#define yyparse         minimal_parse
#define yylex           minimal_lex
#define yyerror         minimal_error
#define yydebug         minimal_debug
#define yynerrs         minimal_nerrs


/* Copy the first part of user declarations.  */
#line 8 "eval/gwion.y" /* yacc.c:339  */

#define YYERROR_VERBOSE
#include "absyn.h"
#define scanner arg->scanner


char *strcat(char *dest, const char *src);
void minimal_error(void* data, const char* s);
int minimal_lex(void*, void* , void*);
static int get_pos(void* data)
{
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  return arg->line;
}

static char* append_doc(void* data, m_str str)
{
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  vector_append(arg->doc, (vtype)str);
  return str;
}

static m_str get_doc(void* data)
{
  m_str ret;
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  ret = (m_str)vector_front(arg->doc);
  vector_remove(arg->doc, 0);
  return ret;
}


#line 105 "eval/parser.c" /* yacc.c:339  */

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
    NUM = 353,
    FLOAT = 354,
    ID = 355,
    STRING_LIT = 356,
    CHAR_LIT = 357,
    DOC = 358,
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
#define NUM 353
#define FLOAT 354
#define ID 355
#define STRING_LIT 356
#define CHAR_LIT 357
#define DOC 358
#define NEG 359

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 41 "eval/gwion.y" /* yacc.c:355  */

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
  Class_Ext class_ext;
//  Class_Ext iface_ext;
  Class_Def class_def;
  Ast ast;

#line 381 "eval/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int minimal_parse (MyArg* arg);

#endif /* !YY_MINIMAL_EVAL_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 397 "eval/parser.c" /* yacc.c:358  */

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
#define YYFINAL  135
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1341

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  105
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  67
/* YYNRULES -- Number of rules.  */
#define YYNRULES  209
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  369

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
       0,   168,   168,   169,   173,   174,   175,   179,   184,   185,
     189,   190,   194,   195,   199,   200,   204,   205,   206,   210,
     211,   215,   216,   220,   221,   225,   226,   230,   231,   235,
     236,   237,   238,   242,   243,   244,   248,   249,   253,   254,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   273,   274,   278,   282,   286,   287,   291,   295,
     297,   299,   301,   303,   305,   307,   312,   314,   319,   320,
     321,   322,   326,   327,   328,   332,   333,   337,   338,   342,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   374,   375,   376,   380,   381,
     382,   386,   387,   388,   392,   393,   397,   398,   402,   404,
     406,   411,   412,   413,   414,   419,   420,   421,   422,   426,
     430,   431,   435,   436,   437,   441,   446,   451,   454,   455,
     460,   461,   466,   467,   472,   473,   478,   479,   484,   485,
     490,   491,   493,   498,   499,   501,   503,   505,   510,   511,
     515,   516,   521,   522,   524,   529,   530,   532,   534,   539,
     540,   546,   547,   549,   551,   553,   555,   557,   562,   563,
     564,   565,   566,   570,   571,   575,   576,   580,   581,   585,
     586,   590,   591,   593,   595,   597,   599,   604,   605,   606,
     607,   608,   609,   610,   611,   612,   613,   614,   615,   616
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
  "UNION", "ATPAREN", "NUM", "FLOAT", "ID", "STRING_LIT", "CHAR_LIT",
  "DOC", "NEG", "$accept", "ast", "section", "class_def", "class_ext",
  "class_decl", "class_body", "class_body2", "class_section", "id_list",
  "id_dot", "stmt_list", "static_decl", "function_decl", "func_ptr",
  "type_decl2", "arg_list", "code_segment", "stmt", "enum_stmt",
  "label_stmt", "goto_stmt", "case_stmt", "switch_stmt", "loop_stmt",
  "selection_stmt", "jump_stmt", "exp_stmt", "exp", "binary_exp",
  "template", "op", "array_exp", "array_empty", "decl_exp", "func_args",
  "func_template", "func_def", "type_decl", "decl_list", "union_stmt",
  "var_decl_list", "var_decl", "complex_exp", "polar_exp", "vec_exp",
  "conditional_expression", "logical_or_expression",
  "logical_and_expression", "inclusive_or_expression",
  "exclusive_or_expression", "and_expression", "equality_expression",
  "relational_expression", "shift_op", "shift_expression",
  "additive_expression", "multiplicative_expression", "cast_exp",
  "unary_expression", "unary_operator", "dur_exp", "type_list",
  "call_paren", "call_template", "postfix_exp", "primary_exp", YY_NULLPTR
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

#define YYPACT_NINF -288

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-288)))

#define YYTABLE_NINF -190

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     544,  -288,  -288,  1119,   759,   819,   622,  1119,  1119,  -288,
      27,    69,   699,    88,    91,   100,    38,   129,    55,    68,
     198,   140,   161,  1239,  1239,     5,   134,    65,  -288,    70,
    -288,  -288,   156,     5,     5,    79,   159,  1119,  -288,  -288,
       1,  -288,  -288,  -288,   178,   310,  -288,   126,  -288,  -288,
    -288,   699,  -288,  -288,  -288,  -288,  -288,  -288,  -288,  -288,
    -288,   179,  1236,  -288,   169,  -288,     7,  -288,    85,  -288,
    -288,  -288,  -288,  -288,    35,   128,   117,   118,   119,    -7,
      72,    17,    92,    41,   162,  -288,  1239,   135,    46,  -288,
       5,    -5,   184,  -288,   183,  -288,   182,  -288,   181,   186,
     187,  1119,  1119,    40,  1119,  1119,    24,   199,  1119,  -288,
       6,   170,   103,  -288,   202,  -288,  -288,  -288,  -288,   180,
     197,   200,     5,    85,   157,   151,   204,   120,   206,   205,
     103,  1179,   208,  -288,  -288,  -288,  -288,   103,  -288,  -288,
    -288,  1119,  -288,  -288,  -288,  -288,  -288,  -288,  -288,  -288,
    -288,  -288,  -288,  -288,  -288,  -288,  -288,  -288,  -288,  -288,
    -288,  -288,  -288,  -288,  -288,  -288,  1119,   879,  -288,  -288,
    -288,   167,   205,  -288,   219,  1119,  1239,  1239,  1239,  1239,
    1239,  1239,  1239,  1239,  1239,  1239,  1239,  -288,  -288,  1239,
    1239,  1239,  1239,  1239,  1239,     5,  -288,    55,  1119,  -288,
    -288,   127,   130,  -288,  -288,   216,  -288,  -288,   215,  -288,
    -288,  -288,   218,   221,   220,   223,   224,   225,    24,  -288,
     226,  -288,  -288,   235,   227,  -288,  -288,  -288,   229,  -288,
      70,   213,   200,   231,   145,  -288,   169,   152,    18,   244,
     237,  -288,   195,  -288,  -288,  -288,  -288,     5,  -288,   169,
      85,   222,   128,   117,   118,   119,    -7,    72,    72,    17,
      17,    17,    17,    92,    41,    41,   162,   162,   162,  -288,
      46,  -288,   250,   163,   939,  -288,   999,  -288,   699,   699,
    1119,  1119,   699,   699,  1059,   200,   103,    19,   158,  -288,
    -288,  -288,    20,   200,   246,  -288,  1179,  1179,   257,    70,
     245,   166,  -288,  1239,   130,  -288,  -288,   249,  -288,   230,
    -288,   256,   259,  -288,  -288,   699,   260,  -288,  -288,  -288,
     273,   264,  -288,   265,    85,  -288,   267,  -288,  -288,  -288,
    -288,   388,   231,  -288,  -288,  -288,   699,   277,   278,  -288,
     699,  -288,   270,  -288,   279,    28,  -288,   269,  -288,   466,
    -288,  -288,   200,  -288,  -288,  -288,  -288,    29,     5,  -288,
     274,  -288,  -288,  -288,  -288,   275,  -288,  -288,  -288
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     116,    73,   181,     0,     0,     0,     0,     0,     0,   180,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    10,     0,
     179,   178,     0,     0,     0,     0,     0,     0,   198,   199,
     197,   200,   201,    74,     0,   116,     6,     0,     4,    50,
      43,    23,    48,    44,    45,    47,    46,    41,    42,    49,
      40,     0,    75,   202,   203,    77,     0,     5,     0,    51,
     204,   205,   206,   111,   138,   140,   142,   144,   146,   148,
     150,   153,   160,   162,   165,   169,     0,   171,   183,   191,
       0,   197,     0,   209,     0,   108,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   197,
     189,   191,     0,    68,     0,    70,    71,   172,   173,   121,
     175,   182,     0,     0,    21,     0,     0,     0,     0,    33,
       0,     0,     0,   122,    54,     1,     3,     0,    24,    72,
      80,     0,   104,    84,    85,    86,    87,    88,    83,    81,
      89,    90,    82,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,     0,     0,   110,    27,
      28,    26,   132,   112,   130,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   158,   159,     0,
       0,     0,     0,     0,     0,     0,   174,     0,     0,   195,
     196,     0,     0,   190,   192,     0,   207,   208,   105,    39,
     136,   135,     0,     0,     0,     0,     0,     0,     0,    55,
       0,    57,    56,    19,     0,    69,   176,   177,     0,   113,
       0,   123,     0,     0,     0,    35,    34,     0,   197,     0,
       0,   137,     9,    76,    78,   109,    25,     0,   133,   134,
       0,     0,   141,   143,   145,   147,   149,   151,   152,   157,
     155,   156,   154,   161,   164,   163,   167,   166,   168,   170,
     184,   194,   185,     0,     0,   193,     0,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
     124,   120,     0,     0,     0,   117,   127,   125,     0,     0,
       0,     0,   131,     0,     0,    79,   187,     0,   107,    66,
      59,     0,     0,    63,    65,     0,     0,    58,    20,    52,
       0,     0,   114,     0,     0,   119,     0,   128,   126,   129,
       8,   116,     0,   139,   186,   188,     0,     0,     0,    61,
       0,    53,     0,   115,    36,     0,    18,     0,    12,   116,
      16,    17,     0,    67,    60,    64,    62,     0,     0,    29,
       0,     7,    15,   118,    30,     0,    37,    31,    32
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -288,   247,  -288,  -287,  -288,  -288,  -288,   -58,  -288,  -119,
    -207,    -6,  -288,  -288,  -288,  -107,  -213,  -116,    -8,  -288,
    -288,  -288,  -288,  -288,  -288,  -288,  -288,   -87,     9,  -288,
    -288,  -288,   -57,  -101,  -123,   -38,  -288,  -276,   -24,  -136,
    -288,  -117,   -23,  -288,  -288,  -288,    -1,  -288,   132,   137,
     125,   133,   124,   -19,  -288,   -37,   116,    -4,   -86,     2,
    -288,  -288,    11,  -288,  -288,   -16,   288
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    44,    45,    46,   300,    47,   347,   348,   349,   224,
     125,    48,   247,   171,    49,   128,   323,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
     203,   166,    63,    64,    65,   293,    66,    67,    68,   240,
      69,   173,   174,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,   189,    81,    82,    83,    84,    85,
      86,    87,   273,   275,   205,    88,    89
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      98,   120,   110,   123,   103,   227,   229,   168,   239,   127,
     129,   237,    92,    94,    96,   228,    99,   100,   242,   218,
     198,   296,   319,   289,   133,   117,   118,     1,   236,   114,
     133,   204,     2,   322,     3,   134,     4,   169,     5,   101,
     221,   359,   364,   244,   346,   138,   132,   133,   192,   193,
     194,     7,     8,   204,   181,   351,   199,   200,  -189,     9,
     198,   182,   346,   226,   201,     3,   123,     4,   175,     5,
      29,   249,   235,   351,    23,    24,    25,    26,   214,    90,
     215,   102,     7,     8,   112,    29,   187,   188,   196,    29,
      30,    31,   330,    29,    29,  -121,   199,   200,   129,   202,
     104,  -121,   170,   105,   201,   119,   266,   267,   268,   176,
     212,   213,   106,   216,   217,   248,   291,   220,  -121,   320,
     119,    37,    38,    39,    91,    41,    42,    43,   119,   119,
      29,   284,   360,   302,   183,   184,   185,   186,   107,   202,
     301,   108,   122,   115,   365,   366,   259,   260,   261,   262,
     243,   277,    37,    38,    39,   109,    41,    42,   190,   191,
     327,   328,   257,   258,   116,   119,   121,   318,   126,   317,
     124,   269,   130,   239,   239,   131,    96,   325,   135,   168,
     137,   270,   139,   167,   251,   172,   264,   265,   177,   178,
     180,   179,   168,   195,   197,   206,   207,   208,   209,   210,
     211,   113,   219,   223,   222,   225,     2,    96,     3,   133,
       4,   198,     5,   204,   231,   230,     6,   232,   234,     5,
     233,   241,   246,   129,   250,     7,     8,   271,   274,   276,
     272,   278,   280,     9,   279,   281,   363,   282,   283,   285,
     286,   288,   290,   292,   287,   294,   295,   297,    23,    24,
      25,    26,   299,    90,   298,   304,   303,   305,   321,   326,
     329,   331,   335,    29,    30,    31,   332,   336,   324,   337,
     309,   310,   338,   340,   313,   314,   341,   342,   343,   345,
     354,   355,   357,   307,   358,    96,   361,   367,   368,   311,
     312,   362,   136,   316,   352,    37,    38,    39,    91,    41,
      42,   344,   333,   254,   256,   263,   111,   339,   252,     0,
      -2,     0,   255,     1,   253,   334,     0,     0,     2,     0,
       3,   324,     4,     0,     5,   350,     6,     0,   353,     0,
       0,     0,   356,   324,   324,     0,     0,     7,     8,     0,
       0,     0,     0,   350,     0,     9,    10,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   -11,    27,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    30,    31,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,     0,     0,
       0,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,    35,     5,     0,     6,   -13,    36,    37,    38,    39,
      40,    41,    42,    43,     0,     7,     8,     0,     0,     0,
       0,     0,     0,     9,    10,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,   -11,    27,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     0,     0,     0,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,    35,
       5,     0,     6,   -14,    36,    37,    38,    39,    40,    41,
      42,    43,     0,     7,     8,     0,     0,     0,     0,     0,
       0,     9,    10,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
     -11,    27,    28,     0,     0,     0,     0,     0,     0,     0,
       0,    29,    30,    31,     0,     0,     0,     0,     0,     0,
       0,    32,    33,    34,     0,     0,     0,     1,     0,     0,
       0,     0,     2,     0,     3,     0,     4,    35,     5,     0,
       6,     0,    36,    37,    38,    39,    40,    41,    42,    43,
       0,     7,     8,     0,     0,     0,     0,     0,     0,     9,
      10,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   -11,    27,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    29,
      30,    31,     0,     0,     0,     0,     0,     0,     0,    32,
      33,    34,     0,     0,     0,     1,     0,     0,     0,     0,
       2,     0,     3,     0,     4,    35,     5,     0,     6,    97,
      36,    37,    38,    39,    40,    41,    42,    43,     0,     7,
       8,     0,     0,     0,     0,     0,     0,     9,    10,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    34,
       0,     0,     1,     0,     0,     0,     0,     2,     0,     3,
       0,     4,     0,     5,     0,     6,     0,     0,    36,    37,
      38,    39,    40,    41,    42,    43,     7,     8,     0,     0,
       0,     0,     0,     0,     9,    10,     0,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     2,     0,     3,
       0,     4,    93,     5,     0,     0,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,     9,    36,    37,    38,    39,    40,
      41,    42,    43,     0,     0,     0,     0,     0,     0,    23,
      24,    25,    26,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     2,     0,     3,
       0,     4,     0,     5,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,     9,     0,    37,    38,    39,    91,
      41,    42,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,    26,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     2,     0,     3,
       0,     4,     0,     5,   245,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,     9,     0,    37,    38,    39,    91,
      41,    42,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,    26,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     2,     0,     3,
       0,     4,   306,     5,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,     9,     0,    37,    38,    39,    91,
      41,    42,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,    26,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     2,     0,     3,
       0,     4,     0,     5,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,     9,     0,    37,    38,    39,    91,
      41,    42,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,    26,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     2,     0,     3,
       0,     4,   315,     5,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,     9,     0,    37,    38,    39,    91,
      41,    42,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,    26,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     2,     0,     3,
       0,     4,     0,     5,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,     9,     0,    37,    38,    39,    91,
      41,    42,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,    26,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     2,     0,     3,
       0,     4,     0,     5,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,     9,     0,    37,    38,    39,    91,
      41,    42,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,    26,     0,    90,     0,     0,     0,     0,     0,
     140,   141,   142,     0,    29,    30,    31,     2,     0,     3,
       0,     4,     0,     5,   143,   144,   145,   146,   147,   148,
     149,   150,   151,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,     9,     0,    37,    38,    39,   238,
      41,    42,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,    26,     0,     0,     0,     0,   152,     0,     0,
       0,     0,     0,     0,     0,    30,    31,     0,     0,     0,
       0,     0,     0,     0,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,    38,    39,   109,
      41,    42
};

static const yytype_int16 yycheck[] =
{
       6,    25,    18,    27,    12,   121,   123,    64,   131,    33,
      34,   130,     3,     4,     5,   122,     7,     8,   137,   106,
      14,     3,     3,   230,    29,    23,    24,     3,   129,    20,
      29,    88,     8,    13,    10,    34,    12,    30,    14,    12,
      34,    13,    13,   166,   331,    51,    37,    29,     7,     8,
       9,    27,    28,   110,    61,   331,    50,    51,    12,    35,
      14,    68,   349,   120,    58,    10,    90,    12,    33,    14,
      65,   172,   129,   349,    50,    51,    52,    53,    38,    55,
      40,    12,    27,    28,    16,    65,    69,    70,    86,    65,
      66,    67,   299,    65,    65,   100,    50,    51,   122,    93,
      12,   100,    95,    12,    58,   100,   192,   193,   194,    74,
     101,   102,    12,   104,   105,   172,   232,   108,   100,   100,
     100,    97,    98,    99,   100,   101,   102,   103,   100,   100,
      65,   218,   345,   250,    62,    63,    64,    65,   100,    93,
     247,    12,    77,     3,   357,   358,   183,   184,   185,   186,
     141,   208,    97,    98,    99,   100,   101,   102,    66,    67,
     296,   297,   181,   182,     3,   100,    32,   286,    12,   285,
     100,   195,    93,   296,   297,    16,   167,   293,     0,   236,
      54,   197,     3,    14,   175,   100,   190,   191,    60,    72,
      71,    73,   249,    31,    59,    11,    13,    15,    17,    13,
      13,     3,     3,   100,    34,     3,     8,   198,    10,    29,
      12,    14,    14,   270,    63,    58,    16,    13,    12,    14,
     100,    13,    55,   247,     5,    27,    28,   100,    12,    14,
     100,    13,    12,    35,    13,    12,   352,    13,    13,    13,
       5,    12,    29,    12,    17,   100,    94,     3,    50,    51,
      52,    53,    57,    55,    17,     5,    34,    94,   100,    13,
       3,    16,    13,    65,    66,    67,   100,    37,   292,    13,
     278,   279,    13,    13,   282,   283,     3,    13,    13,    12,
       3,     3,    12,   274,     5,   276,    17,    13,    13,   280,
     281,   349,    45,   284,   332,    97,    98,    99,   100,   101,
     102,   324,   303,   178,   180,   189,    18,   315,   176,    -1,
       0,    -1,   179,     3,   177,   304,    -1,    -1,     8,    -1,
      10,   345,    12,    -1,    14,   331,    16,    -1,   336,    -1,
      -1,    -1,   340,   357,   358,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,   349,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    91,    14,    -1,    16,    17,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    91,
      14,    -1,    16,    17,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    12,    91,    14,    -1,
      16,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    91,    14,    -1,    16,    17,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    16,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,   103,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,
      -1,    12,    13,    14,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,
      -1,    12,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,
      -1,    12,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,
      -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,
      -1,    12,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,
      -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
       4,     5,     6,    -1,    65,    66,    67,     8,    -1,    10,
      -1,    12,    -1,    14,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    35,
      36,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    55,    56,    65,
      66,    67,    75,    76,    77,    91,    96,    97,    98,    99,
     100,   101,   102,   103,   106,   107,   108,   110,   116,   119,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   137,   138,   139,   141,   142,   143,   145,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   160,   161,   162,   163,   164,   165,   166,   170,   171,
      55,   100,   133,    13,   133,    15,   133,    17,   116,   133,
     133,    12,    12,   123,    12,    12,    12,   100,    12,   100,
     170,   171,    16,     3,   133,     3,     3,   164,   164,   100,
     143,    32,    77,   143,   100,   115,    12,   143,   120,   143,
      93,    16,   133,    29,    34,     0,   106,    54,   116,     3,
       4,     5,     6,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    61,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,   136,    14,   137,    30,
      95,   118,   100,   146,   147,    33,    74,    60,    72,    73,
      71,    61,    68,    62,    63,    64,    65,    69,    70,   159,
      66,    67,     7,     8,     9,    31,   164,    59,    14,    50,
      51,    58,    93,   135,   137,   169,    11,    13,    15,    17,
      13,    13,   133,   133,    38,    40,   133,   133,   132,     3,
     133,    34,    34,   100,   114,     3,   137,   122,   120,   146,
      58,    63,    13,   100,    12,   137,   138,   114,   100,   139,
     144,    13,   114,   133,   139,    15,    55,   117,   137,   138,
       5,   133,   153,   154,   155,   156,   157,   158,   158,   160,
     160,   160,   160,   161,   162,   162,   163,   163,   163,   143,
     170,   100,   100,   167,    12,   168,    14,   137,    13,    13,
      12,    12,    13,    13,   132,    13,     5,    17,    12,   115,
      29,   122,    12,   140,   100,    94,     3,     3,    17,    57,
     109,   120,   146,    34,     5,    94,    13,   133,    15,   123,
     123,   133,   133,   123,   123,    13,   133,   122,   114,     3,
     100,   100,    13,   121,   143,   122,    13,   144,   144,     3,
     115,    16,   100,   151,   167,    13,    37,    13,    13,   123,
      13,     3,    13,    13,   147,    12,   108,   111,   112,   113,
     116,   142,   140,   123,     3,     3,   123,    12,     5,    13,
     121,    17,   112,   122,    13,   121,   121,    13,    13
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   105,   106,   106,   107,   107,   107,   108,   109,   109,
     110,   110,   111,   111,   112,   112,   113,   113,   113,   114,
     114,   115,   115,   116,   116,   117,   117,   118,   118,   119,
     119,   119,   119,   120,   120,   120,   121,   121,   122,   122,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   124,   124,   125,   126,   127,   127,   128,   129,
     129,   129,   129,   129,   129,   129,   130,   130,   131,   131,
     131,   131,   132,   132,   132,   133,   133,   134,   134,   135,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   137,   137,   137,   138,   138,
     138,   139,   139,   139,   140,   140,   141,   141,   142,   142,
     142,   143,   143,   143,   143,   144,   144,   144,   144,   145,
     146,   146,   147,   147,   147,   148,   149,   150,   151,   151,
     152,   152,   153,   153,   154,   154,   155,   155,   156,   156,
     157,   157,   157,   158,   158,   158,   158,   158,   159,   159,
     160,   160,   161,   161,   161,   162,   162,   162,   162,   163,
     163,   164,   164,   164,   164,   164,   164,   164,   165,   165,
     165,   165,   165,   166,   166,   167,   167,   168,   168,   169,
     169,   170,   170,   170,   170,   170,   170,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     7,     2,     0,
       1,     0,     1,     0,     1,     2,     1,     1,     1,     1,
       3,     1,     3,     1,     2,     1,     0,     1,     1,     7,
       8,     8,     9,     1,     2,     2,     2,     4,     2,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     5,     6,     2,     3,     3,     3,     5,     5,
       7,     6,     7,     5,     7,     5,     5,     7,     2,     3,
       2,     2,     2,     1,     1,     1,     3,     1,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     4,     5,     2,     3,
       2,     1,     2,     3,     2,     3,     0,     4,     7,     5,
       4,     1,     2,     3,     4,     2,     3,     2,     3,     5,
       1,     3,     1,     2,     2,     3,     3,     3,     1,     5,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     1,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       3,     1,     2,     2,     2,     2,     3,     3,     1,     1,
       1,     1,     2,     1,     3,     1,     3,     2,     3,     0,
       1,     1,     2,     3,     3,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     2
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
#line 164 "eval/gwion.y" /* yacc.c:1257  */
      { printf("!!!!\n"); free_type_decl(((*yyvaluep).type_decl)); }
#line 1707 "eval/parser.c" /* yacc.c:1257  */
        break;

    case 143: /* type_decl  */
#line 164 "eval/gwion.y" /* yacc.c:1257  */
      { printf("!!!!\n"); free_type_decl(((*yyvaluep).type_decl)); }
#line 1713 "eval/parser.c" /* yacc.c:1257  */
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
#line 168 "eval/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL, get_pos(scanner));  }
#line 1981 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 169 "eval/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(scanner)); }
#line 1987 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 173 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(scanner)); }
#line 1993 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 174 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(scanner)); }
#line 1999 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 175 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(scanner)); }
#line 2005 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 180 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].class_ext), (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->doc = get_doc(scanner);}
#line 2011 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 184 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = new_class_ext( (yyvsp[0].id_list), NULL, get_pos(scanner)); }
#line 2017 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 185 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = NULL; }
#line 2023 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 189 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_public; }
#line 2029 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 190 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_private; }
#line 2035 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 194 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2041 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 195 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2047 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 199 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body( (yyvsp[0].section), get_pos(scanner)); }
#line 2053 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 200 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body( (yyvsp[-1].section), (yyvsp[0].class_body), get_pos(scanner)); }
#line 2059 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 204 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list( (yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2065 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 205 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def( (yyvsp[0].func_def), get_pos(scanner)); }
#line 2071 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 206 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def( (yyvsp[0].class_def), get_pos(scanner)); }
#line 2077 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 210 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2083 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 211 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2089 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 215 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2095 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 216 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2101 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 220 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(scanner));}
#line 2107 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 221 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(scanner));}
#line 2113 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 225 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_static;   }
#line 2119 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 226 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_instance; }
#line 2125 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 230 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_func; }
#line 2131 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 231 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_variadic; }
#line 2137 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 235 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2143 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 236 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_key_static, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2149 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 237 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2155 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 238 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_key_static, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2161 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 242 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2167 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 243 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2173 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 244 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2179 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 248 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
#line 2185 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 249 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
#line 2191 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 253 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( NULL, get_pos(scanner)); }
#line 2197 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 254 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( (yyvsp[-1].stmt_list), get_pos(scanner)); }
#line 2203 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 273 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-2].id_list), NULL, get_pos(scanner)); }
#line 2209 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 274 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sval), get_pos(scanner)); }
#line 2215 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 278 "eval/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 1, get_pos(scanner)); }
#line 2221 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 282 "eval/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 0, get_pos(scanner)); }
#line 2227 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 286 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2233 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 287 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2239 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 291 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner));}
#line 2245 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 296 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2251 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 298 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2257 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 300 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2263 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 302 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2269 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 304 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2275 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 306 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2281 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 308 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop( (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2287 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 313 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(scanner)); }
#line 2293 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 315 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2299 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 319 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( NULL, get_pos(scanner)); }
#line 2305 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 320 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2311 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 321 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_break(get_pos(scanner)); }
#line 2317 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 322 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_continue(get_pos(scanner)); }
#line 2323 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 326 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_expression((yyvsp[-1].exp),   get_pos(scanner)); }
#line 2329 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 327 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_expression(NULL, get_pos(scanner)); }
#line 2335 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 328 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_expression(NULL, get_pos(scanner)); append_doc(scanner, (yyvsp[0].sval)); }
#line 2341 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 332 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2347 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 333 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_expression((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2353 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 337 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2359 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 338 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2365 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 342 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2371 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 346 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2377 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 347 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2383 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 348 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; /* LCOV_EXCL_LINE */ }
#line 2389 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 349 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2395 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 350 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2401 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 351 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2407 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 352 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2413 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 353 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2419 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 354 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2425 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 355 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2431 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 356 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2437 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 357 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2443 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 358 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2449 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 359 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2455 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 360 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2461 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 361 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2467 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 362 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2473 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 363 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2479 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 364 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2485 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 365 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2491 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 366 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2497 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 367 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2503 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 368 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2509 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 369 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2515 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 370 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2521 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 374 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( (yyvsp[-1].exp), get_pos(scanner) ); }
#line 2527 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 375 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[0].array_sub), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2533 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 376 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( new_array_sub( NULL, get_pos(scanner)), (yyvsp[-3].exp), get_pos(scanner)); }
#line 2539 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 380 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( NULL, get_pos(scanner)); }
#line 2545 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 381 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-2].array_sub), NULL, get_pos(scanner)); }
#line 2551 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 382 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-1].array_sub), (yyvsp[0].array_sub)->exp_list, get_pos(scanner)); }
#line 2557 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 387 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_decl_expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); }
#line 2563 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 388 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_decl_expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); }
#line 2569 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 392 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2575 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 393 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2581 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 397 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2587 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 398 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list);   }
#line 2593 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 403 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival), (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); (yyval.func_def)->types = (yyvsp[-6].id_list); }
#line 2599 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 405 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_key_func, ae_key_static, (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_op; (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2605 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 407 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_key_func, ae_key_instance, new_type_decl(new_id_list("void", get_pos(scanner)), 0, get_pos(scanner)), "dtor", NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_dtor; (yyval.func_def)->type_decl->doc = get_doc(scanner);}
#line 2611 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 411 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[0].sval), get_pos(scanner)), 0, get_pos(scanner)); }
#line 2617 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 412 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), 1, get_pos(scanner)); }
#line 2623 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 413 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-1].id_list),  0, get_pos(scanner)); }
#line 2629 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 414 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list),  1, get_pos(scanner)); }
#line 2635 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 419 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list(&(yyvsp[-1].exp)->d.exp_decl, NULL); }
#line 2641 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 420 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list(&(yyvsp[-2].exp)->d.exp_decl, (yyvsp[0].decl_list)); }
#line 2647 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 421 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = NULL; }
#line 2653 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 422 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list(NULL, (yyvsp[0].decl_list)); }
#line 2659 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 426 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-2].decl_list), get_pos(scanner)); }
#line 2665 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 430 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2671 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 431 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2677 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 435 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sval), NULL, get_pos(scanner)); }
#line 2683 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 436 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2689 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 437 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2695 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 442 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2701 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 447 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar((yyvsp[-1].exp), get_pos(scanner)); }
#line 2707 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 451 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec((yyvsp[-1].exp), get_pos(scanner)); }
#line 2713 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 456 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_if_expression( (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2719 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 460 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2725 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 462 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2731 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 466 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2737 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 468 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2743 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 472 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2749 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 474 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2755 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 478 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2761 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 480 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(scanner)); }
#line 2767 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 484 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2773 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 486 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2779 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 490 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2785 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 492 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_eq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2791 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 494 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_neq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2797 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 498 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2803 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 500 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_lt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2809 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 502 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_gt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2815 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 504 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_le, (yyvsp[0].exp), get_pos(scanner)); }
#line 2821 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 506 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_ge, (yyvsp[0].exp), get_pos(scanner)); }
#line 2827 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 510 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 2833 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 511 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 2839 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 515 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2845 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 517 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2851 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 521 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2857 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 523 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_plus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2863 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 525 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_minus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2869 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 529 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2875 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 531 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_times, (yyvsp[0].exp), get_pos(scanner)); }
#line 2881 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 533 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_divide, (yyvsp[0].exp), get_pos(scanner)); }
#line 2887 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 535 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_percent, (yyvsp[0].exp), get_pos(scanner)); }
#line 2893 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 541 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_cast_expression( (yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2899 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 546 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2905 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 548 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_unary( op_plusplus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2911 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 550 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_unary( op_minusminus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2917 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 552 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_unary( (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2923 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 554 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(scanner)); }
#line 2929 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 556 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_unary2(op_new, (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2935 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 558 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_unary3( op_spork, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2941 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 562 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2947 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 563 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 2953 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 564 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 2959 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 565 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 2965 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 566 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 2971 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 571 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_dur( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2977 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 575 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sval), get_pos(scanner)), NULL, get_pos(scanner)); }
#line 2983 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 576 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sval), get_pos(scanner)), (yyvsp[0].type_list), get_pos(scanner)); }
#line 2989 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 580 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 2995 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 581 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3001 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 585 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 3007 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 586 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[0].type_list);}
#line 3013 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 592 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array( (yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3019 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 594 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_call( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); (yyval.exp)->d.exp_func.types = (yyvsp[-1].type_list); }
#line 3025 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 596 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_dot( (yyvsp[-2].exp), (yyvsp[0].sval), get_pos(scanner)); }
#line 3031 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 598 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_postfix_expression( (yyvsp[-1].exp), op_plusplus, get_pos(scanner)); }
#line 3037 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 600 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_postfix_expression( (yyvsp[-1].exp), op_minusminus, get_pos(scanner)); }
#line 3043 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 604 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_prim_ID(     (yyvsp[0].sval), get_pos(scanner)); }
#line 3049 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 605 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_prim_int(    (yyvsp[0].ival), get_pos(scanner)); }
#line 3055 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 606 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_prim_float(  (yyvsp[0].fval), get_pos(scanner)); }
#line 3061 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 607 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_prim_string( (yyvsp[0].sval), get_pos(scanner)); }
#line 3067 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 608 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_prim_char(                  (yyvsp[0].sval), get_pos(scanner)); }
#line 3073 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 609 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_array_lit(             (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3079 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 610 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_array_lit(           (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3085 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 611 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_complex(               (yyvsp[0].c_val), get_pos(scanner)); }
#line 3091 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 612 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_polar(                 (yyvsp[0].polar), get_pos(scanner)); }
#line 3097 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 613 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_prim_vec(                   (yyvsp[0].vec), get_pos(scanner)); }
#line 3103 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 614 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_hack_expression(                (yyvsp[-1].exp), get_pos(scanner)); }
#line 3109 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 615 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3115 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 616 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_expr_prim_nil(       get_pos(scanner)); }
#line 3121 "eval/parser.c" /* yacc.c:1646  */
    break;


#line 3125 "eval/parser.c" /* yacc.c:1646  */
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
#line 618 "eval/gwion.y" /* yacc.c:1906  */

