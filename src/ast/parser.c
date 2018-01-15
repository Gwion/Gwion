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
#define CHECK_TEMPLATE(a, b, c, free_function) { if(c->types) {\
        free_id_list(b);\
        free_function(c);\
        gwion_error(a, "double template decl");\
        YYERROR;\
      }\
      c->types = b;\
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

#line 359 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (Scanner* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 375 "src/ast/parser.c" /* yacc.c:358  */

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
#define YYFINAL  196
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1746

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  100
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  75
/* YYNRULES -- Number of rules.  */
#define YYNRULES  219
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  399

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
       0,   112,   112,   113,   117,   118,   119,   123,   125,   126,
     129,   129,   131,   131,   134,   135,   139,   140,   144,   145,
     149,   150,   154,   155,   159,   160,   164,   165,   171,   175,
     176,   177,   181,   182,   186,   187,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   206,
     208,   208,   211,   212,   216,   220,   224,   225,   229,   233,
     235,   237,   239,   241,   243,   245,   247,   249,   254,   256,
     261,   262,   263,   264,   268,   269,   273,   274,   278,   279,
     282,   282,   284,   284,   284,   285,   285,   286,   286,   287,
     287,   288,   288,   289,   289,   289,   290,   290,   291,   295,
     296,   297,   301,   302,   303,   307,   308,   309,   311,   316,
     317,   320,   323,   325,   327,   331,   331,   331,   331,   331,
     331,   333,   334,   336,   338,   343,   343,   346,   347,   348,
     350,   352,   353,   357,   358,   362,   363,   365,   370,   371,
     375,   376,   377,   380,   381,   382,   384,   384,   387,   387,
     390,   390,   393,   393,   396,   396,   399,   399,   402,   402,
     404,   404,   407,   407,   408,   408,   410,   410,   414,   415,
     418,   418,   421,   421,   423,   423,   426,   426,   426,   428,
     428,   431,   431,   434,   434,   434,   435,   435,   436,   436,
     439,   439,   441,   450,   453,   453,   457,   458,   461,   461,
     463,   463,   465,   465,   467,   469,   471,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488
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
  "var_decl_list", "var_decl", "complex_exp", "polar_exp", "vec_exp",
  "con_exp", "log_or_exp", "log_and_exp", "inc_or_exp", "exc_or_exp",
  "and_exp", "eq_op", "eq_exp", "rel_op", "relational_exp", "shift_op",
  "shift_exp", "add_op", "add_exp", "mul_op", "mul_exp", "cast_exp",
  "unary_op", "unary_exp", "dur_exp", "type_list", "call_paren", "post_op",
  "post_exp", "primary_exp", YY_NULLPTR
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

#define YYPACT_NINF -272

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-272)))

