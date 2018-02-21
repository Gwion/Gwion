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
#line 5 "utils/gwion.y" /* yacc.c:339  */

#include "absyn.h"
#include "scanner.h"
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
int get_pos(Scanner*);

m_str op2str(Operator op);

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
    EXTENDS = 313,
    DOT = 314,
    COLONCOLON = 315,
    AND = 316,
    EQ = 317,
    GE = 318,
    GT = 319,
    LE = 320,
    LT = 321,
    MINUS = 322,
    PLUS = 323,
    NEQ = 324,
    SHIFT_LEFT = 325,
    SHIFT_RIGHT = 326,
    S_AND = 327,
    S_OR = 328,
    S_XOR = 329,
    OR = 330,
    AST_DTOR = 331,
    OPERATOR = 332,
    TYPEDEF = 333,
    RSL = 334,
    RSR = 335,
    RSAND = 336,
    RSOR = 337,
    RSXOR = 338,
    TEMPLATE = 339,
    NOELSE = 340,
    LTB = 341,
    GTB = 342,
    VARARG = 343,
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
#define EXTENDS 313
#define DOT 314
#define COLONCOLON 315
#define AND 316
#define EQ 317
#define GE 318
#define GT 319
#define LE 320
#define LT 321
#define MINUS 322
#define PLUS 323
#define NEQ 324
#define SHIFT_LEFT 325
#define SHIFT_RIGHT 326
#define S_AND 327
#define S_OR 328
#define S_XOR 329
#define OR 330
#define AST_DTOR 331
#define OPERATOR 332
#define TYPEDEF 333
#define RSL 334
#define RSR 335
#define RSAND 336
#define RSOR 337
#define RSXOR 338
#define TEMPLATE 339
#define NOELSE 340
#define LTB 341
#define GTB 342
#define VARARG 343
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
  S_Symbol sym;
  Polar* polar;
  Vec* vec;
  Array_Sub array_sub;
  Var_Decl var_decl;
  Var_Decl_List var_decl_list;
  Type_Decl* type_decl;
  Exp   exp;
  Stmt_Ptr func_ptr;
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

#line 358 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (Scanner* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 374 "src/ast/parser.c" /* yacc.c:358  */

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
#define YYFINAL  195
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1789

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  100
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  74
/* YYNRULES -- Number of rules.  */
#define YYNRULES  218
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  398

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
       0,   109,   109,   110,   114,   115,   116,   120,   122,   123,
     126,   126,   128,   128,   131,   132,   136,   137,   141,   142,
     146,   147,   151,   152,   156,   157,   161,   162,   168,   172,
     173,   174,   178,   179,   183,   184,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   203,
     205,   205,   208,   209,   213,   217,   221,   222,   226,   230,
     232,   234,   236,   238,   240,   242,   244,   246,   251,   253,
     258,   259,   260,   261,   265,   266,   270,   271,   275,   276,
     279,   279,   281,   281,   281,   282,   282,   283,   283,   284,
     284,   285,   285,   286,   286,   286,   287,   287,   288,   292,
     293,   294,   298,   299,   300,   304,   305,   306,   308,   313,
     314,   317,   320,   322,   324,   337,   337,   337,   337,   337,
     337,   339,   340,   342,   344,   349,   349,   352,   353,   354,
     356,   358,   359,   363,   364,   368,   369,   371,   376,   377,
     381,   382,   383,   386,   387,   389,   389,   392,   392,   395,
     395,   398,   398,   401,   401,   404,   404,   407,   407,   409,
     409,   412,   412,   413,   413,   415,   415,   419,   420,   423,
     423,   426,   426,   428,   428,   431,   431,   431,   433,   433,
     436,   436,   439,   439,   439,   440,   440,   441,   441,   444,
     444,   446,   455,   458,   458,   462,   463,   466,   466,   468,
     468,   470,   470,   472,   475,   477,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494
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
  "STATIC", "PUBLIC", "PRIVATE", "EXTENDS", "DOT", "COLONCOLON", "AND",
  "EQ", "GE", "GT", "LE", "LT", "MINUS", "PLUS", "NEQ", "SHIFT_LEFT",
  "SHIFT_RIGHT", "S_AND", "S_OR", "S_XOR", "OR", "AST_DTOR", "OPERATOR",
  "TYPEDEF", "RSL", "RSR", "RSAND", "RSOR", "RSXOR", "TEMPLATE", "NOELSE",
  "LTB", "GTB", "VARARG", "UNION", "ATPAREN", "TYPEOF", "CONST", "AUTO",
  "AUTO_PTR", "NUM", "FLOAT", "ID", "STRING_LIT", "CHAR_LIT", "$accept",
  "ast", "section", "class_def", "class_ext", "class_body", "class_body2",
  "id_list", "id_dot", "stmt_list", "static_decl", "function_decl",
  "func_ptr", "stmt_typedef", "type_decl2", "arg_list", "code_segment",
  "stmt", "id", "opt_id", "enum_stmt", "label_stmt", "goto_stmt",
  "case_stmt", "switch_stmt", "loop_stmt", "selection_stmt", "jump_stmt",
  "exp_stmt", "exp", "binary_exp", "template", "op", "array_exp",
  "array_empty", "decl_exp", "func_args", "decl_template", "func_def_base",
  "op_op", "func_def", "atsym", "type_decl", "decl_list", "union_stmt",
  "var_decl_list", "var_decl", "polar_exp", "vec_exp", "con_exp",
  "log_or_exp", "log_and_exp", "inc_or_exp", "exc_or_exp", "and_exp",
  "eq_op", "eq_exp", "rel_op", "relational_exp", "shift_op", "shift_exp",
  "add_op", "add_exp", "mul_op", "mul_exp", "cast_exp", "unary_op",
  "unary_exp", "dur_exp", "type_list", "call_paren", "post_op", "post_exp",
  "primary_exp", YY_NULLPTR
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

