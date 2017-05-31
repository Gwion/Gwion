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
/*
static m_str get_arg_doc(void* data)
{
  m_str ret;
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  ret = (m_str)vector_back(arg->doc);
  vector_pop(arg->doc);
  return ret;
}
*/

#line 114 "eval/parser.c" /* yacc.c:339  */

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
    SIZEOF = 309,
    TYPEOF = 310,
    CLASS = 311,
    STATIC = 312,
    PUBLIC = 313,
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
    FUNC_PTR = 334,
    RSL = 335,
    RSR = 336,
    RSAND = 337,
    RSOR = 338,
    RSXOR = 339,
    RAND = 340,
    ROR = 341,
    REQ = 342,
    RNEQ = 343,
    RGT = 344,
    RGE = 345,
    RLT = 346,
    RLE = 347,
    TEMPLATE = 348,
    NOELSE = 349,
    LTB = 350,
    GTB = 351,
    VARARG = 352,
    UNION = 353,
    ATPAREN = 354,
    NUM = 355,
    FLOAT = 356,
    ID = 357,
    STRING_LIT = 358,
    CHAR_LIT = 359,
    DOC = 360
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
#define SIZEOF 309
#define TYPEOF 310
#define CLASS 311
#define STATIC 312
#define PUBLIC 313
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
#define FUNC_PTR 334
#define RSL 335
#define RSR 336
#define RSAND 337
#define RSOR 338
#define RSXOR 339
#define RAND 340
#define ROR 341
#define REQ 342
#define RNEQ 343
#define RGT 344
#define RGE 345
#define RLT 346
#define RLE 347
#define TEMPLATE 348
#define NOELSE 349
#define LTB 350
#define GTB 351
#define VARARG 352
#define UNION 353
#define ATPAREN 354
#define NUM 355
#define FLOAT 356
#define ID 357
#define STRING_LIT 358
#define CHAR_LIT 359
#define DOC 360

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 50 "eval/gwion.y" /* yacc.c:355  */

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

#line 392 "eval/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int minimal_parse (MyArg* arg);

#endif /* !YY_MINIMAL_EVAL_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 408 "eval/parser.c" /* yacc.c:358  */

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
#define YYFINAL  141
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1584

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  106
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  62
/* YYNRULES -- Number of rules.  */
#define YYNRULES  209
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  388

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   360

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
     105
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   176,   176,   177,   181,   182,   183,   187,   192,   193,
     197,   198,   202,   203,   207,   208,   212,   213,   214,   218,
     219,   223,   224,   228,   229,   233,   234,   238,   239,   243,
     244,   245,   246,   250,   251,   252,   256,   257,   261,   262,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   281,   282,   286,   290,   294,   295,   299,   303,
     305,   307,   309,   311,   313,   315,   320,   322,   327,   328,
     329,   330,   334,   335,   336,   340,   341,   345,   346,   350,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   382,   383,   384,   388,   389,
     390,   394,   395,   396,   400,   402,   404,   406,   408,   410,
     412,   417,   418,   419,   420,   425,   426,   427,   428,   432,
     436,   437,   441,   442,   443,   447,   452,   456,   459,   460,
     465,   466,   471,   472,   477,   478,   483,   484,   489,   490,
     495,   496,   498,   503,   504,   506,   508,   510,   515,   516,
     518,   523,   524,   526,   531,   532,   534,   536,   541,   542,
     548,   549,   551,   553,   555,   557,   559,   561,   563,   568,
     569,   570,   571,   572,   576,   577,   581,   582,   586,   587,
     589,   591,   593,   595,   597,   599,   601,   606,   607,   608,
     609,   610,   611,   612,   613,   614,   615,   616,   617,   618
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
  "BREAK", "CONTINUE", "PLUSPLUS", "MINUSMINUS", "NEW", "SPORK", "SIZEOF",
  "TYPEOF", "CLASS", "STATIC", "PUBLIC", "EXTENDS", "DOT", "COLONCOLON",
  "AND", "EQ", "GE", "GT", "LE", "LT", "MINUS", "PLUS", "NEQ",
  "SHIFT_LEFT", "SHIFT_RIGHT", "S_AND", "S_OR", "S_XOR", "OR", "AST_DTOR",
  "OPERATOR", "FUNC_PTR", "RSL", "RSR", "RSAND", "RSOR", "RSXOR", "RAND",
  "ROR", "REQ", "RNEQ", "RGT", "RGE", "RLT", "RLE", "TEMPLATE", "NOELSE",
  "LTB", "GTB", "VARARG", "UNION", "ATPAREN", "NUM", "FLOAT", "ID",
  "STRING_LIT", "CHAR_LIT", "DOC", "$accept", "ast", "section",
  "class_def", "class_ext", "class_decl", "class_body", "class_body2",
  "class_section", "id_list", "id_dot", "stmt_list", "static_decl",
  "function_decl", "func_ptr", "type_decl2", "arg_list", "code_segment",
  "stmt", "enum_stmt", "label_stmt", "goto_stmt", "case_stmt",
  "switch_stmt", "loop_stmt", "selection_stmt", "jump_stmt", "exp_stmt",
  "exp", "binary_exp", "template", "op", "array_exp", "array_empty",
  "decl_exp", "func_def", "type_decl", "decl_list", "union_stmt",
  "var_decl_list", "var_decl", "complex_exp", "polar_exp", "vec_exp",
  "conditional_expression", "logical_or_expression",
  "logical_and_expression", "inclusive_or_expression",
  "exclusive_or_expression", "and_expression", "equality_expression",
  "relational_expression", "shift_expression", "additive_expression",
  "multiplicative_expression", "cast_exp", "unary_expression",
  "unary_operator", "dur_exp", "type_list", "postfix_exp", "primary_exp", YY_NULLPTR
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
     355,   356,   357,   358,   359,   360
};
# endif

