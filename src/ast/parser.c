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
#define YYFINAL  209
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1949

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  100
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  72
/* YYNRULES -- Number of rules.  */
#define YYNRULES  224
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  438

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
     139,   140,   149,   151,   153,   158,   160,   164,   165,   166,
     170,   171,   175,   176,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   195,   197,   197,
     200,   201,   202,   203,   207,   211,   215,   216,   220,   224,
     226,   228,   230,   232,   234,   236,   238,   240,   245,   247,
     252,   253,   254,   255,   259,   260,   264,   265,   269,   270,
     273,   273,   275,   275,   275,   276,   276,   277,   277,   278,
     278,   279,   279,   280,   280,   280,   281,   281,   282,   286,
     287,   288,   292,   293,   294,   298,   299,   300,   302,   304,
     309,   310,   314,   315,   318,   321,   323,   325,   327,   329,
     342,   342,   342,   342,   342,   342,   344,   345,   347,   349,
     354,   354,   357,   358,   359,   361,   363,   364,   368,   369,
     373,   374,   376,   378,   380,   385,   386,   390,   391,   392,
     395,   395,   398,   398,   401,   401,   404,   404,   407,   407,
     410,   410,   413,   413,   415,   415,   418,   418,   419,   419,
     421,   421,   425,   426,   429,   429,   432,   432,   434,   434,
     437,   437,   437,   439,   439,   442,   442,   445,   445,   445,
     446,   446,   447,   447,   450,   450,   452,   461,   464,   464,
     468,   469,   472,   472,   474,   474,   476,   476,   478,   481,
     483,   487,   488,   489,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   502
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
  "func_ptr", "stmt_type", "type_decl2", "arg_list", "code_segment",
  "stmt", "id", "opt_id", "enum_stmt", "label_stmt", "goto_stmt",
  "case_stmt", "switch_stmt", "loop_stmt", "selection_stmt", "jump_stmt",
  "exp_stmt", "exp", "binary_exp", "template", "op", "array_exp",
  "array_empty", "decl_exp", "func_args", "arg_type", "decl_template",
  "func_def_base", "op_op", "func_def", "atsym", "type_decl", "decl_list",
  "union_stmt", "var_decl_list", "var_decl", "con_exp", "log_or_exp",
  "log_and_exp", "inc_or_exp", "exc_or_exp", "and_exp", "eq_op", "eq_exp",
  "rel_op", "relational_exp", "shift_op", "shift_exp", "add_op", "add_exp",
  "mul_op", "mul_exp", "cast_exp", "unary_op", "unary_exp", "dur_exp",
  "type_list", "call_paren", "post_op", "post_exp", "vec_type",
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

#define YYPACT_NINF -376

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-376)))