#define YYPACT_NINF -287

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-287)))

#define YYTABLE_NINF -126

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     446,  -287,  -287,  1460,  1110,  1160,   555,  1460,  1460,  -287,
    -287,    23,    29,   652,    34,    36,    44,   -38,    55,    60,
      64,   798,    73,    81,  -287,  -287,   222,    62,   -38,   987,
       6,   937,   -38,  -287,  -287,    95,  1612,   222,    30,   222,
    -287,    99,  1460,   106,   222,  -287,  -287,  -287,  -287,  -287,
     122,   446,  -287,  -287,    69,  -287,  -287,  -287,   652,     9,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,   116,
     321,  -287,   112,  -287,   191,  -287,  -287,   -38,  -287,  -287,
    -287,  -287,    17,    66,    65,    54,    70,    -5,   287,    26,
      45,   167,   110,  1510,  -287,    77,   150,  -287,  1460,  1460,
      11,   135,  1561,  -287,   134,  -287,   137,  -287,  1060,   132,
     141,   148,  1460,  1460,    51,  1460,  1460,   725,   160,  1460,
    -287,   119,   131,   -38,  -287,   164,  -287,  -287,  -287,   139,
     157,   156,   121,   175,  1210,   222,  -287,  -287,  -287,  -287,
       6,  1210,  -287,  -287,    -3,  -287,  -287,   118,   124,   171,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,   222,  -287,  -287,  -287,  -287,  -287,     8,   -38,   185,
     104,  1460,   179,   -38,  -287,  -287,  -287,  -287,   222,  -287,
    -287,  -287,  -287,  -287,  1460,  1460,  1260,  -287,    -3,  -287,
    -287,   157,  -287,   189,  1460,  1561,  1561,  1561,  1561,  1561,
    -287,  -287,  1561,  1561,  1561,  1561,  1561,   222,   222,  -287,
      60,  1460,   -38,   222,   184,  -287,  -287,  -287,  -287,   188,
    -287,  -287,  -287,   186,   190,   192,   199,   200,   216,   -38,
     -38,   864,  -287,   218,  -287,  -287,   217,  -287,  -287,   112,
    -287,   222,   219,   -38,   229,   139,   -38,   156,   230,   -38,
     243,   168,   222,   -32,   256,   245,  -287,   247,   -38,  -287,
    -287,  -287,  -287,   112,   -38,   231,    66,    65,    54,    70,
      -5,   287,    26,    45,   167,   110,  -287,   230,   150,  -287,
     177,  1310,  -287,  1360,  -287,   652,   652,  1460,  1460,   652,
     652,   233,   234,  1410,   156,   -38,  -287,   446,  -287,  -287,
    -287,  -287,    12,   156,   258,  -287,  -287,  -287,   -38,   139,
    1460,   -38,   139,   230,  -287,  1561,   156,  -287,  -287,   261,
    -287,   239,  -287,   267,   268,  -287,  -287,  1460,  1460,   652,
     270,  -287,  -287,   282,   446,   269,  -287,  -287,   274,   -38,
    -287,   230,   225,  -287,  -287,   289,  -287,   156,  -287,  -287,
    -287,   652,   290,   293,  1683,  1706,  -287,   652,  -287,  -287,
    -287,  -287,   285,  -287,   139,  -287,  -287,  -287,  -287,  -287,
     652,   652,  -287,   222,  -287,  -287,  -287,  -287
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    75,   184,     0,     0,     0,     0,     0,     0,    24,
     187,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   185,   186,     0,     0,     0,     0,
       0,     0,     0,   183,   182,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,   207,   208,    49,   209,   210,
       0,     2,     6,     4,    23,    46,    47,    39,    20,   206,
      44,    40,    41,    43,    42,    37,    38,    45,    36,     0,
      76,   211,   212,    78,     0,   121,     5,     0,    48,   214,
     215,   105,   145,   147,   149,   151,   153,   155,   159,   165,
     169,   173,   178,     0,   180,   189,   193,   201,     0,     0,
     206,     0,     0,   218,     0,   102,     0,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     206,    80,   201,     0,    70,     0,    72,    73,   191,   125,
      29,   188,    11,    16,     0,     0,    27,   107,   136,     8,
       0,     0,    53,   108,     0,   113,   137,     0,    18,     0,
      82,    98,   176,   175,   177,    86,    87,    88,    89,    90,
      85,    83,    91,    92,   199,   200,    84,   164,   162,   163,
     161,   172,   171,   167,   168,    93,    94,    95,    96,    97,
     115,     0,   118,   116,   120,   119,   117,     0,     0,   195,
       0,     0,     0,     0,   132,     1,     3,    22,     0,    21,
     126,    54,   127,    74,     0,     0,     0,   104,     0,     9,
     114,   140,   106,   138,     0,     0,     0,     0,     0,     0,
     157,   158,     0,     0,     0,     0,     0,     0,     0,   190,
       0,     0,     0,     0,     0,   202,   205,   216,   217,    99,
      35,   143,   213,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,    57,    56,     0,    71,    31,    30,
     192,     0,     0,     0,     0,   125,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   144,     0,     0,    77,
      79,   103,   141,   142,     0,     0,   148,   150,   152,   154,
     156,   160,   166,   170,   174,   179,   181,     0,   194,   204,
       0,     0,   203,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    10,    13,    17,   128,
      19,   124,     0,     0,     0,    28,   111,   196,     0,   125,
     133,    50,   125,     0,   139,     0,     0,    81,   197,     0,
     101,    68,    59,     0,     0,    65,    67,     0,     0,     0,
       0,    58,    51,     0,    14,     0,    12,   109,     0,     0,
     122,     0,     0,   129,   134,     0,   131,     0,   146,   123,
     198,     0,     0,     0,     0,     0,    61,     0,    52,    15,
       7,   110,    32,    26,   125,   135,   112,    69,    60,    66,
       0,     0,    62,     0,   130,    63,    64,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -287,   251,  -268,    -8,  -287,  -287,   -54,  -119,  -183,    15,
    -287,  -287,   275,  -287,    16,   -90,  -128,   -11,     0,   -24,
      -7,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -103,   249,
    -248,  -287,   276,    -1,  -100,    32,  -286,   -23,   -19,  -287,
    -287,  -250,   -21,   -14,     2,    27,   -39,  -287,  -287,   -12,
    -287,   109,   105,   113,   111,  -287,   114,   296,   115,   298,
     125,   300,   133,   320,   138,   143,     1,   -57,  -287,  -204,
    -287,   322,   -10,   348
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    50,    51,    52,   262,   355,   356,   132,   147,    53,
     198,    54,    55,    56,   189,   358,    57,    58,   100,   353,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,   234,   205,    71,    72,    73,   323,    74,    75,   181,
      76,   202,    77,   275,    78,   212,   213,    79,    80,    81,
      82,    83,    84,    85,    86,   222,    87,   223,    88,   224,
      89,   225,    90,   226,    91,    92,   102,    94,    95,   190,
     302,   236,    96,    97
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      59,    93,   114,   260,   256,   130,    59,   140,   144,   121,
     277,   336,   145,    59,   251,   319,   130,   118,   130,   120,
     269,   109,   139,   194,   142,   357,   129,     9,   133,   300,
     259,   138,   148,   146,   328,   112,   229,   129,   200,   129,
     200,   113,   128,   201,   129,   229,   115,   367,   116,   354,
     214,    59,    93,   187,   208,   210,   117,   220,    59,    47,
      28,   137,    30,   143,   221,    47,   209,   119,   327,   271,
       3,   207,     4,   199,     5,   383,   126,   211,    32,   363,
     123,    38,   366,   320,   127,    40,   354,     7,     8,   245,
      38,   246,   215,   120,   131,   235,   173,   174,    39,   374,
     375,   142,   120,    43,    44,    47,  -125,   149,  -125,    47,
     146,   283,   171,   172,   130,   191,   188,   140,   193,   203,
     235,   144,   195,   133,   197,   210,   206,   216,   218,   258,
     137,   143,   209,   231,   394,   129,   146,   230,   217,   321,
     143,   227,   219,   138,   318,   362,   237,   238,   313,   240,
      42,   264,   239,   254,   241,    45,    46,    47,    48,    49,
     130,   242,   -80,   252,   231,   255,   143,   257,   200,   164,
     165,     5,     6,   137,   152,   153,   154,   130,   232,   261,
     263,   129,   265,   266,   267,   144,   351,   270,   133,   145,
     272,   273,   276,   148,   284,   360,   301,   268,   129,   305,
     164,   165,   303,   306,   307,   233,   130,   130,   369,   232,
     282,   308,   130,   309,   278,   120,   120,   120,   120,   120,
     298,     9,   120,   120,   120,   120,   120,   129,   129,   310,
     120,   314,   299,   129,   315,   317,   233,   280,   304,   386,
     130,   269,   322,   296,   297,    28,   325,    30,   208,   311,
     312,   130,   101,   104,   106,   326,   110,   111,   207,   330,
     332,   129,   331,   133,   337,   335,   148,   347,   348,   324,
     125,   361,   129,   329,   370,    38,   371,   316,   333,    40,
     372,   373,   207,   377,   211,   378,   380,   381,    32,   384,
     393,   192,   385,   388,   341,   342,   389,   235,   345,   346,
     379,   359,   196,   397,   136,    59,    59,   365,    39,    59,
      59,   334,   180,    43,    44,   352,   364,    59,    93,    47,
     382,   287,   129,   368,   286,   150,   204,   151,   148,   289,
     288,   352,   182,   290,   183,   120,   184,   291,   376,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   292,    59,
     167,   168,   169,   170,    59,    93,   185,   293,   186,   211,
     387,   243,   244,   294,   247,   248,   392,   122,   253,   295,
       0,    59,   359,     0,     0,     0,     0,    59,     0,   395,
     396,     0,     0,   166,     0,     0,     0,     0,     0,     0,
      59,    59,     0,   129,     0,     0,     0,     0,     0,     0,
     175,   176,   177,   178,   179,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       0,     0,     0,   279,     2,   106,     3,     0,     4,     0,
       5,     0,     6,   285,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     9,     0,     0,     0,
     106,    10,    11,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,     0,     0,     0,     0,     0,     0,
       0,     0,    32,    33,    34,     0,     0,     0,     0,     0,
       0,     0,    35,    36,    37,     0,     0,     0,     0,     0,
      38,     0,    39,     0,    40,    41,    42,    43,    44,     0,
       0,    45,    46,    47,    48,    49,     0,     0,     0,     0,
     339,     0,   106,     0,     0,     0,   343,   344,     1,     0,
       0,     0,   350,     2,     0,     3,     0,     4,     0,     5,
       0,     6,   107,     0,     0,     0,     0,     0,     0,   274,
       0,     0,     7,     8,     0,     0,     0,     0,     0,     0,
      10,    11,     0,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      29,     0,   108,     0,     0,     0,     0,     0,     0,     0,
       0,    32,    33,    34,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,     0,     0,     0,     0,     0,
       0,    39,     0,     0,    41,    42,    43,    44,     0,     0,
      45,    46,    47,    48,    49,     1,     0,     0,     0,     0,
       2,     0,     3,     0,     4,     0,     5,     0,     6,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,     0,     0,    10,    11,     0,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    29,     0,   108,
       0,     0,     0,     0,     0,     0,     0,     0,    32,    33,
      34,     0,     0,     0,     0,     0,     0,     0,     1,     0,
      37,     0,     0,     2,     0,     3,     0,     4,    39,     5,
       0,    41,    42,    43,    44,     0,     0,    45,    46,    47,
      48,    49,     7,     8,     0,     0,     0,     0,     0,     0,
      10,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    24,    25,    26,    27,     0,
      98,     0,    99,     0,     0,     0,     0,     0,     0,     0,
       0,    32,    33,    34,     0,     0,     0,     0,     0,     0,
       0,   124,     0,     0,     0,     0,     2,     0,     3,     0,
       4,    39,     5,     0,     0,    42,    43,    44,   249,   250,
      45,    46,    47,    48,    49,     7,     8,     0,     0,     0,
       0,     0,     0,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,    25,
      26,    27,     0,    98,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,    32,    33,    34,     1,     0,     0,
       0,     0,     2,     0,     3,     0,     4,     0,     5,     0,
       0,     0,     0,     0,    39,     0,     0,     0,    42,    43,
      44,     7,     8,    45,    46,    47,    48,    49,     0,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    25,    26,    27,     0,    98,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     2,     0,     3,     0,     4,
      39,     5,     0,     0,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,     7,     8,     0,     9,     0,     0,
       0,     0,    10,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    20,     0,     0,     0,    24,    25,    26,
      27,     0,   141,     0,    31,     2,     0,     3,     0,     4,
       0,     5,     0,    32,    33,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
       0,    38,    10,    39,     0,    40,    41,    42,    43,    44,
       0,     0,    45,    46,    47,    48,    49,    24,    25,    26,
      27,     0,    29,     0,   134,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   135,     0,     0,     2,     0,
       3,     0,     4,    39,     5,     0,    41,    42,    43,    44,
       0,     0,    45,    46,    47,    48,    49,     7,     8,     0,
       0,     0,     0,     0,     0,    10,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    20,     0,     0,     0,
      24,    25,    26,    27,     0,   141,     0,   108,     2,     0,
       3,     0,     4,   103,     5,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    39,     0,     0,    41,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
      24,    25,    26,    27,     0,    98,     0,    99,     2,     0,
       3,     0,     4,     0,     5,   105,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    39,     0,     0,     0,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
      24,    25,    26,    27,     0,    98,     0,    99,     2,     0,
       3,     0,     4,     0,     5,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    39,     0,     0,     0,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
      24,    25,    26,    27,     0,   141,     0,   134,     2,     0,
       3,     0,     4,     0,     5,   281,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    39,     0,     0,    41,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
      24,    25,    26,    27,     0,    98,     0,    99,     2,     0,
       3,     0,     4,   338,     5,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    39,     0,     0,     0,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
      24,    25,    26,    27,     0,    98,     0,    99,     2,     0,
       3,     0,     4,     0,     5,   340,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    39,     0,     0,     0,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
      24,    25,    26,    27,     0,    98,     0,    99,     2,     0,
       3,     0,     4,   349,     5,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    39,     0,     0,     0,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
      24,    25,    26,    27,     0,    98,     0,    99,     2,     0,
       3,     0,     4,     0,     5,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    39,     0,     0,     0,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
      24,    25,    26,    27,     0,    98,     0,    99,     2,     0,
       3,     0,     4,     0,     5,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    39,     0,     0,     0,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
      24,    25,    26,    27,     0,     0,     0,     0,     0,     2,
       0,     3,     0,     4,     0,     5,     0,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,   228,     7,     8,
       0,     0,     0,     0,     0,     0,    10,     0,     0,     0,
      42,     0,     0,     0,     0,    45,    46,    47,    48,    49,
       0,    24,    25,    26,    27,     0,   150,     0,   151,   152,
     153,   154,     0,     0,     0,     0,     0,     0,    33,    34,
     155,   156,   157,   158,   159,   160,   161,   162,   163,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    42,     0,     0,     0,     0,    45,    46,    47,    48,
      49,     0,   164,   165,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   166,   167,   168,   169,   170,   171,
     172,     0,   173,   174,     0,     0,     0,   150,     0,   151,
       0,   175,   176,   177,   178,   179,   390,     0,     0,     0,
       0,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     150,     0,   151,     0,     0,     0,     0,     0,     0,   391,
       0,     0,     0,     0,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   166,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   175,   176,   177,   178,   179,     0,   166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,   176,   177,   178,   179
};

