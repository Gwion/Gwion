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
#define YYFINAL  206
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1734

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  100
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  73
/* YYNRULES -- Number of rules.  */
#define YYNRULES  224
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  429

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
     139,   140,   144,   145,   149,   151,   153,   158,   160,   164,
     165,   166,   170,   171,   175,   176,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   195,
     197,   197,   200,   201,   202,   203,   207,   211,   215,   216,
     220,   224,   226,   228,   230,   232,   234,   236,   238,   240,
     245,   247,   252,   253,   254,   255,   259,   260,   264,   265,
     269,   270,   273,   273,   275,   275,   275,   276,   276,   277,
     277,   278,   278,   279,   279,   280,   280,   280,   281,   281,
     282,   286,   287,   288,   292,   293,   294,   298,   299,   300,
     302,   304,   309,   310,   314,   315,   318,   321,   323,   325,
     338,   338,   338,   338,   338,   338,   340,   341,   343,   345,
     350,   350,   353,   354,   355,   357,   359,   360,   364,   365,
     369,   370,   372,   374,   376,   381,   382,   386,   387,   388,
     391,   391,   394,   394,   397,   397,   400,   400,   403,   403,
     406,   406,   409,   409,   411,   411,   414,   414,   415,   415,
     417,   417,   421,   422,   425,   425,   428,   428,   430,   430,
     433,   433,   433,   435,   435,   438,   438,   441,   441,   441,
     442,   442,   443,   443,   446,   446,   448,   457,   460,   460,
     464,   465,   468,   468,   470,   470,   472,   472,   474,   477,
     479,   483,   484,   485,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498
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

#define YYPACT_NINF -365

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-365)))

