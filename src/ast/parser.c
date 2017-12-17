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
#define CHECK_FLAG(a,b,c) if(GET_FLAG(b, c)) gwion_error(a, "flag set twice");  SET_FLAG(b, c);
#define CHECK_TEMPLATE(a, b, c, free_function) { if(c->types) {\
        free_id_list(b);\
        free_function(c);\
        gwion_error(a, "double template decl");\
        YYERROR;\
      }\
      c->types = b;\
    };

int gwion_error(Scanner*, const char*);
int gwion_lex(void*, Scanner*);
int get_pos(Scanner*);

#line 91 "src/ast/parser.c" /* yacc.c:339  */

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
    RAND = 339,
    ROR = 340,
    REQ = 341,
    RNEQ = 342,
    RGT = 343,
    RGE = 344,
    RLT = 345,
    RLE = 346,
    TEMPLATE = 347,
    NOELSE = 348,
    LTB = 349,
    GTB = 350,
    VARARG = 351,
    UNION = 352,
    ATPAREN = 353,
    TYPEOF = 354,
    CONST = 355,
    NUM = 356,
    FLOAT = 357,
    ID = 358,
    STRING_LIT = 359,
    CHAR_LIT = 360
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
#define RAND 339
#define ROR 340
#define REQ 341
#define RNEQ 342
#define RGT 343
#define RGE 344
#define RLT 345
#define RLE 346
#define TEMPLATE 347
#define NOELSE 348
#define LTB 349
#define GTB 350
#define VARARG 351
#define UNION 352
#define ATPAREN 353
#define TYPEOF 354
#define CONST 355
#define NUM 356
#define FLOAT 357
#define ID 358
#define STRING_LIT 359
#define CHAR_LIT 360

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 24 "utils/gwion.y" /* yacc.c:355  */

  char* sval;
  int ival;
  m_float fval;
  S_Symbol sym;
  Complex* c_val;
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

#line 369 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (Scanner* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 385 "src/ast/parser.c" /* yacc.c:358  */

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
#define YYFINAL  157
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1471

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  106
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  74
/* YYNRULES -- Number of rules.  */
#define YYNRULES  216
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  377

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
       0,   109,   109,   110,   114,   115,   116,   120,   122,   123,
     126,   126,   128,   128,   131,   132,   136,   137,   141,   142,
     146,   147,   151,   152,   156,   157,   161,   162,   167,   171,
     172,   173,   177,   178,   182,   183,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   202,
     204,   204,   207,   211,   215,   219,   220,   224,   228,   230,
     232,   234,   236,   238,   240,   245,   247,   252,   253,   254,
     255,   259,   260,   264,   265,   269,   270,   273,   273,   275,
     275,   275,   276,   276,   277,   277,   278,   278,   279,   279,
     280,   280,   280,   281,   281,   281,   282,   282,   282,   283,
     283,   283,   284,   284,   288,   289,   290,   294,   295,   296,
     300,   301,   302,   303,   305,   310,   311,   314,   317,   319,
     321,   326,   327,   329,   334,   334,   337,   338,   339,   340,
     344,   345,   349,   350,   352,   357,   358,   362,   363,   364,
     367,   368,   369,   371,   371,   374,   374,   377,   377,   380,
     380,   383,   383,   386,   386,   389,   389,   391,   391,   394,
     394,   395,   395,   397,   397,   401,   402,   405,   405,   408,
     408,   410,   410,   413,   413,   413,   415,   415,   418,   418,
     421,   421,   421,   422,   422,   423,   423,   426,   426,   428,
     437,   440,   440,   444,   445,   448,   448,   450,   450,   452,
     452,   454,   456,   458,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475
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
  "TYPEDEF", "RSL", "RSR", "RSAND", "RSOR", "RSXOR", "RAND", "ROR", "REQ",
  "RNEQ", "RGT", "RGE", "RLT", "RLE", "TEMPLATE", "NOELSE", "LTB", "GTB",
  "VARARG", "UNION", "ATPAREN", "TYPEOF", "CONST", "NUM", "FLOAT", "ID",
  "STRING_LIT", "CHAR_LIT", "$accept", "ast", "section", "class_def",
  "class_ext", "class_body", "class_body2", "id_list", "id_dot",
  "stmt_list", "static_decl", "function_decl", "func_ptr", "stmt_typedef",
  "type_decl2", "arg_list", "code_segment", "stmt", "id", "opt_id",
  "enum_stmt", "label_stmt", "goto_stmt", "case_stmt", "switch_stmt",
  "loop_stmt", "selection_stmt", "jump_stmt", "exp_stmt", "exp",
  "binary_exp", "template", "op", "array_exp", "array_empty", "decl_exp",
  "func_args", "decl_template", "func_def_base", "func_def", "atsym",
  "type_decl", "decl_list", "union_stmt", "var_decl_list", "var_decl",
  "complex_exp", "polar_exp", "vec_exp", "con_exp", "log_or_exp",
  "log_and_exp", "inc_or_exp", "exc_or_exp", "and_exp", "eq_op", "eq_exp",
  "rel_op", "relational_exp", "shift_op", "shift_exp", "add_op", "add_exp",
  "mul_op", "mul_exp", "cast_exp", "unary_op", "unary_exp", "dur_exp",
  "type_list", "call_paren", "post_op", "post_exp", "primary_exp", YY_NULLPTR
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

