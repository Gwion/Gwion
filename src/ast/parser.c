/* A Bison parser, made by GNU Bison 3.0.5.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.5"

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
#line 5 "utils/gwion.y" /* yacc.c:339  */

#include "absyn.h"
#include "scanner.h"
#define YYMALLOC xmalloc
#define scan arg->scanner
#define CHECK_FLAG(a,b,c) if(GET_FLAG(b, c)) { gwion_error(a, "flag set twice");  ; } SET_FLAG(b, c);
#define CHECK_TEMPLATE(a, b, c, free_function) { if(c->tmpl) {\
        free_id_list(b);\
        free_function(c);\
        gwion_error(a, "double template decl");\
        YYERROR;\
      }\
      c->tmpl = new_tmpl_class(b, -1);\
    };
#define OP_SYM(a) insert_symbol(op2str(a))
int gwion_error(Scanner*, const char*);
int gwion_lex(void*, Scanner*);
ANN int get_pos(const Scanner*);
m_str op2str(const Operator op);

#line 93 "src/ast/parser.c" /* yacc.c:339  */

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
    PROTECT = 313,
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
    TYPEDEF = 334,
    RSL = 335,
    RSR = 336,
    RSAND = 337,
    RSOR = 338,
    RSXOR = 339,
    TEMPLATE = 340,
    NOELSE = 341,
    LTB = 342,
    GTB = 343,
    UNION = 344,
    ATPAREN = 345,
    TYPEOF = 346,
    CONST = 347,
    AUTO = 348,
    AUTO_PTR = 349,
    NUM = 350,
    FLOAT = 351,
    ID = 352,
    STRING_LIT = 353,
    CHAR_LIT = 354
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
#define PROTECT 313
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
#define TYPEDEF 334
#define RSL 335
#define RSR 336
#define RSAND 337
#define RSOR 338
#define RSXOR 339
#define TEMPLATE 340
#define NOELSE 341
#define LTB 342
#define GTB 343
#define UNION 344
#define ATPAREN 345
#define TYPEOF 346
#define CONST 347
#define AUTO 348
#define AUTO_PTR 349
#define NUM 350
#define FLOAT 351
#define ID 352
#define STRING_LIT 353
#define CHAR_LIT 354

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 26 "utils/gwion.y" /* yacc.c:355  */

  char* sval;
  int ival;
  m_float fval;
  Symbol sym;
  Array_Sub array_sub;
  Var_Decl var_decl;
  Var_Decl_List var_decl_list;
  Type_Decl* type_decl;
  Exp   exp;
  Stmt_Fptr func_ptr;
  Stmt stmt;
  Stmt_List stmt_list;
  Arg_List arg_list;
  Decl_List decl_list;
  Func_Def func_def;
  Section* section;
  ID_List id_list;
  Type_List type_list;
  Class_Body class_body;
  ID_List class_ext;
  Class_Def class_def;
  Ast ast;

#line 356 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (Scanner* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 372 "src/ast/parser.c" /* yacc.c:358  */

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
#define YYFINAL  199
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1705

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  100
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  73
/* YYNRULES -- Number of rules.  */
#define YYNRULES  221
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  417

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   354

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
      95,    96,    97,    98,    99
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   102,   102,   103,   107,   108,   109,   113,   115,   116,
     119,   119,   121,   121,   124,   125,   129,   130,   134,   135,
     139,   140,   144,   145,   149,   151,   153,   158,   162,   163,
     164,   168,   169,   173,   174,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   193,   195,
     195,   198,   199,   200,   204,   208,   212,   213,   217,   221,
     223,   225,   227,   229,   231,   233,   235,   237,   242,   244,
     249,   250,   251,   252,   256,   257,   261,   262,   266,   267,
     270,   270,   272,   272,   272,   273,   273,   274,   274,   275,
     275,   276,   276,   277,   277,   277,   278,   278,   279,   283,
     284,   285,   289,   290,   291,   295,   296,   297,   299,   301,
     306,   307,   311,   312,   315,   318,   320,   322,   335,   335,
     335,   335,   335,   335,   337,   338,   340,   342,   347,   347,
     350,   351,   352,   354,   356,   357,   361,   362,   366,   367,
     369,   371,   376,   377,   381,   382,   383,   386,   386,   389,
     389,   392,   392,   395,   395,   398,   398,   401,   401,   404,
     404,   406,   406,   409,   409,   410,   410,   412,   412,   416,
     417,   420,   420,   423,   423,   425,   425,   428,   428,   428,
     430,   430,   433,   433,   436,   436,   436,   437,   437,   438,
     438,   441,   441,   443,   452,   455,   455,   459,   460,   463,
     463,   465,   465,   467,   467,   469,   472,   474,   478,   479,
     480,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493
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
  "STATIC", "PUBLIC", "PRIVATE", "PROTECT", "EXTENDS", "DOT", "COLONCOLON",
  "AND", "EQ", "GE", "GT", "LE", "LT", "MINUS", "PLUS", "NEQ",
  "SHIFT_LEFT", "SHIFT_RIGHT", "S_AND", "S_OR", "S_XOR", "OR", "AST_DTOR",
  "OPERATOR", "TYPEDEF", "RSL", "RSR", "RSAND", "RSOR", "RSXOR",
  "TEMPLATE", "NOELSE", "LTB", "GTB", "UNION", "ATPAREN", "TYPEOF",
  "CONST", "AUTO", "AUTO_PTR", "NUM", "FLOAT", "ID", "STRING_LIT",
  "CHAR_LIT", "$accept", "ast", "section", "class_def", "class_ext",
  "class_body", "class_body2", "id_list", "id_dot", "stmt_list",
  "static_decl", "func_ptr", "stmt_type", "type_decl2", "arg_list",
  "code_segment", "stmt", "id", "opt_id", "enum_stmt", "label_stmt",
  "goto_stmt", "case_stmt", "switch_stmt", "loop_stmt", "selection_stmt",
  "jump_stmt", "exp_stmt", "exp", "binary_exp", "template", "op",
  "array_exp", "array_empty", "decl_exp", "func_args", "arg_type",
  "decl_template", "func_def_base", "op_op", "func_def", "atsym",
  "type_decl", "decl_list", "union_stmt", "var_decl_list", "var_decl",
  "con_exp", "log_or_exp", "log_and_exp", "inc_or_exp", "exc_or_exp",
  "and_exp", "eq_op", "eq_exp", "rel_op", "relational_exp", "shift_op",
  "shift_exp", "add_op", "add_exp", "mul_op", "mul_exp", "cast_exp",
  "unary_op", "unary_exp", "dur_exp", "type_list", "call_paren", "post_op",
  "post_exp", "vec_type", "primary_exp", YY_NULLPTR
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
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354
};
# endif

