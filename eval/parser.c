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
    PROTECTED = 284,
    ATSYM = 285,
    FUNCTION = 286,
    DOLLAR = 287,
    TILDA = 288,
    QUESTION = 289,
    COLON = 290,
    EXCLAMATION = 291,
    IF = 292,
    ELSE = 293,
    WHILE = 294,
    DO = 295,
    UNTIL = 296,
    LOOP = 297,
    FOR = 298,
    GOTO = 299,
    SWITCH = 300,
    CASE = 301,
    ENUM = 302,
    RETURN = 303,
    BREAK = 304,
    CONTINUE = 305,
    PLUSPLUS = 306,
    MINUSMINUS = 307,
    NEW = 308,
    SPORK = 309,
    SIZEOF = 310,
    TYPEOF = 311,
    CLASS = 312,
    INTERFACE = 313,
    STATIC = 314,
    PRIVATE = 315,
    PUBLIC = 316,
    EXTENDS = 317,
    IMPLEMENTS = 318,
    DOT = 319,
    COLONCOLON = 320,
    AND = 321,
    EQ = 322,
    GE = 323,
    GT = 324,
    LE = 325,
    LT = 326,
    MINUS = 327,
    PLUS = 328,
    NEQ = 329,
    SHIFT_LEFT = 330,
    SHIFT_RIGHT = 331,
    S_AND = 332,
    S_OR = 333,
    S_XOR = 334,
    OR = 335,
    AST_DTOR = 336,
    OPERATOR = 337,
    FUNC_PTR = 338,
    RSL = 339,
    RSR = 340,
    RSAND = 341,
    RSOR = 342,
    RSXOR = 343,
    RAND = 344,
    ROR = 345,
    REQ = 346,
    RNEQ = 347,
    RGT = 348,
    RGE = 349,
    RLT = 350,
    RLE = 351,
    RINC = 352,
    RDEC = 353,
    RUNINC = 354,
    RUNDEC = 355,
    TEMPLATE = 356,
    NOELSE = 357,
    ARROW_LEFT = 358,
    ARROW_RIGHT = 359,
    LTB = 360,
    GTB = 361,
    VARARG = 362,
    UNION = 363,
    ATPAREN = 364,
    NUM = 365,
    FLOAT = 366,
    ID = 367,
    STRING_LIT = 368,
    CHAR_LIT = 369,
    DOC = 370
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
#define PROTECTED 284
#define ATSYM 285
#define FUNCTION 286
#define DOLLAR 287
#define TILDA 288
#define QUESTION 289
#define COLON 290
#define EXCLAMATION 291
#define IF 292
#define ELSE 293
#define WHILE 294
#define DO 295
#define UNTIL 296
#define LOOP 297
#define FOR 298
#define GOTO 299
#define SWITCH 300
#define CASE 301
#define ENUM 302
#define RETURN 303
#define BREAK 304
#define CONTINUE 305
#define PLUSPLUS 306
#define MINUSMINUS 307
#define NEW 308
#define SPORK 309
#define SIZEOF 310
#define TYPEOF 311
#define CLASS 312
#define INTERFACE 313
#define STATIC 314
#define PRIVATE 315
#define PUBLIC 316
#define EXTENDS 317
#define IMPLEMENTS 318
#define DOT 319
#define COLONCOLON 320
#define AND 321
#define EQ 322
#define GE 323
#define GT 324
#define LE 325
#define LT 326
#define MINUS 327
#define PLUS 328
#define NEQ 329
#define SHIFT_LEFT 330
#define SHIFT_RIGHT 331
#define S_AND 332
#define S_OR 333
#define S_XOR 334
#define OR 335
#define AST_DTOR 336
#define OPERATOR 337
#define FUNC_PTR 338
#define RSL 339
#define RSR 340
#define RSAND 341
#define RSOR 342
#define RSXOR 343
#define RAND 344
#define ROR 345
#define REQ 346
#define RNEQ 347
#define RGT 348
#define RGE 349
#define RLT 350
#define RLE 351
#define RINC 352
#define RDEC 353
#define RUNINC 354
#define RUNDEC 355
#define TEMPLATE 356
#define NOELSE 357
#define ARROW_LEFT 358
#define ARROW_RIGHT 359
#define LTB 360
#define GTB 361
#define VARARG 362
#define UNION 363
#define ATPAREN 364
#define NUM 365
#define FLOAT 366
#define ID 367
#define STRING_LIT 368
#define CHAR_LIT 369
#define DOC 370

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

#line 413 "eval/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int minimal_parse (MyArg* arg);