static const yytype_int16 yycheck[] =
{
       0,     0,    13,   131,   123,    26,     6,    30,    31,    19,
     193,   297,    31,    13,   117,   265,    37,    17,    39,    19,
      12,     6,    30,    44,    31,    13,    26,    30,    28,   233,
     130,    29,    32,    31,    66,    12,    93,    37,    29,    39,
      29,    12,    26,    34,    44,   102,    12,   333,    12,   317,
      33,    51,    51,    37,    57,    74,    12,    62,    58,    97,
      54,    29,    56,    31,    69,    97,    74,    12,   272,   188,
      10,    72,    12,    58,    14,   361,     3,    77,    66,   329,
      16,    84,   332,   266,     3,    88,   354,    27,    28,    38,
      84,    40,    75,    93,    32,    96,    70,    71,    86,   347,
     348,   108,   102,    91,    92,    97,    97,    12,    97,    97,
     108,   211,    67,    68,   135,    16,    86,   140,    12,     3,
     121,   144,     0,   123,    55,   144,    14,    61,    74,   130,
      98,    99,   140,    14,   384,   135,   134,    60,    73,   267,
     108,    31,    72,   141,   263,   328,    11,    13,   251,    17,
      90,   135,    15,    34,    13,    95,    96,    97,    98,    99,
     181,    13,    12,     3,    14,    34,   134,     3,    29,    50,
      51,    14,    16,   141,     7,     8,     9,   198,    59,    58,
       5,   181,    64,    59,    13,   208,   314,   187,   188,   208,
       5,    87,    13,   193,     5,   323,    12,   181,   198,    13,
      50,    51,    14,    13,    12,    86,   227,   228,   336,    59,
     211,    12,   233,    13,   198,   215,   216,   217,   218,   219,
     230,    30,   222,   223,   224,   225,   226,   227,   228,    13,
     230,    13,   232,   233,    17,    16,    86,   205,   239,   367,
     261,    12,    12,   227,   228,    54,     3,    56,    57,   249,
     250,   272,     3,     4,     5,    87,     7,     8,   259,     3,
      13,   261,    17,   263,    87,    34,   266,    34,    34,   269,
      21,    13,   272,   273,    13,    84,    37,   261,   278,    88,
      13,    13,   283,    13,   284,     3,    17,    13,    66,    64,
       5,    42,     3,     3,   305,   306,     3,   298,   309,   310,
     354,   322,    51,   393,    29,   305,   306,   331,    86,   309,
     310,   284,    36,    91,    92,   315,   330,   317,   317,    97,
     359,   216,   322,   335,   215,     4,     5,     6,   328,   218,
     217,   331,    36,   219,    36,   335,    36,   222,   349,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   223,   349,
      63,    64,    65,    66,   354,   354,    36,   224,    36,   359,
     371,   112,   113,   225,   115,   116,   377,    19,   119,   226,
      -1,   371,   393,    -1,    -1,    -1,    -1,   377,    -1,   390,
     391,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
     390,   391,    -1,   393,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    -1,   204,     8,   206,    10,    -1,    12,    -1,
      14,    -1,    16,   214,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    30,    -1,    -1,    -1,
     231,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    86,    -1,    88,    89,    90,    91,    92,    -1,
      -1,    95,    96,    97,    98,    99,    -1,    -1,    -1,    -1,
     301,    -1,   303,    -1,    -1,    -1,   307,   308,     3,    -1,
      -1,    -1,   313,     8,    -1,    10,    -1,    12,    -1,    14,
      -1,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,   330,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    96,    97,    98,    99,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    -1,    14,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      78,    -1,    -1,     8,    -1,    10,    -1,    12,    86,    14,
      -1,    89,    90,    91,    92,    -1,    -1,    95,    96,    97,
      98,    99,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    86,    14,    -1,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,     3,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    91,
      92,    27,    28,    95,    96,    97,    98,    99,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,
      86,    14,    -1,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    27,    28,    -1,    30,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    52,
      53,    -1,    55,    -1,    57,     8,    -1,    10,    -1,    12,
      -1,    14,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    84,    35,    86,    -1,    88,    89,    90,    91,    92,
      -1,    -1,    95,    96,    97,    98,    99,    50,    51,    52,
      53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,     8,    -1,
      10,    -1,    12,    86,    14,    -1,    89,    90,    91,    92,
      -1,    -1,    95,    96,    97,    98,    99,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    52,    53,    -1,    55,    -1,    57,     8,    -1,
      10,    -1,    12,    13,    14,    -1,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
      50,    51,    52,    53,    -1,    55,    -1,    57,     8,    -1,
      10,    -1,    12,    -1,    14,    15,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    86,    -1,    -1,    -1,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
      50,    51,    52,    53,    -1,    55,    -1,    57,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    86,    -1,    -1,    -1,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
      50,    51,    52,    53,    -1,    55,    -1,    57,     8,    -1,
      10,    -1,    12,    -1,    14,    15,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    86,    -1,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
      50,    51,    52,    53,    -1,    55,    -1,    57,     8,    -1,
      10,    -1,    12,    13,    14,    -1,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    86,    -1,    -1,    -1,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
      50,    51,    52,    53,    -1,    55,    -1,    57,     8,    -1,
      10,    -1,    12,    -1,    14,    15,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    86,    -1,    -1,    -1,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
      50,    51,    52,    53,    -1,    55,    -1,    57,     8,    -1,
      10,    -1,    12,    13,    14,    -1,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    86,    -1,    -1,    -1,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
      50,    51,    52,    53,    -1,    55,    -1,    57,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    86,    -1,    -1,    -1,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
      50,    51,    52,    53,    -1,    55,    -1,    57,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    86,    -1,    -1,    -1,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
      50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    12,    -1,    14,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
      -1,    50,    51,    52,    53,    -1,     4,    -1,     6,     7,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,     4,    -1,     6,
      -1,    79,    80,    81,    82,    83,    13,    -1,    -1,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       4,    -1,     6,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    30,
      35,    36,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    66,    67,    68,    76,    77,    78,    84,    86,
      88,    89,    90,    91,    92,    95,    96,    97,    98,    99,
     101,   102,   103,   109,   111,   112,   113,   116,   117,   118,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   133,   134,   135,   137,   138,   140,   142,   144,   147,
     148,   149,   150,   151,   152,   153,   154,   156,   158,   160,
     162,   164,   165,   166,   167,   168,   172,   173,    55,    57,
     118,   129,   166,    13,   129,    15,   129,    17,    57,   109,
     129,   129,    12,    12,   117,    12,    12,    12,   118,    12,
     118,   172,   173,    16,     3,   129,     3,     3,   114,   118,
     142,    32,   107,   118,    57,    78,   112,   135,   144,   103,
     137,    55,   120,   135,   137,   138,   144,   108,   118,    12,
       4,     6,     7,     8,     9,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    50,    51,    62,    63,    64,    65,
      66,    67,    68,    70,    71,    79,    80,    81,    82,    83,
     132,   139,   157,   159,   161,   163,   171,   114,    86,   114,
     169,    16,   129,    12,   142,     0,   101,    55,   110,   109,
      29,    34,   141,     3,     5,   132,    14,   133,    57,   103,
     138,   118,   145,   146,    33,    75,    61,    73,    74,    72,
      62,    69,   155,   157,   159,   161,   163,    31,    77,   167,
      60,    14,    59,    86,   131,   133,   171,    11,    13,    15,
      17,    13,    13,   129,   129,    38,    40,   129,   129,    93,
      94,   128,     3,   129,    34,    34,   107,     3,   133,   134,
     116,    58,   104,     5,   114,    64,    59,    13,   114,    12,
     118,   107,     5,    87,   129,   143,    13,   108,   114,   129,
     135,    15,   133,   134,     5,   129,   151,   152,   153,   154,
     156,   158,   160,   162,   164,   165,   114,   114,   172,   118,
     169,    12,   170,    14,   133,    13,    13,    12,    12,    13,
      13,   118,   118,   128,    13,    17,   114,    16,   107,   141,
     108,   116,    12,   136,   118,     3,    87,   169,    66,   118,
       3,    17,    13,   118,   145,    34,   136,    87,    13,   129,
      15,   117,   117,   129,   129,   117,   117,    34,    34,    13,
     129,   116,   118,   119,   102,   105,   106,    13,   115,   142,
     116,    13,   108,   141,   143,   119,   141,   136,   149,   116,
      13,    37,    13,    13,   130,   130,   117,    13,     3,   106,
      17,    13,   146,   136,    64,     3,   116,   117,     3,     3,
      13,    13,   117,     5,   141,   117,   117,   115
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   100,   101,   101,   102,   102,   102,   103,   103,   103,
     104,   104,   105,   105,   106,   106,   107,   107,   108,   108,
     109,   109,   110,   110,   111,   111,   112,   112,   113,   114,
     114,   114,   115,   115,   116,   116,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   118,
     119,   119,   120,   120,   121,   122,   123,   123,   124,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   126,   126,
     127,   127,   127,   127,   128,   128,   129,   129,   130,   130,
     131,   131,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   133,
     133,   133,   134,   134,   134,   135,   135,   135,   135,   136,
     136,   137,   138,   138,   138,   139,   139,   139,   139,   139,
     139,   140,   140,   140,   140,   141,   141,   142,   142,   142,
     142,   142,   142,   143,   143,   144,   144,   144,   145,   145,
     146,   146,   146,   147,   148,   149,   149,   150,   150,   151,
     151,   152,   152,   153,   153,   154,   154,   155,   155,   156,
     156,   157,   157,   157,   157,   158,   158,   159,   159,   160,
     160,   161,   161,   162,   162,   163,   163,   163,   164,   164,
     165,   165,   166,   166,   166,   166,   166,   166,   166,   167,
     167,   167,   167,   168,   168,   169,   169,   170,   170,   171,
     171,   172,   172,   172,   172,   172,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     6,     2,     2,
       2,     0,     1,     0,     1,     2,     1,     3,     1,     3,
       1,     2,     1,     0,     1,     1,     6,     2,     4,     1,
       2,     2,     2,     4,     2,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     6,     2,     2,     3,     3,     3,     5,     5,
       7,     6,     7,     8,     8,     5,     7,     5,     5,     7,
       2,     3,     2,     2,     2,     1,     1,     3,     1,     3,
       0,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       4,     5,     2,     3,     2,     1,     2,     2,     2,     2,
       3,     4,     6,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     5,     5,     4,     0,     1,     2,     4,     5,
       7,     5,     2,     2,     3,     6,     2,     2,     1,     3,
       1,     2,     2,     3,     3,     1,     5,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     2,     3,     1,     3,     1,     3,     2,     3,     1,
       1,     1,     2,     3,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     3,     3,     2
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
          case 103: /* class_def  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_def(((*yyvaluep).class_def)); }
#line 1783 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 104: /* class_ext  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1789 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 105: /* class_body  */
#line 100 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1795 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 106: /* class_body2  */
#line 100 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1801 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 112: /* func_ptr  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1807 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 113: /* stmt_typedef  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1813 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 114: /* type_decl2  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1819 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 116: /* code_segment  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1825 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 117: /* stmt  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1831 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 120: /* enum_stmt  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1837 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 121: /* label_stmt  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1843 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 122: /* goto_stmt  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1849 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 123: /* case_stmt  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1855 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 124: /* switch_stmt  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1861 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 125: /* loop_stmt  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1867 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 126: /* selection_stmt  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1873 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 127: /* jump_stmt  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1879 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 128: /* exp_stmt  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1885 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 129: /* exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1891 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 130: /* binary_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1897 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 131: /* template  */
#line 102 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 1903 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 135: /* decl_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1909 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 142: /* type_decl  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1915 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 144: /* union_stmt  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1921 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 149: /* con_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1927 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 150: /* log_or_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1933 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 151: /* log_and_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1939 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 152: /* inc_or_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1945 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 153: /* exc_or_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1951 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 154: /* and_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1957 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 156: /* eq_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1963 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 158: /* relational_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1969 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 160: /* shift_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1975 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 162: /* add_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1981 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 164: /* mul_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1987 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 165: /* cast_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1993 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 167: /* unary_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1999 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 168: /* dur_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 2005 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 169: /* type_list  */
#line 102 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 2011 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 170: /* call_paren  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 2017 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 172: /* post_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 2023 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 173: /* primary_exp  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 2029 "src/ast/parser.c" /* yacc.c:1257  */
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
#line 109 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL, get_pos(arg));  }
#line 2297 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 110 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(arg)); }
#line 2303 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 114 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(arg)); }
#line 2309 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 115 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(arg)); }
#line 2315 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 116 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(arg)); }
#line 2321 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 121 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def(0, (yyvsp[-4].id_list), (yyvsp[-3].type_decl), (yyvsp[-1].class_body), get_pos(arg)); }
#line 2327 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 122 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_global); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2333 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 124 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].class_def), free_class_def); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2339 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 126 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2345 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 126 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = NULL; }
#line 2351 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 128 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2357 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 131 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body((yyvsp[0].section), NULL, get_pos(arg)); }
#line 2363 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 132 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body((yyvsp[-1].section), (yyvsp[0].class_body), get_pos(arg)); }
#line 2369 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 136 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2375 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 137 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2381 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 141 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2387 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 142 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2393 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 146 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(arg));}
#line 2399 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 147 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(arg));}
#line 2405 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 151 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_static;   }
#line 2411 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 152 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2417 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 156 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2423 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 157 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2429 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 161 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-2].sym), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(arg)); }
#line 2435 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 163 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_ptr), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2441 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 169 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_typedef((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2447 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 173 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(arg)); }
#line 2453 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 174 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(arg)); }
#line 2459 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 178 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(arg)); }
#line 2465 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 179 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(arg)); }
#line 2471 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 183 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code(NULL, get_pos(arg)); }
#line 2477 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 184 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code((yyvsp[-1].stmt_list), get_pos(arg)); }
#line 2483 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 203 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = insert_symbol((yyvsp[0].sval)); }
#line 2489 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 205 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = NULL; }
#line 2495 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 208 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sym), get_pos(arg)); }
#line 2501 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 209 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2507 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 213 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 1, get_pos(arg)); }
#line 2513 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 217 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 0, get_pos(arg)); }
#line 2519 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 221 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(arg)); }
#line 2525 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 222 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(arg)); }
#line 2531 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 226 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg));}
#line 2537 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 231 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while((yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2543 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 233 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while((yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2549 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 235 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2555 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 237 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2561 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 239 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2567 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 241 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); (yyval.stmt)->d.stmt_auto.is_ptr = 1;}
#line 2573 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 243 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until((yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2579 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 245 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until((yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2585 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 247 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2591 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 252 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(arg)); }
#line 2597 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 254 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(arg)); }
#line 2603 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 258 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return(NULL, get_pos(arg)); }
#line 2609 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 259 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return((yyvsp[-1].exp), get_pos(arg)); }
#line 2615 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 260 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_break(get_pos(arg)); }
#line 2621 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 261 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_continue(get_pos(arg)); }
#line 2627 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 265 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp((yyvsp[-1].exp),   get_pos(arg)); }
#line 2633 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 266 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp(NULL, get_pos(arg)); }
#line 2639 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 271 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_exp((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 2645 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 276 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2651 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 2657 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2663 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2669 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2675 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2681 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2687 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2693 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2699 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2705 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2711 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2717 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 285 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2723 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 285 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2729 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 286 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2735 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 286 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2741 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 286 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2747 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 287 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2753 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 287 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2759 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 288 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2765 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 292 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub((yyvsp[-1].exp), get_pos(arg)); }
#line 2771 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 293 "utils/gwion.y" /* yacc.c:1646  */
    { if((yyvsp[-2].exp)->next){ gwion_error(arg, "invalid format for array init [...][...]..."); free_exp((yyvsp[-2].exp)); free_array_sub((yyvsp[0].array_sub)); YYERROR; } (yyval.array_sub) = prepend_array_sub((yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2777 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 294 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [...][]..."); free_exp((yyvsp[-3].exp)); YYERROR; }
#line 2783 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 298 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(arg)); }
#line 2789 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 299 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2795 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 300 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [][...]"); free_array_sub((yyvsp[-1].array_sub)); free_array_sub((yyvsp[0].array_sub)); YYERROR; }
#line 2801 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 305 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2807 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 307 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.type, ae_flag_static); (yyval.exp) = (yyvsp[0].exp); }
#line 2813 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 309 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.type, ae_flag_private); (yyval.exp) = (yyvsp[0].exp); }
#line 2819 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 313 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2825 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 314 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2831 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 317 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2837 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 321 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sym), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2843 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 323 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_private); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2849 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 325 "utils/gwion.y" /* yacc.c:1646  */
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
#line 2865 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 341 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_op , (yyvsp[-2].type_decl), OP_SYM((yyvsp[-3].ival)), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2871 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 343 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_op | ae_flag_unary, (yyvsp[-2].type_decl), OP_SYM((yyvsp[-4].ival)), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2877 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 345 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_dtor, new_type_decl(new_id_list(insert_symbol("void"), get_pos(arg)), 0,
      get_pos(arg)), insert_symbol("dtor"), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2884 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 349 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2890 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 349 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 2896 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 352 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)), (yyvsp[0].ival), get_pos(arg)); }