#define YYPACT_NINF -284

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-284)))

#define YYTABLE_NINF -122

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     616,  -284,  -284,  1356,   922,   984,   719,  1356,  1356,  -284,
    -284,    14,    29,   798,    64,    72,    80,   -70,    95,    34,
      93,   860,    77,   117,  1480,  1480,   -42,    92,  1480,  1480,
     -24,  -284,    27,  -284,  -284,   151,   -42,   -42,    74,  -284,
     161,  1356,  -284,  -284,     4,  -284,  -284,  -284,   179,   330,
    -284,   129,  -284,   130,  -284,  -284,   798,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,   193,  1477,  -284,   185,
    -284,  -284,    98,  -284,  -284,  -284,  -284,  -284,    -9,   139,
     132,   133,   136,     0,   101,    68,    18,   152,   172,  -284,
    1480,   149,    91,  -284,   -42,    30,   202,  -284,   201,  -284,
     200,  -284,   199,   204,   206,  1356,  1356,    43,  1356,  1356,
      44,   217,  1356,  -284,    15,   187,   120,  -284,   220,  -284,
    -284,  -284,  -284,   196,   212,   214,  -284,  -284,   -42,    98,
     168,   166,   219,   131,   222,   221,   120,  1418,   223,  -284,
    -284,  -284,  -284,   120,  -284,   -42,  -284,  -284,  -284,  1356,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  1356,  1046,  -284,   221,  -284,   232,
    1356,  1480,  1480,  1480,  1480,  1480,  1480,  1480,  1480,  1480,
    1480,  1480,  1480,  1480,  1480,  1480,  1480,  1480,  1480,   -42,
    -284,    34,  1108,  1356,  -284,  -284,   138,   140,   226,  -284,
    -284,  -284,   227,  -284,  -284,  -284,   230,   231,   233,   234,
     235,   236,    44,  -284,   237,  -284,  -284,   242,   238,  -284,
    -284,  -284,   239,  -284,    27,   224,   214,   240,   154,  -284,
     185,   162,     6,   254,   243,  -284,   203,   157,  -284,  -284,
    -284,  -284,   185,    98,   241,   139,   132,   133,   136,     0,
     101,   101,    68,    68,    68,    68,    18,    18,   152,   152,
     172,   172,   172,  -284,    91,  -284,   248,  -284,   259,   169,
    1170,  1232,  -284,   798,   798,  1356,  1356,   798,   798,  1294,
     214,   120,    20,   165,  -284,  -284,  -284,     2,   255,    -8,
    1418,  1418,   266,    27,   258,   267,  -284,  1480,  -284,   140,
    -284,  -284,   257,  -284,   244,  -284,   265,   269,  -284,  -284,
     798,   270,  -284,  -284,  -284,   277,   271,   214,   272,    98,
     274,   130,  -284,  -284,  -284,  -284,   433,    21,  -284,  -284,
    -284,   798,   284,   285,  -284,   798,  -284,   279,  -284,   214,
     287,    23,   -42,  -284,   276,  -284,   536,  -284,  -284,   214,
     283,  -284,  -284,  -284,  -284,    24,  -284,   -42,  -284,   286,
     195,  -284,  -284,  -284,   214,  -284,   288,  -284,  -284,   290,
    -284,  -284,    26,   214,   291,  -284,   214,  -284
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      11,    73,   182,     0,     0,     0,     0,     0,     0,    27,
     181,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    10,     0,   180,   179,     0,     0,     0,     0,    28,
       0,     0,   198,   199,   197,   200,   201,    74,     0,     2,
       6,     0,     4,    26,    50,    43,    23,    48,    44,    45,
      47,    46,    41,    42,    49,    40,     0,    75,   202,   203,
      77,     5,     0,    51,   204,   205,   206,   111,   138,   140,
     142,   144,   146,   148,   150,   153,   158,   161,   164,   168,
       0,   170,   184,   188,     0,   197,     0,   209,     0,   108,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   197,     0,   188,     0,    68,     0,    70,
      71,   171,   172,   121,   176,   183,   175,   174,     0,     0,
      21,     0,     0,     0,     0,    33,     0,     0,     0,   122,
      54,     1,     3,     0,    25,     0,    24,    72,    80,     0,
     104,    84,    85,    86,    87,    88,    83,    81,    89,    90,
      82,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,     0,     0,   110,   132,   112,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     173,     0,     0,     0,   193,   194,     0,     0,     0,   189,
     207,   208,   105,    39,   136,   135,     0,     0,     0,     0,
       0,     0,     0,    55,     0,    57,    56,    19,     0,    69,
     177,   178,     0,   113,     0,   123,     0,     0,     0,    35,
      34,     0,   197,     0,     0,   137,     9,     0,    76,    78,
     109,   133,   134,     0,     0,   141,   143,   145,   147,   149,
     151,   152,   157,   155,   156,   154,   159,   160,   163,   162,
     166,   165,   167,   169,   185,   190,     0,   192,   186,     0,
       0,     0,   106,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,   124,   116,     0,     0,     0,
     127,   125,     0,     0,     0,     0,   131,     0,   191,     0,
      79,   195,     0,   107,    66,    59,     0,     0,    63,    65,
       0,     0,    58,    20,    52,     0,     0,     0,     0,     0,
       0,    26,   128,   126,   129,     8,    11,     0,   139,   187,
     196,     0,     0,     0,    61,     0,    53,     0,   117,     0,
      36,     0,     0,    18,     0,    12,    11,    16,    17,     0,
       0,    67,    60,    64,    62,     0,   118,     0,    29,     0,
       0,     7,    15,   114,     0,    30,     0,    37,    31,     0,
     115,    32,     0,     0,     0,   119,     0,   120
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -284,   251,  -284,  -283,  -284,  -284,  -284,   -53,  -284,  -115,
    -203,    -5,   -25,     8,  -284,  -125,  -187,  -120,   -11,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,   -92,     9,  -284,
    -284,  -284,   -50,   -95,  -129,  -279,   -26,  -146,  -284,  -122,
     -21,  -284,  -284,  -284,    -2,  -284,   134,   135,   127,   137,
     142,   -12,     3,     5,    10,   -15,   128,  -284,  -284,     7,
     -13,   293
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    48,    49,    50,   304,    51,   354,   355,   356,   228,
     131,    52,   145,    53,    54,   134,   328,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
     208,   174,    68,    69,    70,    71,    72,   244,    73,   178,
     179,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,   279,
      92,    93
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     124,   102,   107,   232,   129,   231,   114,   233,   243,   300,
     133,   135,    96,    98,   100,   327,   103,   104,   222,   176,
     247,   241,     9,   324,   180,    32,   105,   202,   246,   203,
     118,   294,   111,   139,   359,   139,   368,   375,   140,   383,
     240,   106,   209,    32,     3,   249,     4,     1,     5,   225,
     138,   146,     2,   353,     3,   128,     4,   358,     5,   139,
     123,     7,     8,   186,   209,   204,   205,   181,   129,    32,
     187,     7,     8,   353,   230,   206,   108,   358,   123,    10,
     119,   218,   252,   219,   109,   239,   194,   195,    32,    39,
      32,    32,   110,    32,    24,    25,    26,    27,    28,    29,
     335,    94,   135,   202,   123,   203,  -121,   112,  -121,   116,
     207,    32,    33,    34,   216,   217,   296,   220,   221,   135,
     120,   224,   325,   123,   125,   123,   123,   251,   123,   130,
     289,   306,  -121,    41,    42,    43,   113,    45,    46,   192,
     193,   204,   205,    41,    42,    43,    95,    45,    46,    47,
     360,   206,   121,   122,   332,   333,   126,   127,   248,   196,
     197,   198,   282,   132,   369,   188,   189,   190,   191,   136,
     322,   243,   243,   273,   260,   261,   323,   137,   376,   141,
     377,   270,   271,   272,   100,   143,   207,   144,   274,   254,
     176,   262,   263,   264,   265,   384,   147,   266,   267,   175,
     177,   182,   176,   199,   268,   269,   183,   348,   184,   185,
     201,   276,   100,   210,   211,   212,   213,   214,   200,   215,
     223,   226,   227,   229,   209,   139,   203,   370,   234,   366,
       6,   235,   236,   237,   238,     5,   245,   253,   280,   373,
     277,   281,   278,   283,   284,   285,   286,   291,   287,   288,
     290,   293,   297,   295,   380,   292,   298,   301,   299,   305,
     302,   308,   303,   385,   309,   310,   387,   326,   330,   334,
     340,   329,   314,   315,   336,   307,   318,   319,   342,   337,
     346,   341,   343,   345,   347,   349,   351,   362,   363,   312,
     100,   365,   367,   371,   316,   317,   374,   379,   321,   378,
     142,   381,   382,   372,   386,   338,   352,   331,   350,   344,
     257,   329,   115,     0,     0,   255,   339,   256,     0,     0,
       0,   258,     0,     0,     0,   329,   135,   259,     0,     0,
     361,   357,     0,     1,   364,     0,     0,     0,     2,   329,
       3,   329,     4,     0,     5,     0,     6,     0,     0,     0,
       0,   357,     0,     0,     0,     0,   329,     7,     8,     0,
       9,     0,     0,     0,     0,    10,    11,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,   -11,    30,    31,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    33,    34,
       0,     0,     0,     0,     0,     0,     0,    35,    36,    37,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    38,     0,     0,     0,    39,    40,    41,
      42,    43,    44,    45,    46,    47,     1,     0,     0,     0,
       0,     2,     0,     3,     0,     4,     0,     5,     0,     6,
     -13,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     9,     0,     0,     0,     0,    10,    11,
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,     0,
      30,    31,     0,     0,     0,     0,     0,     0,     0,     0,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
      35,    36,    37,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    38,     0,     0,     0,
      39,    40,    41,    42,    43,    44,    45,    46,    47,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,     0,
       5,     0,     6,   -14,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     9,     0,     0,     0,
       0,    10,    11,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     0,     0,     0,     0,
       0,     0,     0,    35,    36,    37,     0,     0,     0,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,    38,
       5,     0,     6,    39,    40,    41,    42,    43,    44,    45,
      46,    47,     0,     7,     8,     0,     9,     0,     0,     0,
       0,    10,    11,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     0,     0,     0,     0,
       0,     0,     0,    35,    36,    37,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    38,
       0,     0,     0,    39,    40,    41,    42,    43,    44,    45,
      46,    47,     1,     0,     0,     0,     0,     2,     0,     3,
       0,     4,     0,     5,     0,     6,   101,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,    10,    11,     0,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,     0,    30,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
       0,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,     0,     5,     0,     6,     0,     0,    40,    41,    42,
      43,    44,    45,    46,    47,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    11,     0,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,     0,    30,     0,     0,     0,     0,
       0,     0,     0,   117,     0,    32,    33,    34,     2,     0,
       3,     0,     4,     0,     5,     0,     0,    37,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    40,    41,    42,    43,
      44,    45,    46,    47,     0,     0,     0,     0,     0,     0,
      24,    25,    26,    27,    28,    29,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    33,    34,
       2,     0,     3,     0,     4,    97,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,     0,     0,    10,     0,    41,
      42,    43,    95,    45,    46,     0,     0,     0,     0,     0,
       0,     0,    24,    25,    26,    27,    28,    29,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    32,
      33,    34,     2,     0,     3,     0,     4,     0,     5,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
       0,    41,    42,    43,    95,    45,    46,     0,     0,     0,
       0,     0,     0,     0,    24,    25,    26,    27,    28,    29,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    32,    33,    34,     2,     0,     3,     0,     4,     0,
       5,   250,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,    41,    42,    43,    95,    45,    46,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
      28,    29,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     2,     0,     3,     0,
       4,   275,     5,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,     0,    41,    42,    43,    95,    45,
      46,     0,     0,     0,     0,     0,     0,     0,    24,    25,
      26,    27,    28,    29,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,     2,     0,
       3,     0,     4,   311,     5,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,     0,    41,    42,    43,
      95,    45,    46,     0,     0,     0,     0,     0,     0,     0,
      24,    25,    26,    27,    28,    29,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    33,    34,
       2,     0,     3,     0,     4,     0,     5,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,     0,     0,    10,     0,    41,
      42,    43,    95,    45,    46,     0,     0,     0,     0,     0,
       0,     0,    24,    25,    26,    27,    28,    29,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    32,
      33,    34,     2,     0,     3,     0,     4,   320,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
       0,    41,    42,    43,    95,    45,    46,     0,     0,     0,
       0,     0,     0,     0,    24,    25,    26,    27,    28,    29,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    32,    33,    34,     2,     0,     3,     0,     4,     0,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,    41,    42,    43,    95,    45,    46,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
      28,    29,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     2,     0,     3,     0,
       4,     0,     5,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,     0,    41,    42,    43,    95,    45,
      46,     0,     0,     0,     0,     0,     0,     0,    24,    25,
      26,    27,    28,    29,     0,    94,     0,     0,     0,     0,
       0,   148,   149,   150,     0,    32,    33,    34,     2,     0,
       3,     0,     4,     0,     5,   151,   152,   153,   154,   155,
     156,   157,   158,   159,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,     0,    41,    42,    43,
     242,    45,    46,     0,     0,     0,     0,     0,     0,     0,
      24,    25,    26,    27,    28,    29,     0,     0,     0,     0,
     160,     0,     0,     0,     0,     0,     0,     0,    33,    34,
       0,     0,     0,     0,     0,     0,     0,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    41,
      42,    43,   113,    45,    46
};