#define YYTABLE_NINF -126

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     446,  -272,  -272,  1448,  1098,  1148,   543,  1448,  1448,  -272,
    -272,    54,    63,   640,    84,    92,    95,    19,   117,   310,
     114,   786,   131,   136,  -272,  -272,    67,   113,    19,   975,
       6,   925,    19,  -272,  -272,   140,  1549,    67,    68,    19,
    -272,   141,  1448,   144,    67,  -272,  -272,  -272,  -272,  -272,
     162,   446,  -272,  -272,   108,  -272,  -272,  -272,   640,    -4,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,   163,
    1617,  -272,   151,  -272,    35,  -272,  -272,    19,  -272,  -272,
    -272,  -272,  -272,     5,   112,   102,   106,   104,    -5,   105,
     -22,   -14,   176,   148,   335,  -272,   132,    28,  -272,  1448,
    1448,    16,   180,  1498,  -272,   182,  -272,   178,  -272,  1048,
     181,   187,   190,  1448,  1448,    87,  1448,  1448,   713,   201,
    1448,  -272,    22,   171,    19,  -272,   203,  -272,  -272,  -272,
     183,   196,   195,   155,   209,  1198,    67,  -272,  -272,  -272,
    -272,     6,  1198,  -272,  -272,    13,  -272,  -272,   158,   173,
     217,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,    67,  -272,  -272,  -272,  -272,  -272,     8,    19,
     230,   150,  1448,   226,    19,  -272,  -272,  -272,  -272,    67,
    -272,  -272,  -272,  -272,  -272,  1448,  1448,  1248,  -272,    13,
    -272,  -272,   196,  -272,   236,  1448,  1498,  1498,  1498,  1498,
    1498,  -272,  -272,  1498,  1498,  1498,  1498,  1498,    67,    67,
    -272,   310,  1448,    19,    19,   231,  -272,  -272,  -272,  -272,
     228,  -272,  -272,  -272,   232,   233,   235,   237,   241,   244,
      19,    19,   852,  -272,   245,  -272,  -272,   227,  -272,  -272,
     151,  -272,    19,   247,    19,   248,   183,    19,   195,   249,
      19,   262,   161,    19,     3,   263,   251,  -272,   256,    19,
    -272,  -272,  -272,  -272,   151,    19,   238,   112,   102,   106,
     104,    -5,   105,   -22,   -14,   176,   148,  -272,   249,    28,
    -272,   184,  1298,  -272,  1348,  -272,   640,   640,  1448,  1448,
     640,   640,   242,   243,  1398,   195,    19,  -272,   446,  -272,
    -272,  -272,  -272,    20,   195,   265,  -272,  -272,  -272,    19,
     183,  1448,    19,   183,   249,  -272,  1498,   195,  -272,  -272,
     267,  -272,   246,  -272,   268,   269,  -272,  -272,  1448,  1448,
     640,   271,  -272,  -272,   272,   446,   270,  -272,  -272,   273,
      19,  -272,   249,   224,  -272,  -272,   286,  -272,   195,  -272,
    -272,  -272,   640,   287,   288,  1640,  1663,  -272,   640,  -272,
    -272,  -272,  -272,   289,  -272,   183,  -272,  -272,  -272,  -272,
    -272,   640,   640,  -272,    67,  -272,  -272,  -272,  -272
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    75,   185,     0,     0,     0,     0,     0,     0,    24,
     188,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   186,   187,     0,     0,     0,     0,
       0,     0,     0,   184,   183,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,   208,   209,    49,   210,   211,
       0,     2,     6,     4,    23,    46,    47,    39,    20,   207,
      44,    40,    41,    43,    42,    37,    38,    45,    36,     0,
      76,   212,   213,    78,     0,   121,     5,     0,    48,   214,
     215,   216,   105,   146,   148,   150,   152,   154,   156,   160,
     166,   170,   174,   179,     0,   181,   190,   194,   202,     0,
       0,   207,     0,     0,   219,     0,   102,     0,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   207,    80,   202,     0,    70,     0,    72,    73,   192,
     125,    29,   189,    11,    16,     0,     0,    27,   107,   136,
       8,     0,     0,    53,   108,     0,   113,   137,     0,    18,
       0,    82,    98,   177,   176,   178,    86,    87,    88,    89,
      90,    85,    83,    91,    92,   200,   201,    84,   165,   163,
     164,   162,   173,   172,   168,   169,    93,    94,    95,    96,
      97,   115,     0,   118,   116,   120,   119,   117,     0,     0,
     196,     0,     0,     0,     0,   132,     1,     3,    22,     0,
      21,   126,    54,   127,    74,     0,     0,     0,   104,     0,
       9,   114,   140,   106,   138,     0,     0,     0,     0,     0,
       0,   158,   159,     0,     0,     0,     0,     0,     0,     0,
     191,     0,     0,     0,     0,     0,   203,   206,   217,   218,
      99,    35,   144,   143,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    55,     0,    57,    56,     0,    71,    31,
      30,   193,     0,     0,     0,     0,   125,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   145,     0,     0,
      77,    79,   103,   141,   142,     0,     0,   149,   151,   153,
     155,   157,   161,   167,   171,   175,   180,   182,     0,   195,
     205,     0,     0,   204,     0,   100,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    10,    13,    17,
     128,    19,   124,     0,     0,     0,    28,   111,   197,     0,
     125,   133,    50,   125,     0,   139,     0,     0,    81,   198,
       0,   101,    68,    59,     0,     0,    65,    67,     0,     0,
       0,     0,    58,    51,     0,    14,     0,    12,   109,     0,
       0,   122,     0,     0,   129,   134,     0,   131,     0,   147,
     123,   199,     0,     0,     0,     0,     0,    61,     0,    52,
      15,     7,   110,    32,    26,   125,   135,   112,    69,    60,
      66,     0,     0,    62,     0,   130,    63,    64,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -272,   250,  -271,    -3,  -272,  -272,   -62,  -113,  -179,    40,
    -272,  -272,   274,  -272,   -27,   -97,  -128,   -11,     0,   -40,
     -10,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -104,   139,
    -228,  -272,   276,   -63,  -102,    32,  -267,   -23,   -19,  -272,
    -272,  -248,   -21,   -33,    -7,    23,   -56,  -272,  -272,  -272,
     -31,  -272,    93,    96,    97,    98,  -272,    94,   285,   103,
     291,   101,   292,   109,   294,   107,   115,     1,   -53,  -272,
    -199,  -272,   299,   -16,   321
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    50,    51,    52,   263,   356,   357,   133,   148,    53,
     199,    54,    55,    56,   129,   359,    57,    58,   101,   354,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,   235,   206,    71,    72,    73,   324,    74,    75,   182,
      76,   203,    77,   276,    78,   213,   214,    79,    80,    81,
      82,    83,    84,    85,    86,    87,   223,    88,   224,    89,
     225,    90,   226,    91,   227,    92,    93,   103,    95,    96,
     191,   303,   237,    97,    98
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      59,    94,   115,   122,   261,   131,    59,   141,   145,   208,
     188,   257,   146,    59,   252,   278,   131,   119,   320,   121,
     270,   143,   139,   195,   147,   201,   130,   140,   134,   260,
     202,   337,   149,   358,   236,   301,   232,   130,   215,   190,
     -80,   230,   232,     9,   130,   201,   110,   355,   174,   175,
     230,    59,    94,   172,   173,   211,   255,   221,    59,   236,
      28,   138,    30,   144,   222,     9,   113,   368,   259,   329,
     209,   210,   165,   166,   328,   114,   272,   212,   165,   166,
     216,   233,   364,   317,   355,   367,    32,   233,   321,    28,
      38,    30,   209,  -125,   121,   384,   116,    38,   200,   143,
      47,    40,   147,   121,   117,    47,    39,   118,   234,   265,
     284,    43,    44,  -125,   234,   131,    47,    47,   141,    38,
     375,   376,   145,    40,   134,   246,   211,   247,   147,   120,
     124,   138,   144,    32,   127,   139,   130,   395,   210,   128,
     322,   144,   102,   105,   107,   132,   111,   112,   314,   283,
     363,   319,   150,    39,   189,   269,   194,   192,    43,    44,
     126,   131,   196,   198,    47,   207,   204,   144,   168,   169,
     170,   171,   279,   217,   138,   218,   220,   305,   131,   228,
     219,   193,   130,   153,   154,   155,   145,   352,   271,   134,
     146,   238,   231,   240,   149,   239,   361,   208,   241,   130,
     242,   297,   298,   243,   253,   256,   258,   131,   131,   370,
       5,     6,   201,   262,   264,   299,   121,   121,   121,   121,
     121,   208,   266,   121,   121,   121,   121,   121,   130,   130,
     268,   121,   267,   300,   190,   273,   236,   274,   281,   277,
     387,   285,   304,   302,   316,   306,   307,   308,   327,   309,
     312,   313,   244,   245,   310,   248,   249,   311,   315,   254,
     270,   323,   149,   318,   134,   326,   331,   149,   332,   333,
     325,   338,   336,   190,   330,   379,   348,   349,   362,   334,
     371,   373,   374,   372,   378,   212,   382,   381,   385,   386,
     389,   390,   366,   380,   394,   342,   343,   398,   365,   346,
     347,   197,   360,   137,   383,   369,    59,    59,   335,   287,
      59,    59,   181,   288,   291,   289,   353,   290,    59,    94,
       3,   183,     4,   130,     5,   293,   292,   184,   185,   149,
     186,   275,   353,   295,   294,   187,   121,     7,     8,   377,
     123,     0,   296,     2,   280,     3,   107,     4,     0,     5,
      59,     0,     0,     0,   286,    59,    94,     0,     0,     0,
     212,   388,     7,     8,     0,     0,     0,   393,     0,     0,
      10,   107,    59,   360,     0,     0,     0,     0,    59,     0,
     396,   397,     0,     0,     0,    24,    25,    26,    27,     0,
       0,    59,    59,     0,   130,     0,     0,     0,     0,     0,
      42,     0,    33,    34,     0,    45,    46,    47,    48,    49,
       0,     0,   229,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
      45,    46,    47,    48,    49,     0,     0,     0,     0,     0,
       0,   340,     0,   107,     0,     0,     0,   344,   345,     1,
       0,     0,     0,   351,     2,     0,     3,     0,     4,     0,
       5,     0,     6,     0,     0,     0,     0,     0,     0,     0,
     275,     0,     0,     7,     8,     0,     9,     0,     0,     0,
       0,    10,    11,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,     0,     0,     0,     0,     0,     0,
       0,     0,    32,    33,    34,     0,     0,     0,     0,     0,
       0,     0,    35,    36,    37,     0,     0,     0,     0,     0,
      38,     0,    39,     0,    40,    41,    42,    43,    44,     0,
       0,    45,    46,    47,    48,    49,     1,     0,     0,     0,
       0,     2,     0,     3,     0,     4,     0,     5,     0,     6,
     108,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,    10,    11,
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    29,     0,
     109,     0,     0,     0,     0,     0,     0,     0,     0,    32,
      33,    34,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,     0,    39,
       0,     0,    41,    42,    43,    44,     0,     0,    45,    46,
      47,    48,    49,     1,     0,     0,     0,     0,     2,     0,
       3,     0,     4,     0,     5,     0,     6,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    11,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    29,     0,   109,     0,     0,
       0,     0,     0,     0,     0,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     1,     0,    37,     0,
       0,     2,     0,     3,     0,     4,    39,     5,     0,    41,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
       7,     8,     0,     0,     0,     0,     0,     0,    10,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    24,    25,    26,    27,     0,    99,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,    32,
      33,    34,     0,     0,     0,     0,     0,     0,     0,   125,
       0,     0,     0,     0,     2,     0,     3,     0,     4,    39,
       5,     0,     0,    42,    43,    44,   250,   251,    45,    46,
      47,    48,    49,     7,     8,     0,     0,     0,     0,     0,
       0,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
       0,    99,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,    32,    33,    34,     1,     0,     0,     0,     0,
       2,     0,     3,     0,     4,     0,     5,     0,     0,     0,
       0,     0,    39,     0,     0,     0,    42,    43,    44,     7,
       8,    45,    46,    47,    48,    49,     0,    10,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,    25,    26,    27,     0,    99,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,    32,    33,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     2,     0,     3,     0,     4,    39,     5,
       0,     0,    42,    43,    44,     0,     0,    45,    46,    47,
      48,    49,     7,     8,     0,     9,     0,     0,     0,     0,
      10,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    20,     0,     0,     0,    24,    25,    26,    27,     0,
     142,     0,    31,     2,     0,     3,     0,     4,     0,     5,
       0,    32,    33,    34,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,     0,    38,
      10,    39,     0,    40,    41,    42,    43,    44,     0,     0,
      45,    46,    47,    48,    49,    24,    25,    26,    27,     0,
      29,     0,   135,     0,     0,     0,     0,     0,     0,     0,
       0,    32,    33,    34,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   136,     0,     0,     2,     0,     3,     0,
       4,    39,     5,     0,    41,    42,    43,    44,     0,     0,
      45,    46,    47,    48,    49,     7,     8,     0,     0,     0,
       0,     0,     0,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    20,     0,     0,     0,    24,    25,
      26,    27,     0,   142,     0,   109,     2,     0,     3,     0,
       4,   104,     5,     0,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    39,     0,     0,    41,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,    24,    25,
      26,    27,     0,    99,     0,   100,     2,     0,     3,     0,
       4,     0,     5,   106,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    39,     0,     0,     0,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,    24,    25,
      26,    27,     0,    99,     0,   100,     2,     0,     3,     0,
       4,     0,     5,     0,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    39,     0,     0,     0,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,    24,    25,
      26,    27,     0,   142,     0,   135,     2,     0,     3,     0,
       4,     0,     5,   282,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    39,     0,     0,    41,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,    24,    25,
      26,    27,     0,    99,     0,   100,     2,     0,     3,     0,
       4,   339,     5,     0,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    39,     0,     0,     0,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,    24,    25,
      26,    27,     0,    99,     0,   100,     2,     0,     3,     0,
       4,     0,     5,   341,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    39,     0,     0,     0,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,    24,    25,
      26,    27,     0,    99,     0,   100,     2,     0,     3,     0,
       4,   350,     5,     0,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    39,     0,     0,     0,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,    24,    25,
      26,    27,     0,    99,     0,   100,     2,     0,     3,     0,
       4,     0,     5,     0,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    39,     0,     0,     0,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,    24,    25,
      26,    27,     0,    99,     0,   100,     2,     0,     3,     0,
       4,     0,     5,     0,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    39,     0,     0,     0,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,    24,    25,
      26,    27,     0,   151,     0,   152,   153,   154,   155,     0,
       0,     0,     0,     0,     0,    33,    34,   156,   157,   158,
     159,   160,   161,   162,   163,   164,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,     0,    45,    46,    47,    48,    49,     0,   165,
     166,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   167,   168,   169,   170,   171,   172,   173,     0,   174,
     175,   151,   205,   152,     0,     0,     0,     0,   176,   177,
     178,   179,   180,     0,     0,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   151,     0,   152,     0,     0,     0,
       0,     0,     0,   391,     0,     0,     0,     0,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   151,     0,   152,
       0,     0,     0,     0,     0,     0,   392,     0,     0,   167,
       0,   156,   157,   158,   159,   160,   161,   162,   163,   164,
       0,     0,     0,     0,     0,     0,   176,   177,   178,   179,
     180,     0,   167,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   176,
     177,   178,   179,   180,     0,   167,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   176,   177,   178,   179,   180
};

