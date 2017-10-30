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
#line 7 "utils/gwion.y" /* yacc.c:339  */

#define YYERROR_VERBOSE
#include "absyn.h"
#include "scanner.h"
#define scanner arg->scanner


char *strcat(char *dest, const char *src);
void gwion_error(void* data, const char* s);
int gwion_lex(void*, void* , void*);
static int get_pos(void* data)
{
  Scanner* scan = (Scanner*)map_get(scan_map, (vtype)data);
  return scan->line;
}


#line 90 "src/ast/parser.c" /* yacc.c:339  */

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
    TYPEOF = 353,
    CONST = 354,
    NUM = 355,
    FLOAT = 356,
    ID = 357,
    STRING_LIT = 358,
    CHAR_LIT = 359,
    NEG = 360
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
#define TYPEOF 353
#define CONST 354
#define NUM 355
#define FLOAT 356
#define ID 357
#define STRING_LIT 358
#define CHAR_LIT 359
#define NEG 360

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 25 "utils/gwion.y" /* yacc.c:355  */

  char* sval;
  int ival;
  m_float fval;
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
  Decl_List decl_list; // for union
  Func_Def func_def;
  Section* section;
  ID_List id_list;
  Type_List type_list; // call template
  Class_Body class_body;
  ID_List class_ext;
  Class_Def class_def;
  Ast ast;

#line 367 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (Scanner* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 383 "src/ast/parser.c" /* yacc.c:358  */

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
#define YYFINAL  141
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1378

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  106
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  73
/* YYNRULES -- Number of rules.  */
#define YYNRULES  215
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  383

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
       0,   152,   152,   153,   157,   158,   159,   163,   167,   167,
     170,   171,   175,   176,   180,   181,   185,   186,   187,   191,
     192,   196,   197,   201,   202,   206,   207,   211,   212,   216,
     217,   221,   222,   223,   227,   228,   232,   233,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     252,   253,   257,   261,   265,   266,   270,   274,   276,   278,
     280,   282,   284,   286,   291,   293,   298,   299,   300,   301,
     305,   306,   310,   311,   315,   316,   320,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   352,   353,   354,   358,   359,   360,   364,   365,
     366,   367,   368,   372,   373,   376,   376,   379,   381,   383,
     388,   388,   391,   392,   393,   394,   395,   396,   401,   402,
     406,   410,   411,   415,   416,   417,   420,   421,   422,   425,
     426,   431,   432,   437,   438,   443,   444,   449,   450,   455,
     456,   460,   460,   462,   463,   467,   467,   467,   467,   470,
     471,   476,   477,   481,   482,   486,   486,   489,   490,   494,
     494,   494,   497,   498,   503,   504,   508,   508,   508,   509,
     509,   510,   510,   514,   515,   517,   519,   521,   527,   528,
     532,   533,   536,   536,   537,   537,   541,   541,   544,   545,
     547,   549,   551,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568
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
  "UNION", "ATPAREN", "TYPEOF", "CONST", "NUM", "FLOAT", "ID",
  "STRING_LIT", "CHAR_LIT", "NEG", "$accept", "ast", "section",
  "class_def", "class_ext", "class_decl", "class_body", "class_body2",
  "class_section", "id_list", "id_dot", "stmt_list", "static_decl",
  "function_decl", "func_ptr", "type_decl2", "arg_list", "code_segment",
  "stmt", "enum_stmt", "label_stmt", "goto_stmt", "case_stmt",
  "switch_stmt", "loop_stmt", "selection_stmt", "jump_stmt", "exp_stmt",
  "exp", "binary_exp", "template", "op", "array_exp", "array_empty",
  "decl_exp", "func_args", "decl_template", "func_def", "atsym",
  "type_decl", "decl_list", "union_stmt", "var_decl_list", "var_decl",
  "complex_exp", "polar_exp", "vec_exp", "conditional_expression",
  "logical_or_expression", "logical_and_expression",
  "inclusive_or_expression", "exclusive_or_expression", "and_expression",
  "equality_op", "equality_expression", "relationnal_op",
  "relational_expression", "shift_op", "shift_expression", "additive_op",
  "additive_expression", "multiplicative_op", "multiplicative_expression",
  "cast_exp", "unary_op", "unary_expression", "dur_exp", "type_list",
  "call_template", "call_paren", "postfix_op", "postfix_exp",
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

