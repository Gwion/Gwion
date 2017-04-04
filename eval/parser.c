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
    ARROW_LEFT = 354,
    ARROW_RIGHT = 355,
    LTB = 356,
    GTB = 357,
    VARARG = 358,
    UNION = 359,
    ATPAREN = 360,
    NUM = 361,
    FLOAT = 362,
    ID = 363,
    STRING_LIT = 364,
    CHAR_LIT = 365,
    DOC = 366
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
#define ARROW_LEFT 354
#define ARROW_RIGHT 355
#define LTB 356
#define GTB 357
#define VARARG 358
#define UNION 359
#define ATPAREN 360
#define NUM 361
#define FLOAT 362
#define ID 363
#define STRING_LIT 364
#define CHAR_LIT 365
#define DOC 366

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
//  Class_Ext iface_ext;
  Class_Def class_def;
  Ast ast;

#line 405 "eval/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int minimal_parse (MyArg* arg);

#endif /* !YY_MINIMAL_EVAL_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 421 "eval/parser.c" /* yacc.c:358  */

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
#define YYLAST   1334

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  112
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  65
/* YYNRULES -- Number of rules.  */
#define YYNRULES  217
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  398

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   366

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
     105,   106,   107,   108,   109,   110,   111
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   182,   182,   183,   187,   188,   189,   193,   195,   204,
     208,   209,   213,   214,   218,   219,   223,   224,   225,   233,
     234,   238,   239,   243,   244,   248,   249,   253,   254,   263,
     264,   265,   266,   270,   271,   272,   276,   277,   281,   282,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   301,   302,   306,   310,   314,   315,   319,   323,
     325,   327,   329,   331,   333,   335,   340,   342,   347,   348,
     349,   350,   354,   355,   356,   360,   361,   365,   366,   369,
     370,   374,   375,   379,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   416,   417,   422,   423,   427,   428,   432,
     436,   438,   440,   442,   444,   446,   451,   452,   453,   454,
     459,   460,   461,   462,   466,   470,   471,   475,   476,   477,
     481,   486,   490,   493,   494,   499,   500,   505,   506,   511,
     512,   517,   518,   523,   524,   529,   530,   532,   537,   538,
     540,   542,   544,   549,   550,   552,   557,   558,   560,   565,
     566,   568,   570,   575,   576,   581,   582,   588,   589,   591,
     593,   595,   597,   599,   601,   603,   608,   609,   610,   611,
     612,   613,   618,   619,   623,   624,   628,   629,   631,   633,
     635,   637,   639,   641,   643,   647,   648,   649,   650,   651,
     652,   653,   654,   655,   656,   657,   658,   659
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
  "RUNINC", "RUNDEC", "TEMPLATE", "NOELSE", "ARROW_LEFT", "ARROW_RIGHT",
  "LTB", "GTB", "VARARG", "UNION", "ATPAREN", "NUM", "FLOAT", "ID",
  "STRING_LIT", "CHAR_LIT", "DOC", "$accept", "ast", "section",
  "class_def", "class_ext", "class_decl", "class_body", "class_body2",
  "class_section", "id_list", "id_dot", "stmt_list", "static_decl",
  "function_decl", "func_ptr", "type_decl2", "arg_list", "code_segment",
  "stmt", "enum_stmt", "label_stmt", "goto_stmt", "case_stmt",
  "switch_stmt", "loop_stmt", "selection_stmt", "jump_stmt", "exp_stmt",
  "exp", "arrow_operator", "binary_exp", "arrow_expression", "template",
  "op", "array_exp", "array_empty", "decl_exp", "func_def", "type_decl",
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
     365,   366
};
# endif

#define YYPACT_NINF -217

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-217)))