#define YYTABLE_NINF -131

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     419,  -365,  -365,  1388,  1082,  1133,   516,  -365,  -365,    -7,
    -365,    50,    62,   613,    79,    85,    88,   -45,    96,   839,
     108,   738,   126,   138,  -365,  -365,    40,   118,   -45,   914,
     491,   863,   980,   -45,  -365,  -365,   144,  1543,    40,    75,
      40,   152,  -365,   158,    40,  -365,  -365,  -365,  -365,  -365,
     172,   419,  -365,  -365,  -365,  -365,  -365,   613,    -2,  -365,
    -365,  -365,  -365,  -365,  -365,  -365,  -365,  -365,   178,  1578,
    -365,   176,  -365,   117,  -365,  -365,   -45,  -365,  -365,     1,
     130,   122,   128,   137,    90,   141,   -29,    97,   111,   181,
    1439,  -365,   154,    52,  1388,  -365,  1388,  1388,  1388,     7,
     205,  1491,  -365,   208,  -365,   203,  -365,   246,   980,   206,
    -365,    40,  1388,  1388,   104,  1388,  1388,   687,   219,  1388,
    -365,    65,   196,   -45,  -365,   228,  -365,  -365,  -365,   210,
     227,   232,   185,   245,  1031,   131,  1031,  1031,    40,  -365,
    -365,   131,   436,     3,     3,    40,  -365,  -365,   -17,  -365,
       3,  -365,  -365,    -1,  -365,  -365,  -365,  -365,  -365,   186,
     193,   241,  -365,  -365,  -365,  -365,  -365,  -365,  -365,  -365,
    -365,  -365,  -365,  -365,  -365,  -365,  -365,  -365,  -365,  -365,
    -365,  -365,  -365,  -365,  -365,  -365,  -365,  -365,  -365,  -365,
    -365,  -365,  -365,    40,  -365,  -365,  -365,  -365,  -365,    12,
     -45,   251,   170,  1388,   -45,  -365,  -365,  -365,  -365,  -365,
    -365,  -365,  -365,  1388,  1388,  1184,  -365,   -17,    -1,  -365,
    -365,   227,  -365,   258,  1388,  1491,  1491,  1491,  1491,  1491,
    -365,  -365,  1491,  1491,  1491,  1491,  1491,    40,    40,  -365,
     839,  1388,   -45,    40,   252,  -365,  -365,   254,  -365,  -365,
     255,  -365,   -45,   259,   262,   256,   264,   265,   267,   -45,
     -45,   789,  -365,   271,  -365,  -365,   270,  -365,  -365,   176,
    -365,    40,   272,   -45,   131,   131,   269,    13,   210,   -45,
     232,   277,   -45,   288,   209,    40,    46,   293,   291,   296,
    -365,  -365,  -365,  -365,   176,   -45,   278,   130,   122,   128,
     137,    90,   141,   -29,    97,   111,   181,  -365,   277,    52,
    -365,   225,  1235,  -365,  -365,  1286,  -365,   277,   613,   613,
    1388,  1388,   613,   613,   283,   286,  1337,   232,   -45,  -365,
     419,  -365,   -45,   -45,   318,  -365,  -365,  -365,    40,   311,
     313,  -365,  -365,  -365,   -45,   210,  1388,   -45,   210,  -365,
    1491,   316,  -365,  -365,   323,  -365,    18,   294,  -365,   328,
     329,  -365,  -365,  1388,  1388,   613,   330,  -365,  -365,   342,
     419,   331,  -365,   333,   336,  -365,  -365,   -45,   232,   277,
     287,  -365,  -365,   348,  -365,  -365,   232,  -365,  -365,   295,
     232,   613,   353,   355,  1624,  1650,  -365,   613,  -365,  -365,
    -365,   277,   277,   354,  -365,    18,   210,  -365,  -365,   300,
    -365,  -365,  -365,  -365,   613,   613,  -365,    18,    18,    40,
    -365,  -365,   351,  -365,  -365,  -365,  -365,  -365,  -365
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    77,   189,     0,     0,     0,     0,   212,   211,    23,
     192,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   190,   191,     0,     0,     0,     0,
       0,     0,     0,     0,   188,   187,     0,     0,     0,     0,
       0,     0,   213,     0,     0,   215,   216,    49,   217,   218,
       0,     2,     6,     4,    46,    47,    39,    20,   214,    44,
      40,    41,    43,    42,    37,    38,    45,    36,     0,    78,
     219,   220,    80,     0,   126,     5,     0,    48,   107,   150,
     152,   154,   156,   158,   160,   164,   170,   174,   178,   183,
       0,   185,   194,   198,     0,   206,     0,     0,     0,   214,
       0,     0,   224,     0,   104,     0,    34,     0,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     214,    82,   206,     0,    72,     0,    74,    75,   196,   130,
      29,   193,    11,    16,     0,     0,     0,     0,     0,   109,
     141,     0,     0,     0,     0,     0,     8,    55,     0,   144,
       0,    53,   110,     0,   118,   142,    54,   111,   143,     0,
      18,     0,    84,   100,   181,   180,   182,    88,    89,    90,
      91,    92,    87,    85,    93,    94,   204,   205,    86,   169,
     167,   168,   166,   177,   176,   172,   173,    95,    96,    97,
      98,    99,   120,     0,   123,   121,   125,   124,   122,     0,
       0,   200,     0,     0,     0,   137,     1,     3,    21,   131,
      56,   132,    76,     0,     0,     0,   106,     0,     0,     9,
     119,   147,   108,   145,     0,     0,     0,     0,     0,     0,
     162,   163,     0,     0,     0,     0,     0,     0,     0,   195,
       0,     0,     0,     0,     0,   207,   210,     0,   222,   223,
     101,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    59,    58,     0,    73,    31,    30,
     197,     0,     0,     0,     0,     0,     0,     0,   130,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      79,    81,   105,   148,   149,     0,     0,   153,   155,   157,
     159,   161,   165,   171,   175,   179,   184,   186,     0,   199,
     209,     0,     0,   208,   221,     0,   102,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    10,
      13,    17,     0,     0,     0,   133,    19,   129,   112,     0,
       0,    27,   116,   201,     0,   130,   138,    50,   130,   146,
       0,     0,    83,   202,     0,   103,     0,    70,    61,     0,
       0,    67,    69,     0,     0,     0,     0,    60,    51,     0,
      14,     0,    12,     0,     0,    28,   113,     0,     0,     0,
       0,   134,   139,     0,   136,   151,     0,   203,   114,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    52,    15,
       7,     0,     0,    32,   127,     0,   130,   140,   128,     0,
     117,    71,    62,    68,     0,     0,    64,     0,     0,     0,
      24,   135,     0,    65,    66,    25,    26,    33,   115
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -365,   310,  -295,    -8,  -365,  -365,    -3,  -114,  -181,    24,
    -365,  -365,  -365,    56,   -53,  -116,   -12,     0,    22,   268,
    -365,  -365,  -365,  -365,  -365,  -365,  -365,  -110,    42,  -184,
    -365,   334,    -4,  -125,   308,  -296,  -364,   -20,   -23,  -365,
    -365,  -260,   -24,    26,    41,    78,     5,    28,  -365,   155,
     157,   159,   153,  -365,   160,   347,   161,   350,   163,   357,
     156,   361,   164,   165,     4,   -21,  -365,  -150,  -365,   363,
     -16,  -365,   373
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    50,    51,    52,   272,   371,   372,   132,   159,    53,
     111,    54,    55,   201,   376,    56,    57,    99,   369,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
     244,   214,    70,    71,    72,   339,   390,    73,    74,   193,
      75,   211,    76,   288,    77,   222,   223,    78,    79,    80,
      81,    82,    83,   232,    84,   233,    85,   234,    86,   235,
      87,   236,    88,    89,   101,    91,    92,   202,   313,   246,
      93,    94,    95
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,   114,   130,   121,    90,   269,    58,   261,   154,   266,
     148,   153,   351,    58,   130,   270,   130,   118,   335,   120,
     205,   356,   146,   289,   282,   333,   129,   209,   133,     9,
     109,   388,   210,   160,   224,   370,   209,    28,   129,   217,
     129,   420,   185,   186,   129,   100,   103,   105,   110,    20,
     220,    58,    47,   425,   426,    90,   218,    58,   141,   150,
     143,   144,   112,   125,   -82,   219,   241,   216,    39,   239,
     140,   149,   155,   158,   113,   370,   221,   225,   389,   241,
     239,   208,   128,   405,    39,   381,   284,   130,   384,   245,
     120,   115,    41,   311,   199,  -130,   294,   116,   336,   264,
     117,   120,   176,   177,  -130,   417,   418,    33,   119,    47,
      47,   129,   242,   344,   130,   176,   177,   245,   164,   165,
     166,   130,   148,   133,   123,   242,   268,    40,   148,   126,
     220,    43,    44,   153,   146,   343,   247,    47,   129,   243,
     219,   127,   255,    47,   256,   129,   421,     9,   149,   155,
     131,   326,   243,   230,   253,   254,   161,   257,   258,   331,
     231,   263,   200,   380,   337,   183,   184,   252,   203,   130,
     204,    28,   206,   217,   218,   140,   149,   155,   158,   394,
     395,   212,   140,   149,   155,   158,   141,   135,   274,   275,
     215,   149,   226,   129,   276,   154,   227,   148,   153,   283,
     133,   277,    39,   228,   160,   179,   180,   181,   182,   146,
     229,   367,   237,   130,   130,   240,   248,   293,   250,   130,
      41,   249,   262,   251,   309,   120,   120,   120,   120,   120,
     265,   267,   120,   120,   120,   120,   120,   129,   129,   209,
     120,     5,   310,   129,   271,   287,   316,   130,     6,   281,
     273,   278,   317,   279,   280,   290,   285,   105,   286,   324,
     325,   130,   404,   295,   312,   216,   296,   314,   320,   315,
     408,   129,   318,   133,   410,   319,   321,   334,   322,   160,
     323,   332,   340,   105,   327,   129,   345,   328,   330,   338,
     216,   341,    20,   307,   308,   221,   346,   342,   147,   151,
     156,   141,   150,   143,   144,   245,   357,   358,   347,   348,
     361,   362,   350,   352,   377,   155,   158,   363,    58,    58,
     364,   375,    58,    58,   378,   145,   379,   329,   368,   386,
      58,   391,   373,   374,    90,    41,   387,   139,   129,   152,
     157,   392,   393,   397,   160,   398,   401,   368,   400,   402,
     120,   407,   406,   396,   354,   409,   412,   105,   413,   419,
     422,   207,   359,   360,   428,    58,   427,   399,   366,   383,
      58,   192,   382,   349,    90,   147,   151,   221,   385,   411,
     297,   300,   403,   298,   194,   416,   299,   195,   287,   301,
     304,    58,   122,   302,   196,   377,   303,    58,   197,   305,
     198,   306,   423,   424,   139,   152,   157,     0,     0,     0,
     147,   151,   156,     0,    58,    58,   152,     0,   147,   129,
       0,     0,     1,     0,     0,     0,     0,     2,     0,     3,
       0,     4,     0,     5,     0,     6,     0,     0,     0,     0,
       0,     0,   139,     0,   152,   157,     7,     8,     0,     9,
       0,     0,     0,     0,    10,    11,     0,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,     0,     0,
       0,     0,    20,     0,     0,     0,    33,    34,    35,     0,
      28,   141,   142,   143,   144,     0,    36,    37,    38,     0,
       0,     0,     0,     0,    39,     0,    40,     0,    41,    42,
      43,    44,     0,     0,    45,    46,    47,    48,    49,     1,
       0,    39,   291,     0,     2,    41,     3,     0,     4,     0,
       5,     0,     6,   106,     0,     0,     0,    20,     0,     0,
       0,     0,     0,     7,     8,    28,   141,   142,   143,   144,
       0,    10,    11,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
     145,    29,   107,   108,    32,     0,    39,     0,     0,     0,
      41,     0,     0,    33,    34,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    40,     0,    41,    42,    43,    44,     0,
       0,    45,    46,    47,    48,    49,     1,     0,     0,     0,
       0,     2,     0,     3,     0,     4,     0,     5,     0,     6,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,    10,    11,
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    29,   107,
     108,    32,     0,     0,     0,     0,     0,     0,     0,     0,
      33,    34,    35,     0,     0,     0,     0,     0,     0,     0,
       1,     0,    38,     0,     0,     2,     0,     3,     0,     4,
      40,     5,    41,    42,    43,    44,     0,     0,    45,    46,
      47,    48,    49,     0,     7,     8,     0,     0,     0,     0,
       0,     0,    10,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24,    25,    26,
      27,   124,    96,     0,    97,    98,     2,     0,     3,     0,
       4,     0,     5,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    40,     0,     0,    42,    43,    44,
     259,   260,    45,    46,    47,    48,    49,     0,    24,    25,
      26,    27,     1,    96,     0,    97,    98,     2,     0,     3,
       0,     4,     0,     5,     0,    33,    34,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,    10,    40,     0,     0,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,     0,    24,
      25,    26,    27,     0,    96,     0,    97,    98,     0,     3,
       0,     4,     0,     5,     0,     0,    33,    34,    35,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     2,     0,     3,     0,     4,    40,     5,     0,    42,
      43,    44,     0,     0,    45,    46,    47,    48,    49,     0,
       7,     8,     0,     9,     0,     0,     0,     0,    10,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    20,
       0,     0,     0,    24,    25,    26,    27,     0,   134,   150,
      31,    32,     2,     0,     3,     0,     4,     0,     5,    42,
      33,    34,    35,     0,    45,    46,    47,    48,    49,     0,
       0,     7,     8,     0,     0,     0,     0,     0,    39,    10,
      40,     0,    41,    42,    43,    44,     0,     0,    45,    46,
      47,    48,    49,     0,    24,    25,    26,    27,     0,   134,
     135,   136,   137,     0,     0,     0,     0,     0,     0,     0,
       0,    33,    34,    35,     0,     0,     0,     0,     2,     0,
       3,     0,     4,   138,     5,     0,     0,     0,     0,     0,
       0,    40,     0,    41,    42,    43,    44,     7,     8,    45,
      46,    47,    48,    49,     0,    10,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    20,     0,     0,     0,
      24,    25,    26,    27,     0,   134,   150,   108,    32,     2,
       0,     3,     0,     4,     0,     5,     0,    33,    34,    35,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     8,
       0,     0,     0,     0,     0,     0,    10,    40,     0,    41,
      42,    43,    44,     0,     0,    45,    46,    47,    48,    49,
       0,    24,    25,    26,    27,     0,   134,   135,   136,   137,
       2,     0,     3,     0,     4,   102,     5,     0,    33,    34,
      35,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,     0,     0,    10,    40,     0,
      41,    42,    43,    44,     0,     0,    45,    46,    47,    48,
      49,     0,    24,    25,    26,    27,     0,    96,     0,    97,
      98,     2,     0,     3,     0,     4,     0,     5,   104,    33,
      34,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,    10,    40,
       0,     0,    42,    43,    44,     0,     0,    45,    46,    47,
      48,    49,     0,    24,    25,    26,    27,     0,    96,     0,
      97,    98,     2,     0,     3,     0,     4,     0,     5,   292,
      33,    34,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,    10,
      40,     0,     0,    42,    43,    44,     0,     0,    45,    46,
      47,    48,    49,     0,    24,    25,    26,    27,     0,    96,
       0,    97,    98,     2,     0,     3,     0,     4,   353,     5,
       0,    33,    34,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,     0,     0,
      10,    40,     0,     0,    42,    43,    44,     0,     0,    45,
      46,    47,    48,    49,     0,    24,    25,    26,    27,     0,
      96,     0,    97,    98,     2,     0,     3,     0,     4,     0,
       5,   355,    33,    34,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     8,     0,     0,     0,     0,     0,
       0,    10,    40,     0,     0,    42,    43,    44,     0,     0,
      45,    46,    47,    48,    49,     0,    24,    25,    26,    27,
       0,    96,     0,    97,    98,     2,     0,     3,     0,     4,
     365,     5,     0,    33,    34,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
       0,     0,    10,    40,     0,     0,    42,    43,    44,     0,
       0,    45,    46,    47,    48,    49,     0,    24,    25,    26,
      27,     0,    96,     0,    97,    98,     2,     0,     3,     0,
       4,     0,     5,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    40,     0,     0,    42,    43,    44,
       0,     0,    45,    46,    47,    48,    49,     0,    24,    25,
      26,    27,     0,    96,     0,    97,    98,     2,     0,     3,
       0,     4,     0,     5,     0,    33,    34,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,    10,    40,     0,     0,    42,    43,
      44,     0,     0,    45,    46,    47,    48,    49,     0,    24,
      25,    26,    27,     0,     0,     0,     0,     0,     0,     2,
       0,     3,     0,     4,     0,     5,     0,    34,    35,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     7,     8,
       0,     0,     0,     0,     0,     0,    10,     0,     0,    42,
       0,     0,     0,     0,    45,    46,    47,    48,    49,     0,
       0,    24,    25,    26,    27,     0,     0,   162,     0,   163,
     164,   165,   166,     0,     0,     0,     0,     0,     0,    34,
      35,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    42,   162,   213,   163,     0,    45,    46,    47,    48,
      49,     0,     0,   176,   177,     0,   167,   168,   169,   170,
     171,   172,   173,   174,   175,     0,   178,   179,   180,   181,
     182,   183,   184,     0,   185,   186,     0,     0,     0,     0,
       0,     0,     0,   187,   188,   189,   190,   191,   162,     0,
     163,     0,     0,     0,     0,     0,     0,   414,     0,     0,
       0,   178,   167,   168,   169,   170,   171,   172,   173,   174,
     175,     0,     0,     0,   162,     0,   163,     0,   187,   188,
     189,   190,   191,   415,     0,     0,     0,     0,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   178,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   187,   188,   189,   190,   191,     0,
       0,     0,     0,   178,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     187,   188,   189,   190,   191
};

