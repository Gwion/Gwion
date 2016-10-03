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
#line 6 "gwion.y" /* yacc.c:339  */

#include "absyn.h"
#define scanner arg->scanner

char *strcat(char *dest, const char *src);
void minimal_error(void* data, const char* s);
int minimal_lex(void*, void* , void*);
static int get_pos(void* data)
{
  MyArg* arg = (MyArg*)map_get(scan_map, data);
  return arg->line;
}

static char* append_doc(void* data, m_str str)
{
  MyArg* arg = (MyArg*)map_get(scan_map, data);
  vector_append(arg->doc, str);
  return str;
}

static m_str get_doc(void* data)
{
  m_str ret;
  MyArg* arg = (MyArg*)map_get(scan_map, data);
  ret = vector_front(arg->doc);
  vector_remove(arg->doc, 0);
//  ret = vector_back(arg->doc);
//  vector_remove(arg->doc, vector_size(arg->doc) - 1);
  return ret;
}

static m_str get_arg_doc(void* data)
{
  m_str ret;
  MyArg* arg = (MyArg*)map_get(scan_map, data);
  ret = vector_back(arg->doc);
  vector_pop(arg->doc);
//  ret = vector_at(arg->doc, 1);
//  vector_remove(arg->doc, 1);
  return ret;
}


#line 116 "parser.c" /* yacc.c:339  */

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
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.h".  */
#ifndef YY_MINIMAL_PARSER_H_INCLUDED
# define YY_MINIMAL_PARSER_H_INCLUDED
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
    PROTECTED = 284,
    ABSTRACT = 285,
    ATSYM = 286,
    FUNCTION = 287,
    DOLLAR = 288,
    TILDA = 289,
    QUESTION = 290,
    COLON = 291,
    EXCLAMATION = 292,
    IF = 293,
    ELSE = 294,
    WHILE = 295,
    DO = 296,
    UNTIL = 297,
    LOOP = 298,
    FOR = 299,
    GOTO = 300,
    SWITCH = 301,
    CASE = 302,
    ENUM = 303,
    RETURN = 304,
    BREAK = 305,
    CONTINUE = 306,
    PLUSPLUS = 307,
    MINUSMINUS = 308,
    NEW = 309,
    SPORK = 310,
    SIZEOF = 311,
    TYPEOF = 312,
    CLASS = 313,
    INTERFACE = 314,
    STATIC = 315,
    PRIVATE = 316,
    PUBLIC = 317,
    EXTENDS = 318,
    IMPLEMENTS = 319,
    DOT = 320,
    COLONCOLON = 321,
    AND = 322,
    EQ = 323,
    GE = 324,
    GT = 325,
    LE = 326,
    LT = 327,
    MINUS = 328,
    PLUS = 329,
    NEQ = 330,
    SHIFT_LEFT = 331,
    SHIFT_RIGHT = 332,
    S_AND = 333,
    S_OR = 334,
    S_XOR = 335,
    OR = 336,
    DTOR = 337,
    OPERATOR = 338,
    FUNC_PTR = 339,
    RSL = 340,
    RSR = 341,
    RSAND = 342,
    RSOR = 343,
    RSXOR = 344,
    RAND = 345,
    ROR = 346,
    REQ = 347,
    RNEQ = 348,
    RGT = 349,
    RGE = 350,
    RLT = 351,
    RLE = 352,
    RINC = 353,
    RDEC = 354,
    RUNINC = 355,
    RUNDEC = 356,
    TEMPLATE = 357,
    NOELSE = 358,
    ARROW_LEFT = 359,
    ARROW_RIGHT = 360,
    LTB = 361,
    GTB = 362,
    VARARG = 363,
    UNION = 364,
    ATPAREN = 365,
    NUM = 366,
    FLOAT = 367,
    ID = 368,
    STRING_LIT = 369,
    CHAR_LIT = 370,
    DOC = 371
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 50 "gwion.y" /* yacc.c:355  */

	char* sval;
	int ival;
	double fval;
  Complex* c_val;
  Polar* polar;
	Vec vec;
  Array_Sub array_sub;
  Var_Decl var_decl;
  Var_Decl_List var_decl_list;
  Type_Decl* type_decl;
  Expression   exp;
  Func_Ptr* func_ptr;
  Stmt* stmt;
  Stmt_List stmt_list;
  Arg_List arg_list;
  Decl_List decl_list; // for union
  Union* union_stmt; // for union
  Func_Def func_def;
  Section* section;
  ID_List id_list;
  Type_List type_list; // call template
  Class_Body class_body;
  Class_Ext class_ext;
  Class_Ext iface_ext;
  Class_Def class_def;
  Ast ast;

#line 302 "parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int minimal_parse (MyArg* arg);

