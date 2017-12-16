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
int gwion_error(Scanner*, const char*);
int gwion_lex(void*, Scanner*);
int get_pos(Scanner*);

#line 82 "src/ast/parser.c" /* yacc.c:339  */

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
#line 15 "utils/gwion.y" /* yacc.c:355  */

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

#line 360 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (Scanner* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 376 "src/ast/parser.c" /* yacc.c:358  */

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
#define YYFINAL  147
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1623

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  106
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  75
/* YYNRULES -- Number of rules.  */
#define YYNRULES  218
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  375

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
       0,    99,    99,   100,   104,   105,   106,   110,   114,   114,
     117,   118,   122,   123,   127,   128,   132,   133,   134,   138,
     139,   143,   144,   148,   149,   153,   154,   158,   159,   163,
     164,   169,   173,   174,   175,   179,   180,   184,   185,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   203,   206,   207,   211,   215,   219,   223,   224,
     228,   232,   234,   236,   238,   240,   242,   244,   249,   251,
     256,   257,   258,   259,   263,   264,   268,   269,   273,   274,
     278,   279,   282,   282,   282,   283,   283,   284,   284,   285,
     285,   286,   286,   287,   287,   287,   288,   288,   288,   289,
     289,   289,   290,   290,   290,   291,   291,   295,   296,   297,
     301,   302,   303,   307,   308,   309,   310,   312,   317,   318,
     321,   321,   324,   326,   328,   330,   335,   335,   338,   339,
     340,   341,   345,   346,   350,   351,   353,   358,   359,   363,
     364,   365,   368,   369,   370,   373,   374,   379,   380,   385,
     386,   391,   392,   397,   398,   403,   404,   408,   408,   410,
     411,   415,   415,   415,   415,   418,   419,   424,   425,   429,
     430,   434,   434,   437,   438,   442,   442,   442,   445,   446,
     451,   452,   456,   456,   456,   457,   457,   458,   458,   462,
     463,   465,   474,   479,   480,   484,   485,   488,   488,   490,
     490,   493,   494,   496,   498,   500,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,   517
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
  "class_ext", "class_decl", "class_body", "class_body2", "class_section",
  "id_list", "id_dot", "stmt_list", "static_decl", "function_decl",
  "func_ptr", "stmt_typedef", "type_decl2", "arg_list", "code_segment",
  "stmt", "id", "opt_id", "enum_stmt", "label_stmt", "goto_stmt",
  "case_stmt", "switch_stmt", "loop_stmt", "selection_stmt", "jump_stmt",
  "exp_stmt", "exp", "binary_exp", "template", "op", "array_exp",
  "array_empty", "decl_exp", "func_args", "decl_template", "func_def",
  "atsym", "type_decl", "decl_list", "union_stmt", "var_decl_list",
  "var_decl", "complex_exp", "polar_exp", "vec_exp", "con_exp",
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
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360
};
# endif

#define YYPACT_NINF -331

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-331)))