#define YYPACT_NINF -270

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-270)))

#define YYTABLE_NINF -125

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     373,  -270,  -270,  1305,   878,   939,   476,  1305,  1305,  -270,
    -270,    23,    42,   555,    49,    53,    62,    -3,    85,    35,
      92,   616,   112,   115,  -270,  -270,   -32,   100,    -3,   817,
      -1,   738,    -3,  -270,  -270,   133,   -32,   -32,    58,    -3,
    -270,   145,  1305,   151,   -32,  -270,  -270,  -270,  -270,  -270,
     162,   373,  -270,  -270,   111,  -270,  -270,  -270,   555,    -5,
    -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,   170,
    1363,  -270,   160,  -270,   126,  -270,  -270,    -3,  -270,  -270,
    -270,  -270,  -270,    15,   114,   103,   104,   107,   -19,   105,
      18,     2,   150,   146,  1366,  -270,   121,    52,  -270,  1305,
    1305,    -4,   173,  -270,   172,  -270,   174,  -270,  1000,   175,
     183,   184,  1305,  1305,    67,  1305,  1305,   677,   195,  1305,
    -270,    70,   165,    -3,  -270,   197,  -270,  -270,  -270,   178,
     187,   192,   152,   207,   -32,  -270,  -270,  -270,  -270,    -1,
    1000,  -270,    30,  -270,  -270,   157,   156,   203,    -3,     9,
      -3,   218,   131,  1305,   216,    -3,  -270,  -270,  -270,  -270,
     -32,  -270,  -270,  -270,  -270,  -270,  -270,  1305,  -270,  -270,
    -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,
    -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,
    -270,  -270,  1305,  1061,  -270,    30,  -270,  -270,   187,  -270,
     225,  1305,  1366,  1366,  1366,  1366,  1366,  -270,  -270,  1366,
    -270,  -270,  -270,  -270,  1366,  -270,  -270,  1366,  -270,  -270,
    1366,  -270,  -270,  -270,  1366,   -32,  -270,    35,  1305,  -270,
    -270,    -3,    -3,   222,  -270,  -270,  -270,  -270,   223,  -270,
    -270,  -270,   226,   227,   224,   229,   231,   232,   677,  -270,
     233,  -270,  -270,   221,  -270,  -270,   160,  -270,    -3,   234,
      -3,   235,   178,    -3,   192,   236,    -3,   248,   158,    -3,
    1305,   249,   237,  -270,   242,    -3,  -270,  -270,  -270,  -270,
     160,    -3,   228,   114,   103,   104,   107,   -19,   105,    18,
       2,   150,   146,  -270,    52,  -270,   161,  1122,  -270,  1183,
    -270,   555,   555,  1305,  1305,   555,   555,  1244,   192,    -3,
    -270,   373,  -270,  -270,  -270,  -270,    44,   192,   244,  -270,
    -270,  -270,  -270,  1305,    -3,   178,   236,  -270,  1366,  -270,
    -270,   246,  -270,   230,  -270,   251,   252,  -270,  -270,   555,
     257,  -270,  -270,   258,   373,   254,  -270,  -270,   259,    -3,
    -270,   236,  -270,   270,  -270,   192,  -270,  -270,   555,   271,
     274,  -270,   555,  -270,  -270,  -270,  -270,   273,  -270,  -270,
    -270,  -270,  -270,  -270,  -270,   -32,  -270
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    72,   182,     0,     0,     0,     0,     0,     0,    24,
     185,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   184,     0,     0,     0,     0,
       0,     0,     0,   181,   180,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,   205,   206,    49,   207,   208,
       0,     2,     6,     4,    23,    46,    47,    39,    20,   204,
      44,    40,    41,    43,    42,    37,    38,    45,    36,     0,
      73,   209,   210,    75,     0,   121,     5,     0,    48,   211,
     212,   213,   110,   143,   145,   147,   149,   151,   153,   157,
     163,   167,   171,   176,     0,   178,   187,   191,   199,     0,
       0,   204,     0,   216,     0,   107,     0,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     204,    77,   199,     0,    67,     0,    69,    70,   189,   124,
      29,   186,    11,    16,     0,    27,   113,   133,     8,     0,
       0,   114,     0,   119,   134,     0,    18,     0,     0,     0,
       0,   193,     0,     0,     0,     0,   129,     1,     3,    22,
       0,    21,   125,    53,   126,    71,    79,     0,   103,    83,
      84,    85,    86,    87,    82,    80,    88,    89,    81,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,     0,     0,   109,     0,     9,   120,   137,   111,
     135,     0,     0,     0,     0,     0,     0,   155,   156,     0,
     162,   160,   161,   159,     0,   165,   166,     0,   170,   169,
       0,   174,   173,   175,     0,     0,   188,     0,     0,   197,
     198,     0,     0,     0,   200,   203,   214,   215,   104,    35,
     141,   140,     0,     0,     0,     0,     0,     0,     0,    54,
       0,    56,    55,     0,    68,    31,    30,   190,     0,     0,
       0,     0,   124,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   142,     0,     0,    74,    76,   108,   138,
     139,     0,     0,   146,   148,   150,   152,   154,   158,   164,
     168,   172,   177,   179,   192,   202,     0,     0,   201,     0,
     105,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      10,    13,    17,   127,    19,   123,     0,     0,     0,    28,
     117,   194,   112,   130,    50,   124,     0,   136,     0,    78,
     195,     0,   106,    65,    58,     0,     0,    62,    64,     0,
       0,    57,    51,     0,    14,     0,    12,   115,     0,     0,
     122,     0,   131,     0,   128,     0,   144,   196,     0,     0,
       0,    60,     0,    52,    15,     7,   116,    32,    26,   132,
     118,    66,    59,    63,    61,     0,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -270,   240,  -269,   -15,  -270,  -270,   -65,  -109,  -133,    34,
    -270,  -270,   253,  -270,   -25,   -95,  -122,   -12,     0,   -41,
    -270,  -270,  -270,  -270,  -270,  -270,  -270,  -270,  -107,    75,
    -270,  -270,  -270,   -45,   -97,   -27,  -220,   -23,   -28,  -270,
    -239,    -6,   -39,   -13,     4,   -63,  -270,  -270,  -270,   -40,
    -270,    90,    84,    91,    93,  -270,    94,  -270,    87,  -270,
      83,  -270,    82,  -270,    88,    80,  -270,   213,  -270,  -176,
    -270,  -270,   -14,   293
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    50,    51,    52,   259,   345,   346,   132,   145,    53,
     160,    54,    55,    56,   128,   348,    57,    58,   101,   343,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,   233,   192,    71,    72,    73,   317,    74,    75,    76,
     164,    77,   272,    78,   199,   200,    79,    80,    81,    82,
      83,    84,    85,    86,    87,   209,    88,   214,    89,   217,
      90,   220,    91,   224,    92,    93,    94,    95,    96,   152,
     298,   235,    97,    98
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      59,   114,   136,   143,   141,   121,    59,   139,   142,   257,
     248,   148,   149,    59,   253,   138,   137,   118,   144,   120,
     130,   266,   274,   313,   162,   162,   129,   194,   133,   163,
     130,   130,   146,   256,    32,   112,   129,   129,   156,   151,
     109,   268,   344,   207,   129,     3,   197,     4,   201,     5,
     208,    59,   234,    28,   113,    30,   296,   347,    59,   196,
       9,   115,     7,     8,   -77,   116,   228,    43,    44,   218,
     219,    47,   136,   141,   117,   344,   234,   198,   102,   104,
     106,   141,   110,   111,   228,   255,   354,   195,   215,   216,
     202,    38,   161,   321,   120,   144,   125,   119,  -124,  -124,
      47,   280,   229,   230,   251,   244,   355,   245,   123,   261,
      32,   231,    47,   136,   197,   126,   139,   154,   127,   142,
     229,   230,    38,   133,   196,   310,    40,   137,   130,   231,
     314,   368,   131,    42,   129,   275,    45,    46,    47,    48,
      49,   307,   315,    43,    44,   147,   232,    47,   265,   267,
     133,   312,   150,   279,   130,   146,     9,   221,   222,   223,
     129,   153,   157,   155,   232,   277,   159,   143,   210,   211,
     212,   213,   142,   165,   193,   203,   204,   225,   205,   206,
      28,   227,    30,   195,   236,   237,   341,   242,   243,   238,
     246,   247,   239,   300,   250,   350,   240,   241,   249,   252,
     254,     5,   120,   120,   120,   120,   120,   162,     6,   120,
     258,   194,   260,   294,   120,   263,   264,   120,    38,   293,
     120,   262,    40,   269,   120,   129,   270,   120,   271,   273,
     281,   295,   151,   370,   297,   194,   303,   299,   309,   301,
     302,   304,   276,   322,   305,   306,   308,   266,   316,   234,
     311,   319,   323,   320,   324,   325,   329,   351,   146,   357,
     133,   363,   328,   146,   359,   360,   318,   358,   106,   151,
     362,   365,   366,   369,   372,   326,   282,   373,   375,   364,
     376,   198,   135,   353,   352,   327,   367,   284,   356,   333,
     334,   158,   283,   337,   338,   285,   288,   289,   286,   290,
     287,    59,    59,   106,   292,    59,    59,   226,   291,   342,
     349,    59,   122,     0,     0,     0,   129,     0,     0,     0,
       0,     0,     0,     0,   342,     0,     0,   361,   120,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
       0,     0,     0,     0,    59,     0,   371,     0,     0,   198,
     374,     0,     0,     0,     0,     0,     0,     0,    59,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,   349,
       0,     0,   331,     0,   106,   129,     1,     0,   335,   336,
       0,     2,   340,     3,     0,     4,     0,     5,     0,     6,
       0,     0,     0,     0,     0,     0,     0,     0,   271,     0,
       7,     8,     0,     9,     0,     0,     0,     0,    10,    11,
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,     0,     0,     0,     0,     0,     0,     0,    32,
      33,    34,     0,     0,     0,     0,     0,     0,     0,    35,
      36,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    38,     0,    39,     0,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,     0,
       5,     0,     6,   107,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,    11,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    29,     0,   108,     0,     0,     0,     0,     0,     0,
       0,     0,    32,    33,    34,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,     0,     0,     0,     1,     0,
       0,     0,     0,     2,     0,     3,     0,     4,     0,     5,
      39,     6,     0,    41,    42,    43,    44,    45,    46,    47,
      48,    49,     7,     8,     0,     0,     0,     0,     0,     0,
      10,    11,     0,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      29,     0,   108,     0,     0,     0,     0,     0,     0,   124,
       0,    32,    33,    34,     2,     0,     3,     0,     4,     0,
       5,     0,     0,    37,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,    39,
       0,    10,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    99,     0,   100,     0,     0,     0,     0,     0,     0,
       1,     0,    32,    33,    34,     2,     0,     3,     0,     4,
       0,     5,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
      39,     0,    10,     0,    42,    43,    44,    45,    46,    47,
      48,    49,     0,     0,     0,     0,     0,    24,    25,    26,
      27,     0,    99,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     2,     0,     3,     0,
       4,     0,     5,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     9,     0,
       0,    39,     0,    10,     0,    42,    43,    44,    45,    46,
      47,    48,    49,     0,     0,     0,     0,     0,    24,    25,
      26,    27,     0,   140,     0,    31,     0,     0,     0,     0,
       0,     0,     0,     0,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     2,     0,     3,     0,     4,
      38,     5,    39,     0,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,     7,     8,     0,     0,     0,     0,
       0,     0,    10,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24,    25,    26,
      27,     0,    29,     0,   108,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     2,     0,     3,     0,
       4,   103,     5,     0,     0,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,    39,     0,    10,    41,    42,    43,    44,    45,    46,
      47,    48,    49,     0,     0,     0,     0,     0,    24,    25,
      26,    27,     0,    99,     0,   100,     0,     0,     0,     0,
       0,     0,     0,     0,    32,    33,    34,     2,     0,     3,
       0,     4,     0,     5,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,    39,     0,    10,     0,    42,    43,    44,    45,
      46,    47,    48,    49,     0,     0,     0,     0,     0,    24,
      25,    26,    27,     0,    99,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,     2,     0,
       3,     0,     4,     0,     5,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,    39,     0,    10,     0,    42,    43,    44,
      45,    46,    47,    48,    49,     0,     0,     0,     0,     0,
      24,    25,    26,    27,     0,   140,     0,   108,     0,     0,
       0,     0,     0,     0,     0,     0,    32,    33,    34,     2,
       0,     3,     0,     4,     0,     5,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
       0,     0,     0,     0,    39,     0,    10,    41,    42,    43,
      44,    45,    46,    47,    48,    49,     0,     0,     0,     0,
       0,    24,    25,    26,    27,     0,    99,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    33,    34,
       2,     0,     3,     0,     4,   330,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,    39,     0,    10,     0,    42,
      43,    44,    45,    46,    47,    48,    49,     0,     0,     0,
       0,     0,    24,    25,    26,    27,     0,    99,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,    32,    33,
      34,     2,     0,     3,     0,     4,     0,     5,   332,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,    39,     0,    10,     0,
      42,    43,    44,    45,    46,    47,    48,    49,     0,     0,
       0,     0,     0,    24,    25,    26,    27,     0,    99,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,    32,
      33,    34,     2,     0,     3,     0,     4,   339,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,    39,     0,    10,
       0,    42,    43,    44,    45,    46,    47,    48,    49,     0,
       0,     0,     0,     0,    24,    25,    26,    27,     0,    99,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
      32,    33,    34,     2,     0,     3,     0,     4,     0,     5,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,    39,     0,
      10,     0,    42,    43,    44,    45,    46,    47,    48,    49,
       0,     0,     0,     0,     0,    24,    25,    26,    27,     0,
      99,     0,   100,     0,     0,     0,     0,   166,   167,   168,
       0,    32,    33,    34,     2,     0,     3,     0,     4,     0,
       5,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,     0,     0,     7,     8,     0,     0,     0,     0,    39,
       0,    10,     0,    42,    43,    44,    45,    46,    47,    48,
      49,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,     0,     0,     0,     0,   178,     0,     0,     0,     0,
       0,     0,     0,    33,    34,     0,     0,     0,     0,     0,
       0,     0,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    42,     0,     0,    45,    46,    47,
      48,    49
};