static const yytype_int16 yycheck[] =
{
       0,    13,    26,    19,     0,   130,     6,   117,    31,   123,
      30,    31,   308,    13,    38,   131,    40,    17,   278,    19,
      44,   317,    30,   204,    12,    12,    26,    29,    28,    30,
       6,    13,    34,    33,    33,   330,    29,    54,    38,    56,
      40,   405,    71,    72,    44,     3,     4,     5,    55,    46,
      73,    51,    97,   417,   418,    51,    57,    57,    55,    56,
      57,    58,    12,    21,    12,    73,    14,    71,    85,    90,
      29,    30,    31,    32,    12,   370,    76,    76,    60,    14,
     101,    57,    26,   379,    85,   345,   200,   111,   348,    93,
      90,    12,    89,   243,    38,    97,   221,    12,   279,    34,
      12,   101,    50,    51,    97,   401,   402,    67,    12,    97,
      97,   111,    60,    67,   138,    50,    51,   121,     7,     8,
       9,   145,   142,   123,    16,    60,   130,    87,   148,     3,
     153,    91,    92,   153,   142,   285,    94,    97,   138,    87,
     148,     3,    38,    97,    40,   145,   406,    30,   107,   108,
      32,   261,    87,    63,   112,   113,    12,   115,   116,   273,
      70,   119,    87,   344,   280,    68,    69,   111,    16,   193,
      12,    54,     0,    56,    57,   134,   135,   136,   137,   363,
     364,     3,   141,   142,   143,   144,    55,    56,    57,    58,
      14,   150,    62,   193,   138,   218,    74,   217,   218,   199,
     200,   145,    85,    75,   204,    64,    65,    66,    67,   217,
      73,   327,    31,   237,   238,    61,    11,   221,    15,   243,
      89,    13,     3,    17,   240,   225,   226,   227,   228,   229,
      34,     3,   232,   233,   234,   235,   236,   237,   238,    29,
     240,    14,   242,   243,    59,   203,   250,   271,    16,   193,
       5,    65,   252,    60,    13,   213,     5,   215,    88,   259,
     260,   285,   378,     5,    12,   269,   224,    13,    12,    14,
     386,   271,    13,   273,   390,    13,    12,   277,    13,   279,
      13,    12,   282,   241,    13,   285,   286,    17,    16,    12,
     294,     3,    46,   237,   238,   295,     3,    88,    30,    31,
      32,    55,    56,    57,    58,   309,   318,   319,    17,    13,
     322,   323,    34,    88,   338,   274,   275,    34,   318,   319,
      34,     3,   322,   323,    13,    79,    13,   271,   328,    13,
     330,    37,   332,   333,   330,    89,    13,    29,   338,    31,
      32,    13,    13,    13,   344,     3,    13,   347,    17,    13,
     350,     3,    65,   365,   312,    60,     3,   315,     3,     5,
      60,    51,   320,   321,    13,   365,   419,   370,   326,   347,
     370,    37,   346,   295,   370,   107,   108,   377,   350,   391,
     225,   228,   377,   226,    37,   397,   227,    37,   346,   229,
     234,   391,    19,   232,    37,   419,   233,   397,    37,   235,
      37,   236,   414,   415,    96,    97,    98,    -1,    -1,    -1,
     142,   143,   144,    -1,   414,   415,   108,    -1,   150,   419,
      -1,    -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,   134,    -1,   136,   137,    27,    28,    -1,    30,
      -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    67,    68,    69,    -1,
      54,    55,    56,    57,    58,    -1,    77,    78,    79,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    87,    -1,    89,    90,
      91,    92,    -1,    -1,    95,    96,    97,    98,    99,     3,
      -1,    85,   214,    -1,     8,    89,    10,    -1,    12,    -1,
      14,    -1,    16,    17,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    27,    28,    54,    55,    56,    57,    58,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      79,    55,    56,    57,    58,    -1,    85,    -1,    -1,    -1,
      89,    -1,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,
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
      51,    52,    53,    -1,    55,    -1,    57,    58,    -1,    10,
      -1,    12,    -1,    14,    -1,    -1,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,     8,    -1,    10,    -1,    12,    87,    14,    -1,    90,
      91,    92,    -1,    -1,    95,    96,    97,    98,    99,    -1,
      27,    28,    -1,    30,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,    56,
      57,    58,     8,    -1,    10,    -1,    12,    -1,    14,    90,
      67,    68,    69,    -1,    95,    96,    97,    98,    99,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    85,    35,
      87,    -1,    89,    90,    91,    92,    -1,    -1,    95,    96,
      97,    98,    99,    -1,    50,    51,    52,    53,    -1,    55,
      56,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    12,    79,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    89,    90,    91,    92,    27,    28,    95,
      96,    97,    98,    99,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    51,    52,    53,    -1,    55,    56,    57,    58,     8,
      -1,    10,    -1,    12,    -1,    14,    -1,    67,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    87,    -1,    89,
      90,    91,    92,    -1,    -1,    95,    96,    97,    98,    99,
      -1,    50,    51,    52,    53,    -1,    55,    56,    57,    58,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    87,    -1,
      89,    90,    91,    92,    -1,    -1,    95,    96,    97,    98,
      99,    -1,    50,    51,    52,    53,    -1,    55,    -1,    57,
      58,     8,    -1,    10,    -1,    12,    -1,    14,    15,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    87,
      -1,    -1,    90,    91,    92,    -1,    -1,    95,    96,    97,
      98,    99,    -1,    50,    51,    52,    53,    -1,    55,    -1,
      57,    58,     8,    -1,    10,    -1,    12,    -1,    14,    15,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      87,    -1,    -1,    90,    91,    92,    -1,    -1,    95,    96,
      97,    98,    99,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    57,    58,     8,    -1,    10,    -1,    12,    13,    14,
      -1,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    87,    -1,    -1,    90,    91,    92,    -1,    -1,    95,
      96,    97,    98,    99,    -1,    50,    51,    52,    53,    -1,
      55,    -1,    57,    58,     8,    -1,    10,    -1,    12,    -1,
      14,    15,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    87,    -1,    -1,    90,    91,    92,    -1,    -1,
      95,    96,    97,    98,    99,    -1,    50,    51,    52,    53,
      -1,    55,    -1,    57,    58,     8,    -1,    10,    -1,    12,
      13,    14,    -1,    67,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    87,    -1,    -1,    90,    91,    92,    -1,
      -1,    95,    96,    97,    98,    99,    -1,    50,    51,    52,
      53,    -1,    55,    -1,    57,    58,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    67,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    87,    -1,    -1,    90,    91,    92,
      -1,    -1,    95,    96,    97,    98,    99,    -1,    50,    51,
      52,    53,    -1,    55,    -1,    57,    58,     8,    -1,    10,
      -1,    12,    -1,    14,    -1,    67,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    87,    -1,    -1,    90,    91,
      92,    -1,    -1,    95,    96,    97,    98,    99,    -1,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    12,    -1,    14,    -1,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,    -1,
      -1,    50,    51,    52,    53,    -1,    -1,     4,    -1,     6,
       7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,     4,     5,     6,    -1,    95,    96,    97,    98,
      99,    -1,    -1,    50,    51,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    63,    64,    65,    66,
      67,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    81,    82,    83,    84,     4,    -1,
       6,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    63,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,     4,    -1,     6,    -1,    80,    81,
      82,    83,    84,    13,    -1,    -1,    -1,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    81,    82,    83,    84
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
     142,    32,   107,   117,    55,    56,    57,    58,    79,   134,
     144,    55,    56,    57,    58,    79,   103,   119,   137,   144,
      56,   119,   134,   137,   138,   144,   119,   134,   144,   108,
     117,    12,     4,     6,     7,     8,     9,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    50,    51,    63,    64,
      65,    66,    67,    68,    69,    71,    72,    80,    81,    82,
      83,    84,   131,   139,   155,   157,   159,   161,   169,   113,
      87,   113,   167,    16,    12,   142,     0,   101,   109,    29,
      34,   141,     3,     5,   131,    14,   132,    56,    57,   103,
     138,   117,   145,   146,    33,    76,    62,    74,    75,    73,
      63,    70,   153,   155,   157,   159,   161,    31,    78,   165,
      61,    14,    60,    87,   130,   132,   169,   128,    11,    13,
      15,    17,   113,   128,   128,    38,    40,   128,   128,    93,
      94,   127,     3,   128,    34,    34,   107,     3,   132,   133,
     115,    59,   104,     5,    57,    58,   113,   113,    65,    60,
      13,   113,    12,   117,   107,     5,    88,   128,   143,   108,
     128,   134,    15,   132,   133,     5,   128,   149,   150,   151,
     152,   154,   156,   158,   160,   162,   163,   113,   113,   170,
     117,   167,    12,   168,    13,    14,   132,   117,    13,    13,
      12,    12,    13,    13,   117,   117,   127,    13,    17,   113,
      16,   107,    12,    12,   117,   141,   108,   115,    12,   135,
     117,     3,    88,   167,    67,   117,     3,    17,    13,   145,
      34,   135,    88,    13,   128,    15,   135,   116,   116,   128,
     128,   116,   116,    34,    34,    13,   128,   115,   117,   118,
     102,   105,   106,   117,   117,     3,   114,   142,    13,    13,
     108,   141,   143,   118,   141,   147,    13,    13,    13,    60,
     136,    37,    13,    13,   129,   129,   116,    13,     3,   106,
      17,    13,    13,   146,   115,   135,    65,     3,   115,    60,
     115,   116,     3,     3,    13,    13,   116,   135,   135,     5,
     136,   141,    60,   116,   116,   136,   136,   114,    13
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   100,   101,   101,   102,   102,   102,   103,   103,   103,
     104,   104,   105,   105,   106,   106,   107,   107,   108,   108,
     109,   109,   110,   110,   111,   111,   111,   112,   112,   113,
     113,   113,   114,   114,   115,   115,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   117,
     118,   118,   119,   119,   119,   119,   120,   121,   122,   122,
     123,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     125,   125,   126,   126,   126,   126,   127,   127,   128,   128,
     129,   129,   130,   130,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   132,   132,   132,   133,   133,   133,   134,   134,   134,
     134,   134,   135,   135,   136,   136,   137,   138,   138,   138,
     139,   139,   139,   139,   139,   139,   140,   140,   140,   140,
     141,   141,   142,   142,   142,   142,   142,   142,   143,   143,
     144,   144,   144,   144,   144,   145,   145,   146,   146,   146,
     147,   147,   148,   148,   149,   149,   150,   150,   151,   151,
     152,   152,   153,   153,   154,   154,   155,   155,   155,   155,
     156,   156,   157,   157,   158,   158,   159,   159,   160,   160,
     161,   161,   161,   162,   162,   163,   163,   164,   164,   164,
     164,   164,   164,   164,   165,   165,   165,   165,   166,   166,
     167,   167,   168,   168,   169,   169,   170,   170,   170,   170,
     170,   171,   171,   171,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     6,     2,     2,
       2,     0,     1,     0,     1,     2,     1,     3,     1,     3,
       1,     2,     1,     0,     7,     8,     8,     4,     5,     1,
       2,     2,     2,     4,     2,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     6,     2,     2,     2,     2,     3,     3,     3,
       5,     5,     7,     6,     7,     8,     8,     5,     7,     5,
       5,     7,     2,     3,     2,     2,     2,     1,     1,     3,
       1,     3,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     5,     2,     3,     2,     1,     2,     2,
       2,     2,     1,     2,     1,     4,     4,     7,     2,     2,
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
          case 111: /* func_ptr  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1784 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 112: /* stmt_type  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1790 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 115: /* code_segment  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1796 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 116: /* stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1802 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 119: /* enum_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1808 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 120: /* label_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1814 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 121: /* goto_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1820 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 122: /* case_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1826 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 123: /* switch_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1832 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 124: /* loop_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1838 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 125: /* selection_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1844 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 126: /* jump_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1850 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 127: /* exp_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1856 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 128: /* exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1862 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 129: /* binary_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1868 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 134: /* decl_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1874 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 144: /* union_stmt  */
#line 96 "utils/gwion.y" /* yacc.c:1258  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1880 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 147: /* con_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1886 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 148: /* log_or_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1892 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 149: /* log_and_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1898 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 150: /* inc_or_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1904 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 151: /* exc_or_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1910 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 152: /* and_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1916 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 154: /* eq_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1922 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 156: /* relational_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1928 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 158: /* shift_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1934 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 160: /* add_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1940 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 162: /* mul_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1946 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 163: /* cast_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1952 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 165: /* unary_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1958 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 166: /* dur_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1964 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 168: /* call_paren  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1970 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 170: /* post_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1976 "src/ast/parser.c" /* yacc.c:1258  */
        break;

    case 172: /* primary_exp  */
#line 97 "utils/gwion.y" /* yacc.c:1258  */
      { free_exp(((*yyvaluep).exp)); }
#line 1982 "src/ast/parser.c" /* yacc.c:1258  */
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
#line 2250 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 3:
#line 103 "utils/gwion.y" /* yacc.c:1648  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast)); }
#line 2256 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 4:
#line 107 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list)); }
#line 2262 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 5:
#line 108 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def)); }
#line 2268 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 6:
#line 109 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def)); }
#line 2274 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 7:
#line 114 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_def) = new_class_def(0, (yyvsp[-4].id_list), (yyvsp[-3].type_decl), (yyvsp[-1].class_body)); }
#line 2280 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 8:
#line 115 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_global); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2286 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 9:
#line 117 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].class_def), free_class_def); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2292 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 10:
#line 119 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2298 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 11:
#line 119 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = NULL; }
#line 2304 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 13:
#line 121 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = NULL; }
#line 2310 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 14:
#line 124 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = new_class_body((yyvsp[0].section), NULL); }
#line 2316 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 15:
#line 125 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.class_body) = new_class_body((yyvsp[-1].section), (yyvsp[0].class_body)); }
#line 2322 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 16:
#line 129 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2328 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 17:
#line 130 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2334 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 18:
#line 134 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2340 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 19:
#line 135 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2346 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 20:
#line 139 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL);}
#line 2352 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 21:
#line 140 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list));}
#line 2358 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 22:
#line 144 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_flag_static;   }
#line 2364 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 23:
#line 145 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 2370 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 24:
#line 150 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_fptr((yyvsp[-3].sym), (yyvsp[-5].type_decl), (yyvsp[-1].arg_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2376 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 25:
#line 152 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_fptr((yyvsp[-3].sym), (yyvsp[-5].type_decl), (yyvsp[-1].arg_list), (yyvsp[0].ival) | ae_flag_static, get_pos(arg)); }
#line 2382 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 26:
#line 154 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_fptr((yyvsp[-3].sym), (yyvsp[-5].type_decl), (yyvsp[-1].arg_list), (yyvsp[0].ival) | ae_flag_global, get_pos(arg)); }
#line 2388 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 27:
#line 159 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_type((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2394 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 28:
#line 161 "utils/gwion.y" /* yacc.c:1648  */
    { SET_FLAG((yyvsp[-2].type_decl), ae_flag_global); (yyval.stmt) = new_stmt_type((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2400 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 30:
#line 165 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub)); }
#line 2406 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 31:
#line 166 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub)); }
#line 2412 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 32:
#line 170 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL); }
#line 2418 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 33:
#line 171 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list)); }
#line 2424 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 34:
#line 175 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_code(NULL, get_pos(arg)); }
#line 2430 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 35:
#line 176 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_code((yyvsp[-1].stmt_list), get_pos(arg)); }
#line 2436 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 49:
#line 195 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.sym) = insert_symbol((yyvsp[0].sval)); }
#line 2442 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 50:
#line 197 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.sym) = NULL; }
#line 2448 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 52:
#line 200 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sym), get_pos(arg)); }
#line 2454 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 53:
#line 201 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2460 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 54:
#line 202 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2466 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 55:
#line 203 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_global);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 2472 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 56:
#line 207 "utils/gwion.y" /* yacc.c:1648  */
    {  (yyval.stmt) = new_stmt_jump((yyvsp[-1].sym), 1, get_pos(arg)); }
#line 2478 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 57:
#line 211 "utils/gwion.y" /* yacc.c:1648  */
    {  (yyval.stmt) = new_stmt_jump((yyvsp[-1].sym), 0, get_pos(arg)); }
#line 2484 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 58:
#line 215 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_case, (yyvsp[-1].exp), get_pos(arg)); }
#line 2490 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 59:
#line 216 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_case, (yyvsp[-1].exp), get_pos(arg)); }
#line 2496 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 60:
#line 220 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg));}
#line 2502 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 61:
#line 225 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_while, (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2508 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 62:
#line 227 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_while, (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2514 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 63:
#line 229 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2520 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 64:
#line 231 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2526 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 65:
#line 233 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2532 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 66:
#line 235 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_auto((yyvsp[-4].sym), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); (yyval.stmt)->d.stmt_auto.is_ptr = 1;}
#line 2538 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 67:
#line 237 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_until, (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2544 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 68:
#line 239 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_flow(ae_stmt_until, (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2550 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 69:
#line 241 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_loop((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2556 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 70:
#line 246 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(arg)); }
#line 2562 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 71:
#line 248 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(arg)); }
#line 2568 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 72:
#line 252 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_return, NULL, get_pos(arg)); }
#line 2574 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 73:
#line 253 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_return, (yyvsp[-1].exp), get_pos(arg)); }
#line 2580 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 74:
#line 254 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt(ae_stmt_break, get_pos(arg)); }
#line 2586 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 75:
#line 255 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt(ae_stmt_continue, get_pos(arg)); }
#line 2592 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 76:
#line 259 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_exp, (yyvsp[-1].exp),   get_pos(arg)); }
#line 2598 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 77:
#line 260 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_exp(ae_stmt_exp, NULL, get_pos(arg)); }
#line 2604 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 79:
#line 265 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = prepend_exp((yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 2610 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 81:
#line 270 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2616 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 82:
#line 273 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = NULL; }
#line 2622 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 83:
#line 273 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2628 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 84:
#line 275 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_chuck; }
#line 2634 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 85:
#line 275 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_unchuck; }
#line 2640 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 86:
#line 275 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_eq; }
#line 2646 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 87:
#line 276 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_at_chuck; }
#line 2652 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 88:
#line 276 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plus_chuck; }
#line 2658 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 89:
#line 277 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minus_chuck; }
#line 2664 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 90:
#line 277 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_times_chuck; }
#line 2670 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 91:
#line 278 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_divide_chuck; }
#line 2676 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 92:
#line 278 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2682 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 93:
#line 279 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_trig; }
#line 2688 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 94:
#line 279 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_untrig; }
#line 2694 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 95:
#line 280 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsl; }
#line 2700 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 96:
#line 280 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsr; }
#line 2706 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 97:
#line 280 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsand; }
#line 2712 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 98:
#line 281 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsor; }
#line 2718 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 99:
#line 281 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_rsxor; }
#line 2724 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 100:
#line 282 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_assign; }
#line 2730 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 101:
#line 286 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = new_array_sub((yyvsp[-1].exp), get_pos(arg)); }
#line 2736 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 102:
#line 287 "utils/gwion.y" /* yacc.c:1648  */
    { if((yyvsp[-2].exp)->next){ gwion_error(arg, "invalid format for array init [...][...]..."); free_exp((yyvsp[-2].exp)); free_array_sub((yyvsp[0].array_sub)); YYERROR; } (yyval.array_sub) = prepend_array_sub((yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2742 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 103:
#line 288 "utils/gwion.y" /* yacc.c:1648  */
    { gwion_error(arg, "partially empty array init [...][]..."); free_exp((yyvsp[-3].exp)); YYERROR; }
#line 2748 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 104:
#line 292 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(arg)); }
#line 2754 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 105:
#line 293 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2760 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 106:
#line 294 "utils/gwion.y" /* yacc.c:1648  */
    { gwion_error(arg, "partially empty array init [][...]"); free_array_sub((yyvsp[-1].array_sub)); free_array_sub((yyvsp[0].array_sub)); YYERROR; }
#line 2766 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 108:
#line 299 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2772 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 109:
#line 301 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_static); (yyval.exp) = (yyvsp[0].exp); }
#line 2778 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 110:
#line 303 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_private); (yyval.exp) = (yyvsp[0].exp); }
#line 2784 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 111:
#line 305 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.td, ae_flag_protect); (yyval.exp) = (yyvsp[0].exp); }
#line 2790 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 112:
#line 309 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = NULL; }
#line 2796 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 113:
#line 310 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.arg_list) = (yyvsp[0].arg_list); }
#line 2802 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 114:
#line 314 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 2808 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 115:
#line 315 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2814 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 116:
#line 318 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2820 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 117:
#line 322 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-4].type_decl), (yyvsp[-3].sym), (yyvsp[-2].arg_list), (yyvsp[0].stmt), (yyvsp[-5].ival) | (yyvsp[-1].ival)); }
#line 2826 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 118:
#line 324 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_private); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2832 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 119:
#line 326 "utils/gwion.y" /* yacc.c:1648  */
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
#line 2848 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 127:
#line 342 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-3].type_decl), OP_SYM((yyvsp[-4].ival)), (yyvsp[-2].arg_list), (yyvsp[0].stmt), ae_flag_op); }
#line 2854 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 128:
#line 344 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def((yyvsp[-3].type_decl), OP_SYM((yyvsp[-5].ival)), (yyvsp[-2].arg_list), (yyvsp[0].stmt), ae_flag_op | ae_flag_unary); }
#line 2860 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 129:
#line 346 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.func_def) = new_func_def(new_type_decl(new_id_list(insert_symbol("void"), get_pos(arg)), 0,
      get_pos(arg)), insert_symbol("dtor"), NULL, (yyvsp[0].stmt), ae_flag_dtor); }
#line 2867 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 130:
#line 350 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = 0; }
#line 2873 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 131:
#line 350 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_flag_ref; }
#line 2879 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 132:
#line 353 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)), (yyvsp[0].ival), get_pos(arg)); }
#line 2885 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 133:
#line 354 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2891 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 134:
#line 355 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)),
      (yyvsp[0].ival), get_pos(arg)); (yyval.type_decl)->types = (yyvsp[-3].type_list); }
#line 2898 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 135:
#line 357 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg));
      (yyval.type_decl)->types = (yyvsp[-5].type_list); }
#line 2905 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 136:
#line 359 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2911 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 137:
#line 360 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (yyvsp[0].type_decl), ae_flag_const); (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2917 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 138:
#line 364 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2923 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 139:
#line 365 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2929 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 140:
#line 369 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(arg));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sym); }
#line 2935 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 141:
#line 371 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2941 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 142:
#line 373 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2947 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 143:
#line 375 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_protect); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2953 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 144:
#line 377 "utils/gwion.y" /* yacc.c:1648  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_global);  (yyval.stmt) = (yyvsp[0].stmt); }
#line 2959 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 145:
#line 381 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL); }
#line 2965 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 146:
#line 382 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list)); }
#line 2971 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 147:
#line 386 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sym), NULL, get_pos(arg)); }
#line 2977 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 148:
#line 387 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2983 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 149:
#line 388 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2989 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 151:
#line 392 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_if((yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 2995 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 153:
#line 395 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(arg)); }
#line 3001 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 155:
#line 398 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(arg)); }
#line 3007 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 157:
#line 401 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(arg)); }
#line 3013 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 159:
#line 404 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(arg)); }
#line 3019 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 161:
#line 407 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(arg)); }
#line 3025 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 162:
#line 409 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_eq; }
#line 3031 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 163:
#line 409 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_neq; }
#line 3037 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 165:
#line 412 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3043 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 166:
#line 414 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_lt; }
#line 3049 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 167:
#line 414 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_gt; }
#line 3055 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 168:
#line 415 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_le; }
#line 3061 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 169:
#line 415 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_ge; }
#line 3067 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 171:
#line 418 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3073 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 172:
#line 421 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_shift_left;  }
#line 3079 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 173:
#line 422 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_shift_right; }
#line 3085 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 175:
#line 426 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3091 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 176:
#line 428 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plus; }
#line 3097 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 177:
#line 428 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minus; }
#line 3103 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 179:
#line 431 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3109 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 180:
#line 433 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_times; }
#line 3115 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 181:
#line 433 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_divide; }
#line 3121 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 182:
#line 433 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_percent; }
#line 3127 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 184:
#line 436 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3133 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 186:
#line 439 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_cast((yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(arg)); }
#line 3139 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 187:
#line 441 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plus; }
#line 3145 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 188:
#line 441 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minus; }
#line 3151 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 189:
#line 441 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_times; }
#line 3157 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 190:
#line 442 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plusplus; }
#line 3163 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 191:
#line 442 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minusminus; }
#line 3169 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 192:
#line 443 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_exclamation; }
#line 3175 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 193:
#line 443 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_spork; }
#line 3181 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 195:
#line 447 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_unary((yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 3187 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 196:
#line 449 "utils/gwion.y" /* yacc.c:1648  */
    {
      if((yyvsp[0].type_decl)->array && !(yyvsp[0].type_decl)->array->exp) {
        free_type_decl((yyvsp[0].type_decl));
        gwion_error(arg, "can't use empty '[]' in 'new' expression");
        YYERROR;
      }
      (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), get_pos(arg));
    }
