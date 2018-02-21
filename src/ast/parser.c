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

#line 357 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (Scanner* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 373 "src/ast/parser.c" /* yacc.c:358  */

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
#define YYFINAL  194
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1763

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  100
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  73
/* YYNRULES -- Number of rules.  */
#define YYNRULES  217
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  397

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
       0,   107,   107,   108,   112,   113,   114,   118,   120,   121,
     124,   124,   126,   126,   129,   130,   134,   135,   139,   140,
     144,   145,   149,   150,   154,   155,   159,   160,   166,   170,
     171,   172,   176,   177,   181,   182,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   201,
     203,   203,   206,   207,   211,   215,   219,   220,   224,   228,
     230,   232,   234,   236,   238,   240,   242,   244,   249,   251,
     256,   257,   258,   259,   263,   264,   268,   269,   273,   274,
     277,   277,   279,   279,   279,   280,   280,   281,   281,   282,
     282,   283,   283,   284,   284,   284,   285,   285,   286,   290,
     291,   292,   296,   297,   298,   302,   303,   304,   306,   311,
     312,   315,   318,   320,   322,   335,   335,   335,   335,   335,
     335,   337,   338,   340,   342,   347,   347,   350,   351,   352,
     354,   356,   357,   361,   362,   366,   367,   369,   374,   375,
     379,   380,   381,   384,   386,   386,   389,   389,   392,   392,
     395,   395,   398,   398,   401,   401,   404,   404,   406,   406,
     409,   409,   410,   410,   412,   412,   416,   417,   420,   420,
     423,   423,   425,   425,   428,   428,   428,   430,   430,   433,
     433,   436,   436,   436,   437,   437,   438,   438,   441,   441,
     443,   452,   455,   455,   459,   460,   463,   463,   465,   465,
     467,   467,   469,   472,   474,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491
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
  "var_decl_list", "var_decl", "vec_exp", "con_exp", "log_or_exp",
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
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354
};
# endif

#define YYPACT_NINF -286

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-286)))