#endif /* !YY_MINIMAL_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 318 "parser.c" /* yacc.c:358  */

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
#define YYFINAL  148
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1559

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  117
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  66
/* YYNRULES -- Number of rules.  */
#define YYNRULES  228
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  430

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   371

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
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   180,   180,   181,   185,   186,   187,   191,   193,   195,
     197,   202,   203,   204,   205,   209,   210,   211,   215,   216,
     220,   221,   225,   226,   227,   231,   235,   236,   240,   241,
     245,   246,   250,   251,   252,   256,   257,   261,   262,   263,
     264,   268,   269,   273,   274,   278,   279,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   298,
     299,   303,   307,   311,   312,   316,   320,   322,   324,   326,
     328,   330,   332,   337,   339,   344,   345,   346,   347,   351,
     352,   353,   357,   358,   362,   363,   366,   367,   371,   372,
     376,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     413,   414,   419,   420,   424,   425,   426,   427,   428,   429,
     430,   433,   435,   437,   442,   447,   449,   452,   458,   461,
     467,   468,   469,   470,   475,   476,   480,   483,   484,   488,
     489,   490,   494,   499,   503,   506,   507,   512,   513,   518,
     519,   524,   525,   530,   531,   536,   537,   542,   543,   545,
     550,   551,   553,   555,   557,   562,   563,   565,   570,   571,
     573,   578,   579,   581,   583,   588,   589,   594,   595,   601,
     602,   604,   606,   608,   610,   612,   614,   616,   621,   622,
     623,   624,   625,   626,   631,   632,   636,   637,   640,   641,
     645,   647,   649,   651,   653,   655,   657,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEMICOLON", "CHUCK", "COMMA", "ASSIGN",
  "DIVIDE", "TIMES", "PERCENT", "L_HACK", "R_HACK", "LPAREN", "RPAREN",
  "LBRACK", "RBRACK", "LBRACE", "RBRACE", "PLUSCHUCK", "MINUSCHUCK",
  "TIMESCHUCK", "DIVIDECHUCK", "MODULOCHUCK", "ATCHUCK", "UNCHUCK", "TRIG",
  "UNTRIG", "PERCENTPAREN", "SHARPPAREN", "PROTECTED", "ABSTRACT", "ATSYM",
  "FUNCTION", "DOLLAR", "TILDA", "QUESTION", "COLON", "EXCLAMATION", "IF",
  "ELSE", "WHILE", "DO", "UNTIL", "LOOP", "FOR", "GOTO", "SWITCH", "CASE",
  "ENUM", "RETURN", "BREAK", "CONTINUE", "PLUSPLUS", "MINUSMINUS", "NEW",
  "SPORK", "SIZEOF", "TYPEOF", "CLASS", "INTERFACE", "STATIC", "PRIVATE",
  "PUBLIC", "EXTENDS", "IMPLEMENTS", "DOT", "COLONCOLON", "AND", "EQ",
  "GE", "GT", "LE", "LT", "MINUS", "PLUS", "NEQ", "SHIFT_LEFT",
  "SHIFT_RIGHT", "S_AND", "S_OR", "S_XOR", "OR", "DTOR", "OPERATOR",
  "FUNC_PTR", "RSL", "RSR", "RSAND", "RSOR", "RSXOR", "RAND", "ROR", "REQ",
  "RNEQ", "RGT", "RGE", "RLT", "RLE", "RINC", "RDEC", "RUNINC", "RUNDEC",
  "TEMPLATE", "NOELSE", "ARROW_LEFT", "ARROW_RIGHT", "LTB", "GTB",
  "VARARG", "UNION", "ATPAREN", "NUM", "FLOAT", "ID", "STRING_LIT",
  "CHAR_LIT", "DOC", "$accept", "ast", "section", "class_def", "class_ext",
  "class_decl", "class_body", "class_body2", "class_section", "iface_ext",
  "id_list", "id_dot", "stmt_list", "static_decl", "function_decl",
  "func_ptr", "type_decl2", "arg_list", "code_segment", "stmt",
  "enum_stmt", "label_stmt", "goto_stmt", "case_stmt", "switch_stmt",
  "loop_stmt", "selection_stmt", "jump_stmt", "exp_stmt", "exp",
  "arrow_operator", "binary_exp", "arrow_expression", "template", "op",
  "array_exp", "array_empty", "decl_exp", "func_def", "type_decl",
  "decl_list", "union", "var_decl_list", "var_decl", "complex_exp",
  "polar_exp", "vec_exp", "conditional_expression",
  "logical_or_expression", "logical_and_expression",
  "inclusive_or_expression", "exclusive_or_expression", "and_expression",
  "equality_expression", "relational_expression", "shift_expression",
  "additive_expression", "multiplicative_expression", "tilda_expression",
  "cast_exp", "unary_expression", "unary_operator", "dur_exp", "type_list",
  "postfix_exp", "primary_exp", YY_NULLPTR
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
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371
};
# endif

#define YYPACT_NINF -334

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-334)))