#define YYTABLE_NINF -131

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     609,  -376,  -376,  1695,  1389,  1440,   706,  -376,  -376,   117,
    -376,    13,    19,   803,    27,    50,    60,   -75,    61,    92,
      68,   928,    89,   104,  -376,  -376,   117,    78,   -75,  1030,
     460,  1104,  1221,   -75,  -376,  -376,   110,  1750,   117,    29,
     117,   118,  -376,   130,   117,  -376,  -376,  -376,  -376,  -376,
     143,   609,  -376,  -376,  -376,  -376,  -376,   803,     8,  -376,
    -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,   154,  1819,
    -376,   135,  -376,   115,  -376,  -376,   -75,  -376,  -376,    -6,
      98,    91,    88,    93,    17,   146,    23,    44,   242,   137,
     484,  -376,   113,    49,  1695,  -376,  1695,  1695,  1695,    -8,
     164,   536,  -376,   165,  -376,   166,  -376,  1272,   624,  1221,
     168,   -75,   170,   178,  1695,  1695,     9,  1695,  1695,   877,
     191,  1695,  -376,    64,   161,   -75,  -376,   198,  -376,  -376,
    -376,   189,   156,   212,  1155,   306,  1155,  1338,   117,  -376,
     163,  -376,  -376,   306,   404,   510,    72,   117,  -376,  -376,
    -376,  -376,   510,  -376,  -376,  -376,  -376,  1338,    72,  -376,
    -376,  -376,   159,   167,   213,  -376,  -376,  -376,  -376,  -376,
    -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,
    -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,
    -376,  -376,  -376,  -376,  -376,  -376,   117,  -376,  -376,  -376,
    -376,  -376,    -1,   -75,   229,   155,  1695,   -75,  -376,  -376,
    -376,  -376,  -376,  -376,  -376,  -376,  1695,  1695,  1491,  -376,
     163,   115,   163,  -376,  -376,   178,  -376,   240,  1695,   536,
     536,   536,   536,   536,  -376,  -376,   536,   536,   536,   536,
     536,   117,   117,  -376,    92,  1695,   -75,   117,   241,  -376,
    -376,   239,  -376,  -376,   243,    97,  1338,    97,    72,  -376,
     244,  -376,   135,   245,   246,   248,   251,   252,   253,   -75,
     -75,   979,  -376,   258,  -376,  -376,   256,  -376,  -376,   117,
     259,   -75,   306,    97,   264,   163,    11,   170,   -75,   189,
     244,   -75,   274,   196,   117,   -22,   282,   270,   276,  -376,
    -376,  -376,  -376,   135,   -75,   262,    98,    91,    88,    93,
      17,   146,    23,    44,   242,   137,  -376,   244,    49,  -376,
     204,  1542,  -376,  -376,  1593,  -376,    97,   117,    22,   803,
     803,  1695,  1695,   803,   803,   263,   268,  1644,   189,   -75,
    -376,   609,  -376,   -75,   -75,   290,  -376,  -376,  -376,   285,
     292,  -376,  -376,  -376,   -75,   170,  1695,   -75,   170,  -376,
     536,   293,  -376,  -376,   295,  -376,  -376,   -75,  -376,   249,
     189,   266,  -376,   297,   298,  -376,  -376,  1695,  1695,   803,
     299,  -376,  -376,   312,   609,   300,  -376,   303,   307,  -376,
     189,   244,   260,  -376,  -376,   318,  -376,  -376,   189,  -376,
     319,   271,  -376,   803,   323,   329,  1842,  1865,  -376,   803,
    -376,  -376,  -376,   244,   244,  -376,    22,   170,  -376,  -376,
     117,   322,  -376,  -376,  -376,   803,   803,  -376,    22,    22,
    -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    75,   189,     0,     0,     0,     0,   212,   211,     0,
     192,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   190,   191,     0,     0,     0,     0,
       0,     0,     0,     0,   188,   187,     0,     0,     0,     0,
       0,     0,   213,     0,     0,   215,   216,    47,   217,   218,
       0,     2,     6,     4,    44,    45,    37,    20,   214,    42,
      38,    39,    41,    40,    35,    36,    43,    34,     0,    76,
     219,   220,    78,     0,   126,     5,     0,    46,   105,   150,
     152,   154,   156,   158,   160,   164,   170,   174,   178,   183,
       0,   185,   194,   198,     0,   206,     0,     0,     0,   214,
       0,     0,   224,     0,   102,     0,    32,     0,     0,     0,
       0,     0,   130,    27,     0,     0,     0,     0,     0,     0,
       0,     0,   214,    80,   206,     0,    70,     0,    72,    73,
     196,   193,    11,    16,     0,     0,     0,     0,     0,   107,
       0,   117,   141,     0,     0,     0,     0,     0,     8,    53,
     118,   144,     0,    51,   108,   116,   142,     0,     0,    52,
     109,   143,     0,    18,     0,    82,    98,   181,   180,   182,
      86,    87,    88,    89,    90,    85,    83,    91,    92,   204,
     205,    84,   169,   167,   168,   166,   177,   176,   172,   173,
      93,    94,    95,    96,    97,   120,     0,   123,   121,   125,
     124,   122,     0,     0,   200,     0,     0,     0,   137,     1,
       3,    21,   131,    54,   132,    74,     0,     0,     0,   104,
       0,     0,     0,     9,   119,   147,   106,   145,     0,     0,
       0,     0,     0,     0,   162,   163,     0,     0,     0,     0,
       0,     0,     0,   195,     0,     0,     0,     0,     0,   207,
     210,     0,   222,   223,    99,     0,     0,     0,     0,    33,
       0,    29,    28,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,    57,    56,     0,    71,   197,     0,
       0,     0,     0,     0,     0,     0,     0,   130,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    77,
      79,   103,   148,   149,     0,     0,   153,   155,   157,   159,
     161,   165,   171,   175,   179,   184,   186,     0,   199,   209,
       0,     0,   208,   221,     0,   100,     0,   110,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      10,    13,    17,     0,     0,     0,   133,    19,   129,     0,
       0,    25,   114,   201,     0,   130,   138,    48,   130,   146,
       0,     0,    81,   202,     0,   101,   111,     0,   112,     0,
       0,    68,    59,     0,     0,    65,    67,     0,     0,     0,
       0,    58,    49,     0,    14,     0,    12,     0,     0,    26,
       0,     0,     0,   134,   139,     0,   136,   151,     0,   203,
      30,     0,   115,     0,     0,     0,     0,     0,    61,     0,
      50,    15,     7,     0,     0,   127,     0,   130,   140,   128,
       0,     0,    69,    60,    66,     0,     0,    62,     0,     0,
      22,   135,    31,   113,    63,    64,    23,    24
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -376,   286,  -305,   -23,  -376,  -376,   -44,  -117,  -195,    12,
    -376,  -376,    59,   -72,  -116,   -11,     0,    -4,   339,  -376,
    -376,  -376,  -376,  -376,  -376,  -376,  -109,    62,  -246,  -376,
     321,    10,   -99,   373,  -274,  -375,     4,     1,  -376,  -376,
    -267,    20,     3,   320,    51,    -2,     6,  -376,   127,   142,
     144,   141,  -376,   145,   337,   140,   340,   150,   343,   152,
     344,   157,   160,     5,   -42,  -376,  -223,  -376,   345,   -16,
    -376,   378
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    50,    51,    52,   280,   385,   386,   132,   162,    53,
      54,    55,   204,   366,    56,    57,    99,   383,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,   248,
     217,    70,    71,    72,   328,   370,   140,   150,   196,    75,
     214,    76,   297,    77,   226,   227,    78,    79,    80,    81,
      82,    83,   236,    84,   237,    85,   238,    86,   239,    87,
     240,    88,    89,   101,    91,    92,   205,   322,   250,    93,
      94,    95
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,    74,   116,   123,    73,    90,    58,   148,   276,   112,
     271,   291,   298,    58,   262,   278,   349,   120,   110,   122,
     346,   212,    47,   344,   320,   114,   112,   228,   133,   113,
     141,   115,   155,   163,    73,   368,   384,   212,   112,   117,
     112,   430,   213,   361,   112,   354,   113,   265,   243,   266,
     223,    58,    74,   436,   437,    73,    90,    58,   113,   243,
     113,   -80,   118,   245,   208,   100,   103,   105,   111,   211,
     229,   353,   119,   121,   224,    47,   225,    73,   245,   384,
     234,   219,   369,   127,   125,   130,   293,   235,   393,  -130,
     122,   396,   128,   347,   188,   189,    47,   202,   274,   179,
     180,   122,     3,   249,     4,  -130,     5,   129,    47,   246,
     131,   260,   186,   187,   179,   180,   203,   416,    20,     7,
       8,   148,   164,   261,   246,   133,   303,   257,   158,   258,
     146,   406,   407,   249,   206,   141,   247,   155,   112,   428,
     429,   224,   207,   209,   141,     9,   155,   112,    73,   218,
     431,   247,   257,   255,   326,   283,   251,   215,   113,   392,
     230,    41,   337,   232,   342,   231,   233,   113,   241,    28,
     220,   221,   222,   348,   244,   252,   263,   264,   253,   267,
     268,   254,    42,   273,    33,   259,    41,    45,    46,    47,
      48,    49,     5,     9,   272,   275,   112,   284,   148,   212,
      39,   277,   292,   133,    40,     6,   286,   163,    43,    44,
     182,   183,   184,   185,    47,   279,   113,   281,   220,   285,
     222,   141,   381,   155,   287,    73,   289,   288,   318,   122,
     122,   122,   122,   122,   294,   302,   122,   122,   122,   122,
     122,   112,   112,   295,   122,   304,   319,   112,    39,   167,
     168,   169,   323,   321,   402,   290,   327,   324,   329,   330,
     331,   113,   113,   332,   325,   333,   334,   113,   296,   335,
     336,   338,   219,   339,   415,   341,   343,   351,   299,   112,
     105,   133,   419,   155,   352,   356,   345,   357,   163,   358,
     305,   350,   362,   389,   112,   355,   360,   377,   390,   113,
     316,   317,   378,   403,   225,   391,   398,   105,   399,   401,
     404,   405,   409,   219,   113,   410,   413,   412,   371,   372,
     414,   418,   375,   376,   420,   417,   423,   112,   249,    58,
      58,   421,   424,    58,    58,   433,     9,   210,   340,   382,
     411,    58,    74,   387,   388,    73,    90,   367,   432,   142,
     151,   156,   161,   395,   163,   359,   306,   382,   195,   394,
     122,   143,   135,   282,   283,   400,   397,   225,   408,   149,
     153,   159,   307,   309,   197,   308,   311,   198,   310,    58,
     199,   200,   201,   364,    58,    74,   105,   312,    73,    90,
     313,    39,   422,   373,   374,    41,   314,   124,   427,   380,
     315,     0,   139,    58,   154,   160,     0,     0,     0,    58,
       0,     0,     0,     0,   434,   435,     0,     0,   296,     0,
     112,     0,     0,     0,     0,    58,    58,   142,   151,   156,
       0,     0,     0,     0,     9,     0,     0,     0,     0,     0,
     367,     0,     0,     0,     0,     0,     0,   149,   153,     0,
      20,     0,     0,     0,   142,   151,   156,   161,    28,   143,
     144,   145,   146,   142,   151,   156,   161,     0,     0,   139,
     154,   160,   151,     0,     0,     0,     0,   142,   151,     0,
     139,     0,   154,   149,   153,   159,     0,     0,     0,    39,
       9,   149,     2,    41,     3,     0,     4,   149,     5,     0,
       0,     0,     0,     0,     0,     0,    20,   139,     0,   154,
     160,     7,     8,     0,    28,   143,   144,   145,   146,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     139,     0,     0,     0,    24,    25,    26,    27,     0,   147,
       9,     0,     0,     0,     2,    39,     3,     0,     4,    41,
       5,     0,    34,    35,     0,     0,    20,     0,     0,     0,
       0,     0,   242,     7,     8,   143,   152,   145,   146,     0,
       0,    10,     0,     0,    42,   151,   156,   142,   156,    45,
      46,    47,    48,    49,     0,     0,    24,    25,    26,    27,
     300,     0,     0,     0,     0,    39,     0,   153,     0,    41,
       0,     0,   156,   161,    34,    35,     0,     0,     0,     0,
       0,     0,     1,     0,     0,     0,     0,     2,     0,     3,
       0,     4,     0,     5,     0,     6,    42,     0,     0,   154,
       0,    45,    46,    47,    48,    49,     7,     8,     0,     9,
       0,     0,     0,     0,    10,    11,   156,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,     0,     0,
      20,     0,     0,     0,     0,     0,    33,    34,    35,   257,
     158,   258,   146,     0,     0,     0,    36,    37,    38,     0,
       0,     0,     0,     0,    39,     0,    40,     0,    41,    42,
      43,    44,     0,   147,    45,    46,    47,    48,    49,     1,
       0,     0,     0,    41,     2,     0,     3,     0,     4,     0,
       5,     0,     6,   106,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,    11,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,   107,   108,   109,    32,     0,     0,     0,     0,     0,
       0,     0,     0,    33,    34,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    40,     0,    41,    42,    43,    44,     0,
       0,    45,    46,    47,    48,    49,     1,     0,     0,     0,
       0,     2,     0,     3,     0,     4,     0,     5,     0,     6,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,    10,    11,
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,   107,   108,
     109,    32,     0,     0,     0,     0,     0,     0,     0,     0,
      33,    34,    35,     0,     0,     0,     0,     0,     0,     0,
       1,     0,    38,     0,     0,     2,     0,     3,     0,     4,
      40,     5,    41,    42,    43,    44,     0,     0,    45,    46,
      47,    48,    49,     0,     7,     8,     0,     0,     0,     0,
       0,     0,    10,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24,    25,    26,
      27,   126,    96,     0,    97,    98,     2,     0,     3,     0,
       4,     0,     5,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    40,     0,     0,    42,    43,    44,
     269,   270,    45,    46,    47,    48,    49,     0,    24,    25,
      26,    27,     1,    96,     0,    97,    98,     2,     0,     3,
       0,     4,     0,     5,     0,    33,    34,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,    10,    40,     0,     0,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,     0,    24,
      25,    26,    27,     0,    96,     0,    97,    98,     2,     0,
       3,     0,     4,     0,     5,     0,    33,    34,    35,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       9,     0,     0,     0,     0,    10,    40,     0,     0,    42,
      43,    44,     0,     0,    45,    46,    47,    48,    49,     0,
      24,    25,    26,    27,     0,   134,   135,   136,   137,     0,
       0,     0,     0,     0,     0,     0,     0,    33,    34,    35,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   138,
       0,     0,     2,     0,     3,    39,     4,    40,     5,    41,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
       0,     7,     8,     0,     9,     0,     0,     0,     0,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      20,     0,     0,     0,    24,    25,    26,    27,     0,   134,
     152,    31,    32,     2,     0,     3,     0,     4,     0,     5,
       0,    33,    34,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     0,     9,     0,     0,     0,    39,
      10,    40,     0,    41,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,     0,    24,    25,    26,    27,     0,
     134,   135,   136,   137,     0,     0,     0,     0,     0,     0,
       0,     0,    33,    34,    35,     0,     0,     0,     0,     2,
       0,     3,     0,     4,     0,     5,     0,     0,     0,     0,
      39,     0,    40,     0,    41,    42,    43,    44,     7,     8,
      45,    46,    47,    48,    49,     0,    10,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    20,     0,     0,
       0,    24,    25,    26,    27,     0,   157,   158,   109,    32,
       2,     0,     3,     0,     4,     0,     5,     0,    33,    34,
      35,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,     0,     0,    10,    40,     0,
      41,    42,    43,    44,     0,     0,    45,    46,    47,    48,
      49,     0,    24,    25,    26,    27,     0,   157,   255,   256,
     137,     0,     0,     0,     0,     0,     0,     0,     0,    33,
      34,    35,     0,     0,     0,     0,     2,     0,     3,     0,
       4,   138,     5,     0,     0,     0,     0,     0,     0,    40,
       0,    41,    42,    43,    44,     7,     8,    45,    46,    47,
      48,    49,     0,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,    25,
      26,    27,     0,   157,   255,   256,   137,     2,     0,     3,
       0,     4,   102,     5,     0,    33,    34,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,    10,    40,     0,    41,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,     0,    24,
      25,    26,    27,     0,    96,     0,    97,    98,     2,     0,
       3,     0,     4,     0,     5,   104,    33,    34,    35,     0,
       0,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    40,     0,     0,    42,
      43,    44,     0,     0,    45,    46,    47,    48,    49,     0,
      24,    25,    26,    27,     0,    96,     0,    97,    98,     2,
       0,     3,     0,     4,     0,     5,   301,    33,    34,    35,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
       0,     0,     0,     0,     0,     0,    10,    40,     0,     0,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
       0,    24,    25,    26,    27,     0,    96,     0,    97,    98,
       2,     0,     3,     0,     4,   363,     5,     0,    33,    34,
      35,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,     0,     0,    10,    40,     0,
       0,    42,    43,    44,     0,     0,    45,    46,    47,    48,
      49,     0,    24,    25,    26,    27,     0,    96,     0,    97,
      98,     2,     0,     3,     0,     4,     0,     5,   365,    33,
      34,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,    10,    40,
       0,     0,    42,    43,    44,     0,     0,    45,    46,    47,
      48,    49,     0,    24,    25,    26,    27,     0,    96,     0,
      97,    98,     2,     0,     3,     0,     4,   379,     5,     0,
      33,    34,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      40,     0,     0,    42,    43,    44,     0,     0,    45,    46,
      47,    48,    49,     0,    24,    25,    26,    27,     0,    96,
       0,    97,    98,     2,     0,     3,     0,     4,     0,     5,
       0,    33,    34,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,     0,     0,
      10,    40,     0,     0,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,     0,    24,    25,    26,    27,     0,
      96,     0,    97,    98,   165,     0,   166,   167,   168,   169,
       0,     0,    33,    34,    35,     0,     0,     0,   170,   171,
     172,   173,   174,   175,   176,   177,   178,     0,     0,     0,
       0,     0,    40,     0,     0,    42,    43,    44,     0,     0,
      45,    46,    47,    48,    49,     0,     0,     0,     0,     0,
     179,   180,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   181,   182,   183,   184,   185,   186,   187,
       0,   188,   189,   165,   216,   166,     0,     0,     0,     0,
     190,   191,   192,   193,   194,     0,     0,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   165,     0,   166,     0,
       0,     0,     0,     0,     0,   425,     0,     0,     0,     0,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   165,
       0,   166,     0,     0,     0,     0,     0,     0,   426,     0,
       0,     0,   181,   170,   171,   172,   173,   174,   175,   176,
     177,   178,     0,     0,     0,     0,     0,     0,     0,   190,
     191,   192,   193,   194,     0,   181,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   190,   191,   192,   193,   194,     0,   181,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   190,   191,   192,   193,   194
};