#define YYTABLE_NINF -127

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     628,  -331,  -331,  1518,  1091,  1152,   707,  1518,  1518,  -331,
      37,    39,   786,    40,    43,    52,   -40,    54,    55,    56,
     847,    65,    75,  -331,  -331,    -9,    57,  1030,   969,   -40,
    -331,  -331,    81,    -9,    -9,     1,   -40,    85,  1518,   103,
      -9,  -331,  -331,  -331,  -331,  -331,   123,   391,  -331,  -331,
    -331,  -331,  -331,   786,     8,  -331,  -331,  -331,  -331,  -331,
    -331,  -331,  -331,  -331,   125,  1515,  -331,   118,  -331,   -11,
    -331,   -40,  -331,  -331,  -331,  -331,  -331,    11,    74,    63,
      68,    82,   -19,    44,    34,    51,   177,   130,    46,  -331,
     105,   119,  -331,  1518,  1518,   -16,   155,  -331,   158,  -331,
     153,  -331,  1213,   156,   161,   163,  1518,  1518,    21,  1518,
    1518,   908,   174,  1518,  -331,   138,   145,   -40,  -331,   178,
    -331,  -331,  -331,   162,   173,   182,    -9,  -331,  -331,  -331,
    1213,  -331,    -4,  -331,  -331,   126,   141,   188,   -40,    -1,
     -40,   201,   113,  1518,   196,   -40,  -331,  -331,  -331,  -331,
    -331,  -331,  -331,  -331,  -331,  1518,  -331,  -331,  -331,  -331,
    -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,
    -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,
    1518,  1274,  -331,  -331,  -331,  -331,   157,   164,   173,  -331,
     207,  1518,    46,    46,    46,    46,    46,  -331,  -331,    46,
    -331,  -331,  -331,  -331,    46,  -331,  -331,    46,  -331,  -331,
      46,  -331,  -331,  -331,    46,    -9,  -331,    55,  1518,  -331,
    -331,   -40,   -40,   204,  -331,  -331,  -331,  -331,   206,  -331,
    -331,  -331,   215,   216,   219,   221,   222,   224,   908,  -331,
     225,  -331,  -331,   217,   236,  -331,  -331,   118,  -331,   231,
     162,   -40,   182,   232,   -40,   242,   151,   -40,  1518,   244,
     233,  -331,   235,  -331,  -331,  -331,   -40,  -331,    -9,  -331,
     118,   -40,   218,    74,    63,    68,    82,   -19,    44,    34,
      51,   177,   130,  -331,   119,  -331,   154,  1335,  -331,  1396,
    -331,   786,   786,  1518,  1518,   786,   786,  1457,   182,   -40,
     -40,  -331,  -331,  -331,    64,   182,   240,  -331,  -331,  -331,
    -331,  1518,   -40,   162,   197,   -40,  -331,    46,  -331,  -331,
     243,  -331,   226,  -331,   246,   248,  -331,  -331,   786,   249,
    -331,  -331,   261,  -331,  -331,   252,   -40,  -331,   232,  -331,
     264,  -331,   -40,   253,   232,  -331,  -331,   786,   267,   270,
    -331,   786,  -331,  -331,   269,  -331,  -331,  -331,   470,   182,
    -331,  -331,  -331,  -331,    -9,  -331,   258,  -331,   549,  -331,
    -331,  -331,  -331,  -331,  -331
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     120,    75,   184,     0,     0,     0,     0,     0,     0,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   185,   186,     0,     0,     0,   120,     0,
     183,   182,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   207,   208,    52,   209,   210,     0,   120,     6,     4,
      49,    50,    42,    23,   206,    47,    43,    44,    46,    45,
      40,    41,    48,    39,     0,    76,   211,   212,    78,    11,
       5,     0,    51,   213,   214,   215,   113,   145,   147,   149,
     151,   153,   155,   159,   165,   169,   173,   178,     0,   180,
     189,   193,   201,     0,     0,   206,     0,   218,     0,   110,
       0,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   206,    80,   201,     0,    70,     0,
      72,    73,   191,   126,    32,   188,     0,    30,   116,   135,
       0,   117,     0,   123,   136,     0,    21,     0,     0,     0,
       0,   195,     0,     0,     0,     0,   131,     1,     3,    24,
     127,    56,   128,    74,    82,     0,   106,    86,    87,    88,
      89,    90,    85,    83,    91,    92,    84,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
       0,     0,   112,    27,    10,    28,     0,    26,   139,   114,
     137,     0,     0,     0,     0,     0,     0,   157,   158,     0,
     164,   162,   163,   161,     0,   167,   168,     0,   172,   171,
       0,   176,   175,   177,     0,     0,   190,     0,     0,   199,
     200,     0,     0,     0,   202,   205,   216,   217,   107,    38,
     143,   142,     0,     0,     0,     0,     0,     0,     0,    57,
       0,    59,    58,     0,    19,    71,    34,    33,   192,     0,
     126,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   144,     0,    77,    79,   111,     0,    25,     0,   140,
     141,     0,     0,   148,   150,   152,   154,   156,   160,   166,
     170,   174,   179,   181,   194,   204,     0,     0,   203,     0,
     108,     0,     0,     0,     0,     0,     0,     0,     0,    53,
       0,   129,    22,   125,     0,     0,     0,    31,   121,   196,
     115,   132,    53,   126,     9,     0,   138,     0,    81,   197,
       0,   109,    68,    61,     0,     0,    65,    67,     0,     0,
      60,    54,     0,    20,   118,     0,     0,   124,     0,   133,
       0,   130,     0,     0,     0,   146,   198,     0,     0,     0,
      63,     0,    55,   119,    35,    29,   134,     8,   120,     0,
      69,    62,    66,    64,     0,    18,     0,    12,   120,    16,
      17,   122,    36,     7,    15
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -331,   229,  -331,  -330,  -331,  -331,  -331,   -91,  -331,  -120,
    -124,    -5,  -331,  -331,   251,  -331,   -26,   -84,  -123,    -7,
       0,   -33,  -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,
     -97,   117,  -331,  -331,  -331,   -45,   -85,   -18,  -303,   254,
     -25,  -233,   -10,   -30,     4,    12,   -50,  -331,  -331,  -331,
     -27,  -331,    95,   100,   107,   102,  -331,   106,  -331,   104,
    -331,   101,  -331,    99,  -331,    97,    96,  -331,   223,  -331,
    -195,  -331,  -331,   -14,   291
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    46,    47,    48,   343,   186,   366,   367,   368,   243,
     135,    49,   268,   187,    50,    51,   122,   335,    52,    53,
      95,   332,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,   223,   180,    66,    67,    68,   305,    69,
      70,   152,    71,   260,    72,   189,   190,    73,    74,    75,
      76,    77,    78,    79,    80,    81,   199,    82,   204,    83,
     207,    84,   210,    85,   214,    86,    87,    88,    89,    90,
     142,   288,   225,    91,    92
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      54,   103,   248,   133,   115,   108,    54,   138,   139,   128,
     131,   254,    54,   150,   238,   124,   112,   301,   114,   183,
     256,   262,   182,   124,   124,   123,   183,   286,   365,   136,
     146,   129,   134,   123,   123,   355,   141,   150,   365,   247,
     123,   359,   151,   197,   191,   184,   224,    54,   149,   106,
     198,   107,   109,    54,     2,   110,     3,    29,     4,   234,
       5,   235,   309,    43,   111,     3,   113,     4,   120,     5,
     224,   188,   117,     7,     8,   128,   131,   334,   121,   246,
     341,     9,     7,     8,   131,   185,   192,  -126,   114,   125,
      39,    40,   185,   137,    43,   140,    23,    24,    25,    26,
     249,   143,    43,   270,   205,   206,   134,   200,   201,   202,
     203,  -126,   128,    30,    31,   145,   124,   244,   208,   209,
      96,    98,   100,   147,   104,   105,   123,   302,   153,   303,
      29,   -80,   181,   218,   129,   193,   194,   119,   253,   255,
     244,   297,   195,   269,    38,   136,   314,    41,    42,    43,
      44,    45,   218,    38,   196,   144,    41,    42,    43,    44,
      45,   215,   264,    39,    40,   217,   226,    43,   228,   219,
     220,   227,   241,   229,   230,   330,   231,   239,   221,   242,
     333,   245,   337,   290,   211,   212,   213,     5,   219,   220,
     250,   150,   114,   114,   114,   114,   114,   221,     6,   114,
     251,   252,   182,   284,   114,   283,   257,   114,   258,   261,
     114,   266,   271,   222,   114,   123,   287,   114,   357,   267,
     289,   285,   141,   232,   233,   182,   236,   237,   291,   292,
     240,   293,   222,   294,   299,   295,   371,   296,   298,   224,
     310,   300,   315,   254,   304,   307,   308,   311,   313,   318,
     312,   136,   317,   338,   306,   342,   346,   141,   124,   348,
     259,   349,   351,   347,   352,   353,   244,   356,   123,   358,
     361,   188,   263,   362,   364,   373,   148,   374,   127,   340,
     372,   339,   132,   316,   322,   323,   354,   273,   326,   327,
     345,    54,    54,   274,   336,    54,    54,   276,   100,   331,
     244,   275,   277,   278,   123,   279,   280,   281,   272,   116,
     282,   216,   331,     0,     0,   344,     0,   114,     0,     0,
       0,   350,     0,     0,     0,     0,     0,     0,    54,     0,
       0,     0,     0,   370,     0,   100,   188,     0,     0,     0,
     360,     0,   136,   370,   363,     0,     0,    54,     0,     0,
       0,    54,     0,   369,   336,     0,     0,     0,    54,     0,
       0,     0,     0,   369,   123,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    -2,     0,     0,     1,     0,     0,     0,     0,     2,
       0,     3,     0,     4,   320,     5,   100,     6,     0,     0,
     324,   325,     0,     0,   329,     0,     0,     0,     7,     8,
       0,     0,     0,     0,     0,     0,     9,    10,   259,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,     0,    28,     0,
       0,     0,     0,     0,     0,     0,     0,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,    32,    33,    34,
       0,     0,     0,     1,     0,     0,     0,     0,     2,     0,
       3,     0,     4,    35,     5,    36,     6,   -13,    37,    38,
      39,    40,    41,    42,    43,    44,    45,     7,     8,     0,
       0,     0,     0,     0,     0,     9,    10,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,     0,    28,     0,     0,
       0,     0,     0,     0,     0,     0,    29,    30,    31,     0,
       0,     0,     0,     0,     0,     0,    32,    33,    34,     0,
       0,     0,     1,     0,     0,     0,     0,     2,     0,     3,
       0,     4,    35,     5,    36,     6,   -14,    37,    38,    39,
      40,    41,    42,    43,    44,    45,     7,     8,     0,     0,
       0,     0,     0,     0,     9,    10,     0,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,     0,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    30,    31,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,     0,     0,
       0,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,    35,     5,    36,     6,     0,    37,    38,    39,    40,
      41,    42,    43,    44,    45,     7,     8,     0,     0,     0,
       0,     0,     0,     9,    10,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,     0,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     0,     0,     0,
       0,     0,     0,     0,    32,    33,    34,     0,     0,     0,
       1,     0,     0,     0,     0,     2,     0,     3,     0,     4,
      35,     5,    36,     6,   101,    37,    38,    39,    40,    41,
      42,    43,    44,    45,     7,     8,     0,     0,     0,     0,
       0,     0,     9,    10,     0,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,     0,   102,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,     0,     0,     0,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,     0,
       5,    36,     6,     0,    37,    38,    39,    40,    41,    42,
      43,    44,    45,     7,     8,     0,     0,     0,     0,     0,
       0,     9,    10,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,     0,   102,     0,     0,     0,     0,     0,     0,
     118,     0,    29,    30,    31,     2,     0,     3,     0,     4,
       0,     5,     0,     0,    34,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
      36,     0,     9,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     0,     0,     0,     0,     0,    23,    24,    25,
      26,     0,    93,     0,    94,     0,     0,     0,     0,     0,
       0,     1,     0,    29,    30,    31,     2,     0,     3,     0,
       4,     0,     5,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,    36,     0,     9,     0,    38,    39,    40,    41,    42,
      43,    44,    45,     0,     0,     0,     0,     0,    23,    24,
      25,    26,     0,    93,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     2,     0,     3,
       0,     4,     0,     5,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,    36,     0,     9,     0,    38,    39,    40,    41,
      42,    43,    44,    45,     0,     0,     0,     0,     0,    23,
      24,    25,    26,     0,   130,     0,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    30,    31,     2,     0,
       3,     0,     4,     0,     5,    32,    33,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,    35,     0,    36,     0,     9,    37,    38,    39,    40,
      41,    42,    43,    44,    45,     0,     0,     0,     0,     0,
      23,    24,    25,    26,     0,    27,     0,   102,     0,     0,
       0,     0,     0,     0,     0,     0,    29,    30,    31,     2,
       0,     3,     0,     4,    97,     5,     0,     0,   126,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
       0,     0,     0,     0,    36,     0,     9,    37,    38,    39,
      40,    41,    42,    43,    44,    45,     0,     0,     0,     0,
       0,    23,    24,    25,    26,     0,    93,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    29,    30,    31,
       2,     0,     3,     0,     4,     0,     5,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,    36,     0,     9,     0,    38,
      39,    40,    41,    42,    43,    44,    45,     0,     0,     0,
       0,     0,    23,    24,    25,    26,     0,    93,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    29,    30,
      31,     2,     0,     3,     0,     4,     0,     5,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,    36,     0,     9,     0,
      38,    39,    40,    41,    42,    43,    44,    45,     0,     0,
       0,     0,     0,    23,    24,    25,    26,     0,   130,     0,
     102,     0,     0,     0,     0,     0,     0,     0,     0,    29,
      30,    31,     2,     0,     3,     0,     4,     0,     5,   265,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,    36,     0,     9,
      37,    38,    39,    40,    41,    42,    43,    44,    45,     0,
       0,     0,     0,     0,    23,    24,    25,    26,     0,    93,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      29,    30,    31,     2,     0,     3,     0,     4,   319,     5,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,    36,     0,
       9,     0,    38,    39,    40,    41,    42,    43,    44,    45,
       0,     0,     0,     0,     0,    23,    24,    25,    26,     0,
      93,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    29,    30,    31,     2,     0,     3,     0,     4,     0,
       5,   321,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,    36,
       0,     9,     0,    38,    39,    40,    41,    42,    43,    44,
      45,     0,     0,     0,     0,     0,    23,    24,    25,    26,
       0,    93,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    29,    30,    31,     2,     0,     3,     0,     4,
     328,     5,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
      36,     0,     9,     0,    38,    39,    40,    41,    42,    43,
      44,    45,     0,     0,     0,     0,     0,    23,    24,    25,
      26,     0,    93,     0,    94,     0,     0,     0,     0,   154,
     155,   156,     0,    29,    30,    31,     2,     0,     3,     0,
       4,     0,     5,   157,   158,   159,   160,   161,   162,   163,
     164,   165,     0,     0,     0,     7,     8,     0,     0,     0,
       0,    36,     0,     9,     0,    38,    39,    40,    41,    42,
      43,    44,    45,     0,     0,     0,     0,     0,    23,    24,
      25,    26,     0,    93,     0,    94,     0,   166,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     0,     0,     0,
       0,     0,     0,     0,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,     0,     0,     0,
       0,     0,    36,     0,     0,     0,    38,    39,    40,    41,
      42,    43,    44,    45
};