#define YYTABLE_NINF -141

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     704,  -334,  -334,  1444,  1109,  1444,   818,  1444,  1444,   -30,
    -334,  -334,  -334,    29,    67,   908,   103,   119,   122,   -87,
     140,   141,   148,  1042,   162,   164,   129,   129,   -30,   139,
     129,   129,    10,   -30,  -334,    62,  -334,  -334,   175,   -30,
     -30,    70,  -334,   172,  1444,  -334,  -334,    17,  -334,  -334,
    -334,   191,   385,  -334,   -14,  -334,    13,  -334,  -334,   908,
    -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,   190,
    1441,   -44,  -334,  -334,  -334,    81,  -334,  -334,  -334,  -334,
    -334,    31,   128,   118,   121,   120,    36,   137,    -2,    26,
     171,   170,   177,  -334,   129,   145,    44,  -334,     6,   -30,
     -11,   201,  -334,   204,   203,  -334,   208,   207,   213,   196,
      81,  1444,  1444,    98,  1444,  1444,   975,   218,  1444,  -334,
      40,   192,   116,  -334,   227,  -334,  -334,  -334,  -334,   217,
     216,  -334,  -334,   -30,   -30,   -30,    81,    81,   168,   165,
     221,   124,   226,   116,  1444,   232,  -334,  -334,  -334,  -334,
     116,   116,  -334,  -334,   -30,  -334,  -334,  -334,  1444,  -334,
    -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,
    -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,
    -334,  -334,  -334,  -334,  -334,  -334,  -334,  1444,  -334,  -334,
    1444,   233,  -334,   241,  1444,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   -30,  -334,   141,  1176,  -334,  -334,
     135,   136,   238,  -334,  -334,  -334,   217,  -334,  -334,  -334,
    -334,   245,   246,   248,   249,   250,   251,   975,  -334,   252,
    -334,  -334,   257,   254,  -334,  -334,  -334,    81,    81,   255,
    -334,  -334,    62,   235,   216,   260,   156,   166,   271,   258,
    -334,    43,    79,   163,   263,  -334,   -44,  -334,  1243,  -334,
     264,    81,   244,   128,   118,   121,   120,    36,   137,   137,
      -2,    -2,    -2,    -2,    26,    26,   171,   171,   170,   170,
     170,   177,  -334,    44,  -334,   268,  -334,   277,   176,  1310,
    -334,   908,   908,  1444,  1444,   908,   908,  1377,   216,   116,
      14,  -334,  -334,   173,  -334,  -334,  -334,     5,   272,     0,
    1444,   281,   499,    62,   116,   274,   499,   116,   275,   282,
     280,   264,  -334,   283,  -334,   129,  -334,   136,  -334,  -334,
     286,   261,  -334,   289,   290,  -334,  -334,   908,   291,  -334,
    -334,  -334,   284,   298,   216,   299,    81,   301,    13,  -334,
    -334,  -334,   279,  -334,   613,  -334,  -334,   253,   256,   499,
     297,  -334,   499,     8,  -334,  -334,  -334,  -334,   908,   312,
     313,  -334,   908,  -334,   306,  -334,   216,   315,     9,   -30,
    -334,  -334,   116,    62,   304,  -334,   305,    46,   310,  -334,
    -334,  -334,  -334,    15,  -334,   -30,  -334,   311,   212,  -334,
    -334,  -334,  -334,  -334,  -334,    47,  -334,   314,  -334,  -334,
     316,  -334,  -334,  -334,    16,   216,   317,  -334,   216,  -334
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      17,    80,   202,     0,     0,     0,     0,     0,     0,     0,
      35,   200,   201,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    16,    15,     0,   199,   198,     0,     0,
       0,     0,    36,     0,     0,   218,   219,   217,   220,   221,
      81,     0,    17,     6,     0,     4,    34,    57,    50,    30,
      55,    51,    52,    54,    53,    48,    49,    56,    47,     0,
      82,    86,   222,    88,     5,     0,    58,   223,   224,   225,
     124,   155,   157,   159,   161,   163,   165,   167,   170,   175,
     178,   181,   185,   187,     0,   189,   204,   208,     0,     0,
     217,     0,   228,     0,     0,    45,     0,     0,     0,   140,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   217,
       0,   208,     0,    75,     0,    77,    78,   190,   191,   195,
     203,   194,   193,     0,     0,     0,     0,     0,    28,     0,
       0,     0,     0,     0,     0,     0,   141,    61,     1,     3,
       0,     0,    33,    32,     0,    31,    79,    91,     0,   119,
      94,    95,    96,    97,    98,    93,    92,    99,   100,   118,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,     0,    84,    85,
       0,   149,   125,   147,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   192,     0,     0,   213,   214,
       0,     0,     0,   209,   226,   227,   120,    46,   153,   152,
     129,     0,     0,     0,     0,     0,     0,     0,    62,     0,
      64,    63,    26,     0,    76,   196,   197,     0,     0,     0,
     126,   127,     0,   142,     0,     0,     0,     0,     0,     0,
     154,     0,     0,     0,    41,    83,    87,    89,     0,   150,
     151,     0,     0,   158,   160,   162,   164,   166,   168,   169,
     174,   172,   173,   171,   176,   177,   180,   179,   183,   182,
     184,   186,   188,   205,   210,     0,   212,   206,     0,     0,
     121,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   130,   128,     0,    29,   143,   135,     0,     0,     0,
     144,     0,    17,     0,     0,     0,    17,     0,     0,     0,
       0,    42,   122,     0,   148,     0,   211,     0,    90,   215,
       0,    73,    66,     0,     0,    70,    72,     0,     0,    65,
      27,    59,     0,     0,     0,     0,     0,     0,    34,   145,
     146,    24,     0,    18,    17,    22,    23,    13,    11,    17,
       0,    25,    17,     0,   123,   156,   207,   216,     0,     0,
       0,    68,     0,    60,     0,   136,     0,    43,     0,     0,
       7,    21,     0,     0,     0,     9,     0,     0,     0,    74,
      67,    71,    69,     0,   137,     0,    37,     0,     0,    14,
      12,     8,    10,   132,   131,     0,    38,     0,    44,    39,
       0,   134,   133,    40,     0,     0,     0,   138,     0,   139
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -334,   285,  -334,    33,  -334,  -334,  -225,   -35,  -334,  -334,
    -135,  -239,    51,   -32,    19,  -334,   -58,  -333,  -118,   -13,
    -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,  -102,     2,
    -334,  -334,   146,  -334,  -334,   -93,    68,  -143,    34,    -9,
      20,  -334,   -99,   -21,  -334,  -334,  -334,     1,  -334,   144,
     147,   149,   143,   150,   -42,    11,   -45,   -38,    12,   131,
      38,  -334,  -334,    18,   -17,   321
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    51,    52,   361,   325,    54,   362,   363,   364,   328,
     243,   139,   365,   154,    56,    57,   263,   355,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
     190,    70,    71,   222,   187,    72,   270,    73,   366,    75,
     259,    76,   192,   193,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,   298,    96,    97
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     110,   258,   113,   223,   120,   101,   103,   104,   257,   107,
     108,   230,   246,   314,   237,   261,   262,   351,   354,   129,
     146,   397,   406,   136,   137,   124,   117,   223,   416,   425,
     141,   142,    10,    53,    74,   133,   245,   250,   251,   133,
     398,   111,    35,   152,   150,   151,   145,   267,   146,   413,
     421,    55,   217,   147,     5,   407,   217,   106,     5,   322,
     188,   189,     6,     6,   127,   128,   194,   134,   131,   132,
     417,   134,   418,   153,   206,   207,   240,    35,    35,   112,
      35,    35,    35,   109,   367,    53,    74,    35,    35,   136,
     137,   426,   218,   219,   135,   326,   218,   219,   269,   208,
     209,   370,  -140,    55,   200,   220,   323,   324,    42,   220,
     155,   201,   195,   231,   232,   114,   235,   236,   109,   109,
     239,   109,   109,   109,   247,   248,   249,   352,   109,   109,
    -140,   115,   215,   300,   116,   307,   316,     2,   233,     3,
     234,     4,   327,     5,   394,   264,   221,   396,   311,   312,
     221,     3,   118,     4,   410,     5,     7,     8,   278,   279,
     265,   284,   285,    11,   122,   125,    12,   126,     7,     8,
     286,   287,   334,   130,   350,   138,   143,   258,   210,   211,
     212,    26,    27,    28,    29,    30,    31,   140,   144,   368,
     349,   148,   371,   156,   191,   196,   272,   197,   199,   293,
     223,   198,    36,    37,   213,   292,   202,   203,   204,   205,
     214,   216,   224,   280,   281,   282,   283,   225,   226,   295,
     228,   238,   288,   289,   290,   227,   229,   146,   241,   242,
     244,     5,     6,   252,   254,   253,   385,   255,   256,    44,
      45,    46,   119,    48,    49,   260,   271,   268,   296,   297,
     299,    44,    45,    46,   119,    48,    49,   409,   301,   302,
     303,   304,   309,   305,   306,   308,   315,   313,   404,   318,
     104,   310,   317,   319,   320,   321,   329,   330,   333,   414,
     335,   336,   337,   338,   360,   357,   353,   383,   341,   342,
     369,   372,   345,   346,   373,   332,   390,   422,   374,   377,
     378,   340,   379,   380,   382,   343,   344,   427,   356,   348,
     429,   384,   386,   388,   395,   400,   401,   392,   403,   393,
     405,   411,   412,   415,   419,   420,   389,   423,   424,   391,
     428,   408,   331,   266,   381,   387,   375,   149,   358,   273,
     359,   276,   121,   274,   291,     0,   275,     0,     0,   277,
       0,     0,     0,     0,     0,   376,     0,     0,     0,     0,
       0,     0,     0,     0,   356,   399,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
     264,     0,     0,     0,     0,    -2,     0,     0,     1,     0,
       0,     0,     0,     2,   356,     3,   356,     4,     0,     5,
       0,     6,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     9,   356,     0,    10,     0,    11,
       0,     0,    12,    13,     0,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,    32,    33,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    35,    36,    37,
       0,     0,     0,     0,     0,     0,     0,    38,    39,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,    42,    43,    44,    45,    46,    47,    48,
      49,    50,     1,     0,     0,     0,     0,     2,     0,     3,
       0,     4,     0,     5,     0,     6,   -19,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     9,     0,
       0,    10,     0,    11,     0,     0,    12,    13,     0,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,     0,     0,    32,
      33,    34,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    35,    36,    37,     0,     0,     0,     0,     0,     0,
       0,    38,    39,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    41,     0,     0,     0,     0,     0,    42,    43,    44,
      45,    46,    47,    48,    49,    50,     1,     0,     0,     0,
       0,     2,     0,     3,     0,     4,     0,     5,     0,     6,
     -20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     9,     0,     0,    10,     0,    11,     0,     0,
      12,    13,     0,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,     0,    32,    33,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    35,    36,    37,     0,     0,
       0,     0,     0,     0,     0,    38,    39,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     0,     0,
       0,     0,     2,     0,     3,    41,     4,     0,     5,     0,
       6,    42,    43,    44,    45,    46,    47,    48,    49,    50,
       0,     7,     8,     9,     0,     0,    10,     0,    11,     0,
       0,    12,    13,     0,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,     0,     0,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    35,    36,    37,     0,
       0,     0,     0,     0,     0,     0,    38,    39,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    41,     0,     0,     0,
       0,     0,    42,    43,    44,    45,    46,    47,    48,    49,
      50,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,     0,     5,     0,     6,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     9,     0,     0,
       0,     0,    11,     0,     0,    12,    13,     0,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     0,     0,    32,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      35,    36,    37,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,     0,     5,     0,     6,     0,     0,    43,    44,    45,
      46,    47,    48,    49,    50,     7,     8,     9,     0,     0,
       0,     0,    11,     0,     0,    12,    13,     0,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     0,     0,    32,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     1,     0,
      35,    36,    37,     2,     0,     3,     0,     4,     0,     5,
       0,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     9,     0,     0,     0,     0,    11,
       0,     0,    12,     0,     0,     0,     0,    43,    44,    45,
      46,    47,    48,    49,    50,     0,     0,    26,    27,    28,
      29,    30,    31,     0,     0,    98,    99,     0,     0,     0,
       0,     0,     0,     0,     0,   123,     0,    35,    36,    37,
       2,     0,     3,     0,     4,     0,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     9,     0,     0,     0,     0,    11,     0,     0,    12,
       0,     0,     0,     0,     0,    44,    45,    46,   100,    48,
      49,    50,     0,     0,    26,    27,    28,    29,    30,    31,
       0,     0,    98,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    35,    36,    37,     2,     0,     3,
       0,     4,   102,     5,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     9,     0,
       0,     0,     0,    11,     0,     0,    12,     0,     0,     0,
       0,     0,    44,    45,    46,   100,    48,    49,     0,     0,
       0,    26,    27,    28,    29,    30,    31,     0,     0,    98,
      99,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    35,    36,    37,     2,     0,     3,     0,     4,   294,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     9,     0,     0,     0,     0,
      11,     0,     0,    12,     0,     0,     0,     0,     0,    44,
      45,    46,   100,    48,    49,     0,     0,     0,    26,    27,
      28,    29,    30,    31,     0,     0,    98,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    35,    36,
      37,     2,     0,     3,     0,     4,     0,     5,   332,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     9,     0,     0,     0,     0,    11,     0,     0,
      12,     0,     0,     0,     0,     0,    44,    45,    46,   100,
      48,    49,     0,     0,     0,    26,    27,    28,    29,    30,
      31,     0,     0,    98,    99,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    35,    36,    37,     2,     0,
       3,     0,     4,   339,     5,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     9,
       0,     0,     0,     0,    11,     0,     0,    12,     0,     0,
       0,     0,     0,    44,    45,    46,   100,    48,    49,     0,
       0,     0,    26,    27,    28,    29,    30,    31,     0,     0,
      98,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    35,    36,    37,     2,     0,     3,     0,     4,
     347,     5,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     7,     8,     9,     0,     0,     0,
       0,    11,     0,     0,    12,     0,     0,     0,     0,     0,
      44,    45,    46,   100,    48,    49,     0,     0,     0,    26,
      27,    28,    29,    30,    31,     0,     0,    98,    99,     0,
       0,     0,     0,     0,     0,   157,   158,   159,     0,    35,
      36,    37,     2,     0,     3,     0,     4,     0,     5,   160,
     161,   162,   163,   164,   165,   166,   167,   168,     0,     0,
       0,     7,     8,     9,     0,     0,     0,     0,    11,     0,
       0,    12,     0,     0,     0,     0,     0,    44,    45,    46,
     100,    48,    49,     0,     0,     0,    26,    27,    28,    29,
      30,    31,     0,     0,    98,    99,     0,     0,     0,   169,
       0,     0,     0,     0,     0,     0,    35,    36,    37,     0,
       0,     0,     0,     0,     0,     0,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,    45,    46,   100,    48,    49
};