#define YYPACT_NINF -344

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-344)))

#define YYTABLE_NINF -129

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     435,  -344,  -344,  1413,  1056,  1107,   541,  -344,  -344,     3,
    -344,    23,    49,   638,    77,    95,   109,    20,   130,   864,
     136,   763,   150,   153,  -344,  -344,    92,   125,    20,   939,
      76,   888,  1005,    20,  -344,  -344,   146,  1549,    92,    73,
      92,   154,  -344,   159,    92,  -344,  -344,  -344,  -344,  -344,
     172,   435,  -344,  -344,  -344,  -344,  -344,   638,     2,  -344,
    -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,   171,  1525,
    -344,   161,  -344,    11,  -344,  -344,    20,  -344,  -344,    26,
     123,   114,   112,   126,    45,   101,     9,    66,   105,   165,
    1464,  -344,   140,    35,  1413,  -344,  1413,  1413,  1413,    -5,
     191,   360,  -344,   190,  -344,   189,  -344,   127,  1005,   188,
    -344,    92,  1413,  1413,    78,  1413,  1413,   712,   204,  1413,
    -344,   113,   174,    20,  -344,   206,  -344,  -344,  -344,   182,
     200,   199,   163,   218,  1158,  1158,  1158,    92,  -344,  -344,
      44,    92,  -344,    44,  -344,  -344,   -25,  -344,  -344,  -344,
    -344,  -344,   166,   176,   229,  -344,  -344,  -344,  -344,  -344,
    -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,
    -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,
    -344,  -344,  -344,  -344,  -344,  -344,    92,  -344,  -344,  -344,
    -344,  -344,    -4,    20,   240,   162,  1413,    20,  -344,  -344,
    -344,  -344,  -344,  -344,  -344,  -344,  1413,  1413,  1209,  -344,
     -25,  -344,  -344,   200,  -344,   248,  1413,   360,   360,   360,
     360,   360,  -344,  -344,   360,   360,   360,   360,   360,    92,
      92,  -344,   864,  1413,    20,    92,   242,  -344,  -344,   243,
    -344,  -344,   244,  -344,    20,   247,   249,   252,   254,   255,
     257,    20,    20,   814,  -344,   260,  -344,  -344,   250,  -344,
    -344,   161,  -344,    92,   245,    20,   262,   265,   182,    20,
     199,   266,    20,   276,   192,    92,   -28,   278,   267,   269,
    -344,  -344,  -344,  -344,   161,    20,   253,   123,   114,   112,
     126,    45,   101,     9,    66,   105,   165,  -344,   266,    35,
    -344,   198,  1260,  -344,  -344,  1311,  -344,   266,   638,   638,
    1413,  1413,   638,   638,   256,   258,  1362,   199,    20,  -344,
     435,  -344,    20,    20,  -344,  -344,  -344,    92,   275,   280,
    -344,  -344,  -344,    20,   182,  1413,    20,   182,  -344,   360,
     281,  -344,  -344,   282,  -344,    24,   261,  -344,   286,   289,
    -344,  -344,  1413,  1413,   638,   290,  -344,  -344,   288,   435,
     272,  -344,   291,   292,  -344,    20,   199,   266,   241,  -344,
    -344,   304,  -344,  -344,   199,  -344,  -344,   251,   199,   638,
     307,   311,  1572,  1621,  -344,   638,  -344,  -344,  -344,   266,
     266,   310,  -344,    24,   182,  -344,  -344,   259,  -344,  -344,
    -344,  -344,   638,   638,  -344,    24,    24,    92,  -344,  -344,
     303,  -344,  -344,  -344,  -344,  -344,  -344
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    75,   186,     0,     0,     0,     0,   209,   208,    23,
     189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   187,   188,     0,     0,     0,     0,
       0,     0,     0,     0,   185,   184,     0,     0,     0,     0,
       0,     0,   210,     0,     0,   212,   213,    48,   214,   215,
       0,     2,     6,     4,    45,    46,    38,    20,   211,    43,
      39,    40,    42,    41,    36,    37,    44,    35,     0,    76,
     216,   217,    78,     0,   124,     5,     0,    47,   105,   147,
     149,   151,   153,   155,   157,   161,   167,   171,   175,   180,
       0,   182,   191,   195,     0,   203,     0,     0,     0,   211,
       0,     0,   221,     0,   102,     0,    33,     0,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     211,    80,   203,     0,    70,     0,    72,    73,   193,   128,
      28,   190,    11,    16,     0,     0,     0,     0,   107,   139,
       0,     0,     8,     0,    52,   108,     0,   116,   140,    53,
     109,   141,     0,    18,     0,    82,    98,   178,   177,   179,
      86,    87,    88,    89,    90,    85,    83,    91,    92,   201,
     202,    84,   166,   164,   165,   163,   174,   173,   169,   170,
      93,    94,    95,    96,    97,   118,     0,   121,   119,   123,
     122,   120,     0,     0,   197,     0,     0,     0,   135,     1,
       3,    21,   129,    54,   130,    74,     0,     0,     0,   104,
       0,     9,   117,   144,   106,   142,     0,     0,     0,     0,
       0,     0,   159,   160,     0,     0,     0,     0,     0,     0,
       0,   192,     0,     0,     0,     0,     0,   204,   207,     0,
     219,   220,    99,    34,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,    57,    56,     0,    71,
      30,    29,   194,     0,     0,     0,     0,     0,   128,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      77,    79,   103,   145,   146,     0,     0,   150,   152,   154,
     156,   158,   162,   168,   172,   176,   181,   183,     0,   196,
     206,     0,     0,   205,   218,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    10,
      13,    17,     0,     0,   131,    19,   127,   110,     0,     0,
      27,   114,   198,     0,   128,   136,    49,   128,   143,     0,
       0,    81,   199,     0,   101,     0,    68,    59,     0,     0,
      65,    67,     0,     0,     0,     0,    58,    50,     0,    14,
       0,    12,     0,     0,   111,     0,     0,     0,     0,   132,
     137,     0,   134,   148,     0,   200,   112,     0,     0,     0,
       0,     0,     0,     0,    61,     0,    51,    15,     7,     0,
       0,    31,   125,     0,   128,   138,   126,     0,   115,    69,
      60,    66,     0,     0,    62,     0,     0,     0,    24,   133,
       0,    63,    64,    25,    26,    32,   113
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -344,   270,  -295,    -7,  -344,  -344,   -42,  -121,  -187,    48,
    -344,  -344,  -344,   -17,   -82,  -119,   -12,     0,   -10,    -2,
    -344,  -344,  -344,  -344,  -344,  -344,  -344,  -110,   236,  -249,
    -344,   294,   -51,  -116,    42,  -280,  -343,   -15,   -20,  -344,
    -344,  -246,     8,    -6,    46,    43,   -33,    -1,  -344,   117,
     119,   121,   124,  -344,   120,   306,   122,   308,   128,   313,
     131,   319,   133,   134,     4,   -45,  -344,  -192,  -344,   321,
     -16,  -344,   328
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    50,    51,    52,   264,   360,   361,   132,   152,    53,
     111,    54,    55,   194,   364,    56,    57,    99,   358,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
     236,   207,    70,    71,    72,   328,   378,    73,    74,   186,
      75,   204,    76,   278,    77,   214,   215,    78,    79,    80,
      81,    82,    83,   224,    84,   225,    85,   226,    86,   227,
      87,   228,    88,    89,   101,    91,    92,   195,   303,   238,
      93,    94,    95
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,   114,   258,   121,    90,     9,    58,   253,   272,   128,
     279,   147,   262,    58,   261,   143,   146,   118,   340,   120,
     209,   192,   324,   142,   202,   359,   129,   345,   133,   144,
     149,   202,   210,   153,   130,   112,   203,   376,   129,   333,
     129,     9,   237,   301,   129,   231,   130,   -80,   130,   233,
     408,    58,   198,   212,   109,    90,   231,    58,   110,   216,
      39,   113,   413,   414,   359,    28,   211,   140,   210,    47,
     237,   138,   274,   145,   150,   139,   213,   148,   151,   260,
     178,   179,   325,   332,   377,   169,   170,   393,   369,   115,
     120,   372,  -128,    47,   244,   234,    39,   284,    28,  -128,
     140,   120,   217,   382,   383,   201,   144,   116,   222,   405,
     406,   129,   157,   158,   159,   223,   247,    47,   248,   130,
     266,   117,   235,   133,   267,   143,   212,   233,   143,    39,
      28,   146,   140,   142,   176,   177,   211,   129,   138,   145,
     150,   129,   119,   316,   321,   130,   368,   256,   409,   130,
     145,   326,   123,   126,   148,   141,   127,   131,   154,    33,
     193,    39,   283,   169,   170,   172,   173,   174,   175,   271,
     196,   197,   199,   234,   205,   208,   138,   145,   150,    40,
     139,   148,   151,    43,    44,   218,   129,   220,   219,    47,
     147,   306,   273,   133,   130,   146,   229,   153,   356,   221,
     235,   232,   240,   241,   242,   243,   141,   254,   257,   259,
     209,   202,   297,   298,     5,     6,   299,   120,   120,   120,
     120,   120,   263,   265,   120,   120,   120,   120,   120,   129,
     129,   268,   120,   209,   300,   129,   269,   130,   130,   100,
     103,   105,   270,   130,   307,   275,   319,   392,   237,   281,
     276,   314,   315,   285,   302,   396,   304,   125,   305,   398,
     308,   320,   309,   129,   310,   133,   311,   318,   312,   153,
     313,   130,   329,   317,   322,   129,   334,   323,   327,   330,
     331,   335,   337,   130,   336,   213,   341,   339,   366,   388,
     352,   386,   353,   367,   374,   375,   346,   347,   379,   380,
     350,   351,   381,   385,   389,   390,   394,   395,    58,    58,
     400,   397,    58,    58,   401,   407,   416,   387,   357,   410,
      58,   200,   362,   363,    90,   415,   371,   129,   338,   370,
     239,   185,   391,   153,   287,   365,   357,   288,   373,   120,
     289,   291,   384,   187,   290,   188,   292,   122,   245,   246,
     189,   249,   250,   293,    58,   255,   190,   294,   191,    58,
     295,     0,   296,    90,     0,   213,     0,   399,     2,     0,
       3,     0,     4,   404,     5,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,    58,     0,     7,     8,     0,
     411,   412,     0,     0,     0,    10,     0,     0,     0,     0,
       0,     0,    58,    58,     0,     0,     0,   129,     0,     0,
      24,    25,    26,    27,     0,   365,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,    35,
       0,     0,   277,     0,     0,     0,     0,     0,     1,     0,
       0,     0,   280,     2,   105,     3,     0,     4,     0,     5,
      42,     6,   286,     0,     0,    45,    46,    47,    48,    49,
       0,     0,     7,     8,     0,     9,     0,     0,     0,   105,
      10,    11,     0,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,     0,     0,     0,     0,     0,     0,
       0,     0,    33,    34,    35,     0,     0,     0,     0,     0,
       0,     0,    36,    37,    38,     0,     0,     0,     0,     0,
      39,     0,    40,     0,    41,    42,    43,    44,     0,     0,
      45,    46,    47,    48,    49,     0,     0,     0,   343,     0,
       0,   105,     0,     0,     1,     0,   348,   349,     0,     2,
       0,     3,   355,     4,     0,     5,     0,     6,   106,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
       0,   277,     0,     0,     0,     0,    10,    11,     0,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    29,   107,   108,    32,
       0,     0,     0,     0,     0,     0,     0,     0,    33,    34,
      35,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    40,     0,
      41,    42,    43,    44,     0,     0,    45,    46,    47,    48,
      49,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,     0,     5,     0,     6,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    11,     0,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    29,   107,   108,    32,     0,     0,     0,
       0,     0,     0,     0,     0,    33,    34,    35,     0,     0,
       0,     0,     0,     0,     0,     1,     0,    38,     0,     0,
       2,     0,     3,     0,     4,    40,     5,    41,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,     0,     7,
       8,     0,     0,     0,     0,     0,     0,    10,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,    25,    26,    27,   124,    96,     0,    97,
      98,     2,     0,     3,     0,     4,     0,     5,     0,    33,
      34,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,    10,    40,
       0,     0,    42,    43,    44,   251,   252,    45,    46,    47,
      48,    49,     0,    24,    25,    26,    27,     1,    96,     0,
      97,    98,     2,     0,     3,     0,     4,     0,     5,     0,
      33,    34,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      40,     0,     0,    42,    43,    44,     0,     0,    45,    46,
      47,    48,    49,     0,    24,    25,    26,    27,     0,    96,
       0,    97,    98,     0,     3,     0,     4,     0,     5,     0,
       0,    33,    34,    35,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     2,     0,     3,     0,
       4,    40,     5,     0,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,     0,     7,     8,     0,     9,     0,
       0,     0,     0,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    20,     0,     0,     0,    24,    25,
      26,    27,     0,   134,     0,    31,    32,     2,     0,     3,
       0,     4,     0,     5,    42,    33,    34,    35,     0,    45,
      46,    47,    48,    49,     0,     0,     7,     8,     0,     0,
       0,     0,     0,    39,    10,    40,     0,    41,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,     0,    24,
      25,    26,    27,     0,   134,     0,   135,   136,     0,     0,
       0,     0,     0,     0,     0,     0,    33,    34,    35,     0,
       0,     0,     0,     2,     0,     3,     0,     4,   137,     5,
       0,     0,     0,     0,     0,     0,    40,     0,    41,    42,
      43,    44,     7,     8,    45,    46,    47,    48,    49,     0,
      10,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    20,     0,     0,     0,    24,    25,    26,    27,     0,
     134,     0,   108,    32,     2,     0,     3,     0,     4,   102,
       5,     0,    33,    34,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,    40,     0,    41,    42,    43,    44,     0,     0,
      45,    46,    47,    48,    49,     0,    24,    25,    26,    27,
       0,    96,     0,    97,    98,     2,     0,     3,     0,     4,
       0,     5,   104,    33,    34,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
       0,     0,    10,    40,     0,     0,    42,    43,    44,     0,
       0,    45,    46,    47,    48,    49,     0,    24,    25,    26,
      27,     0,    96,     0,    97,    98,     2,     0,     3,     0,
       4,     0,     5,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    40,     0,     0,    42,    43,    44,
       0,     0,    45,    46,    47,    48,    49,     0,    24,    25,
      26,    27,     0,   134,     0,   135,   136,     2,     0,     3,
       0,     4,     0,     5,   282,    33,    34,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,    10,    40,     0,    41,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,     0,    24,
      25,    26,    27,     0,    96,     0,    97,    98,     2,     0,
       3,     0,     4,   342,     5,     0,    33,    34,    35,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    40,     0,     0,    42,
      43,    44,     0,     0,    45,    46,    47,    48,    49,     0,
      24,    25,    26,    27,     0,    96,     0,    97,    98,     2,
       0,     3,     0,     4,     0,     5,   344,    33,    34,    35,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
       0,     0,     0,     0,     0,     0,    10,    40,     0,     0,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
       0,    24,    25,    26,    27,     0,    96,     0,    97,    98,
       2,     0,     3,     0,     4,   354,     5,     0,    33,    34,
      35,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,     0,     0,    10,    40,     0,
       0,    42,    43,    44,     0,     0,    45,    46,    47,    48,
      49,     0,    24,    25,    26,    27,     0,    96,     0,    97,
      98,     2,     0,     3,     0,     4,     0,     5,     0,    33,
      34,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,    10,    40,
       0,     0,    42,    43,    44,     0,     0,    45,    46,    47,
      48,    49,     0,    24,    25,    26,    27,     0,    96,     0,
      97,    98,     2,     0,     3,     0,     4,     0,     5,     0,
      33,    34,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      40,     0,     0,    42,    43,    44,     0,     0,    45,    46,
      47,    48,    49,     0,    24,    25,    26,    27,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   155,
     206,   156,    34,    35,     0,     0,     0,     0,     0,     0,
       0,     0,   230,   160,   161,   162,   163,   164,   165,   166,
     167,   168,     0,   155,    42,   156,   157,   158,   159,    45,
      46,    47,    48,    49,     0,     0,     0,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   155,     0,   156,     0,
       0,     0,     0,     0,     0,   402,     0,     0,   171,     0,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,     0,     0,     0,     0,   180,   181,   182,   183,   184,
       0,     0,   171,   172,   173,   174,   175,   176,   177,     0,
     178,   179,     0,     0,     0,   155,     0,   156,     0,   180,
     181,   182,   183,   184,   403,   171,     0,     0,     0,   160,
     161,   162,   163,   164,   165,   166,   167,   168,     0,     0,
       0,     0,   180,   181,   182,   183,   184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   171,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,   181,   182,   183,   184
};