#line 2902 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 353 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2908 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 354 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)),
      (yyvsp[0].ival), get_pos(arg)); (yyval.type_decl)->types = (yyvsp[-3].type_list); }
#line 2915 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 356 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg));
      (yyval.type_decl)->types = (yyvsp[-5].type_list); }
#line 2922 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 358 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2928 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 359 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].type_decl), ae_flag_const); (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2934 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 363 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2940 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 364 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2946 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 368 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(arg));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sym); }
#line 2952 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 370 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2958 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 372 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2964 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 376 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(arg)); }
#line 2970 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 377 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2976 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 381 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sym), NULL, get_pos(arg)); }
#line 2982 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 382 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2988 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 383 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2994 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 386 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar(  (yyvsp[-1].exp), get_pos(arg)); }
#line 3000 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 387 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec(    (yyvsp[-1].exp), get_pos(arg)); }
#line 3006 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 390 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_if((yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3012 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 393 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(arg)); }
#line 3018 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 396 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(arg)); }
#line 3024 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 399 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(arg)); }
#line 3030 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 402 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(arg)); }
#line 3036 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 405 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(arg)); }
#line 3042 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 407 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 3048 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 407 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_neq; }
#line 3054 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 410 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3060 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 412 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_lt; }
#line 3066 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 412 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_gt; }
#line 3072 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 413 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_le; }
#line 3078 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 413 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ge; }
#line 3084 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 416 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3090 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 419 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 3096 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 420 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 3102 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 424 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3108 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 426 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3114 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 426 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3120 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 429 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3126 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 431 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3132 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 431 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide; }
#line 3138 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 431 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_percent; }
#line 3144 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 434 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3150 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 437 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_cast((yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(arg)); }
#line 3156 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 439 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3162 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 439 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3168 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 439 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3174 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 440 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3180 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 440 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3186 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 441 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3192 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 441 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3198 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 445 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary((yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3204 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 447 "utils/gwion.y" /* yacc.c:1646  */
    {
      if((yyvsp[0].type_decl)->array && !(yyvsp[0].type_decl)->array->exp_list) {
        free_type_decl((yyvsp[0].type_decl));
        gwion_error(arg, "can't use empty '[]' in 'new' expression");
        YYERROR;
      }
      (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), get_pos(arg));
    }
