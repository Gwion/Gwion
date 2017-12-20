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
    NUM = 348,
    FLOAT = 349,
    ID = 350,
    STRING_LIT = 351,
    CHAR_LIT = 352
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
#define NUM 348
#define FLOAT 349
#define ID 350
#define STRING_LIT 351
#define CHAR_LIT 352

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

#line 355 "src/ast/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int gwion_parse (Scanner* arg);

#endif /* !YY_GWION_INCLUDE_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 371 "src/ast/parser.c" /* yacc.c:358  */

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
#define YYLAST   1485

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  75
/* YYNRULES -- Number of rules.  */
#define YYNRULES  216
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  382

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   352

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
      95,    96,    97
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   111,   111,   112,   116,   117,   118,   122,   124,   125,
     128,   128,   130,   130,   133,   134,   138,   139,   143,   144,
     148,   149,   153,   154,   158,   159,   163,   164,   169,   173,
     174,   175,   179,   180,   184,   185,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   204,
     206,   206,   209,   210,   214,   218,   222,   223,   227,   231,
     233,   235,   237,   239,   241,   243,   248,   250,   255,   256,
     257,   258,   262,   263,   267,   268,   272,   273,   276,   276,
     278,   278,   278,   279,   279,   280,   280,   281,   281,   282,
     282,   283,   283,   283,   284,   284,   285,   289,   290,   291,
     295,   296,   297,   301,   302,   303,   304,   306,   311,   312,
     315,   318,   320,   322,   326,   326,   326,   326,   326,   326,
     328,   329,   331,   333,   338,   338,   341,   342,   343,   344,
     348,   349,   353,   354,   356,   361,   362,   366,   367,   368,
     371,   372,   373,   375,   375,   378,   378,   381,   381,   384,
     384,   387,   387,   390,   390,   393,   393,   395,   395,   398,
     398,   399,   399,   401,   401,   405,   406,   409,   409,   412,
     412,   414,   414,   417,   417,   417,   419,   419,   422,   422,
     425,   425,   425,   426,   426,   427,   427,   430,   430,   432,
     441,   444,   444,   448,   449,   452,   452,   454,   454,   456,
     456,   458,   460,   462,   467,   468,   469,   470,   471,   472,
     473,   474,   475,   476,   477,   478,   479
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
  "LTB", "GTB", "VARARG", "UNION", "ATPAREN", "TYPEOF", "CONST", "NUM",
  "FLOAT", "ID", "STRING_LIT", "CHAR_LIT", "$accept", "ast", "section",
  "class_def", "class_ext", "class_body", "class_body2", "id_list",
  "id_dot", "stmt_list", "static_decl", "function_decl", "func_ptr",
  "stmt_typedef", "type_decl2", "arg_list", "code_segment", "stmt", "id",
  "opt_id", "enum_stmt", "label_stmt", "goto_stmt", "case_stmt",
  "switch_stmt", "loop_stmt", "selection_stmt", "jump_stmt", "exp_stmt",
  "exp", "binary_exp", "template", "op", "array_exp", "array_empty",
  "decl_exp", "func_args", "decl_template", "func_def_base", "op_op",
  "func_def", "atsym", "type_decl", "decl_list", "union_stmt",
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
     345,   346,   347,   348,   349,   350,   351,   352
};
# endif

#define YYPACT_NINF -274

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-274)))