#define YYTABLE_NINF -193

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     578,  -331,  -331,  1214,   854,   914,   656,  1214,  1214,  -331,
      44,    46,   734,    52,    53,    55,   -31,    62,    45,    70,
     207,    88,    89,  -331,  -331,    33,    63,    25,    -8,  -331,
    -331,    91,    33,    33,    14,     4,    84,  1214,    98,     3,
    -331,  -331,    -5,  -331,  -331,   114,   344,  -331,  -331,  -331,
    -331,   734,  -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,
    -331,   123,  1271,  -331,   116,  -331,    -2,  -331,    31,  -331,
    -331,  -331,  -331,  -331,     1,    74,    72,    77,    81,   -19,
     -25,     8,    22,   129,   124,  1274,  -331,    95,    11,  -331,
     -17,    -3,   145,  -331,   144,  -331,   143,  -331,   147,   148,
     152,  1214,  1214,    13,  1214,  1214,   794,   157,  1214,  -331,
      29,   132,    65,  -331,   165,  -331,  -331,   140,   156,   155,
      33,     4,    31,   115,   109,   162,    75,   164,   166,    65,
     176,    90,  1214,   170,    -8,    -8,   173,   140,  -331,  -331,
    -331,  -331,  -331,  -331,  -331,  -331,  1214,  -331,  -331,  -331,
    -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,
    -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,  -331,
    -331,  1214,   974,  -331,  -331,  -331,  -331,   134,   131,   166,
    -331,   185,  1214,  1274,  1274,  1274,  1274,  1274,  -331,  -331,
    1274,  -331,  -331,  -331,  -331,  1274,  -331,  -331,  1274,  -331,
    -331,  1274,  -331,  -331,  -331,  1274,    33,  -331,    45,  1214,
    -331,  -331,    93,     4,  -331,  -331,   180,  -331,  -331,  -331,
     179,  -331,  -331,  -331,   187,   188,   186,   190,   191,   192,
     794,  -331,   193,  -331,  -331,   198,   194,  -331,  -331,  -331,
     195,   118,  -331,    -8,   140,   155,   196,   111,  -331,   116,
     120,     4,    33,   215,   203,  -331,   209,   161,    -8,  -331,
    -331,  -331,  -331,    65,  -331,    33,  -331,   116,    31,   199,
      74,    72,    77,    81,   -19,   -25,     8,    22,   129,   124,
    -331,    11,  -331,   133,  1034,  -331,  1094,  -331,   734,   734,
    1214,  1214,   734,   734,  1154,   155,    65,    17,   128,    33,
    -331,  -331,  -331,    18,   155,   212,  -331,  -331,    31,  1214,
     229,   140,   140,   223,   181,   135,  -331,  1274,  -331,  -331,
     226,  -331,   206,  -331,   227,   231,  -331,  -331,   734,   232,
    -331,  -331,  -331,   243,   234,    31,  -331,   235,    31,  -331,
     196,  -331,  -331,  -331,  -331,  -331,   140,    -8,   236,   196,
    -331,  -331,   734,   247,   248,  -331,   734,  -331,   196,  -331,
    -331,   249,  -331,  -331,  -331,   422,   155,  -331,  -331,  -331,
    -331,  -331,    33,  -331,   238,  -331,   500,  -331,  -331,  -331,
    -331,  -331,  -331
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     115,    71,   178,     0,     0,     0,     0,     0,     0,   181,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   179,   180,     0,     0,     0,     0,   177,
     176,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     204,   205,   203,   206,   207,     0,   115,     6,     4,    48,
      41,    23,    46,    42,    43,    45,    44,    39,    40,    47,
      38,     0,    72,   208,   209,    74,    11,     5,     0,    49,
     210,   211,   212,   108,   139,   141,   143,   145,   147,   149,
     153,   159,   163,   167,   172,     0,   174,   183,   188,   198,
       0,   203,     0,   215,     0,   105,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   203,
     192,   198,     0,    66,     0,    68,    69,   120,   185,   182,
       0,     0,     0,    21,     0,     0,     0,     0,    31,     0,
     190,     0,     0,     0,     0,     0,     0,   120,   121,    52,
     122,     1,     3,    24,    70,    77,     0,   101,    81,    82,
      83,    84,    85,    80,    78,    86,    87,    79,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,     0,     0,   107,    27,    10,    28,     0,    26,   133,
     109,   131,     0,     0,     0,     0,     0,     0,   151,   152,
       0,   158,   156,   157,   155,     0,   161,   162,     0,   166,
     165,     0,   170,   169,   171,     0,     0,   184,     0,     0,
     196,   197,     0,     0,   193,   199,     0,   202,   213,   214,
     102,    37,   137,   136,     0,     0,     0,     0,     0,     0,
       0,    53,     0,    55,    54,    19,     0,    67,   186,   187,
       0,     0,   111,     0,   120,     0,     0,     0,    33,    32,
       0,     0,     0,     0,     0,   138,     0,     0,     0,   123,
      73,    75,   106,     0,    25,     0,   134,   135,     0,     0,
     142,   144,   146,   148,   150,   154,   160,   164,   168,   173,
     175,   189,   201,     0,     0,   200,     0,   103,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,   124,   119,     0,     0,     0,   116,   191,     0,   128,
       0,   120,   120,     0,     9,     0,   132,     0,    76,   194,
       0,   104,    64,    57,     0,     0,    61,    63,     0,     0,
      56,    20,    50,     0,     0,     0,   113,     0,     0,   118,
       0,   110,   129,   130,   126,   125,   120,     0,     0,     0,
     140,   195,     0,     0,     0,    59,     0,    51,     0,   112,
     114,    34,    29,   127,     8,   115,     0,    65,    58,    62,
      60,    30,     0,    18,     0,    12,   115,    16,    17,   117,
      35,     7,    15
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -331,   210,  -331,  -330,  -331,  -331,  -331,  -123,  -331,  -122,
    -118,    -6,  -331,  -331,  -331,  -102,  -111,  -117,    -7,  -331,
    -331,  -331,  -331,  -331,  -331,  -331,  -331,   -87,     7,  -331,
    -331,  -331,   -58,   -95,    92,  -308,  -331,  -329,  -115,   -24,
     -45,  -331,  -109,   -73,  -331,  -331,  -331,   -51,  -331,    85,
      83,    86,    94,  -331,    82,  -331,    80,  -331,    97,  -331,
      78,  -331,    76,    73,  -331,   202,  -331,  -100,  -331,  -331,
    -331,   -14,   265
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    45,    46,    47,   348,   177,   374,   375,   376,   236,
     124,    48,   265,   178,    49,   127,   337,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
     214,   171,    63,    64,    65,   304,    66,    67,   140,    68,
     254,    69,   180,   181,    70,    71,    72,    73,    74,    75,
      76,    77,    78,   190,    79,   195,    80,   198,    81,   201,
      82,   205,    83,    84,    85,    86,    87,   131,   216,   285,
     217,    88,    89
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      98,   118,   239,   122,   110,   103,   173,   250,   126,   128,
      92,    94,    96,   242,    99,   100,   256,   257,   240,   230,
     332,   241,   259,  -192,   138,   209,   138,   114,   174,   139,
     215,   336,   362,   249,   182,   373,   378,   191,   192,   193,
     194,   366,   188,   209,   133,   143,   373,   378,    28,   189,
     371,   226,   215,   227,   175,     3,   101,     4,   102,     5,
     238,   210,   211,   233,   104,   105,   122,   106,   135,   212,
     248,   107,     7,     8,   108,   183,   121,   196,   197,   210,
     211,    38,    39,    28,   267,   117,   112,   212,   199,   200,
      28,   115,   116,   176,   123,   119,   128,  -120,    28,  -120,
     132,   136,   120,   125,   213,   137,   130,   129,   224,   225,
     134,   228,   229,   283,   141,   232,    38,    39,   121,   333,
     117,   266,   213,    38,    39,   300,   144,   117,   302,   301,
     172,    38,    39,   179,   184,   117,   202,   203,   204,   253,
     313,   314,    37,   294,   185,    40,    41,   109,    43,    44,
     186,   307,   187,   260,   208,   206,   218,   219,   220,   316,
     231,   222,   287,   315,   221,   223,   234,   235,   237,   138,
     209,     6,   244,   243,   331,   245,   247,   246,   330,    96,
       5,   251,   280,   255,   252,   258,   264,   339,   263,   269,
     268,   173,   284,   286,   281,   282,   344,   345,   290,   341,
     288,   289,   291,   296,   292,   293,   295,   298,   303,   173,
     113,   297,   299,   305,   306,     2,    96,     3,   309,     4,
     310,     5,   311,   215,   312,   340,   359,   318,   308,   364,
     334,   363,   343,   317,     7,     8,   346,   349,   347,   351,
     353,   128,     9,   352,   354,   356,   357,   358,   360,   379,
     368,   369,   365,   382,   372,   381,   142,    23,    24,    25,
      26,   380,    90,   261,   342,   361,   350,   271,   270,   274,
     275,   272,    28,    29,    30,   335,   277,   278,   279,   338,
     273,   322,   323,   111,     0,   326,   327,   207,     0,     0,
       0,   320,   276,    96,     0,     0,     0,   324,   325,     0,
      35,   329,     0,     0,    37,    38,    39,    40,    41,    91,
      43,    44,     0,     0,     0,     0,   253,     0,     0,     0,
       0,   355,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    -2,   367,     0,     1,   338,   370,
       0,     0,     2,     0,     3,     0,     4,     0,     5,   377,
       6,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     377,     7,     8,     0,     0,     0,     0,     0,     0,     9,
      10,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      29,    30,     0,     0,     0,     0,     0,     0,     0,    31,
      32,    33,     0,     0,     0,     1,     0,     0,     0,     0,
       2,     0,     3,     0,     4,    34,     5,    35,     6,   -13,
      36,    37,    38,    39,    40,    41,    42,    43,    44,     7,
       8,     0,     0,     0,     0,     0,     0,     9,    10,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,    29,    30,
       0,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     1,     0,     0,     0,     0,     2,     0,
       3,     0,     4,    34,     5,    35,     6,   -14,    36,    37,
      38,    39,    40,    41,    42,    43,    44,     7,     8,     0,
       0,     0,     0,     0,     0,     9,    10,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    28,    29,    30,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    33,     0,     0,
       0,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,    34,     5,    35,     6,     0,    36,    37,    38,    39,
      40,    41,    42,    43,    44,     7,     8,     0,     0,     0,
       0,     0,     0,     9,    10,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    28,    29,    30,     0,     0,     0,     0,
       0,     0,     0,    31,    32,    33,     0,     0,     0,     1,
       0,     0,     0,     0,     2,     0,     3,     0,     4,    34,
       5,    35,     6,    97,    36,    37,    38,    39,    40,    41,
      42,    43,    44,     7,     8,     0,     0,     0,     0,     0,
       0,     9,    10,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    29,    30,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    33,     0,     0,     0,     1,     0,     0,
       0,     0,     2,     0,     3,     0,     4,     0,     5,    35,
       6,     0,    36,    37,    38,    39,    40,    41,    42,    43,
      44,     7,     8,     0,     0,     0,     0,     0,     0,     9,
      10,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
       0,     0,     0,     0,     0,     0,     0,     1,     0,    28,
      29,    30,     2,     0,     3,     0,     4,     0,     5,     0,
       0,    33,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,    35,     0,     9,
      36,    37,    38,    39,    40,    41,    42,    43,    44,     0,
       0,     0,     0,     0,    23,    24,    25,    26,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      29,    30,     2,     0,     3,     0,     4,    93,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,    35,     0,     9,
       0,    37,    38,    39,    40,    41,    91,    43,    44,     0,
       0,     0,     0,     0,    23,    24,    25,    26,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      29,    30,     2,     0,     3,     0,     4,     0,     5,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,    35,     0,     9,
       0,    37,    38,    39,    40,    41,    91,    43,    44,     0,
       0,     0,     0,     0,    23,    24,    25,    26,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      29,    30,     2,     0,     3,     0,     4,     0,     5,   262,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,    35,     0,     9,
       0,    37,    38,    39,    40,    41,    91,    43,    44,     0,
       0,     0,     0,     0,    23,    24,    25,    26,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      29,    30,     2,     0,     3,     0,     4,   319,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,    35,     0,     9,
       0,    37,    38,    39,    40,    41,    91,    43,    44,     0,
       0,     0,     0,     0,    23,    24,    25,    26,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      29,    30,     2,     0,     3,     0,     4,     0,     5,   321,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,    35,     0,     9,
       0,    37,    38,    39,    40,    41,    91,    43,    44,     0,
       0,     0,     0,     0,    23,    24,    25,    26,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      29,    30,     2,     0,     3,     0,     4,   328,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,    35,     0,     9,
       0,    37,    38,    39,    40,    41,    91,    43,    44,     0,
       0,     0,     0,     0,    23,    24,    25,    26,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      29,    30,     2,     0,     3,     0,     4,     0,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,    35,     0,     9,
       0,    37,    38,    39,    40,    41,    91,    43,    44,     0,
       0,     0,     0,     0,    23,    24,    25,    26,     0,    90,
       0,     0,     0,     0,     0,   145,   146,   147,     0,    28,
      29,    30,     2,     0,     3,     0,     4,     0,     5,   148,
     149,   150,   151,   152,   153,   154,   155,   156,     0,     0,
       0,     7,     8,     0,     0,     0,     0,    35,     0,     9,
       0,    37,    38,    39,    40,    41,    91,    43,    44,     0,
       0,     0,     0,     0,    23,    24,    25,    26,     0,     0,
       0,     0,   157,     0,     0,     0,     0,     0,     0,     0,
      29,    30,     0,     0,     0,     0,     0,     0,     0,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,     0,    40,    41,   109,    43,    44
};