#define YYTABLE_NINF -126

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     412,  -286,  -286,  1414,  1064,  1114,   509,  1414,  1414,  -286,
    -286,    36,    50,   606,    52,    55,    84,   -47,    99,    71,
     101,   752,   116,   123,  -286,  -286,    97,    96,   -47,   941,
      41,   891,   -47,  -286,  -286,   119,  1566,    97,    49,    97,
    -286,   126,  1414,   133,    97,  -286,  -286,  -286,  -286,  -286,
     147,   412,  -286,  -286,    94,  -286,  -286,  -286,   606,     7,
    -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,   149,
    1634,  -286,   139,  -286,   256,  -286,  -286,   -47,  -286,  -286,
    -286,    12,    93,    82,    83,    86,   -15,    10,    38,    56,
      48,   125,  1464,  -286,   102,    21,  -286,  1414,  1414,     5,
     154,  1515,  -286,   158,  -286,   157,  -286,  1014,   164,   169,
     177,  1414,  1414,    23,  1414,  1414,   679,   190,  1414,  -286,
     240,   161,   -47,  -286,   196,  -286,  -286,  -286,   171,   187,
     186,   145,   199,  1164,    97,  -286,  -286,  -286,  -286,    41,
    1164,  -286,  -286,    76,  -286,  -286,   146,   150,   215,  -286,
    -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,
    -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,
    -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,
      97,  -286,  -286,  -286,  -286,  -286,     8,   -47,   225,   148,
    1414,   220,   -47,  -286,  -286,  -286,  -286,    97,  -286,  -286,
    -286,  -286,  -286,  1414,  1414,  1214,  -286,    76,  -286,  -286,
     187,  -286,   229,  1414,  1515,  1515,  1515,  1515,  1515,  -286,
    -286,  1515,  1515,  1515,  1515,  1515,    97,    97,  -286,    71,
    1414,   -47,    97,   226,  -286,  -286,  -286,  -286,   223,  -286,
    -286,  -286,   227,   228,   232,   233,   234,   239,   -47,   -47,
     818,  -286,   242,  -286,  -286,   236,  -286,  -286,   139,  -286,
      97,   230,   -47,   245,   171,   -47,   186,   246,   -47,   258,
     172,    97,    -6,   260,   247,  -286,   253,   -47,  -286,  -286,
    -286,  -286,   139,   -47,   235,    93,    82,    83,    86,   -15,
      10,    38,    56,    48,   125,  -286,   246,    21,  -286,   180,
    1264,  -286,  1314,  -286,   606,   606,  1414,  1414,   606,   606,
     241,   244,  1364,   186,   -47,  -286,   412,  -286,  -286,  -286,
    -286,    46,   186,   257,  -286,  -286,  -286,   -47,   171,  1414,
     -47,   171,   246,  -286,  1515,   186,  -286,  -286,   266,  -286,
     250,  -286,   276,   279,  -286,  -286,  1414,  1414,   606,   283,
    -286,  -286,   270,   412,   284,  -286,  -286,   289,   -47,  -286,
     246,   243,  -286,  -286,   300,  -286,   186,  -286,  -286,  -286,
     606,   303,   308,  1657,  1680,  -286,   606,  -286,  -286,  -286,
    -286,   310,  -286,   171,  -286,  -286,  -286,  -286,  -286,   606,
     606,  -286,    97,  -286,  -286,  -286,  -286
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    75,   183,     0,     0,     0,     0,     0,     0,    24,
     186,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,   185,     0,     0,     0,     0,
       0,     0,     0,   182,   181,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,   206,   207,    49,   208,   209,
       0,     2,     6,     4,    23,    46,    47,    39,    20,   205,
      44,    40,    41,    43,    42,    37,    38,    45,    36,     0,
      76,   210,   211,    78,     0,   121,     5,     0,    48,   214,
     105,   144,   146,   148,   150,   152,   154,   158,   164,   168,
     172,   177,     0,   179,   188,   192,   200,     0,     0,   205,
       0,     0,   217,     0,   102,     0,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   205,
      80,   200,     0,    70,     0,    72,    73,   190,   125,    29,
     187,    11,    16,     0,     0,    27,   107,   136,     8,     0,
       0,    53,   108,     0,   113,   137,     0,    18,     0,    82,
      98,   175,   174,   176,    86,    87,    88,    89,    90,    85,
      83,    91,    92,   198,   199,    84,   163,   161,   162,   160,
     171,   170,   166,   167,    93,    94,    95,    96,    97,   115,
       0,   118,   116,   120,   119,   117,     0,     0,   194,     0,
       0,     0,     0,   132,     1,     3,    22,     0,    21,   126,
      54,   127,    74,     0,     0,     0,   104,     0,     9,   114,
     140,   106,   138,     0,     0,     0,     0,     0,     0,   156,
     157,     0,     0,     0,     0,     0,     0,     0,   189,     0,
       0,     0,     0,     0,   201,   204,   215,   216,    99,    35,
     213,   212,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,    57,    56,     0,    71,    31,    30,   191,
       0,     0,     0,     0,   125,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   143,     0,     0,    77,    79,
     103,   141,   142,     0,     0,   147,   149,   151,   153,   155,
     159,   165,   169,   173,   178,   180,     0,   193,   203,     0,
       0,   202,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,    10,    13,    17,   128,    19,
     124,     0,     0,     0,    28,   111,   195,     0,   125,   133,
      50,   125,     0,   139,     0,     0,    81,   196,     0,   101,
      68,    59,     0,     0,    65,    67,     0,     0,     0,     0,
      58,    51,     0,    14,     0,    12,   109,     0,     0,   122,
       0,     0,   129,   134,     0,   131,     0,   145,   123,   197,
       0,     0,     0,     0,     0,    61,     0,    52,    15,     7,
     110,    32,    26,   125,   135,   112,    69,    60,    66,     0,
       0,    62,     0,   130,    63,    64,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -286,   267,  -285,    -9,  -286,  -286,   -34,  -118,  -181,    32,
    -286,  -286,   291,  -286,    16,   -70,  -115,   -11,     0,    -5,
      -7,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -102,   170,
    -207,  -286,   287,    -2,  -100,   -19,  -266,   -23,   -28,  -286,
    -286,  -242,   -21,    -1,    -4,    53,   -29,  -286,    -3,  -286,
     110,   117,   122,   118,  -286,   115,   305,   121,   307,   124,
     309,   127,   311,   128,   114,     1,   -52,  -286,  -189,  -286,
     313,   -10,   332
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    50,    51,    52,   261,   354,   355,   131,   146,    53,
     197,    54,    55,    56,   188,   357,    57,    58,    99,   352,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,   233,   204,    71,    72,    73,   322,    74,    75,   180,
      76,   201,    77,   274,    78,   211,   212,    79,    80,    81,
      82,    83,    84,    85,   221,    86,   222,    87,   223,    88,
     224,    89,   225,    90,    91,   101,    93,    94,   189,   301,
     235,    95,    96
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      59,    92,   113,   144,   255,   129,    59,   139,   143,   120,
     136,   276,   142,    59,   250,   259,   129,   117,   129,   119,
     268,   138,   318,   193,   141,   137,   128,   145,   132,   258,
     335,   353,   147,   -80,   199,   230,   199,   128,   108,   128,
     228,   200,   127,   299,   128,   213,   209,   219,   111,   228,
      47,    59,    92,   186,   220,   151,   152,   153,    59,   356,
     327,   244,   112,   245,   114,   208,   366,   115,   353,   270,
     206,   163,   164,   166,   167,   168,   169,   210,   136,   142,
     231,     3,   326,     4,   319,     5,   362,   214,   142,   365,
     198,    47,   119,   234,   382,    28,   116,    30,     7,     8,
     141,   119,  -125,   145,  -125,    47,     9,   232,   172,   173,
     282,   118,    32,   129,   142,   209,   139,   122,   234,   125,
     143,   136,   132,   170,   171,    38,   126,   257,   130,   145,
     208,   148,    39,   207,   128,   187,   137,    43,    44,   373,
     374,   393,   190,    47,   317,   192,   361,   194,   312,   196,
     263,   320,   202,   205,   215,   216,   226,   217,   218,   129,
      38,    42,   229,    32,    40,   236,    45,    46,    47,    48,
      49,   237,   238,   100,   103,   105,   129,   109,   110,   144,
     128,   239,   240,    39,   143,   279,   269,   132,    43,    44,
     241,   124,   147,   251,    47,   254,   267,   128,   350,   256,
     199,     5,     6,   260,   262,   129,   129,   359,   281,   265,
     264,   129,   191,   277,   119,   119,   119,   119,   119,   297,
     368,   119,   119,   119,   119,   119,   128,   128,   266,   119,
     271,   298,   128,   275,   283,   272,   303,   302,   300,   129,
     304,   305,   295,   296,   306,   307,   316,   308,   310,   311,
     129,   385,   309,   314,   230,   313,   206,   268,   321,   325,
     128,   324,   132,   329,   330,   147,   331,   336,   323,   334,
     360,   128,   328,   377,   253,   346,   315,   332,   347,   369,
     206,   242,   243,   210,   246,   247,     9,   370,   252,   371,
     163,   164,   372,   340,   341,   234,   376,   344,   345,   231,
     358,   379,   380,   384,    59,    59,   387,   383,    59,    59,
      28,   388,    30,   207,   351,   392,    59,    92,   195,   378,
     135,   128,   396,   179,   285,   364,   232,   147,   363,   381,
     351,   367,   286,   289,   119,   288,   333,   375,   287,   294,
      38,   181,   290,   182,    40,   183,   291,   184,    59,   185,
     292,   121,   293,    59,    92,     0,     0,     0,   210,   386,
     273,     0,     0,     0,     0,   391,     0,     0,     0,     0,
      59,   358,     0,   278,     0,   105,    59,     0,   394,   395,
       0,     0,     0,   284,     0,     0,     0,     0,     0,    59,
      59,     0,   128,     0,     0,     0,     0,     0,     0,     0,
     105,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     1,     0,     0,     0,     0,
       2,     0,     3,     0,     4,     0,     5,     0,     6,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     9,     0,     0,     0,     0,    10,    11,     0,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
     338,     0,   105,     0,     0,     0,   342,   343,    32,    33,
      34,     0,   349,     0,     0,     0,     0,     0,    35,    36,
      37,     0,     0,     0,     0,     0,    38,     0,    39,   273,
      40,    41,    42,    43,    44,     0,     0,    45,    46,    47,
      48,    49,     1,     0,     0,     0,     0,     2,     0,     3,
       0,     4,     0,     5,     0,     6,   106,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,    10,    11,     0,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    29,     0,   107,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,     0,    39,     0,     0,    41,    42,
      43,    44,     0,     0,    45,    46,    47,    48,    49,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,     0,
       5,     0,     6,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,    11,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    29,     0,   107,     0,     0,     0,     0,     0,     0,
       0,     0,    32,    33,    34,     0,     0,     0,     0,     0,
       0,     0,     1,     0,    37,     0,     0,     2,     0,     3,
       0,     4,    39,     5,     0,    41,    42,    43,    44,     0,
       0,    45,    46,    47,    48,    49,     7,     8,     0,     0,
       0,     0,     0,     0,    10,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
      25,    26,    27,     0,    97,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,     0,     0,
       0,     0,     0,     0,     0,   123,     0,     0,     0,     0,
       2,     0,     3,     0,     4,    39,     5,     0,     0,    42,
      43,    44,   248,   249,    45,    46,    47,    48,    49,     7,
       8,     0,     0,     0,     0,     0,     0,    10,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,    25,    26,    27,     0,    97,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,    32,    33,
      34,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,     0,     5,     0,     0,     0,     0,     0,    39,     0,
       0,     0,    42,    43,    44,     7,     8,    45,    46,    47,
      48,    49,     0,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,    25,
      26,    27,     0,    97,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,    32,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       0,     3,     0,     4,    39,     5,     0,     0,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,     7,     8,
       0,     9,     0,     0,     0,     0,    10,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    20,     0,     0,
       0,    24,    25,    26,    27,     0,   140,     0,    31,     2,
       0,     3,     0,     4,     0,     5,     0,    32,    33,    34,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
       0,     0,     0,     0,     0,    38,    10,    39,     0,    40,
      41,    42,    43,    44,     0,     0,    45,    46,    47,    48,
      49,    24,    25,    26,    27,     0,    29,     0,   133,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    33,    34,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   134,
       0,     0,     2,     0,     3,     0,     4,    39,     5,     0,
      41,    42,    43,    44,     0,     0,    45,    46,    47,    48,
      49,     7,     8,     0,     0,     0,     0,     0,     0,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      20,     0,     0,     0,    24,    25,    26,    27,     0,   140,
       0,   107,     2,     0,     3,     0,     4,   102,     5,     0,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      39,     0,     0,    41,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,    24,    25,    26,    27,     0,    97,
       0,    98,     2,     0,     3,     0,     4,     0,     5,   104,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      39,     0,     0,     0,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,    24,    25,    26,    27,     0,    97,
       0,    98,     2,     0,     3,     0,     4,     0,     5,     0,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      39,     0,     0,     0,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,    24,    25,    26,    27,     0,   140,
       0,   133,     2,     0,     3,     0,     4,     0,     5,   280,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      39,     0,     0,    41,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,    24,    25,    26,    27,     0,    97,
       0,    98,     2,     0,     3,     0,     4,   337,     5,     0,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      39,     0,     0,     0,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,    24,    25,    26,    27,     0,    97,
       0,    98,     2,     0,     3,     0,     4,     0,     5,   339,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      39,     0,     0,     0,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,    24,    25,    26,    27,     0,    97,
       0,    98,     2,     0,     3,     0,     4,   348,     5,     0,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      39,     0,     0,     0,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,    24,    25,    26,    27,     0,    97,
       0,    98,     2,     0,     3,     0,     4,     0,     5,     0,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      39,     0,     0,     0,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,    24,    25,    26,    27,     0,    97,
       0,    98,     2,     0,     3,     0,     4,     0,     5,     0,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      39,     0,     0,     0,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,    24,    25,    26,    27,     0,     0,
       0,     0,     0,     2,     0,     3,     0,     4,     0,     5,
       0,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,   227,     7,     8,     0,     0,     0,     0,     0,     0,
      10,     0,     0,     0,    42,     0,     0,     0,     0,    45,
      46,    47,    48,    49,     0,    24,    25,    26,    27,     0,
     149,     0,   150,   151,   152,   153,     0,     0,     0,     0,
       0,     0,    33,    34,   154,   155,   156,   157,   158,   159,
     160,   161,   162,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
      45,    46,    47,    48,    49,     0,   163,   164,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   165,   166,
     167,   168,   169,   170,   171,     0,   172,   173,   149,   203,
     150,     0,     0,     0,     0,   174,   175,   176,   177,   178,
       0,     0,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   149,     0,   150,     0,     0,     0,     0,     0,     0,
     389,     0,     0,     0,     0,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   149,     0,   150,     0,     0,     0,
       0,     0,     0,   390,     0,     0,   165,     0,   154,   155,
     156,   157,   158,   159,   160,   161,   162,     0,     0,     0,
       0,     0,     0,   174,   175,   176,   177,   178,     0,   165,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   174,   175,   176,   177,
     178,     0,   165,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   174,
     175,   176,   177,   178
};