#define YYTABLE_NINF -125

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     373,  -274,  -274,  1255,   919,   967,   468,  1255,  1255,  -274,
    -274,    33,    41,   539,    53,    55,    59,     5,    96,    58,
      93,   610,   112,   114,  -274,  -274,    54,    89,     5,   800,
     -18,   752,     5,  -274,  -274,   113,  1402,    54,    43,     5,
    -274,   118,  1255,   126,    54,  -274,  -274,  -274,  -274,  -274,
     130,   373,  -274,  -274,    88,  -274,  -274,  -274,   539,    12,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,   144,
     316,  -274,   136,  -274,   153,  -274,  -274,     5,  -274,  -274,
    -274,  -274,  -274,    22,    97,    98,    83,   100,   -15,   103,
     -22,    -5,   154,   139,  1303,  -274,   115,   127,  -274,  1255,
    1255,   -21,   163,  1351,  -274,   166,  -274,   165,  -274,   871,
     164,   171,   172,  1255,  1255,    74,  1255,  1255,   681,   187,
    1255,  -274,    45,   158,     5,  -274,   192,  -274,  -274,  -274,
     183,   200,   199,   174,   217,  1015,    54,  -274,  -274,  -274,
    -274,   -18,  1015,  -274,  -274,    76,  -274,  -274,   175,   179,
     227,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,    54,  -274,  -274,  -274,  -274,  -274,     3,     5,
     225,   155,  1255,   230,     5,  -274,  -274,  -274,  -274,    54,
    -274,  -274,  -274,  -274,  -274,  1255,  1255,  1063,  -274,    76,
    -274,  -274,   200,  -274,   239,  1255,  1351,  1351,  1351,  1351,
    1351,  -274,  -274,  1351,  1351,  1351,  1351,  1351,    54,    54,
    -274,    58,  1255,     5,     5,   234,  -274,  -274,  -274,  -274,
     233,  -274,  -274,  -274,   235,   236,   240,   242,   238,   243,
     681,  -274,   245,  -274,  -274,   244,  -274,  -274,   136,  -274,
       5,   247,     5,   252,   183,     5,   199,   254,     5,   256,
     168,     5,  1255,   264,   253,  -274,   259,     5,  -274,  -274,
    -274,  -274,   136,     5,   241,    97,    98,    83,   100,   -15,
     103,   -22,    -5,   154,   139,  -274,   254,   127,  -274,   182,
    1111,  -274,  1159,  -274,   539,   539,  1255,  1255,   539,   539,
    1207,   199,     5,  -274,   373,  -274,  -274,  -274,  -274,    27,
     199,   260,  -274,  -274,  -274,  -274,  1255,     5,   183,   254,
    -274,  1351,   199,  -274,  -274,   261,  -274,   248,  -274,   265,
     266,  -274,  -274,   539,   267,  -274,  -274,   278,   373,   269,
    -274,  -274,   274,     5,  -274,   254,  -274,   279,  -274,   199,
    -274,  -274,  -274,   539,   285,   287,  -274,   539,  -274,  -274,
    -274,  -274,   290,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
      54,  -274
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    73,   182,     0,     0,     0,     0,     0,     0,    24,
     185,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   184,     0,     0,     0,     0,
       0,     0,     0,   181,   180,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,   205,   206,    49,   207,   208,
       0,     2,     6,     4,    23,    46,    47,    39,    20,   204,
      44,    40,    41,    43,    42,    37,    38,    45,    36,     0,
      74,   209,   210,    76,     0,   120,     5,     0,    48,   211,
     212,   213,   103,   143,   145,   147,   149,   151,   153,   157,
     163,   167,   171,   176,     0,   178,   187,   191,   199,     0,
       0,   204,     0,     0,   216,     0,   100,     0,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   204,    78,   199,     0,    68,     0,    70,    71,   189,
     124,    29,   186,    11,    16,     0,     0,    27,   106,   133,
       8,     0,     0,    53,   107,     0,   112,   134,     0,    18,
       0,    80,    96,   174,   173,   175,    84,    85,    86,    87,
      88,    83,    81,    89,    90,   197,   198,    82,   162,   160,
     161,   159,   170,   169,   165,   166,    91,    92,    93,    94,
      95,   114,     0,   117,   115,   119,   118,   116,     0,     0,
     193,     0,     0,     0,     0,   129,     1,     3,    22,     0,
      21,   125,    54,   126,    72,     0,     0,     0,   102,     0,
       9,   113,   137,   104,   135,     0,     0,     0,     0,     0,
       0,   155,   156,     0,     0,     0,     0,     0,     0,     0,
     188,     0,     0,     0,     0,     0,   200,   203,   214,   215,
      97,    35,   141,   140,     0,     0,     0,     0,     0,     0,
       0,    55,     0,    57,    56,     0,    69,    31,    30,   190,
       0,     0,     0,     0,   124,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   142,     0,     0,    75,    77,
     101,   138,   139,     0,     0,   146,   148,   150,   152,   154,
     158,   164,   168,   172,   177,   179,     0,   192,   202,     0,
       0,   201,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    10,    13,    17,   127,    19,   123,     0,
       0,     0,    28,   110,   194,   105,   130,    50,   124,     0,
     136,     0,     0,    79,   195,     0,    99,    66,    59,     0,
       0,    63,    65,     0,     0,    58,    51,     0,    14,     0,
      12,   108,     0,     0,   121,     0,   131,     0,   128,     0,
     144,   122,   196,     0,     0,     0,    61,     0,    52,    15,
       7,   109,    32,    26,   132,   111,    67,    60,    64,    62,
       0,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -274,   249,  -272,   -14,  -274,  -274,   -56,  -120,  -187,    25,
    -274,  -274,   272,  -274,   -26,   -78,   -75,   -11,     0,   -24,
       2,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -113,    84,
    -274,  -274,   237,   -47,  -111,   -19,  -273,    -1,   -13,  -274,
    -274,  -229,   -23,   -20,    -7,    28,   -43,  -274,  -274,  -274,
      -8,  -274,   101,   107,    95,    99,  -274,   105,   292,   106,
     294,   102,   297,   119,   309,   120,   123,     1,   -60,  -274,
    -207,  -274,   311,   -10,   332
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    50,    51,    52,   261,   349,   350,   133,   148,    53,
     199,    54,    55,    56,   129,   352,    57,    58,   101,   347,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,   235,   181,    71,    72,    73,   320,    74,    75,   182,
      76,   203,    77,   274,    78,   213,   214,    79,    80,    81,
      82,    83,    84,    85,    86,    87,   223,    88,   224,    89,
     225,    90,   226,    91,   227,    92,    93,   103,    95,    96,
     191,   301,   237,    97,    98
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      59,    94,   115,   131,   255,   250,    59,   276,   201,   122,
     138,   188,   144,    59,   131,   268,   140,   119,   146,   121,
     258,   195,   139,   332,   147,   208,   130,   299,   134,   141,
     145,   110,   149,   143,   230,   316,    28,   130,    30,   190,
     351,   201,   348,   230,   130,   113,   202,   221,   174,   175,
     236,    59,    94,   114,   222,   215,   359,   259,    59,   232,
     210,   211,   172,   173,   324,   116,    38,   117,     3,   270,
       4,   118,     5,   313,  -124,   236,   348,   212,   317,   253,
     138,   144,   373,   200,   257,     7,     8,   102,   105,   107,
     144,   111,   112,    32,   121,   165,   166,   216,    47,   358,
      47,   282,   147,   121,   233,   126,     9,  -124,   120,   124,
     263,   143,   246,   131,   247,   127,   144,   128,    43,    44,
      32,   132,    47,   138,   134,   150,   193,   210,   147,   189,
     196,   234,   211,   209,   192,   139,   130,   310,   194,   -78,
     141,   232,   315,   198,   145,    43,    44,   204,    42,    47,
     207,    45,    46,    47,    48,    49,   267,   219,   217,   131,
      38,   153,   154,   155,    40,   281,   168,   169,   170,   171,
     228,   218,   220,   277,   238,   231,   131,   165,   166,   239,
     240,   241,   130,     9,   242,   243,   233,   279,   269,   134,
     251,   318,   254,   303,   149,   256,   146,   244,   245,   130,
     248,   249,   295,   296,   252,   131,   131,    28,   145,    30,
     209,   208,   201,   234,     5,     6,   121,   121,   121,   121,
     121,   297,   262,   121,   121,   121,   121,   121,   130,   130,
     271,   121,   260,   298,   190,   208,   345,    38,   265,   264,
     266,    40,   272,   275,   283,   354,   300,   302,   304,   305,
     236,   308,   306,   325,   307,   323,   309,   361,   311,   322,
     149,   312,   134,   314,   268,   149,   319,   326,   321,   333,
     327,   190,   328,   355,   362,   331,   273,   329,   364,   365,
     367,   368,   374,   212,   375,   363,   370,   371,   377,   278,
     378,   107,   369,   337,   338,   380,   353,   341,   342,   284,
     197,   137,   381,   357,    59,    59,   356,   206,    59,    59,
     372,   330,   346,   287,    59,    94,   107,   285,   288,   130,
     151,   205,   152,   360,   286,   289,   291,   346,   183,   290,
     184,   121,   366,   185,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    59,   292,   186,   293,   187,    59,    94,
     294,   123,   376,   212,     0,     0,   379,   353,     0,     0,
       0,     0,     0,    59,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     1,     0,   167,     0,
     130,     2,     0,     3,   335,     4,   107,     5,     0,     6,
     339,   340,     0,     0,   344,   176,   177,   178,   179,   180,
       7,     8,     0,     9,     0,     0,     0,     0,    10,    11,
     273,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,     0,     0,     0,     0,     0,     0,     0,    32,
      33,    34,     0,     0,     0,     0,     0,     0,     0,    35,
      36,    37,     0,     0,     0,     0,     0,    38,     0,    39,
       0,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     1,     0,     0,     0,     0,     2,     0,     3,     0,
       4,     0,     5,     0,     6,   108,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     8,     0,     0,     0,
       0,     0,     0,    10,    11,     0,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    29,     0,   109,     0,     0,     0,     0,
       0,     0,     0,     0,    32,    33,    34,     0,     0,     0,
       0,     0,     1,     0,     0,     0,    37,     2,     0,     3,
       0,     4,     0,     5,    39,     6,     0,    41,    42,    43,
      44,    45,    46,    47,    48,    49,     7,     8,     0,     0,
       0,     0,     0,     0,    10,    11,     0,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    29,     0,   109,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,     0,     0,
       0,     0,     0,   125,     0,     0,     0,    37,     2,     0,
       3,     0,     4,     0,     5,    39,     0,     0,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     7,     8,     0,
       0,     0,     0,     0,     0,    10,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    25,    26,    27,     0,    99,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     1,     0,     0,     0,     0,     2,
       0,     3,     0,     4,     0,     5,    39,     0,     0,     0,
      42,    43,    44,    45,    46,    47,    48,    49,     7,     8,
       0,     0,     0,     0,     0,     0,    10,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    24,    25,    26,    27,     0,    99,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    33,    34,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       2,     0,     3,     0,     4,     0,     5,    39,     0,     0,
       0,    42,    43,    44,    45,    46,    47,    48,    49,     7,
       8,     0,     9,     0,     0,     0,     0,    10,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    20,     0,
       0,     0,    24,    25,    26,    27,     0,   142,     2,    31,
       3,     0,     4,     0,     5,     0,     0,     0,    32,    33,
      34,     0,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,    10,    38,     0,    39,     0,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      24,    25,    26,    27,     0,    29,     0,   135,     0,     0,
       0,     0,     0,     0,     0,     0,    32,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   136,     2,
       0,     3,     0,     4,     0,     5,    39,     0,     0,    41,
      42,    43,    44,    45,    46,    47,    48,    49,     7,     8,
       0,     0,     0,     0,     0,     0,    10,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    20,     0,     0,
       0,    24,    25,    26,    27,     0,   142,     2,   109,     3,
       0,     4,   104,     5,     0,     0,     0,    32,    33,    34,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,    10,     0,     0,    39,     0,     0,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    24,
      25,    26,    27,     0,    99,     2,   100,     3,     0,     4,
       0,     5,   106,     0,     0,    32,    33,    34,     0,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
       0,     0,    10,     0,     0,    39,     0,     0,     0,    42,
      43,    44,    45,    46,    47,    48,    49,    24,    25,    26,
      27,     0,    99,     2,   100,     3,     0,     4,     0,     5,
       0,     0,     0,    32,    33,    34,     0,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,     0,     0,
      10,     0,     0,    39,     0,     0,     0,    42,    43,    44,
      45,    46,    47,    48,    49,    24,    25,    26,    27,     0,
     142,     2,   135,     3,     0,     4,     0,     5,   280,     0,
       0,    32,    33,    34,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,    10,     0,
       0,    39,     0,     0,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    24,    25,    26,    27,     0,    99,     2,
     100,     3,     0,     4,   334,     5,     0,     0,     0,    32,
      33,    34,     0,     0,     0,     0,     0,     0,     7,     8,
       0,     0,     0,     0,     0,     0,    10,     0,     0,    39,
       0,     0,     0,    42,    43,    44,    45,    46,    47,    48,
      49,    24,    25,    26,    27,     0,    99,     2,   100,     3,
       0,     4,     0,     5,   336,     0,     0,    32,    33,    34,
       0,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,    10,     0,     0,    39,     0,     0,
       0,    42,    43,    44,    45,    46,    47,    48,    49,    24,
      25,    26,    27,     0,    99,     2,   100,     3,     0,     4,
     343,     5,     0,     0,     0,    32,    33,    34,     0,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
       0,     0,    10,     0,     0,    39,     0,     0,     0,    42,
      43,    44,    45,    46,    47,    48,    49,    24,    25,    26,
      27,     0,    99,     2,   100,     3,     0,     4,     0,     5,
       0,     0,     0,    32,    33,    34,     0,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,     0,     0,
      10,     0,     0,    39,     0,     0,     0,    42,    43,    44,
      45,    46,    47,    48,    49,    24,    25,    26,    27,     0,
      99,     2,   100,     3,     0,     4,     0,     5,     0,     0,
       0,    32,    33,    34,     0,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,    10,     0,
       0,    39,     0,     0,     0,    42,    43,    44,    45,    46,
      47,    48,    49,    24,    25,    26,    27,     0,     0,     2,
       0,     3,     0,     4,     0,     5,     0,     0,     0,     0,
      33,    34,     0,     0,     0,     0,     0,     0,     7,     8,
     229,     0,     0,     0,     0,     0,    10,     0,     0,     0,
       0,     0,     0,    42,     0,     0,    45,    46,    47,    48,
      49,    24,    25,    26,    27,     0,   151,     0,   152,   153,
     154,   155,     0,     0,     0,     0,     0,     0,    33,    34,
     156,   157,   158,   159,   160,   161,   162,   163,   164,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    42,     0,     0,    45,    46,    47,    48,    49,     0,
       0,     0,   165,   166,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,   168,   169,   170,   171,   172,
     173,     0,   174,   175,     0,     0,     0,     0,     0,     0,
       0,   176,   177,   178,   179,   180
};