static const yytype_int16 yycheck[] =
{
       0,    13,    29,    31,    31,    19,     6,    30,    31,   131,
     117,    36,    37,    13,   123,    30,    29,    17,    31,    19,
      26,    12,   155,   262,    29,    29,    26,    72,    28,    34,
      36,    37,    32,   130,    66,    12,    36,    37,    44,    39,
       6,   150,   311,    62,    44,    10,    74,    12,    33,    14,
      69,    51,    97,    54,    12,    56,   232,    13,    58,    74,
      30,    12,    27,    28,    12,    12,    14,    99,   100,    67,
      68,   103,    99,   100,    12,   344,   121,    77,     3,     4,
       5,   108,     7,     8,    14,   130,   325,    57,    70,    71,
      75,    92,    58,   269,    94,   108,    21,    12,   103,   103,
     103,   198,    50,    51,    34,    38,   326,    40,    16,   134,
      66,    59,   103,   140,   142,     3,   139,    42,     3,   142,
      50,    51,    92,   123,   139,   258,    96,   140,   134,    59,
     263,   351,    32,    98,   134,   160,   101,   102,   103,   104,
     105,   248,   264,    99,   100,    12,    94,   103,   148,   149,
     150,   260,    94,   198,   160,   155,    30,     7,     8,     9,
     160,    16,     0,    12,    94,   192,    55,   195,    63,    64,
      65,    66,   195,     3,    14,    61,    73,    31,    74,    72,
      54,    60,    56,    57,    11,    13,   308,   112,   113,    15,
     115,   116,    17,   238,   119,   317,    13,    13,     3,    34,
       3,    14,   202,   203,   204,   205,   206,    29,    16,   209,
      58,   256,     5,   227,   214,    59,    13,   217,    92,   225,
     220,    64,    96,     5,   224,   225,    95,   227,   153,    13,
       5,   231,   232,   355,    12,   280,    12,    14,    17,    13,
      13,    12,   167,   270,    13,    13,    13,    12,    12,   294,
      16,     3,     3,    95,    17,    13,    95,    13,   258,    13,
     260,     3,    34,   263,    13,    13,   266,    37,   193,   269,
      13,    17,    13,     3,     3,   275,   201,     3,     5,   344,
     375,   281,    29,   324,   323,   281,   349,   203,   328,   301,
     302,    51,   202,   305,   306,   204,   209,   214,   205,   217,
     206,   301,   302,   228,   224,   305,   306,    94,   220,   309,
     316,   311,    19,    -1,    -1,    -1,   316,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   324,    -1,    -1,   339,   328,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,
      -1,    -1,    -1,    -1,   344,    -1,   358,    -1,    -1,   349,
     362,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,    -1,
      -1,    -1,   362,    -1,    -1,    -1,    -1,    -1,    -1,   375,
      -1,    -1,   297,    -1,   299,   375,     3,    -1,   303,   304,
      -1,     8,   307,    10,    -1,    12,    -1,    14,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   323,    -1,
      27,    28,    -1,    30,    -1,    -1,    -1,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    94,    -1,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    12,    -1,    14,
      94,    16,    -1,    97,    98,    99,   100,   101,   102,   103,
     104,   105,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    66,    67,    68,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    94,
      -1,    35,    97,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    66,    67,    68,     8,    -1,    10,    -1,    12,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      94,    -1,    35,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,
      53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    30,    -1,
      -1,    94,    -1,    35,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,
      92,    14,    94,    -1,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,
      53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,     8,    -1,    10,    -1,
      12,    13,    14,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    94,    -1,    35,    97,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,     8,    -1,    10,
      -1,    12,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    94,    -1,    35,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    94,    -1,    35,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,
      50,    51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,     8,
      -1,    10,    -1,    12,    -1,    14,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    94,    -1,    35,    97,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,
      -1,    50,    51,    52,    53,    -1,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    94,    -1,    35,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,     8,    -1,    10,    -1,    12,    -1,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    94,    -1,    35,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      67,    68,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    94,    -1,    35,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,     8,    -1,    10,    -1,    12,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    94,    -1,
      35,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,     4,     5,     6,
      -1,    66,    67,    68,     8,    -1,    10,    -1,    12,    -1,
      14,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    94,
      -1,    35,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,    -1,   101,   102,   103,
     104,   105
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    30,
      35,    36,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    66,    67,    68,    76,    77,    78,    92,    94,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     107,   108,   109,   115,   117,   118,   119,   122,   123,   124,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   139,   140,   141,   143,   144,   145,   147,   149,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   162,   164,
     166,   168,   170,   171,   172,   173,   174,   178,   179,    55,
      57,   124,   135,    13,   135,    15,   135,    17,    57,   115,
     135,   135,    12,    12,   123,    12,    12,    12,   124,    12,
     124,   178,   179,    16,     3,   135,     3,     3,   120,   124,
     147,    32,   113,   124,    78,   118,   141,   149,   109,   143,
      55,   141,   143,   144,   149,   114,   124,    12,   120,   120,
      94,   124,   175,    16,   135,    12,   147,     0,   107,    55,
     116,   115,    29,    34,   146,     3,     4,     5,     6,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    62,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,   138,    14,   139,    57,   109,   144,   124,   150,
     151,    33,    75,    61,    73,    74,    72,    62,    69,   161,
      63,    64,    65,    66,   163,    70,    71,   165,    67,    68,
     167,     7,     8,     9,   169,    31,   173,    60,    14,    50,
      51,    59,    94,   137,   139,   177,    11,    13,    15,    17,
      13,    13,   135,   135,    38,    40,   135,   135,   134,     3,
     135,    34,    34,   113,     3,   139,   140,   122,    58,   110,
       5,   120,    64,    59,    13,   124,    12,   124,   113,     5,
      95,   135,   148,    13,   114,   120,   135,   141,    15,   139,
     140,     5,   135,   157,   158,   159,   160,   162,   164,   166,
     168,   170,   171,   147,   178,   124,   175,    12,   176,    14,
     139,    13,    13,    12,    12,    13,    13,   134,    13,    17,
     114,    16,   113,   146,   114,   122,    12,   142,   124,     3,
      95,   175,   141,     3,    17,    13,   124,   150,    34,    95,
      13,   135,    15,   123,   123,   135,   135,   123,   123,    13,
     135,   122,   124,   125,   108,   111,   112,    13,   121,   147,
     122,    13,   148,   125,   146,   142,   155,    13,    37,    13,
      13,   123,    13,     3,   112,    17,    13,   151,   142,     3,
     122,   123,     3,     3,   123,     5,   121
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   106,   107,   107,   108,   108,   108,   109,   109,   109,
     110,   110,   111,   111,   112,   112,   113,   113,   114,   114,
     115,   115,   116,   116,   117,   117,   118,   118,   119,   120,
     120,   120,   121,   121,   122,   122,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   124,
     125,   125,   126,   127,   128,   129,   129,   130,   131,   131,
     131,   131,   131,   131,   131,   132,   132,   133,   133,   133,
     133,   134,   134,   135,   135,   136,   136,   137,   137,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   139,   139,   139,   140,   140,   140,
     141,   141,   141,   141,   141,   142,   142,   143,   144,   144,
     144,   145,   145,   145,   146,   146,   147,   147,   147,   147,
     148,   148,   149,   149,   149,   150,   150,   151,   151,   151,
     152,   153,   154,   155,   155,   156,   156,   157,   157,   158,
     158,   159,   159,   160,   160,   161,   161,   162,   162,   163,
     163,   163,   163,   164,   164,   165,   165,   166,   166,   167,
     167,   168,   168,   169,   169,   169,   170,   170,   171,   171,
     172,   172,   172,   172,   172,   172,   172,   173,   173,   173,
     173,   174,   174,   175,   175,   176,   176,   177,   177,   178,
     178,   178,   178,   178,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     6,     2,     2,
       2,     0,     1,     0,     1,     2,     1,     3,     1,     3,
       1,     2,     1,     0,     1,     1,     6,     2,     4,     1,
       2,     2,     2,     4,     2,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     6,     2,     3,     3,     3,     5,     5,     7,
       6,     7,     5,     7,     5,     5,     7,     2,     3,     2,
       2,     2,     1,     1,     3,     1,     3,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     4,     5,     2,     3,     2,
       1,     2,     4,     2,     2,     2,     3,     4,     6,     2,
       2,     1,     5,     4,     0,     1,     2,     4,     5,     2,
       2,     3,     6,     2,     2,     1,     3,     1,     2,     2,
       3,     3,     3,     1,     5,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     3,     1,     1,     1,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     2,     1,     2,     2,
       3,     1,     3,     1,     3,     2,     3,     1,     1,     1,
       2,     3,     3,     2,     1,     1,     1,     1,     1,     1,
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
          case 109: /* class_def  */
#line 100 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_def(((*yyvaluep).class_def)); }
#line 1728 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 111: /* class_body  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1734 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 112: /* class_body2  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1740 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 118: /* func_ptr  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1746 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 119: /* stmt_typedef  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1752 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 120: /* type_decl2  */
#line 102 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1758 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 122: /* code_segment  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1764 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 123: /* stmt  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1770 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 126: /* enum_stmt  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1776 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 127: /* label_stmt  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1782 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 128: /* goto_stmt  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1788 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 129: /* case_stmt  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1794 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 130: /* switch_stmt  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1800 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 131: /* loop_stmt  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1806 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 132: /* selection_stmt  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1812 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 133: /* jump_stmt  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1818 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 134: /* exp_stmt  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1824 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 137: /* template  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 1830 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 147: /* type_decl  */
#line 102 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1836 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 149: /* union_stmt  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1842 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 175: /* type_list  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 1848 "src/ast/parser.c" /* yacc.c:1257  */
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
#line 2116 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 110 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(arg)); }
#line 2122 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 114 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(arg)); }
#line 2128 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 115 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(arg)); }
#line 2134 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 116 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(arg)); }
#line 2140 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 121 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def(0, (yyvsp[-4].id_list), (yyvsp[-3].id_list), (yyvsp[-1].class_body), get_pos(arg)); }
#line 2146 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 122 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_global); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2152 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 124 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].class_def), free_class_def); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2158 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 126 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[0].id_list); }
#line 2164 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 126 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2170 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 128 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2176 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 131 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body((yyvsp[0].section), NULL, get_pos(arg)); }
#line 2182 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 132 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body((yyvsp[-1].section), (yyvsp[0].class_body), get_pos(arg)); }
#line 2188 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 136 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2194 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 137 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2200 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 141 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2206 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 142 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2212 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 146 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(arg));}
#line 2218 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 147 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(arg));}
#line 2224 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 151 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_static;   }
#line 2230 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 152 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2236 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 156 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2242 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 157 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2248 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 161 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-2].sym), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(arg)); }
#line 2254 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 163 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_ptr), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2260 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 168 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_typedef((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2266 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 172 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(arg)); }
#line 2272 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 173 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(arg)); }
#line 2278 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 177 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(arg)); }
#line 2284 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 178 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(arg)); }
#line 2290 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 182 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code(NULL, get_pos(arg)); }
#line 2296 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 183 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code((yyvsp[-1].stmt_list), get_pos(arg)); }
#line 2302 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 202 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = insert_symbol((yyvsp[0].sval)); }
#line 2308 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 204 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = NULL; }
#line 2314 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 207 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sym), get_pos(arg)); }
#line 2320 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 211 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 1, get_pos(arg)); }
#line 2326 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 215 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 0, get_pos(arg)); }
#line 2332 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 219 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(arg)); }
#line 2338 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 220 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(arg)); }
#line 2344 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 224 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg));}
#line 2350 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 229 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while((yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2356 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 231 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while((yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2362 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 233 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2368 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 235 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2374 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 237 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until((yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2380 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 239 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until((yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2386 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 241 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2392 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 246 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(arg)); }
#line 2398 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 248 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(arg)); }
#line 2404 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 252 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return(NULL, get_pos(arg)); }
#line 2410 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 253 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return((yyvsp[-1].exp), get_pos(arg)); }
#line 2416 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 254 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_break(get_pos(arg)); }
#line 2422 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 255 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_continue(get_pos(arg)); }
#line 2428 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 259 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp((yyvsp[-1].exp),   get_pos(arg)); }
#line 2434 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 260 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp(NULL, get_pos(arg)); }
#line 2440 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 265 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_exp((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 2446 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 270 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2452 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 273 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 2458 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 273 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2464 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 275 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2470 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 275 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2476 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 275 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2482 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 276 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2488 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 276 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2494 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 277 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2500 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 277 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2506 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 278 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2512 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 278 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2518 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2524 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2530 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2536 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2542 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2548 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2554 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2560 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2566 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2572 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2578 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2584 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2590 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2596 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2602 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2608 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2614 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 288 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub((yyvsp[-1].exp), get_pos(arg)); }
#line 2620 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 289 "utils/gwion.y" /* yacc.c:1646  */
    { if((yyvsp[-2].exp)->next){ gwion_error(arg, "invalid format for array init [...][...]..."); free_exp((yyvsp[-2].exp)); free_array_sub((yyvsp[0].array_sub)); YYERROR; } (yyval.array_sub) = prepend_array_sub((yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2626 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 290 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [...][]..."); free_exp((yyvsp[-3].exp)); YYERROR; }
#line 2632 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 294 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(arg)); }
#line 2638 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 295 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2644 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 296 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [][...]"); free_array_sub((yyvsp[-1].array_sub)); free_array_sub((yyvsp[0].array_sub)); YYERROR; }
#line 2650 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 301 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2656 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 302 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); (yyval.exp)->d.exp_decl.types = (yyvsp[-2].type_list); }
#line 2662 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 304 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.type, ae_flag_static); (yyval.exp) = (yyvsp[0].exp); }
#line 2668 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 306 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.type, ae_flag_private); (yyval.exp) = (yyvsp[0].exp); }
#line 2674 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 310 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2680 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 311 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2686 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 314 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2692 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 318 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sym), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2698 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 320 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_private); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2704 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 322 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].func_def), free_func_def);
      (yyval.func_def) = (yyvsp[0].func_def); SET_FLAG((yyval.func_def), ae_flag_template); }
#line 2711 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 328 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_static | ae_flag_op , (yyvsp[-3].type_decl), (yyvsp[-2].sym), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2717 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 330 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_dtor, new_type_decl(new_id_list(insert_symbol("void"), get_pos(arg)), 0,
      get_pos(arg)), insert_symbol("dtor"), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2724 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 334 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2730 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 334 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 2736 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 337 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)), (yyvsp[0].ival), get_pos(arg)); }