static const yytype_int16 yycheck[] =
{
       0,     6,   125,    28,    18,    12,     6,    33,    34,    27,
      28,    12,    12,    29,   111,    25,    16,   250,    18,    30,
     140,   145,    67,    33,    34,    25,    30,   222,   358,    29,
      40,    27,    28,    33,    34,   338,    36,    29,   368,   124,
      40,   344,    34,    62,    33,    56,    91,    47,    53,    12,
      69,    12,    12,    53,     8,    12,    10,    66,    12,    38,
      14,    40,   257,   103,    12,    10,    12,    12,     3,    14,
     115,    71,    16,    27,    28,    93,    94,    13,     3,   124,
     313,    35,    27,    28,   102,    96,    75,   103,    88,    32,
      99,   100,    96,    12,   103,    94,    50,    51,    52,    53,
     126,    16,   103,   188,    70,    71,   102,    63,    64,    65,
      66,   103,   130,    67,    68,    12,   126,   117,    67,    68,
       3,     4,     5,     0,     7,     8,   126,   251,     3,   252,
      66,    12,    14,    14,   130,    61,    73,    20,   138,   139,
     140,   238,    74,   188,    98,   145,   266,   101,   102,   103,
     104,   105,    14,    98,    72,    38,   101,   102,   103,   104,
     105,    31,   180,    99,   100,    60,    11,   103,    15,    50,
      51,    13,    34,    17,    13,   298,    13,     3,    59,    34,
     300,     3,   305,   228,     7,     8,     9,    14,    50,    51,
      64,    29,   192,   193,   194,   195,   196,    59,    16,   199,
      59,    13,   247,   217,   204,   215,     5,   207,    95,    13,
     210,    54,     5,    94,   214,   215,    12,   217,   342,    55,
      14,   221,   222,   106,   107,   270,   109,   110,    13,    13,
     113,    12,    94,    12,    17,    13,   359,    13,    13,   284,
     258,     5,   268,    12,    12,     3,    95,     3,    13,    95,
      17,   251,    34,    13,   254,    58,    13,   257,   268,    13,
     143,    13,    13,    37,     3,    13,   266,     3,   268,    16,
       3,   271,   155,     3,     5,    17,    47,   368,    27,   312,
     364,   311,    28,   271,   291,   292,   336,   192,   295,   296,
     317,   291,   292,   193,   304,   295,   296,   195,   181,   299,
     300,   194,   196,   199,   304,   204,   207,   210,   191,    18,
     214,    88,   312,    -1,    -1,   315,    -1,   317,    -1,    -1,
      -1,   328,    -1,    -1,    -1,    -1,    -1,    -1,   328,    -1,
      -1,    -1,    -1,   358,    -1,   218,   336,    -1,    -1,    -1,
     347,    -1,   342,   368,   351,    -1,    -1,   347,    -1,    -1,
      -1,   351,    -1,   358,   364,    -1,    -1,    -1,   358,    -1,
      -1,    -1,    -1,   368,   364,    -1,    -1,    -1,   368,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     0,    -1,    -1,     3,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    12,   287,    14,   289,    16,    -1,    -1,
     293,   294,    -1,    -1,   297,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    36,   311,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    -1,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    78,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    12,    92,    14,    94,    16,    17,    97,    98,
      99,   100,   101,   102,   103,   104,   105,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    78,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    12,    92,    14,    94,    16,    17,    97,    98,    99,
     100,   101,   102,   103,   104,   105,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    77,    78,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    92,    14,    94,    16,    -1,    97,    98,    99,   100,
     101,   102,   103,   104,   105,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    77,    78,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,
      92,    14,    94,    16,    17,    97,    98,    99,   100,   101,
     102,   103,   104,   105,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    -1,
      14,    94,    16,    -1,    97,    98,    99,   100,   101,   102,
     103,   104,   105,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    66,    67,    68,     8,    -1,    10,    -1,    12,
      -1,    14,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      94,    -1,    35,    97,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,
      53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    66,    67,    68,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    94,    -1,    35,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    94,    -1,    35,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,     8,    -1,
      10,    -1,    12,    -1,    14,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    92,    -1,    94,    -1,    35,    97,    98,    99,   100,
     101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,
      50,    51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,     8,
      -1,    10,    -1,    12,    13,    14,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    94,    -1,    35,    97,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,
      -1,    50,    51,    52,    53,    -1,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
       8,    -1,    10,    -1,    12,    -1,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    94,    -1,    35,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,     8,    -1,    10,    -1,    12,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    94,    -1,    35,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      67,    68,     8,    -1,    10,    -1,    12,    -1,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    94,    -1,    35,
      97,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,     8,    -1,    10,    -1,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    94,    -1,
      35,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,     8,    -1,    10,    -1,    12,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    94,
      -1,    35,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,     8,    -1,    10,    -1,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      94,    -1,    35,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,
      53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,     4,
       5,     6,    -1,    66,    67,    68,     8,    -1,    10,    -1,
      12,    -1,    14,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    94,    -1,    35,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    35,
      36,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    55,    57,    66,
      67,    68,    76,    77,    78,    92,    94,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   107,   108,   109,   117,
     120,   121,   124,   125,   126,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   141,   142,   143,   145,
     146,   148,   150,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   163,   165,   167,   169,   171,   172,   173,   174,
     175,   179,   180,    55,    57,   126,   137,    13,   137,    15,
     137,    17,    57,   117,   137,   137,    12,    12,   125,    12,
      12,    12,   126,    12,   126,   179,   180,    16,     3,   137,
       3,     3,   122,   126,   148,    32,    78,   120,   143,   150,
      55,   143,   145,   146,   150,   116,   126,    12,   122,   122,
      94,   126,   176,    16,   137,    12,   148,     0,   107,   117,
      29,    34,   147,     3,     4,     5,     6,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    62,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
     140,    14,   141,    30,    56,    96,   111,   119,   126,   151,
     152,    33,    75,    61,    73,    74,    72,    62,    69,   162,
      63,    64,    65,    66,   164,    70,    71,   166,    67,    68,
     168,     7,     8,     9,   170,    31,   174,    60,    14,    50,
      51,    59,    94,   139,   141,   178,    11,    13,    15,    17,
      13,    13,   137,   137,    38,    40,   137,   137,   136,     3,
     137,    34,    34,   115,   126,     3,   141,   142,   124,   122,
      64,    59,    13,   126,    12,   126,   115,     5,    95,   137,
     149,    13,   116,   137,   143,    15,    54,    55,   118,   141,
     142,     5,   137,   158,   159,   160,   161,   163,   165,   167,
     169,   171,   172,   148,   179,   126,   176,    12,   177,    14,
     141,    13,    13,    12,    12,    13,    13,   136,    13,    17,
       5,   147,   116,   124,    12,   144,   126,     3,    95,   176,
     143,     3,    17,    13,   115,   122,   151,    34,    95,    13,
     137,    15,   125,   125,   137,   137,   125,   125,    13,   137,
     124,   126,   127,   115,    13,   123,   148,   124,    13,   149,
     127,   147,    58,   110,   126,   156,    13,    37,    13,    13,
     125,    13,     3,    13,   152,   144,     3,   116,    16,   144,
     125,     3,     3,   125,     5,   109,   112,   113,   114,   117,
     146,   124,   123,    17,   113
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   106,   107,   107,   108,   108,   108,   109,   110,   110,
     111,   111,   112,   112,   113,   113,   114,   114,   114,   115,
     115,   116,   116,   117,   117,   118,   118,   119,   119,   120,
     120,   121,   122,   122,   122,   123,   123,   124,   124,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   126,   127,   127,   128,   129,   130,   131,   131,
     132,   133,   133,   133,   133,   133,   133,   133,   134,   134,
     135,   135,   135,   135,   136,   136,   137,   137,   138,   138,
     139,   139,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   141,   141,   141,
     142,   142,   142,   143,   143,   143,   143,   143,   144,   144,
     145,   145,   146,   146,   146,   146,   147,   147,   148,   148,
     148,   148,   149,   149,   150,   150,   150,   151,   151,   152,
     152,   152,   153,   154,   155,   156,   156,   157,   157,   158,
     158,   159,   159,   160,   160,   161,   161,   162,   162,   163,
     163,   164,   164,   164,   164,   165,   165,   166,   166,   167,
     167,   168,   168,   169,   169,   170,   170,   170,   171,   171,
     172,   172,   173,   173,   173,   173,   173,   173,   173,   174,
     174,   174,   174,   175,   175,   176,   176,   177,   177,   178,
     178,   179,   179,   179,   179,   179,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     8,     2,     0,
       1,     0,     1,     0,     1,     2,     1,     1,     1,     1,
       3,     1,     3,     1,     2,     1,     0,     1,     1,     6,
       2,     4,     1,     2,     2,     2,     4,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     6,     2,     3,     3,     3,
       5,     5,     7,     6,     7,     5,     7,     5,     5,     7,
       2,     3,     2,     2,     2,     1,     1,     3,     1,     3,
       0,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     4,     5,
       2,     3,     2,     1,     2,     4,     2,     2,     2,     3,
       0,     4,     7,     2,     5,     4,     0,     1,     2,     4,
       5,     2,     2,     3,     6,     2,     2,     1,     3,     1,
       2,     2,     3,     3,     3,     1,     5,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     2,     3,     1,     3,     1,     3,     2,     3,     1,
       1,     1,     2,     3,     3,     2,     1,     1,     1,     1,
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
#line 92 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_def(((*yyvaluep).class_def)); }
#line 1750 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 112: /* class_body  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1756 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 113: /* class_body2  */
#line 93 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1762 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 120: /* func_ptr  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1768 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 121: /* stmt_typedef  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1774 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 122: /* type_decl2  */
#line 94 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1780 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 124: /* code_segment  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1786 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 125: /* stmt  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1792 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 128: /* enum_stmt  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1798 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 129: /* label_stmt  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1804 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 130: /* goto_stmt  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1810 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 131: /* case_stmt  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1816 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 132: /* switch_stmt  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1822 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 133: /* loop_stmt  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1828 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 134: /* selection_stmt  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1834 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 135: /* jump_stmt  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1840 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 136: /* exp_stmt  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1846 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 139: /* template  */
#line 95 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 1852 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 148: /* type_decl  */
#line 94 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1858 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 150: /* union_stmt  */
#line 91 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1864 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 176: /* type_list  */
#line 95 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 1870 "src/ast/parser.c" /* yacc.c:1257  */
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
#line 99 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL, get_pos(arg));  }
#line 2138 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 100 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(arg)); }
#line 2144 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 104 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(arg)); }
#line 2150 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 105 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(arg)); }
#line 2156 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 106 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(arg)); }
#line 2162 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 111 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def((yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].id_list), (yyvsp[-1].class_body), get_pos(arg)); (yyval.class_def)->types = (yyvsp[-7].id_list); }
#line 2168 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 114 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[0].id_list); }
#line 2174 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 114 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2180 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 117 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_public; }
#line 2186 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 118 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_private; }
#line 2192 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 122 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2198 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 123 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2204 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 127 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body((yyvsp[0].section), get_pos(arg)); }
#line 2210 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 128 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body((yyvsp[-1].section), (yyvsp[0].class_body), get_pos(arg)); }
#line 2216 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 132 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(arg)); }
#line 2222 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 133 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def((yyvsp[0].func_def), get_pos(arg)); }
#line 2228 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 134 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(arg)); }
#line 2234 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 138 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2240 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 139 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2246 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 143 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2252 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 144 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2258 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 148 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(arg));}
#line 2264 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 149 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(arg));}
#line 2270 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 153 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_static;   }
#line 2276 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 154 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2282 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 158 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2288 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 159 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2294 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 163 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-2].sym), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(arg)); }
#line 2300 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 165 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_ptr), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2306 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 170 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_typedef((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2312 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 173 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2318 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 174 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(arg)); }
#line 2324 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 175 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(arg)); }
#line 2330 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 179 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(arg)); }
#line 2336 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 180 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(arg)); }
#line 2342 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 184 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code(NULL, get_pos(arg)); }
#line 2348 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 185 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code((yyvsp[-1].stmt_list), get_pos(arg)); }
#line 2354 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 203 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = insert_symbol((yyvsp[0].sval)); }
#line 2360 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 206 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = NULL; }
#line 2366 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 207 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = (yyvsp[0].sym);  }
#line 2372 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 211 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sym), get_pos(arg)); }
#line 2378 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 215 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 1, get_pos(arg)); }
#line 2384 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 219 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 0, get_pos(arg)); }
#line 2390 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 223 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(arg)); }
#line 2396 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 224 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(arg)); }
#line 2402 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 228 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg));}
#line 2408 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 233 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while((yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2414 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 235 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while((yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2420 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 237 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2426 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 239 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2432 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 241 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until((yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2438 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 243 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until((yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2444 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 245 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2450 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 250 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(arg)); }
#line 2456 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 252 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(arg)); }
#line 2462 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 256 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return(NULL, get_pos(arg)); }
#line 2468 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 257 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return((yyvsp[-1].exp), get_pos(arg)); }
#line 2474 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 258 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_break(get_pos(arg)); }
#line 2480 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 259 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_continue(get_pos(arg)); }
#line 2486 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 263 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp((yyvsp[-1].exp),   get_pos(arg)); }
#line 2492 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 264 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp(NULL, get_pos(arg)); }
#line 2498 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 268 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2504 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 269 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_exp((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 2510 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 273 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2516 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 274 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2522 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 278 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 2528 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2534 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2540 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2546 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2552 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2558 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2564 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2570 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2576 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 285 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2582 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 285 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2588 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 286 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2594 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 286 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2600 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 287 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2606 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 287 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2612 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 287 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2618 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 288 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2624 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 288 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2630 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 288 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2636 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 289 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2642 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 289 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2648 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 289 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2654 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 290 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2660 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 290 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2666 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 290 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2672 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 291 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2678 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 291 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2684 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 295 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub((yyvsp[-1].exp), get_pos(arg)); }
#line 2690 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 296 "utils/gwion.y" /* yacc.c:1646  */
    { if((yyvsp[-2].exp)->next){ gwion_error(arg, "invalid format for array init [...][...]..."); free_exp((yyvsp[-2].exp)); free_array_sub((yyvsp[0].array_sub)); YYERROR; } (yyval.array_sub) = prepend_array_sub((yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2696 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 297 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [...][]..."); free_exp((yyvsp[-3].exp)); YYERROR; }
#line 2702 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 301 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(arg)); }
#line 2708 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 302 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2714 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 303 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [][...]"); free_array_sub((yyvsp[-1].array_sub)); free_array_sub((yyvsp[0].array_sub)); YYERROR; }
#line 2720 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 308 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2726 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 309 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); (yyval.exp)->d.exp_decl.types = (yyvsp[-2].type_list); }
#line 2732 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 311 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.type, ae_flag_static); (yyval.exp) = (yyvsp[0].exp); }
#line 2738 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 313 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.type, ae_flag_private); (yyval.exp) = (yyvsp[0].exp); }
#line 2744 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 317 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2750 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 318 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2756 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 321 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2762 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 321 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2768 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 325 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sym), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); (yyval.func_def)->types = (yyvsp[-6].id_list); if((yyvsp[-6].id_list)) SET_FLAG((yyval.func_def), ae_flag_template);}
#line 2774 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 327 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_private); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2780 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 329 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_static | ae_flag_op , (yyvsp[-3].type_decl), (yyvsp[-2].sym), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2786 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 331 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_dtor, new_type_decl(new_id_list(insert_symbol("void"), get_pos(arg)), 0,
      get_pos(arg)), insert_symbol("dtor"), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2793 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 335 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2799 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 335 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 2805 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 338 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)), (yyvsp[0].ival), get_pos(arg)); }