static const yytype_int16 yycheck[] =
{
       6,    25,   119,    27,    18,    12,    64,   129,    32,    33,
       3,     4,     5,   122,     7,     8,   134,   135,   120,   106,
       3,   121,   137,    12,    29,    14,    29,    20,    30,    34,
      88,    13,   340,   128,    33,   365,   365,    62,    63,    64,
      65,   349,    61,    14,    37,    51,   376,   376,    65,    68,
     358,    38,   110,    40,    56,    10,    12,    12,    12,    14,
     118,    50,    51,    34,    12,    12,    90,    12,    65,    58,
     128,   102,    27,    28,    12,    74,    93,    69,    70,    50,
      51,    98,    99,    65,   179,   102,    16,    58,    66,    67,
      65,     3,     3,    95,   102,    32,   120,   102,    65,   102,
      16,    98,    77,    12,    93,   102,   102,    93,   101,   102,
      12,   104,   105,   213,     0,   108,    98,    99,    93,   102,
     102,   179,    93,    98,    99,   243,     3,   102,   245,   244,
      14,    98,    99,   102,    60,   102,     7,     8,     9,   132,
     258,   263,    97,   230,    72,   100,   101,   102,   103,   104,
      73,   251,    71,   146,    59,    31,    11,    13,    15,   268,
       3,    13,   220,   265,    17,    13,    34,   102,     3,    29,
      14,    16,    63,    58,   296,    13,    12,   102,   295,   172,
      14,     5,   206,    13,    94,    12,    55,   304,    54,   182,
       5,   249,    12,    14,   208,   102,   311,   312,    12,   308,
      13,    13,    12,     5,    13,    13,    13,    12,    12,   267,
       3,    17,    94,   102,    94,     8,   209,    10,     3,    12,
      17,    14,    13,   281,    63,    13,   335,    94,   252,   347,
     102,   346,     3,    34,    27,    28,    13,   102,    57,    13,
      13,   265,    35,    37,    13,    13,     3,    13,    13,   366,
       3,     3,    16,   376,     5,    17,    46,    50,    51,    52,
      53,   372,    55,   171,   309,   338,   317,   184,   183,   187,
     190,   185,    65,    66,    67,   299,   198,   201,   205,   303,
     186,   288,   289,    18,    -1,   292,   293,    85,    -1,    -1,
      -1,   284,   195,   286,    -1,    -1,    -1,   290,   291,    -1,
      93,   294,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,   309,    -1,    -1,    -1,
      -1,   328,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     0,   352,    -1,     3,   372,   356,
      -1,    -1,     8,    -1,    10,    -1,    12,    -1,    14,   365,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     376,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    91,    14,    93,    16,    17,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    12,    91,    14,    93,    16,    17,    96,    97,
      98,    99,   100,   101,   102,   103,   104,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    91,    14,    93,    16,    -1,    96,    97,    98,    99,
     100,   101,   102,   103,   104,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    12,    91,
      14,    93,    16,    17,    96,    97,    98,    99,   100,   101,
     102,   103,   104,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    12,    -1,    14,    93,
      16,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
     104,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    65,
      66,    67,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    93,    -1,    35,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,    67,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    93,    -1,    35,
      -1,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,    67,     8,    -1,    10,    -1,    12,    -1,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    93,    -1,    35,
      -1,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,    67,     8,    -1,    10,    -1,    12,    -1,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    93,    -1,    35,
      -1,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,    67,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    93,    -1,    35,
      -1,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,    67,     8,    -1,    10,    -1,    12,    -1,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    93,    -1,    35,
      -1,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,    67,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    93,    -1,    35,
      -1,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,    67,     8,    -1,    10,    -1,    12,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    93,    -1,    35,
      -1,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,     4,     5,     6,    -1,    65,
      66,    67,     8,    -1,    10,    -1,    12,    -1,    14,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    93,    -1,    35,
      -1,    97,    98,    99,   100,   101,   102,   103,   104,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    -1,    -1,   100,   101,   102,   103,   104
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    35,
      36,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    55,    65,    66,
      67,    75,    76,    77,    91,    93,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   107,   108,   109,   117,   120,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   138,   139,   140,   142,   143,   145,   147,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   160,
     162,   164,   166,   168,   169,   170,   171,   172,   177,   178,
      55,   102,   134,    13,   134,    15,   134,    17,   117,   134,
     134,    12,    12,   124,    12,    12,    12,   102,    12,   102,
     177,   178,    16,     3,   134,     3,     3,   102,   145,    32,
      77,    93,   145,   102,   116,    12,   145,   121,   145,    93,
     102,   173,    16,   134,    12,    65,    98,   102,    29,    34,
     144,     0,   107,   117,     3,     4,     5,     6,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    61,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,   137,    14,   138,    30,    56,    95,   111,   119,   102,
     148,   149,    33,    74,    60,    72,    73,    71,    61,    68,
     159,    62,    63,    64,    65,   161,    69,    70,   163,    66,
      67,   165,     7,     8,     9,   167,    31,   171,    59,    14,
      50,    51,    58,    93,   136,   138,   174,   176,    11,    13,
      15,    17,    13,    13,   134,   134,    38,    40,   134,   134,
     133,     3,   134,    34,    34,   102,   115,     3,   138,   123,
     121,   173,   148,    58,    63,    13,   102,    12,   138,   139,
     115,     5,    94,   134,   146,    13,   116,   116,    12,   144,
     134,   140,    15,    54,    55,   118,   138,   139,     5,   134,
     155,   156,   157,   158,   160,   162,   164,   166,   168,   169,
     145,   177,   102,   173,    12,   175,    14,   138,    13,    13,
      12,    12,    13,    13,   133,    13,     5,    17,    12,    94,
     116,   144,   123,    12,   141,   102,    94,   173,   145,     3,
      17,    13,    63,   116,   115,   121,   148,    34,    94,    13,
     134,    15,   124,   124,   134,   134,   124,   124,    13,   134,
     123,   115,     3,   102,   102,   145,    13,   122,   145,   123,
      13,   148,   146,     3,   144,   144,    13,    57,   110,   102,
     153,    13,    37,    13,    13,   124,    13,     3,    13,   148,
      13,   149,   141,   144,   116,    16,   141,   124,     3,     3,
     124,   141,     5,   109,   112,   113,   114,   117,   143,   123,
     122,    17,   113
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   106,   107,   107,   108,   108,   108,   109,   110,   110,
     111,   111,   112,   112,   113,   113,   114,   114,   114,   115,
     115,   116,   116,   117,   117,   118,   118,   119,   119,   120,
     120,   121,   121,   121,   122,   122,   123,   123,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     125,   125,   126,   127,   128,   128,   129,   130,   130,   130,
     130,   130,   130,   130,   131,   131,   132,   132,   132,   132,
     133,   133,   134,   134,   135,   135,   136,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   138,   138,   138,   139,   139,   139,   140,   140,
     140,   140,   140,   141,   141,   142,   142,   143,   143,   143,
     144,   144,   145,   145,   145,   145,   145,   145,   146,   146,
     147,   148,   148,   149,   149,   149,   150,   151,   152,   153,
     153,   154,   154,   155,   155,   156,   156,   157,   157,   158,
     158,   159,   159,   160,   160,   161,   161,   161,   161,   162,
     162,   163,   163,   164,   164,   165,   165,   166,   166,   167,
     167,   167,   168,   168,   169,   169,   170,   170,   170,   170,
     170,   170,   170,   171,   171,   171,   171,   171,   172,   172,
     173,   173,   174,   174,   175,   175,   176,   176,   177,   177,
     177,   177,   177,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     8,     2,     0,
       1,     0,     1,     0,     1,     2,     1,     1,     1,     1,
       3,     1,     3,     1,     2,     1,     0,     1,     1,     6,
       7,     1,     2,     2,     2,     4,     2,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     6,     2,     3,     3,     3,     5,     5,     7,     6,
       7,     5,     7,     5,     5,     7,     2,     3,     2,     2,
       2,     1,     1,     3,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     4,     5,     2,     3,     2,     1,     2,
       5,     3,     6,     2,     3,     0,     4,     7,     5,     4,
       0,     1,     2,     3,     4,     5,     5,     6,     2,     3,
       5,     1,     3,     1,     2,     2,     3,     3,     3,     1,
       5,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     2,     3,     3,     1,     3,
       1,     3,     0,     1,     2,     3,     1,     1,     1,     2,
       3,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     2
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
          case 121: /* type_decl2  */
#line 148 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1713 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 145: /* type_decl  */
#line 148 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1719 "src/ast/parser.c" /* yacc.c:1257  */
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
#line 152 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL, get_pos(scanner));  }
#line 1987 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 153 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(scanner)); }
#line 1993 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 157 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(scanner)); }
#line 1999 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 158 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(scanner)); }
#line 2005 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 159 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(scanner)); }
#line 2011 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 164 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def( (yyvsp[-6].ival), (yyvsp[-4].id_list), (yyvsp[-3].id_list), (yyvsp[-1].class_body), get_pos(scanner)); (yyval.class_def)->types = (yyvsp[-7].id_list); }
#line 2017 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 167 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[0].id_list); }
#line 2023 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 167 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2029 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 170 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_public; }
#line 2035 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 171 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_private; }
#line 2041 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 175 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = (yyvsp[0].class_body); }
#line 2047 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 176 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2053 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 180 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body( (yyvsp[0].section), get_pos(scanner)); }
#line 2059 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 181 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = prepend_class_body( (yyvsp[-1].section), (yyvsp[0].class_body), get_pos(scanner)); }
#line 2065 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 185 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list( (yyvsp[0].stmt_list), get_pos(scanner)); }
#line 2071 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 186 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def( (yyvsp[0].func_def), get_pos(scanner)); }
#line 2077 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 187 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def( (yyvsp[0].class_def), get_pos(scanner)); }
#line 2083 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 191 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2089 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 192 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2095 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 196 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list( (yyvsp[0].sval), get_pos(scanner)); }
#line 2101 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 197 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list( (yyvsp[-2].sval), (yyvsp[0].id_list), get_pos(scanner)); }
#line 2107 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 201 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(scanner));}
#line 2113 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 202 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(scanner));}
#line 2119 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 206 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_static;   }
#line 2125 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 207 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_instance; }
#line 2131 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 211 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_func; }
#line 2137 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 212 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2143 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 216 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-2].sval), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(scanner)); }
#line 2149 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 217 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(ae_flag_static, (yyvsp[-2].sval), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(scanner)); }
#line 2155 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 221 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2161 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 222 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2167 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 223 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array( (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2173 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 227 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(scanner)); }
#line 2179 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 228 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(scanner)); }
#line 2185 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 232 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( NULL, get_pos(scanner)); }
#line 2191 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 233 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code( (yyvsp[-1].stmt_list), get_pos(scanner)); }
#line 2197 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 252 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-2].id_list), NULL, get_pos(scanner)); }
#line 2203 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 253 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sval), get_pos(scanner)); }
#line 2209 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 257 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 1, get_pos(scanner)); }
#line 2215 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 261 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sval), 0, get_pos(scanner)); }
#line 2221 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 265 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2227 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 266 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(scanner)); }
#line 2233 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 270 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner));}
#line 2239 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 275 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2245 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 277 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2251 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2257 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for( (yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2263 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(scanner)); }
#line 2269 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 285 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until( (yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(scanner)); }
#line 2275 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 287 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop( (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2281 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 292 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(scanner)); }
#line 2287 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 294 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if( (yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2293 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 298 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( NULL, get_pos(scanner)); }
#line 2299 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 299 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2305 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 300 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_break(get_pos(scanner)); }
#line 2311 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 301 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_continue(get_pos(scanner)); }
#line 2317 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 305 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_expression((yyvsp[-1].exp),   get_pos(scanner)); }
#line 2323 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 306 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_expression(NULL, get_pos(scanner)); }
#line 2329 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 310 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2335 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 311 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_expression((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2341 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 315 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2347 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 316 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2353 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 320 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2359 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 324 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2365 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 325 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2371 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 326 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; /* LCOV_EXCL_LINE */ }
#line 2377 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 327 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2383 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 328 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2389 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 329 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2395 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 330 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2401 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 331 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2407 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 332 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2413 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 333 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2419 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 334 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2425 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 335 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2431 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 336 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2437 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 337 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2443 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 338 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2449 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 339 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2455 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 340 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rand; }
#line 2461 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 341 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ror; }
#line 2467 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 342 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_req; }
#line 2473 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 343 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rneq; }
#line 2479 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 344 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rgt; }
#line 2485 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 345 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rge; }
#line 2491 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 346 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rlt; }
#line 2497 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 347 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rle; }
#line 2503 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 348 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2509 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 352 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub( (yyvsp[-1].exp), get_pos(scanner) ); }
#line 2515 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 353 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( (yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2521 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 354 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub( new_array_sub( NULL, get_pos(scanner)), (yyvsp[-3].exp)); }
#line 2527 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 358 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(scanner)); }
#line 2533 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 359 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2539 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 360 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-1].array_sub), (yyvsp[0].array_sub)->exp_list); }
#line 2545 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 365 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); }
#line 2551 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 366 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 0, get_pos(scanner)); (yyval.exp)->d.exp_decl.types = (yyvsp[-3].type_list); }
#line 2557 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 367 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); }
#line 2563 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 368 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), 1, get_pos(scanner)); (yyval.exp)->d.exp_decl.types = (yyvsp[-3].type_list); }
#line 2569 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 372 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2575 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 114:
#line 373 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2581 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 115:
#line 376 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2587 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 116:
#line 376 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2593 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 117:
#line 380 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scanner)); (yyval.func_def)->types = (yyvsp[-6].id_list); if((yyvsp[-6].id_list)) SET_FLAG((yyval.func_def), ae_flag_template);}
#line 2599 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 118:
#line 382 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_func | ae_flag_static | ae_flag_op , (yyvsp[-3].type_decl), (yyvsp[-2].sval), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(scanner)); }
#line 2605 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 119:
#line 384 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_func | ae_flag_instance | ae_flag_dtor, new_type_decl(new_id_list("void", get_pos(scanner)), 0, 
      get_pos(scanner)), "dtor", NULL, (yyvsp[0].stmt), get_pos(scanner)); }