static const yytype_int16 yycheck[] =
{
       0,     0,    13,    31,   122,    26,     6,    30,    31,    19,
      29,   192,    31,    13,   116,   130,    37,    17,    39,    19,
      12,    30,   264,    44,    31,    29,    26,    31,    28,   129,
     296,   316,    32,    12,    29,    14,    29,    37,     6,    39,
      92,    34,    26,   232,    44,    33,    74,    62,    12,   101,
      97,    51,    51,    37,    69,     7,     8,     9,    58,    13,
      66,    38,    12,    40,    12,    74,   332,    12,   353,   187,
      72,    50,    51,    63,    64,    65,    66,    77,    97,    98,
      59,    10,   271,    12,   265,    14,   328,    75,   107,   331,
      58,    97,    92,    95,   360,    54,    12,    56,    27,    28,
     107,   101,    97,   107,    97,    97,    30,    86,    70,    71,
     210,    12,    66,   134,   133,   143,   139,    16,   120,     3,
     143,   140,   122,    67,    68,    84,     3,   129,    32,   133,
     139,    12,    86,    57,   134,    86,   140,    91,    92,   346,
     347,   383,    16,    97,   262,    12,   327,     0,   250,    55,
     134,   266,     3,    14,    61,    73,    31,    74,    72,   180,
      84,    90,    60,    66,    88,    11,    95,    96,    97,    98,
      99,    13,    15,     3,     4,     5,   197,     7,     8,   207,
     180,    17,    13,    86,   207,   204,   186,   187,    91,    92,
      13,    21,   192,     3,    97,    34,   180,   197,   313,     3,
      29,    14,    16,    58,     5,   226,   227,   322,   210,    59,
      64,   232,    42,   197,   214,   215,   216,   217,   218,   229,
     335,   221,   222,   223,   224,   225,   226,   227,    13,   229,
       5,   231,   232,    13,     5,    87,   238,    14,    12,   260,
      13,    13,   226,   227,    12,    12,    16,    13,   248,   249,
     271,   366,    13,    17,    14,    13,   258,    12,    12,    87,
     260,     3,   262,     3,    17,   265,    13,    87,   268,    34,
      13,   271,   272,     3,    34,    34,   260,   277,    34,    13,
     282,   111,   112,   283,   114,   115,    30,    37,   118,    13,
      50,    51,    13,   304,   305,   297,    13,   308,   309,    59,
     321,    17,    13,     3,   304,   305,     3,    64,   308,   309,
      54,     3,    56,    57,   314,     5,   316,   316,    51,   353,
      29,   321,   392,    36,   214,   330,    86,   327,   329,   358,
     330,   334,   215,   218,   334,   217,   283,   348,   216,   225,
      84,    36,   221,    36,    88,    36,   222,    36,   348,    36,
     223,    19,   224,   353,   353,    -1,    -1,    -1,   358,   370,
     190,    -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,
     370,   392,    -1,   203,    -1,   205,   376,    -1,   389,   390,
      -1,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,   389,
     390,    -1,   392,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     230,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    -1,    14,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    30,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
     300,    -1,   302,    -1,    -1,    -1,   306,   307,    66,    67,
      68,    -1,   312,    -1,    -1,    -1,    -1,    -1,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    84,    -1,    86,   329,
      88,    89,    90,    91,    92,    -1,    -1,    95,    96,    97,
      98,    99,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    89,    90,
      91,    92,    -1,    -1,    95,    96,    97,    98,    99,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    -1,
      14,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    78,    -1,    -1,     8,    -1,    10,
      -1,    12,    86,    14,    -1,    89,    90,    91,    92,    -1,
      -1,    95,    96,    97,    98,    99,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    86,    14,    -1,    -1,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    90,    91,    92,    27,    28,    95,    96,    97,
      98,    99,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    12,    86,    14,    -1,    -1,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    27,    28,
      -1,    30,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    52,    53,    -1,    55,    -1,    57,     8,
      -1,    10,    -1,    12,    -1,    14,    -1,    66,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    -1,    84,    35,    86,    -1,    88,
      89,    90,    91,    92,    -1,    -1,    95,    96,    97,    98,
      99,    50,    51,    52,    53,    -1,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,     8,    -1,    10,    -1,    12,    86,    14,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    96,    97,    98,
      99,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    57,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    50,    51,    52,    53,    -1,    55,
      -1,    57,     8,    -1,    10,    -1,    12,    -1,    14,    15,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      86,    -1,    -1,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    50,    51,    52,    53,    -1,    55,
      -1,    57,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      86,    -1,    -1,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    50,    51,    52,    53,    -1,    55,
      -1,    57,     8,    -1,    10,    -1,    12,    -1,    14,    15,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      86,    -1,    -1,    89,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    50,    51,    52,    53,    -1,    55,
      -1,    57,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      86,    -1,    -1,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    50,    51,    52,    53,    -1,    55,
      -1,    57,     8,    -1,    10,    -1,    12,    -1,    14,    15,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      86,    -1,    -1,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    50,    51,    52,    53,    -1,    55,
      -1,    57,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      86,    -1,    -1,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    50,    51,    52,    53,    -1,    55,
      -1,    57,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      86,    -1,    -1,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    50,    51,    52,    53,    -1,    55,
      -1,    57,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      86,    -1,    -1,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    12,    -1,    14,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,    -1,    50,    51,    52,    53,    -1,
       4,    -1,     6,     7,     8,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,    -1,    50,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,     4,     5,
       6,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     4,    -1,     6,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     4,    -1,     6,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    -1,    -1,    62,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83
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
     148,   149,   150,   151,   152,   153,   155,   157,   159,   161,
     163,   164,   165,   166,   167,   171,   172,    55,    57,   118,
     129,   165,    13,   129,    15,   129,    17,    57,   109,   129,
     129,    12,    12,   117,    12,    12,    12,   118,    12,   118,
     171,   172,    16,     3,   129,     3,     3,   114,   118,   142,
      32,   107,   118,    57,    78,   112,   135,   144,   103,   137,
      55,   120,   135,   137,   138,   144,   108,   118,    12,     4,
       6,     7,     8,     9,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    50,    51,    62,    63,    64,    65,    66,
      67,    68,    70,    71,    79,    80,    81,    82,    83,   132,
     139,   156,   158,   160,   162,   170,   114,    86,   114,   168,
      16,   129,    12,   142,     0,   101,    55,   110,   109,    29,
      34,   141,     3,     5,   132,    14,   133,    57,   103,   138,
     118,   145,   146,    33,    75,    61,    73,    74,    72,    62,
      69,   154,   156,   158,   160,   162,    31,    77,   166,    60,
      14,    59,    86,   131,   133,   170,    11,    13,    15,    17,
      13,    13,   129,   129,    38,    40,   129,   129,    93,    94,
     128,     3,   129,    34,    34,   107,     3,   133,   134,   116,
      58,   104,     5,   114,    64,    59,    13,   114,    12,   118,
     107,     5,    87,   129,   143,    13,   108,   114,   129,   135,
      15,   133,   134,     5,   129,   150,   151,   152,   153,   155,
     157,   159,   161,   163,   164,   114,   114,   171,   118,   168,
      12,   169,    14,   133,    13,    13,    12,    12,    13,    13,
     118,   118,   128,    13,    17,   114,    16,   107,   141,   108,
     116,    12,   136,   118,     3,    87,   168,    66,   118,     3,
      17,    13,   118,   145,    34,   136,    87,    13,   129,    15,
     117,   117,   129,   129,   117,   117,    34,    34,    13,   129,
     116,   118,   119,   102,   105,   106,    13,   115,   142,   116,
      13,   108,   141,   143,   119,   141,   136,   148,   116,    13,
      37,    13,    13,   130,   130,   117,    13,     3,   106,    17,
      13,   146,   136,    64,     3,   116,   117,     3,     3,    13,
      13,   117,     5,   141,   117,   117,   115
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
     146,   146,   146,   147,   148,   148,   149,   149,   150,   150,
     151,   151,   152,   152,   153,   153,   154,   154,   155,   155,
     156,   156,   156,   156,   157,   157,   158,   158,   159,   159,
     160,   160,   161,   161,   162,   162,   162,   163,   163,   164,
     164,   165,   165,   165,   165,   165,   165,   165,   166,   166,
     166,   166,   167,   167,   168,   168,   169,   169,   170,   170,
     171,   171,   171,   171,   171,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172
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
       1,     2,     2,     3,     1,     5,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     3,     1,     1,     1,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       2,     3,     1,     3,     1,     3,     2,     3,     1,     1,
       1,     2,     3,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     1,     3,     3,     2
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
#line 97 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_def(((*yyvaluep).class_def)); }
#line 1777 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 104: /* class_ext  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1783 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 105: /* class_body  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1789 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 106: /* class_body2  */
#line 98 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1795 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 112: /* func_ptr  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1801 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 113: /* stmt_typedef  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1807 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 114: /* type_decl2  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1813 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 116: /* code_segment  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1819 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 117: /* stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1825 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 120: /* enum_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1831 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 121: /* label_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1837 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 122: /* goto_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1843 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 123: /* case_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1849 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 124: /* switch_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1855 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 125: /* loop_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1861 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 126: /* selection_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1867 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 127: /* jump_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1873 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 128: /* exp_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1879 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 129: /* exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1885 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 130: /* binary_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1891 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 131: /* template  */
#line 100 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 1897 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 135: /* decl_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1903 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 142: /* type_decl  */
#line 99 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1909 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 144: /* union_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1915 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 148: /* con_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1921 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 149: /* log_or_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1927 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 150: /* log_and_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1933 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 151: /* inc_or_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1939 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 152: /* exc_or_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1945 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 153: /* and_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1951 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 155: /* eq_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1957 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 157: /* relational_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1963 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 159: /* shift_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1969 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 161: /* add_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1975 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 163: /* mul_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1981 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 164: /* cast_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1987 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 166: /* unary_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1993 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 167: /* dur_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 1999 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 168: /* type_list  */
#line 100 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 2005 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 169: /* call_paren  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 2011 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 171: /* post_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 2017 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 172: /* primary_exp  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_exp(((*yyvaluep).exp)); }
#line 2023 "src/ast/parser.c" /* yacc.c:1257  */
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
#line 107 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL, get_pos(arg));  }
#line 2291 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 108 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(arg)); }
#line 2297 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 112 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(arg)); }
#line 2303 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 113 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(arg)); }
#line 2309 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 114 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(arg)); }
#line 2315 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 119 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def(0, (yyvsp[-4].id_list), (yyvsp[-3].type_decl), (yyvsp[-1].class_body), get_pos(arg)); }
#line 2321 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 120 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_global); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2327 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 122 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].class_def), free_class_def); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2333 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 124 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2339 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 124 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = NULL; }
#line 2345 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 126 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2351 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 129 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body((yyvsp[0].section), NULL, get_pos(arg)); }
#line 2357 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 130 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body((yyvsp[-1].section), (yyvsp[0].class_body), get_pos(arg)); }
#line 2363 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 134 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2369 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 135 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2375 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 139 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2381 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 140 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2387 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 144 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(arg));}
#line 2393 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 145 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(arg));}
#line 2399 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 149 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_static;   }
#line 2405 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 150 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2411 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 154 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2417 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 155 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2423 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 159 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-2].sym), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(arg)); }
#line 2429 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 161 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_ptr), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2435 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 167 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_typedef((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2441 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 171 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(arg)); }
#line 2447 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 172 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(arg)); }
#line 2453 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 176 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(arg)); }
#line 2459 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 177 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(arg)); }
#line 2465 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 181 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code(NULL, get_pos(arg)); }
#line 2471 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 182 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code((yyvsp[-1].stmt_list), get_pos(arg)); }
#line 2477 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 201 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = insert_symbol((yyvsp[0].sval)); }
#line 2483 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 203 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = NULL; }
#line 2489 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 206 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sym), get_pos(arg)); }
#line 2495 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 207 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2501 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 211 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 1, get_pos(arg)); }
#line 2507 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 215 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 0, get_pos(arg)); }
#line 2513 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 219 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(arg)); }
#line 2519 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 220 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(arg)); }
#line 2525 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 224 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg));}
#line 2531 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 229 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while((yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2537 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 231 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while((yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2543 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 233 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2549 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 235 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2555 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 237 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2561 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 239 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); (yyval.stmt)->d.stmt_auto.is_ptr = 1;}
#line 2567 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 241 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until((yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2573 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 243 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until((yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2579 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 245 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2585 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 250 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(arg)); }
#line 2591 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 252 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(arg)); }
#line 2597 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 256 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return(NULL, get_pos(arg)); }
#line 2603 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 257 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return((yyvsp[-1].exp), get_pos(arg)); }
#line 2609 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 258 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_break(get_pos(arg)); }
#line 2615 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 259 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_continue(get_pos(arg)); }
#line 2621 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 263 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp((yyvsp[-1].exp),   get_pos(arg)); }
#line 2627 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 264 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp(NULL, get_pos(arg)); }
#line 2633 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 269 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_exp((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 2639 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 274 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2645 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 277 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 2651 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 277 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2657 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2663 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2669 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2675 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2681 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2687 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2693 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2699 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2705 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2711 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2717 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2723 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2729 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2735 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2741 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 285 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2747 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 285 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2753 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 286 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2759 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 290 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub((yyvsp[-1].exp), get_pos(arg)); }
#line 2765 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 291 "utils/gwion.y" /* yacc.c:1646  */
    { if((yyvsp[-2].exp)->next){ gwion_error(arg, "invalid format for array init [...][...]..."); free_exp((yyvsp[-2].exp)); free_array_sub((yyvsp[0].array_sub)); YYERROR; } (yyval.array_sub) = prepend_array_sub((yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2771 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 292 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [...][]..."); free_exp((yyvsp[-3].exp)); YYERROR; }
#line 2777 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 296 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(arg)); }
#line 2783 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 297 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2789 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 298 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [][...]"); free_array_sub((yyvsp[-1].array_sub)); free_array_sub((yyvsp[0].array_sub)); YYERROR; }
#line 2795 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 303 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2801 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 305 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.type, ae_flag_static); (yyval.exp) = (yyvsp[0].exp); }
#line 2807 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 307 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.type, ae_flag_private); (yyval.exp) = (yyvsp[0].exp); }
#line 2813 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 311 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2819 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 312 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2825 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 315 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2831 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 319 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sym), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2837 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 321 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_private); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2843 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 323 "utils/gwion.y" /* yacc.c:1646  */
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
#line 2859 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 339 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_op , (yyvsp[-2].type_decl), OP_SYM((yyvsp[-3].ival)), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2865 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 341 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_op | ae_flag_unary, (yyvsp[-2].type_decl), OP_SYM((yyvsp[-4].ival)), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2871 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 343 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_dtor, new_type_decl(new_id_list(insert_symbol("void"), get_pos(arg)), 0,
      get_pos(arg)), insert_symbol("dtor"), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2878 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 347 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2884 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 347 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 2890 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 350 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)), (yyvsp[0].ival), get_pos(arg)); }
