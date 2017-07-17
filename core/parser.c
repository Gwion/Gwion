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
#line 8 "util/gwion.y" /* yacc.c:339  */

#define YYERROR_VERBOSE
#include "absyn.h"
#include "arg.h"
#define scanner arg->scanner


char *strcat(char *dest, const char *src);
void minimal_error(void* data, const char* s);
int minimal_lex(void*, void* , void*);
static int get_pos(void* data)
{
  MyArg* arg = (MyArg*)map_get(scan_map, (vtype)data);
  return arg->line;
}


#line 90 "core/parser.c" /* yacc.c:339  */

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
#ifndef YY_MINIMAL_INCLUDE_PARSER_H_INCLUDED
# define YY_MINIMAL_INCLUDE_PARSER_H_INCLUDED
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
    NEG = 358
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
#define NEG 358

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 26 "util/gwion.y" /* yacc.c:355  */

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
  Class_Ext class_ext;
  Class_Def class_def;
  Ast ast;

#line 363 "core/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int minimal_parse (MyArg* arg);

#endif /* !YY_MINIMAL_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 379 "core/parser.c" /* yacc.c:358  */

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
#define YYFINAL  132
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1250

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  104
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  67
/* YYNRULES -- Number of rules.  */
#define YYNRULES  206
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  363

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
       0,   152,   152,   153,   157,   158,   159,   163,   167,   167,
     170,   171,   175,   176,   180,   181,   185,   186,   187,   191,
     192,   196,   197,   201,   202,   206,   207,   211,   212,   216,
     217,   218,   219,   223,   224,   225,   229,   230,   234,   235,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   254,   255,   259,   263,   267,   268,   272,   276,
     278,   280,   282,   284,   286,   288,   293,   295,   300,   301,
     302,   303,   307,   308,   312,   313,   317,   318,   322,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   354,   355,   356,   360,   361,   362,
     366,   367,   368,   372,   373,   377,   378,   382,   384,   386,
     392,   393,   394,   395,   400,   401,   405,   409,   410,   414,
     415,   416,   420,   425,   430,   433,   434,   439,   440,   445,
     446,   451,   452,   457,   458,   463,   464,   469,   470,   472,
     477,   478,   480,   482,   484,   489,   490,   494,   495,   500,
     501,   503,   508,   509,   511,   513,   518,   519,   524,   525,
     527,   529,   531,   536,   537,   538,   539,   540,   541,   542,
     546,   547,   551,   552,   555,   555,   556,   556,   559,   560,
     562,   564,   566,   568,   573,   574,   575,   576,   577,   578,
     579,   580,   581,   582,   583,   584,   585
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
  "NEG", "$accept", "ast", "section", "class_def", "class_ext",
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
  "call_template", "call_paren", "postfix_exp", "primary_exp", YY_NULLPTR
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

#define YYPACT_NINF -302

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-302)))