static const yytype_int16 yycheck[] =
{
       0,     0,    13,    26,   124,   118,     6,   194,    29,    19,
      29,    37,    31,    13,    37,    12,    30,    17,    31,    19,
     131,    44,    29,   296,    31,    72,    26,   234,    28,    30,
      31,     6,    32,    31,    94,   264,    54,    37,    56,    39,
      13,    29,   314,   103,    44,    12,    34,    62,    70,    71,
      97,    51,    51,    12,    69,    33,   329,   132,    58,    14,
      74,    74,    67,    68,   271,    12,    84,    12,    10,   189,
      12,    12,    14,   260,    95,   122,   348,    77,   265,    34,
      99,   100,   355,    58,   131,    27,    28,     3,     4,     5,
     109,     7,     8,    66,    94,    50,    51,    75,    95,   328,
      95,   212,   109,   103,    59,    21,    30,    95,    12,    16,
     136,   109,    38,   136,    40,     3,   135,     3,    91,    92,
      66,    32,    95,   142,   124,    12,    42,   141,   135,    86,
       0,    86,   145,    57,    16,   142,   136,   250,    12,    12,
     141,    14,   262,    55,   145,    91,    92,     3,    90,    95,
      14,    93,    94,    95,    96,    97,   182,    74,    61,   182,
      84,     7,     8,     9,    88,   212,    63,    64,    65,    66,
      31,    73,    72,   199,    11,    60,   199,    50,    51,    13,
      15,    17,   182,    30,    13,    13,    59,   206,   188,   189,
       3,   266,    34,   240,   194,     3,   209,   113,   114,   199,
     116,   117,   228,   229,   120,   228,   229,    54,   209,    56,
      57,   258,    29,    86,    14,    16,   216,   217,   218,   219,
     220,   231,     5,   223,   224,   225,   226,   227,   228,   229,
       5,   231,    58,   233,   234,   282,   311,    84,    59,    64,
      13,    88,    87,    13,     5,   320,    12,    14,    13,    13,
     297,    13,    12,   272,    12,    87,    13,   332,    13,     3,
     260,    17,   262,    16,    12,   265,    12,     3,   268,    87,
      17,   271,    13,    13,    13,    34,   192,   277,    13,    13,
      13,     3,     3,   283,   359,    37,    17,    13,     3,   205,
       3,   207,   348,   304,   305,     5,   319,   308,   309,   215,
      51,    29,   380,   327,   304,   305,   326,    70,   308,   309,
     353,   283,   312,   218,   314,   314,   232,   216,   219,   319,
       4,     5,     6,   331,   217,   220,   224,   327,    36,   223,
      36,   331,   343,    36,    18,    19,    20,    21,    22,    23,
      24,    25,    26,   343,   225,    36,   226,    36,   348,   348,
     227,    19,   363,   353,    -1,    -1,   367,   380,    -1,    -1,
      -1,    -1,    -1,   363,    -1,    -1,    -1,   367,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    62,    -1,
     380,     8,    -1,    10,   300,    12,   302,    14,    -1,    16,
     306,   307,    -1,    -1,   310,    79,    80,    81,    82,    83,
      27,    28,    -1,    30,    -1,    -1,    -1,    -1,    35,    36,
     326,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      77,    78,    -1,    -1,    -1,    -1,    -1,    84,    -1,    86,
      -1,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,     3,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      12,    -1,    14,    -1,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    78,     8,    -1,    10,
      -1,    12,    -1,    14,    86,    16,    -1,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    78,     8,    -1,
      10,    -1,    12,    -1,    14,    86,    -1,    -1,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    12,    -1,    14,    86,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    96,    97,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    51,    52,    53,    -1,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    12,    -1,    14,    86,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    27,
      28,    -1,    30,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    51,    52,    53,    -1,    55,     8,    57,
      10,    -1,    12,    -1,    14,    -1,    -1,    -1,    66,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    84,    -1,    86,    -1,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      50,    51,    52,    53,    -1,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,     8,
      -1,    10,    -1,    12,    -1,    14,    86,    -1,    -1,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    51,    52,    53,    -1,    55,     8,    57,    10,
      -1,    12,    13,    14,    -1,    -1,    -1,    66,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    86,    -1,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    50,
      51,    52,    53,    -1,    55,     8,    57,    10,    -1,    12,
      -1,    14,    15,    -1,    -1,    66,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    86,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    96,    97,    50,    51,    52,
      53,    -1,    55,     8,    57,    10,    -1,    12,    -1,    14,
      -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    86,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    50,    51,    52,    53,    -1,
      55,     8,    57,    10,    -1,    12,    -1,    14,    15,    -1,
      -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    50,    51,    52,    53,    -1,    55,     8,
      57,    10,    -1,    12,    13,    14,    -1,    -1,    -1,    66,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    86,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    50,    51,    52,    53,    -1,    55,     8,    57,    10,
      -1,    12,    -1,    14,    15,    -1,    -1,    66,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    86,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    50,
      51,    52,    53,    -1,    55,     8,    57,    10,    -1,    12,
      13,    14,    -1,    -1,    -1,    66,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    86,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    96,    97,    50,    51,    52,
      53,    -1,    55,     8,    57,    10,    -1,    12,    -1,    14,
      -1,    -1,    -1,    66,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    86,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    50,    51,    52,    53,    -1,
      55,     8,    57,    10,    -1,    12,    -1,    14,    -1,    -1,
      -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    86,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    96,    97,    50,    51,    52,    53,    -1,    -1,     8,
      -1,    10,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      77,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    93,    94,    95,    96,
      97,    50,    51,    52,    53,    -1,     4,    -1,     6,     7,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    93,    94,    95,    96,    97,    -1,
      -1,    -1,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    12,    14,    16,    27,    28,    30,
      35,    36,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    66,    67,    68,    76,    77,    78,    84,    86,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      99,   100,   101,   107,   109,   110,   111,   114,   115,   116,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   131,   132,   133,   135,   136,   138,   140,   142,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   155,   157,
     159,   161,   163,   164,   165,   166,   167,   171,   172,    55,
      57,   116,   127,   165,    13,   127,    15,   127,    17,    57,
     107,   127,   127,    12,    12,   115,    12,    12,    12,   116,
      12,   116,   171,   172,    16,     3,   127,     3,     3,   112,
     116,   140,    32,   105,   116,    57,    78,   110,   133,   142,
     101,   135,    55,   118,   133,   135,   136,   142,   106,   116,
      12,     4,     6,     7,     8,     9,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    50,    51,    62,    63,    64,
      65,    66,    67,    68,    70,    71,    79,    80,    81,    82,
      83,   130,   137,   156,   158,   160,   162,   170,   112,    86,
     116,   168,    16,   127,    12,   140,     0,    99,    55,   108,
     107,    29,    34,   139,     3,     5,   130,    14,   131,    57,
     101,   136,   116,   143,   144,    33,    75,    61,    73,    74,
      72,    62,    69,   154,   156,   158,   160,   162,    31,    77,
     166,    60,    14,    59,    86,   129,   131,   170,    11,    13,
      15,    17,    13,    13,   127,   127,    38,    40,   127,   127,
     126,     3,   127,    34,    34,   105,     3,   131,   132,   114,
      58,   102,     5,   112,    64,    59,    13,   112,    12,   116,
     105,     5,    87,   127,   141,    13,   106,   112,   127,   133,
      15,   131,   132,     5,   127,   150,   151,   152,   153,   155,
     157,   159,   161,   163,   164,   112,   112,   171,   116,   168,
      12,   169,    14,   131,    13,    13,    12,    12,    13,    13,
     126,    13,    17,   106,    16,   105,   139,   106,   114,    12,
     134,   116,     3,    87,   168,   133,     3,    17,    13,   116,
     143,    34,   134,    87,    13,   127,    15,   115,   115,   127,
     127,   115,   115,    13,   127,   114,   116,   117,   100,   103,
     104,    13,   113,   140,   114,    13,   141,   117,   139,   134,
     148,   114,    13,    37,    13,    13,   115,    13,     3,   104,
      17,    13,   144,   134,     3,   114,   115,     3,     3,   115,
       5,   113
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    98,    99,    99,   100,   100,   100,   101,   101,   101,
     102,   102,   103,   103,   104,   104,   105,   105,   106,   106,
     107,   107,   108,   108,   109,   109,   110,   110,   111,   112,
     112,   112,   113,   113,   114,   114,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   116,
     117,   117,   118,   118,   119,   120,   121,   121,   122,   123,
     123,   123,   123,   123,   123,   123,   124,   124,   125,   125,
     125,   125,   126,   126,   127,   127,   128,   128,   129,   129,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   131,   131,   131,
     132,   132,   132,   133,   133,   133,   133,   133,   134,   134,
     135,   136,   136,   136,   137,   137,   137,   137,   137,   137,
     138,   138,   138,   138,   139,   139,   140,   140,   140,   140,
     141,   141,   142,   142,   142,   143,   143,   144,   144,   144,
     145,   146,   147,   148,   148,   149,   149,   150,   150,   151,
     151,   152,   152,   153,   153,   154,   154,   155,   155,   156,
     156,   156,   156,   157,   157,   158,   158,   159,   159,   160,
     160,   161,   161,   162,   162,   162,   163,   163,   164,   164,
     165,   165,   165,   165,   165,   165,   165,   166,   166,   166,
     166,   167,   167,   168,   168,   169,   169,   170,   170,   171,
     171,   171,   171,   171,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172
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
       7,     6,     7,     5,     7,     5,     5,     7,     2,     3,
       2,     2,     2,     1,     1,     3,     1,     3,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     4,     5,
       2,     3,     2,     1,     2,     4,     2,     2,     2,     3,
       4,     6,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     5,     5,     4,     0,     1,     2,     4,     5,     2,
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
          case 101: /* class_def  */
#line 102 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_def(((*yyvaluep).class_def)); }
#line 1717 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 103: /* class_body  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1723 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 104: /* class_body2  */
#line 103 "utils/gwion.y" /* yacc.c:1257  */
      { free_class_body(((*yyvaluep).class_body)); }
#line 1729 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 110: /* func_ptr  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1735 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 111: /* stmt_typedef  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1741 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 112: /* type_decl2  */
#line 104 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1747 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 114: /* code_segment  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1753 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 115: /* stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1759 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 118: /* enum_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1765 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 119: /* label_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1771 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 120: /* goto_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1777 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 121: /* case_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1783 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 122: /* switch_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1789 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 123: /* loop_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1795 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 124: /* selection_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1801 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 125: /* jump_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1807 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 126: /* exp_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1813 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 129: /* template  */
#line 105 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 1819 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 140: /* type_decl  */
#line 104 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_decl(((*yyvaluep).type_decl)); }
#line 1825 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 142: /* union_stmt  */
#line 101 "utils/gwion.y" /* yacc.c:1257  */
      { free_stmt(((*yyvaluep).stmt)); }
#line 1831 "src/ast/parser.c" /* yacc.c:1257  */
        break;

    case 168: /* type_list  */
#line 105 "utils/gwion.y" /* yacc.c:1257  */
      { free_type_list(((*yyvaluep).type_list)); }
#line 1837 "src/ast/parser.c" /* yacc.c:1257  */
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
#line 111 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[0].section), NULL, get_pos(arg));  }
#line 2105 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 112 "utils/gwion.y" /* yacc.c:1646  */
    { arg->ast = (yyval.ast) = new_ast((yyvsp[-1].section), (yyvsp[0].ast), get_pos(arg)); }
#line 2111 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 116 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_stmt_list((yyvsp[0].stmt_list), get_pos(arg)); }
#line 2117 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 117 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_func_def ((yyvsp[0].func_def), get_pos(arg)); }
#line 2123 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 118 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.section) = new_section_class_def((yyvsp[0].class_def), get_pos(arg)); }
#line 2129 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 123 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_def) = new_class_def(0, (yyvsp[-4].id_list), (yyvsp[-3].id_list), (yyvsp[-1].class_body), get_pos(arg)); }
#line 2135 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 124 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].class_def), ae_flag_global); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2141 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 126 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].class_def), free_class_def); (yyval.class_def) = (yyvsp[0].class_def); }
#line 2147 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 128 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[0].id_list); }
#line 2153 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 128 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = NULL; }
#line 2159 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 130 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = NULL; }
#line 2165 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 133 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body((yyvsp[0].section), NULL, get_pos(arg)); }
#line 2171 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 134 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.class_body) = new_class_body((yyvsp[-1].section), (yyvsp[0].class_body), get_pos(arg)); }
#line 2177 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 138 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2183 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 139 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2189 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 143 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = new_id_list((yyvsp[0].sym), get_pos(arg)); }
#line 2195 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 144 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = prepend_id_list((yyvsp[-2].sym), (yyvsp[0].id_list), get_pos(arg)); }
#line 2201 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 148 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[0].stmt), NULL, get_pos(arg));}
#line 2207 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 149 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new_stmt_list((yyvsp[-1].stmt), (yyvsp[0].stmt_list), get_pos(arg));}
#line 2213 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 153 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_static;   }
#line 2219 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 154 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2225 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 158 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2231 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 159 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = ae_flag_variadic; }
#line 2237 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 163 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_func_ptr_stmt(0, (yyvsp[-2].sym), (yyvsp[-4].type_decl), (yyvsp[0].arg_list), get_pos(arg)); }
#line 2243 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 165 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_ptr), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2249 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 170 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_typedef((yyvsp[-2].type_decl), (yyvsp[-1].sym), get_pos(arg)); }
#line 2255 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 174 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(arg)); }
#line 2261 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 175 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = add_type_decl_array((yyvsp[-1].type_decl), (yyvsp[0].array_sub), get_pos(arg)); }
#line 2267 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 179 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-1].type_decl), (yyvsp[0].var_decl), NULL, get_pos(arg)); }
#line 2273 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 180 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = new_arg_list((yyvsp[-3].type_decl), (yyvsp[-2].var_decl), (yyvsp[0].arg_list), get_pos(arg)); }
#line 2279 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 184 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code(NULL, get_pos(arg)); }
#line 2285 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 185 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_code((yyvsp[-1].stmt_list), get_pos(arg)); }
#line 2291 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 204 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = insert_symbol((yyvsp[0].sval)); }
#line 2297 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 206 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.sym) = NULL; }
#line 2303 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 209 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_enum((yyvsp[-3].id_list), (yyvsp[-1].sym), get_pos(arg)); }
#line 2309 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 210 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_enum), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2315 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 214 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 1, get_pos(arg)); }
#line 2321 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 218 "utils/gwion.y" /* yacc.c:1646  */
    {  (yyval.stmt) = new_stmt_gotolabel((yyvsp[-1].sym), 0, get_pos(arg)); }
#line 2327 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 222 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(arg)); }
#line 2333 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 223 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_case((yyvsp[-1].exp), get_pos(arg)); }
#line 2339 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 227 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_switch((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg));}
#line 2345 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 232 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while((yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2351 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 234 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_while((yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2357 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 236 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-3].stmt), (yyvsp[-2].stmt), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2363 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 238 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_for((yyvsp[-4].stmt), (yyvsp[-3].stmt), (yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2369 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 240 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until((yyvsp[-2].exp), (yyvsp[0].stmt), 0, get_pos(arg)); }
#line 2375 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 242 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_until((yyvsp[-2].exp), (yyvsp[-5].stmt), 1, get_pos(arg)); }
#line 2381 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 244 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_loop((yyvsp[-2].exp), (yyvsp[0].stmt), get_pos(arg)); }
#line 2387 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 249 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-2].exp), (yyvsp[0].stmt), NULL, get_pos(arg)); }
#line 2393 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 251 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_if((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt), get_pos(arg)); }
#line 2399 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 255 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return(NULL, get_pos(arg)); }
#line 2405 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 256 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_return((yyvsp[-1].exp), get_pos(arg)); }
#line 2411 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 257 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_break(get_pos(arg)); }
#line 2417 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 258 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_continue(get_pos(arg)); }
#line 2423 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 262 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp((yyvsp[-1].exp),   get_pos(arg)); }
#line 2429 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 263 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_exp(NULL, get_pos(arg)); }
#line 2435 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 268 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = prepend_exp((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 2441 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 273 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2447 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 276 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = NULL; }
#line 2453 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 276 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = (yyvsp[-1].type_list); }
#line 2459 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 278 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_chuck; }
#line 2465 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 278 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_unchuck; }
#line 2471 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 278 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2477 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_at_chuck; }
#line 2483 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 279 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus_chuck; }
#line 2489 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus_chuck; }
#line 2495 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 280 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times_chuck; }
#line 2501 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide_chuck; }
#line 2507 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 281 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_modulo_chuck; }
#line 2513 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_trig; }
#line 2519 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 282 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_untrig; }
#line 2525 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsl; }
#line 2531 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsr; }
#line 2537 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 283 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsand; }
#line 2543 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsor; }
#line 2549 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 284 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_rsxor; }
#line 2555 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 285 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_assign; }
#line 2561 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 289 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub((yyvsp[-1].exp), get_pos(arg)); }
#line 2567 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 290 "utils/gwion.y" /* yacc.c:1646  */
    { if((yyvsp[-2].exp)->next){ gwion_error(arg, "invalid format for array init [...][...]..."); free_exp((yyvsp[-2].exp)); free_array_sub((yyvsp[0].array_sub)); YYERROR; } (yyval.array_sub) = prepend_array_sub((yyvsp[0].array_sub), (yyvsp[-2].exp)); }
#line 2573 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 291 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [...][]..."); free_exp((yyvsp[-3].exp)); YYERROR; }
#line 2579 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 295 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = new_array_sub(NULL, get_pos(arg)); }
#line 2585 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 296 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.array_sub) = prepend_array_sub((yyvsp[-2].array_sub), NULL); }
#line 2591 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 297 "utils/gwion.y" /* yacc.c:1646  */
    { gwion_error(arg, "partially empty array init [][...]"); free_array_sub((yyvsp[-1].array_sub)); free_array_sub((yyvsp[0].array_sub)); YYERROR; }
#line 2597 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 302 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp)= new_exp_decl((yyvsp[-1].type_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2603 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 303 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[0].exp); (yyval.exp)->d.exp_decl.types = (yyvsp[-2].type_list); }
#line 2609 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 305 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.type, ae_flag_static); (yyval.exp) = (yyvsp[0].exp); }
#line 2615 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 107:
#line 307 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].exp)->d.exp_decl.type, ae_flag_private); (yyval.exp) = (yyvsp[0].exp); }
#line 2621 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 108:
#line 311 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = NULL; }
#line 2627 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 109:
#line 312 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.arg_list) = (yyvsp[-1].arg_list); }
#line 2633 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 110:
#line 315 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.id_list) = (yyvsp[-1].id_list); }
#line 2639 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 111:
#line 319 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def((yyvsp[-5].ival) | (yyvsp[-4].ival), (yyvsp[-3].type_decl), (yyvsp[-2].sym), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2645 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 112:
#line 321 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].func_def), ae_flag_private); (yyval.func_def) = (yyvsp[0].func_def); }
#line 2651 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 113:
#line 323 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_TEMPLATE(arg, (yyvsp[-1].id_list), (yyvsp[0].func_def), free_func_def);
      (yyval.func_def) = (yyvsp[0].func_def); SET_FLAG((yyval.func_def), ae_flag_template); }
