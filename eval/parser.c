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
    RINC = 348,
    RDEC = 349,
    RUNINC = 350,
    RUNDEC = 351,
    TEMPLATE = 352,
    NOELSE = 353,
    LTB = 354,
    GTB = 355,
    VARARG = 356,
    UNION = 357,
    ATPAREN = 358,
    NUM = 359,
    FLOAT = 360,
    ID = 361,
    STRING_LIT = 362,
    CHAR_LIT = 363,
    DOC = 364
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
#define RINC 348
#define RDEC 349
#define RUNINC 350
#define RUNDEC 351
#define TEMPLATE 352
#define NOELSE 353
#define LTB 354
#define GTB 355
#define VARARG 356
#define UNION 357
#define ATPAREN 358
#define NUM 359
#define FLOAT 360
#define ID 361
#define STRING_LIT 362
#define CHAR_LIT 363
#define DOC 364

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

#line 400 "eval/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int minimal_parse (MyArg* arg);

#endif /* !YY_MINIMAL_EVAL_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 416 "eval/parser.c" /* yacc.c:358  */

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
#define YYLAST   1550

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  110
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  62
/* YYNRULES -- Number of rules.  */
#define YYNRULES  213
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  392

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
       0,   177,   177,   178,   182,   183,   184,   188,   193,   194,
     198,   199,   203,   204,   208,   209,   213,   214,   215,   219,
     220,   224,   225,   229,   230,   234,   235,   239,   240,   244,
     245,   246,   247,   251,   252,   253,   257,   258,   262,   263,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   282,   283,   287,   291,   295,   296,   300,   304,
     306,   308,   310,   312,   314,   316,   321,   323,   328,   329,
     330,   331,   335,   336,   337,   341,   342,   346,   347,   351,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   387,
     388,   389,   393,   394,   395,   399,   400,   401,   405,   407,
     409,   411,   413,   415,   417,   422,   423,   424,   425,   430,
     431,   432,   433,   437,   441,   442,   446,   447,   448,   452,
     457,   461,   464,   465,   470,   471,   476,   477,   482,   483,
     488,   489,   494,   495,   500,   501,   503,   508,   509,   511,
     513,   515,   520,   521,   523,   528,   529,   531,   536,   537,
     539,   541,   546,   547,   553,   554,   556,   558,   560,   562,
     564,   566,   568,   573,   574,   575,   576,   577,   581,   582,
     586,   587,   591,   592,   594,   596,   598,   600,   602,   604,
     606,   611,   612,   613,   614,   615,   616,   617,   618,   619,
     620,   621,   622,   623
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
  "ROR", "REQ", "RNEQ", "RGT", "RGE", "RLT", "RLE", "RINC", "RDEC",
  "RUNINC", "RUNDEC", "TEMPLATE", "NOELSE", "LTB", "GTB", "VARARG",
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
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364
};
# endif

#define YYPACT_NINF -297

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-297)))