static const yytype_int16 yycheck[] =
{
      26,     6,    13,   128,    30,   125,    19,   129,   137,     3,
      36,    37,     3,     4,     5,    13,     7,     8,   110,    69,
     145,   136,    30,     3,    33,    67,    12,    12,   143,    14,
      21,   234,   102,    29,    13,    29,    13,    13,    34,    13,
     135,    12,    92,    67,    10,   174,    12,     3,    14,    34,
      41,    56,     8,   336,    10,    79,    12,   336,    14,    29,
     102,    27,    28,    63,   114,    50,    51,    76,    94,    67,
      70,    27,    28,   356,   124,    60,    12,   356,   102,    35,
       3,    38,   177,    40,    12,   135,    68,    69,    67,    97,
      67,    67,    12,    67,    50,    51,    52,    53,    54,    55,
     303,    57,   128,    12,   102,    14,   102,    12,   102,    16,
      95,    67,    68,    69,   105,   106,   236,   108,   109,   145,
       3,   112,   102,   102,    32,   102,   102,   177,   102,   102,
     222,   253,   102,    99,   100,   101,   102,   103,   104,    71,
      72,    50,    51,    99,   100,   101,   102,   103,   104,   105,
     337,    60,    24,    25,   300,   301,    28,    29,   149,     7,
       8,     9,   212,    12,   351,    64,    65,    66,    67,    95,
     290,   300,   301,   199,   186,   187,   291,    16,   365,     0,
     367,   196,   197,   198,   175,    56,    95,    57,   201,   180,
     240,   188,   189,   190,   191,   382,     3,   192,   193,    14,
     102,    62,   252,    31,   194,   195,    74,   327,    75,    73,
      61,   202,   203,    11,    13,    15,    17,    13,    90,    13,
       3,    34,   102,     3,   274,    29,    14,   352,    60,   349,
      16,    65,    13,   102,    12,    14,    13,     5,    12,   359,
     102,    14,   102,    13,    13,    12,    12,     5,    13,    13,
      13,    12,    12,    29,   374,    17,   102,     3,    96,   102,
      17,    13,    59,   383,     5,    96,   386,   102,    13,     3,
      13,   297,   283,   284,    16,    34,   287,   288,    13,    12,
       3,    37,    13,    13,    13,    13,    12,     3,     3,   280,
     281,    12,     5,    17,   285,   286,    13,   102,   289,    13,
      49,    13,    12,   356,    13,   307,   331,   299,   329,   320,
     183,   337,    19,    -1,    -1,   181,   309,   182,    -1,    -1,
      -1,   184,    -1,    -1,    -1,   351,   352,   185,    -1,    -1,
     341,   336,    -1,     3,   345,    -1,    -1,    -1,     8,   365,
      10,   367,    12,    -1,    14,    -1,    16,    -1,    -1,    -1,
      -1,   356,    -1,    -1,    -1,    -1,   382,    27,    28,    -1,
      30,    -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,   103,   104,   105,     3,    -1,    -1,    -1,
      -1,     8,    -1,    10,    -1,    12,    -1,    14,    -1,    16,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    30,    -1,    -1,    -1,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,   104,   105,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    30,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    78,    79,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    93,
      14,    -1,    16,    97,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    27,    28,    -1,    30,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
     104,   105,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    16,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    67,    68,    69,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    52,    53,    54,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    54,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,     8,    -1,    10,    -1,    12,    -1,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    54,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,     8,    -1,    10,    -1,    12,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      54,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,     8,    -1,    10,    -1,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    54,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,     8,    -1,
      10,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    52,    53,    54,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
       8,    -1,    10,    -1,    12,    -1,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    54,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    99,   100,   101,   102,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    54,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      54,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    54,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,     4,     5,     6,    -1,    67,    68,    69,     8,    -1,
      10,    -1,    12,    -1,    14,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    52,    53,    54,    55,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,   103,   104
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    30,
      35,    36,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      57,    58,    67,    68,    69,    77,    78,    79,    93,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   107,   108,
     109,   111,   117,   119,   120,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   138,   139,
     140,   141,   142,   144,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   166,   167,    57,   102,   134,    13,   134,    15,
     134,    17,   117,   134,   134,    12,    12,   124,    12,    12,
      12,   102,    12,   102,   166,   167,    16,     3,   134,     3,
       3,   162,   162,   102,   142,    32,   162,   162,    79,   142,
     102,   116,    12,   142,   121,   142,    95,    16,   134,    29,
      34,     0,   107,    56,    57,   118,   117,     3,     4,     5,
       6,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      63,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,   137,    14,   138,   102,   145,   146,
      33,    76,    62,    74,    75,    73,    63,    70,    64,    65,
      66,    67,    71,    72,    68,    69,     7,     8,     9,    31,
     162,    61,    12,    14,    50,    51,    60,    95,   136,   138,
      11,    13,    15,    17,    13,    13,   134,   134,    38,    40,
     134,   134,   133,     3,   134,    34,    34,   102,   115,     3,
     138,   123,   121,   145,    60,    65,    13,   102,    12,   138,
     139,   115,   102,   140,   143,    13,   115,   121,   134,   140,
      15,   138,   139,     5,   134,   152,   153,   154,   155,   156,
     157,   157,   158,   158,   158,   158,   159,   159,   160,   160,
     161,   161,   161,   142,   166,    13,   134,   102,   102,   165,
      12,    14,   138,    13,    13,    12,    12,    13,    13,   133,
      13,     5,    17,    12,   116,    29,   123,    12,   102,    96,
       3,     3,    17,    59,   110,   102,   145,    34,    13,     5,
      96,    13,   134,    15,   124,   124,   134,   134,   124,   124,
      13,   134,   123,   115,     3,   102,   102,    13,   122,   142,
      13,   119,   143,   143,     3,   116,    16,    12,   150,   165,
      13,    37,    13,    13,   124,    13,     3,    13,   123,    13,
     146,    12,   118,   109,   112,   113,   114,   117,   141,    13,
     122,   124,     3,     3,   124,    12,   123,     5,    13,   122,
     121,    17,   113,   123,    13,    13,   122,   122,    13,   102,
     123,    13,    12,    13,   122,   123,    13,   123
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   106,   107,   107,   108,   108,   108,   109,   110,   110,
     111,   111,   112,   112,   113,   113,   114,   114,   114,   115,
     115,   116,   116,   117,   117,   118,   118,   119,   119,   120,
     120,   120,   120,   121,   121,   121,   122,   122,   123,   123,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   125,   125,   126,   127,   128,   128,   129,   130,
     130,   130,   130,   130,   130,   130,   131,   131,   132,   132,
     132,   132,   133,   133,   133,   134,   134,   135,   135,   136,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   138,   138,   138,   139,   139,
     139,   140,   140,   140,   141,   141,   141,   141,   141,   141,
     141,   142,   142,   142,   142,   143,   143,   143,   143,   144,
     145,   145,   146,   146,   146,   147,   148,   149,   150,   150,
     151,   151,   152,   152,   153,   153,   154,   154,   155,   155,
     156,   156,   156,   157,   157,   157,   157,   157,   158,   158,
     158,   159,   159,   159,   160,   160,   160,   160,   161,   161,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   163,
     163,   163,   163,   163,   164,   164,   165,   165,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167
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
       2,     1,     2,     3,     7,     8,     4,     6,     7,    11,
      12,     1,     2,     3,     4,     2,     3,     2,     3,     5,
       1,     3,     1,     2,     2,     3,     3,     3,     1,     5,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     1,     3,
       1,     2,     2,     2,     2,     2,     2,     3,     3,     1,
       1,     1,     1,     2,     1,     3,     1,     3,     1,     2,
       3,     4,     3,     2,     2,     4,     5,     1,     1,     1,
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
          case 121: /* type_decl2  */
#line 172 "eval/gwion.y" /* yacc.c:1257  */
      { printf("!!!!\n"); free_type_decl(((*yyvaluep).type_decl)); }
#line 1772 "eval/parser.c" /* yacc.c:1257  */
        break;

    case 142: /* type_decl  */
#line 172 "eval/gwion.y" /* yacc.c:1257  */
      { printf("!!!!\n"); free_type_decl(((*yyvaluep).type_decl)); }
#line 1778 "eval/parser.c" /* yacc.c:1257  */
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
#line 176 "eval/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL, get_pos(scanner));  }
#line 2046 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 177 "eval/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(scanner)); }
#line 2052 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 181 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2058 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 182 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(scanner)); }
#line 2064 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 183 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(scanner)); }
#line 2070 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 188 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].class_ext), (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->doc = get_doc(scanner);}
#line 2076 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 192 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = new_class_ext( (yyvsp[0].id_list), NULL, get_pos(scanner)); }
#line 2082 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 193 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = NULL; }
#line 2088 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 197 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_public; }
#line 2094 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 198 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_private; }
#line 2100 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 202 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2106 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 203 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2112 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 207 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body( (yyvsp[0].section), get_pos(scanner)); }
#line 2118 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 208 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body( (yyvsp[-1].section), (yyvsp[0].class_body), get_pos(scanner)); }
#line 2124 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 212 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list( (yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2130 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 213 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def( (yyvsp[0].func_def), get_pos(scanner)); }
#line 2136 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 214 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def( (yyvsp[0].class_def), get_pos(scanner)); }
#line 2142 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 218 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2148 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 219 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2154 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 223 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2160 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 224 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2166 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 228 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(scanner));}
#line 2172 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 229 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(scanner));}
#line 2178 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 233 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_static;   }
#line 2184 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 234 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_instance; }
#line 2190 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 238 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_func; }
#line 2196 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 239 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_variadic; }
#line 2202 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 243 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2208 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 244 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_key_static, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2214 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 245 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2220 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 246 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_key_static, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2226 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 250 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2232 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 251 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2238 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 252 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2244 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 256 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
#line 2250 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 257 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
#line 2256 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 261 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_code( NULL, get_pos(scanner)); }
#line 2262 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 262 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_code( (yyvsp[-1].stmt_list), get_pos(scanner)); }
#line 2268 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 281 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_enum((yyvsp[-2].id_list), NULL, get_pos(scanner)); }
#line 2274 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 282 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_enum((yyvsp[-3].id_list), (yyvsp[-1].sval), get_pos(scanner)); }
#line 2280 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 286 "eval/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_from_gotolabel((yyvsp[-1].sval), 1, get_pos(scanner)); }
#line 2286 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 290 "eval/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_from_gotolabel((yyvsp[-1].sval), 0, get_pos(scanner)); }
#line 2292 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 294 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2298 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 295 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2304 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 299 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner));}
#line 2310 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 304 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_while( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2316 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 306 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_while( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2322 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 308 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2328 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 310 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2334 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 312 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_until( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2340 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 314 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_until( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2346 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 316 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_loop( (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2352 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 321 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(scanner)); }
#line 2358 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 323 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2364 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 327 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_return( NULL, get_pos(scanner)); }
#line 2370 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 328 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_return( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2376 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 329 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_break(get_pos(scanner)); }
#line 2382 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 330 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_continue(get_pos(scanner)); }
#line 2388 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 334 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_expression((yyvsp[-1].exp),   get_pos(scanner)); }
#line 2394 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 335 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_expression(NULL, get_pos(scanner)); }
#line 2400 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 336 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_expression(NULL, get_pos(scanner)); append_doc(scanner, (yyvsp[0].sval)); }
#line 2406 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 340 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2412 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 341 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_expression((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2418 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 345 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2424 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 346 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2430 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 350 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2436 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 354 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2442 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 355 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2448 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 356 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; /* LCOV_EXCL_LINE */ }
#line 2454 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 357 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2460 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 358 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2466 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 359 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2472 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 360 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2478 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 361 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2484 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 362 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2490 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 363 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2496 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 364 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2502 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 365 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2508 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 366 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2514 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 367 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2520 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 368 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2526 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 369 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2532 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 370 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2538 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 371 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2544 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 372 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2550 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 373 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2556 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 374 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2562 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 375 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2568 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 376 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2574 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 377 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2580 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 378 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2586 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 382 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( (yyvsp[-1].exp), get_pos(scanner) ); }
#line 2592 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 383 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[0].array_sub), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2598 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 384 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( new_array_sub( NULL, get_pos(scanner)), (yyvsp[-3].exp), get_pos(scanner)); }
#line 2604 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 388 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( NULL, get_pos(scanner)); }
#line 2610 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 389 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-2].array_sub), NULL, get_pos(scanner)); }
#line 2616 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 390 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-1].array_sub), (yyvsp[0].array_sub)->exp_list, get_pos(scanner)); }
#line 2622 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 395 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_decl_expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); }
#line 2628 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 396 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_decl_expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); }
#line 2634 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 401 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2640 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 403 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-7].ival), (yyvsp[-6].ival), (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2646 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 405 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_key_func, ae_key_instance, new_type_decl(new_id_list("void", get_pos(scanner)), 0, get_pos(scanner)), "dtor", NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_dtor; (yyval.func_def)->type_decl->doc = get_doc(scanner);}
#line 2652 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 407 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_key_func, ae_key_static, (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_op; (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2658 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 409 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_key_func, ae_key_static, (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_op; (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2664 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 411 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); (yyval.func_def)->types = (yyvsp[-8].id_list); }
#line 2670 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 413 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-7].ival), (yyvsp[-6].ival), (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); (yyval.func_def)->types = (yyvsp[-9].id_list); }
#line 2676 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 417 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[0].sval), get_pos(scanner)), 0, get_pos(scanner)); }
#line 2682 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 418 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), 1, get_pos(scanner)); }
#line 2688 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 419 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-1].id_list),  0, get_pos(scanner)); }
#line 2694 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 420 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list),  1, get_pos(scanner)); }
#line 2700 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 425 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list(&(yyvsp[-1].exp)->d.exp_decl, NULL); }
#line 2706 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 426 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list(&(yyvsp[-2].exp)->d.exp_decl, (yyvsp[0].decl_list)); }
#line 2712 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 427 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = NULL; }
#line 2718 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 428 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list(NULL, (yyvsp[0].decl_list)); }
#line 2724 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 432 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_union((yyvsp[-2].decl_list), get_pos(scanner)); }
#line 2730 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 436 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2736 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 437 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2742 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 441 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sval), NULL, get_pos(scanner)); }
#line 2748 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 442 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2754 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 443 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2760 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 448 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2766 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 453 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar((yyvsp[-1].exp), get_pos(scanner)); }
#line 2772 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 456 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec((yyvsp[-1].exp), get_pos(scanner)); }
#line 2778 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 461 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_if_expression( (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2784 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 465 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2790 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 467 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2796 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 471 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2802 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 473 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2808 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 477 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2814 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 479 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2820 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 483 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2826 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 485 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(scanner)); }
#line 2832 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 489 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2838 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 491 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2844 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 495 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2850 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 497 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_eq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2856 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 499 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_neq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2862 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 503 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2868 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 505 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_lt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2874 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 507 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_gt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2880 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 509 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_le, (yyvsp[0].exp), get_pos(scanner)); }
#line 2886 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 511 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_ge, (yyvsp[0].exp), get_pos(scanner)); }
#line 2892 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 515 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2898 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 517 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_shift_left, (yyvsp[0].exp), get_pos(scanner)); }
#line 2904 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 519 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_shift_right, (yyvsp[0].exp), get_pos(scanner)); }
#line 2910 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 523 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2916 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 525 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_plus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2922 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 527 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_minus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2928 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 531 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2934 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 533 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_times, (yyvsp[0].exp), get_pos(scanner)); }
#line 2940 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 535 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_divide, (yyvsp[0].exp), get_pos(scanner)); }
#line 2946 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 537 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_percent, (yyvsp[0].exp), get_pos(scanner)); }
#line 2952 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 543 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_cast_expression( (yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2958 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 548 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2964 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 550 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_plusplus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2970 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 552 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_minusminus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2976 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 554 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2982 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 556 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_typeof, (yyvsp[0].exp), get_pos(scanner)); }
#line 2988 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 558 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_sizeof, (yyvsp[0].exp), get_pos(scanner)); }
#line 2994 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 560 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(scanner)); }
#line 3000 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 562 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary2(op_new, (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3006 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 564 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary3( op_spork, (yyvsp[0].stmt), get_pos(scanner)); }
#line 3012 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 568 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3018 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 569 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3024 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 570 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3030 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 571 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3036 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 572 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3042 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 577 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 3048 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 581 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sval), get_pos(scanner)), NULL, get_pos(scanner)); }
#line 3054 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 582 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sval), get_pos(scanner)), (yyvsp[0].type_list), get_pos(scanner)); }
#line 3060 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 588 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array( (yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3066 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 590 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_func_call( (yyvsp[-2].exp), NULL, get_pos(scanner)); }
#line 3072 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 592 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_func_call( (yyvsp[-3].exp), (yyvsp[-1].exp), get_pos(scanner)); }
#line 3078 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 594 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_member_dot( (yyvsp[-2].exp), (yyvsp[0].sval), get_pos(scanner)); }
#line 3084 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 596 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_postfix_expression( (yyvsp[-1].exp), op_plusplus, get_pos(scanner)); }
#line 3090 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 598 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_postfix_expression( (yyvsp[-1].exp), op_minusminus, get_pos(scanner)); }
#line 3096 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 600 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_func_call( (yyvsp[-3].exp), NULL, get_pos(scanner)); (yyval.exp)->d.exp_func.types = (yyvsp[-2].type_list); }
#line 3102 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 602 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_func_call( (yyvsp[-4].exp), (yyvsp[-1].exp), get_pos(scanner)); (yyval.exp)->d.exp_func.types = (yyvsp[-3].type_list); }
#line 3108 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 606 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_ID(     (yyvsp[0].sval), get_pos(scanner)); }
#line 3114 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 607 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_int(    (yyvsp[0].ival), get_pos(scanner)); }
#line 3120 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 608 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_float(  (yyvsp[0].fval), get_pos(scanner)); }
#line 3126 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 609 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_string( (yyvsp[0].sval), get_pos(scanner)); }
#line 3132 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 610 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_char(                  (yyvsp[0].sval), get_pos(scanner)); }
#line 3138 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 611 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_array_lit(             (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3144 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 612 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_array_lit(           (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3150 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 613 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_complex(               (yyvsp[0].c_val), get_pos(scanner)); }
#line 3156 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 614 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_polar(                 (yyvsp[0].polar), get_pos(scanner)); }
#line 3162 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 615 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_vec(                   (yyvsp[0].vec), get_pos(scanner)); }
#line 3168 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 616 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_hack_expression(                (yyvsp[-1].exp), get_pos(scanner)); }
#line 3174 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 617 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3180 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 618 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_nil(       get_pos(scanner)); }
#line 3186 "eval/parser.c" /* yacc.c:1646  */
    break;


#line 3190 "eval/parser.c" /* yacc.c:1646  */
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
#line 620 "eval/gwion.y" /* yacc.c:1906  */