#line 2658 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 121:
#line 330 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_static | ae_flag_op , (yyvsp[-2].type_decl), OP_SYM((yyvsp[-3].ival)), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2664 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 122:
#line 332 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_static | ae_flag_op | ae_flag_unary, (yyvsp[-2].type_decl), OP_SYM((yyvsp[-4].ival)), (yyvsp[-1].arg_list), (yyvsp[0].stmt), get_pos(arg)); }
#line 2670 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 123:
#line 334 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.func_def) = new_func_def(ae_flag_dtor, new_type_decl(new_id_list(insert_symbol("void"), get_pos(arg)), 0,
      get_pos(arg)), insert_symbol("dtor"), NULL, (yyvsp[0].stmt), get_pos(arg)); }
#line 2677 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 124:
#line 338 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 2683 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 125:
#line 338 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 2689 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 126:
#line 341 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl(new_id_list((yyvsp[-1].sym), get_pos(arg)), (yyvsp[0].ival), get_pos(arg)); }
#line 2695 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 127:
#line 342 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2701 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 128:
#line 343 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_decl) = new_type_decl2((yyvsp[-2].id_list), (yyvsp[0].ival), get_pos(arg)); }
#line 2707 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 129:
#line 344 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (yyvsp[0].type_decl), ae_flag_const); (yyval.type_decl) = (yyvsp[0].type_decl); }
#line 2713 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 130:
#line 348 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-1].exp), NULL); }
#line 2719 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 131:
#line 349 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.decl_list) = new_decl_list((yyvsp[-2].exp), (yyvsp[0].decl_list)); }
#line 2725 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 132:
#line 353 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.stmt) = new_stmt_union((yyvsp[-3].decl_list), get_pos(arg));(yyval.stmt)->d.stmt_union.xid = (yyvsp[-1].sym); }
#line 2731 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 133:
#line 355 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_static); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2737 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 134:
#line 357 "utils/gwion.y" /* yacc.c:1646  */
    { CHECK_FLAG(arg, (&(yyvsp[0].stmt)->d.stmt_union), ae_flag_private); (yyval.stmt) = (yyvsp[0].stmt); }