#define YYTABLE_NINF -126

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     632,  -297,  -297,  1318,   132,   946,   739,  1318,  1318,  -297,
    -297,    13,    28,   822,    36,    50,    53,   -72,    57,   200,
      64,   884,    70,    81,  1442,  1442,   -43,    56,  1442,  1442,
     -24,  -297,    25,  -297,  -297,    79,   -43,   -43,    11,  -297,
     117,  1318,  -297,  -297,     3,  -297,  -297,  -297,   141,   334,
    -297,    87,  -297,   104,  -297,  -297,   822,  -297,  -297,  -297,
    -297,  -297,  -297,  -297,  -297,  -297,   176,  1439,  -297,   167,
    -297,  -297,    85,  -297,  -297,  -297,  -297,  -297,   -15,   142,
     134,   138,   136,     4,    72,    32,     7,   148,   186,  -297,
    1442,   157,   191,  -297,   -43,    -1,   209,  -297,   208,  -297,
     207,  -297,   206,   212,   213,  1318,  1318,    19,  1318,  1318,
      44,   226,  1318,  -297,   120,   196,   126,  -297,   231,  -297,
    -297,  -297,  -297,   215,   232,   229,  -297,  -297,   -43,    85,
     187,   183,   236,   144,   240,   239,   126,  1380,   241,  -297,
    -297,  -297,  -297,   126,  -297,   -43,  -297,  -297,  -297,  1318,
    -297,  -297,  -297,  -297,  -297,  -297,  -297,  -297,  -297,  -297,
    -297,  -297,  -297,  -297,  -297,  -297,  -297,  -297,  -297,  -297,
    -297,  -297,  -297,  -297,  -297,  -297,  -297,  -297,  1318,  1008,
    -297,   239,  -297,   250,  1318,  1442,  1442,  1442,  1442,  1442,
    1442,  1442,  1442,  1442,  1442,  1442,  1442,  1442,  1442,  1442,
    1442,  1442,  1442,   -43,  -297,   200,  1070,  1318,  -297,  -297,
     150,   151,   247,  -297,  -297,  -297,   246,  -297,  -297,  -297,
     248,   249,   251,   252,   253,   255,    44,  -297,   256,  -297,
    -297,   260,   254,  -297,  -297,  -297,   261,  -297,    25,   243,
     229,   262,   172,  -297,   167,   179,    24,   279,   266,  -297,
     225,   180,  -297,  -297,  -297,  -297,   167,    85,   257,   142,
     134,   138,   136,     4,    72,    72,    32,    32,    32,    32,
       7,     7,   148,   148,   186,   186,   186,  -297,   191,  -297,
     272,  -297,   282,   188,  1132,  1194,  -297,   822,   822,  1318,
    1318,   822,   822,  1256,   229,   126,    18,   189,  -297,  -297,
    -297,    10,   276,    -8,  1380,  1380,   289,    25,   280,   285,
    -297,  1442,  -297,   151,  -297,  -297,   287,  -297,   264,  -297,
     296,   297,  -297,  -297,   822,   298,  -297,  -297,  -297,   309,
     301,   229,   303,    85,   305,   104,  -297,  -297,  -297,  -297,
     441,    16,  -297,  -297,  -297,   822,   315,   316,  -297,   822,
    -297,   308,  -297,   229,   317,    20,   -43,  -297,   304,  -297,
     548,  -297,  -297,   229,   310,  -297,  -297,  -297,  -297,    22,
    -297,   -43,  -297,   311,   219,  -297,  -297,  -297,   229,  -297,
     313,  -297,  -297,   319,  -297,  -297,    23,   229,   314,  -297,
     229,  -297
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      11,    73,   186,     0,     0,     0,     0,     0,     0,    27,
     185,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    10,     0,   184,   183,     0,     0,     0,     0,    28,
       0,     0,   202,   203,   201,   204,   205,    74,     0,     2,
       6,     0,     4,    26,    50,    43,    23,    48,    44,    45,
      47,    46,    41,    42,    49,    40,     0,    75,   206,   207,
      77,     5,     0,    51,   208,   209,   210,   115,   142,   144,
     146,   148,   150,   152,   154,   157,   162,   165,   168,   172,
       0,   174,   188,   192,     0,   201,     0,   213,     0,   112,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   201,     0,   192,     0,    68,     0,    70,
      71,   175,   176,   125,   180,   187,   179,   178,     0,     0,
      21,     0,     0,     0,     0,    33,     0,     0,     0,   126,
      54,     1,     3,     0,    25,     0,    24,    72,    80,     0,
     108,    84,    85,    86,    87,    88,    83,    81,    89,    90,
      82,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,     0,     0,
     114,   136,   116,   134,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   177,     0,     0,     0,   197,   198,
       0,     0,     0,   193,   211,   212,   109,    39,   140,   139,
       0,     0,     0,     0,     0,     0,     0,    55,     0,    57,
      56,    19,     0,    69,   181,   182,     0,   117,     0,   127,
       0,     0,     0,    35,    34,     0,   201,     0,     0,   141,
       9,     0,    76,    78,   113,   137,   138,     0,     0,   145,
     147,   149,   151,   153,   155,   156,   161,   159,   160,   158,
     163,   164,   167,   166,   170,   169,   171,   173,   189,   194,
       0,   196,   190,     0,     0,     0,   110,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,   128,
     120,     0,     0,     0,   131,   129,     0,     0,     0,     0,
     135,     0,   195,     0,    79,   199,     0,   111,    66,    59,
       0,     0,    63,    65,     0,     0,    58,    20,    52,     0,
       0,     0,     0,     0,     0,    26,   132,   130,   133,     8,
      11,     0,   143,   191,   200,     0,     0,     0,    61,     0,
      53,     0,   121,     0,    36,     0,     0,    18,     0,    12,
      11,    16,    17,     0,     0,    67,    60,    64,    62,     0,
     122,     0,    29,     0,     0,     7,    15,   118,     0,    30,
       0,    37,    31,     0,   119,    32,     0,     0,     0,   123,
       0,   124
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -297,   283,  -297,  -296,  -297,  -297,  -297,   -32,  -297,  -117,
    -207,    -5,    -2,    33,  -297,  -125,  -263,  -120,   -11,  -297,
    -297,  -297,  -297,  -297,  -297,  -297,  -297,  -101,     9,  -297,
    -297,  -297,   -54,   -96,  -129,  -274,   -26,  -136,  -297,  -122,
       6,  -297,  -297,  -297,    29,  -297,   156,   161,   162,   163,
     164,   -18,   -29,     1,     8,    -6,    17,  -297,  -297,    39,
     -13,   335
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    48,    49,    50,   308,    51,   358,   359,   360,   232,
     131,    52,   145,    53,    54,   134,   332,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
     212,   178,    68,    69,    70,    71,    72,   248,    73,   182,
     183,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,   283,
      92,    93
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     124,   102,   107,   236,   129,   235,   114,   237,   247,   226,
     133,   135,    96,    98,   100,   180,   103,   104,   184,   245,
     251,   328,     9,   331,    32,   105,   250,   304,   139,   363,
     118,   298,   139,   372,   111,   379,   387,   140,   213,   244,
     106,   121,   122,    32,   357,   126,   127,     1,   108,   253,
     138,   146,     2,   139,     3,   128,     4,   222,     5,   223,
     213,   185,   109,   123,   357,   110,   362,   190,   129,   112,
     234,     7,     8,   119,   191,   198,   199,    32,   364,    10,
     116,   243,   123,    32,   120,   256,   362,    32,   125,    32,
      32,   132,   373,    39,    24,    25,    26,    27,    28,    29,
     339,    94,   135,   196,   197,  -125,   380,   204,   381,  -125,
     136,    32,    33,    34,   220,   221,   123,   224,   225,   135,
     300,   228,   123,   388,   329,   293,   123,   255,   123,   123,
    -125,   130,   206,   137,   207,   310,   192,   193,   194,   195,
       2,   141,     3,   143,     4,    97,     5,    41,    42,    43,
      95,    45,    46,    47,   229,   200,   201,   202,   252,     7,
       8,   144,   286,   266,   267,   268,   269,    10,   336,   337,
     208,   209,   264,   265,   326,   247,   247,   277,   327,   147,
     210,   179,    24,    25,    26,    27,    28,    29,   100,    94,
     180,   181,   278,   258,   274,   275,   276,   270,   271,    32,
      33,    34,   180,   206,   186,   207,   272,   273,   187,   189,
       3,   352,     4,   188,     5,   280,   100,   203,   205,   211,
     214,   215,   216,   217,   213,   218,   219,     7,     8,   227,
     230,   374,   231,   370,   233,    41,    42,    43,    95,    45,
      46,   208,   209,   377,   139,     6,   207,   238,   239,   240,
     241,   210,   242,     5,   249,   257,   281,   282,   384,   284,
     285,   287,   288,   289,   290,   295,   291,   389,   292,   294,
     391,   296,   299,   297,   301,   333,   318,   319,   302,   303,
     322,   323,   305,   306,   307,   312,   309,   313,   314,   334,
     211,   311,   338,   316,   100,   330,   340,   341,   320,   321,
     344,   345,   325,    41,    42,    43,   113,    45,    46,   346,
     347,   349,   350,   348,   351,   333,   353,   355,   366,   367,
     369,   375,   371,   378,   382,   383,   385,   390,   376,   333,
     135,   386,   142,   356,   365,   361,   335,     1,   368,   354,
     342,   259,     2,   333,     3,   333,     4,   260,     5,   261,
       6,   262,   343,   263,   115,   361,     0,     0,     0,     0,
     333,     7,     8,     0,     9,     0,     0,     0,     0,    10,
      11,     0,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
     -11,    30,    31,     0,     0,     0,     0,     0,     0,     0,
       0,    32,    33,    34,     0,     0,     0,     0,     0,     0,
       0,    35,    36,    37,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    38,     0,     0,     0,    39,    40,    41,    42,    43,
      44,    45,    46,    47,     1,     0,     0,     0,     0,     2,
       0,     3,     0,     4,     0,     5,     0,     6,   -13,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
       0,     9,     0,     0,     0,     0,    10,    11,     0,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,     0,     0,     0,     0,     0,     0,     0,    32,    33,
      34,     0,     0,     0,     0,     0,     0,     0,    35,    36,
      37,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    38,     0,
       0,     0,    39,    40,    41,    42,    43,    44,    45,    46,
      47,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,     0,     5,     0,     6,   -14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     9,     0,
       0,     0,     0,    10,    11,     0,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,     0,    30,    31,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,     0,     0,
       0,     0,     0,     0,     0,    35,    36,    37,     0,     0,
       0,     0,     0,     0,     0,     1,     0,     0,     0,     0,
       2,     0,     3,     0,     4,    38,     5,     0,     6,    39,
      40,    41,    42,    43,    44,    45,    46,    47,     0,     7,
       8,     0,     9,     0,     0,     0,     0,    10,    11,     0,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,     0,     0,     0,     0,     0,     0,     0,    32,
      33,    34,     0,     0,     0,     0,     0,     0,     0,    35,
      36,    37,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     1,     0,     0,     0,     0,
       2,     0,     3,     0,     4,     0,     5,     0,     6,     0,
       0,    40,    41,    42,    43,    44,    45,    46,    47,     7,
       8,     0,     0,     0,     0,     0,     0,    10,    11,     0,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,     0,    30,
       0,     0,     0,     0,     0,     0,     0,   117,     0,    32,
      33,    34,     2,     0,     3,     0,     4,     0,     5,     0,
       0,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
       0,     0,     0,     0,    40,    41,    42,    43,    44,    45,
      46,    47,     0,     0,    24,    25,    26,    27,    28,    29,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    32,    33,    34,     2,     0,     3,     0,     4,     0,
       5,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,     0,     0,     0,     0,    41,    42,    43,
      95,    45,    46,     0,     0,     0,    24,    25,    26,    27,
      28,    29,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     2,     0,     3,     0,
       4,     0,     5,   254,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,     0,     0,     0,     0,     0,    41,
      42,    43,    95,    45,    46,     0,     0,     0,    24,    25,
      26,    27,    28,    29,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,     2,     0,
       3,     0,     4,   279,     5,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,     0,     0,     0,     0,
       0,    41,    42,    43,    95,    45,    46,     0,     0,     0,
      24,    25,    26,    27,    28,    29,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    33,    34,
       2,     0,     3,     0,     4,   315,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,     0,     0,    10,     0,     0,
       0,     0,     0,    41,    42,    43,    95,    45,    46,     0,
       0,     0,    24,    25,    26,    27,    28,    29,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    32,
      33,    34,     2,     0,     3,     0,     4,     0,     5,   317,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
       0,     0,     0,     0,     0,    41,    42,    43,    95,    45,
      46,     0,     0,     0,    24,    25,    26,    27,    28,    29,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    32,    33,    34,     2,     0,     3,     0,     4,   324,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,     0,     0,     0,     0,    41,    42,    43,
      95,    45,    46,     0,     0,     0,    24,    25,    26,    27,
      28,    29,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     2,     0,     3,     0,
       4,     0,     5,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,     0,     0,     0,     0,     0,    41,
      42,    43,    95,    45,    46,     0,     0,     0,    24,    25,
      26,    27,    28,    29,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,     2,     0,
       3,     0,     4,     0,     5,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,     0,     0,     0,     0,
       0,    41,    42,    43,    95,    45,    46,     0,     0,     0,
      24,    25,    26,    27,    28,    29,     0,    94,     0,     0,
       0,     0,     0,   148,   149,   150,     0,    32,    33,    34,
       2,     0,     3,     0,     4,     0,     5,   151,   152,   153,
     154,   155,   156,   157,   158,   159,     0,     0,     0,     7,
       8,     0,     0,     0,     0,     0,     0,    10,     0,     0,
       0,     0,     0,    41,    42,    43,   246,    45,    46,     0,
       0,     0,    24,    25,    26,    27,    28,    29,     0,     0,
       0,     0,   160,     0,     0,     0,     0,     0,     0,     0,
      33,    34,     0,     0,     0,     0,     0,     0,     0,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    41,    42,    43,   113,    45,
      46
};