#define YYTABLE_NINF -127

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     636,  -217,  -217,  1114,   832,   894,    33,  1114,  1114,  -217,
    -217,  -217,    14,    16,   722,    34,    41,    45,   -70,    50,
     145,    48,   784,    89,   106,  1224,  1224,    -4,    67,  1224,
    1224,    57,  -217,    37,  -217,  -217,   136,    -4,    -4,    49,
    -217,   140,  1114,  -217,  -217,   -13,  -217,  -217,  -217,   158,
     332,  -217,   104,  -217,   105,  -217,  -217,   722,  -217,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,  -217,   165,  1221,   -66,
    -217,   173,  -217,  -217,    92,  -217,  -217,  -217,  -217,  -217,
      -6,   144,   134,   138,   142,   -38,    62,    82,    96,   176,
     190,   196,  -217,  1224,   168,   166,  -217,   -10,   219,  -217,
     218,  -217,   217,  -217,   216,   221,   222,  1114,  1114,    -9,
    1114,  1114,   193,   233,  1114,  -217,   101,   203,   130,  -217,
     236,  -217,  -217,  -217,  -217,   211,   227,   240,  -217,  -217,
      -4,   197,   194,   245,   156,   253,   130,  1162,   255,  -217,
    -217,  -217,  -217,   130,  -217,    -4,  -217,  -217,  -217,  1114,
    -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  1114,  -217,
    -217,  1114,   251,   256,  -217,   264,  1114,  1224,  1224,  1224,
    1224,  1224,  1224,  1224,  1224,  1224,  1224,  1224,  1224,  1224,
    1224,  1224,  1224,  1224,  1224,  1224,    -4,  -217,   145,   942,
    1114,  -217,  -217,   163,   171,   261,  -217,  -217,  -217,   227,
    -217,  -217,  -217,   267,   270,   262,   272,   273,   275,   193,
    -217,   276,  -217,  -217,   280,   277,  -217,  -217,  -217,   278,
      37,   268,   240,   281,   188,   204,    15,   302,   290,  -217,
     108,   200,   256,  -217,   -66,  -217,  -217,  -217,   173,    92,
     279,   144,   134,   138,   142,   -38,    62,    62,    82,    82,
      82,    82,    96,    96,   176,   176,   190,   190,   190,   196,
    -217,   166,  -217,   296,  -217,   305,   209,  1004,  -217,   722,
     722,  1114,  1114,   722,   722,  1052,   240,   130,    12,   206,
    -217,  -217,  -217,    22,   299,    -7,  1162,  1162,   312,   441,
      37,   301,   306,  -217,   173,  -217,  1224,  -217,   171,  -217,
    -217,   307,   282,  -217,   308,   309,  -217,  -217,   722,   310,
    -217,  -217,  -217,   322,   313,   240,   314,    92,   316,   105,
    -217,  -217,  -217,  -217,   315,  -217,   550,  -217,  -217,  -217,
     441,    24,  -217,  -217,  -217,   722,   326,   327,  -217,   722,
    -217,   319,  -217,   240,   328,    26,    -4,  -217,  -217,   317,
     240,   323,  -217,  -217,  -217,  -217,    38,  -217,    -4,  -217,
     324,   237,  -217,  -217,   240,  -217,   337,  -217,  -217,   329,
    -217,  -217,    39,   240,   339,  -217,   240,  -217
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      11,    73,   190,     0,     0,     0,     0,     0,     0,    27,
     188,   189,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    10,     0,   187,   186,     0,     0,     0,     0,
      28,     0,     0,   206,   207,   205,   208,   209,    74,     0,
       2,     6,     0,     4,    26,    50,    43,    23,    48,    44,
      45,    47,    46,    41,    42,    49,    40,     0,    75,    79,
     210,   211,    81,     5,     0,    51,   212,   213,   214,   117,
     143,   145,   147,   149,   151,   153,   155,   158,   163,   166,
     169,   173,   175,     0,   177,   192,   196,   205,     0,   217,
       0,   115,     0,    38,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   205,     0,   196,     0,    68,
       0,    70,    71,   178,   179,   126,   183,   191,   182,   181,
       0,    21,     0,     0,     0,     0,     0,     0,     0,   127,
      54,     1,     3,     0,    25,     0,    24,    72,    84,     0,
     112,    87,    88,    89,    90,    91,    86,    85,    92,    93,
     111,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,     0,    77,
      78,     0,     0,   137,   118,   135,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   180,     0,     0,
       0,   201,   202,     0,     0,     0,   197,   215,   216,   113,
      39,   141,   140,     0,     0,     0,     0,     0,     0,     0,
      55,     0,    57,    56,    19,     0,    69,   184,   185,     0,
       0,   128,     0,     0,     0,     0,   205,     0,     0,   142,
       0,     0,    33,    76,    80,    82,   116,   138,   139,     0,
       0,   146,   148,   150,   152,   154,   156,   157,   162,   160,
     161,   159,   164,   165,   168,   167,   171,   170,   172,   174,
     176,   193,   198,     0,   200,   194,     0,     0,   114,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,   129,   121,     0,     0,     0,   132,   130,     0,    11,
       0,     0,     0,    35,    34,   136,     0,   199,     0,    83,
     203,     0,    66,    59,     0,     0,    63,    65,     0,     0,
      58,    20,    52,     0,     0,     0,     0,     0,     0,    26,
     133,   131,   134,    18,     0,    12,    11,    16,    17,     9,
      11,     0,   144,   195,   204,     0,     0,     0,    61,     0,
      53,     0,   122,     0,    36,     0,     0,     7,    15,     0,
       0,     0,    67,    60,    64,    62,     0,   123,     0,    29,
       0,     0,     8,   119,     0,    30,     0,    37,    31,     0,
     120,    32,     0,     0,     0,   124,     0,   125
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -217,   303,  -217,    17,  -217,  -217,     4,     9,  -217,  -123,
    -216,   119,    18,    51,  -217,    -8,  -188,  -121,   -12,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,  -217,   -98,     0,  -217,
    -217,   183,  -217,  -217,   -86,  -135,  -125,    66,   -27,  -136,
    -217,   107,    32,  -217,  -217,  -217,    47,  -217,   205,   207,
     202,   208,   212,    19,    -3,    20,    23,    -5,   189,    29,
    -217,  -217,    75,   -19,   376
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    49,    50,   343,   311,    52,   344,   345,   346,   235,
     132,   347,   145,    54,    55,   251,   336,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,   181,
      68,    69,   215,   178,    70,    71,    72,   348,    74,   248,
      75,   184,   185,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,   286,    95,    96
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     126,   116,   109,    98,   100,   102,   238,   105,   106,   216,
     134,   135,   247,   245,   229,   332,   139,    51,   306,   139,
     250,   140,   120,     9,   300,   192,   107,   186,   108,   225,
     216,   226,   193,   179,   180,   335,     1,   370,   113,   379,
     237,     2,   138,     3,   139,     4,   110,     5,   258,     6,
     103,   385,   393,   111,   123,   124,   255,   112,   128,   129,
       7,     8,   114,    33,   118,    10,    73,    51,    11,    12,
     187,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    33,
      31,    33,   121,    33,   349,  -126,    40,   257,  -126,   127,
      33,    34,    35,   239,   125,    33,    33,   223,   224,   122,
     227,   228,    38,   209,   231,   210,    73,   314,   252,    53,
     333,   302,   207,  -126,   309,   104,   194,   195,   196,   197,
     125,   295,   125,   288,   125,   232,   130,    41,    42,    43,
      44,    45,    46,    47,    48,   131,   125,   125,   133,   253,
     136,   211,   212,   198,   199,     3,   137,     4,   141,     5,
     143,   213,   144,   371,   200,   201,   313,   310,   147,    53,
     340,   341,     7,     8,   331,   330,   146,   380,   209,   280,
     210,   247,   247,   202,   203,   204,   260,   182,   386,   281,
     387,   268,   269,   270,   271,   216,     1,   276,   277,   278,
     183,     2,   214,     3,   394,     4,   188,     5,   189,   283,
     102,   266,   267,   190,   362,   191,   211,   212,   272,   273,
       7,     8,   205,   274,   275,    10,   213,   206,    11,   208,
     217,   218,   219,   220,   221,   222,   230,   233,   234,   236,
     139,   210,   377,    25,    26,    27,    28,    29,    30,   383,
      42,    43,    44,   115,    46,    47,     6,   240,   242,   241,
      33,    34,    35,   390,   243,   244,   256,   214,   249,   259,
       5,   284,   395,   287,   291,   397,   337,   322,   323,   285,
     289,   326,   327,   290,   292,   297,   293,   321,   294,   296,
     299,   324,   325,   303,   298,   329,   304,   301,    42,    43,
      44,    97,    46,    47,    48,   307,   305,   308,   312,   317,
     318,   319,   338,   316,   334,   342,   358,   350,   351,   355,
     354,   356,   357,   359,   337,   360,   361,   363,   365,   373,
     374,   376,   367,   378,   382,     1,   384,   388,   337,   252,
       2,   392,     3,   372,     4,   389,     5,   375,     6,   337,
     391,   337,   396,   142,   369,   368,   339,   366,   381,     7,
       8,   254,     9,   352,    10,   337,   315,    11,    12,   364,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,   -11,    31,
      32,   263,   261,   353,   279,   262,   117,     0,   264,    33,
      34,    35,     0,   265,     0,     0,     0,     0,     0,    36,
      37,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    39,
       0,     0,     0,     0,     0,    40,    41,    42,    43,    44,
      45,    46,    47,    48,     1,     0,     0,     0,     0,     2,
       0,     3,     0,     4,     0,     5,     0,     6,   -13,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
       0,     9,     0,    10,     0,     0,    11,    12,     0,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,     0,    31,    32,
       0,     0,     0,     0,     0,     0,     0,     0,    33,    34,
      35,     0,     0,     0,     0,     0,     0,     0,    36,    37,
      38,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    39,     0,
       0,     0,     0,     0,    40,    41,    42,    43,    44,    45,
      46,    47,    48,     1,     0,     0,     0,     0,     2,     0,
       3,     0,     4,     0,     5,     0,     6,   -14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       9,     0,    10,     0,     0,    11,    12,     0,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,     0,    31,    32,     0,
       0,     0,     0,     0,     0,     0,     0,    33,    34,    35,
       0,     0,     0,     0,     0,     0,     0,    36,    37,    38,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       0,     0,     0,     0,     2,     0,     3,    39,     4,     0,
       5,     0,     6,    40,    41,    42,    43,    44,    45,    46,
      47,    48,     0,     7,     8,     0,     9,     0,    10,     0,
       0,    11,    12,     0,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,     0,    31,    32,     0,     0,     0,     0,     0,
       0,     0,     0,    33,    34,    35,     0,     0,     0,     0,
       0,     0,     0,    36,    37,    38,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     1,     0,     0,     0,     0,
       2,     0,     3,    39,     4,     0,     5,     0,     6,    40,
      41,    42,    43,    44,    45,    46,    47,    48,     0,     7,
       8,     0,     0,     0,    10,     0,     0,    11,    12,     0,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,     0,    31,
       0,     0,     0,     0,     0,     0,     0,   119,     0,    33,
      34,    35,     2,     0,     3,     0,     4,     0,     5,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,    10,     0,     0,    11,
       0,     0,     0,     0,     0,     0,    41,    42,    43,    44,
      45,    46,    47,    48,    25,    26,    27,    28,    29,    30,
       2,     0,     3,     0,     4,    99,     5,     0,     0,     0,
       0,    33,    34,    35,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,    10,     0,     0,    11,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,    26,    27,    28,    29,    30,     0,    42,
      43,    44,    97,    46,    47,     0,     0,     0,     0,    33,
      34,    35,     2,     0,     3,     0,     4,     0,     5,   101,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,    10,     0,     0,    11,
       0,     0,     0,     0,     0,     0,     0,    42,    43,    44,
      97,    46,    47,     0,    25,    26,    27,    28,    29,    30,
       2,     0,     3,     0,     4,   282,     5,     0,     0,     0,
       0,    33,    34,    35,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,    10,     0,     0,    11,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,    26,    27,    28,    29,    30,     0,    42,
      43,    44,    97,    46,    47,     0,     0,     0,     0,    33,
      34,    35,     2,     0,     3,     0,     4,   320,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,    10,     0,     0,    11,
       0,     0,     0,     0,     0,     0,     0,    42,    43,    44,
      97,    46,    47,     0,    25,    26,    27,    28,    29,    30,
       2,     0,     3,     0,     4,   328,     5,     0,     0,     0,
       0,    33,    34,    35,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,    10,     0,     0,    11,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,    26,    27,    28,    29,    30,     0,    42,
      43,    44,    97,    46,    47,     0,     0,     0,     0,    33,
      34,    35,     2,     0,     3,     0,     4,     0,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,    10,     0,     0,    11,
       0,     0,     0,     0,     0,     0,     0,    42,    43,    44,
      97,    46,    47,     0,    25,    26,    27,    28,    29,    30,
       2,     0,     3,     0,     4,     0,     5,     0,     0,     0,
       0,    33,    34,    35,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,    10,     0,     0,    11,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,    26,    27,    28,    29,    30,     0,    42,
      43,    44,    97,    46,    47,   148,   149,   150,     0,    33,
      34,    35,     2,     0,     3,     0,     4,     0,     5,   151,
     152,   153,   154,   155,   156,   157,   158,   159,     0,     0,
       0,     7,     8,     0,     0,     0,    10,     0,     0,    11,
       0,     0,     0,     0,     0,     0,     0,    42,    43,    44,
     246,    46,    47,     0,    25,    26,    27,    28,    29,    30,
       0,     0,     0,     0,   160,     0,     0,     0,     0,     0,
       0,     0,    34,    35,     0,     0,     0,     0,     0,     0,
       0,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    42,
      43,    44,   115,    46,    47
};