#line 2896 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 351 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2902 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 352 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)),
      (yyvsp[0].ival), get_pos(arg)); (yyval.type_decl)->types = (yyvsp[-3].type_list); }
#line 2909 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 354 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg));
      (yyval.type_decl)->types = (yyvsp[-5].type_list); }
#line 2916 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 356 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2922 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 357 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].type_decl), ae_flag_const); (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2928 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 361 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2934 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 362 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2940 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 366 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(arg));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sym); }
#line 2946 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 368 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2952 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 370 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2958 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 374 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(arg)); }
#line 2964 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 375 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2970 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 379 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sym), NULL, get_pos(arg)); }
#line 2976 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 380 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2982 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 381 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2988 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 384 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec(    (yyvsp[-1].exp), get_pos(arg)); }
#line 2994 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 387 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_if((yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3000 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 390 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(arg)); }
#line 3006 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 393 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(arg)); }
#line 3012 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 396 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(arg)); }
#line 3018 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 399 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(arg)); }
#line 3024 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 402 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(arg)); }
#line 3030 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 404 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 3036 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 404 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_neq; }
#line 3042 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 407 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3048 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 409 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_lt; }
#line 3054 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 409 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_gt; }
#line 3060 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 410 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_le; }
#line 3066 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 410 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ge; }
#line 3072 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 413 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3078 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 416 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 3084 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 417 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 3090 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 421 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3096 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 423 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3102 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 423 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3108 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 426 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3114 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 428 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3120 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 428 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide; }
#line 3126 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 428 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_percent; }
#line 3132 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 431 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3138 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 434 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_cast((yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(arg)); }
#line 3144 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 436 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 3150 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 436 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 3156 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 436 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 3162 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 437 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3168 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 437 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3174 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 438 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 3180 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 438 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 3186 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 442 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary((yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3192 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 444 "utils/gwion.y" /* yacc.c:1646  */
    {
      if((yyvsp[0].type_decl)->array && !(yyvsp[0].type_decl)->array->exp_list) {
        free_type_decl((yyvsp[0].type_decl));
        gwion_error(arg, "can't use empty '[]' in 'new' expression");
        YYERROR;
      }
      (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), get_pos(arg));
    }