#line 2743 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 135:
#line 361 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[0].var_decl), NULL, get_pos(arg)); }
#line 2749 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 136:
#line 362 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl_list) = new_var_decl_list((yyvsp[-2].var_decl), (yyvsp[0].var_decl_list), get_pos(arg)); }
#line 2755 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 137:
#line 366 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[0].sym), NULL, get_pos(arg)); }
#line 2761 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 138:
#line 367 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2767 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 139:
#line 368 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.var_decl) = new_var_decl((yyvsp[-1].sym),   (yyvsp[0].array_sub), get_pos(arg)); }
#line 2773 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 140:
#line 371 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.c_val) = new_complex((yyvsp[-1].exp), get_pos(arg)); }
#line 2779 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 141:
#line 372 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.polar) = new_polar(  (yyvsp[-1].exp), get_pos(arg)); }
#line 2785 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 142:
#line 373 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.vec) = new_vec(    (yyvsp[-1].exp), get_pos(arg)); }
#line 2791 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 144:
#line 376 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_if((yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 2797 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 146:
#line 379 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_or, (yyvsp[0].exp), get_pos(arg)); }
#line 2803 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 148:
#line 382 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_and, (yyvsp[0].exp), get_pos(arg)); }
#line 2809 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 150:
#line 385 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_or, (yyvsp[0].exp), get_pos(arg)); }
#line 2815 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 152:
#line 388 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_xor, (yyvsp[0].exp), get_pos(arg)); }
#line 2821 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 154:
#line 391 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), op_s_and, (yyvsp[0].exp), get_pos(arg)); }
#line 2827 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 155:
#line 393 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_eq; }
#line 2833 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 156:
#line 393 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_neq; }
#line 2839 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 158:
#line 396 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2845 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 159:
#line 398 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_lt; }
#line 2851 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 160:
#line 398 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_gt; }
#line 2857 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 161:
#line 399 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_le; }
#line 2863 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 162:
#line 399 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_ge; }
#line 2869 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 164:
#line 402 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2875 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 165:
#line 405 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_left;  }
#line 2881 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 166:
#line 406 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_shift_right; }
#line 2887 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 168:
#line 410 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2893 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 169:
#line 412 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2899 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 170:
#line 412 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 2905 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 172:
#line 415 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2911 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 173:
#line 417 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 2917 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 174:
#line 417 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_divide; }
#line 2923 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 175:
#line 417 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_percent; }
#line 2929 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 177:
#line 420 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_binary((yyvsp[-2].exp), (yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2935 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 179:
#line 423 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_cast((yyvsp[0].type_decl), (yyvsp[-2].exp), get_pos(arg)); }
#line 2941 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 180:
#line 425 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plus; }
#line 2947 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 181:
#line 425 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minus; }
#line 2953 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 182:
#line 425 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_times; }
#line 2959 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 183:
#line 426 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 2965 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 184:
#line 426 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 2971 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 185:
#line 427 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_exclamation; }
#line 2977 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 186:
#line 427 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_spork; }
#line 2983 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 188:
#line 431 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary((yyvsp[-1].ival), (yyvsp[0].exp), get_pos(arg)); }
#line 2989 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 189:
#line 433 "utils/gwion.y" /* yacc.c:1646  */
    {
      if((yyvsp[0].type_decl)->array && !(yyvsp[0].type_decl)->array->exp_list) {
        free_type_decl((yyvsp[0].type_decl));
        gwion_error(arg, "can't use empty '[]' in 'new' expression");
        YYERROR;
      }
      (yyval.exp) = new_exp_unary2(op_new, (yyvsp[0].type_decl), NULL, get_pos(arg));
    }