#line 2742 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 338 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2748 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 339 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2754 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 340 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].type_decl), ae_flag_const); (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2760 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 344 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2766 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 345 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2772 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 349 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(arg));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sym); }
#line 2778 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 351 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2784 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 353 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2790 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 357 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(arg)); }
#line 2796 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 358 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2802 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 362 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sym), NULL, get_pos(arg)); }
#line 2808 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 363 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2814 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 364 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2820 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 367 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex((yyvsp[-1].exp), get_pos(arg)); }
#line 2826 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 368 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar(  (yyvsp[-1].exp), get_pos(arg)); }
#line 2832 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 369 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec(    (yyvsp[-1].exp), get_pos(arg)); }
#line 2838 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 372 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_if((yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 2844 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 375 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(arg)); }
#line 2850 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 378 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(arg)); }
#line 2856 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 381 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(arg)); }
#line 2862 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 384 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(arg)); }
#line 2868 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 387 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(arg)); }
#line 2874 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 389 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2880 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 389 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_neq; }
#line 2886 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 392 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2892 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 394 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_lt; }
#line 2898 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 394 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_gt; }
#line 2904 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 395 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_le; }
#line 2910 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 395 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ge; }
#line 2916 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 398 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2922 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 401 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 2928 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 402 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 2934 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 406 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2940 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 408 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2946 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 408 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 2952 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 411 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2958 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 413 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 2964 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 413 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide; }
#line 2970 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 413 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_percent; }
#line 2976 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 416 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2982 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 419 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_cast((yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(arg)); }
#line 2988 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 421 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2994 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 421 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3000 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 421 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3006 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 422 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3012 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 422 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3018 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 423 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3024 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 423 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3030 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 427 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary((yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3036 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 429 "utils/gwion.y" /* yacc.c:1646  */
    {
      if((yyvsp[0].type_decl)->array && !(yyvsp[0].type_decl)->array->exp_list) {
        free_type_decl((yyvsp[0].type_decl));
        gwion_error(arg, "can't use empty '[]' in 'new' expression");
        YYERROR;
      }
      (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(arg));
    }