static const yytype_int16 yycheck[] =
{
       0,    13,   123,    19,     0,    30,     6,   117,    12,    26,
     197,    31,   131,    13,   130,    30,    31,    17,   298,    19,
      71,    38,   268,    30,    29,   320,    26,   307,    28,    31,
      32,    29,    57,    33,    26,    12,    34,    13,    38,    67,
      40,    30,    93,   235,    44,    90,    38,    12,    40,    14,
     393,    51,    44,    73,     6,    51,   101,    57,    55,    33,
      85,    12,   405,   406,   359,    54,    73,    56,    57,    97,
     121,    29,   193,    31,    32,    29,    76,    31,    32,   130,
      71,    72,   269,   275,    60,    50,    51,   367,   334,    12,
      90,   337,    97,    97,   111,    60,    85,   213,    54,    97,
      56,   101,    76,   352,   353,    57,   108,    12,    63,   389,
     390,   111,     7,     8,     9,    70,    38,    97,    40,   111,
     137,    12,    87,   123,   141,   140,   146,    14,   143,    85,
      54,   146,    56,   140,    68,    69,   143,   137,    96,    97,
      98,   141,    12,   253,   265,   137,   333,    34,   394,   141,
     108,   270,    16,     3,   108,    79,     3,    32,    12,    67,
      87,    85,   213,    50,    51,    64,    65,    66,    67,   186,
      16,    12,     0,    60,     3,    14,   134,   135,   136,    87,
     134,   135,   136,    91,    92,    62,   186,    75,    74,    97,
     210,   242,   192,   193,   186,   210,    31,   197,   317,    73,
      87,    61,    11,    13,    15,    17,    79,     3,    34,     3,
     261,    29,   229,   230,    14,    16,   232,   217,   218,   219,
     220,   221,    59,     5,   224,   225,   226,   227,   228,   229,
     230,    65,   232,   284,   234,   235,    60,   229,   230,     3,
       4,     5,    13,   235,   244,     5,   263,   366,   299,   207,
      88,   251,   252,     5,    12,   374,    13,    21,    14,   378,
      13,    16,    13,   263,    12,   265,    12,    17,    13,   269,
      13,   263,   272,    13,    12,   275,   276,    12,    12,     3,
      88,     3,    13,   275,    17,   285,    88,    34,    13,    17,
      34,     3,    34,    13,    13,    13,   308,   309,    37,    13,
     312,   313,    13,    13,    13,    13,    65,     3,   308,   309,
       3,    60,   312,   313,     3,     5,    13,   359,   318,    60,
     320,    51,   322,   323,   320,   407,   336,   327,   285,   335,
      94,    37,   365,   333,   217,   327,   336,   218,   339,   339,
     219,   221,   354,    37,   220,    37,   224,    19,   112,   113,
      37,   115,   116,   225,   354,   119,    37,   226,    37,   359,
     227,    -1,   228,   359,    -1,   365,    -1,   379,     8,    -1,
      10,    -1,    12,   385,    14,    -1,    -1,    -1,    -1,   379,
      -1,    -1,    -1,    -1,    -1,   385,    -1,    27,    28,    -1,
     402,   403,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    -1,   402,   403,    -1,    -1,    -1,   407,    -1,    -1,
      50,    51,    52,    53,    -1,   407,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,
      -1,    -1,   196,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,   206,     8,   208,    10,    -1,    12,    -1,    14,
      90,    16,   216,    -1,    -1,    95,    96,    97,    98,    99,
      -1,    -1,    27,    28,    -1,    30,    -1,    -1,    -1,   233,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    78,    79,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    87,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    96,    97,    98,    99,    -1,    -1,    -1,   302,    -1,
      -1,   305,    -1,    -1,     3,    -1,   310,   311,    -1,     8,
      -1,    10,   316,    12,    -1,    14,    -1,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,   335,    -1,    -1,    -1,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    -1,    55,    56,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    96,    97,    98,
      99,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    55,    56,    57,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    79,    -1,    -1,
       8,    -1,    10,    -1,    12,    87,    14,    89,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,     3,    55,    -1,    57,
      58,     8,    -1,    10,    -1,    12,    -1,    14,    -1,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    87,
      -1,    -1,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,    -1,    50,    51,    52,    53,     3,    55,    -1,
      57,    58,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      87,    -1,    -1,    90,    91,    92,    -1,    -1,    95,    96,
      97,    98,    99,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    57,    58,    -1,    10,    -1,    12,    -1,    14,    -1,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    87,    14,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    -1,    27,    28,    -1,    30,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,    58,     8,    -1,    10,
      -1,    12,    -1,    14,    90,    67,    68,    69,    -1,    95,
      96,    97,    98,    99,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    85,    35,    87,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    12,    79,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    89,    90,
      91,    92,    27,    28,    95,    96,    97,    98,    99,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    52,    53,    -1,
      55,    -1,    57,    58,     8,    -1,    10,    -1,    12,    13,
      14,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    87,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    96,    97,    98,    99,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    57,    58,     8,    -1,    10,    -1,    12,
      -1,    14,    15,    67,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    87,    -1,    -1,    90,    91,    92,    -1,
      -1,    95,    96,    97,    98,    99,    -1,    50,    51,    52,
      53,    -1,    55,    -1,    57,    58,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    67,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    87,    -1,    -1,    90,    91,    92,
      -1,    -1,    95,    96,    97,    98,    99,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,    58,     8,    -1,    10,
      -1,    12,    -1,    14,    15,    67,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    87,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    57,    58,     8,    -1,
      10,    -1,    12,    13,    14,    -1,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    87,    -1,    -1,    90,
      91,    92,    -1,    -1,    95,    96,    97,    98,    99,    -1,
      50,    51,    52,    53,    -1,    55,    -1,    57,    58,     8,
      -1,    10,    -1,    12,    -1,    14,    15,    67,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    87,    -1,    -1,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
      -1,    50,    51,    52,    53,    -1,    55,    -1,    57,    58,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    87,    -1,
      -1,    90,    91,    92,    -1,    -1,    95,    96,    97,    98,
      99,    -1,    50,    51,    52,    53,    -1,    55,    -1,    57,
      58,     8,    -1,    10,    -1,    12,    -1,    14,    -1,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    87,
      -1,    -1,    90,    91,    92,    -1,    -1,    95,    96,    97,
      98,    99,    -1,    50,    51,    52,    53,    -1,    55,    -1,
      57,    58,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      87,    -1,    -1,    90,    91,    92,    -1,    -1,    95,    96,
      97,    98,    99,    -1,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,
       5,     6,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,     4,    90,     6,     7,     8,     9,    95,
      96,    97,    98,    99,    -1,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     4,    -1,     6,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,    63,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    50,
      51,    -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,
      -1,    -1,    63,    64,    65,    66,    67,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,     4,    -1,     6,    -1,    80,
      81,    82,    83,    84,    13,    63,    -1,    -1,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      -1,    -1,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    81,    82,    83,    84
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    30,
      35,    36,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    67,    68,    69,    77,    78,    79,    85,
      87,    89,    90,    91,    92,    95,    96,    97,    98,    99,
     101,   102,   103,   109,   111,   112,   115,   116,   117,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     132,   133,   134,   137,   138,   140,   142,   144,   147,   148,
     149,   150,   151,   152,   154,   156,   158,   160,   162,   163,
     164,   165,   166,   170,   171,   172,    55,    57,    58,   117,
     128,   164,    13,   128,    15,   128,    17,    56,    57,   109,
      55,   110,    12,    12,   116,    12,    12,    12,   117,    12,
     117,   170,   172,    16,     3,   128,     3,     3,   113,   117,
     142,    32,   107,   117,    55,    57,    58,    79,   134,   144,
      56,    79,   103,   137,   119,   134,   137,   138,   144,   119,
     134,   144,   108,   117,    12,     4,     6,     7,     8,     9,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    50,
      51,    63,    64,    65,    66,    67,    68,    69,    71,    72,
      80,    81,    82,    83,    84,   131,   139,   155,   157,   159,
     161,   169,   113,    87,   113,   167,    16,    12,   142,     0,
     101,   109,    29,    34,   141,     3,     5,   131,    14,   132,
      57,   103,   138,   117,   145,   146,    33,    76,    62,    74,
      75,    73,    63,    70,   153,   155,   157,   159,   161,    31,
      78,   165,    61,    14,    60,    87,   130,   132,   169,   128,
      11,    13,    15,    17,   113,   128,   128,    38,    40,   128,
     128,    93,    94,   127,     3,   128,    34,    34,   107,     3,
     132,   133,   115,    59,   104,     5,   113,   113,    65,    60,
      13,   113,    12,   117,   107,     5,    88,   128,   143,   108,
     128,   134,    15,   132,   133,     5,   128,   149,   150,   151,
     152,   154,   156,   158,   160,   162,   163,   113,   113,   170,
     117,   167,    12,   168,    13,    14,   132,   117,    13,    13,
      12,    12,    13,    13,   117,   117,   127,    13,    17,   113,
      16,   107,    12,    12,   141,   108,   115,    12,   135,   117,
       3,    88,   167,    67,   117,     3,    17,    13,   145,    34,
     135,    88,    13,   128,    15,   135,   116,   116,   128,   128,
     116,   116,    34,    34,    13,   128,   115,   117,   118,   102,
     105,   106,   117,   117,   114,   142,    13,    13,   108,   141,
     143,   118,   141,   147,    13,    13,    13,    60,   136,    37,
      13,    13,   129,   129,   116,    13,     3,   106,    17,    13,
      13,   146,   115,   135,    65,     3,   115,    60,   115,   116,
       3,     3,    13,    13,   116,   135,   135,     5,   136,   141,
      60,   116,   116,   136,   136,   114,    13
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   100,   101,   101,   102,   102,   102,   103,   103,   103,
     104,   104,   105,   105,   106,   106,   107,   107,   108,   108,
     109,   109,   110,   110,   111,   111,   111,   112,   113,   113,
     113,   114,   114,   115,   115,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   117,   118,
     118,   119,   119,   119,   120,   121,   122,   122,   123,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   125,   125,
     126,   126,   126,   126,   127,   127,   128,   128,   129,   129,
     130,   130,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   132,
     132,   132,   133,   133,   133,   134,   134,   134,   134,   134,
     135,   135,   136,   136,   137,   138,   138,   138,   139,   139,
     139,   139,   139,   139,   140,   140,   140,   140,   141,   141,
     142,   142,   142,   142,   142,   142,   143,   143,   144,   144,
     144,   144,   145,   145,   146,   146,   146,   147,   147,   148,
     148,   149,   149,   150,   150,   151,   151,   152,   152,   153,
     153,   154,   154,   155,   155,   155,   155,   156,   156,   157,
     157,   158,   158,   159,   159,   160,   160,   161,   161,   161,
     162,   162,   163,   163,   164,   164,   164,   164,   164,   164,
     164,   165,   165,   165,   165,   166,   166,   167,   167,   168,
     168,   169,   169,   170,   170,   170,   170,   170,   171,   171,
     171,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     6,     2,     2,
       2,     0,     1,     0,     1,     2,     1,     3,     1,     3,
       1,     2,     1,     0,     7,     8,     8,     4,     1,     2,
       2,     2,     4,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     6,     2,     2,     2,     3,     3,     3,     5,     5,
       7,     6,     7,     8,     8,     5,     7,     5,     5,     7,
       2,     3,     2,     2,     2,     1,     1,     3,     1,     3,
       0,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       4,     5,     2,     3,     2,     1,     2,     2,     2,     2,
       1,     2,     1,     4,     4,     7,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     6,     6,     4,     0,     1,
       2,     4,     5,     7,     5,     2,     2,     3,     6,     2,
       2,     2,     1,     3,     1,     2,     2,     1,     5,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       1,     1,     3,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     3,     1,     1,     1,     3,     1,     1,     1,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     2,     3,     1,     3,     1,     3,     2,
       3,     1,     1,     1,     2,     3,     3,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
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
    default: /* Avoid compiler warnings. */
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
          case 111: /* func_ptr  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1775 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 112: /* stmt_type  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1781 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 115: /* code_segment  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1787 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 116: /* stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1793 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 119: /* enum_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1799 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 120: /* label_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1805 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 121: /* goto_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1811 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 122: /* case_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1817 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 123: /* switch_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1823 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 124: /* loop_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1829 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 125: /* selection_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1835 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 126: /* jump_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1841 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 127: /* exp_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1847 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 128: /* exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1853 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 129: /* binary_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1859 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 134: /* decl_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1865 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 144: /* union_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1871 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 147: /* con_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1877 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 148: /* log_or_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1883 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 149: /* log_and_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1889 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 150: /* inc_or_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1895 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 151: /* exc_or_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1901 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 152: /* and_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1907 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 154: /* eq_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1913 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 156: /* relational_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1919 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 158: /* shift_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1925 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 160: /* add_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1931 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 162: /* mul_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1937 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 163: /* cast_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1943 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 165: /* unary_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1949 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 166: /* dur_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1955 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 168: /* call_paren  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1961 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 170: /* post_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1967 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 172: /* primary_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1973 "src/ast/parser.c" /* yacc.c:1258  */
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
      yychar = yylex (&yylval, scan);
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
#line 102 "utils/gwion.y" /* yacc.c:1648  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL);  }
#line 2241 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 3:
#line 103 "utils/gwion.y" /* yacc.c:1648  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast)); }
#line 2247 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 4:
#line 107 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list)); }
#line 2253 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 5:
#line 108 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def)); }
#line 2259 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 6:
#line 109 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def)); }
#line 2265 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 7:
#line 114 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_def) = new_class_def(0, (yyvsp[-4].id_list), (yyvsp[-3].type_decl), (yyvsp[-1].class_body)); }
#line 2271 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 8:
#line 115 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_global); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2277 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 9:
#line 117 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].class_def), free_class_def); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2283 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 10:
#line 119 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2289 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 11:
#line 119 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = NULL; }
#line 2295 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 13:
#line 121 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = NULL; }
#line 2301 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 14:
#line 124 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = new_class_body((yyvsp[0].section), NULL); }
#line 2307 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 15:
#line 125 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = new_class_body((yyvsp[-1].section), (yyvsp[0].class_body)); }
#line 2313 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 16:
#line 129 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2319 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 17:
#line 130 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2325 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 18:
#line 134 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2331 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 19:
#line 135 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2337 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 20:
#line 139 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL);}
#line 2343 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 21:
#line 140 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list));}
#line 2349 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 22:
#line 144 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_flag_static;   }
#line 2355 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 23:
#line 145 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 2361 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 24:
#line 150 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_fptr((yyvsp[-3].sym), (yyvsp[-5].type_decl), (yyvsp[-1].arg_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2367 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 25:
#line 152 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_fptr((yyvsp[-3].sym), (yyvsp[-5].type_decl), (yyvsp[-1].arg_list), (yyvsp[0].ival) | ae_flag_static, get_pos(arg)); }
#line 2373 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 26:
#line 154 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_fptr((yyvsp[-3].sym), (yyvsp[-5].type_decl), (yyvsp[-1].arg_list), (yyvsp[0].ival) | ae_flag_global, get_pos(arg)); }
#line 2379 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 27:
#line 159 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_type((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2385 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 29:
#line 163 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub)); }
#line 2391 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 30:
#line 164 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub)); }
#line 2397 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 31:
#line 168 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL); }
#line 2403 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 32:
#line 169 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list)); }
#line 2409 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 33:
#line 173 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_code(NULL, get_pos(arg)); }
#line 2415 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 34:
#line 174 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_code((yyvsp[-1].stmt_list), get_pos(arg)); }
#line 2421 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 48:
#line 193 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.sym) = insert_symbol((yyvsp[0].sval)); }
#line 2427 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 49:
#line 195 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.sym) = NULL; }
#line 2433 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 51:
#line 198 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sym), get_pos(arg)); }
#line 2439 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 52:
#line 199 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2445 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 53:
#line 200 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2451 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 54:
#line 204 "utils/gwion.y" /* yacc.c:1648  */
    {  (yyval.stmt) = new_stmt_jump((yyvsp[-1].sym), 1, get_pos(arg)); }
#line 2457 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 55:
#line 208 "utils/gwion.y" /* yacc.c:1648  */
    {  (yyval.stmt) = new_stmt_jump((yyvsp[-1].sym), 0, get_pos(arg)); }
#line 2463 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 56:
#line 212 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_case, (yyvsp[-1].exp), get_pos(arg)); }
#line 2469 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 57:
#line 213 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_case, (yyvsp[-1].exp), get_pos(arg)); }
#line 2475 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 58:
#line 217 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg));}
#line 2481 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 59:
#line 222 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_while, (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2487 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 60:
#line 224 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_while, (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2493 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 61:
#line 226 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2499 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 62:
#line 228 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2505 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 63:
#line 230 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2511 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 64:
#line 232 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); (yyval.stmt)->d.stmt_auto.is_ptr = 1;}
#line 2517 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 65:
#line 234 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_until, (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2523 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 66:
#line 236 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_until, (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2529 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 67:
#line 238 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_loop((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2535 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 68:
#line 243 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(arg)); }
#line 2541 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 69:
#line 245 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(arg)); }
#line 2547 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 70:
#line 249 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_return, NULL, get_pos(arg)); }
#line 2553 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 71:
#line 250 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_return, (yyvsp[-1].exp), get_pos(arg)); }
#line 2559 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 72:
#line 251 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt(ae_stmt_break, get_pos(arg)); }
#line 2565 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 73:
#line 252 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt(ae_stmt_continue, get_pos(arg)); }
#line 2571 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 74:
#line 256 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_exp, (yyvsp[-1].exp),   get_pos(arg)); }
#line 2577 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 75:
#line 257 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_exp, NULL, get_pos(arg)); }
#line 2583 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 77:
#line 262 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = prepend_exp((yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 2589 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 79:
#line 267 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2595 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 80:
#line 270 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = NULL; }
#line 2601 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 81:
#line 270 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2607 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 82:
#line 272 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_chuck; }
#line 2613 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 83:
#line 272 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_unchuck; }
#line 2619 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 84:
#line 272 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_eq; }
#line 2625 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 85:
#line 273 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_at_chuck; }
#line 2631 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 86:
#line 273 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plus_chuck; }
#line 2637 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 87:
#line 274 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minus_chuck; }
#line 2643 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 88:
#line 274 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_times_chuck; }
#line 2649 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 89:
#line 275 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_divide_chuck; }
#line 2655 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 90:
#line 275 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2661 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 91:
#line 276 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_trig; }
#line 2667 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 92:
#line 276 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_untrig; }
#line 2673 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 93:
#line 277 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsl; }
#line 2679 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 94:
#line 277 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsr; }
#line 2685 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 95:
#line 277 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsand; }
#line 2691 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 96:
#line 278 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsor; }
#line 2697 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 97:
#line 278 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsxor; }
#line 2703 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 98:
#line 279 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_assign; }
#line 2709 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 99:
#line 283 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = new_array_sub((yyvsp[-1].exp), get_pos(arg)); }
#line 2715 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 100:
#line 284 "utils/gwion.y" /* yacc.c:1648  */
    { if((yyvsp[-2].exp)->next){ gwion_error(arg, "invalid format for array init [...][...]..."); free_exp((yyvsp[-2].exp)); free_array_sub((yyvsp[0].array_sub)); YYERROR; } (yyval.array_sub) = prepend_array_sub((yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2721 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 101:
#line 285 "utils/gwion.y" /* yacc.c:1648  */
    { gwion_error(arg, "partially empty array init [...][]..."); free_exp((yyvsp[-3].exp)); YYERROR; }
#line 2727 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 102:
#line 289 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(arg)); }
#line 2733 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 103:
#line 290 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2739 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 104:
#line 291 "utils/gwion.y" /* yacc.c:1648  */
    { gwion_error(arg, "partially empty array init [][...]"); free_array_sub((yyvsp[-1].array_sub)); free_array_sub((yyvsp[0].array_sub)); YYERROR; }
#line 2745 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 106:
#line 296 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2751 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 107:
#line 298 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_static); (yyval.exp) = (yyvsp[0].exp); }
#line 2757 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 108:
#line 300 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_private); (yyval.exp) = (yyvsp[0].exp); }
#line 2763 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 109:
#line 302 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_protect); (yyval.exp) = (yyvsp[0].exp); }
#line 2769 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 110:
#line 306 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = NULL; }
#line 2775 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 111:
#line 307 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = (yyvsp[0].arg_list); }
#line 2781 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 112:
#line 311 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 2787 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 113:
#line 312 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2793 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 114:
#line 315 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2799 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 115:
#line 319 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-4].type_decl), (yyvsp[-3].sym), (yyvsp[-2].arg_list), (yyvsp[0].stmt), (yyvsp[-5].ival) | (yyvsp[-1].ival)); }
#line 2805 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 116:
#line 321 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_private); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2811 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 117:
#line 323 "utils/gwion.y" /* yacc.c:1648  */
    { //CHECK_TEMPLATE(arg, $1, $2, free_func_def);

      if((yyvsp[0].func_def)->tmpl) {
        free_id_list((yyvsp[-1].id_list));
        free_func_def((yyvsp[0].func_def));
        gwion_error(arg, "double template decl");
        YYERROR;
      }
      (yyvsp[0].func_def)->tmpl = new_tmpl_list((yyvsp[-1].id_list), -1);
      (yyval.func_def) = (yyvsp[0].func_def); SET_FLAG((yyval.func_def), ae_flag_template);
    }
#line 2827 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 125:
#line 339 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-3].type_decl), OP_SYM((yyvsp[-4].ival)), (yyvsp[-2].arg_list), (yyvsp[0].stmt), ae_flag_op); }
#line 2833 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 126:
#line 341 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-3].type_decl), OP_SYM((yyvsp[-5].ival)), (yyvsp[-2].arg_list), (yyvsp[0].stmt), ae_flag_op | ae_flag_unary); }
#line 2839 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 127:
#line 343 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def(new_type_decl(new_id_list(insert_symbol("void"), get_pos(arg)), 0,
      get_pos(arg)), insert_symbol("dtor"), NULL, (yyvsp[0].stmt), ae_flag_dtor); }
#line 2846 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 128:
#line 347 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 2852 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 129:
#line 347 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_flag_ref; }
#line 2858 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 130:
#line 350 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)), (yyvsp[0].ival), get_pos(arg)); }
#line 2864 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 131:
#line 351 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2870 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 132:
#line 352 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)),
      (yyvsp[0].ival), get_pos(arg)); (yyval.type_decl)->types = (yyvsp[-3].type_list); }