#line 3200 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 197:
#line 458 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_unary3(op_spork, (yyvsp[0].stmt), get_pos(arg)); }
#line 3206 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 199:
#line 461 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_dur((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3212 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 200:
#line 464 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = new_type_list((yyvsp[0].type_decl), NULL); }
#line 3218 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 201:
#line 465 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.type_list) = new_type_list((yyvsp[-2].type_decl), (yyvsp[0].type_list)); }
#line 3224 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 202:
#line 468 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = NULL; }
#line 3230 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 203:
#line 468 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3236 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 204:
#line 470 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_plusplus; }
#line 3242 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 205:
#line 470 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = op_minusminus; }
#line 3248 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 207:
#line 473 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_array((yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(arg)); }
#line 3254 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 208:
#line 475 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_call((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg));
      if((yyvsp[-1].type_list))(yyval.exp)->d.exp_func.tmpl = new_tmpl_call((yyvsp[-1].type_list)); }
#line 3261 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 209:
#line 478 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_dot((yyvsp[-2].exp), (yyvsp[0].sym), get_pos(arg)); }
#line 3267 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 210:
#line 480 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_post((yyvsp[-1].exp), (yyvsp[0].ival), get_pos(arg)); }
#line 3273 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 211:
#line 483 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_complex; }
#line 3279 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 212:
#line 484 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_polar;   }
#line 3285 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 213:
#line 485 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.ival) = ae_primary_vec;     }
#line 3291 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 214:
#line 488 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_id(     (yyvsp[0].sym), get_pos(arg)); }
#line 3297 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 215:
#line 489 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_int(    (yyvsp[0].ival), get_pos(arg)); }
#line 3303 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 216:
#line 490 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_float(  (yyvsp[0].fval), get_pos(arg)); }
#line 3309 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 217:
#line 491 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_string( (yyvsp[0].sval), get_pos(arg)); }
#line 3315 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 218:
#line 492 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_char(   (yyvsp[0].sval), get_pos(arg)); }
#line 3321 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 219:
#line 493 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3327 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 220:
#line 494 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3333 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 221:
#line 495 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_vec((yyvsp[-2].ival), (yyvsp[-1].exp), get_pos(arg)); }
#line 3339 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 222:
#line 496 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_hack(   (yyvsp[-1].exp), get_pos(arg)); }
#line 3345 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 223:
#line 497 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) =                      (yyvsp[-1].exp);                }
#line 3351 "src/ast/parser.c" /* yacc.c:1648  */
    break;

  case 224:
#line 498 "utils/gwion.y" /* yacc.c:1648  */
    { (yyval.exp) = new_exp_prim_nil(        get_pos(arg)); }
#line 3357 "src/ast/parser.c" /* yacc.c:1648  */
    break;


#line 3361 "src/ast/parser.c" /* yacc.c:1648  */
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
#line 500 "utils/gwion.y" /* yacc.c:1907  */