static const yytype_int16 yycheck[] =
{
      27,    20,    14,     3,     4,     5,   127,     7,     8,    95,
      37,    38,   137,   136,   112,     3,    29,     0,     3,    29,
     143,    34,    22,    30,   240,    63,    12,    33,    12,    38,
     116,    40,    70,    99,   100,    13,     3,    13,   108,    13,
     126,     8,    42,    10,    29,    12,    12,    14,   183,    16,
      17,    13,    13,    12,    25,    26,   181,    12,    29,    30,
      27,    28,    12,    67,    16,    32,     0,    50,    35,    36,
      76,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    67,
      57,    67,     3,    67,   310,   108,   103,   183,   108,    32,
      67,    68,    69,   130,   108,    67,    67,   107,   108,     3,
     110,   111,    79,    12,   114,    14,    50,   252,   145,     0,
     108,   242,    93,   108,    16,     6,    64,    65,    66,    67,
     108,   229,   108,   219,   108,    34,    79,   104,   105,   106,
     107,   108,   109,   110,   111,   108,   108,   108,    12,   149,
     101,    50,    51,    71,    72,    10,    16,    12,     0,    14,
      56,    60,    57,   351,    68,    69,   252,    59,     3,    50,
     306,   307,    27,    28,   297,   296,    57,   365,    12,   206,
      14,   306,   307,     7,     8,     9,   186,    14,   376,   208,
     378,   194,   195,   196,   197,   281,     3,   202,   203,   204,
     108,     8,   101,    10,   392,    12,    62,    14,    74,   209,
     210,   192,   193,    75,   335,    73,    50,    51,   198,   199,
      27,    28,    32,   200,   201,    32,    60,    31,    35,    61,
      11,    13,    15,    17,    13,    13,     3,    34,   108,     3,
      29,    14,   363,    50,    51,    52,    53,    54,    55,   370,
     105,   106,   107,   108,   109,   110,    16,    60,    13,    65,
      67,    68,    69,   384,   108,    12,    15,   101,    13,     5,
      14,   108,   393,    12,    12,   396,   303,   289,   290,   108,
      13,   293,   294,    13,    12,     5,    13,   287,    13,    13,
      12,   291,   292,    12,    17,   295,   108,    29,   105,   106,
     107,   108,   109,   110,   111,     3,   102,    17,   108,    13,
       5,   102,    13,    34,   108,     3,   328,    16,    12,    37,
      13,    13,    13,    13,   351,     3,    13,    13,    12,     3,
       3,    12,    17,     5,    17,     3,    13,    13,   365,   366,
       8,    12,    10,   355,    12,   108,    14,   359,    16,   376,
      13,   378,    13,    50,   350,   346,   305,   339,   366,    27,
      28,   178,    30,   316,    32,   392,   259,    35,    36,   337,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,   189,   187,   318,   205,   188,    20,    -1,   190,    67,
      68,    69,    -1,   191,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,     3,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    12,    -1,    14,    -1,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    30,    -1,    32,    -1,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,   107,   108,
     109,   110,   111,     3,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    16,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      30,    -1,    32,    -1,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    97,    12,    -1,
      14,    -1,    16,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,    27,    28,    -1,    30,    -1,    32,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    97,    12,    -1,    14,    -1,    16,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,    27,
      28,    -1,    -1,    -1,    32,    -1,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    67,
      68,    69,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    32,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,   107,
     108,   109,   110,   111,    50,    51,    52,    53,    54,    55,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    54,    55,    -1,   105,
     106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,    67,
      68,    69,     8,    -1,    10,    -1,    12,    -1,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    32,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,   109,   110,    -1,    50,    51,    52,    53,    54,    55,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    54,    55,    -1,   105,
     106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,    67,
      68,    69,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    32,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,   109,   110,    -1,    50,    51,    52,    53,    54,    55,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    54,    55,    -1,   105,
     106,   107,   108,   109,   110,    -1,    -1,    -1,    -1,    67,
      68,    69,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    32,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,   109,   110,    -1,    50,    51,    52,    53,    54,    55,
       8,    -1,    10,    -1,    12,    -1,    14,    -1,    -1,    -1,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    54,    55,    -1,   105,
     106,   107,   108,   109,   110,     4,     5,     6,    -1,    67,
      68,    69,     8,    -1,    10,    -1,    12,    -1,    14,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    32,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,   109,   110,    -1,    50,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,   109,   110
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    30,
      32,    35,    36,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    57,    58,    67,    68,    69,    77,    78,    79,    97,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   113,
     114,   115,   117,   123,   125,   126,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   142,   143,
     146,   147,   148,   149,   150,   152,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   175,   176,   108,   140,    13,
     140,    15,   140,    17,   123,   140,   140,    12,    12,   130,
      12,    12,    12,   108,    12,   108,   175,   176,    16,     3,
     140,     3,     3,   171,   171,   108,   150,    32,   171,   171,
      79,   108,   122,    12,   150,   150,   101,    16,   140,    29,
      34,     0,   113,    56,    57,   124,   123,     3,     4,     5,
       6,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      63,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,   145,    99,
     100,   141,    14,   108,   153,   154,    33,    76,    62,    74,
      75,    73,    63,    70,    64,    65,    66,    67,    71,    72,
      68,    69,     7,     8,     9,    32,    31,   171,    61,    12,
      14,    50,    51,    60,   101,   144,   146,    11,    13,    15,
      17,    13,    13,   140,   140,    38,    40,   140,   140,   139,
       3,   140,    34,    34,   108,   121,     3,   146,   129,   150,
      60,    65,    13,   108,    12,   121,   108,   148,   151,    13,
     121,   127,   150,   140,   143,   148,    15,   146,   147,     5,
     140,   160,   161,   162,   163,   164,   165,   165,   166,   166,
     166,   166,   167,   167,   168,   168,   169,   169,   169,   170,
     150,   175,    13,   140,   108,   108,   174,    12,   146,    13,
      13,    12,    12,    13,    13,   139,    13,     5,    17,    12,
     122,    29,   129,    12,   108,   102,     3,     3,    17,    16,
      59,   116,   108,   146,   147,   153,    34,    13,     5,   102,
      13,   140,   130,   130,   140,   140,   130,   130,    13,   140,
     129,   121,     3,   108,   108,    13,   128,   150,    13,   125,
     151,   151,     3,   115,   118,   119,   120,   123,   149,   122,
      16,    12,   158,   174,    13,    37,    13,    13,   130,    13,
       3,    13,   129,    13,   154,    12,   124,    17,   119,   118,
      13,   128,   130,     3,     3,   130,    12,   129,     5,    13,
     128,   127,    17,   129,    13,    13,   128,   128,    13,   108,
     129,    13,    12,    13,   128,   129,    13,   129
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   112,   113,   113,   114,   114,   114,   115,   115,   116,
     117,   117,   118,   118,   119,   119,   120,   120,   120,   121,
     121,   122,   122,   123,   123,   124,   124,   125,   125,   126,
     126,   126,   126,   127,   127,   127,   128,   128,   129,   129,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   131,   131,   132,   133,   134,   134,   135,   136,
     136,   136,   136,   136,   136,   136,   137,   137,   138,   138,
     138,   138,   139,   139,   139,   140,   140,   141,   141,   142,
     142,   143,   143,   144,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   146,   146,   147,   147,   148,   148,   149,
     149,   149,   149,   149,   149,   149,   150,   150,   150,   150,
     151,   151,   151,   151,   152,   153,   153,   154,   154,   154,
     155,   156,   157,   158,   158,   159,   159,   160,   160,   161,
     161,   162,   162,   163,   163,   164,   164,   164,   165,   165,
     165,   165,   165,   166,   166,   166,   167,   167,   167,   168,
     168,   168,   168,   169,   169,   170,   170,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   172,   172,   172,   172,
     172,   172,   173,   173,   174,   174,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     6,     7,     2,
       1,     0,     1,     0,     1,     2,     1,     1,     1,     1,
       3,     1,     3,     1,     2,     1,     0,     1,     1,     7,
       8,     8,     9,     1,     2,     2,     2,     4,     2,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     5,     6,     2,     3,     3,     3,     5,     5,
       7,     6,     7,     5,     7,     5,     5,     7,     2,     3,
       2,     2,     2,     1,     1,     1,     3,     1,     1,     1,
       3,     1,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     2,     3,     1,     2,     7,
       8,     4,     6,     7,    11,    12,     1,     2,     3,     4,
       2,     3,     2,     3,     5,     1,     3,     1,     2,     2,
       3,     3,     3,     1,     5,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     1,     3,     1,     3,     1,     2,     2,
       2,     2,     2,     2,     3,     3,     1,     1,     1,     1,
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
          case 127: /* type_decl2  */
#line 178 "eval/gwion.y" /* yacc.c:1257  */
      { printf("!!!!\n"); free_Type_Decl(((*yyvaluep).type_decl)); }
#line 1744 "eval/parser.c" /* yacc.c:1257  */
        break;

    case 150: /* type_decl  */
#line 178 "eval/gwion.y" /* yacc.c:1257  */
      { printf("!!!!\n"); free_Type_Decl(((*yyvaluep).type_decl)); }
#line 1750 "eval/parser.c" /* yacc.c:1257  */
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
#line 182 "eval/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_Ast((yyvsp[0].section), NULL, get_pos(scanner));  }
#line 2018 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 183 "eval/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_Ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(scanner)); }
#line 2024 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 187 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Stmt_List((yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2030 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 188 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Func_Def ((yyvsp[0].func_def), get_pos(scanner)); }
#line 2036 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 189 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Class_Def((yyvsp[0].class_def), get_pos(scanner)); }
#line 2042 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 194 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-5].ival), (yyvsp[-3].id_list), NULL, (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->doc = get_doc(scanner); }
#line 2048 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 196 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].class_ext), (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->doc = get_doc(scanner);}
#line 2054 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 204 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = new_class_ext( (yyvsp[0].id_list), NULL, get_pos(scanner)); }
#line 2060 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 208 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_public; }
#line 2066 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 209 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_private; }
#line 2072 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 213 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2078 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 214 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2084 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 218 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body( (yyvsp[0].section), get_pos(scanner)); }
#line 2090 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 219 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body( (yyvsp[-1].section), (yyvsp[0].class_body), get_pos(scanner)); }
#line 2096 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 223 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Stmt_List( (yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2102 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 224 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Func_Def( (yyvsp[0].func_def), get_pos(scanner)); }
#line 2108 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 225 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Class_Def( (yyvsp[0].class_def), get_pos(scanner)); }
#line 2114 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 233 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2120 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 234 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2126 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 238 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2132 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 239 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2138 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 243 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_Stmt_List((yyvsp[0].stmt), NULL, get_pos(scanner));}
#line 2144 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 244 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_Stmt_List((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(scanner));}
#line 2150 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 248 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_static;   }
#line 2156 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 249 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_instance; }
#line 2162 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 253 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_func; }
#line 2168 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 254 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_variadic; }
#line 2174 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 263 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Func_Ptr_Stmt(0, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2180 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 264 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Func_Ptr_Stmt(ae_key_static, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2186 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 265 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Func_Ptr_Stmt(0, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2192 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 266 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Func_Ptr_Stmt(ae_key_static, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2198 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 270 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2204 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 271 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2210 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 272 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2216 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 276 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_Arg_List((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
#line 2222 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 277 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_Arg_List((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
#line 2228 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 281 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_code( NULL, get_pos(scanner)); }
#line 2234 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 282 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_code( (yyvsp[-1].stmt_list), get_pos(scanner)); }
#line 2240 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 297 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_Union((yyvsp[0].union_stmt), 0); }
#line 2246 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 301 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_enum((yyvsp[-2].id_list), NULL, get_pos(scanner)); }
#line 2252 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 302 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_enum((yyvsp[-3].id_list), (yyvsp[-1].sval), get_pos(scanner)); }
#line 2258 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 306 "eval/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_from_gotolabel((yyvsp[-1].sval), 1, get_pos(scanner)); }
#line 2264 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 310 "eval/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_from_gotolabel((yyvsp[-1].sval), 0, get_pos(scanner)); }
#line 2270 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 314 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2276 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 315 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2282 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 319 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner));}
#line 2288 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 324 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_while( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2294 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 326 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_while( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2300 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 328 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2306 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 330 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2312 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 332 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_until( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2318 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 334 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_until( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2324 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 336 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_loop( (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2330 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 341 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(scanner)); }
#line 2336 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 343 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2342 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 347 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_return( NULL, get_pos(scanner)); }
#line 2348 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 348 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_return( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2354 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 349 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_break(get_pos(scanner)); }
#line 2360 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 350 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_continue(get_pos(scanner)); }
#line 2366 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 354 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Stmt_from_Expression((yyvsp[-1].exp),   get_pos(scanner)); }
#line 2372 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 355 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Stmt_from_Expression(NULL, get_pos(scanner)); }
#line 2378 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 356 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Stmt_from_Expression(NULL, get_pos(scanner)); append_doc(scanner, (yyvsp[0].sval)); }
#line 2384 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 360 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2390 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 361 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_Expression((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2396 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 365 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_op_arrow_left; }
#line 2402 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 366 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_op_arrow_right; }
#line 2408 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 369 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2414 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 370 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2420 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 374 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2426 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 376 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2432 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 379 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2438 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 384 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2444 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 385 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2450 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 386 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2456 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 387 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2462 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 388 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2468 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 389 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2474 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 390 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2480 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 391 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2486 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 392 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2492 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 393 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2498 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 394 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2504 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 395 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2510 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 396 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2516 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 397 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2522 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 398 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2528 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 399 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2534 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 400 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2540 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 401 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2546 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 402 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2552 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 403 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2558 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 404 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2564 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 405 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2570 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 406 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2576 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 407 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rinc; }
#line 2582 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 408 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rdec; }
#line 2588 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 409 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_runinc; }
#line 2594 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 410 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rundec; }
#line 2600 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 411 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2606 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 412 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2612 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 416 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( (yyvsp[-1].exp), get_pos(scanner) ); }
#line 2618 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 418 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[0].array_sub), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2624 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 422 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( NULL, get_pos(scanner)); }
#line 2630 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 423 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-2].array_sub), NULL, get_pos(scanner)); }
#line 2636 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 428 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_Decl_Expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); }
#line 2642 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 433 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2648 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 437 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-7].ival), (yyvsp[-6].ival), (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2654 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 439 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def(ae_key_func, ae_key_instance, new_Type_Decl(new_id_list("void", get_pos(scanner)), 0, get_pos(scanner)), "dtor", NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_dtor; (yyval.func_def)->type_decl->doc = get_doc(scanner);}
#line 2660 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 441 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def(ae_key_func, ae_key_static, (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_op; (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2666 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 443 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def(ae_key_func, ae_key_static, (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_op; (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2672 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 445 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); (yyval.func_def)->types = (yyvsp[-8].id_list); }
#line 2678 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 447 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-7].ival), (yyvsp[-6].ival), (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); (yyval.func_def)->types = (yyvsp[-9].id_list); }
#line 2684 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 451 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_Type_Decl(new_id_list((yyvsp[0].sval), get_pos(scanner)), 0, get_pos(scanner)); }
#line 2690 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 452 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_Type_Decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), 1, get_pos(scanner)); }
#line 2696 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 453 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_Type_Decl((yyvsp[-1].id_list),  0, get_pos(scanner)); }
#line 2702 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 454 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_Type_Decl((yyvsp[-2].id_list),  1, get_pos(scanner)); }
#line 2708 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 459 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_Decl_List((yyvsp[-1].exp)->d.exp_decl, NULL); }
#line 2714 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 460 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_Decl_List((yyvsp[-2].exp)->d.exp_decl, (yyvsp[0].decl_list)); }
#line 2720 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 461 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = NULL; }
#line 2726 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 462 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_Decl_List(NULL, (yyvsp[0].decl_list)); }
#line 2732 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 466 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.union_stmt) = new_Union((yyvsp[-2].decl_list)); }
#line 2738 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 470 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_Var_Decl_List((yyvsp[0].var_decl), NULL, get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2744 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 471 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_Var_Decl_List((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2750 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 475 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_Var_Decl((yyvsp[0].sval), NULL, get_pos(scanner)); }
#line 2756 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 476 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_Var_Decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2762 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 477 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_Var_Decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2768 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 482 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2774 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 487 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar((yyvsp[-1].exp), get_pos(scanner)); }
#line 2780 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 490 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_Vec((yyvsp[-1].exp), get_pos(scanner)); }
#line 2786 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 495 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_If_Expression( (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2792 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 499 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2798 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 501 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2804 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 505 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2810 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 507 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2816 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 511 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2822 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 513 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2828 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 517 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2834 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 519 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(scanner)); }
#line 2840 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 523 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2846 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 525 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2852 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 529 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2858 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 531 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_eq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2864 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 533 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_neq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2870 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 537 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2876 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 539 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_lt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2882 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 541 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_gt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2888 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 543 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_le, (yyvsp[0].exp), get_pos(scanner)); }
#line 2894 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 545 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_ge, (yyvsp[0].exp), get_pos(scanner)); }
#line 2900 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 549 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2906 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 551 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_shift_left, (yyvsp[0].exp), get_pos(scanner)); }
#line 2912 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 553 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_shift_right, (yyvsp[0].exp), get_pos(scanner)); }
#line 2918 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 557 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2924 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 559 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_plus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2930 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 561 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_minus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2936 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 565 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2942 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 567 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_times, (yyvsp[0].exp), get_pos(scanner)); }
#line 2948 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 569 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_divide, (yyvsp[0].exp), get_pos(scanner)); }
#line 2954 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 571 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_percent, (yyvsp[0].exp), get_pos(scanner)); }
#line 2960 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 575 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2966 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 577 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_tilda, (yyvsp[0].exp), get_pos(scanner)); }
#line 2972 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 583 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Cast_Expression( (yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2978 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 588 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2984 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 590 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_plusplus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2990 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 592 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_minusminus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2996 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 594 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 3002 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 596 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_typeof, (yyvsp[0].exp), get_pos(scanner)); }
#line 3008 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 598 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_sizeof, (yyvsp[0].exp), get_pos(scanner)); }
#line 3014 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 600 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(scanner)); }
#line 3020 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 602 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary2(op_new, (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3026 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 604 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary3( op_spork, (yyvsp[0].stmt), get_pos(scanner)); }
#line 3032 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 608 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3038 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 609 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3044 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 610 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_tilda; }
#line 3050 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 611 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3056 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 612 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3062 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 613 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3068 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 619 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Exp_Dur( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 3074 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 623 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sval), get_pos(scanner)), NULL, get_pos(scanner)); }
#line 3080 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 624 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sval), get_pos(scanner)), (yyvsp[0].type_list), get_pos(scanner)); }
#line 3086 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 630 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Array( (yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3092 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 632 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Func_Call( (yyvsp[-2].exp), NULL, get_pos(scanner)); }
#line 3098 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 634 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Func_Call( (yyvsp[-3].exp), (yyvsp[-1].exp), get_pos(scanner)); }
#line 3104 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 636 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_member_dot( (yyvsp[-2].exp), (yyvsp[0].sval), get_pos(scanner)); }
#line 3110 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 638 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Postfix_Expression( (yyvsp[-1].exp), op_plusplus, get_pos(scanner)); }
#line 3116 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 640 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Postfix_Expression( (yyvsp[-1].exp), op_minusminus, get_pos(scanner)); }
#line 3122 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 642 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Func_Call( (yyvsp[-3].exp), NULL, get_pos(scanner)); (yyval.exp)->d.exp_func->types = (yyvsp[-2].type_list); }
#line 3128 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 644 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Func_Call( (yyvsp[-4].exp), (yyvsp[-1].exp), get_pos(scanner)); (yyval.exp)->d.exp_func->types = (yyvsp[-3].type_list); }
#line 3134 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 647 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_ID(     (yyvsp[0].sval), get_pos(scanner)); }
#line 3140 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 648 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_int(    (yyvsp[0].ival), get_pos(scanner)); }
#line 3146 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 649 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_float(  (yyvsp[0].fval), get_pos(scanner)); }
#line 3152 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 650 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_string( (yyvsp[0].sval), get_pos(scanner)); }
#line 3158 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 651 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_char(                  (yyvsp[0].sval), get_pos(scanner)); }
#line 3164 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 652 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_array_lit(             (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3170 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 653 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_array_lit(           (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3176 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 654 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_complex(               (yyvsp[0].c_val), get_pos(scanner)); }
#line 3182 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 655 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_polar(                 (yyvsp[0].polar), get_pos(scanner)); }
#line 3188 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 656 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_vec(                   (yyvsp[0].vec), get_pos(scanner)); }
#line 3194 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 657 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Hack_Expression(                (yyvsp[-1].exp), get_pos(scanner)); }
#line 3200 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 658 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3206 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 217:
#line 659 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_nil(       get_pos(scanner)); }
#line 3212 "eval/parser.c" /* yacc.c:1646  */
    break;


#line 3216 "eval/parser.c" /* yacc.c:1646  */
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
#line 661 "eval/gwion.y" /* yacc.c:1906  */