#endif /* !YY_MINIMAL_EVAL_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 429 "eval/parser.c" /* yacc.c:358  */

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
#define YYFINAL  150
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1592

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  116
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  65
/* YYNRULES -- Number of rules.  */
#define YYNRULES  223
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  415

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   370

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
     115
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   183,   183,   184,   188,   189,   190,   194,   196,   207,
     212,   213,   214,   218,   219,   223,   224,   228,   229,   230,
     238,   239,   243,   244,   248,   249,   253,   254,   258,   259,
     268,   269,   270,   271,   275,   276,   280,   281,   285,   286,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   305,   306,   310,   314,   318,   319,   323,   327,
     329,   331,   333,   335,   337,   339,   344,   346,   351,   352,
     353,   354,   358,   359,   360,   364,   365,   369,   370,   373,
     374,   378,   379,   383,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   420,   421,   426,   427,   431,   432,   433,
     434,   435,   436,   437,   440,   442,   444,   446,   448,   450,
     452,   454,   459,   460,   461,   462,   467,   468,   469,   470,
     474,   478,   479,   483,   484,   485,   489,   494,   498,   501,
     502,   507,   508,   513,   514,   519,   520,   525,   526,   531,
     532,   537,   538,   540,   545,   546,   548,   550,   552,   557,
     558,   560,   565,   566,   568,   573,   574,   576,   578,   583,
     584,   589,   590,   596,   597,   599,   601,   603,   605,   607,
     609,   611,   616,   617,   618,   619,   620,   621,   626,   627,
     631,   632,   636,   637,   639,   641,   643,   645,   647,   649,
     651,   655,   656,   657,   658,   659,   660,   661,   662,   663,
     664,   665,   666,   667
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
  "UNTRIG", "PERCENTPAREN", "SHARPPAREN", "PROTECTED", "ATSYM", "FUNCTION",
  "DOLLAR", "TILDA", "QUESTION", "COLON", "EXCLAMATION", "IF", "ELSE",
  "WHILE", "DO", "UNTIL", "LOOP", "FOR", "GOTO", "SWITCH", "CASE", "ENUM",
  "RETURN", "BREAK", "CONTINUE", "PLUSPLUS", "MINUSMINUS", "NEW", "SPORK",
  "SIZEOF", "TYPEOF", "CLASS", "INTERFACE", "STATIC", "PRIVATE", "PUBLIC",
  "EXTENDS", "IMPLEMENTS", "DOT", "COLONCOLON", "AND", "EQ", "GE", "GT",
  "LE", "LT", "MINUS", "PLUS", "NEQ", "SHIFT_LEFT", "SHIFT_RIGHT", "S_AND",
  "S_OR", "S_XOR", "OR", "AST_DTOR", "OPERATOR", "FUNC_PTR", "RSL", "RSR",
  "RSAND", "RSOR", "RSXOR", "RAND", "ROR", "REQ", "RNEQ", "RGT", "RGE",
  "RLT", "RLE", "RINC", "RDEC", "RUNINC", "RUNDEC", "TEMPLATE", "NOELSE",
  "ARROW_LEFT", "ARROW_RIGHT", "LTB", "GTB", "VARARG", "UNION", "ATPAREN",
  "NUM", "FLOAT", "ID", "STRING_LIT", "CHAR_LIT", "DOC", "$accept", "ast",
  "section", "class_def", "class_ext", "class_decl", "class_body",
  "class_body2", "class_section", "id_list", "id_dot", "stmt_list",
  "static_decl", "function_decl", "func_ptr", "type_decl2", "arg_list",
  "code_segment", "stmt", "enum_stmt", "label_stmt", "goto_stmt",
  "case_stmt", "switch_stmt", "loop_stmt", "selection_stmt", "jump_stmt",
  "exp_stmt", "exp", "arrow_operator", "binary_exp", "arrow_expression",
  "template", "op", "array_exp", "array_empty", "decl_exp", "func_def",
  "type_decl", "decl_list", "union", "var_decl_list", "var_decl",
  "complex_exp", "polar_exp", "vec_exp", "conditional_expression",
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
     365,   366,   367,   368,   369,   370
};
# endif

#define YYPACT_NINF -227

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-227)))