static const yytype_int16 yycheck[] =
{
       0,     0,    13,    19,   132,    26,     6,    30,    31,    72,
      37,   124,    31,    13,   118,   194,    37,    17,   266,    19,
      12,    31,    29,    44,    31,    29,    26,    30,    28,   131,
      34,   298,    32,    13,    97,   234,    14,    37,    33,    39,
      12,    94,    14,    30,    44,    29,     6,   318,    70,    71,
     103,    51,    51,    67,    68,    74,    34,    62,    58,   122,
      54,    29,    56,    31,    69,    30,    12,   334,   131,    66,
      57,    74,    50,    51,   273,    12,   189,    77,    50,    51,
      75,    59,   330,   262,   355,   333,    66,    59,   267,    54,
      84,    56,    57,    97,    94,   362,    12,    84,    58,   109,
      97,    88,   109,   103,    12,    97,    86,    12,    86,   136,
     212,    91,    92,    97,    86,   136,    97,    97,   141,    84,
     348,   349,   145,    88,   124,    38,   145,    40,   135,    12,
      16,    99,   100,    66,     3,   142,   136,   385,   141,     3,
     268,   109,     3,     4,     5,    32,     7,     8,   252,   212,
     329,   264,    12,    86,    86,   182,    12,    16,    91,    92,
      21,   182,     0,    55,    97,    14,     3,   135,    63,    64,
      65,    66,   199,    61,   142,    73,    72,   240,   199,    31,
      74,    42,   182,     7,     8,     9,   209,   315,   188,   189,
     209,    11,    60,    15,   194,    13,   324,   260,    17,   199,
      13,   228,   229,    13,     3,    34,     3,   228,   229,   337,
      14,    16,    29,    58,     5,   231,   216,   217,   218,   219,
     220,   284,    64,   223,   224,   225,   226,   227,   228,   229,
      13,   231,    59,   233,   234,     5,   299,    87,   206,    13,
     368,     5,    14,    12,    17,    13,    13,    12,    87,    12,
     250,   251,   113,   114,    13,   116,   117,    13,    13,   120,
      12,    12,   262,    16,   264,     3,     3,   267,    17,    13,
     270,    87,    34,   273,   274,     3,    34,    34,    13,   279,
      13,    13,    13,    37,    13,   285,    13,    17,    64,     3,
       3,     3,   332,   355,     5,   306,   307,   394,   331,   310,
     311,    51,   323,    29,   360,   336,   306,   307,   285,   216,
     310,   311,    36,   217,   220,   218,   316,   219,   318,   318,
      10,    36,    12,   323,    14,   224,   223,    36,    36,   329,
      36,   192,   332,   226,   225,    36,   336,    27,    28,   350,
      19,    -1,   227,     8,   205,    10,   207,    12,    -1,    14,
     350,    -1,    -1,    -1,   215,   355,   355,    -1,    -1,    -1,
     360,   372,    27,    28,    -1,    -1,    -1,   378,    -1,    -1,
      35,   232,   372,   394,    -1,    -1,    -1,    -1,   378,    -1,
     391,   392,    -1,    -1,    -1,    50,    51,    52,    53,    -1,
      -1,   391,   392,    -1,   394,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    67,    68,    -1,    95,    96,    97,    98,    99,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,   302,    -1,   304,    -1,    -1,    -1,   308,   309,     3,
      -1,    -1,    -1,   314,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     331,    -1,    -1,    27,    28,    -1,    30,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    86,    -1,    88,    89,    90,    91,    92,    -1,
      -1,    95,    96,    97,    98,    99,     3,    -1,    -1,    -1,
      -1,     8,    -1,    10,    -1,    12,    -1,    14,    -1,    16,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,    96,
      97,    98,    99,     3,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    78,    -1,
      -1,     8,    -1,    10,    -1,    12,    86,    14,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    86,
      14,    -1,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    90,    91,    92,    27,
      28,    95,    96,    97,    98,    99,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    12,    86,    14,
      -1,    -1,    90,    91,    92,    -1,    -1,    95,    96,    97,
      98,    99,    27,    28,    -1,    30,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    51,    52,    53,    -1,
      55,    -1,    57,     8,    -1,    10,    -1,    12,    -1,    14,
      -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    84,
      35,    86,    -1,    88,    89,    90,    91,    92,    -1,    -1,
      95,    96,    97,    98,    99,    50,    51,    52,    53,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,     8,    -1,    10,    -1,
      12,    86,    14,    -1,    89,    90,    91,    92,    -1,    -1,
      95,    96,    97,    98,    99,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,     8,    -1,    10,    -1,
      12,    13,    14,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    50,    51,
      52,    53,    -1,    55,    -1,    57,     8,    -1,    10,    -1,
      12,    -1,    14,    15,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    86,    -1,    -1,    -1,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    50,    51,
      52,    53,    -1,    55,    -1,    57,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    86,    -1,    -1,    -1,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    50,    51,
      52,    53,    -1,    55,    -1,    57,     8,    -1,    10,    -1,
      12,    -1,    14,    15,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    86,    -1,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    50,    51,
      52,    53,    -1,    55,    -1,    57,     8,    -1,    10,    -1,
      12,    13,    14,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    86,    -1,    -1,    -1,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    50,    51,
      52,    53,    -1,    55,    -1,    57,     8,    -1,    10,    -1,
      12,    -1,    14,    15,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    86,    -1,    -1,    -1,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    50,    51,
      52,    53,    -1,    55,    -1,    57,     8,    -1,    10,    -1,
      12,    13,    14,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    86,    -1,    -1,    -1,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    50,    51,
      52,    53,    -1,    55,    -1,    57,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    86,    -1,    -1,    -1,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    50,    51,
      52,    53,    -1,    55,    -1,    57,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    86,    -1,    -1,    -1,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    50,    51,
      52,    53,    -1,     4,    -1,     6,     7,     8,     9,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,    -1,    50,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,     4,     5,     6,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     4,    -1,     6,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     4,    -1,     6,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,    62,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83
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
     148,   149,   150,   151,   152,   153,   154,   155,   157,   159,
     161,   163,   165,   166,   167,   168,   169,   173,   174,    55,
      57,   118,   129,   167,    13,   129,    15,   129,    17,    57,
     109,   129,   129,    12,    12,   117,    12,    12,    12,   118,
      12,   118,   173,   174,    16,     3,   129,     3,     3,   114,
     118,   142,    32,   107,   118,    57,    78,   112,   135,   144,
     103,   137,    55,   120,   135,   137,   138,   144,   108,   118,
      12,     4,     6,     7,     8,     9,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    50,    51,    62,    63,    64,
      65,    66,    67,    68,    70,    71,    79,    80,    81,    82,
      83,   132,   139,   158,   160,   162,   164,   172,   114,    86,
     118,   170,    16,   129,    12,   142,     0,   101,    55,   110,
     109,    29,    34,   141,     3,     5,   132,    14,   133,    57,
     103,   138,   118,   145,   146,    33,    75,    61,    73,    74,
      72,    62,    69,   156,   158,   160,   162,   164,    31,    77,
     168,    60,    14,    59,    86,   131,   133,   172,    11,    13,
      15,    17,    13,    13,   129,   129,    38,    40,   129,   129,
      93,    94,   128,     3,   129,    34,    34,   107,     3,   133,
     134,   116,    58,   104,     5,   114,    64,    59,    13,   114,
      12,   118,   107,     5,    87,   129,   143,    13,   108,   114,
     129,   135,    15,   133,   134,     5,   129,   152,   153,   154,
     155,   157,   159,   161,   163,   165,   166,   114,   114,   173,
     118,   170,    12,   171,    14,   133,    13,    13,    12,    12,
      13,    13,   118,   118,   128,    13,    17,   108,    16,   107,
     141,   108,   116,    12,   136,   118,     3,    87,   170,    66,
     118,     3,    17,    13,   118,   145,    34,   136,    87,    13,
     129,    15,   117,   117,   129,   129,   117,   117,    34,    34,
      13,   129,   116,   118,   119,   102,   105,   106,    13,   115,
     142,   116,    13,   108,   141,   143,   119,   141,   136,   150,
     116,    13,    37,    13,    13,   130,   130,   117,    13,     3,
     106,    17,    13,   146,   136,    64,     3,   116,   117,     3,
       3,    13,    13,   117,     5,   141,   117,   117,   115
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
     146,   146,   146,   147,   148,   149,   150,   150,   151,   151,
     152,   152,   153,   153,   154,   154,   155,   155,   156,   156,
     157,   157,   158,   158,   158,   158,   159,   159,   160,   160,
     161,   161,   162,   162,   163,   163,   164,   164,   164,   165,
     165,   166,   166,   167,   167,   167,   167,   167,   167,   167,
     168,   168,   168,   168,   169,   169,   170,   170,   171,   171,
     172,   172,   173,   173,   173,   173,   173,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174
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
       1,     2,     2,     3,     3,     3,     1,     5,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     1,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     2,
       1,     2,     2,     3,     1,     3,     1,     3,     2,     3,
       1,     1,     1,     2,     3,     3,     2,     1,     1,     1,
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
#line 102 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_def(((*yyvaluep).class_def)); }
#line 1776 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 105: /* class_body  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1782 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 106: /* class_body2  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1788 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 112: /* func_ptr  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1794 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 113: /* stmt_typedef  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1800 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 114: /* type_decl2  */
#line 104 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1806 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 116: /* code_segment  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1812 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 117: /* stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1818 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 120: /* enum_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1824 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 121: /* label_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1830 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 122: /* goto_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1836 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 123: /* case_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1842 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 124: /* switch_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1848 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 125: /* loop_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1854 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 126: /* selection_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1860 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 127: /* jump_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1866 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 128: /* exp_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1872 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 129: /* exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1878 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 130: /* binary_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1884 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 131: /* template  */
#line 105 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 1890 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 135: /* decl_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1896 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 142: /* type_decl  */
#line 104 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1902 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 144: /* union_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1908 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 150: /* con_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1914 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 151: /* log_or_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1920 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 152: /* log_and_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1926 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 153: /* inc_or_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1932 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 154: /* exc_or_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1938 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 155: /* and_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1944 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 157: /* eq_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1950 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 159: /* relational_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1956 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 161: /* shift_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1962 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 163: /* add_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1968 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 165: /* mul_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1974 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 166: /* cast_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1980 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 168: /* unary_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1986 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 169: /* dur_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1992 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 170: /* type_list  */
#line 105 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 1998 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 171: /* call_paren  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 2004 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 173: /* post_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 2010 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 174: /* primary_exp  */
#line 106 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 2016 "src/ast/parser.c" /* yacc.c:1257  */
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
#line 112 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL, get_pos(arg));  }
#line 2284 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 113 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(arg)); }
#line 2290 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 117 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(arg)); }
#line 2296 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 118 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(arg)); }
#line 2302 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 119 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(arg)); }
#line 2308 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 124 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def(0, (yyvsp[-4].id_list), (yyvsp[-3].id_list), (yyvsp[-1].class_body), get_pos(arg)); }
#line 2314 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 125 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_global); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2320 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 127 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].class_def), free_class_def); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2326 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 129 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[0].id_list); }
#line 2332 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 129 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2338 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 131 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2344 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 134 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body((yyvsp[0].section), NULL, get_pos(arg)); }
#line 2350 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 135 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body((yyvsp[-1].section), (yyvsp[0].class_body), get_pos(arg)); }
#line 2356 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 139 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2362 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 140 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2368 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 144 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2374 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 145 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2380 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 149 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(arg));}
#line 2386 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 150 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(arg));}
#line 2392 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 154 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_static;   }
#line 2398 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 155 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2404 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 159 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2410 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 160 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2416 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 164 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-2].sym), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(arg)); }
#line 2422 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 166 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_ptr), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2428 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 172 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_typedef((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2434 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 176 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(arg)); }
#line 2440 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 177 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(arg)); }
#line 2446 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 181 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(arg)); }
#line 2452 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 182 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(arg)); }
#line 2458 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 186 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code(NULL, get_pos(arg)); }
#line 2464 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 187 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code((yyvsp[-1].stmt_list), get_pos(arg)); }
#line 2470 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 206 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = insert_symbol((yyvsp[0].sval)); }
#line 2476 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 208 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = NULL; }
#line 2482 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 211 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sym), get_pos(arg)); }
#line 2488 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 212 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2494 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 216 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 1, get_pos(arg)); }
#line 2500 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 220 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 0, get_pos(arg)); }
#line 2506 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 224 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(arg)); }
#line 2512 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 225 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(arg)); }
#line 2518 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 229 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg));}
#line 2524 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 234 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while((yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2530 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 236 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while((yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2536 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 238 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2542 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 240 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2548 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 242 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2554 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 244 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); (yyval.stmt)->d.stmt_auto.is_ptr = 1;}
#line 2560 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 246 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until((yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2566 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 248 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until((yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2572 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 250 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2578 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 255 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(arg)); }
#line 2584 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 257 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(arg)); }
#line 2590 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 261 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return(NULL, get_pos(arg)); }
#line 2596 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 262 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return((yyvsp[-1].exp), get_pos(arg)); }
#line 2602 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 263 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_break(get_pos(arg)); }
#line 2608 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 264 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_continue(get_pos(arg)); }
#line 2614 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 268 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp((yyvsp[-1].exp),   get_pos(arg)); }
#line 2620 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 269 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp(NULL, get_pos(arg)); }
#line 2626 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 274 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_exp((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 2632 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2638 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 2644 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2650 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2656 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2662 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2668 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 285 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2674 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 285 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2680 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 286 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2686 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 286 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2692 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 287 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2698 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 287 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2704 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 288 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2710 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 288 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2716 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 289 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2722 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 289 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2728 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 289 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2734 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 290 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2740 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 290 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2746 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 291 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2752 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 295 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub((yyvsp[-1].exp), get_pos(arg)); }
#line 2758 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 296 "utils/gwion.y" /* yacc.c:1646  */
    { if((yyvsp[-2].exp)->next){ gwion_error(arg, "invalid format for array init [...][...]..."); free_exp((yyvsp[-2].exp)); free_array_sub((yyvsp[0].array_sub)); YYERROR; } (yyval.array_sub) = prepend_array_sub((yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2764 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 297 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [...][]..."); free_exp((yyvsp[-3].exp)); YYERROR; }
#line 2770 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 301 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(arg)); }
#line 2776 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 302 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2782 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 303 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [][...]"); free_array_sub((yyvsp[-1].array_sub)); free_array_sub((yyvsp[0].array_sub)); YYERROR; }
#line 2788 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 308 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2794 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 310 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.type, ae_flag_static); (yyval.exp) = (yyvsp[0].exp); }
#line 2800 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 312 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.type, ae_flag_private); (yyval.exp) = (yyvsp[0].exp); }
#line 2806 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 316 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2812 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 317 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2818 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 320 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2824 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 324 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sym), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2830 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 326 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_private); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2836 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 328 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].func_def), free_func_def);
      (yyval.func_def) = (yyvsp[0].func_def); SET_FLAG((yyval.func_def), ae_flag_template); }
#line 2843 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 335 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_op , (yyvsp[-2].type_decl), OP_SYM((yyvsp[-3].ival)), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2849 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 337 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_op | ae_flag_unary, (yyvsp[-2].type_decl), OP_SYM((yyvsp[-4].ival)), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2855 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 339 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_dtor, new_type_decl(new_id_list(insert_symbol("void"), get_pos(arg)), 0,
      get_pos(arg)), insert_symbol("dtor"), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2862 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 343 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2868 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 343 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 2874 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 346 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)), (yyvsp[0].ival), get_pos(arg)); }