#line 3217 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 456 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary3(op_spork, (yyvsp[0].stmt), get_pos(arg)); }
#line 3223 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 459 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3229 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 462 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list((yyvsp[0].type_decl), NULL, get_pos(arg)); }
#line 3235 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 463 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list((yyvsp[-2].type_decl), (yyvsp[0].type_list), get_pos(arg)); }
#line 3241 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 466 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 3247 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 466 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3253 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 468 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3259 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 468 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3265 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 471 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array((yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(arg)); }
#line 3271 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 473 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_call((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg));
      if((yyvsp[-1].type_list))(yyval.exp)->d.exp_func.tmpl = new_tmpl_call((yyvsp[-1].type_list)); }
#line 3278 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 476 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dot((yyvsp[-2].exp), (yyvsp[0].sym), get_pos(arg)); }
#line 3284 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 478 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_post((yyvsp[-1].exp), (yyvsp[0].ival), get_pos(arg)); }
#line 3290 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 482 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_id(     (yyvsp[0].sym), get_pos(arg)); }
#line 3296 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 483 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_int(    (yyvsp[0].ival), get_pos(arg)); }
#line 3302 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 484 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_float(  (yyvsp[0].fval), get_pos(arg)); }
#line 3308 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 485 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_string( (yyvsp[0].sval), get_pos(arg)); }
#line 3314 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 486 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_char(   (yyvsp[0].sval), get_pos(arg)); }
#line 3320 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 487 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3326 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 488 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3332 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 489 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_complex((yyvsp[-1].exp), get_pos(arg)); }
#line 3338 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 490 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_polar(  (yyvsp[0].polar), get_pos(arg)); }
#line 3344 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 491 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_vec(    (yyvsp[0].vec), get_pos(arg)); }
#line 3350 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 492 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_hack(   (yyvsp[-1].exp), get_pos(arg)); }
#line 3356 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 217:
#line 493 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) =                       (yyvsp[-1].exp);                    }
#line 3362 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 218:
#line 494 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_nil(        get_pos(arg)); }
#line 3368 "src/ast/parser.c" /* yacc.c:1646  */
    break;


#line 3372 "src/ast/parser.c" /* yacc.c:1646  */
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
#line 496 "utils/gwion.y" /* yacc.c:1906  */