#define YYTABLE_NINF -133

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     682,  -227,  -227,  1412,  1082,  1148,   795,  1412,  1412,   -54,
    -227,  -227,  -227,    38,    53,   884,    62,    75,    84,   -55,
      87,    42,    88,  1016,   108,   111,   215,   215,   -54,   104,
     215,   215,    47,   -54,  -227,    28,  -227,  -227,   134,   -54,
     -54,    43,  -227,   144,  1412,  -227,  -227,   -10,  -227,  -227,
    -227,   171,   366,  -227,   125,  -227,   128,  -227,  -227,   884,
    -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,   185,
    1475,   -26,  -227,   176,  -227,  -227,    80,  -227,  -227,  -227,
    -227,  -227,    30,   127,   117,   118,   119,   -34,    94,    68,
      49,    52,   165,   167,  -227,   215,   135,    34,  -227,    46,
     -54,    -3,   190,  -227,   189,  -227,   188,  -227,   192,   194,
     197,   175,    80,  1412,  1412,    32,  1412,  1412,   950,   208,
    1412,  -227,    37,   177,   101,  -227,   211,  -227,  -227,  -227,
    -227,   201,   200,  -227,  -227,   -54,   -54,   -54,    80,    80,
     155,   151,   209,   109,   214,   101,  1478,   217,  -227,  -227,
    -227,  -227,   101,  -227,   -54,  -227,  -227,  -227,  1412,  -227,
    -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,
    -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,
    -227,  -227,  -227,  -227,  -227,  -227,  -227,  1412,  -227,  -227,
    1412,   213,   218,  -227,   226,  1412,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   -54,  -227,    42,  1214,  1412,
    -227,  -227,   121,   122,   223,  -227,  -227,  -227,   201,  -227,
    -227,  -227,  -227,   224,   225,   227,   228,   231,   232,   950,
    -227,   233,  -227,  -227,   236,   219,  -227,  -227,  -227,    80,
      80,   235,  -227,  -227,    28,   220,   200,   237,   141,   148,
      11,   252,   239,  -227,    31,   145,   244,  -227,   -26,  -227,
    -227,  -227,   176,    80,   229,   127,   117,   118,   119,   -34,
      94,    94,    68,    68,    68,    68,    49,    49,    52,    52,
     165,   165,   165,   167,  -227,    34,  -227,   247,  -227,   256,
     156,  1280,  -227,   884,   884,  1412,  1412,   884,   884,  1346,
     200,   101,    12,  -227,  -227,   153,  -227,  -227,  -227,     8,
     250,     1,  1478,  1478,   269,   479,    28,   257,   263,   261,
     176,  -227,   215,  -227,   122,  -227,  -227,   264,   240,  -227,
     266,   267,  -227,  -227,   884,   268,  -227,  -227,  -227,   279,
     271,   200,   272,    80,   280,   128,  -227,  -227,  -227,  -227,
     274,  -227,   592,  -227,  -227,  -227,   479,    13,  -227,  -227,
    -227,   884,   292,   293,  -227,   884,  -227,   285,  -227,   200,
     294,    21,   -54,  -227,  -227,   281,    39,   288,  -227,  -227,
    -227,  -227,    23,  -227,   -54,  -227,   289,   191,  -227,  -227,
    -227,   200,  -227,   296,  -227,  -227,   295,  -227,  -227,    24,
     200,   298,  -227,   200,  -227
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      12,    73,   196,     0,     0,     0,     0,     0,     0,     0,
      28,   194,   195,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    11,    10,     0,   193,   192,     0,     0,
       0,     0,    29,     0,     0,   212,   213,   211,   214,   215,
      74,     0,     2,     6,     0,     4,    27,    50,    43,    24,
      48,    44,    45,    47,    46,    41,    42,    49,    40,     0,
      75,    79,   216,   217,    81,     5,     0,    51,   218,   219,
     220,   117,   149,   151,   153,   155,   157,   159,   161,   164,
     169,   172,   175,   179,   181,     0,   183,   198,   202,     0,
       0,   211,     0,   223,     0,   115,     0,    38,     0,     0,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   211,     0,   202,     0,    68,     0,    70,    71,   184,
     185,   189,   197,   188,   187,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,   133,    54,
       1,     3,     0,    26,     0,    25,    72,    84,     0,   112,
      87,    88,    89,    90,    91,    86,    85,    92,    93,   111,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,     0,    77,    78,
       0,     0,   143,   118,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   186,     0,     0,     0,
     207,   208,     0,     0,     0,   203,   221,   222,   113,    39,
     147,   146,   122,     0,     0,     0,     0,     0,     0,     0,
      55,     0,    57,    56,    20,     0,    69,   190,   191,     0,
       0,     0,   119,   120,     0,   134,     0,     0,     0,     0,
     211,     0,     0,   148,     0,     0,    34,    76,    80,    82,
     116,   144,   145,     0,     0,   152,   154,   156,   158,   160,
     162,   163,   168,   166,   167,   165,   170,   171,   174,   173,
     177,   176,   178,   180,   182,   199,   204,     0,   206,   200,
       0,     0,   114,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   123,   121,     0,    23,   135,   127,     0,
       0,     0,   138,   136,     0,    12,     0,     0,     0,     0,
      35,   142,     0,   205,     0,    83,   209,     0,    66,    59,
       0,     0,    63,    65,     0,     0,    58,    21,    52,     0,
       0,     0,     0,     0,     0,    27,   139,   137,   140,    19,
       0,    13,    12,    17,    18,     9,    12,     0,   150,   201,
     210,     0,     0,     0,    61,     0,    53,     0,   128,     0,
      36,     0,     0,     7,    16,     0,     0,     0,    67,    60,
      64,    62,     0,   129,     0,    30,     0,     0,     8,   125,
     124,     0,    31,     0,    37,    32,     0,   126,    33,     0,
       0,     0,   130,     0,   131
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -227,   254,  -227,    16,  -227,  -227,   -53,   -50,  -227,  -134,
    -226,   132,   -41,    -6,  -227,   -66,  -220,  -127,   -14,  -227,
    -227,  -227,  -227,  -227,  -227,  -227,  -227,  -105,    -1,  -227,
    -227,   130,  -227,  -227,   -87,  -163,  -137,    45,    -9,  -153,
    -227,  -100,   -35,  -227,  -227,  -227,   -12,  -227,   123,   124,
     133,   136,   137,   -23,  -123,   -32,   -29,   -45,   120,    36,
    -227,  -227,   -11,   -13,   301
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    51,    52,   359,   327,    54,   360,   361,   362,   245,
     141,   363,   154,    56,    57,   265,   352,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,   190,
      70,    71,   224,   187,    72,    73,    74,   364,    76,   262,
      77,   193,   194,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,   300,    97,    98
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     112,   115,   102,   104,   106,   248,   109,   110,   122,   261,
     225,   259,   232,   239,   322,   348,    53,    35,   264,   131,
     148,   351,   126,   138,   139,   149,   386,   148,   316,   272,
     143,   144,    10,   201,   395,   225,   402,   410,   252,   253,
     202,   148,   399,   147,   247,    75,   218,   325,   219,   218,
     113,   219,     3,   269,     4,     6,     5,   119,   111,   211,
     212,   213,   129,   130,   195,   114,   133,   134,    53,     7,
       8,   235,   242,   236,   116,   135,   135,   188,   189,    35,
     282,   283,   284,   285,    35,   220,   221,   117,   220,   221,
     138,   139,    35,   326,    35,    35,   118,    75,   222,   120,
     365,   222,  -132,   330,   124,   271,   136,   136,    42,  -132,
     196,   127,   233,   234,   128,   237,   238,    35,    35,   241,
     111,   209,   210,  -132,   349,   111,   249,   250,   251,   318,
     137,   216,    55,   111,   309,   111,   111,   132,   108,   223,
     140,   302,   223,   207,   208,   266,   142,   387,   145,   313,
     314,    44,    45,    46,   121,    48,    49,   267,   111,   111,
     146,   396,   203,   204,   205,   206,   290,   291,   292,   356,
     357,   150,   403,   331,   404,   286,   287,   347,   280,   281,
     288,   289,   152,   346,    55,   261,   261,   153,   156,   411,
     191,   155,   192,   197,   274,   198,   200,   199,   214,   215,
     217,   226,   227,   228,   295,   148,   294,   230,   225,   229,
     231,   240,   243,   244,   246,   219,     6,   297,   106,   254,
     255,   257,   256,     2,   378,     3,   258,     4,   270,     5,
     263,   273,     5,   298,   299,   301,   312,   303,   304,   305,
     306,   311,     7,     8,   307,   308,   310,   315,    11,   319,
     317,    12,   393,   320,   321,   323,   324,   328,   329,   400,
     333,   334,   335,   354,   332,   350,    26,    27,    28,    29,
      30,    31,   358,   366,   407,   367,   105,   370,   371,   372,
     373,   375,   376,   412,   377,   379,   414,    36,    37,   338,
     339,   383,   381,   342,   343,   389,   390,   392,   398,   394,
     337,   401,   405,   406,   340,   341,   151,   409,   345,   408,
     353,   413,   384,   385,   382,   355,   397,   268,   380,   275,
     368,   276,   123,   369,    44,    45,    46,   121,    48,    49,
     374,   277,     0,     0,   293,   278,     0,   279,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   388,   353,     0,
       0,   391,     0,     0,     0,     0,     0,     0,     0,     1,
       0,     0,   353,   266,     2,     0,     3,     0,     4,     0,
       5,     0,     6,   353,     0,   353,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     9,     0,    10,     0,    11,
     353,     0,    12,    13,     0,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,   -12,     0,    32,    33,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    35,    36,    37,
       0,     0,     0,     0,     0,     0,     0,    38,    39,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,    42,    43,    44,    45,    46,    47,    48,
      49,    50,     1,     0,     0,     0,     0,     2,     0,     3,
       0,     4,     0,     5,     0,     6,   -14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     9,     0,
      10,     0,    11,     0,     0,    12,    13,     0,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     0,     0,    32,    33,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      35,    36,    37,     0,     0,     0,     0,     0,     0,     0,
      38,    39,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      41,     0,     0,     0,     0,     0,    42,    43,    44,    45,
      46,    47,    48,    49,    50,     1,     0,     0,     0,     0,
       2,     0,     3,     0,     4,     0,     5,     0,     6,   -15,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     9,     0,    10,     0,    11,     0,     0,    12,    13,
       0,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     0,
       0,    32,    33,    34,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    35,    36,    37,     0,     0,     0,     0,
       0,     0,     0,    38,    39,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     1,     0,     0,     0,     0,
       2,     0,     3,    41,     4,     0,     5,     0,     6,    42,
      43,    44,    45,    46,    47,    48,    49,    50,     0,     7,
       8,     9,     0,    10,     0,    11,     0,     0,    12,    13,
       0,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     0,
       0,    32,    33,    34,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    35,    36,    37,     0,     0,     0,     0,
       0,     0,     0,    38,    39,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,    42,
      43,    44,    45,    46,    47,    48,    49,    50,     1,     0,
       0,     0,     0,     2,     0,     3,     0,     4,     0,     5,
       0,     6,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     9,     0,     0,     0,    11,     0,
       0,    12,    13,     0,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,     0,     0,    32,   100,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    35,    36,    37,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     0,     0,
       0,     0,     2,     0,     3,     0,     4,     0,     5,     0,
       6,     0,     0,    43,    44,    45,    46,    47,    48,    49,
      50,     7,     8,     9,     0,     0,     0,    11,     0,     0,
      12,    13,     0,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,     0,    32,   100,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     0,    35,    36,    37,     2,     0,
       3,     0,     4,     0,     5,     0,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     9,
       0,     0,     0,    11,     0,     0,    12,     0,     0,     0,
       0,     0,    43,    44,    45,    46,    47,    48,    49,    50,
       0,    26,    27,    28,    29,    30,    31,     0,     0,    99,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   125,
       0,    35,    36,    37,     2,     0,     3,     0,     4,     0,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     9,     0,     0,     0,    11,
       0,     0,    12,     0,     0,     0,     0,     0,     0,    44,
      45,    46,   101,    48,    49,    50,     0,    26,    27,    28,
      29,    30,    31,     0,     0,    99,   100,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    35,    36,    37,
       2,     0,     3,     0,     4,   103,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     9,     0,     0,     0,    11,     0,     0,    12,     0,
       0,     0,     0,     0,     0,    44,    45,    46,   101,    48,
      49,     0,     0,    26,    27,    28,    29,    30,    31,     0,
       0,    99,   100,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    35,    36,    37,     2,     0,     3,     0,
       4,     0,     5,   105,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     9,     0,     0,
       0,    11,     0,     0,    12,     0,     0,     0,     0,     0,
       0,    44,    45,    46,   101,    48,    49,     0,     0,    26,
      27,    28,    29,    30,    31,     0,     0,    99,   100,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    35,
      36,    37,     2,     0,     3,     0,     4,   296,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     9,     0,     0,     0,    11,     0,     0,
      12,     0,     0,     0,     0,     0,     0,    44,    45,    46,
     101,    48,    49,     0,     0,    26,    27,    28,    29,    30,
      31,     0,     0,    99,   100,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    35,    36,    37,     2,     0,
       3,     0,     4,   336,     5,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     9,
       0,     0,     0,    11,     0,     0,    12,     0,     0,     0,
       0,     0,     0,    44,    45,    46,   101,    48,    49,     0,
       0,    26,    27,    28,    29,    30,    31,     0,     0,    99,
     100,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    35,    36,    37,     2,     0,     3,     0,     4,   344,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     9,     0,     0,     0,    11,
       0,     0,    12,     0,     0,     0,     0,     0,     0,    44,
      45,    46,   101,    48,    49,     0,     0,    26,    27,    28,
      29,    30,    31,     0,     0,    99,   100,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    35,    36,    37,
       2,     0,     3,     0,     4,     0,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     9,     0,     0,     0,    11,     0,     0,    12,     0,
       0,     0,     0,     0,     0,    44,    45,    46,   101,    48,
      49,     0,     0,    26,    27,    28,    29,    30,    31,     0,
       0,    99,   100,     0,     0,     0,     0,     0,     0,   157,
     158,   159,     0,    35,    36,    37,     2,     0,     3,     0,
       4,     0,     5,   160,   161,   162,   163,   164,   165,   166,
     167,   168,     0,     0,     0,     7,     8,     9,     0,     0,
       0,    11,     0,     0,    12,     0,     0,     0,     0,     0,
       0,    44,    45,    46,   101,    48,    49,     0,     0,    26,
      27,    28,    29,    30,    31,     0,     0,    99,   100,     0,
       0,     0,   169,     0,     0,     0,     0,     0,     0,    35,
      36,    37,     0,     0,     0,     0,     0,     0,     0,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    44,    45,    46,
     260,    48,    49
};