#define YYTABLE_NINF -185

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     532,  -302,  -302,  1088,   728,   788,   196,  1088,  1088,  -302,
      24,    29,   608,    35,    45,    51,   -20,    57,    54,    94,
      23,   128,   130,  -302,  -302,   -17,   113,   -16,  -302,    49,
    -302,  -302,   147,   -17,   -17,    67,   145,  1088,  -302,  -302,
      -7,  -302,  -302,   163,   304,  -302,   110,  -302,  -302,  -302,
     608,  -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,
     164,  1145,  -302,   152,  -302,    -1,  -302,    69,  -302,  -302,
    -302,  -302,  -302,     5,   111,    98,   101,   104,   -15,    41,
      60,    77,    78,   148,  -302,  1148,   117,     9,  -302,   -17,
      -4,   171,  -302,   170,  -302,   169,  -302,   168,   173,   174,
    1088,  1088,    79,  1088,  1088,   668,   185,  1088,  -302,    84,
     155,    91,  -302,   189,  -302,  -302,   165,   179,   180,   -17,
      69,   137,   134,   187,   102,   186,   191,    91,  1088,   188,
    -302,  -302,  -302,  -302,    91,  -302,  -302,  -302,  1088,  -302,
    -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,
    -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,
    -302,  -302,  -302,  1088,   848,  -302,  -302,  -302,   154,   191,
    -302,   202,  1088,  1148,  1148,  1148,  1148,  1148,  1148,  1148,
    1148,  1148,  1148,  1148,  -302,  -302,  1148,  1148,  1148,  1148,
    1148,  1148,   -17,  -302,    54,  1088,  -302,  -302,   114,   115,
    -302,  -302,   204,  -302,  -302,   203,  -302,  -302,  -302,   205,
     207,   209,   210,   212,   213,   668,  -302,   214,  -302,  -302,
     223,   216,  -302,  -302,  -302,   217,  -302,    49,   221,   180,
     240,   153,  -302,   152,   160,   252,   239,  -302,   200,  -302,
    -302,  -302,  -302,   -17,  -302,   152,    69,   224,   111,    98,
     101,   104,   -15,    41,    41,    60,    60,    60,    60,    77,
      78,    78,   148,   148,   148,  -302,     9,  -302,   254,   166,
     908,  -302,   968,  -302,   608,   608,  1088,  1088,   608,   608,
    1028,   180,    91,    16,   172,  -302,  -302,  -302,     7,   180,
     255,  -302,  1088,   262,    49,   253,   175,  -302,  1148,   115,
    -302,  -302,   261,  -302,   242,  -302,   263,   264,  -302,  -302,
     608,   268,  -302,  -302,  -302,   279,   270,  -302,   273,    69,
    -302,   275,  -302,  -302,  -302,   380,   240,  -302,  -302,  -302,
     608,   286,   287,  -302,   608,  -302,   289,  -302,   294,    26,
    -302,   274,  -302,   456,  -302,  -302,   180,  -302,  -302,  -302,
    -302,    27,   -17,  -302,   290,  -302,  -302,  -302,  -302,   292,
    -302,  -302,  -302
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     115,    73,   175,     0,     0,     0,     0,     0,     0,   178,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   176,   177,     0,     0,     0,    10,     0,
     174,   173,     0,     0,     0,     0,     0,     0,   195,   196,
     194,   197,   198,     0,   115,     6,     0,     4,    50,    43,
      23,    48,    44,    45,    47,    46,    41,    42,    49,    40,
       0,    74,   199,   200,    76,     0,     5,     0,    51,   201,
     202,   203,   110,   135,   137,   139,   141,   143,   145,   147,
     150,   157,   159,   162,   166,     0,   168,   180,   188,     0,
     194,     0,   206,     0,   107,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   194,   184,
     188,     0,    68,     0,    70,    71,   120,   170,   179,     0,
       0,    21,     0,     0,     0,     0,    33,     0,     0,     0,
     121,    54,     1,     3,     0,    24,    72,    79,     0,   103,
      83,    84,    85,    86,    87,    82,    80,    88,    89,    81,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,     0,     0,   109,    27,    28,    26,   129,
     111,   127,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   155,   156,     0,     0,     0,     0,
       0,     0,     0,   169,     0,     0,   192,   193,     0,     0,
     185,   189,     0,   204,   205,   104,    39,   133,   132,     0,
       0,     0,     0,     0,     0,     0,    55,     0,    57,    56,
      19,     0,    69,   171,   172,     0,   112,     0,   122,     0,
       0,     0,    35,    34,     0,     0,     0,   134,     9,    75,
      77,   108,    25,     0,   130,   131,     0,     0,   138,   140,
     142,   144,   146,   148,   149,   154,   152,   153,   151,   158,
     161,   160,   164,   163,   165,   167,   181,   191,   182,     0,
       0,   190,     0,   105,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,   123,   119,     0,     0,
       0,   116,   124,     0,     0,     0,     0,   128,     0,     0,
      78,   186,     0,   106,    66,    59,     0,     0,    63,    65,
       0,     0,    58,    20,    52,     0,     0,   113,     0,     0,
     118,     0,   125,   126,     8,   115,     0,   136,   183,   187,
       0,     0,     0,    61,     0,    53,     0,   114,    36,     0,
      18,     0,    12,   115,    16,    17,     0,    67,    60,    64,
      62,     0,     0,    29,     0,     7,    15,   117,    30,     0,
      37,    31,    32
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -302,   265,  -302,  -301,  -302,  -302,  -302,   -37,  -302,  -120,
    -197,    -6,  -302,  -302,  -302,  -102,  -296,  -116,    -8,  -302,
    -302,  -302,  -302,  -302,  -302,  -302,  -302,   -87,     8,  -302,
    -302,  -302,   -55,   -92,   150,   -18,  -302,  -273,   -24,    18,
    -302,  -114,    -2,  -302,  -302,  -302,    13,  -302,   151,   149,
     146,   157,   158,   -31,  -302,   -43,   139,   -30,   -90,   244,
    -302,  -302,    31,  -302,  -302,   -13,   316
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    43,    44,    45,   295,    46,   341,   342,   343,   221,
     122,    47,   243,   168,    48,   125,   318,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
     200,   163,    62,    63,    64,   289,    65,    66,    67,   236,
      68,   170,   171,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,   186,    80,    81,    82,    83,    84,
      85,    86,   269,   202,   271,    87,    88
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      97,   117,   224,   120,   102,   109,   226,   234,   165,   124,
     126,    91,    93,    95,   238,    98,    99,   225,   215,   314,
     317,  -184,   130,   195,   340,   130,   112,   131,   113,   166,
     285,     2,   201,     3,   233,     4,   100,     5,   172,   353,
     358,   101,   340,   354,   135,   129,   178,   103,    29,    29,
       7,     8,   345,   179,   201,   359,   360,   104,     9,   196,
     197,   119,   223,   105,     3,   120,     4,   198,     5,   107,
     345,   232,    29,    23,    24,    25,    26,   245,    89,   173,
     106,     7,     8,   116,   116,   189,   190,   191,    29,    30,
      31,    29,    29,  -120,   167,   126,  -120,   324,   195,   262,
     263,   264,   199,   180,   181,   182,   183,   116,   209,   210,
     111,   213,   214,   287,   244,   217,   315,   211,   218,   212,
      37,    38,    39,    90,    41,    42,   116,   116,   280,   184,
     185,   114,   297,   115,   196,   197,   235,   255,   256,   257,
     258,   296,   198,   187,   188,   118,   239,   253,   254,   121,
     273,    37,    38,    39,   108,    41,    42,   260,   261,   123,
     127,   128,   313,   132,   134,   312,   164,   136,   265,   169,
     175,   174,    95,   320,   176,   177,   194,   199,   165,   192,
     247,   266,   203,   204,   205,   206,   207,   208,   216,   219,
     165,   220,   222,   195,   130,   227,     6,   228,   231,     1,
     229,   237,   230,    95,     2,     5,     3,   246,     4,   242,
       5,   201,     6,    96,   267,   268,   270,   272,   274,   126,
     275,   276,   277,     7,     8,   278,   279,   281,   282,   284,
     357,     9,    10,   283,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
     286,    27,   288,   290,   291,   292,   293,   294,   298,   299,
     300,    29,    30,    31,   319,   323,   304,   305,   321,   325,
     308,   309,   316,    34,   329,   326,   331,   332,   302,   330,
      95,   334,   335,   336,   306,   307,   337,   339,   311,   348,
     349,   355,    36,    37,    38,    39,    40,    41,    42,   352,
     235,   351,   333,   361,    -2,   362,   356,     1,   346,   133,
     322,   327,     2,   240,     3,   319,     4,   338,     5,   344,
       6,   250,   347,   249,   248,   259,   350,   319,   319,   193,
     328,     7,     8,   251,   110,   252,     0,   344,     0,     9,
      10,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   -11,    27,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    29,
      30,    31,     0,     0,     0,     0,     0,     0,     0,    32,
      33,    34,     0,     1,     0,     0,     0,     0,     2,     0,
       3,     0,     4,     0,     5,    35,     6,   -13,     0,     0,
      36,    37,    38,    39,    40,    41,    42,     7,     8,     0,
       0,     0,     0,     0,     0,     9,    10,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   -11,    27,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    30,    31,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,     0,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,     0,
       5,    35,     6,   -14,     0,     0,    36,    37,    38,    39,
      40,    41,    42,     7,     8,     0,     0,     0,     0,     0,
       0,     9,    10,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
     -11,    27,    28,     0,     0,     0,     0,     0,     0,     0,
       0,    29,    30,    31,     0,     0,     0,     0,     0,     0,
       0,    32,    33,    34,     0,     1,     0,     0,     0,     0,
       2,     0,     3,     0,     4,     0,     5,    35,     6,     0,
       0,     0,    36,    37,    38,    39,    40,    41,    42,     7,
       8,     0,     0,     0,     0,     0,     0,     9,    10,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   -11,    27,    28,     0,
       0,     0,     0,     0,     0,     0,     0,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,    32,    33,    34,
       0,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,     0,     5,    35,     6,     0,     0,     0,    36,    37,
      38,    39,    40,    41,    42,     7,     8,     0,     0,     0,
       0,     0,     0,     9,    10,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,     0,     0,     0,     0,     0,     0,
       0,     1,     0,    29,    30,    31,     2,     0,     3,     0,
       4,     0,     5,     0,     0,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,     9,    36,    37,    38,    39,    40,    41,
      42,     0,     0,     0,     0,     0,     0,     0,    23,    24,
      25,    26,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     2,     0,     3,     0,
       4,    92,     5,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,     9,     0,    37,    38,    39,    90,    41,
      42,     0,     0,     0,     0,     0,     0,     0,    23,    24,
      25,    26,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     2,     0,     3,     0,
       4,     0,     5,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,     9,     0,    37,    38,    39,    90,    41,
      42,     0,     0,     0,     0,     0,     0,     0,    23,    24,
      25,    26,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     2,     0,     3,     0,
       4,     0,     5,   241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,     9,     0,    37,    38,    39,    90,    41,
      42,     0,     0,     0,     0,     0,     0,     0,    23,    24,
      25,    26,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     2,     0,     3,     0,
       4,   301,     5,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,     9,     0,    37,    38,    39,    90,    41,
      42,     0,     0,     0,     0,     0,     0,     0,    23,    24,
      25,    26,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     2,     0,     3,     0,
       4,     0,     5,   303,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,     9,     0,    37,    38,    39,    90,    41,
      42,     0,     0,     0,     0,     0,     0,     0,    23,    24,
      25,    26,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     2,     0,     3,     0,
       4,   310,     5,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,     9,     0,    37,    38,    39,    90,    41,
      42,     0,     0,     0,     0,     0,     0,     0,    23,    24,
      25,    26,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     2,     0,     3,     0,
       4,     0,     5,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,     9,     0,    37,    38,    39,    90,    41,
      42,     0,     0,     0,     0,     0,     0,     0,    23,    24,
      25,    26,     0,    89,     0,     0,     0,     0,     0,   137,
     138,   139,     0,    29,    30,    31,     2,     0,     3,     0,
       4,     0,     5,   140,   141,   142,   143,   144,   145,   146,
     147,   148,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,     9,     0,    37,    38,    39,    90,    41,
      42,     0,     0,     0,     0,     0,     0,     0,    23,    24,
      25,    26,     0,     0,     0,     0,   149,     0,     0,     0,
       0,     0,     0,     0,    30,    31,     0,     0,     0,     0,
       0,     0,     0,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,    38,    39,   108,    41,
      42
};