#line 2877 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 133:
#line 354 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg));
      (yyval.type_decl)->types = (yyvsp[-5].type_list); }
#line 2884 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 134:
#line 356 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2890 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 135:
#line 357 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].type_decl), ae_flag_const); (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2896 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 136:
#line 361 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2902 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 137:
#line 362 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2908 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 138:
#line 366 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(arg));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sym); }
#line 2914 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 139:
#line 368 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2920 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 140:
#line 370 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2926 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 141:
#line 372 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2932 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 142:
#line 376 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL); }
#line 2938 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 143:
#line 377 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list)); }
#line 2944 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 144:
#line 381 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sym), NULL, get_pos(arg)); }
#line 2950 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 145:
#line 382 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2956 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 146:
#line 383 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2962 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 148:
#line 387 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_if((yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 2968 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 150:
#line 390 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(arg)); }
#line 2974 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 152:
#line 393 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(arg)); }
#line 2980 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 154:
#line 396 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(arg)); }
#line 2986 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 156:
#line 399 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(arg)); }
#line 2992 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 158:
#line 402 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(arg)); }
#line 2998 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 159:
#line 404 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_eq; }
#line 3004 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 160:
#line 404 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_neq; }
#line 3010 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 162:
#line 407 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3016 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 163:
#line 409 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_lt; }
#line 3022 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 164:
#line 409 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_gt; }
#line 3028 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 165:
#line 410 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_le; }
#line 3034 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 166:
#line 410 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_ge; }
#line 3040 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 168:
#line 413 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3046 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 169:
#line 416 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_shift_left;  }
#line 3052 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 170:
#line 417 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_shift_right; }
#line 3058 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 172:
#line 421 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3064 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 173:
#line 423 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plus; }
#line 3070 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 174:
#line 423 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minus; }
#line 3076 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 176:
#line 426 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3082 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 177:
#line 428 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_times; }
#line 3088 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 178:
#line 428 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_divide; }
#line 3094 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 179:
#line 428 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_percent; }
#line 3100 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 181:
#line 431 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3106 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 183:
#line 434 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_cast((yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(arg)); }
#line 3112 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 184:
#line 436 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plus; }
#line 3118 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 185:
#line 436 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minus; }
#line 3124 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 186:
#line 436 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_times; }
#line 3130 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 187:
#line 437 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plusplus; }
#line 3136 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 188:
#line 437 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minusminus; }
#line 3142 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 189:
#line 438 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_exclamation; }
#line 3148 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 190:
#line 438 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_spork; }
#line 3154 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 192:
#line 442 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_unary((yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3160 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 193:
#line 444 "utils/gwion.y" /* yacc.c:1648  */
    {
      if((yyvsp[0].type_decl)->array && !(yyvsp[0].type_decl)->array->exp) {
        free_type_decl((yyvsp[0].type_decl));
        gwion_error(arg, "can't use empty '[]' in 'new' expression");
        YYERROR;
      }
      (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), get_pos(arg));
    }