static const yytype_int16 yycheck[] =
{
       9,   144,    15,    96,    21,     3,     4,     5,   143,     7,
       8,   110,   130,   252,   116,   150,   151,     3,    13,    28,
      31,    13,    13,    32,    33,    23,   113,   120,    13,    13,
      39,    40,    32,     0,     0,    29,   129,   136,   137,    29,
     373,    12,    72,    30,    58,    59,    44,   190,    31,     3,
       3,     0,    12,    36,    14,   388,    12,     6,    14,    16,
     104,   105,    16,    16,    26,    27,    35,    61,    30,    31,
     403,    61,   405,    60,    76,    77,    36,    72,    72,    12,
      72,    72,    72,   113,   323,    52,    52,    72,    72,    98,
      99,   424,    52,    53,    84,    16,    52,    53,   191,    73,
      74,   326,   113,    52,    68,    65,    63,    64,   108,    65,
      59,    75,    81,   111,   112,    12,   114,   115,   113,   113,
     118,   113,   113,   113,   133,   134,   135,   113,   113,   113,
     113,    12,    94,   226,    12,   237,   254,     8,    40,    10,
      42,    12,    63,    14,   369,   154,   106,   372,   247,   248,
     106,    10,    12,    12,   393,    14,    27,    28,   200,   201,
     158,   206,   207,    34,    16,     3,    37,     3,    27,    28,
     208,   209,   271,    34,   309,   113,   106,   320,     7,     8,
       9,    52,    53,    54,    55,    56,    57,    12,    16,   324,
     308,     0,   327,     3,   113,    67,   194,    79,    78,   216,
     293,    80,    73,    74,    34,   214,    69,    70,    71,    72,
      33,    66,    11,   202,   203,   204,   205,    13,    15,   217,
      13,     3,   210,   211,   212,    17,    13,    31,    36,   113,
       3,    14,    16,    65,    13,    70,   354,   113,    12,   110,
     111,   112,   113,   114,   115,    13,     5,    14,   113,   113,
      12,   110,   111,   112,   113,   114,   115,   392,    13,    13,
      12,    12,     5,    13,    13,    13,    31,    12,   386,   113,
     268,    17,    12,   107,     3,    17,   113,    14,    14,   397,
      36,    13,     5,   107,     3,    13,   113,     3,   301,   302,
      16,    16,   305,   306,    12,    15,    17,   415,    15,    13,
      39,   299,    13,    13,    13,   303,   304,   425,   317,   307,
     428,    13,    13,    12,    17,     3,     3,    64,    12,    63,
       5,    17,    17,    13,    13,   113,   358,    13,    12,   364,
      13,   389,   264,   187,   347,   356,   335,    52,   319,   195,
     320,   198,    21,   196,   213,    -1,   197,    -1,    -1,   199,
      -1,    -1,    -1,    -1,    -1,   337,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   373,   378,    -1,    -1,    -1,   382,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   388,
     389,    -1,    -1,    -1,    -1,     0,    -1,    -1,     3,    -1,
      -1,    -1,    -1,     8,   403,    10,   405,    12,    -1,    14,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,   424,    -1,    32,    -1,    34,
      -1,    -1,    37,    38,    -1,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    -1,    60,    61,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,
      -1,    32,    -1,    34,    -1,    -1,    37,    38,    -1,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,     3,    -1,    -1,    -1,
      -1,     8,    -1,    10,    -1,    12,    -1,    14,    -1,    16,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    29,    -1,    -1,    32,    -1,    34,    -1,    -1,
      37,    38,    -1,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    -1,    60,    61,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,     8,    -1,    10,   102,    12,    -1,    14,    -1,
      16,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      -1,    27,    28,    29,    -1,    -1,    32,    -1,    34,    -1,
      -1,    37,    38,    -1,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    61,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    37,    38,    -1,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    16,    -1,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,    27,    28,    29,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    37,    38,    -1,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      72,    73,    74,     8,    -1,    10,    -1,    12,    -1,    14,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    37,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,    -1,    -1,    52,    53,    54,
      55,    56,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    72,    73,    74,
       8,    -1,    10,    -1,    12,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    -1,    -1,    -1,    -1,    34,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,    52,    53,    54,    55,    56,    57,
      -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,     8,    -1,    10,
      -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,     8,    -1,    10,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,   114,   115,    -1,    -1,    -1,    52,    53,
      54,    55,    56,    57,    -1,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,     8,    -1,    10,    -1,    12,    -1,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    29,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
     114,   115,    -1,    -1,    -1,    52,    53,    54,    55,    56,
      57,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,     8,    -1,
      10,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,   114,   115,    -1,
      -1,    -1,    52,    53,    54,    55,    56,    57,    -1,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,     8,    -1,    10,    -1,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,    52,
      53,    54,    55,    56,    57,    -1,    -1,    60,    61,    -1,
      -1,    -1,    -1,    -1,    -1,     4,     5,     6,    -1,    72,
      73,    74,     8,    -1,    10,    -1,    12,    -1,    14,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      -1,    27,    28,    29,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,   115
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    29,
      32,    34,    37,    38,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    60,    61,    62,    72,    73,    74,    82,    83,
      84,   102,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   118,   119,   120,   122,   129,   131,   132,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     148,   149,   152,   154,   155,   156,   158,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   181,   182,    60,    61,
     113,   146,    13,   146,   146,    17,   129,   146,   146,   113,
     156,    12,    12,   136,    12,    12,    12,   113,    12,   113,
     181,   182,    16,     3,   146,     3,     3,   177,   177,   156,
      34,   177,   177,    29,    61,    84,   156,   156,   113,   128,
      12,   156,   156,   106,    16,   146,    31,    36,     0,   118,
      58,    59,    30,    60,   130,   129,     3,     4,     5,     6,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    68,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   151,   104,   105,
     147,   113,   159,   160,    35,    81,    67,    79,    80,    78,
      68,    75,    69,    70,    71,    72,    76,    77,    73,    74,
       7,     8,     9,    34,    33,   177,    66,    12,    52,    53,
      65,   106,   150,   152,    11,    13,    15,    17,    13,    13,
     159,   146,   146,    40,    42,   146,   146,   145,     3,   146,
      36,    36,   113,   127,     3,   152,   135,   156,   156,   156,
     159,   159,    65,    70,    13,   113,    12,   127,   154,   157,
      13,   127,   127,   133,   156,   146,   149,   154,    14,   152,
     153,     5,   146,   166,   167,   168,   169,   170,   171,   171,
     172,   172,   172,   172,   173,   173,   174,   174,   175,   175,
     175,   176,   156,   181,    13,   146,   113,   113,   180,    12,
     152,    13,    13,    12,    12,    13,    13,   145,    13,     5,
      17,   159,   159,    12,   128,    31,   135,    12,   113,   107,
       3,    17,    16,    63,    64,   121,    16,    63,   126,   113,
      14,   153,    15,    14,   159,    36,    13,     5,   107,    13,
     146,   136,   136,   146,   146,   136,   136,    13,   146,   135,
     127,     3,   113,   113,    13,   134,   156,    13,   131,   157,
       3,   120,   123,   124,   125,   129,   155,   128,   127,    16,
     123,   127,    16,    12,    15,   164,   180,    13,    39,    13,
      13,   136,    13,     3,    13,   135,    13,   160,    12,   130,
      17,   124,    64,    63,   123,    17,   123,    13,   134,   136,
       3,     3,   136,    12,   135,     5,    13,   134,   133,   127,
     128,    17,    17,     3,   135,    13,    13,   134,   134,    13,
     113,     3,   135,    13,    12,    13,   134,   135,    13,   135
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   117,   118,   118,   119,   119,   119,   120,   120,   120,
     120,   121,   121,   121,   121,   122,   122,   122,   123,   123,
     124,   124,   125,   125,   125,   126,   127,   127,   128,   128,
     129,   129,   130,   130,   130,   131,   131,   132,   132,   132,
     132,   133,   133,   134,   134,   135,   135,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   137,
     137,   138,   139,   140,   140,   141,   142,   142,   142,   142,
     142,   142,   142,   143,   143,   144,   144,   144,   144,   145,
     145,   145,   146,   146,   147,   147,   148,   148,   149,   149,
     150,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     152,   152,   153,   153,   154,   154,   154,   154,   154,   154,
     154,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     156,   156,   156,   156,   157,   157,   158,   159,   159,   160,
     160,   160,   161,   162,   163,   164,   164,   165,   165,   166,
     166,   167,   167,   168,   168,   169,   169,   170,   170,   170,
     171,   171,   171,   171,   171,   172,   172,   172,   173,   173,
     173,   174,   174,   174,   174,   175,   175,   176,   176,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   178,   178,
     178,   178,   178,   178,   179,   179,   180,   180,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     6,     7,     6,
       7,     2,     4,     2,     4,     1,     1,     0,     1,     0,
       1,     2,     1,     1,     1,     2,     1,     3,     1,     3,
       1,     2,     1,     1,     0,     1,     1,     7,     8,     8,
       9,     1,     2,     2,     4,     2,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     5,
       6,     2,     3,     3,     3,     5,     5,     7,     6,     7,
       5,     7,     5,     5,     7,     2,     3,     2,     2,     2,
       1,     1,     1,     3,     1,     1,     1,     3,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     2,     3,     1,     2,     3,     3,     4,     3,
       4,     7,     7,     8,     8,     4,     6,     7,    11,    12,
       1,     2,     3,     4,     2,     3,     5,     1,     3,     1,
       2,     2,     3,     3,     3,     1,     5,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     1,     3,     1,     3,     1,
       2,     2,     2,     2,     2,     2,     3,     3,     1,     1,
       1,     1,     1,     2,     1,     3,     1,     3,     1,     2,
       3,     4,     3,     2,     2,     4,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     2
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
  YYUSE (yytype);
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
#line 180 "gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_Ast((yyvsp[0].section), NULL, get_pos(scanner));  }
#line 1956 "parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 181 "gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_Ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(scanner)); }
#line 1962 "parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 185 "gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Stmt_List((yyvsp[0].stmt_list), get_pos(scanner)); }
#line 1968 "parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 186 "gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Func_Def ((yyvsp[0].func_def), get_pos(scanner)); }
#line 1974 "parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 187 "gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Class_Def((yyvsp[0].class_def), get_pos(scanner)); }
#line 1980 "parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 192 "gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-5].ival), (yyvsp[-3].id_list), NULL, (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->doc = get_doc(scanner); }
#line 1986 "parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 194 "gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].class_ext), (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->doc = get_doc(scanner);}
#line 1992 "parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 196 "gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_iface_def( (yyvsp[-5].ival), (yyvsp[-3].id_list), NULL, (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->doc = get_doc(scanner);}
#line 1998 "parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 198 "gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_iface_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].class_ext), (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->doc = get_doc(scanner);}
#line 2004 "parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 202 "gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = new_class_ext( NULL, (yyvsp[0].id_list), get_pos(scanner)); }
#line 2010 "parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 203 "gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = new_class_ext( (yyvsp[0].id_list), (yyvsp[-2].id_list), get_pos(scanner)); }
#line 2016 "parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 204 "gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = new_class_ext( (yyvsp[0].id_list), NULL, get_pos(scanner)); }
#line 2022 "parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 205 "gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = new_class_ext( (yyvsp[-2].id_list), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2028 "parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 209 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_public; }
#line 2034 "parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 210 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_private; }
#line 2040 "parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 211 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_private; }
#line 2046 "parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 215 "gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2052 "parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 216 "gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2058 "parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 220 "gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body( (yyvsp[0].section), get_pos(scanner)); }
#line 2064 "parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 221 "gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body( (yyvsp[-1].section), (yyvsp[0].class_body), get_pos(scanner)); }
#line 2070 "parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 225 "gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Stmt_List( (yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2076 "parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 226 "gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Func_Def( (yyvsp[0].func_def), get_pos(scanner)); }
#line 2082 "parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 227 "gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Class_Def( (yyvsp[0].class_def), get_pos(scanner)); }
#line 2088 "parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 231 "gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = new_class_ext( NULL, (yyvsp[0].id_list), get_pos(scanner)); }
#line 2094 "parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 235 "gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2100 "parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 236 "gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2106 "parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 240 "gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2112 "parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 241 "gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2118 "parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 245 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_Stmt_List((yyvsp[0].stmt), NULL, get_pos(scanner));}
#line 2124 "parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 246 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_Stmt_List((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(scanner));}
#line 2130 "parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 250 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_static;   }
#line 2136 "parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 251 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_abstract; }
#line 2142 "parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 252 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_instance; }
#line 2148 "parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 256 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_func; }
#line 2154 "parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 257 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_variadic; }
#line 2160 "parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 261 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Func_Ptr_Stmt(0, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2166 "parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 262 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Func_Ptr_Stmt(1, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2172 "parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 263 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Func_Ptr_Stmt(0, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2178 "parser.c" /* yacc.c:1646  */
    break;

  case 40:
#line 264 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Func_Ptr_Stmt(1, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2184 "parser.c" /* yacc.c:1646  */
    break;

  case 41:
#line 268 "gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2190 "parser.c" /* yacc.c:1646  */
    break;

  case 42:
#line 269 "gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2196 "parser.c" /* yacc.c:1646  */
    break;

  case 43:
#line 273 "gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_Arg_List((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(scanner)); (yyval.arg_list)->doc = get_arg_doc(scanner); }
#line 2202 "parser.c" /* yacc.c:1646  */
    break;

  case 44:
#line 274 "gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_Arg_List((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(scanner)); (yyval.arg_list)->doc = get_arg_doc(scanner); }
#line 2208 "parser.c" /* yacc.c:1646  */
    break;

  case 45:
#line 278 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_code( NULL, get_pos(scanner)); }
#line 2214 "parser.c" /* yacc.c:1646  */
    break;

  case 46:
#line 279 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_code( (yyvsp[-1].stmt_list), get_pos(scanner)); }
#line 2220 "parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 294 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_Union((yyvsp[0].union_stmt), 0); }
#line 2226 "parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 298 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_enum((yyvsp[-2].id_list), NULL, get_pos(scanner)); }
#line 2232 "parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 299 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_enum((yyvsp[-3].id_list), (yyvsp[-1].sval), get_pos(scanner)); }
#line 2238 "parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 303 "gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_from_gotolabel((yyvsp[-1].sval), 1, get_pos(scanner)); }
#line 2244 "parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 307 "gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_from_gotolabel((yyvsp[-1].sval), 0, get_pos(scanner)); }
#line 2250 "parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 311 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2256 "parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 312 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2262 "parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 316 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner));}
#line 2268 "parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 321 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_while( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2274 "parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 323 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_while( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2280 "parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 325 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2286 "parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 327 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2292 "parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 329 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_until( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2298 "parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 331 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_until( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2304 "parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 333 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_loop( (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2310 "parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 338 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(scanner)); }
#line 2316 "parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 340 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2322 "parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 344 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_return( NULL, get_pos(scanner)); }
#line 2328 "parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 345 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_return( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2334 "parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 346 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_break(get_pos(scanner)); }
#line 2340 "parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 347 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_continue(get_pos(scanner)); }
#line 2346 "parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 351 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Stmt_from_Expression((yyvsp[-1].exp),   get_pos(scanner)); }
#line 2352 "parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 352 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Stmt_from_Expression(NULL, get_pos(scanner)); }
#line 2358 "parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 353 "gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Stmt_from_Expression(NULL, get_pos(scanner)); append_doc(scanner, (yyvsp[0].sval)); }
#line 2364 "parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 357 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2370 "parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 358 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_Expression((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2376 "parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 362 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_op_arrow_left; }
#line 2382 "parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 363 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_op_arrow_right; }
#line 2388 "parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 366 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2394 "parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 367 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2400 "parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 371 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2406 "parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 373 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2412 "parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 376 "gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2418 "parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 381 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2424 "parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 382 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2430 "parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 383 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2436 "parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 384 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2442 "parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 385 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2448 "parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 386 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2454 "parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 387 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2460 "parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 388 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2466 "parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 389 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2472 "parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 390 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2478 "parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 391 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2484 "parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 392 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2490 "parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 393 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2496 "parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 394 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2502 "parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 395 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2508 "parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 396 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2514 "parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 397 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2520 "parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 398 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2526 "parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 399 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2532 "parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 400 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2538 "parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 401 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2544 "parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 402 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2550 "parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 403 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2556 "parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 404 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rinc; }
#line 2562 "parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 405 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rdec; }
#line 2568 "parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 406 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_runinc; }
#line 2574 "parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 407 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rundec; }
#line 2580 "parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 408 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2586 "parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 409 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2592 "parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 413 "gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( (yyvsp[-1].exp), get_pos(scanner) ); }
#line 2598 "parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 415 "gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[0].array_sub), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2604 "parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 419 "gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( NULL, get_pos(scanner)); }
#line 2610 "parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 420 "gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-2].array_sub), NULL, get_pos(scanner)); }
#line 2616 "parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 425 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_Decl_Expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); }
#line 2622 "parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 426 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_Decl_Expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); }
#line 2628 "parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 427 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_Decl_Expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 2, get_pos(scanner)); }
#line 2634 "parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 428 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_Decl_Expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 3, get_pos(scanner)); }
#line 2640 "parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 429 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_Decl_Expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 4, get_pos(scanner)); }
#line 2646 "parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 430 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_Decl_Expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 5, get_pos(scanner)); }
#line 2652 "parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 434 "gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2658 "parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 436 "gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, NULL, get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2664 "parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 438 "gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-7].ival), (yyvsp[-6].ival), (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2670 "parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 443 "gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-7].ival), (yyvsp[-6].ival), (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), NULL, get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2676 "parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 448 "gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def(ae_key_func, ae_key_instance, new_Type_Decl(new_id_list("void", get_pos(scanner)), 0, get_pos(scanner)), "dtor", NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_dtor; (yyval.func_def)->type_decl->doc = get_doc(scanner);}
#line 2682 "parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 450 "gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def(ae_key_func, ae_key_static, (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_op; (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2688 "parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 453 "gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def(ae_key_func, ae_key_static, (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_op; (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2694 "parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 459 "gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner));
      (yyval.func_def)->type_decl->doc = get_doc(scanner); (yyval.func_def)->types = (yyvsp[-8].id_list); }