#line 2811 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 339 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2817 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 340 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2823 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 341 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].type_decl), ae_flag_const); (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2829 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 345 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2835 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 346 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2841 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 350 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(arg));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sym); }
#line 2847 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 352 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2853 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 354 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2859 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 358 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(arg)); }
#line 2865 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 359 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2871 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 363 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sym), NULL, get_pos(arg)); }
#line 2877 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 364 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2883 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 365 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2889 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 368 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex((yyvsp[-1].exp), get_pos(arg)); }
#line 2895 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 369 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar(  (yyvsp[-1].exp), get_pos(arg)); }
#line 2901 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 370 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec(    (yyvsp[-1].exp), get_pos(arg)); }
#line 2907 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 375 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_if((yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 2913 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 379 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2919 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 381 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(arg)); }
#line 2925 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 385 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2931 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 387 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(arg)); }
#line 2937 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 391 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2943 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 393 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(arg)); }
#line 2949 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 397 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2955 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 399 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(arg)); }
#line 2961 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 403 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2967 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 405 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(arg)); }
#line 2973 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 408 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2979 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 408 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_neq; }
#line 2985 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 410 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2991 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 412 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2997 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 415 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_lt; }
#line 3003 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 415 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_gt; }
#line 3009 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 415 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_le; }
#line 3015 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 415 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ge; }
#line 3021 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 418 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3027 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 420 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3033 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 424 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 3039 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 425 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 3045 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 429 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3051 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 431 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3057 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 434 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3063 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 434 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3069 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 437 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3075 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 439 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3081 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 442 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3087 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 442 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide; }
#line 3093 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 442 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_percent; }
#line 3099 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 445 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3105 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 447 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3111 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 453 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_cast((yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(arg)); }
#line 3117 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 456 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3123 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 456 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3129 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 456 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3135 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 457 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3141 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 457 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3147 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 458 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3153 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 458 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3159 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 462 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 3165 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 464 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary((yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3171 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 466 "utils/gwion.y" /* yacc.c:1646  */
    {
      if((yyvsp[0].type_decl)->array && !(yyvsp[0].type_decl)->array->exp_list) {
        free_type_decl((yyvsp[0].type_decl));
        gwion_error(arg, "can't use empty '[]' in 'new' expression");
        YYERROR;
      }
      (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(arg));
    }