#line 3173 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 194:
#line 453 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_unary3(op_spork, (yyvsp[0].stmt), get_pos(arg)); }
#line 3179 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 196:
#line 456 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_dur((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3185 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 197:
#line 459 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = new_type_list((yyvsp[0].type_decl), NULL); }
#line 3191 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 198:
#line 460 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = new_type_list((yyvsp[-2].type_decl), (yyvsp[0].type_list)); }
#line 3197 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 199:
#line 463 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = NULL; }
#line 3203 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 200:
#line 463 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3209 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 201:
#line 465 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plusplus; }
#line 3215 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 202:
#line 465 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minusminus; }
#line 3221 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 204:
#line 468 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_array((yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(arg)); }
#line 3227 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 205:
#line 470 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_call((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg));
      if((yyvsp[-1].type_list))(yyval.exp)->d.exp_func.tmpl = new_tmpl_call((yyvsp[-1].type_list)); }
#line 3234 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 206:
#line 473 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_dot((yyvsp[-2].exp), (yyvsp[0].sym), get_pos(arg)); }
#line 3240 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 207:
#line 475 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_post((yyvsp[-1].exp), (yyvsp[0].ival), get_pos(arg)); }
#line 3246 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 208:
#line 478 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_complex; }
#line 3252 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 209:
#line 479 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_polar;   }
#line 3258 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 210:
#line 480 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_vec;     }
#line 3264 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 211:
#line 483 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_id(     (yyvsp[0].sym), get_pos(arg)); }
#line 3270 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 212:
#line 484 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_int(    (yyvsp[0].ival), get_pos(arg)); }
#line 3276 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 213:
#line 485 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_float(  (yyvsp[0].fval), get_pos(arg)); }
#line 3282 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 214:
#line 486 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_string( (yyvsp[0].sval), get_pos(arg)); }
#line 3288 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 215:
#line 487 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_char(   (yyvsp[0].sval), get_pos(arg)); }
#line 3294 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 216:
#line 488 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3300 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 217:
#line 489 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3306 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 218:
#line 490 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_vec((yyvsp[-2].ival), (yyvsp[-1].exp), get_pos(arg)); }
#line 3312 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 219:
#line 491 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_hack(   (yyvsp[-1].exp), get_pos(arg)); }
#line 3318 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 220:
#line 492 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) =                      (yyvsp[-1].exp);                }
#line 3324 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 221:
#line 493 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_nil(        get_pos(arg)); }
#line 3330 "src/ast/parser.c" /* yacc.c:1648  */
    break;


#line 3334 "src/ast/parser.c" /* yacc.c:1648  */
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
#line 495 "utils/gwion.y" /* yacc.c:1907  */