static const yytype_int16 yycheck[] =
{
       6,    25,   118,    27,    12,    18,   120,   127,    63,    33,
      34,     3,     4,     5,   134,     7,     8,   119,   105,     3,
      13,    12,    29,    14,   325,    29,     3,    34,    20,    30,
     227,     8,    87,    10,   126,    12,    12,    14,    33,    13,
      13,    12,   343,   339,    50,    37,    61,    12,    65,    65,
      27,    28,   325,    68,   109,   351,   352,    12,    35,    50,
      51,    77,   117,    12,    10,    89,    12,    58,    14,    12,
     343,   126,    65,    50,    51,    52,    53,   169,    55,    74,
     100,    27,    28,   100,   100,     7,     8,     9,    65,    66,
      67,    65,    65,   100,    95,   119,   100,   294,    14,   189,
     190,   191,    93,    62,    63,    64,    65,   100,   100,   101,
      16,   103,   104,   229,   169,   107,   100,    38,    34,    40,
      97,    98,    99,   100,   101,   102,   100,   100,   215,    69,
      70,     3,   246,     3,    50,    51,   128,   180,   181,   182,
     183,   243,    58,    66,    67,    32,   138,   178,   179,   100,
     205,    97,    98,    99,   100,   101,   102,   187,   188,    12,
      93,    16,   282,     0,    54,   281,    14,     3,   192,   100,
      72,    60,   164,   289,    73,    71,    59,    93,   233,    31,
     172,   194,    11,    13,    15,    17,    13,    13,     3,    34,
     245,   100,     3,    14,    29,    58,    16,    63,    12,     3,
      13,    13,   100,   195,     8,    14,    10,     5,    12,    55,
      14,   266,    16,    17,   100,   100,    12,    14,    13,   243,
      13,    12,    12,    27,    28,    13,    13,    13,     5,    12,
     346,    35,    36,    17,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      29,    55,    12,   100,    94,     3,    17,    57,    34,     5,
      94,    65,    66,    67,   288,     3,   274,   275,    13,    16,
     278,   279,   100,    77,    13,   100,    13,    13,   270,    37,
     272,    13,     3,    13,   276,   277,    13,    12,   280,     3,
       3,    17,    96,    97,    98,    99,   100,   101,   102,     5,
     292,    12,   310,    13,     0,    13,   343,     3,   326,    44,
     292,   298,     8,   163,    10,   339,    12,   319,    14,   325,
      16,   175,   330,   174,   173,   186,   334,   351,   352,    85,
     299,    27,    28,   176,    18,   177,    -1,   343,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    -1,     3,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    12,    -1,    14,    91,    16,    17,    -1,    -1,
      96,    97,    98,    99,   100,   101,   102,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    -1,
      14,    91,    16,    17,    -1,    -1,    96,    97,    98,    99,
     100,   101,   102,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    -1,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    -1,    14,    91,    16,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    -1,    14,    91,    16,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,   102,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    65,    66,    67,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    96,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,    -1,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,    -1,
      12,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,    -1,
      12,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,    -1,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,    -1,
      12,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,    -1,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,     4,
       5,     6,    -1,    65,    66,    67,     8,    -1,    10,    -1,
      12,    -1,    14,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    35,
      36,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    55,    56,    65,
      66,    67,    75,    76,    77,    91,    96,    97,    98,    99,
     100,   101,   102,   105,   106,   107,   109,   115,   118,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   136,   137,   138,   140,   141,   142,   144,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     159,   160,   161,   162,   163,   164,   165,   169,   170,    55,
     100,   132,    13,   132,    15,   132,    17,   115,   132,   132,
      12,    12,   122,    12,    12,    12,   100,    12,   100,   169,
     170,    16,     3,   132,     3,     3,   100,   142,    32,    77,
     142,   100,   114,    12,   142,   119,   142,    93,    16,   132,
      29,    34,     0,   105,    54,   115,     3,     4,     5,     6,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    61,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,   135,    14,   136,    30,    95,   117,   100,
     145,   146,    33,    74,    60,    72,    73,    71,    61,    68,
      62,    63,    64,    65,    69,    70,   158,    66,    67,     7,
       8,     9,    31,   163,    59,    14,    50,    51,    58,    93,
     134,   136,   167,    11,    13,    15,    17,    13,    13,   132,
     132,    38,    40,   132,   132,   131,     3,   132,    34,    34,
     100,   113,     3,   136,   121,   119,   145,    58,    63,    13,
     100,    12,   136,   137,   113,   132,   143,    13,   113,   132,
     138,    15,    55,   116,   136,   137,     5,   132,   152,   153,
     154,   155,   156,   157,   157,   159,   159,   159,   159,   160,
     161,   161,   162,   162,   162,   142,   169,   100,   100,   166,
      12,   168,    14,   136,    13,    13,    12,    12,    13,    13,
     131,    13,     5,    17,    12,   114,    29,   121,    12,   139,
     100,    94,     3,    17,    57,   108,   119,   145,    34,     5,
      94,    13,   132,    15,   122,   122,   132,   132,   122,   122,
      13,   132,   121,   113,     3,   100,   100,    13,   120,   142,
     121,    13,   143,     3,   114,    16,   100,   150,   166,    13,
      37,    13,    13,   122,    13,     3,    13,    13,   146,    12,
     107,   110,   111,   112,   115,   141,   139,   122,     3,     3,
     122,    12,     5,    13,   120,    17,   111,   121,    13,   120,
     120,    13,    13
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
     130,   130,   131,   131,   132,   132,   133,   133,   134,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   136,   136,   136,   137,   137,   137,
     138,   138,   138,   139,   139,   140,   140,   141,   141,   141,
     142,   142,   142,   142,   143,   143,   144,   145,   145,   146,
     146,   146,   147,   148,   149,   150,   150,   151,   151,   152,
     152,   153,   153,   154,   154,   155,   155,   156,   156,   156,
     157,   157,   157,   157,   157,   158,   158,   159,   159,   160,
     160,   160,   161,   161,   161,   161,   162,   162,   163,   163,
     163,   163,   163,   164,   164,   164,   164,   164,   164,   164,
     165,   165,   166,   166,   167,   167,   168,   168,   169,   169,
     169,   169,   169,   169,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170
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
       2,     2,     2,     1,     1,     3,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     4,     5,     2,     3,     2,
       1,     2,     3,     2,     3,     0,     4,     7,     5,     4,
       1,     2,     3,     4,     2,     3,     5,     1,     3,     1,
       2,     2,     3,     3,     3,     1,     5,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     1,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     1,     3,     1,     2,
       2,     3,     3,     1,     1,     1,     1,     1,     1,     2,
       1,     3,     1,     3,     0,     1,     2,     3,     1,     2,
       3,     3,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     2
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
#line 148 "util/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1671 "core/parser.c" /* yacc.c:1257  */
        break;

    case 142: /* type_decl  */
#line 148 "util/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1677 "core/parser.c" /* yacc.c:1257  */
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
#line 152 "util/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL, get_pos(scanner));  }
#line 1945 "core/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 153 "util/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(scanner)); }
#line 1951 "core/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 157 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(scanner)); }
#line 1957 "core/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 158 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(scanner)); }
#line 1963 "core/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 159 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(scanner)); }
#line 1969 "core/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 164 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].class_ext), (yyvsp[-1].class_body), get_pos(scanner)); }
#line 1975 "core/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 167 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = new_class_ext( (yyvsp[0].id_list), NULL, get_pos(scanner)); }
#line 1981 "core/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 167 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.class_ext) = NULL; }
#line 1987 "core/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 170 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_public; }
#line 1993 "core/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 171 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_private; }
#line 1999 "core/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 175 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2005 "core/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 176 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2011 "core/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 180 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body( (yyvsp[0].section), get_pos(scanner)); }
#line 2017 "core/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 181 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body( (yyvsp[-1].section), (yyvsp[0].class_body), get_pos(scanner)); }
#line 2023 "core/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 185 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list( (yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2029 "core/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 186 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def( (yyvsp[0].func_def), get_pos(scanner)); }
#line 2035 "core/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 187 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def( (yyvsp[0].class_def), get_pos(scanner)); }
#line 2041 "core/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 191 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2047 "core/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 192 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2053 "core/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 196 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2059 "core/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 197 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2065 "core/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 201 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(scanner));}
#line 2071 "core/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 202 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(scanner));}
#line 2077 "core/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 206 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_static;   }
#line 2083 "core/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 207 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_instance; }
#line 2089 "core/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 211 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_func; }
#line 2095 "core/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 212 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2101 "core/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 216 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2107 "core/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 217 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_flag_static, (yyvsp[-3].sval), (yyvsp[-5].type_decl), NULL, get_pos(scanner)); }
#line 2113 "core/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 218 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2119 "core/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 219 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_flag_static, (yyvsp[-4].sval), (yyvsp[-6].type_decl), (yyvsp[-1].arg_list), get_pos(scanner)); }
#line 2125 "core/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 223 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2131 "core/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 224 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2137 "core/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 225 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2143 "core/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 229 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(scanner)); }
#line 2149 "core/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 230 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(scanner)); }
#line 2155 "core/parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 234 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( NULL, get_pos(scanner)); }
#line 2161 "core/parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 235 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( (yyvsp[-1].stmt_list), get_pos(scanner)); }
#line 2167 "core/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 254 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-2].id_list), NULL, get_pos(scanner)); }
#line 2173 "core/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 255 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sval), get_pos(scanner)); }
#line 2179 "core/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 259 "util/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 1, get_pos(scanner)); }
#line 2185 "core/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 263 "util/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 0, get_pos(scanner)); }
#line 2191 "core/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 267 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2197 "core/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 268 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2203 "core/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 272 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner));}
#line 2209 "core/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 277 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2215 "core/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 279 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2221 "core/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 281 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2227 "core/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 283 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2233 "core/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 285 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2239 "core/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 287 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2245 "core/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 289 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop( (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2251 "core/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 294 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(scanner)); }
#line 2257 "core/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 296 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2263 "core/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 300 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( NULL, get_pos(scanner)); }
#line 2269 "core/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 301 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2275 "core/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 302 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_break(get_pos(scanner)); }
#line 2281 "core/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 303 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_continue(get_pos(scanner)); }
#line 2287 "core/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 307 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_expression((yyvsp[-1].exp),   get_pos(scanner)); }
#line 2293 "core/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 308 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_expression(NULL, get_pos(scanner)); }
#line 2299 "core/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 312 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2305 "core/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 313 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_expression((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2311 "core/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 317 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2317 "core/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 318 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2323 "core/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 322 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2329 "core/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 326 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2335 "core/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 327 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2341 "core/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 328 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; /* LCOV_EXCL_LINE */ }
#line 2347 "core/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 329 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2353 "core/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 330 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2359 "core/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 331 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2365 "core/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 332 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2371 "core/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 333 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2377 "core/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 334 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2383 "core/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 335 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2389 "core/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 336 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2395 "core/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 337 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2401 "core/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 338 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2407 "core/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 339 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2413 "core/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 340 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2419 "core/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 341 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2425 "core/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 342 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2431 "core/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 343 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2437 "core/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 344 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2443 "core/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 345 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2449 "core/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 346 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2455 "core/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 347 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2461 "core/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 348 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2467 "core/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 349 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2473 "core/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 350 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2479 "core/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 354 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( (yyvsp[-1].exp), get_pos(scanner) ); }
#line 2485 "core/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 355 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[0].array_sub), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2491 "core/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 356 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( new_array_sub( NULL, get_pos(scanner)), (yyvsp[-3].exp), get_pos(scanner)); }
#line 2497 "core/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 360 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( NULL, get_pos(scanner)); }
#line 2503 "core/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 361 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-2].array_sub), NULL, get_pos(scanner)); }
#line 2509 "core/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 362 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[-1].array_sub), (yyvsp[0].array_sub)->exp_list, get_pos(scanner)); }
#line 2515 "core/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 367 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); }
#line 2521 "core/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 368 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); }
#line 2527 "core/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 372 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2533 "core/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 373 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2539 "core/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 377 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2545 "core/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 378 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list);   }
#line 2551 "core/parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 383 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->types = (yyvsp[-6].id_list); if((yyvsp[-6].id_list)) SET_FLAG((yyval.func_def), ae_flag_template);}
#line 2557 "core/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 385 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_func | ae_flag_static | ae_flag_op , (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2563 "core/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 387 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_func | ae_flag_instance | ae_flag_dtor, new_type_decl(new_id_list("void", get_pos(scanner)), 0, 
      get_pos(scanner)), "dtor", NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2570 "core/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 392 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[0].sval), get_pos(scanner)), 0, get_pos(scanner)); }