static const yytype_int16 yycheck[] =
{
      26,     6,    13,   128,    30,   125,    19,   129,   137,   110,
      36,    37,     3,     4,     5,    69,     7,     8,    33,   136,
     145,     3,    30,    13,    67,    12,   143,     3,    29,    13,
      21,   238,    29,    13,   106,    13,    13,    34,    92,   135,
      12,    24,    25,    67,   340,    28,    29,     3,    12,   178,
      41,    56,     8,    29,    10,    79,    12,    38,    14,    40,
     114,    76,    12,   106,   360,    12,   340,    63,    94,    12,
     124,    27,    28,     3,    70,    68,    69,    67,   341,    35,
      16,   135,   106,    67,     3,   181,   360,    67,    32,    67,
      67,    12,   355,   101,    50,    51,    52,    53,    54,    55,
     307,    57,   128,    71,    72,   106,   369,    90,   371,   106,
      99,    67,    68,    69,   105,   106,   106,   108,   109,   145,
     240,   112,   106,   386,   106,   226,   106,   181,   106,   106,
     106,   106,    12,    16,    14,   257,    64,    65,    66,    67,
       8,     0,    10,    56,    12,    13,    14,   103,   104,   105,
     106,   107,   108,   109,    34,     7,     8,     9,   149,    27,
      28,    57,   216,   192,   193,   194,   195,    35,   304,   305,
      50,    51,   190,   191,   294,   304,   305,   203,   295,     3,
      60,    14,    50,    51,    52,    53,    54,    55,   179,    57,
     244,   106,   205,   184,   200,   201,   202,   196,   197,    67,
      68,    69,   256,    12,    62,    14,   198,   199,    74,    73,
      10,   331,    12,    75,    14,   206,   207,    31,    61,    99,
      11,    13,    15,    17,   278,    13,    13,    27,    28,     3,
      34,   356,   106,   353,     3,   103,   104,   105,   106,   107,
     108,    50,    51,   363,    29,    16,    14,    60,    65,    13,
     106,    60,    12,    14,    13,     5,   106,   106,   378,    12,
      14,    13,    13,    12,    12,     5,    13,   387,    13,    13,
     390,    17,    29,    12,    12,   301,   287,   288,   106,   100,
     291,   292,     3,    17,    59,    13,   106,     5,   100,    13,
      99,    34,     3,   284,   285,   106,    16,    12,   289,   290,
      13,    37,   293,   103,   104,   105,   106,   107,   108,    13,
      13,    13,     3,   324,    13,   341,    13,    12,     3,     3,
      12,    17,     5,    13,    13,   106,    13,    13,   360,   355,
     356,    12,    49,   335,   345,   340,   303,     3,   349,   333,
     311,   185,     8,   369,    10,   371,    12,   186,    14,   187,
      16,   188,   313,   189,    19,   360,    -1,    -1,    -1,    -1,
     386,    27,    28,    -1,    30,    -1,    -1,    -1,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    -1,    -1,    -1,   101,   102,   103,   104,   105,
     106,   107,   108,   109,     3,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    12,    -1,    14,    -1,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    30,    -1,    -1,    -1,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,   101,   102,   103,   104,   105,   106,   107,   108,
     109,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    30,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    97,    14,    -1,    16,   101,
     102,   103,   104,   105,   106,   107,   108,   109,    -1,    27,
      28,    -1,    30,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    57,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,   101,   102,   103,   104,   105,   106,   107,
     108,   109,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    -1,    14,    -1,    16,    -1,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    67,
      68,    69,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,   102,   103,   104,   105,   106,   107,
     108,   109,    -1,    -1,    50,    51,    52,    53,    54,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,     8,    -1,    10,    -1,    12,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,   107,   108,    -1,    -1,    -1,    50,    51,    52,    53,
      54,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,     8,    -1,    10,    -1,
      12,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,   108,    -1,    -1,    -1,    50,    51,
      52,    53,    54,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,     8,    -1,
      10,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,   107,   108,    -1,    -1,    -1,
      50,    51,    52,    53,    54,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,   107,   108,    -1,
      -1,    -1,    50,    51,    52,    53,    54,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,     8,    -1,    10,    -1,    12,    -1,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,
     108,    -1,    -1,    -1,    50,    51,    52,    53,    54,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,     8,    -1,    10,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,   107,   108,    -1,    -1,    -1,    50,    51,    52,    53,
      54,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,   108,    -1,    -1,    -1,    50,    51,
      52,    53,    54,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,   107,   108,    -1,    -1,    -1,
      50,    51,    52,    53,    54,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,     4,     5,     6,    -1,    67,    68,    69,
       8,    -1,    10,    -1,    12,    -1,    14,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,   107,   108,    -1,
      -1,    -1,    50,    51,    52,    53,    54,    55,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,
     108
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    30,
      35,    36,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      57,    58,    67,    68,    69,    77,    78,    79,    97,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   111,   112,
     113,   115,   121,   123,   124,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   142,   143,
     144,   145,   146,   148,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   170,   171,    57,   106,   138,    13,   138,    15,
     138,    17,   121,   138,   138,    12,    12,   128,    12,    12,
      12,   106,    12,   106,   170,   171,    16,     3,   138,     3,
       3,   166,   166,   106,   146,    32,   166,   166,    79,   146,
     106,   120,    12,   146,   125,   146,    99,    16,   138,    29,
      34,     0,   111,    56,    57,   122,   121,     3,     4,     5,
       6,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      63,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,   141,    14,
     142,   106,   149,   150,    33,    76,    62,    74,    75,    73,
      63,    70,    64,    65,    66,    67,    71,    72,    68,    69,
       7,     8,     9,    31,   166,    61,    12,    14,    50,    51,
      60,    99,   140,   142,    11,    13,    15,    17,    13,    13,
     138,   138,    38,    40,   138,   138,   137,     3,   138,    34,
      34,   106,   119,     3,   142,   127,   125,   149,    60,    65,
      13,   106,    12,   142,   143,   119,   106,   144,   147,    13,
     119,   125,   138,   144,    15,   142,   143,     5,   138,   156,
     157,   158,   159,   160,   161,   161,   162,   162,   162,   162,
     163,   163,   164,   164,   165,   165,   165,   146,   170,    13,
     138,   106,   106,   169,    12,    14,   142,    13,    13,    12,
      12,    13,    13,   137,    13,     5,    17,    12,   120,    29,
     127,    12,   106,   100,     3,     3,    17,    59,   114,   106,
     149,    34,    13,     5,   100,    13,   138,    15,   128,   128,
     138,   138,   128,   128,    13,   138,   127,   119,     3,   106,
     106,    13,   126,   146,    13,   123,   147,   147,     3,   120,
      16,    12,   154,   169,    13,    37,    13,    13,   128,    13,
       3,    13,   127,    13,   150,    12,   122,   113,   116,   117,
     118,   121,   145,    13,   126,   128,     3,     3,   128,    12,
     127,     5,    13,   126,   125,    17,   117,   127,    13,    13,
     126,   126,    13,   106,   127,    13,    12,    13,   126,   127,
      13,   127
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   110,   111,   111,   112,   112,   112,   113,   114,   114,
     115,   115,   116,   116,   117,   117,   118,   118,   118,   119,
     119,   120,   120,   121,   121,   122,   122,   123,   123,   124,
     124,   124,   124,   125,   125,   125,   126,   126,   127,   127,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   129,   129,   130,   131,   132,   132,   133,   134,
     134,   134,   134,   134,   134,   134,   135,   135,   136,   136,
     136,   136,   137,   137,   137,   138,   138,   139,   139,   140,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   142,
     142,   142,   143,   143,   143,   144,   144,   144,   145,   145,
     145,   145,   145,   145,   145,   146,   146,   146,   146,   147,
     147,   147,   147,   148,   149,   149,   150,   150,   150,   151,
     152,   153,   154,   154,   155,   155,   156,   156,   157,   157,
     158,   158,   159,   159,   160,   160,   160,   161,   161,   161,
     161,   161,   162,   162,   162,   163,   163,   163,   164,   164,
     164,   164,   165,   165,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   167,   167,   167,   167,   167,   168,   168,
     169,   169,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       4,     5,     2,     3,     2,     1,     2,     3,     7,     8,
       4,     6,     7,    11,    12,     1,     2,     3,     4,     2,
       3,     2,     3,     5,     1,     3,     1,     2,     2,     3,
       3,     3,     1,     5,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     1,     3,     1,     2,     2,     2,     2,     2,
       2,     3,     3,     1,     1,     1,     1,     2,     1,     3,
       1,     3,     1,     2,     3,     4,     3,     2,     2,     4,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     2
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
          case 125: /* type_decl2  */
#line 173 "eval/gwion.y" /* yacc.c:1257  */
      { printf("!!!!\n"); free_type_decl(((*yyvaluep).type_decl)); }
#line 1781 "eval/parser.c" /* yacc.c:1257  */
        break;

    case 146: /* type_decl  */
#line 173 "eval/gwion.y" /* yacc.c:1257  */
      { printf("!!!!\n"); free_type_decl(((*yyvaluep).type_decl)); }
#line 1787 "eval/parser.c" /* yacc.c:1257  */
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
#line 177 "eval/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL, get_pos(scanner));  }
#line 2055 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 178 "eval/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(scanner)); }
#line 2061 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 182 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2067 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 183 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(scanner)); }
#line 2073 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 184 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(scanner)); }
#line 2079 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 189 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].class_ext), (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->doc = get_doc(scanner);}
#line 2085 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 193 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = new_class_ext( (yyvsp[0].id_list), NULL, get_pos(scanner)); }
#line 2091 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 194 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = NULL; }
#line 2097 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 198 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_public; }
#line 2103 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 199 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_private; }
#line 2109 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 203 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2115 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 204 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2121 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 208 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body( (yyvsp[0].section), get_pos(scanner)); }
#line 2127 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 209 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body( (yyvsp[-1].section), (yyvsp[0].class_body), get_pos(scanner)); }
#line 2133 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 213 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list( (yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2139 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 214 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def( (yyvsp[0].func_def), get_pos(scanner)); }
#line 2145 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 215 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def( (yyvsp[0].class_def), get_pos(scanner)); }
#line 2151 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 219 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2157 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 220 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2163 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 224 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2169 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 225 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2175 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 229 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(scanner));}
#line 2181 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 230 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(scanner));}
#line 2187 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 234 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_static;   }
#line 2193 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 235 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_instance; }
#line 2199 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 239 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_func; }
#line 2205 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 240 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_variadic; }
#line 2211 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 244 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2217 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 245 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_key_static, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2223 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 246 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2229 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 247 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_key_static, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2235 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 251 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2241 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 252 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2247 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 253 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2253 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 257 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
#line 2259 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 258 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
#line 2265 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 262 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_code( NULL, get_pos(scanner)); }
#line 2271 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 263 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_code( (yyvsp[-1].stmt_list), get_pos(scanner)); }
#line 2277 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 282 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_enum((yyvsp[-2].id_list), NULL, get_pos(scanner)); }
#line 2283 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 283 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_enum((yyvsp[-3].id_list), (yyvsp[-1].sval), get_pos(scanner)); }
#line 2289 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 287 "eval/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_from_gotolabel((yyvsp[-1].sval), 1, get_pos(scanner)); }
#line 2295 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 291 "eval/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_from_gotolabel((yyvsp[-1].sval), 0, get_pos(scanner)); }
#line 2301 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 295 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2307 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 296 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2313 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 300 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner));}
#line 2319 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 305 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_while( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2325 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 307 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_while( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2331 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 309 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2337 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 311 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2343 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 313 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_until( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2349 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 315 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_until( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2355 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 317 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_loop( (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2361 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 322 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(scanner)); }
#line 2367 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 324 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2373 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 328 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_return( NULL, get_pos(scanner)); }
#line 2379 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 329 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_return( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2385 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 330 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_break(get_pos(scanner)); }
#line 2391 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 331 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_continue(get_pos(scanner)); }
#line 2397 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 335 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_expression((yyvsp[-1].exp),   get_pos(scanner)); }
#line 2403 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 336 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_expression(NULL, get_pos(scanner)); }
#line 2409 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 337 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_expression(NULL, get_pos(scanner)); append_doc(scanner, (yyvsp[0].sval)); }
#line 2415 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 341 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2421 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 342 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_expression((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2427 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 346 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2433 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 347 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2439 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 351 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2445 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 355 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2451 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 356 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2457 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 357 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; /* LCOV_EXCL_LINE */ }
#line 2463 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 358 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2469 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 359 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2475 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 360 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2481 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 361 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2487 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 362 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2493 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 363 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2499 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 364 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2505 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 365 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2511 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 366 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2517 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 367 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2523 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 368 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2529 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 369 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2535 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 370 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2541 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 371 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2547 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 372 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2553 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 373 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2559 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 374 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2565 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 375 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2571 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 376 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2577 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 377 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2583 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 378 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2589 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 379 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rinc; }
#line 2595 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 380 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rdec; }
#line 2601 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 381 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_runinc; }
#line 2607 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 382 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rundec; }
#line 2613 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 383 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2619 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 387 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( (yyvsp[-1].exp), get_pos(scanner) ); }
#line 2625 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 388 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[0].array_sub), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2631 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 389 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( new_array_sub( NULL, get_pos(scanner)), (yyvsp[-3].exp), get_pos(scanner)); }
#line 2637 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 393 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( NULL, get_pos(scanner)); }
#line 2643 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 394 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-2].array_sub), NULL, get_pos(scanner)); }
#line 2649 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 395 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-1].array_sub), (yyvsp[0].array_sub)->exp_list, get_pos(scanner)); }
#line 2655 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 400 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_decl_expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); }
#line 2661 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 401 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_decl_expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); }
#line 2667 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 406 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2673 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 408 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-7].ival), (yyvsp[-6].ival), (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2679 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 410 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_key_func, ae_key_instance, new_type_decl(new_id_list("void", get_pos(scanner)), 0, get_pos(scanner)), "dtor", NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_dtor; (yyval.func_def)->type_decl->doc = get_doc(scanner);}
#line 2685 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 412 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_key_func, ae_key_static, (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_op; (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2691 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 414 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_key_func, ae_key_static, (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_op; (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2697 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 416 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); (yyval.func_def)->types = (yyvsp[-8].id_list); }
#line 2703 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 418 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-7].ival), (yyvsp[-6].ival), (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); (yyval.func_def)->types = (yyvsp[-9].id_list); }
#line 2709 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 422 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[0].sval), get_pos(scanner)), 0, get_pos(scanner)); }
#line 2715 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 423 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), 1, get_pos(scanner)); }
#line 2721 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 424 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-1].id_list),  0, get_pos(scanner)); }
#line 2727 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 425 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list),  1, get_pos(scanner)); }
#line 2733 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 430 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list(&(yyvsp[-1].exp)->d.exp_decl, NULL); }
#line 2739 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 431 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list(&(yyvsp[-2].exp)->d.exp_decl, (yyvsp[0].decl_list)); }
#line 2745 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 432 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = NULL; }
#line 2751 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 433 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list(NULL, (yyvsp[0].decl_list)); }
#line 2757 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 437 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_union((yyvsp[-2].decl_list), get_pos(scanner)); }
#line 2763 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 441 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2769 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 442 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2775 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 446 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sval), NULL, get_pos(scanner)); }
#line 2781 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 447 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2787 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 448 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2793 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 453 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2799 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 458 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar((yyvsp[-1].exp), get_pos(scanner)); }
#line 2805 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 461 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec((yyvsp[-1].exp), get_pos(scanner)); }
#line 2811 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 466 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_if_expression( (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2817 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 470 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2823 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 472 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2829 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 476 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2835 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 478 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2841 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 482 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2847 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 484 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2853 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 488 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2859 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 490 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(scanner)); }
#line 2865 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 494 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2871 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 496 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2877 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 500 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2883 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 502 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_eq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2889 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 504 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_neq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2895 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 508 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2901 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 510 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_lt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2907 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 512 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_gt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2913 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 514 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_le, (yyvsp[0].exp), get_pos(scanner)); }
#line 2919 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 516 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_ge, (yyvsp[0].exp), get_pos(scanner)); }
#line 2925 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 520 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2931 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 522 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_shift_left, (yyvsp[0].exp), get_pos(scanner)); }
#line 2937 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 524 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_shift_right, (yyvsp[0].exp), get_pos(scanner)); }
#line 2943 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 528 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2949 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 530 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_plus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2955 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 532 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_minus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2961 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 536 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2967 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 538 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_times, (yyvsp[0].exp), get_pos(scanner)); }
#line 2973 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 540 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_divide, (yyvsp[0].exp), get_pos(scanner)); }
#line 2979 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 542 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_binary_expression( (yyvsp[-2].exp), op_percent, (yyvsp[0].exp), get_pos(scanner)); }
#line 2985 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 548 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_cast_expression( (yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2991 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 553 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2997 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 555 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_plusplus, (yyvsp[0].exp), get_pos(scanner)); }
#line 3003 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 557 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_minusminus, (yyvsp[0].exp), get_pos(scanner)); }
#line 3009 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 559 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 3015 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 561 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_typeof, (yyvsp[0].exp), get_pos(scanner)); }
#line 3021 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 563 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_sizeof, (yyvsp[0].exp), get_pos(scanner)); }
#line 3027 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 565 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(scanner)); }
#line 3033 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 567 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary2(op_new, (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3039 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 569 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary3( op_spork, (yyvsp[0].stmt), get_pos(scanner)); }
#line 3045 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 573 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3051 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 574 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3057 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 575 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3063 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 576 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3069 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 577 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3075 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 582 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 3081 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 586 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sval), get_pos(scanner)), NULL, get_pos(scanner)); }
#line 3087 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 587 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sval), get_pos(scanner)), (yyvsp[0].type_list), get_pos(scanner)); }
#line 3093 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 593 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array( (yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3099 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 595 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_func_call( (yyvsp[-2].exp), NULL, get_pos(scanner)); }
#line 3105 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 597 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_func_call( (yyvsp[-3].exp), (yyvsp[-1].exp), get_pos(scanner)); }
#line 3111 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 599 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_member_dot( (yyvsp[-2].exp), (yyvsp[0].sval), get_pos(scanner)); }
#line 3117 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 601 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_postfix_expression( (yyvsp[-1].exp), op_plusplus, get_pos(scanner)); }
#line 3123 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 603 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_postfix_expression( (yyvsp[-1].exp), op_minusminus, get_pos(scanner)); }
#line 3129 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 605 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_func_call( (yyvsp[-3].exp), NULL, get_pos(scanner)); (yyval.exp)->d.exp_func.types = (yyvsp[-2].type_list); }
#line 3135 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 607 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_func_call( (yyvsp[-4].exp), (yyvsp[-1].exp), get_pos(scanner)); (yyval.exp)->d.exp_func.types = (yyvsp[-3].type_list); }
#line 3141 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 611 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_ID(     (yyvsp[0].sval), get_pos(scanner)); }
#line 3147 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 612 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_int(    (yyvsp[0].ival), get_pos(scanner)); }
#line 3153 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 613 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_float(  (yyvsp[0].fval), get_pos(scanner)); }
#line 3159 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 614 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_string( (yyvsp[0].sval), get_pos(scanner)); }
#line 3165 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 615 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_char(                  (yyvsp[0].sval), get_pos(scanner)); }
#line 3171 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 616 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_array_lit(             (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3177 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 617 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_array_lit(           (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3183 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 618 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_complex(               (yyvsp[0].c_val), get_pos(scanner)); }
#line 3189 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 619 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_polar(                 (yyvsp[0].polar), get_pos(scanner)); }
#line 3195 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 620 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_vec(                   (yyvsp[0].vec), get_pos(scanner)); }
#line 3201 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 621 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_hack_expression(                (yyvsp[-1].exp), get_pos(scanner)); }
#line 3207 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 622 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3213 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 623 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_primary_expression_from_nil(       get_pos(scanner)); }
#line 3219 "eval/parser.c" /* yacc.c:1646  */
    break;


#line 3223 "eval/parser.c" /* yacc.c:1646  */
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
#line 625 "eval/gwion.y" /* yacc.c:1906  */