#line 2880 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 347 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2886 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 348 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)),
      (yyvsp[0].ival), get_pos(arg)); (yyval.type_decl)->types = (yyvsp[-3].type_list); }
#line 2893 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 350 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg));
      (yyval.type_decl)->types = (yyvsp[-5].type_list); }
#line 2900 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 352 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2906 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 353 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].type_decl), ae_flag_const); (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2912 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 357 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2918 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 358 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2924 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 362 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(arg));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sym); }
#line 2930 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 364 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2936 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 366 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2942 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 370 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(arg)); }
#line 2948 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 371 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2954 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 375 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sym), NULL, get_pos(arg)); }
#line 2960 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 376 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2966 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 377 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2972 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 380 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex((yyvsp[-1].exp), get_pos(arg)); }
#line 2978 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 381 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar(  (yyvsp[-1].exp), get_pos(arg)); }
#line 2984 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 382 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec(    (yyvsp[-1].exp), get_pos(arg)); }
#line 2990 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 385 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_if((yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 2996 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 388 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(arg)); }
#line 3002 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 391 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(arg)); }
#line 3008 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 394 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(arg)); }
#line 3014 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 397 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(arg)); }
#line 3020 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 400 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(arg)); }
#line 3026 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 402 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 3032 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 402 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_neq; }
#line 3038 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 405 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3044 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 407 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_lt; }
#line 3050 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 407 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_gt; }
#line 3056 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 408 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_le; }
#line 3062 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 408 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ge; }
#line 3068 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 411 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3074 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 414 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 3080 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 415 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 3086 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 419 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3092 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 421 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3098 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 421 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3104 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 424 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3110 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 426 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3116 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 426 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide; }
#line 3122 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 426 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_percent; }
#line 3128 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 429 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3134 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 432 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_cast((yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(arg)); }
#line 3140 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 434 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3146 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 434 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3152 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 434 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3158 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 435 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3164 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 435 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3170 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 436 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3176 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 436 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3182 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 440 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary((yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3188 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 442 "utils/gwion.y" /* yacc.c:1646  */
    {
      if((yyvsp[0].type_decl)->array && !(yyvsp[0].type_decl)->array->exp_list) {
        free_type_decl((yyvsp[0].type_decl));
        gwion_error(arg, "can't use empty '[]' in 'new' expression");
        YYERROR;
      }
      (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(arg));
    }