#line 3049 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 438 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary3(op_spork, (yyvsp[0].stmt), get_pos(arg)); }
#line 3055 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 441 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3061 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 444 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sym), get_pos(arg)), NULL, get_pos(arg)); }
#line 3067 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 445 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sym), get_pos(arg)), (yyvsp[0].type_list), get_pos(arg)); }
#line 3073 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 448 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 3079 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 448 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3085 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 450 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3091 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 450 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3097 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 453 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array((yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(arg)); }
#line 3103 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 455 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_call((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); (yyval.exp)->d.exp_func.types = (yyvsp[-1].type_list); }
#line 3109 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 457 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dot((yyvsp[-2].exp), (yyvsp[0].sym), get_pos(arg)); }
#line 3115 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 459 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_post((yyvsp[-1].exp), (yyvsp[0].ival), get_pos(arg)); }
#line 3121 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 463 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_id(     (yyvsp[0].sym), get_pos(arg)); }
#line 3127 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 464 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_int(    (yyvsp[0].ival), get_pos(arg)); }
#line 3133 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 465 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_float(  (yyvsp[0].fval), get_pos(arg)); }
#line 3139 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 466 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_string( (yyvsp[0].sval), get_pos(arg)); }
#line 3145 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 467 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_char(   (yyvsp[0].sval), get_pos(arg)); }
#line 3151 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 468 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3157 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 469 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3163 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 470 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_complex((yyvsp[0].c_val), get_pos(arg)); }
#line 3169 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 471 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_polar(  (yyvsp[0].polar), get_pos(arg)); }
#line 3175 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 472 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_vec(    (yyvsp[0].vec), get_pos(arg)); }
#line 3181 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 473 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_hack(   (yyvsp[-1].exp), get_pos(arg)); }
#line 3187 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 474 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) =                       (yyvsp[-1].exp);                    }
#line 3193 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 475 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_nil(        get_pos(arg)); }
#line 3199 "src/ast/parser.c" /* yacc.c:1646  */
    break;


#line 3203 "src/ast/parser.c" /* yacc.c:1646  */
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
#line 477 "utils/gwion.y" /* yacc.c:1906  */