#line 3184 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 475 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary3(op_spork, (yyvsp[0].stmt), get_pos(arg)); }
#line 3190 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 480 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3196 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 484 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sym), get_pos(arg)), NULL, get_pos(arg)); }
#line 3202 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 485 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sym), get_pos(arg)), (yyvsp[0].type_list), get_pos(arg)); }
#line 3208 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 488 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 3214 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 488 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3220 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 490 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3226 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 490 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3232 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 495 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array((yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(arg)); }
#line 3238 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 497 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_call((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); (yyval.exp)->d.exp_func.types = (yyvsp[-1].type_list); }
#line 3244 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 499 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dot((yyvsp[-2].exp), (yyvsp[0].sym), get_pos(arg)); }
#line 3250 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 501 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_post((yyvsp[-1].exp), (yyvsp[0].ival), get_pos(arg)); }
#line 3256 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 505 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_id(     (yyvsp[0].sym), get_pos(arg)); }
#line 3262 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 506 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_int(    (yyvsp[0].ival), get_pos(arg)); }
#line 3268 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 507 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_float(  (yyvsp[0].fval), get_pos(arg)); }
#line 3274 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 508 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_string( (yyvsp[0].sval), get_pos(arg)); }
#line 3280 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 509 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_char(   (yyvsp[0].sval), get_pos(arg)); }
#line 3286 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 510 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3292 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 511 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3298 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 512 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_complex((yyvsp[0].c_val), get_pos(arg)); }
#line 3304 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 513 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_polar(  (yyvsp[0].polar), get_pos(arg)); }
#line 3310 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 514 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_vec(    (yyvsp[0].vec), get_pos(arg)); }
#line 3316 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 515 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_hack(   (yyvsp[-1].exp), get_pos(arg)); }
#line 3322 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 217:
#line 516 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) =                       (yyvsp[-1].exp);                    }
#line 3328 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 218:
#line 517 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_nil(        get_pos(arg)); }
#line 3334 "src/ast/parser.c" /* yacc.c:1646  */
    break;


#line 3338 "src/ast/parser.c" /* yacc.c:1646  */
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
#line 519 "utils/gwion.y" /* yacc.c:1906  */