#line 3201 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 451 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary3(op_spork, (yyvsp[0].stmt), get_pos(arg)); }
#line 3207 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 454 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3213 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 457 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sym), get_pos(arg)), NULL, get_pos(arg)); }
#line 3219 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 458 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sym), get_pos(arg)), (yyvsp[0].type_list), get_pos(arg)); }
#line 3225 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 461 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 3231 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 461 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3237 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 463 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3243 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 463 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3249 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 466 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array((yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(arg)); }
#line 3255 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 468 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_call((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); (yyval.exp)->d.exp_func.types = (yyvsp[-1].type_list); }
#line 3261 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 470 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dot((yyvsp[-2].exp), (yyvsp[0].sym), get_pos(arg)); }
#line 3267 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 472 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_post((yyvsp[-1].exp), (yyvsp[0].ival), get_pos(arg)); }
#line 3273 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 476 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_id(     (yyvsp[0].sym), get_pos(arg)); }
#line 3279 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 477 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_int(    (yyvsp[0].ival), get_pos(arg)); }
#line 3285 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 478 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_float(  (yyvsp[0].fval), get_pos(arg)); }
#line 3291 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 479 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_string( (yyvsp[0].sval), get_pos(arg)); }
#line 3297 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 480 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_char(   (yyvsp[0].sval), get_pos(arg)); }
#line 3303 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 481 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3309 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 482 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3315 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 483 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_complex((yyvsp[0].c_val), get_pos(arg)); }
#line 3321 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 484 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_polar(  (yyvsp[0].polar), get_pos(arg)); }
#line 3327 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 485 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_vec(    (yyvsp[0].vec), get_pos(arg)); }
#line 3333 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 217:
#line 486 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_hack(   (yyvsp[-1].exp), get_pos(arg)); }
#line 3339 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 218:
#line 487 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) =                       (yyvsp[-1].exp);                    }
#line 3345 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 219:
#line 488 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_nil(        get_pos(arg)); }
#line 3351 "src/ast/parser.c" /* yacc.c:1646  */
    break;


#line 3355 "src/ast/parser.c" /* yacc.c:1646  */
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
#line 490 "utils/gwion.y" /* yacc.c:1906  */