#line 2701 "parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 462 "gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-7].ival), (yyvsp[-6].ival), (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner);
      (yyval.func_def)->types = (yyvsp[-9].id_list); }
#line 2708 "parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 467 "gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_Type_Decl(new_id_list((yyvsp[0].sval), get_pos(scanner)), 0, get_pos(scanner)); }
#line 2714 "parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 468 "gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_Type_Decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), 1, get_pos(scanner)); }
#line 2720 "parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 469 "gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_Type_Decl((yyvsp[-1].id_list),  0, get_pos(scanner)); }
#line 2726 "parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 470 "gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_Type_Decl((yyvsp[-2].id_list),  1, get_pos(scanner)); }
#line 2732 "parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 475 "gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_Decl_List((yyvsp[-1].exp)->decl_exp, NULL); }
#line 2738 "parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 476 "gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_Decl_List((yyvsp[-2].exp)->decl_exp, (yyvsp[0].decl_list)); }
#line 2744 "parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 480 "gwion.y" /* yacc.c:1646  */
    {(yyval.union_stmt) = new_Union((yyvsp[-2].decl_list)); }
#line 2750 "parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 483 "gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_Var_Decl_List((yyvsp[0].var_decl), NULL, get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2756 "parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 484 "gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_Var_Decl_List((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2762 "parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 488 "gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_Var_Decl((yyvsp[0].sval), NULL, get_pos(scanner)); }
#line 2768 "parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 489 "gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_Var_Decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2774 "parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 490 "gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_Var_Decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2780 "parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 495 "gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2786 "parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 500 "gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar((yyvsp[-1].exp), get_pos(scanner)); }
#line 2792 "parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 503 "gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_Vec((yyvsp[-1].exp), get_pos(scanner)); }
#line 2798 "parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 508 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_If_Expression( (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2804 "parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 512 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2810 "parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 514 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2816 "parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 518 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2822 "parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 520 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2828 "parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 524 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2834 "parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 526 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2840 "parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 530 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2846 "parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 532 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(scanner)); }
#line 2852 "parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 536 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2858 "parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 538 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2864 "parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 542 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2870 "parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 544 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_eq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2876 "parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 546 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_neq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2882 "parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 550 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2888 "parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 552 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_lt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2894 "parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 554 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_gt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2900 "parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 556 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_le, (yyvsp[0].exp), get_pos(scanner)); }
#line 2906 "parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 558 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_ge, (yyvsp[0].exp), get_pos(scanner)); }
#line 2912 "parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 562 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2918 "parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 564 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_shift_left, (yyvsp[0].exp), get_pos(scanner)); }
#line 2924 "parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 566 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_shift_right, (yyvsp[0].exp), get_pos(scanner)); }
#line 2930 "parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 570 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2936 "parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 572 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_plus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2942 "parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 574 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_minus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2948 "parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 578 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2954 "parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 580 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_times, (yyvsp[0].exp), get_pos(scanner)); }
#line 2960 "parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 582 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_divide, (yyvsp[0].exp), get_pos(scanner)); }
#line 2966 "parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 584 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_percent, (yyvsp[0].exp), get_pos(scanner)); }
#line 2972 "parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 588 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2978 "parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 590 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_tilda, (yyvsp[0].exp), get_pos(scanner)); }
#line 2984 "parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 596 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Cast_Expression( (yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2990 "parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 601 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2996 "parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 603 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_plusplus, (yyvsp[0].exp), get_pos(scanner)); }
#line 3002 "parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 605 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_minusminus, (yyvsp[0].exp), get_pos(scanner)); }
#line 3008 "parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 607 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 3014 "parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 609 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_typeof, (yyvsp[0].exp), get_pos(scanner)); }
#line 3020 "parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 611 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_sizeof, (yyvsp[0].exp), get_pos(scanner)); }
#line 3026 "parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 613 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(scanner)); }
#line 3032 "parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 615 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary2(op_new, (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3038 "parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 617 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary3( op_spork, (yyvsp[0].stmt), get_pos(scanner)); }
#line 3044 "parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 621 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3050 "parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 622 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3056 "parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 623 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_tilda; }
#line 3062 "parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 624 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3068 "parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 625 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3074 "parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 626 "gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3080 "parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 632 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Exp_Dur( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 3086 "parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 636 "gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sval), get_pos(scanner)), NULL, get_pos(scanner)); }
#line 3092 "parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 637 "gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sval), get_pos(scanner)), (yyvsp[0].type_list), get_pos(scanner)); }
#line 3098 "parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 642 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Array( (yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3104 "parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 646 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Func_Call( (yyvsp[-2].exp), NULL, get_pos(scanner)); }
#line 3110 "parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 648 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Func_Call( (yyvsp[-3].exp), (yyvsp[-1].exp), get_pos(scanner)); }
#line 3116 "parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 650 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_member_dot( (yyvsp[-2].exp), (yyvsp[0].sval), get_pos(scanner)); }
#line 3122 "parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 652 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Postfix_Expression( (yyvsp[-1].exp), op_plusplus, get_pos(scanner)); }
#line 3128 "parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 654 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Postfix_Expression( (yyvsp[-1].exp), op_minusminus, get_pos(scanner)); }
#line 3134 "parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 656 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Func_Call( (yyvsp[-3].exp), NULL, get_pos(scanner)); (yyval.exp)->func_call->types = (yyvsp[-2].type_list); }
#line 3140 "parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 658 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Func_Call( (yyvsp[-4].exp), (yyvsp[-1].exp), get_pos(scanner)); (yyval.exp)->func_call->types = (yyvsp[-3].type_list); }
#line 3146 "parser.c" /* yacc.c:1646  */
    break;

  case 217:
#line 665 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_ID(    (yyvsp[0].sval), get_pos(scanner)); }
#line 3152 "parser.c" /* yacc.c:1646  */
    break;

  case 218:
#line 666 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_int(   (yyvsp[0].ival), get_pos(scanner)); }
#line 3158 "parser.c" /* yacc.c:1646  */
    break;

  case 219:
#line 667 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_float( (yyvsp[0].fval), get_pos(scanner)); }
#line 3164 "parser.c" /* yacc.c:1646  */
    break;

  case 220:
#line 668 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_string((yyvsp[0].sval), get_pos(scanner)); }
#line 3170 "parser.c" /* yacc.c:1646  */
    break;

  case 221:
#line 669 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_char(                 (yyvsp[0].sval), get_pos(scanner)); }
#line 3176 "parser.c" /* yacc.c:1646  */
    break;

  case 222:
#line 670 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_array_lit(            (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3182 "parser.c" /* yacc.c:1646  */
    break;

  case 223:
#line 671 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_complex(              (yyvsp[0].c_val), get_pos(scanner)); }
#line 3188 "parser.c" /* yacc.c:1646  */
    break;

  case 224:
#line 672 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_polar(                (yyvsp[0].polar), get_pos(scanner)); }
#line 3194 "parser.c" /* yacc.c:1646  */
    break;

  case 225:
#line 673 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_vec(                 (yyvsp[0].vec), get_pos(scanner)); }
#line 3200 "parser.c" /* yacc.c:1646  */
    break;

  case 226:
#line 674 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Hack_Expression(               (yyvsp[-1].exp), get_pos(scanner)); }
#line 3206 "parser.c" /* yacc.c:1646  */
    break;

  case 227:
#line 675 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3212 "parser.c" /* yacc.c:1646  */
    break;

  case 228:
#line 676 "gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_nil(       get_pos(scanner)); }
#line 3218 "parser.c" /* yacc.c:1646  */
    break;


#line 3222 "parser.c" /* yacc.c:1646  */
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
#line 678 "gwion.y" /* yacc.c:1906  */

