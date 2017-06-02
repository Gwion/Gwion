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
    DOC = 358
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

#line 379 "eval/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int minimal_parse (MyArg* arg);

#endif /* !YY_MINIMAL_EVAL_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 395 "eval/parser.c" /* yacc.c:358  */

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
#define YYLAST   1608

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  104
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  62
/* YYNRULES -- Number of rules.  */
#define YYNRULES  207
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  384

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   358

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
      95,    96,    97,    98,    99,   100,   101,   102,   103
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   167,   167,   168,   172,   173,   174,   178,   183,   184,
     188,   189,   193,   194,   198,   199,   203,   204,   205,   209,
     210,   214,   215,   219,   220,   224,   225,   229,   230,   234,
     235,   236,   237,   241,   242,   243,   247,   248,   252,   253,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   272,   273,   277,   281,   285,   286,   290,   294,
     296,   298,   300,   302,   304,   306,   311,   313,   318,   319,
     320,   321,   325,   326,   327,   331,   332,   336,   337,   341,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   373,   374,   375,   379,   380,
     381,   385,   386,   387,   391,   393,   395,   397,   399,   401,
     403,   408,   409,   410,   411,   416,   417,   418,   419,   423,
     427,   428,   432,   433,   434,   438,   443,   447,   450,   451,
     456,   457,   462,   463,   468,   469,   474,   475,   480,   481,
     486,   487,   489,   494,   495,   497,   499,   501,   506,   507,
     509,   514,   515,   517,   522,   523,   525,   527,   532,   533,
     539,   540,   542,   544,   546,   548,   550,   555,   556,   557,
     558,   559,   563,   564,   568,   569,   573,   574,   576,   578,
     580,   582,   584,   586,   588,   593,   594,   595,   596,   597,
     598,   599,   600,   601,   602,   603,   604,   605
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
  "DOC", "$accept", "ast", "section", "class_def", "class_ext",
  "class_decl", "class_body", "class_body2", "class_section", "id_list",
  "id_dot", "stmt_list", "static_decl", "function_decl", "func_ptr",
  "type_decl2", "arg_list", "code_segment", "stmt", "enum_stmt",
  "label_stmt", "goto_stmt", "case_stmt", "switch_stmt", "loop_stmt",
  "selection_stmt", "jump_stmt", "exp_stmt", "exp", "binary_exp",
  "template", "op", "array_exp", "array_empty", "decl_exp", "func_def",
  "type_decl", "decl_list", "union_stmt", "var_decl_list", "var_decl",
  "complex_exp", "polar_exp", "vec_exp", "conditional_expression",
  "logical_or_expression", "logical_and_expression",
  "inclusive_or_expression", "exclusive_or_expression", "and_expression",
  "equality_expression", "relational_expression", "shift_expression",
  "additive_expression", "multiplicative_expression", "cast_exp",
  "unary_expression", "unary_operator", "dur_exp", "type_list",
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
     355,   356,   357,   358
};
# endif

#define YYPACT_NINF -299

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-299)))