static const yytype_int16 yycheck[] =
{
       0,     0,    13,    19,     0,     0,     6,    30,   125,     9,
     119,    12,   207,    13,   113,   131,   290,    17,     6,    19,
     287,    29,    97,    12,   247,    12,    26,    33,    28,     9,
      29,    12,    31,    33,    30,    13,   341,    29,    38,    12,
      40,   416,    34,   317,    44,    67,    26,    38,    90,    40,
      73,    51,    51,   428,   429,    51,    51,    57,    38,   101,
      40,    12,    12,    14,    44,     3,     4,     5,     9,    57,
      76,   294,    12,    12,    73,    97,    76,    73,    14,   384,
      63,    71,    60,    21,    16,    26,   203,    70,   355,    97,
      90,   358,     3,   288,    71,    72,    97,    38,    34,    50,
      51,   101,    10,    93,    12,    97,    14,     3,    97,    60,
      32,   111,    68,    69,    50,    51,    87,   391,    46,    27,
      28,   144,    12,   113,    60,   125,   225,    55,    56,    57,
      58,   377,   378,   123,    16,   134,    87,   136,   138,   413,
     414,   140,    12,     0,   143,    30,   145,   147,   144,    14,
     417,    87,    55,    56,    57,    58,    94,     3,   138,   354,
      62,    89,   271,    75,   281,    74,    73,   147,    31,    54,
      55,    56,    57,   289,    61,    11,   114,   115,    13,   117,
     118,    15,    90,   121,    67,    17,    89,    95,    96,    97,
      98,    99,    14,    30,     3,    34,   196,   138,   221,    29,
      85,     3,   202,   203,    87,    16,   147,   207,    91,    92,
      64,    65,    66,    67,    97,    59,   196,     5,    55,    56,
      57,   220,   338,   222,    65,   221,    13,    60,   244,   229,
     230,   231,   232,   233,     5,   225,   236,   237,   238,   239,
     240,   241,   242,    88,   244,     5,   246,   247,    85,     7,
       8,     9,    13,    12,   370,   196,    12,    14,    13,    13,
      12,   241,   242,    12,   254,    13,    13,   247,   206,   269,
     270,    13,   262,    17,   390,    16,    12,     3,   216,   279,
     218,   281,   398,   282,    88,     3,   286,    17,   288,    13,
     228,   291,    88,     3,   294,   295,    34,    34,    13,   279,
     241,   242,    34,    37,   304,    13,    13,   245,    13,    60,
      13,    13,    13,   303,   294,     3,    13,    17,   329,   330,
      13,     3,   333,   334,     5,    65,     3,   327,   318,   329,
     330,    60,     3,   333,   334,    13,    30,    51,   279,   339,
     384,   341,   341,   343,   344,   341,   341,   327,   420,    29,
      30,    31,    32,   357,   354,   304,   229,   357,    37,   356,
     360,    55,    56,    57,    58,   367,   360,   367,   379,    30,
      31,    32,   230,   232,    37,   231,   236,    37,   233,   379,
      37,    37,    37,   321,   384,   384,   324,   237,   384,   384,
     238,    85,   403,   331,   332,    89,   239,    19,   409,   337,
     240,    -1,    29,   403,    31,    32,    -1,    -1,    -1,   409,
      -1,    -1,    -1,    -1,   425,   426,    -1,    -1,   356,    -1,
     420,    -1,    -1,    -1,    -1,   425,   426,   107,   108,   109,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
     420,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,
      46,    -1,    -1,    -1,   134,   135,   136,   137,    54,    55,
      56,    57,    58,   143,   144,   145,   146,    -1,    -1,    96,
      97,    98,   152,    -1,    -1,    -1,    -1,   157,   158,    -1,
     107,    -1,   109,   144,   145,   146,    -1,    -1,    -1,    85,
      30,   152,     8,    89,    10,    -1,    12,   158,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,   134,    -1,   136,
     137,    27,    28,    -1,    54,    55,    56,    57,    58,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    79,
      30,    -1,    -1,    -1,     8,    85,    10,    -1,    12,    89,
      14,    -1,    68,    69,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    78,    27,    28,    55,    56,    57,    58,    -1,
      -1,    35,    -1,    -1,    90,   255,   256,   257,   258,    95,
      96,    97,    98,    99,    -1,    -1,    50,    51,    52,    53,
     217,    -1,    -1,    -1,    -1,    85,    -1,   258,    -1,    89,
      -1,    -1,   282,   283,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    16,    90,    -1,    -1,   256,
      -1,    95,    96,    97,    98,    99,    27,    28,    -1,    30,
      -1,    -1,    -1,    -1,    35,    36,   326,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    55,
      56,    57,    58,    -1,    -1,    -1,    77,    78,    79,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    87,    -1,    89,    90,
      91,    92,    -1,    79,    95,    96,    97,    98,    99,     3,
      -1,    -1,    -1,    89,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    56,    57,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    96,    97,    98,    99,     3,    -1,    -1,    -1,
      -1,     8,    -1,    10,    -1,    12,    -1,    14,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    55,    56,
      57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    79,    -1,    -1,     8,    -1,    10,    -1,    12,
      87,    14,    89,    90,    91,    92,    -1,    -1,    95,    96,
      97,    98,    99,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    52,
      53,     3,    55,    -1,    57,    58,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    67,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    87,    -1,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,    -1,    50,    51,
      52,    53,     3,    55,    -1,    57,    58,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    67,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    87,    -1,    -1,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    57,    58,     8,    -1,
      10,    -1,    12,    -1,    14,    -1,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      30,    -1,    -1,    -1,    -1,    35,    87,    -1,    -1,    90,
      91,    92,    -1,    -1,    95,    96,    97,    98,    99,    -1,
      50,    51,    52,    53,    -1,    55,    56,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,     8,    -1,    10,    85,    12,    87,    14,    89,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
      -1,    27,    28,    -1,    30,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      56,    57,    58,     8,    -1,    10,    -1,    12,    -1,    14,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    30,    -1,    -1,    -1,    85,
      35,    87,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    -1,    50,    51,    52,    53,    -1,
      55,    56,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,
      85,    -1,    87,    -1,    89,    90,    91,    92,    27,    28,
      95,    96,    97,    98,    99,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    52,    53,    -1,    55,    56,    57,    58,
       8,    -1,    10,    -1,    12,    -1,    14,    -1,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    87,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    96,    97,    98,
      99,    -1,    50,    51,    52,    53,    -1,    55,    56,    57,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    79,    14,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    89,    90,    91,    92,    27,    28,    95,    96,    97,
      98,    99,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    56,    57,    58,     8,    -1,    10,
      -1,    12,    13,    14,    -1,    67,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    87,    -1,    89,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    57,    58,     8,    -1,
      10,    -1,    12,    -1,    14,    15,    67,    68,    69,    -1,
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
      58,     8,    -1,    10,    -1,    12,    -1,    14,    15,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    87,
      -1,    -1,    90,    91,    92,    -1,    -1,    95,    96,    97,
      98,    99,    -1,    50,    51,    52,    53,    -1,    55,    -1,
      57,    58,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      87,    -1,    -1,    90,    91,    92,    -1,    -1,    95,    96,
      97,    98,    99,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    57,    58,     8,    -1,    10,    -1,    12,    -1,    14,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    87,    -1,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    -1,    50,    51,    52,    53,    -1,
      55,    -1,    57,    58,     4,    -1,     6,     7,     8,     9,
      -1,    -1,    67,    68,    69,    -1,    -1,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    90,    91,    92,    -1,    -1,
      95,    96,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,     4,     5,     6,    -1,    -1,    -1,    -1,
      80,    81,    82,    83,    84,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     4,    -1,     6,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     4,
      -1,     6,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,
      -1,    -1,    63,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      81,    82,    83,    84,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    81,    82,    83,    84,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,    84
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
     101,   102,   103,   109,   110,   111,   114,   115,   116,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     131,   132,   133,   136,   137,   139,   141,   143,   146,   147,
     148,   149,   150,   151,   153,   155,   157,   159,   161,   162,
     163,   164,   165,   169,   170,   171,    55,    57,    58,   116,
     127,   163,    13,   127,    15,   127,    17,    55,    56,    57,
     109,   112,   116,   141,    12,    12,   115,    12,    12,    12,
     116,    12,   116,   169,   171,    16,     3,   127,     3,     3,
     112,    32,   107,   116,    55,    56,    57,    58,    79,   133,
     136,   137,   143,    55,    56,    57,    58,    79,   103,   118,
     137,   143,    56,   118,   133,   137,   143,    55,    56,   118,
     133,   143,   108,   116,    12,     4,     6,     7,     8,     9,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    50,
      51,    63,    64,    65,    66,    67,    68,    69,    71,    72,
      80,    81,    82,    83,    84,   130,   138,   154,   156,   158,
     160,   168,   112,    87,   112,   166,    16,    12,   141,     0,
     101,   109,    29,    34,   140,     3,     5,   130,    14,   131,
      55,    56,    57,   103,   137,   116,   144,   145,    33,    76,
      62,    74,    75,    73,    63,    70,   152,   154,   156,   158,
     160,    31,    78,   164,    61,    14,    60,    87,   129,   131,
     168,   127,    11,    13,    15,    56,    57,    55,    57,    17,
     116,   131,   132,   127,   127,    38,    40,   127,   127,    93,
      94,   126,     3,   127,    34,    34,   107,     3,   114,    59,
     104,     5,    57,    58,   112,    56,   112,    65,    60,    13,
     112,    12,   116,   107,     5,    88,   127,   142,   108,   127,
     133,    15,   131,   132,     5,   127,   148,   149,   150,   151,
     153,   155,   157,   159,   161,   162,   112,   112,   169,   116,
     166,    12,   167,    13,    14,   131,    57,    12,   134,    13,
      13,    12,    12,    13,    13,   116,   116,   126,    13,    17,
     112,    16,   107,    12,    12,   116,   140,   108,   114,   134,
     116,     3,    88,   166,    67,   116,     3,    17,    13,   144,
      34,   134,    88,    13,   127,    15,   113,   141,    13,    60,
     135,   115,   115,   127,   127,   115,   115,    34,    34,    13,
     127,   114,   116,   117,   102,   105,   106,   116,   116,     3,
      13,    13,   108,   140,   142,   117,   140,   146,    13,    13,
     145,    60,   114,    37,    13,    13,   128,   128,   115,    13,
       3,   106,    17,    13,    13,   114,   134,    65,     3,   114,
       5,    60,   115,     3,     3,    13,    13,   115,   134,   134,
     135,   140,   113,    13,   115,   115,   135,   135
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   100,   101,   101,   102,   102,   102,   103,   103,   103,
     104,   104,   105,   105,   106,   106,   107,   107,   108,   108,
     109,   109,   110,   110,   110,   111,   111,   112,   112,   112,
     113,   113,   114,   114,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   116,   117,   117,
     118,   118,   118,   118,   119,   120,   121,   121,   122,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   124,   124,
     125,   125,   125,   125,   126,   126,   127,   127,   128,   128,
     129,   129,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   131,
     131,   131,   132,   132,   132,   133,   133,   133,   133,   133,
     134,   134,   135,   135,   136,   137,   137,   137,   137,   137,
     138,   138,   138,   138,   138,   138,   139,   139,   139,   139,
     140,   140,   141,   141,   141,   141,   141,   141,   142,   142,
     143,   143,   143,   143,   143,   144,   144,   145,   145,   145,
     146,   146,   147,   147,   148,   148,   149,   149,   150,   150,
     151,   151,   152,   152,   153,   153,   154,   154,   154,   154,
     155,   155,   156,   156,   157,   157,   158,   158,   159,   159,
     160,   160,   160,   161,   161,   162,   162,   163,   163,   163,
     163,   163,   163,   163,   164,   164,   164,   164,   165,   165,
     166,   166,   167,   167,   168,   168,   169,   169,   169,   169,
     169,   170,   170,   170,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     6,     2,     2,
       2,     0,     1,     0,     1,     2,     1,     3,     1,     3,
       1,     2,     7,     8,     8,     4,     5,     1,     2,     2,
       2,     4,     2,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       6,     2,     2,     2,     2,     3,     3,     3,     5,     5,
       7,     6,     7,     8,     8,     5,     7,     5,     5,     7,
       2,     3,     2,     2,     2,     1,     1,     3,     1,     3,
       0,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       4,     5,     2,     3,     2,     1,     2,     2,     2,     2,
       1,     2,     1,     4,     4,     6,     2,     2,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     6,     6,     4,
       0,     1,     2,     4,     5,     7,     5,     2,     2,     3,
       6,     2,     2,     2,     2,     1,     3,     1,     2,     2,
       1,     5,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     1,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     2,     1,     2,     2,     3,     1,     3,
       1,     3,     2,     3,     1,     1,     1,     2,     3,     3,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     2
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
          case 110: /* func_ptr  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1829 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 111: /* stmt_type  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1835 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 114: /* code_segment  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1841 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 115: /* stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1847 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 118: /* enum_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1853 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 119: /* label_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1859 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 120: /* goto_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1865 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 121: /* case_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1871 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 122: /* switch_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1877 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 123: /* loop_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1883 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 124: /* selection_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1889 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 125: /* jump_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1895 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 126: /* exp_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1901 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 127: /* exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1907 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 128: /* binary_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1913 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 133: /* decl_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1919 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 143: /* union_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1925 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 146: /* con_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1931 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 147: /* log_or_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1937 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 148: /* log_and_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1943 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 149: /* inc_or_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1949 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 150: /* exc_or_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1955 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 151: /* and_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1961 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 153: /* eq_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1967 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 155: /* relational_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1973 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 157: /* shift_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1979 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 159: /* add_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1985 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 161: /* mul_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1991 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 162: /* cast_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1997 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 164: /* unary_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 2003 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 165: /* dur_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 2009 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 167: /* call_paren  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 2015 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 169: /* post_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 2021 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 171: /* primary_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 2027 "src/ast/parser.c" /* yacc.c:1258  */
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
#line 2295 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 3:
#line 103 "utils/gwion.y" /* yacc.c:1648  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast)); }
#line 2301 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 4:
#line 107 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list)); }
#line 2307 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 5:
#line 108 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def)); }
#line 2313 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 6:
#line 109 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def)); }
#line 2319 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 7:
#line 114 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_def) = new_class_def(0, (yyvsp[-4].id_list), (yyvsp[-3].type_decl), (yyvsp[-1].class_body)); }
#line 2325 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 8:
#line 115 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_global); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2331 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 9:
#line 117 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].class_def), free_class_def); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2337 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 10:
#line 119 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2343 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 11:
#line 119 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = NULL; }
#line 2349 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 13:
#line 121 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = NULL; }
#line 2355 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 14:
#line 124 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = new_class_body((yyvsp[0].section), NULL); }
#line 2361 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 15:
#line 125 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = new_class_body((yyvsp[-1].section), (yyvsp[0].class_body)); }
#line 2367 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 16:
#line 129 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2373 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 17:
#line 130 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2379 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 18:
#line 134 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2385 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 19:
#line 135 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2391 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 20:
#line 139 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL);}
#line 2397 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 21:
#line 140 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list));}
#line 2403 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 22:
#line 150 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_fptr((yyvsp[-3].sym), (yyvsp[-5].type_decl), (yyvsp[-1].arg_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2409 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 23:
#line 152 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_fptr((yyvsp[-3].sym), (yyvsp[-5].type_decl), (yyvsp[-1].arg_list), (yyvsp[0].ival) | ae_flag_static, get_pos(arg)); }
#line 2415 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 24:
#line 154 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_fptr((yyvsp[-3].sym), (yyvsp[-5].type_decl), (yyvsp[-1].arg_list), (yyvsp[0].ival) | ae_flag_global, get_pos(arg)); }
#line 2421 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 25:
#line 159 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_type((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2427 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 26:
#line 161 "utils/gwion.y" /* yacc.c:1648  */
    { SET_FLAG((yyvsp[-2].type_decl), ae_flag_global); (yyval.stmt) = new_stmt_type((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2433 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 28:
#line 165 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub)); }
#line 2439 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 29:
#line 166 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub)); }
#line 2445 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 30:
#line 170 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL); }
#line 2451 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 31:
#line 171 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list)); }
#line 2457 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 32:
#line 175 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_code(NULL, get_pos(arg)); }
#line 2463 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 33:
#line 176 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_code((yyvsp[-1].stmt_list), get_pos(arg)); }
#line 2469 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 47:
#line 195 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.sym) = insert_symbol((yyvsp[0].sval)); }
#line 2475 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 48:
#line 197 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.sym) = NULL; }
#line 2481 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 50:
#line 200 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sym), get_pos(arg)); }
#line 2487 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 51:
#line 201 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2493 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 52:
#line 202 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2499 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 53:
#line 203 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_global);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 2505 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 54:
#line 207 "utils/gwion.y" /* yacc.c:1648  */
    {  (yyval.stmt) = new_stmt_jump((yyvsp[-1].sym), 1, get_pos(arg)); }
#line 2511 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 55:
#line 211 "utils/gwion.y" /* yacc.c:1648  */
    {  (yyval.stmt) = new_stmt_jump((yyvsp[-1].sym), 0, get_pos(arg)); }
#line 2517 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 56:
#line 215 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_case, (yyvsp[-1].exp), get_pos(arg)); }
#line 2523 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 57:
#line 216 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_case, (yyvsp[-1].exp), get_pos(arg)); }
#line 2529 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 58:
#line 220 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg));}
#line 2535 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 59:
#line 225 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_while, (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2541 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 60:
#line 227 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_while, (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2547 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 61:
#line 229 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2553 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 62:
#line 231 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2559 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 63:
#line 233 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2565 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 64:
#line 235 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); (yyval.stmt)->d.stmt_auto.is_ptr = 1;}
#line 2571 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 65:
#line 237 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_until, (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2577 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 66:
#line 239 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_until, (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2583 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 67:
#line 241 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_loop((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2589 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 68:
#line 246 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(arg)); }
#line 2595 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 69:
#line 248 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(arg)); }
#line 2601 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 70:
#line 252 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_return, NULL, get_pos(arg)); }
#line 2607 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 71:
#line 253 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_return, (yyvsp[-1].exp), get_pos(arg)); }
#line 2613 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 72:
#line 254 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt(ae_stmt_break, get_pos(arg)); }
#line 2619 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 73:
#line 255 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt(ae_stmt_continue, get_pos(arg)); }
#line 2625 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 74:
#line 259 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_exp, (yyvsp[-1].exp),   get_pos(arg)); }
#line 2631 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 75:
#line 260 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_exp, NULL, get_pos(arg)); }
#line 2637 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 77:
#line 265 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = prepend_exp((yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 2643 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 79:
#line 270 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2649 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 80:
#line 273 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = NULL; }
#line 2655 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 81:
#line 273 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2661 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 82:
#line 275 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_chuck; }
#line 2667 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 83:
#line 275 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_unchuck; }
#line 2673 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 84:
#line 275 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_eq; }
#line 2679 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 85:
#line 276 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_at_chuck; }
#line 2685 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 86:
#line 276 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plus_chuck; }
#line 2691 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 87:
#line 277 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minus_chuck; }
#line 2697 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 88:
#line 277 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_times_chuck; }
#line 2703 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 89:
#line 278 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_divide_chuck; }
#line 2709 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 90:
#line 278 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2715 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 91:
#line 279 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_trig; }
#line 2721 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 92:
#line 279 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_untrig; }
#line 2727 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 93:
#line 280 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsl; }
#line 2733 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 94:
#line 280 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsr; }
#line 2739 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 95:
#line 280 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsand; }
#line 2745 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 96:
#line 281 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsor; }
#line 2751 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 97:
#line 281 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsxor; }
#line 2757 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 98:
#line 282 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_assign; }
#line 2763 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 99:
#line 286 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = new_array_sub((yyvsp[-1].exp), get_pos(arg)); }
#line 2769 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 100:
#line 287 "utils/gwion.y" /* yacc.c:1648  */
    { if((yyvsp[-2].exp)->next){ gwion_error(arg, "invalid format for array init [...][...]..."); free_exp((yyvsp[-2].exp)); free_array_sub((yyvsp[0].array_sub)); YYERROR; } (yyval.array_sub) = prepend_array_sub((yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2775 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 101:
#line 288 "utils/gwion.y" /* yacc.c:1648  */
    { gwion_error(arg, "partially empty array init [...][]..."); free_exp((yyvsp[-3].exp)); YYERROR; }
#line 2781 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 102:
#line 292 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(arg)); }
#line 2787 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 103:
#line 293 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2793 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 104:
#line 294 "utils/gwion.y" /* yacc.c:1648  */
    { gwion_error(arg, "partially empty array init [][...]"); free_array_sub((yyvsp[-1].array_sub)); free_array_sub((yyvsp[0].array_sub)); YYERROR; }
#line 2799 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 106:
#line 299 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2805 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 107:
#line 301 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_static); (yyval.exp) = (yyvsp[0].exp); }
#line 2811 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 108:
#line 303 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_private); (yyval.exp) = (yyvsp[0].exp); }
#line 2817 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 109:
#line 305 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_protect); (yyval.exp) = (yyvsp[0].exp); }
#line 2823 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 110:
#line 309 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = NULL; }
#line 2829 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 111:
#line 310 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = (yyvsp[0].arg_list); }
#line 2835 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 112:
#line 314 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 2841 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 113:
#line 315 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2847 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 114:
#line 318 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2853 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 115:
#line 322 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-4].type_decl), (yyvsp[-3].sym), (yyvsp[-2].arg_list), (yyvsp[0].stmt), (yyvsp[-1].ival)); }
#line 2859 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 116:
#line 324 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_private); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2865 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 117:
#line 326 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_static); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2871 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 118:
#line 328 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_global); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2877 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 119:
#line 330 "utils/gwion.y" /* yacc.c:1648  */
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
#line 2893 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 127:
#line 346 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-3].type_decl), OP_SYM((yyvsp[-4].ival)), (yyvsp[-2].arg_list), (yyvsp[0].stmt), ae_flag_op); }
#line 2899 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 128:
#line 348 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-3].type_decl), OP_SYM((yyvsp[-5].ival)), (yyvsp[-2].arg_list), (yyvsp[0].stmt), ae_flag_op | ae_flag_unary); }
#line 2905 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 129:
#line 350 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def(new_type_decl(new_id_list(insert_symbol("void"), get_pos(arg)), 0,
      get_pos(arg)), insert_symbol("dtor"), NULL, (yyvsp[0].stmt), ae_flag_dtor); }
#line 2912 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 130:
#line 354 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 2918 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 131:
#line 354 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_flag_ref; }
#line 2924 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 132:
#line 357 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)), (yyvsp[0].ival), get_pos(arg)); }
#line 2930 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 133:
#line 358 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2936 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 134:
#line 359 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)),
      (yyvsp[0].ival), get_pos(arg)); (yyval.type_decl)->types = (yyvsp[-3].type_list); }
#line 2943 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 135:
#line 361 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg));
      (yyval.type_decl)->types = (yyvsp[-5].type_list); }
#line 2950 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 136:
#line 363 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2956 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 137:
#line 364 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].type_decl), ae_flag_const); (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2962 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 138:
#line 368 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2968 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 139:
#line 369 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2974 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 140:
#line 373 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(arg));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sym); }
#line 2980 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 141:
#line 375 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2986 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 142:
#line 377 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2992 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 143:
#line 379 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2998 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 144:
#line 381 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_global);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 3004 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 145:
#line 385 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL); }
#line 3010 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 146:
#line 386 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list)); }
#line 3016 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 147:
#line 390 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sym), NULL, get_pos(arg)); }
#line 3022 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 148:
#line 391 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 3028 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 149:
#line 392 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 3034 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 151:
#line 396 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_if((yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3040 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 153:
#line 399 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(arg)); }
#line 3046 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 155:
#line 402 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(arg)); }
#line 3052 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 157:
#line 405 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(arg)); }
#line 3058 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 159:
#line 408 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(arg)); }
#line 3064 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 161:
#line 411 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(arg)); }
#line 3070 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 162:
#line 413 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_eq; }
#line 3076 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 163:
#line 413 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_neq; }
#line 3082 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 165:
#line 416 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3088 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 166:
#line 418 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_lt; }
#line 3094 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 167:
#line 418 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_gt; }
#line 3100 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 168:
#line 419 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_le; }
#line 3106 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 169:
#line 419 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_ge; }
#line 3112 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 171:
#line 422 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3118 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 172:
#line 425 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_shift_left;  }
#line 3124 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 173:
#line 426 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_shift_right; }
#line 3130 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 175:
#line 430 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3136 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 176:
#line 432 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plus; }
#line 3142 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 177:
#line 432 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minus; }
#line 3148 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 179:
#line 435 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3154 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 180:
#line 437 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_times; }
#line 3160 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 181:
#line 437 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_divide; }
#line 3166 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 182:
#line 437 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_percent; }
#line 3172 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 184:
#line 440 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3178 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 186:
#line 443 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_cast((yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(arg)); }
#line 3184 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 187:
#line 445 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plus; }
#line 3190 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 188:
#line 445 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minus; }
#line 3196 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 189:
#line 445 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_times; }
#line 3202 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 190:
#line 446 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plusplus; }
#line 3208 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 191:
#line 446 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minusminus; }
#line 3214 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 192:
#line 447 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_exclamation; }
#line 3220 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 193:
#line 447 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_spork; }
#line 3226 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 195:
#line 451 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_unary((yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3232 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 196:
#line 453 "utils/gwion.y" /* yacc.c:1648  */
    {
      if((yyvsp[0].type_decl)->array && !(yyvsp[0].type_decl)->array->exp) {
        free_type_decl((yyvsp[0].type_decl));
        gwion_error(arg, "can't use empty '[]' in 'new' expression");
        YYERROR;
      }
      (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), get_pos(arg));
    }