#line 3002 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 190:
#line 442 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_unary3(op_spork, (yyvsp[0].stmt), get_pos(arg)); }
#line 3008 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 192:
#line 445 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dur((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); }
#line 3014 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 193:
#line 448 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[0].sym), get_pos(arg)), NULL, get_pos(arg)); }
#line 3020 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 194:
#line 449 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.type_list) = new_type_list(new_id_list((yyvsp[-2].sym), get_pos(arg)), (yyvsp[0].type_list), get_pos(arg)); }
#line 3026 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 195:
#line 452 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = NULL; }
#line 3032 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 196:
#line 452 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 3038 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 197:
#line 454 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_plusplus; }
#line 3044 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 198:
#line 454 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.ival) = op_minusminus; }
#line 3050 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 200:
#line 457 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_array((yyvsp[-1].exp), (yyvsp[0].array_sub), get_pos(arg)); }
#line 3056 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 201:
#line 459 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_call((yyvsp[-2].exp), (yyvsp[0].exp), get_pos(arg)); (yyval.exp)->d.exp_func.types = (yyvsp[-1].type_list); }
#line 3062 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 202:
#line 461 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_dot((yyvsp[-2].exp), (yyvsp[0].sym), get_pos(arg)); }
#line 3068 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 203:
#line 463 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_post((yyvsp[-1].exp), (yyvsp[0].ival), get_pos(arg)); }
#line 3074 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 204:
#line 467 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_id(     (yyvsp[0].sym), get_pos(arg)); }
#line 3080 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 205:
#line 468 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_int(    (yyvsp[0].ival), get_pos(arg)); }
#line 3086 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 206:
#line 469 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_float(  (yyvsp[0].fval), get_pos(arg)); }
#line 3092 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 207:
#line 470 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_string( (yyvsp[0].sval), get_pos(arg)); }
#line 3098 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 208:
#line 471 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_char(   (yyvsp[0].sval), get_pos(arg)); }
#line 3104 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 209:
#line 472 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3110 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 210:
#line 473 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_array(  (yyvsp[0].array_sub), get_pos(arg)); }
#line 3116 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 211:
#line 474 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_complex((yyvsp[0].c_val), get_pos(arg)); }
#line 3122 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 212:
#line 475 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_polar(  (yyvsp[0].polar), get_pos(arg)); }
#line 3128 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 213:
#line 476 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_vec(    (yyvsp[0].vec), get_pos(arg)); }
#line 3134 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 214:
#line 477 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_hack(   (yyvsp[-1].exp), get_pos(arg)); }
#line 3140 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 215:
#line 478 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) =                       (yyvsp[-1].exp);                    }
#line 3146 "src/ast/parser.c" /* yacc.c:1646  */
    break;

  case 216:
#line 479 "utils/gwion.y" /* yacc.c:1646  */
    { (yyval.exp) = new_exp_prim_nil(        get_pos(arg)); }
#line 3152 "src/ast/parser.c" /* yacc.c:1646  */
    break;


#line 3156 "src/ast/parser.c" /* yacc.c:1646  */
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
#line 481 "utils/gwion.y" /* yacc.c:1906  */