static const yytype_int16 yycheck[] =
{
       9,    15,     3,     4,     5,   132,     7,     8,    21,   146,
      97,   145,   112,   118,     3,     3,     0,    71,   152,    28,
      30,    13,    23,    32,    33,    35,    13,    30,   254,   192,
      39,    40,    31,    67,    13,   122,    13,    13,   138,   139,
      74,    30,     3,    44,   131,     0,    12,    16,    14,    12,
      12,    14,    10,   190,    12,    16,    14,   112,   112,     7,
       8,     9,    26,    27,    34,    12,    30,    31,    52,    27,
      28,    39,    35,    41,    12,    29,    29,   103,   104,    71,
     203,   204,   205,   206,    71,    51,    52,    12,    51,    52,
      99,   100,    71,    62,    71,    71,    12,    52,    64,    12,
     326,    64,   112,   266,    16,   192,    60,    60,   107,   112,
      80,     3,   113,   114,     3,   116,   117,    71,    71,   120,
     112,    72,    73,   112,   112,   112,   135,   136,   137,   256,
      83,    95,     0,   112,   239,   112,   112,    33,     6,   105,
     112,   228,   105,    75,    76,   154,    12,   367,   105,   249,
     250,   109,   110,   111,   112,   113,   114,   158,   112,   112,
      16,   381,    68,    69,    70,    71,   211,   212,   213,   322,
     323,     0,   392,   273,   394,   207,   208,   311,   201,   202,
     209,   210,    57,   310,    52,   322,   323,    59,     3,   409,
      14,    59,   112,    66,   195,    78,    77,    79,    33,    32,
      65,    11,    13,    15,   217,    30,   215,    13,   295,    17,
      13,     3,    35,   112,     3,    14,    16,   218,   219,    64,
      69,   112,    13,     8,   351,    10,    12,    12,    15,    14,
      13,     5,    14,   112,   112,    12,    17,    13,    13,    12,
      12,     5,    27,    28,    13,    13,    13,    12,    33,    12,
      30,    36,   379,   112,   106,     3,    17,   112,    14,   386,
      13,     5,   106,    13,    35,   112,    51,    52,    53,    54,
      55,    56,     3,    16,   401,    12,    15,    13,    38,    13,
      13,    13,     3,   410,    13,    13,   413,    72,    73,   303,
     304,    17,    12,   307,   308,     3,     3,    12,    17,     5,
     301,    13,    13,   112,   305,   306,    52,    12,   309,    13,
     319,    13,   362,   366,   355,   321,   382,   187,   353,   196,
     332,   197,    21,   334,   109,   110,   111,   112,   113,   114,
     344,   198,    -1,    -1,   214,   199,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   371,   367,    -1,
      -1,   375,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,   381,   382,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    16,   392,    -1,   394,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    29,    -1,    31,    -1,    33,
     409,    -1,    36,    37,    -1,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,   107,   108,   109,   110,   111,   112,   113,
     114,   115,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,
      31,    -1,    33,    -1,    -1,    36,    37,    -1,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    -1,    -1,    59,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,
     111,   112,   113,   114,   115,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    -1,    14,    -1,    16,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    -1,    31,    -1,    33,    -1,    -1,    36,    37,
      -1,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      -1,    59,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,   101,    12,    -1,    14,    -1,    16,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,    27,
      28,    29,    -1,    31,    -1,    33,    -1,    -1,    36,    37,
      -1,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      -1,    59,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,   107,
     108,   109,   110,   111,   112,   113,   114,   115,     3,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    12,    -1,    14,
      -1,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    -1,    -1,    -1,    33,    -1,
      -1,    36,    37,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      16,    -1,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,    27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,
      36,    37,    -1,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    71,    72,    73,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      -1,    -1,    -1,    33,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,    51,    52,    53,    54,    55,    56,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    71,    72,    73,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,    33,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,   115,    -1,    51,    52,    53,
      54,    55,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
     114,    -1,    -1,    51,    52,    53,    54,    55,    56,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,     8,    -1,    10,    -1,
      12,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,
      -1,    33,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,    -1,    -1,    51,
      52,    53,    54,    55,    56,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,    -1,    -1,    51,    52,    53,    54,    55,
      56,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,     8,    -1,
      10,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      -1,    -1,    -1,    33,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,   114,    -1,
      -1,    51,    52,    53,    54,    55,    56,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,     8,    -1,    10,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,    33,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,    -1,    -1,    51,    52,    53,
      54,    55,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
       8,    -1,    10,    -1,    12,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
     114,    -1,    -1,    51,    52,    53,    54,    55,    56,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,     4,
       5,     6,    -1,    71,    72,    73,     8,    -1,    10,    -1,
      12,    -1,    14,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,
      -1,    33,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,    -1,    -1,    51,
      52,    53,    54,    55,    56,    -1,    -1,    59,    60,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    29,
      31,    33,    36,    37,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    59,    60,    61,    71,    72,    73,    81,    82,
      83,   101,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   117,   118,   119,   121,   127,   129,   130,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     146,   147,   150,   151,   152,   153,   154,   156,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   179,   180,    59,
      60,   112,   144,    13,   144,    15,   144,    17,   127,   144,
     144,   112,   154,    12,    12,   134,    12,    12,    12,   112,
      12,   112,   179,   180,    16,     3,   144,     3,     3,   175,
     175,   154,    33,   175,   175,    29,    60,    83,   154,   154,
     112,   126,    12,   154,   154,   105,    16,   144,    30,    35,
       0,   117,    57,    59,   128,   127,     3,     4,     5,     6,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    67,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   149,   103,   104,
     145,    14,   112,   157,   158,    34,    80,    66,    78,    79,
      77,    67,    74,    68,    69,    70,    71,    75,    76,    72,
      73,     7,     8,     9,    33,    32,   175,    65,    12,    14,
      51,    52,    64,   105,   148,   150,    11,    13,    15,    17,
      13,    13,   157,   144,   144,    39,    41,   144,   144,   143,
       3,   144,    35,    35,   112,   125,     3,   150,   133,   154,
     154,   154,   157,   157,    64,    69,    13,   112,    12,   125,
     112,   152,   155,    13,   125,   131,   154,   144,   147,   152,
      15,   150,   151,     5,   144,   164,   165,   166,   167,   168,
     169,   169,   170,   170,   170,   170,   171,   171,   172,   172,
     173,   173,   173,   174,   154,   179,    13,   144,   112,   112,
     178,    12,   150,    13,    13,    12,    12,    13,    13,   143,
      13,     5,    17,   157,   157,    12,   126,    30,   133,    12,
     112,   106,     3,     3,    17,    16,    62,   120,   112,    14,
     151,   157,    35,    13,     5,   106,    13,   144,   134,   134,
     144,   144,   134,   134,    13,   144,   133,   125,     3,   112,
     112,    13,   132,   154,    13,   129,   155,   155,     3,   119,
     122,   123,   124,   127,   153,   126,    16,    12,   162,   178,
      13,    38,    13,    13,   134,    13,     3,    13,   133,    13,
     158,    12,   128,    17,   123,   122,    13,   132,   134,     3,
       3,   134,    12,   133,     5,    13,   132,   131,    17,     3,
     133,    13,    13,   132,   132,    13,   112,   133,    13,    12,
      13,   132,   133,    13,   133
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   116,   117,   117,   118,   118,   118,   119,   119,   120,
     121,   121,   121,   122,   122,   123,   123,   124,   124,   124,
     125,   125,   126,   126,   127,   127,   128,   128,   129,   129,
     130,   130,   130,   130,   131,   131,   132,   132,   133,   133,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   135,   135,   136,   137,   138,   138,   139,   140,
     140,   140,   140,   140,   140,   140,   141,   141,   142,   142,
     142,   142,   143,   143,   143,   144,   144,   145,   145,   146,
     146,   147,   147,   148,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   150,   150,   151,   151,   152,   152,   152,
     152,   152,   152,   152,   153,   153,   153,   153,   153,   153,
     153,   153,   154,   154,   154,   154,   155,   155,   155,   155,
     156,   157,   157,   158,   158,   158,   159,   160,   161,   162,
     162,   163,   163,   164,   164,   165,   165,   166,   166,   167,
     167,   168,   168,   168,   169,   169,   169,   169,   169,   170,
     170,   170,   171,   171,   171,   172,   172,   172,   172,   173,
     173,   174,   174,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   176,   176,   176,   176,   176,   176,   177,   177,
     178,   178,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     6,     7,     2,
       1,     1,     0,     1,     0,     1,     2,     1,     1,     1,
       1,     3,     1,     3,     1,     2,     1,     0,     1,     1,
       7,     8,     8,     9,     1,     2,     2,     4,     2,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     5,     6,     2,     3,     3,     3,     5,     5,
       7,     6,     7,     5,     7,     5,     5,     7,     2,     3,
       2,     2,     2,     1,     1,     1,     3,     1,     1,     1,
       3,     1,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     2,     3,     1,     2,     3,
       3,     4,     3,     4,     7,     7,     8,     4,     6,     7,
      11,    12,     1,     2,     3,     4,     2,     3,     2,     3,
       5,     1,     3,     1,     2,     2,     3,     3,     3,     1,
       5,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       3,     1,     3,     1,     2,     2,     2,     2,     2,     2,
       3,     3,     1,     1,     1,     1,     1,     2,     1,     3,
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
          case 131: /* type_decl2  */
#line 179 "eval/gwion.y" /* yacc.c:1257  */
      { printf("!!!!\n"); free_Type_Decl(((*yyvaluep).type_decl)); }
#line 1814 "eval/parser.c" /* yacc.c:1257  */
        break;

    case 154: /* type_decl  */
#line 179 "eval/gwion.y" /* yacc.c:1257  */
      { printf("!!!!\n"); free_Type_Decl(((*yyvaluep).type_decl)); }
#line 1820 "eval/parser.c" /* yacc.c:1257  */
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
#line 183 "eval/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_Ast((yyvsp[0].section), NULL, get_pos(scanner));  }
#line 2088 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 184 "eval/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_Ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(scanner)); }
#line 2094 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 188 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Stmt_List((yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2100 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 189 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Func_Def ((yyvsp[0].func_def), get_pos(scanner)); }
#line 2106 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 190 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Class_Def((yyvsp[0].class_def), get_pos(scanner)); }
#line 2112 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 195 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-5].ival), (yyvsp[-3].id_list), NULL, (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->doc = get_doc(scanner); }
#line 2118 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 197 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].class_ext), (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->doc = get_doc(scanner);}
#line 2124 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 207 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = new_class_ext( (yyvsp[0].id_list), NULL, get_pos(scanner)); }
#line 2130 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 212 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_public; }
#line 2136 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 213 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_private; }
#line 2142 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 214 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_private; }
#line 2148 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 218 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2154 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 219 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2160 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 223 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body( (yyvsp[0].section), get_pos(scanner)); }
#line 2166 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 224 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body( (yyvsp[-1].section), (yyvsp[0].class_body), get_pos(scanner)); }
#line 2172 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 228 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Stmt_List( (yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2178 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 229 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Func_Def( (yyvsp[0].func_def), get_pos(scanner)); }
#line 2184 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 230 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_Class_Def( (yyvsp[0].class_def), get_pos(scanner)); }
#line 2190 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 238 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2196 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 239 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2202 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 243 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2208 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 244 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2214 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 248 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_Stmt_List((yyvsp[0].stmt), NULL, get_pos(scanner));}
#line 2220 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 249 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_Stmt_List((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(scanner));}
#line 2226 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 253 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_static;   }
#line 2232 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 254 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_instance; }
#line 2238 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 258 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_func; }
#line 2244 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 259 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_key_variadic; }
#line 2250 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 268 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Func_Ptr_Stmt(0, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2256 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 269 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Func_Ptr_Stmt(ae_key_static, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2262 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 270 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Func_Ptr_Stmt(0, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2268 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 271 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Func_Ptr_Stmt(ae_key_static, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2274 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 275 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2280 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 276 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2286 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 280 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_Arg_List((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
#line 2292 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 281 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_Arg_List((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(scanner)); /* $$->doc = get_arg_doc(scanner); */ }
#line 2298 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 285 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_code( NULL, get_pos(scanner)); }
#line 2304 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 286 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_code( (yyvsp[-1].stmt_list), get_pos(scanner)); }
#line 2310 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 301 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_Union((yyvsp[0].union_stmt), 0); }
#line 2316 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 305 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_enum((yyvsp[-2].id_list), NULL, get_pos(scanner)); }
#line 2322 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 306 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_enum((yyvsp[-3].id_list), (yyvsp[-1].sval), get_pos(scanner)); }
#line 2328 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 310 "eval/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_from_gotolabel((yyvsp[-1].sval), 1, get_pos(scanner)); }
#line 2334 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 314 "eval/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_from_gotolabel((yyvsp[-1].sval), 0, get_pos(scanner)); }
#line 2340 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 318 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2346 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 319 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2352 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 323 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner));}
#line 2358 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 328 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_while( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2364 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 330 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_while( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2370 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 332 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2376 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 334 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_for( (yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2382 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 336 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_until( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2388 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 338 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_until( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2394 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 340 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_loop( (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2400 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 345 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(scanner)); }
#line 2406 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 347 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_if( (yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2412 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 351 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_return( NULL, get_pos(scanner)); }
#line 2418 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 352 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_return( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2424 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 353 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_break(get_pos(scanner)); }
#line 2430 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 354 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_from_continue(get_pos(scanner)); }
#line 2436 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 358 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Stmt_from_Expression((yyvsp[-1].exp),   get_pos(scanner)); }
#line 2442 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 359 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Stmt_from_Expression(NULL, get_pos(scanner)); }
#line 2448 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 360 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_Stmt_from_Expression(NULL, get_pos(scanner)); append_doc(scanner, (yyvsp[0].sval)); }
#line 2454 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 364 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2460 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 365 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_Expression((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2466 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 369 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_op_arrow_left; }
#line 2472 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 370 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_op_arrow_right; }
#line 2478 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 373 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2484 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 374 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2490 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 378 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2496 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 380 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2502 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 383 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2508 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 388 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2514 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 389 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2520 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 390 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2526 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 391 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2532 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 392 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2538 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 393 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2544 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 394 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2550 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 395 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2556 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 396 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2562 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 397 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2568 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 398 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2574 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 399 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2580 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 400 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2586 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 401 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2592 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 402 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2598 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 403 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2604 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 404 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2610 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 405 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2616 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 406 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2622 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 407 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2628 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 408 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2634 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 409 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2640 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 410 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2646 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 411 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rinc; }
#line 2652 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 412 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rdec; }
#line 2658 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 413 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_runinc; }
#line 2664 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 414 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rundec; }
#line 2670 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 415 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2676 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 416 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2682 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 420 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( (yyvsp[-1].exp), get_pos(scanner) ); }
#line 2688 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 422 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[0].array_sub), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2694 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 426 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( NULL, get_pos(scanner)); }
#line 2700 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 427 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-2].array_sub), NULL, get_pos(scanner)); }
#line 2706 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 432 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_Decl_Expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); }
#line 2712 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 433 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_Decl_Expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); }
#line 2718 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 434 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_Decl_Expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 2, get_pos(scanner)); }
#line 2724 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 435 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_Decl_Expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 3, get_pos(scanner)); }
#line 2730 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 436 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_Decl_Expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 4, get_pos(scanner)); }
#line 2736 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 437 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_Decl_Expression((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 5, get_pos(scanner)); }
#line 2742 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 441 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2748 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 443 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, NULL, get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2754 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 445 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-7].ival), (yyvsp[-6].ival), (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2760 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 447 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def(ae_key_func, ae_key_instance, new_Type_Decl(new_id_list("void", get_pos(scanner)), 0, get_pos(scanner)), "dtor", NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_dtor; (yyval.func_def)->type_decl->doc = get_doc(scanner);}
#line 2766 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 449 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def(ae_key_func, ae_key_static, (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_op; (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2772 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 451 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def(ae_key_func, ae_key_static, (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->spec = ae_func_spec_op; (yyval.func_def)->type_decl->doc = get_doc(scanner); }
#line 2778 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 453 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-6].ival), (yyvsp[-5].ival), (yyvsp[-4].type_decl), (yyvsp[-3].sval), NULL, (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); (yyval.func_def)->types = (yyvsp[-8].id_list); }
#line 2784 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 455 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_Func_Def((yyvsp[-7].ival), (yyvsp[-6].ival), (yyvsp[-5].type_decl), (yyvsp[-4].sval), (yyvsp[-2].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->type_decl->doc = get_doc(scanner); (yyval.func_def)->types = (yyvsp[-9].id_list); }
#line 2790 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 459 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_Type_Decl(new_id_list((yyvsp[0].sval), get_pos(scanner)), 0, get_pos(scanner)); }
#line 2796 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 460 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_Type_Decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), 1, get_pos(scanner)); }
#line 2802 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 461 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_Type_Decl((yyvsp[-1].id_list),  0, get_pos(scanner)); }
#line 2808 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 462 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_Type_Decl((yyvsp[-2].id_list),  1, get_pos(scanner)); }
#line 2814 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 467 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_Decl_List((yyvsp[-1].exp)->d.exp_decl, NULL); }
#line 2820 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 468 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_Decl_List((yyvsp[-2].exp)->d.exp_decl, (yyvsp[0].decl_list)); }
#line 2826 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 469 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = NULL; }
#line 2832 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 470 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_Decl_List(NULL, (yyvsp[0].decl_list)); }
#line 2838 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 474 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.union_stmt) = new_Union((yyvsp[-2].decl_list)); }
#line 2844 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 478 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_Var_Decl_List((yyvsp[0].var_decl), NULL, get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2850 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 479 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_Var_Decl_List((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(scanner)); (yyval.var_decl_list)->doc = get_doc(scanner); }
#line 2856 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 483 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_Var_Decl((yyvsp[0].sval), NULL, get_pos(scanner)); }
#line 2862 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 484 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_Var_Decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2868 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 485 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_Var_Decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2874 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 490 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2880 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 495 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar((yyvsp[-1].exp), get_pos(scanner)); }
#line 2886 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 498 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_Vec((yyvsp[-1].exp), get_pos(scanner)); }
#line 2892 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 503 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_If_Expression( (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2898 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 507 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2904 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 509 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2910 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 513 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2916 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 515 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2922 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 519 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2928 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 521 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2934 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 525 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2940 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 527 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(scanner)); }
#line 2946 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 531 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2952 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 533 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2958 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 537 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2964 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 539 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_eq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2970 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 541 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_neq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2976 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 545 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2982 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 547 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_lt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2988 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 549 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_gt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2994 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 551 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_le, (yyvsp[0].exp), get_pos(scanner)); }
#line 3000 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 553 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_ge, (yyvsp[0].exp), get_pos(scanner)); }
#line 3006 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 557 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3012 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 559 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_shift_left, (yyvsp[0].exp), get_pos(scanner)); }
#line 3018 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 561 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_shift_right, (yyvsp[0].exp), get_pos(scanner)); }
#line 3024 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 565 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3030 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 567 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_plus, (yyvsp[0].exp), get_pos(scanner)); }
#line 3036 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 569 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_minus, (yyvsp[0].exp), get_pos(scanner)); }
#line 3042 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 573 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3048 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 575 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_times, (yyvsp[0].exp), get_pos(scanner)); }
#line 3054 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 577 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_divide, (yyvsp[0].exp), get_pos(scanner)); }
#line 3060 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 579 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_percent, (yyvsp[0].exp), get_pos(scanner)); }
#line 3066 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 583 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3072 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 585 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Binary_Expression( (yyvsp[-2].exp), op_tilda, (yyvsp[0].exp), get_pos(scanner)); }
#line 3078 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 591 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Cast_Expression( (yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(scanner)); }
#line 3084 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 596 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3090 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 598 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_plusplus, (yyvsp[0].exp), get_pos(scanner)); }
#line 3096 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 600 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_minusminus, (yyvsp[0].exp), get_pos(scanner)); }
#line 3102 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 602 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 3108 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 604 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_typeof, (yyvsp[0].exp), get_pos(scanner)); }
#line 3114 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 606 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary( op_sizeof, (yyvsp[0].exp), get_pos(scanner)); }
#line 3120 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 608 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(scanner)); }
#line 3126 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 610 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary2(op_new, (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3132 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 612 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_unary3( op_spork, (yyvsp[0].stmt), get_pos(scanner)); }
#line 3138 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 616 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3144 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 617 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3150 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 618 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_tilda; }
#line 3156 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 619 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3162 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 620 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3168 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 621 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3174 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 627 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Exp_Dur( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 3180 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 631 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sval), get_pos(scanner)), NULL, get_pos(scanner)); }
#line 3186 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 632 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sval), get_pos(scanner)), (yyvsp[0].type_list), get_pos(scanner)); }
#line 3192 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 638 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Array( (yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3198 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 640 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Func_Call( (yyvsp[-2].exp), NULL, get_pos(scanner)); }
#line 3204 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 642 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Func_Call( (yyvsp[-3].exp), (yyvsp[-1].exp), get_pos(scanner)); }
#line 3210 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 644 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_member_dot( (yyvsp[-2].exp), (yyvsp[0].sval), get_pos(scanner)); }
#line 3216 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 646 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Postfix_Expression( (yyvsp[-1].exp), op_plusplus, get_pos(scanner)); }
#line 3222 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 648 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Postfix_Expression( (yyvsp[-1].exp), op_minusminus, get_pos(scanner)); }
#line 3228 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 650 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Func_Call( (yyvsp[-3].exp), NULL, get_pos(scanner)); (yyval.exp)->d.exp_func->types = (yyvsp[-2].type_list); }
#line 3234 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 652 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Func_Call( (yyvsp[-4].exp), (yyvsp[-1].exp), get_pos(scanner)); (yyval.exp)->d.exp_func->types = (yyvsp[-3].type_list); }
#line 3240 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 655 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_ID(     (yyvsp[0].sval), get_pos(scanner)); }
#line 3246 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 656 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_int(    (yyvsp[0].ival), get_pos(scanner)); }
#line 3252 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 657 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_float(  (yyvsp[0].fval), get_pos(scanner)); }
#line 3258 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 658 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_string( (yyvsp[0].sval), get_pos(scanner)); }
#line 3264 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 659 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_char(                  (yyvsp[0].sval), get_pos(scanner)); }
#line 3270 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 660 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_array_lit(             (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3276 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 217:
#line 661 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_array_lit(           (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3282 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 218:
#line 662 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_complex(               (yyvsp[0].c_val), get_pos(scanner)); }
#line 3288 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 219:
#line 663 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_polar(                 (yyvsp[0].polar), get_pos(scanner)); }
#line 3294 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 220:
#line 664 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_from_vec(                   (yyvsp[0].vec), get_pos(scanner)); }
#line 3300 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 221:
#line 665 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Hack_Expression(                (yyvsp[-1].exp), get_pos(scanner)); }
#line 3306 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 222:
#line 666 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3312 "eval/parser.c" /* yacc.c:1646  */
    break;

  case 223:
#line 667 "eval/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_Primary_Expression_from_nil(       get_pos(scanner)); }
#line 3318 "eval/parser.c" /* yacc.c:1646  */
    break;


#line 3322 "eval/parser.c" /* yacc.c:1646  */
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
#line 669 "eval/gwion.y" /* yacc.c:1906  */