#line 2612 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 120:
#line 388 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2618 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 388 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 2624 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 391 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), (yyvsp[0].ival), get_pos(scanner)); }
#line 2630 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 392 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sval), get_pos(scanner)), (yyvsp[0].ival), get_pos(scanner)); SET_FLAG((yyval.type_decl), ae_flag_const); }
#line 2636 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 393 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scanner)); }
#line 2642 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 394 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scanner)); SET_FLAG((yyval.type_decl), ae_flag_const); }
#line 2648 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 395 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scanner)); }
#line 2654 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 396 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(scanner)); SET_FLAG((yyval.type_decl), ae_flag_const); }
#line 2660 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 401 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2666 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 402 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2672 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 406 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-2].decl_list), get_pos(scanner)); }
#line 2678 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 410 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(scanner)); }
#line 2684 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 411 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(scanner)); }
#line 2690 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 415 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sval), NULL, get_pos(scanner)); }
#line 2696 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 416 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2702 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 417 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sval),   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 2708 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 420 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex( (yyvsp[-1].exp), get_pos(scanner)); }
#line 2714 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 421 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar(   (yyvsp[-1].exp), get_pos(scanner)); }
#line 2720 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 422 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec(     (yyvsp[-1].exp), get_pos(scanner)); }
#line 2726 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 427 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_if( (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 2732 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 431 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2738 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 433 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2744 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 143:
#line 437 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2750 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 439 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2756 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 145:
#line 443 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2762 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 445 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(scanner)); }
#line 2768 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 147:
#line 449 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2774 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 451 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(scanner)); }
#line 2780 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 149:
#line 455 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2786 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 457 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(scanner)); }
#line 2792 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 151:
#line 460 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2798 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 460 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_neq; }
#line 2804 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 153:
#line 462 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2810 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 464 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2816 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 467 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_lt; }
#line 2822 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 467 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_gt; }
#line 2828 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 157:
#line 467 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_le; }
#line 2834 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 467 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ge; }
#line 2840 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 470 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2846 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 472 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2852 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 476 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 2858 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 477 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 2864 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 163:
#line 481 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2870 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 483 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2876 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 486 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2882 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 486 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 2888 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 167:
#line 489 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2894 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 491 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2900 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 494 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 2906 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 494 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide; }
#line 2912 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 171:
#line 494 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_percent; }
#line 2918 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 497 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2924 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 499 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary( (yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2930 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 505 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_cast( (yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(scanner)); }
#line 2936 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 176:
#line 508 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2942 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 508 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 2948 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 178:
#line 508 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 2954 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 509 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 2960 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 509 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 2966 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 510 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 2972 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 510 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 2978 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 514 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); }
#line 2984 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 516 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary( (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(scanner)); }
#line 2990 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 518 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(scanner)); }
#line 2996 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 520 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary2(op_new, (yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3002 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 187:
#line 522 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary3( op_spork, (yyvsp[0].stmt), get_pos(scanner)); }
#line 3008 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 528 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); }
#line 3014 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 532 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sval), get_pos(scanner)), NULL, get_pos(scanner)); }
#line 3020 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 191:
#line 533 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sval), get_pos(scanner)), (yyvsp[0].type_list), get_pos(scanner)); }
#line 3026 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 536 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 3032 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 536 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[0].type_list);}
#line 3038 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 537 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 3044 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 537 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3050 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 541 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3056 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 541 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3062 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 199:
#line 546 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array( (yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3068 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 548 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_call( (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(scanner)); (yyval.exp)->d.exp_func.types = (yyvsp[-1].type_list); }
#line 3074 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 550 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dot( (yyvsp[-2].exp), (yyvsp[0].sval), get_pos(scanner)); }
#line 3080 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 552 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_postfix( (yyvsp[-1].exp), (yyvsp[0].ival), get_pos(scanner)); }
#line 3086 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 556 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_ID(      (yyvsp[0].sval), get_pos(scanner)); }
#line 3092 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 557 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_int(     (yyvsp[0].ival), get_pos(scanner)); }
#line 3098 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 558 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_float(   (yyvsp[0].fval), get_pos(scanner)); }
#line 3104 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 559 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_string(  (yyvsp[0].sval), get_pos(scanner)); }
#line 3110 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 560 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_char(    (yyvsp[0].sval), get_pos(scanner)); }
#line 3116 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 561 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3122 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 562 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(   (yyvsp[0].array_sub), get_pos(scanner)); }
#line 3128 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 563 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_complex( (yyvsp[0].c_val), get_pos(scanner)); }
#line 3134 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 564 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_polar(   (yyvsp[0].polar), get_pos(scanner)); }
#line 3140 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 565 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_vec(     (yyvsp[0].vec), get_pos(scanner)); }
#line 3146 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 566 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_hack(    (yyvsp[-1].exp), get_pos(scanner)); }
#line 3152 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 567 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) =                       (yyvsp[-1].exp);                    }
#line 3158 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 568 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_nil(         get_pos(scanner)); }
#line 3164 "src/ast/parser.c" /* yacc.c:1646  */
    break;


#line 3168 "src/ast/parser.c" /* yacc.c:1646  */
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
#line 570 "utils/gwion.y" /* yacc.c:1906  */