#define YYTABLE_NINF -122

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     608,  -299,  -299,  1386,   966,  1026,   709,  1386,  1386,  -299,
    -299,    53,    55,   786,    78,    84,    90,   -47,   127,    23,
     134,   906,    54,   148,  1506,  1506,   -13,   121,   -21,  -299,
      57,  -299,  -299,   146,   -13,   -13,    66,  -299,   144,  1386,
    -299,  -299,    -3,  -299,  -299,  -299,   161,   328,  -299,   108,
    -299,   109,  -299,  -299,   786,  -299,  -299,  -299,  -299,  -299,
    -299,  -299,  -299,  -299,   160,  1503,  -299,   151,  -299,  -299,
      70,  -299,  -299,  -299,  -299,  -299,    -1,   111,   101,   102,
     103,    20,    21,    35,    41,    52,   145,  -299,  1506,   118,
      87,  -299,   -13,     0,   168,  -299,   171,  -299,   166,  -299,
     169,   174,   175,  1386,  1386,    42,  1386,  1386,   846,   186,
    1386,  -299,    13,   156,    91,  -299,   189,  -299,  -299,  -299,
    -299,   164,   181,   180,   -13,    70,   139,   135,   187,    99,
     190,   193,    91,  1446,   188,  -299,  -299,  -299,  -299,    91,
    -299,   -13,  -299,  -299,  -299,  1386,  -299,  -299,  -299,  -299,
    -299,  -299,  -299,  -299,  -299,  -299,  -299,  -299,  -299,  -299,
    -299,  -299,  -299,  -299,  -299,  -299,  -299,  -299,  -299,  -299,
    1386,  1086,  -299,   193,  -299,   199,  1386,  1506,  1506,  1506,
    1506,  1506,  1506,  1506,  1506,  1506,  1506,  1506,  1506,  1506,
    1506,  1506,  1506,  1506,  1506,   -13,  -299,    23,  1146,  1386,
    -299,  -299,   110,   112,   196,  -299,  -299,  -299,   195,  -299,
    -299,  -299,   198,   200,   202,   203,   204,   205,   846,  -299,
     206,  -299,  -299,   215,   207,  -299,  -299,  -299,   209,  -299,
      57,   194,   180,   210,   126,  -299,   151,   133,    16,   225,
     212,  -299,   176,   131,  -299,  -299,  -299,  -299,   151,    70,
     208,   111,   101,   102,   103,    20,    21,    21,    35,    35,
      35,    35,    41,    41,    52,    52,   145,   145,   145,  -299,
      87,  -299,   219,  -299,   229,   142,  1206,  1266,  -299,   786,
     786,  1386,  1386,   786,   786,  1326,   180,    91,    18,   137,
    -299,  -299,  -299,     9,   226,     8,  1446,  1446,   235,    57,
     224,   231,  -299,  1506,  -299,   112,  -299,  -299,   228,  -299,
     211,  -299,   232,   233,  -299,  -299,   786,   234,  -299,  -299,
    -299,   241,   236,   180,   238,    70,   240,   109,  -299,  -299,
    -299,  -299,   429,    26,  -299,  -299,  -299,   786,   250,   251,
    -299,   786,  -299,   243,  -299,   180,   252,    27,   -13,  -299,
     239,  -299,   530,  -299,  -299,   180,   245,  -299,  -299,  -299,
    -299,    29,  -299,   -13,  -299,   247,   163,  -299,  -299,  -299,
     180,  -299,   248,  -299,  -299,   253,  -299,  -299,    30,   180,
     255,  -299,   180,  -299
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      11,    73,   180,     0,     0,     0,     0,     0,     0,    27,
     179,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    10,
       0,   178,   177,     0,     0,     0,     0,    28,     0,     0,
     196,   197,   195,   198,   199,    74,     0,     2,     6,     0,
       4,    26,    50,    43,    23,    48,    44,    45,    47,    46,
      41,    42,    49,    40,     0,    75,   200,   201,    77,     5,
       0,    51,   202,   203,   204,   111,   138,   140,   142,   144,
     146,   148,   150,   153,   158,   161,   164,   168,     0,   170,
     182,   186,     0,   195,     0,   207,     0,   108,     0,    38,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   195,     0,   186,     0,    68,     0,    70,    71,   171,
     172,   121,   174,   181,     0,     0,    21,     0,     0,     0,
       0,    33,     0,     0,     0,   122,    54,     1,     3,     0,
      25,     0,    24,    72,    80,     0,   104,    84,    85,    86,
      87,    88,    83,    81,    89,    90,    82,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
       0,     0,   110,   132,   112,   130,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   173,     0,     0,     0,
     191,   192,     0,     0,     0,   187,   205,   206,   105,    39,
     136,   135,     0,     0,     0,     0,     0,     0,     0,    55,
       0,    57,    56,    19,     0,    69,   175,   176,     0,   113,
       0,   123,     0,     0,     0,    35,    34,     0,   195,     0,
       0,   137,     9,     0,    76,    78,   109,   133,   134,     0,
       0,   141,   143,   145,   147,   149,   151,   152,   157,   155,
     156,   154,   159,   160,   163,   162,   166,   165,   167,   169,
     183,   188,     0,   190,   184,     0,     0,     0,   106,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,   124,   116,     0,     0,     0,   127,   125,     0,     0,
       0,     0,   131,     0,   189,     0,    79,   193,     0,   107,
      66,    59,     0,     0,    63,    65,     0,     0,    58,    20,
      52,     0,     0,     0,     0,     0,     0,    26,   128,   126,
     129,     8,    11,     0,   139,   185,   194,     0,     0,     0,
      61,     0,    53,     0,   117,     0,    36,     0,     0,    18,
       0,    12,    11,    16,    17,     0,     0,    67,    60,    64,
      62,     0,   118,     0,    29,     0,     0,     7,    15,   114,
       0,    30,     0,    37,    31,     0,   115,    32,     0,     0,
       0,   119,     0,   120
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -299,   217,  -299,  -298,  -299,  -299,  -299,   -86,  -299,  -115,
    -210,    -5,   -57,   -22,  -299,  -118,  -285,  -120,    -8,  -299,
    -299,  -299,  -299,  -299,  -299,  -299,  -299,   -90,     7,  -299,
    -299,  -299,   -54,  -101,  -129,  -277,   -26,  -165,  -299,  -109,
     -56,  -299,  -299,  -299,   -29,  -299,   100,   104,   106,    98,
     105,   -36,   -43,   -40,   -35,   -58,    45,  -299,  -299,   -25,
     -12,   260
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    46,    47,    48,   300,    49,   350,   351,   352,   224,
     127,    50,   141,    51,    52,   130,   324,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
     204,   170,    66,    67,    68,    69,    70,   240,    71,   174,
     175,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,   275,
      90,    91
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     122,   100,   125,   227,   239,   105,   228,   112,   129,   131,
      94,    96,    98,   172,   101,   102,   229,   237,   218,   296,
     290,   320,   323,   243,   242,   198,   135,   199,   116,   135,
     236,   136,   176,     3,   349,     4,   205,     5,     9,   355,
     364,   245,   371,   379,    30,   135,   134,   221,   356,   142,
       7,     8,    30,   109,   349,   354,   124,   117,   205,   192,
     193,   194,   365,   200,   201,   103,   125,   104,   226,   119,
     120,   202,   248,   177,    30,   354,   372,   235,   373,   121,
     214,   182,   215,   184,   185,   186,   187,   121,   183,   331,
     106,    30,    30,   380,    30,    30,   107,  -121,   131,   198,
    -121,   199,   108,    37,   188,   189,   203,   190,   191,   121,
     212,   213,   292,   216,   217,   131,  -121,   220,   321,   247,
      39,    40,    41,   111,    43,    44,   121,   121,   285,   121,
     121,   328,   329,   196,   266,   267,   268,   200,   201,   110,
     302,   258,   259,   260,   261,   202,   256,   257,   262,   263,
     114,   118,   244,   123,   278,   264,   265,   126,   128,   132,
     133,   137,   139,   143,   140,   171,   318,   239,   239,   269,
     173,   178,   319,   179,   181,   180,   195,   197,    98,   206,
     203,   208,   172,   250,   207,   270,   209,   210,   211,   219,
     222,   223,   225,   135,   172,   199,     6,   230,   231,   233,
     232,   241,   234,   344,   249,   272,    98,     5,   276,   277,
     273,   279,   274,   280,   281,   282,   205,   283,   284,   286,
     287,   289,   293,   291,   288,   362,   294,   295,   297,   298,
     366,   301,   304,   299,   305,   369,   306,   322,   330,   326,
     332,   336,   303,   333,   342,   338,   339,   341,   337,   343,
     376,   345,   347,   358,   359,   361,   367,   363,   370,   381,
     374,   377,   383,   375,   138,   378,   368,   325,   382,   346,
     348,   310,   311,   327,   334,   314,   315,   251,   254,   113,
     335,     0,   252,   308,    98,   253,   255,     0,   312,   313,
       0,     0,   317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   325,   340,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   325,   131,     0,     0,     0,     0,   353,     0,   357,
       0,     1,     0,   360,     0,   325,     2,   325,     3,     0,
       4,     0,     5,     0,     6,     0,     0,   353,     0,     0,
       0,     0,   325,     0,     0,     7,     8,     0,     9,     0,
       0,     0,     0,    10,    11,     0,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,   -11,    28,    29,     0,     0,     0,     0,     0,
       0,     0,     0,    30,    31,    32,     0,     0,     0,     0,
       0,     0,     0,    33,    34,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
       0,     0,     0,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     1,     0,     0,     0,     0,     2,     0,     3,
       0,     4,     0,     5,     0,     6,   -13,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     9,
       0,     0,     0,     0,    10,    11,     0,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,     0,     0,     0,
       0,     0,     0,     0,    30,    31,    32,     0,     0,     0,
       0,     0,     0,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      36,     0,     0,     0,    37,    38,    39,    40,    41,    42,
      43,    44,    45,     1,     0,     0,     0,     0,     2,     0,
       3,     0,     4,     0,     5,     0,     6,   -14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       9,     0,     0,     0,     0,    10,    11,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,     0,     0,
       0,     0,     0,     0,     0,    30,    31,    32,     0,     0,
       0,     0,     0,     0,     0,    33,    34,    35,     0,     0,
       0,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,    36,     5,     0,     6,    37,    38,    39,    40,    41,
      42,    43,    44,    45,     0,     7,     8,     0,     9,     0,
       0,     0,     0,    10,    11,     0,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,     0,     0,     0,     0,
       0,     0,     0,    30,    31,    32,     0,     0,     0,     0,
       0,     0,     0,    33,    34,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
       0,     0,     0,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     1,     0,     0,     0,     0,     2,     0,     3,
       0,     4,     0,     5,     0,     6,    99,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,    10,    11,     0,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    30,    31,    32,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    35,     0,     0,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,     0,
       5,     0,     6,     0,     0,    38,    39,    40,    41,    42,
      43,    44,    45,     7,     8,     0,     0,     0,     0,     0,
       0,    10,    11,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,     0,     0,     0,     0,     0,     0,     0,     1,
       0,    30,    31,    32,     2,     0,     3,     0,     4,     0,
       5,     0,     0,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,    38,    39,    40,    41,    42,    43,    44,    45,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    92,     0,     0,     0,     0,     0,     0,     0,   115,
       0,    30,    31,    32,     2,     0,     3,     0,     4,     0,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,    39,    40,    41,    93,    43,    44,    45,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    30,    31,    32,     2,     0,     3,     0,     4,    95,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,    39,    40,    41,    93,    43,    44,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    30,    31,    32,     2,     0,     3,     0,     4,     0,
       5,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,    39,    40,    41,    93,    43,    44,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    30,    31,    32,     2,     0,     3,     0,     4,     0,
       5,   246,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,    39,    40,    41,    93,    43,    44,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    30,    31,    32,     2,     0,     3,     0,     4,   271,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,    39,    40,    41,    93,    43,    44,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    30,    31,    32,     2,     0,     3,     0,     4,   307,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,    39,    40,    41,    93,    43,    44,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    30,    31,    32,     2,     0,     3,     0,     4,     0,
       5,   309,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,    39,    40,    41,    93,    43,    44,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    30,    31,    32,     2,     0,     3,     0,     4,   316,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,    39,    40,    41,    93,    43,    44,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    30,    31,    32,     2,     0,     3,     0,     4,     0,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,    39,    40,    41,    93,    43,    44,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    30,    31,    32,     2,     0,     3,     0,     4,     0,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,    39,    40,    41,    93,    43,    44,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    92,     0,     0,     0,     0,     0,   144,   145,   146,
       0,    30,    31,    32,     2,     0,     3,     0,     4,     0,
       5,   147,   148,   149,   150,   151,   152,   153,   154,   155,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,    39,    40,    41,   238,    43,    44,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,     0,     0,     0,   156,     0,     0,     0,     0,     0,
       0,     0,    31,    32,     0,     0,     0,     0,     0,     0,
       0,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    39,    40,    41,   111,    43,    44
};