#line 3205 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 453 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary3(op_spork, (yyvsp[0].stmt), get_pos(arg)); }
#line 3211 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 456 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3217 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 459 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list((yyvsp[0].type_decl), NULL, get_pos(arg)); }
#line 3223 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 460 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list((yyvsp[-2].type_decl), (yyvsp[0].type_list), get_pos(arg)); }
#line 3229 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 463 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 3235 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 463 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3241 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 465 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3247 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 465 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3253 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 468 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array((yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(arg)); }
#line 3259 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 470 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_call((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg));
      if((yyvsp[-1].type_list))(yyval.exp)->d.exp_func.tmpl = new_tmpl_call((yyvsp[-1].type_list)); }
#line 3266 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 473 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dot((yyvsp[-2].exp), (yyvsp[0].sym), get_pos(arg)); }
#line 3272 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 475 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_post((yyvsp[-1].exp), (yyvsp[0].ival), get_pos(arg)); }
#line 3278 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 479 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_id(     (yyvsp[0].sym), get_pos(arg)); }
#line 3284 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 480 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_int(    (yyvsp[0].ival), get_pos(arg)); }
#line 3290 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 481 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_float(  (yyvsp[0].fval), get_pos(arg)); }
#line 3296 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 482 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_string( (yyvsp[0].sval), get_pos(arg)); }
#line 3302 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 483 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_char(   (yyvsp[0].sval), get_pos(arg)); }
#line 3308 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 484 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3314 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 485 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3320 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 486 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_complex((yyvsp[-1].exp), get_pos(arg)); }
#line 3326 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 487 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_polar((yyvsp[-1].exp), get_pos(arg)); }
#line 3332 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 488 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_vec(    (yyvsp[0].vec), get_pos(arg)); }
#line 3338 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 489 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_hack(   (yyvsp[-1].exp), get_pos(arg)); }
#line 3344 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 490 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) =                       (yyvsp[-1].exp);                    }
#line 3350 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 217:
#line 491 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_nil(        get_pos(arg)); }
#line 3356 "src/ast/parser.c" /* yacc.c:1646  */
    break;


#line 3360 "src/ast/parser.c" /* yacc.c:1646  */
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
#line 493 "utils/gwion.y" /* yacc.c:1906  */