#line 2576 "core/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 393 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), 1, get_pos(scanner)); }
#line 2582 "core/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 394 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-1].id_list),  0, get_pos(scanner)); }
#line 2588 "core/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 395 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list),  1, get_pos(scanner)); }
#line 2594 "core/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 400 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2600 "core/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 401 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2606 "core/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 405 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-2].decl_list), get_pos(scanner)); }
#line 2612 "core/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 409 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(scanner)); }
#line 2618 "core/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 410 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(scanner)); }
#line 2624 "core/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 414 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sval), NULL, get_pos(scanner)); }
#line 2630 "core/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 415 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2636 "core/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 416 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2642 "core/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 421 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2648 "core/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 426 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar((yyvsp[-1].exp), get_pos(scanner)); }
#line 2654 "core/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 430 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec((yyvsp[-1].exp), get_pos(scanner)); }
#line 2660 "core/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 435 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_if( (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2666 "core/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 439 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2672 "core/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 441 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2678 "core/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 445 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2684 "core/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 447 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2690 "core/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 451 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2696 "core/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 453 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2702 "core/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 457 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2708 "core/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 459 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(scanner)); }
#line 2714 "core/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 463 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2720 "core/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 465 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2726 "core/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 469 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2732 "core/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 471 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_eq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2738 "core/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 473 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_neq, (yyvsp[0].exp), get_pos(scanner)); }
#line 2744 "core/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 477 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2750 "core/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 479 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_lt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2756 "core/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 481 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_gt, (yyvsp[0].exp), get_pos(scanner)); }
#line 2762 "core/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 483 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_le, (yyvsp[0].exp), get_pos(scanner)); }
#line 2768 "core/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 485 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_ge, (yyvsp[0].exp), get_pos(scanner)); }
#line 2774 "core/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 489 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 2780 "core/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 490 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 2786 "core/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 494 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2792 "core/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 496 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2798 "core/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 500 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2804 "core/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 502 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_plus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2810 "core/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 504 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_minus, (yyvsp[0].exp), get_pos(scanner)); }
#line 2816 "core/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 508 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2822 "core/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 510 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_times, (yyvsp[0].exp), get_pos(scanner)); }
#line 2828 "core/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 512 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_divide, (yyvsp[0].exp), get_pos(scanner)); }
#line 2834 "core/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 514 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_percent, (yyvsp[0].exp), get_pos(scanner)); }
#line 2840 "core/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 520 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_cast( (yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2846 "core/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 524 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2852 "core/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 526 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary( (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2858 "core/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 528 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(scanner)); }
#line 2864 "core/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 530 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary2(op_new, (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2870 "core/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 532 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary3( op_spork, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2876 "core/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 536 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2882 "core/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 537 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 2888 "core/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 538 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 2894 "core/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 539 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 2900 "core/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 540 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 2906 "core/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 541 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 2912 "core/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 542 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 2918 "core/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 547 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2924 "core/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 551 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sval), get_pos(scanner)), NULL, get_pos(scanner)); }
#line 2930 "core/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 552 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sval), get_pos(scanner)), (yyvsp[0].type_list), get_pos(scanner)); }
#line 2936 "core/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 555 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 2942 "core/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 555 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[0].type_list);}
#line 2948 "core/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 556 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 2954 "core/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 556 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 2960 "core/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 561 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array( (yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2966 "core/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 563 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_call( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); (yyval.exp)->d.exp_func.types = (yyvsp[-1].type_list); }
#line 2972 "core/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 565 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dot( (yyvsp[-2].exp), (yyvsp[0].sval), get_pos(scanner)); }
#line 2978 "core/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 567 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_postfix( (yyvsp[-1].exp), op_plusplus, get_pos(scanner)); }
#line 2984 "core/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 569 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_postfix( (yyvsp[-1].exp), op_minusminus, get_pos(scanner)); }
#line 2990 "core/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 573 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_ID(     (yyvsp[0].sval), get_pos(scanner)); }
#line 2996 "core/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 574 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_int(    (yyvsp[0].ival), get_pos(scanner)); }
#line 3002 "core/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 575 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_float(  (yyvsp[0].fval), get_pos(scanner)); }
#line 3008 "core/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 576 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_string( (yyvsp[0].sval), get_pos(scanner)); }
#line 3014 "core/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 577 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_char(                  (yyvsp[0].sval), get_pos(scanner)); }
#line 3020 "core/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 578 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(             (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3026 "core/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 579 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(           (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3032 "core/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 580 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_complex(               (yyvsp[0].c_val), get_pos(scanner)); }
#line 3038 "core/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 581 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_polar(                 (yyvsp[0].polar), get_pos(scanner)); }
#line 3044 "core/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 582 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_vec(                   (yyvsp[0].vec), get_pos(scanner)); }
#line 3050 "core/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 583 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_hack(                (yyvsp[-1].exp), get_pos(scanner)); }
#line 3056 "core/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 584 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3062 "core/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 585 "util/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_nil(       get_pos(scanner)); }
#line 3068 "core/parser.c" /* yacc.c:1646  */
    break;


#line 3072 "core/parser.c" /* yacc.c:1646  */
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
#line 587 "util/gwion.y" /* yacc.c:1906  */