#line 3245 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 197:
#line 462 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_unary3(op_spork, (yyvsp[0].stmt), get_pos(arg)); }
#line 3251 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 199:
#line 465 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_dur((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3257 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 200:
#line 468 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = new_type_list((yyvsp[0].type_decl), NULL); }
#line 3263 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 201:
#line 469 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = new_type_list((yyvsp[-2].type_decl), (yyvsp[0].type_list)); }
#line 3269 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 202:
#line 472 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = NULL; }
#line 3275 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 203:
#line 472 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3281 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 204:
#line 474 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plusplus; }
#line 3287 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 205:
#line 474 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minusminus; }
#line 3293 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 207:
#line 477 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_array((yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(arg)); }
#line 3299 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 208:
#line 479 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_call((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg));
      if((yyvsp[-1].type_list))(yyval.exp)->d.exp_func.tmpl = new_tmpl_call((yyvsp[-1].type_list)); }
#line 3306 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 209:
#line 482 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_dot((yyvsp[-2].exp), (yyvsp[0].sym), get_pos(arg)); }
#line 3312 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 210:
#line 484 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_post((yyvsp[-1].exp), (yyvsp[0].ival), get_pos(arg)); }
#line 3318 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 211:
#line 487 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_complex; }
#line 3324 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 212:
#line 488 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_polar;   }
#line 3330 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 213:
#line 489 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_vec;     }
#line 3336 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 214:
#line 492 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_id(     (yyvsp[0].sym), get_pos(arg)); }
#line 3342 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 215:
#line 493 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_int(    (yyvsp[0].ival), get_pos(arg)); }
#line 3348 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 216:
#line 494 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_float(  (yyvsp[0].fval), get_pos(arg)); }
#line 3354 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 217:
#line 495 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_string( (yyvsp[0].sval), get_pos(arg)); }
#line 3360 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 218:
#line 496 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_char(   (yyvsp[0].sval), get_pos(arg)); }
#line 3366 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 219:
#line 497 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3372 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 220:
#line 498 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3378 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 221:
#line 499 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_vec((yyvsp[-2].ival), (yyvsp[-1].exp), get_pos(arg)); }
#line 3384 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 222:
#line 500 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_hack(   (yyvsp[-1].exp), get_pos(arg)); }
#line 3390 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 223:
#line 501 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) =                      (yyvsp[-1].exp);                }
#line 3396 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 224:
#line 502 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_nil(        get_pos(arg)); }
#line 3402 "src/ast/parser.c" /* yacc.c:1648  */
    break;


#line 3406 "src/ast/parser.c" /* yacc.c:1648  */
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
#line 504 "utils/gwion.y" /* yacc.c:1907  */