static const yytype_int16 yycheck[] =
{
      26,     6,    28,   123,   133,    13,   124,    19,    34,    35,
       3,     4,     5,    67,     7,     8,   125,   132,   108,     3,
     230,     3,    13,   141,   139,    12,    29,    14,    21,    29,
     131,    34,    33,    10,   332,    12,    90,    14,    30,    13,
      13,   170,    13,    13,    65,    29,    39,    34,   333,    54,
      27,    28,    65,   100,   352,   332,    77,     3,   112,     7,
       8,     9,   347,    50,    51,    12,    92,    12,   122,    24,
      25,    58,   173,    74,    65,   352,   361,   131,   363,   100,
      38,    61,    40,    62,    63,    64,    65,   100,    68,   299,
      12,    65,    65,   378,    65,    65,    12,   100,   124,    12,
     100,    14,    12,    95,    69,    70,    93,    66,    67,   100,
     103,   104,   232,   106,   107,   141,   100,   110,   100,   173,
      97,    98,    99,   100,   101,   102,   100,   100,   218,   100,
     100,   296,   297,    88,   192,   193,   194,    50,    51,    12,
     249,   184,   185,   186,   187,    58,   182,   183,   188,   189,
      16,     3,   145,    32,   208,   190,   191,   100,    12,    93,
      16,     0,    54,     3,    55,    14,   286,   296,   297,   195,
     100,    60,   287,    72,    71,    73,    31,    59,   171,    11,
      93,    15,   236,   176,    13,   197,    17,    13,    13,     3,
      34,   100,     3,    29,   248,    14,    16,    58,    63,   100,
      13,    13,    12,   323,     5,   198,   199,    14,    12,    14,
     100,    13,   100,    13,    12,    12,   270,    13,    13,    13,
       5,    12,    12,    29,    17,   345,   100,    94,     3,    17,
     348,   100,    13,    57,     5,   355,    94,   100,     3,    13,
      16,    13,    34,    12,     3,    13,    13,    13,    37,    13,
     370,    13,    12,     3,     3,    12,    17,     5,    13,   379,
      13,    13,   382,   100,    47,    12,   352,   293,    13,   325,
     327,   279,   280,   295,   303,   283,   284,   177,   180,    19,
     305,    -1,   178,   276,   277,   179,   181,    -1,   281,   282,
      -1,    -1,   285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   333,   316,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   347,   348,    -1,    -1,    -1,    -1,   332,    -1,   337,
      -1,     3,    -1,   341,    -1,   361,     8,   363,    10,    -1,
      12,    -1,    14,    -1,    16,    -1,    -1,   352,    -1,    -1,
      -1,    -1,   378,    -1,    -1,    27,    28,    -1,    30,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    30,
      -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     3,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    16,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      30,    -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    91,    14,    -1,    16,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,    27,    28,    -1,    30,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,   103,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    16,    -1,    -1,    96,    97,    98,    99,   100,
     101,   102,   103,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,     4,     5,     6,
      -1,    65,    66,    67,     8,    -1,    10,    -1,    12,    -1,
      14,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    97,    98,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    30,
      35,    36,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    55,    56,
      65,    66,    67,    75,    76,    77,    91,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   105,   106,   107,   109,
     115,   117,   118,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   136,   137,   138,   139,
     140,   142,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     164,   165,    55,   100,   132,    13,   132,    15,   132,    17,
     115,   132,   132,    12,    12,   122,    12,    12,    12,   100,
      12,   100,   164,   165,    16,     3,   132,     3,     3,   160,
     160,   100,   140,    32,    77,   140,   100,   114,    12,   140,
     119,   140,    93,    16,   132,    29,    34,     0,   105,    54,
      55,   116,   115,     3,     4,     5,     6,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    61,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     135,    14,   136,   100,   143,   144,    33,    74,    60,    72,
      73,    71,    61,    68,    62,    63,    64,    65,    69,    70,
      66,    67,     7,     8,     9,    31,   160,    59,    12,    14,
      50,    51,    58,    93,   134,   136,    11,    13,    15,    17,
      13,    13,   132,   132,    38,    40,   132,   132,   131,     3,
     132,    34,    34,   100,   113,     3,   136,   121,   119,   143,
      58,    63,    13,   100,    12,   136,   137,   113,   100,   138,
     141,    13,   113,   119,   132,   138,    15,   136,   137,     5,
     132,   150,   151,   152,   153,   154,   155,   155,   156,   156,
     156,   156,   157,   157,   158,   158,   159,   159,   159,   140,
     164,    13,   132,   100,   100,   163,    12,    14,   136,    13,
      13,    12,    12,    13,    13,   131,    13,     5,    17,    12,
     114,    29,   121,    12,   100,    94,     3,     3,    17,    57,
     108,   100,   143,    34,    13,     5,    94,    13,   132,    15,
     122,   122,   132,   132,   122,   122,    13,   132,   121,   113,
       3,   100,   100,    13,   120,   140,    13,   117,   141,   141,
       3,   114,    16,    12,   148,   163,    13,    37,    13,    13,
     122,    13,     3,    13,   121,    13,   144,    12,   116,   107,
     110,   111,   112,   115,   139,    13,   120,   122,     3,     3,
     122,    12,   121,     5,    13,   120,   119,    17,   111,   121,
      13,    13,   120,   120,    13,   100,   121,    13,    12,    13,
     120,   121,    13,   121
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   104,   105,   105,   106,   106,   106,   107,   108,   108,
     109,   109,   110,   110,   111,   111,   112,   112,   112,   113,
     113,   114,   114,   115,   115,   116,   116,   117,   117,   118,
     118,   118,   118,   119,   119,   119,   120,   120,   121,   121,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   123,   123,   124,   125,   126,   126,   127,   128,
     128,   128,   128,   128,   128,   128,   129,   129,   130,   130,
     130,   130,   131,   131,   131,   132,   132,   133,   133,   134,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   136,   136,   136,   137,   137,
     137,   138,   138,   138,   139,   139,   139,   139,   139,   139,
     139,   140,   140,   140,   140,   141,   141,   141,   141,   142,
     143,   143,   144,   144,   144,   145,   146,   147,   148,   148,
     149,   149,   150,   150,   151,   151,   152,   152,   153,   153,
     154,   154,   154,   155,   155,   155,   155,   155,   156,   156,
     156,   157,   157,   157,   158,   158,   158,   158,   159,   159,
     160,   160,   160,   160,   160,   160,   160,   161,   161,   161,
     161,   161,   162,   162,   163,   163,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165
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
       1,     2,     2,     2,     2,     3,     3,     1,     1,     1,
       1,     2,     1,     3,     1,     3,     1,     2,     3,     4,
       3,     2,     2,     4,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     2
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
          case 119: /* type_decl2  */
#line 163 "eval/gwion.y" /* yacc.c:1257  */
      { printf("!!!!\n"); free_type_decl(((*yyvaluep).type_decl)); }
#line 1762 "eval/parser.c" /* yacc.c:1257  */
        break;

    case 140: /* type_decl  */
#line 163 "eval/gwion.y" /* yacc.c:1257  */
      { printf("!!!!\n"); free_type_decl(((*yyvaluep).type_decl)); }
#line 1768 "eval/parser.c" /* yacc.c:1257  */
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
#line 167 "eval/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL, get_pos(scanner));  }
#line 2036 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 168 "eval/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(scanner)); }
#line 2042 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 172 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2048 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 173 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(scanner)); }
#line 2054 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 174 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(scanner)); }
#line 2060 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 179 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].class_ext), (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->doc = get_doc(scanner);}
#line 2066 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 183 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = new_class_ext( (yyvsp[0].id_list), NULL, get_pos(scanner)); }
#line 2072 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 184 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = NULL; }
#line 2078 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 188 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_public; }
#line 2084 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 189 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_private; }
#line 2090 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 193 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2096 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 194 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2102 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 198 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body( (yyvsp[0].section), get_pos(scanner)); }
#line 2108 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 199 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body( (yyvsp[-1].section), (yyvsp[0].class_body), get_pos(scanner)); }
#line 2114 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 203 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list( (yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2120 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 204 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def( (yyvsp[0].func_def), get_pos(scanner)); }
#line 2126 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 205 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def( (yyvsp[0].class_def), get_pos(scanner)); }
#line 2132 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 209 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2138 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 210 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2144 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 214 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2150 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 215 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2156 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 219 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(scanner));}
#line 2162 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 220 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(scanner));}
#line 2168 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 224 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_static;   }
#line 2174 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 225 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_instance; }
#line 2180 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 229 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_func; }
#line 2186 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 230 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_variadic; }
#line 2192 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 234 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2198 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 235 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_key_static, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2204 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 236 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2210 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 237 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_key_static, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2216 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 241 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2222 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 242 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2228 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 243 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2234 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 247 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
#line 2240 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 248 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
#line 2246 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 252 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_code( NULL, get_pos(scanner)); }
#line 2252 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 253 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_code( (yyvsp[-1].stmt_list), get_pos(scanner)); }
#line 2258 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 272 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_enum((yyvsp[-2].id_list), NULL, get_pos(scanner)); }
#line 2264 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 273 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_enum((yyvsp[-3].id_list), (yyvsp[-1].sval), get_pos(scanner)); }
#line 2270 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 277 "eval/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_from_gotolabel((yyvsp[-1].sval), 1, get_pos(scanner)); }
#line 2276 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 281 "eval/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_from_gotolabel((yyvsp[-1].sval), 0, get_pos(scanner)); }
#line 2282 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 285 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2288 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 286 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2294 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 290 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner));}
#line 2300 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 295 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_while( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2306 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 297 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_while( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2312 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 299 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2318 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 301 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2324 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 303 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_until( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2330 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 305 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_until( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2336 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 307 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_loop( (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2342 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 312 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(scanner)); }
#line 2348 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 314 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2354 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 318 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_return( NULL, get_pos(scanner)); }
#line 2360 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 319 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_return( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2366 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 320 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_break(get_pos(scanner)); }
#line 2372 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 321 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_continue(get_pos(scanner)); }
#line 2378 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 325 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_expression((yyvsp[-1].exp),   get_pos(scanner)); }
#line 2384 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 326 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_expression(NULL, get_pos(scanner)); }
#line 2390 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 327 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_expression(NULL, get_pos(scanner)); append_doc(scanner, (yyvsp[0].sval)); }
#line 2396 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 331 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2402 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 332 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_expression((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2408 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 336 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2414 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 337 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2420 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 341 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2426 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 345 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2432 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 346 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2438 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 347 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; /* LCOV_EXCL_LINE */ }
#line 2444 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 348 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2450 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 349 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2456 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 350 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2462 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 351 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2468 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 352 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2474 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 353 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2480 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 354 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2486 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 355 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2492 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 356 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2498 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 357 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2504 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 358 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2510 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 359 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2516 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 360 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2522 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 361 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2528 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 362 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2534 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 363 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2540 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 364 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2546 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 365 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2552 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 366 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2558 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 367 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2564 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 368 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2570 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 369 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2576 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 373 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( (yyvsp[-1].exp), get_pos(scanner) ); }
#line 2582 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 374 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[0].array_sub), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2588 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 375 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( new_array_sub( NULL, get_pos(scanner)), (yyvsp[-3].exp), get_pos(scanner)); }
#line 2594 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 379 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( NULL, get_pos(scanner)); }
#line 2600 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 380 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-2].array_sub), NULL, get_pos(scanner)); }
#line 2606 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 381 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-1].array_sub), (yyvsp[0].array_sub)->exp_list, get_pos(scanner)); }
#line 2612 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 386 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_decl_expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); }
#line 2618 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 387 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_decl_expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); }
#line 2624 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 392 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2630 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 394 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-7].ival), (yyvsp[-6].ival), (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2636 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 396 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_key_func, ae_key_instance, new_type_decl(new_id_list("void", get_pos(scanner)), 0, get_pos(scanner)), "dtor", NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_dtor; (yyval.func_def)->type_decl->doc = get_doc(scanner);}
#line 2642 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 398 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_key_func, ae_key_static, (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_op; (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2648 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 400 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_key_func, ae_key_static, (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_op; (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2654 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 402 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); (yyval.func_def)->types = (yyvsp[-8].id_list); }
#line 2660 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 404 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-7].ival), (yyvsp[-6].ival), (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); (yyval.func_def)->types = (yyvsp[-9].id_list); }
#line 2666 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 408 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[0].sval), get_pos(scanner)), 0, get_pos(scanner)); }
#line 2672 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 409 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), 1, get_pos(scanner)); }
#line 2678 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 410 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-1].id_list),  0, get_pos(scanner)); }
#line 2684 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 411 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list),  1, get_pos(scanner)); }
#line 2690 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 416 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list(&(yyvsp[-1].exp)->d.exp_decl, NULL); }
#line 2696 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 417 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list(&(yyvsp[-2].exp)->d.exp_decl, (yyvsp[0].decl_list)); }
#line 2702 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 418 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = NULL; }
#line 2708 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 419 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list(NULL, (yyvsp[0].decl_list)); }
#line 2714 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 423 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_union((yyvsp[-2].decl_list), get_pos(scanner)); }
#line 2720 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 427 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2726 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 428 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2732 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 432 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sval), NULL, get_pos(scanner)); }
#line 2738 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 433 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2744 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 434 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2750 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 439 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2756 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 444 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar((yyvsp[-1].exp), get_pos(scanner)); }
#line 2762 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 447 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec((yyvsp[-1].exp), get_pos(scanner)); }
#line 2768 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 452 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_if_expression( (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2774 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 456 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2780 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 458 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2786 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 462 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2792 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 464 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2798 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 468 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2804 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 470 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2810 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 474 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2816 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 476 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(scanner)); }
#line 2822 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 480 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2828 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 482 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2834 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 486 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2840 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 488 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_eq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2846 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 490 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_neq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2852 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 494 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2858 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 496 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_lt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2864 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 498 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_gt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2870 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 500 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_le, (yyvsp[0].exp), get_pos(scanner)); }
#line 2876 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 502 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_ge, (yyvsp[0].exp), get_pos(scanner)); }
#line 2882 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 506 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2888 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 508 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_shift_left, (yyvsp[0].exp), get_pos(scanner)); }
#line 2894 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 510 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_shift_right, (yyvsp[0].exp), get_pos(scanner)); }
#line 2900 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 514 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2906 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 516 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_plus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2912 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 518 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_minus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2918 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 522 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2924 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 524 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_times, (yyvsp[0].exp), get_pos(scanner)); }
#line 2930 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 526 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_divide, (yyvsp[0].exp), get_pos(scanner)); }
#line 2936 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 528 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_percent, (yyvsp[0].exp), get_pos(scanner)); }
#line 2942 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 534 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_cast_expression( (yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2948 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 539 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2954 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 541 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_plusplus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2960 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 543 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_minusminus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2966 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 545 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2972 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 547 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(scanner)); }
#line 2978 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 549 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary2(op_new, (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2984 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 551 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary3( op_spork, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2990 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 555 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2996 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 556 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3002 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 557 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3008 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 558 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3014 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 559 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3020 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 564 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 3026 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 568 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sval), get_pos(scanner)), NULL, get_pos(scanner)); }
#line 3032 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 569 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sval), get_pos(scanner)), (yyvsp[0].type_list), get_pos(scanner)); }
#line 3038 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 575 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array( (yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3044 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 577 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_func_call( (yyvsp[-2].exp), NULL, get_pos(scanner)); }
#line 3050 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 579 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_func_call( (yyvsp[-3].exp), (yyvsp[-1].exp), get_pos(scanner)); }
#line 3056 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 581 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_member_dot( (yyvsp[-2].exp), (yyvsp[0].sval), get_pos(scanner)); }
#line 3062 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 583 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_postfix_expression( (yyvsp[-1].exp), op_plusplus, get_pos(scanner)); }
#line 3068 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 585 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_postfix_expression( (yyvsp[-1].exp), op_minusminus, get_pos(scanner)); }
#line 3074 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 587 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_func_call( (yyvsp[-3].exp), NULL, get_pos(scanner)); (yyval.exp)->d.exp_func.types = (yyvsp[-2].type_list); }
#line 3080 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 589 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_func_call( (yyvsp[-4].exp), (yyvsp[-1].exp), get_pos(scanner)); (yyval.exp)->d.exp_func.types = (yyvsp[-3].type_list); }
#line 3086 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 593 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_ID(     (yyvsp[0].sval), get_pos(scanner)); }
#line 3092 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 594 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_int(    (yyvsp[0].ival), get_pos(scanner)); }
#line 3098 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 595 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_float(  (yyvsp[0].fval), get_pos(scanner)); }
#line 3104 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 596 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_string( (yyvsp[0].sval), get_pos(scanner)); }
#line 3110 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 597 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_char(                  (yyvsp[0].sval), get_pos(scanner)); }
#line 3116 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 598 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_array_lit(             (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3122 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 599 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_array_lit(           (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3128 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 600 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_complex(               (yyvsp[0].c_val), get_pos(scanner)); }
#line 3134 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 601 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_polar(                 (yyvsp[0].polar), get_pos(scanner)); }
#line 3140 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 602 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_vec(                   (yyvsp[0].vec), get_pos(scanner)); }
#line 3146 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 603 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_hack_expression(                (yyvsp[-1].exp), get_pos(scanner)); }
#line 3152 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 604 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3158 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 605 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_nil(       get_pos(scanner)); }
#line 3164 "eval/parser.c" /* yacc.c:1646  */
    break;


#line 3168 "eval/parser.c" /* yacc.c:1646  */
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
#line 607 "eval/gwion.y" /* yacc.c:1906  */

