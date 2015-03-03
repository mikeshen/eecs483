/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 11 "parser.y"


/* Just like lex, the text within this first region delimited by %{ and %}
 * is assumed to be C/C++ code and will be copied verbatim to the y.tab.c
 * file ahead of the definitions of the yyparse() function. Add other header
 * file inclusions or C++ variable declarations/prototypes that are needed
 * by your code here.
 */
#include "scanner.h" // for yylex
#include "parser.h"
#include "errors.h"

void yyerror(const char *msg); // standard error-handling routine



/* Line 189 of yacc.c  */
#line 89 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_Void = 258,
     T_Bool = 259,
     T_Int = 260,
     T_Double = 261,
     T_String = 262,
     T_Class = 263,
     T_LessEqual = 264,
     T_GreaterEqual = 265,
     T_Equal = 266,
     T_NotEqual = 267,
     T_Dims = 268,
     T_And = 269,
     T_Or = 270,
     T_Null = 271,
     T_Extends = 272,
     T_This = 273,
     T_Interface = 274,
     T_Implements = 275,
     T_While = 276,
     T_For = 277,
     T_If = 278,
     T_Else = 279,
     T_Return = 280,
     T_Break = 281,
     T_New = 282,
     T_NewArray = 283,
     T_Print = 284,
     T_ReadInteger = 285,
     T_ReadLine = 286,
     T_Identifier = 287,
     T_StringConstant = 288,
     T_IntConstant = 289,
     T_DoubleConstant = 290,
     T_BoolConstant = 291,
     T_Increm = 292,
     T_Decrem = 293,
     T_Switch = 294,
     T_Case = 295,
     T_Default = 296,
     P_Else = 297,
     P_Empty = 298,
     P_EmptyDefault = 299,
     P_NonEmpty = 300,
     P_NonEmptyDefault = 301
   };
#endif
/* Tokens.  */
#define T_Void 258
#define T_Bool 259
#define T_Int 260
#define T_Double 261
#define T_String 262
#define T_Class 263
#define T_LessEqual 264
#define T_GreaterEqual 265
#define T_Equal 266
#define T_NotEqual 267
#define T_Dims 268
#define T_And 269
#define T_Or 270
#define T_Null 271
#define T_Extends 272
#define T_This 273
#define T_Interface 274
#define T_Implements 275
#define T_While 276
#define T_For 277
#define T_If 278
#define T_Else 279
#define T_Return 280
#define T_Break 281
#define T_New 282
#define T_NewArray 283
#define T_Print 284
#define T_ReadInteger 285
#define T_ReadLine 286
#define T_Identifier 287
#define T_StringConstant 288
#define T_IntConstant 289
#define T_DoubleConstant 290
#define T_BoolConstant 291
#define T_Increm 292
#define T_Decrem 293
#define T_Switch 294
#define T_Case 295
#define T_Default 296
#define P_Else 297
#define P_Empty 298
#define P_EmptyDefault 299
#define P_NonEmpty 300
#define P_NonEmptyDefault 301




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 41 "parser.y"

    int integerConstant;
    bool boolConstant;
    char *stringConstant;
    double doubleConstant;
    char identifier[MaxIdentLen+1]; // +1 for terminating null
    Decl *decl;
    ClassDecl *classDecl;
    InterfaceDecl *interDecl;
    NamedType *namedType;
    VarDecl *varDecl;
    FnDecl *fnDecl;
    Type *type;
    StmtBlock *stmtBlock;
    Stmt *stmt;
    CaseStmt *caseStmt;
    DefaultStmt *defaultStmt;
    Expr *expr;
    LValue *lValue;
    Call *call;
    List<Decl*> *declList;
    List<VarDecl*> *formals;
    List<Stmt*> *stmts;
    List<CaseStmt*> *cases;
    List<Expr*> *exprs;
    List<NamedType*> *implements;



/* Line 214 of yacc.c  */
#line 247 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 272 "y.tab.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  21
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   724

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  69
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  108
/* YYNRULES -- Number of states.  */
#define YYNSTATES  219

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    53,     2,     2,     2,    52,    44,     2,
      65,    66,    50,    48,    63,    49,    54,    51,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    67,    64,
      46,    42,    47,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    55,     2,    68,    45,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    61,    43,    62,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    56,    57,    58,
      59,    60
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      26,    33,    36,    37,    40,    41,    45,    47,    50,    53,
      55,    57,    63,    68,    71,    74,    76,    78,    80,    82,
      84,    87,    94,   101,   104,   106,   113,   120,   122,   123,
     127,   129,   134,   138,   142,   145,   148,   150,   153,   155,
     158,   160,   163,   166,   170,   176,   185,   191,   201,   207,
     215,   218,   220,   225,   229,   233,   236,   240,   242,   244,
     245,   247,   249,   251,   253,   257,   261,   265,   269,   273,
     277,   281,   284,   288,   292,   296,   300,   304,   308,   312,
     316,   319,   323,   327,   332,   339,   342,   345,   347,   351,
     356,   361,   368,   370,   371,   373,   375,   377,   379
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      70,     0,    -1,    71,    -1,    71,    72,    -1,    72,    -1,
      79,    -1,    82,    -1,    73,    -1,    78,    -1,     8,    32,
      74,    75,    61,    77,    62,    -1,     8,    32,    74,    75,
      61,    62,    -1,    17,    32,    -1,    -1,    20,    76,    -1,
      -1,    76,    63,    32,    -1,    32,    -1,    77,    79,    -1,
      77,    82,    -1,    79,    -1,    82,    -1,    19,    32,    61,
      83,    62,    -1,    19,    32,    61,    62,    -1,    80,    64,
      -1,    81,    32,    -1,     5,    -1,     6,    -1,     4,    -1,
       7,    -1,    32,    -1,    81,    13,    -1,    81,    32,    65,
      85,    66,    87,    -1,     3,    32,    65,    85,    66,    87,
      -1,    83,    84,    -1,    84,    -1,    81,    32,    65,    85,
      66,    64,    -1,     3,    32,    65,    85,    66,    64,    -1,
      86,    -1,    -1,    86,    63,    80,    -1,    80,    -1,    61,
      88,    89,    62,    -1,    61,    88,    62,    -1,    61,    89,
      62,    -1,    61,    62,    -1,    88,    79,    -1,    79,    -1,
      89,    90,    -1,    90,    -1,    95,    64,    -1,    87,    -1,
      26,    64,    -1,    25,    64,    -1,    25,    96,    64,    -1,
      29,    65,    94,    66,    64,    -1,    39,    65,    96,    66,
      61,    91,    93,    62,    -1,    21,    65,    96,    66,    90,
      -1,    22,    65,    95,    64,    96,    64,    95,    66,    90,
      -1,    23,    65,    96,    66,    90,    -1,    23,    65,    96,
      66,    90,    24,    90,    -1,    91,    92,    -1,    92,    -1,
      40,    34,    67,    89,    -1,    40,    34,    67,    -1,    41,
      67,    89,    -1,    41,    67,    -1,    94,    63,    96,    -1,
      96,    -1,    96,    -1,    -1,   100,    -1,    97,    -1,    18,
      -1,    98,    -1,    65,    96,    66,    -1,    97,    42,    96,
      -1,    96,    48,    96,    -1,    96,    49,    96,    -1,    96,
      50,    96,    -1,    96,    51,    96,    -1,    96,    52,    96,
      -1,    49,    96,    -1,    96,    46,    96,    -1,    96,    47,
      96,    -1,    96,     9,    96,    -1,    96,    10,    96,    -1,
      96,    11,    96,    -1,    96,    12,    96,    -1,    96,    15,
      96,    -1,    96,    14,    96,    -1,    53,    96,    -1,    30,
      65,    66,    -1,    31,    65,    66,    -1,    27,    65,    32,
      66,    -1,    28,    65,    96,    63,    81,    66,    -1,    97,
      37,    -1,    97,    38,    -1,    32,    -1,    96,    54,    32,
      -1,    96,    55,    96,    68,    -1,    32,    65,    99,    66,
      -1,    96,    54,    32,    65,    99,    66,    -1,    94,    -1,
      -1,    34,    -1,    35,    -1,    36,    -1,    33,    -1,    16,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   147,   147,   158,   159,   161,   162,   163,   164,   166,
     167,   169,   170,   172,   173,   175,   176,   178,   179,   180,
     181,   183,   184,   186,   188,   190,   191,   192,   193,   194,
     195,   197,   198,   200,   201,   202,   203,   205,   206,   208,
     209,   211,   212,   213,   214,   216,   217,   219,   220,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     234,   235,   237,   238,   240,   241,   243,   244,   246,   247,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   277,   278,   279,
     281,   282,   284,   285,   287,   288,   289,   290,   291
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_Void", "T_Bool", "T_Int", "T_Double",
  "T_String", "T_Class", "T_LessEqual", "T_GreaterEqual", "T_Equal",
  "T_NotEqual", "T_Dims", "T_And", "T_Or", "T_Null", "T_Extends", "T_This",
  "T_Interface", "T_Implements", "T_While", "T_For", "T_If", "T_Else",
  "T_Return", "T_Break", "T_New", "T_NewArray", "T_Print", "T_ReadInteger",
  "T_ReadLine", "T_Identifier", "T_StringConstant", "T_IntConstant",
  "T_DoubleConstant", "T_BoolConstant", "T_Increm", "T_Decrem", "T_Switch",
  "T_Case", "T_Default", "'='", "'|'", "'&'", "'^'", "'<'", "'>'", "'+'",
  "'-'", "'*'", "'/'", "'%'", "'!'", "'.'", "'['", "P_Else", "P_Empty",
  "P_EmptyDefault", "P_NonEmpty", "P_NonEmptyDefault", "'{'", "'}'", "','",
  "';'", "'('", "')'", "':'", "']'", "$accept", "Program", "DeclList",
  "Decl", "ClassDecl", "CExtend", "CImplement", "Implements", "Field",
  "InterDecl", "VarDecl", "Var", "Type", "FnDecl", "Prototypes",
  "Prototype", "Formals", "Vars", "StmtBlock", "VarDecls", "Stmts", "Stmt",
  "Cases", "CaseStmt", "DefaultStmt", "Exprs", "OutExpr", "Expr", "LValue",
  "Call", "Actuals", "Constant", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,    61,   124,    38,    94,    60,    62,    43,    45,
      42,    47,    37,    33,    46,    91,   297,   298,   299,   300,
     301,   123,   125,    44,    59,    40,    41,    58,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    69,    70,    71,    71,    72,    72,    72,    72,    73,
      73,    74,    74,    75,    75,    76,    76,    77,    77,    77,
      77,    78,    78,    79,    80,    81,    81,    81,    81,    81,
      81,    82,    82,    83,    83,    84,    84,    85,    85,    86,
      86,    87,    87,    87,    87,    88,    88,    89,    89,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      91,    91,    92,    92,    93,    93,    94,    94,    95,    95,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    97,    97,    97,
      98,    98,    99,    99,   100,   100,   100,   100,   100
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     7,
       6,     2,     0,     2,     0,     3,     1,     2,     2,     1,
       1,     5,     4,     2,     2,     1,     1,     1,     1,     1,
       2,     6,     6,     2,     1,     6,     6,     1,     0,     3,
       1,     4,     3,     3,     2,     2,     1,     2,     1,     2,
       1,     2,     2,     3,     5,     8,     5,     9,     5,     7,
       2,     1,     4,     3,     3,     2,     3,     1,     1,     0,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     3,     3,     4,     6,     2,     2,     1,     3,     4,
       4,     6,     1,     0,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    27,    25,    26,    28,     0,     0,    29,     0,
       2,     4,     7,     8,     5,     0,     0,     6,     0,    12,
       0,     1,     3,    23,    30,    24,    38,     0,    14,     0,
      38,    40,     0,     0,    37,    11,     0,     0,     0,    22,
       0,     0,    34,     0,    24,     0,     0,    16,    13,     0,
       0,     0,    21,    33,     0,    69,    32,    39,     0,    10,
       0,    19,    20,    38,    38,    31,   108,    72,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,   107,
     104,   105,   106,     0,     0,     0,    44,     0,    46,    50,
      69,    69,    48,     0,    68,    71,    73,    70,    15,     9,
      17,    18,     0,     0,     0,    69,     0,    97,    52,     0,
      51,     0,     0,     0,     0,     0,   103,     0,    81,    90,
       0,    42,    45,    69,    43,    47,    49,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,    67,    91,    92,   102,     0,     0,
      74,    41,    84,    85,    86,    87,    89,    88,    82,    83,
      76,    77,    78,    79,    80,    98,     0,    75,    36,    35,
      69,     0,    69,    93,     0,     0,     0,   100,     0,   103,
      99,    56,     0,    58,     0,    66,    54,     0,     0,    69,
      69,    94,     0,     0,    61,   101,     0,    59,     0,     0,
      60,     0,    69,    63,    65,    55,    57,    62,    64
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     9,    10,    11,    12,    28,    37,    48,    60,    13,
      14,    15,    32,    17,    41,    42,    33,    34,    89,    90,
      91,    92,   203,   204,   211,   157,    93,    94,    95,    96,
     158,    97
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -103
static const yytype_int16 yypact[] =
{
     504,   -24,  -103,  -103,  -103,  -103,    -2,    16,  -103,    22,
     504,  -103,  -103,  -103,  -103,   -32,     2,  -103,   -12,    57,
      18,  -103,  -103,  -103,  -103,    15,     7,    59,    73,    20,
       7,  -103,     3,    36,    46,  -103,    79,    55,    80,  -103,
       4,    51,  -103,    52,  -103,    56,     7,  -103,    58,    82,
      70,    72,  -103,  -103,    56,   125,  -103,  -103,    88,  -103,
     101,  -103,  -103,     7,     7,  -103,  -103,  -103,    74,    77,
      84,   586,    98,   100,   102,   103,   104,   105,    71,  -103,
    -103,  -103,  -103,   106,   610,   610,  -103,   610,  -103,  -103,
     175,   416,  -103,   108,   643,    39,  -103,  -103,  -103,  -103,
    -103,  -103,   107,   110,   610,   610,   610,   112,  -103,   326,
    -103,    92,   610,   610,   117,   119,   610,   610,    44,   -10,
     232,  -103,  -103,   464,  -103,  -103,  -103,   610,   610,   610,
     610,   610,   610,   610,   610,   610,   610,   610,   610,   610,
     134,   610,  -103,  -103,   610,   124,   128,   253,   130,   279,
    -103,   123,   373,   -59,   643,  -103,  -103,    75,   129,   300,
    -103,  -103,   669,   669,   358,   358,   655,   622,   669,   669,
      44,    44,   -10,   -10,   -10,   147,   206,   643,  -103,  -103,
     562,   610,   562,  -103,     7,   610,   135,  -103,   152,   610,
    -103,  -103,   347,   195,    -7,   643,  -103,   182,   157,   610,
     562,  -103,   191,    60,  -103,  -103,   160,  -103,   162,   163,
    -103,   165,   562,   512,   512,  -103,  -103,   512,   512
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -103,  -103,  -103,   221,  -103,  -103,  -103,  -103,  -103,  -103,
      85,   438,     0,   -29,  -103,   192,   -21,  -103,   -17,  -103,
     -88,   -90,  -103,    29,  -103,   121,  -102,   -66,  -103,  -103,
      49,  -103
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -70
static const yytype_int16 yytable[] =
{
      16,   125,   123,   148,   185,   109,    24,   186,    18,    43,
      16,     2,     3,     4,     5,    24,    24,    24,   118,   119,
      62,   120,    21,    38,     2,     3,     4,     5,    56,    40,
      19,   101,    23,   125,    25,    44,    51,    65,   147,     8,
     149,    40,   102,   103,   140,   141,   152,   154,    20,    16,
     154,   159,     8,    26,    38,     2,     3,     4,     5,   201,
      16,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,    27,   176,   142,   143,   177,    29,
      30,   144,    39,     8,   -29,     1,     2,     3,     4,     5,
     191,    35,   193,    36,   137,   138,   139,   206,   140,   141,
     202,   209,    45,   -29,     1,     2,     3,     4,     5,    46,
     207,    47,    50,    52,     8,   192,    49,    55,    54,   195,
      98,    58,   216,   154,   151,   217,   218,   125,   125,     2,
       3,     4,     5,     8,    61,    63,   116,    64,   185,   104,
      88,    66,   105,    67,    59,   100,    68,    69,    70,   106,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,   110,    99,    83,   111,   175,   112,   113,   114,
     115,   117,   126,   145,    84,   122,   146,   116,    85,     2,
       3,     4,     5,   155,   194,   156,    55,    86,   178,   183,
      87,    66,   179,    67,   181,   187,    68,    69,    70,   196,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,   189,   197,    83,   127,   128,   129,   130,   200,
     131,   132,   202,   205,    84,   208,   212,   215,    85,   213,
     214,    22,   210,    53,   153,     0,    55,   121,   198,     0,
      87,   127,   128,   129,   130,     0,   131,   132,     0,     0,
       0,     0,   133,   134,   135,   136,   137,   138,   139,     0,
     140,   141,   127,   128,   129,   130,     0,   131,   132,     0,
       0,     0,     0,     0,   190,     0,     0,     0,   133,   134,
     135,   136,   137,   138,   139,     0,   140,   141,   127,   128,
     129,   130,     0,   131,   132,     0,     0,     0,   160,   133,
     134,   135,   136,   137,   138,   139,     0,   140,   141,   127,
     128,   129,   130,     0,   131,   132,     0,     0,     0,   180,
       0,     0,     0,     0,     0,   133,   134,   135,   136,   137,
     138,   139,     0,   140,   141,   127,   128,   129,   130,     0,
     131,   132,     0,     0,     0,   182,   133,   134,   135,   136,
     137,   138,   139,     0,   140,   141,   127,   128,   129,   130,
       0,   131,   132,     0,     0,     0,   188,   127,   128,   -70,
     -70,     0,   133,   134,   135,   136,   137,   138,   139,     0,
     140,   141,   127,   128,   129,   130,     0,   131,   132,     0,
     150,     0,     0,   133,   134,   135,   136,   137,   138,   139,
       0,   140,   141,     0,   133,   134,   135,   136,   137,   138,
     139,   199,   140,   141,     0,     0,     0,     0,     0,   133,
     134,   135,   136,   137,   138,   139,     0,   140,   141,     0,
       0,     0,    66,     0,    67,     0,   184,    68,    69,    70,
       0,    71,    72,    73,    74,    75,    76,    77,   107,    79,
      80,    81,    82,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,     0,    31,    84,     0,     0,    31,    85,
       0,     0,     0,     0,     0,     0,     0,    55,   124,     0,
      66,    87,    67,     0,    57,    68,    69,    70,     0,    71,
      72,    73,    74,    75,    76,    77,   107,    79,    80,    81,
      82,    31,    31,    83,     0,     0,     0,     1,     2,     3,
       4,     5,     6,    84,     0,     0,     0,    85,     0,     0,
       0,     0,     0,     7,     0,    55,   161,     0,    66,    87,
      67,     0,     0,    68,    69,    70,     8,    71,    72,    73,
      74,    75,    76,    77,   107,    79,    80,    81,    82,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    84,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    55,     0,     0,   -69,    87,    66,     0,
      67,     0,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,   107,    79,    80,    81,    82,     0,
       0,    83,    66,     0,    67,     0,     0,     0,     0,     0,
       0,    84,     0,    73,    74,    85,    76,    77,   107,    79,
      80,    81,    82,    55,     0,     0,    66,    87,    67,     0,
       0,   127,   128,   129,   130,    84,   131,    73,    74,    85,
      76,    77,   107,    79,    80,    81,    82,     0,     0,     0,
     108,    87,   127,   128,   129,   130,     0,   131,   132,    84,
       0,     0,     0,    85,   127,   128,   129,   130,   133,   134,
     135,   136,   137,   138,   139,    87,   140,   141,   -70,   -70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   133,
     134,   135,   136,   137,   138,   139,     0,   140,   141,     0,
       0,   133,   134,   135,   136,   137,   138,   139,     0,   140,
     141,     0,     0,     0,     0,   -70,   -70,   135,   136,   137,
     138,   139,     0,   140,   141
};

static const yytype_int16 yycheck[] =
{
       0,    91,    90,   105,    63,    71,    13,    66,    32,    30,
      10,     4,     5,     6,     7,    13,    13,    13,    84,    85,
      49,    87,     0,     3,     4,     5,     6,     7,    45,    29,
      32,    60,    64,   123,    32,    32,    32,    54,   104,    32,
     106,    41,    63,    64,    54,    55,   112,   113,    32,    49,
     116,   117,    32,    65,     3,     4,     5,     6,     7,    66,
      60,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    17,   141,    37,    38,   144,    61,
      65,    42,    62,    32,    13,     3,     4,     5,     6,     7,
     180,    32,   182,    20,    50,    51,    52,   199,    54,    55,
      40,    41,    66,    32,     3,     4,     5,     6,     7,    63,
     200,    32,    32,    62,    32,   181,    61,    61,    66,   185,
      32,    63,   212,   189,    32,   213,   214,   217,   218,     4,
       5,     6,     7,    32,    49,    65,    65,    65,    63,    65,
      55,    16,    65,    18,    62,    60,    21,    22,    23,    65,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    64,    62,    39,    65,    32,    65,    65,    65,
      65,    65,    64,    66,    49,    90,    66,    65,    53,     4,
       5,     6,     7,    66,   184,    66,    61,    62,    64,    66,
      65,    16,    64,    18,    64,    66,    21,    22,    23,    64,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    65,    61,    39,     9,    10,    11,    12,    24,
      14,    15,    40,    66,    49,    34,    66,    62,    53,    67,
      67,    10,   203,    41,   113,    -1,    61,    62,   189,    -1,
      65,     9,    10,    11,    12,    -1,    14,    15,    -1,    -1,
      -1,    -1,    46,    47,    48,    49,    50,    51,    52,    -1,
      54,    55,     9,    10,    11,    12,    -1,    14,    15,    -1,
      -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    46,    47,
      48,    49,    50,    51,    52,    -1,    54,    55,     9,    10,
      11,    12,    -1,    14,    15,    -1,    -1,    -1,    66,    46,
      47,    48,    49,    50,    51,    52,    -1,    54,    55,     9,
      10,    11,    12,    -1,    14,    15,    -1,    -1,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
      51,    52,    -1,    54,    55,     9,    10,    11,    12,    -1,
      14,    15,    -1,    -1,    -1,    66,    46,    47,    48,    49,
      50,    51,    52,    -1,    54,    55,     9,    10,    11,    12,
      -1,    14,    15,    -1,    -1,    -1,    66,     9,    10,    11,
      12,    -1,    46,    47,    48,    49,    50,    51,    52,    -1,
      54,    55,     9,    10,    11,    12,    -1,    14,    15,    -1,
      64,    -1,    -1,    46,    47,    48,    49,    50,    51,    52,
      -1,    54,    55,    -1,    46,    47,    48,    49,    50,    51,
      52,    64,    54,    55,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    49,    50,    51,    52,    -1,    54,    55,    -1,
      -1,    -1,    16,    -1,    18,    -1,    63,    21,    22,    23,
      -1,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    49,    -1,    -1,    30,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    -1,
      16,    65,    18,    -1,    46,    21,    22,    23,    -1,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    63,    64,    39,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,    49,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    61,    62,    -1,    16,    65,
      18,    -1,    -1,    21,    22,    23,    32,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    64,    65,    16,    -1,
      18,    -1,    -1,    21,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    16,    -1,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    27,    28,    53,    30,    31,    32,    33,
      34,    35,    36,    61,    -1,    -1,    16,    65,    18,    -1,
      -1,     9,    10,    11,    12,    49,    14,    27,    28,    53,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    -1,
      64,    65,     9,    10,    11,    12,    -1,    14,    15,    49,
      -1,    -1,    -1,    53,     9,    10,    11,    12,    46,    47,
      48,    49,    50,    51,    52,    65,    54,    55,     9,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    49,    50,    51,    52,    -1,    54,    55,    -1,
      -1,    46,    47,    48,    49,    50,    51,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
      51,    52,    -1,    54,    55
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    19,    32,    70,
      71,    72,    73,    78,    79,    80,    81,    82,    32,    32,
      32,     0,    72,    64,    13,    32,    65,    17,    74,    61,
      65,    80,    81,    85,    86,    32,    20,    75,     3,    62,
      81,    83,    84,    85,    32,    66,    63,    32,    76,    61,
      32,    32,    62,    84,    66,    61,    87,    80,    63,    62,
      77,    79,    82,    65,    65,    87,    16,    18,    21,    22,
      23,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    39,    49,    53,    62,    65,    79,    87,
      88,    89,    90,    95,    96,    97,    98,   100,    32,    62,
      79,    82,    85,    85,    65,    65,    65,    32,    64,    96,
      64,    65,    65,    65,    65,    65,    65,    65,    96,    96,
      96,    62,    79,    89,    62,    90,    64,     9,    10,    11,
      12,    14,    15,    46,    47,    48,    49,    50,    51,    52,
      54,    55,    37,    38,    42,    66,    66,    96,    95,    96,
      64,    32,    96,    94,    96,    66,    66,    94,    99,    96,
      66,    62,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    32,    96,    96,    64,    64,
      66,    64,    66,    66,    63,    63,    66,    66,    66,    65,
      68,    90,    96,    90,    81,    96,    64,    61,    99,    64,
      24,    66,    40,    91,    92,    66,    95,    90,    34,    41,
      92,    93,    66,    67,    67,    62,    90,    89,    89
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
  *++yyvsp = yylval;
  *++yylsp = yylloc;
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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1464 of yacc.c  */
#line 147 "parser.y"
    {
                                        (yylsp[(1) - (1)]);
                                        /* pp2: The @1 is needed to convince
                                        * yacc to set up yylloc. You can remove
                                        * it once you have other uses of @n*/
                                        Program *program = new Program((yyvsp[(1) - (1)].declList));
                                        // if no errors, advance to next phase
                                        if (ReportError::NumErrors() == 0)
                                            program->Print(0);
                                        }
    break;

  case 3:

/* Line 1464 of yacc.c  */
#line 158 "parser.y"
    { ((yyval.declList)=(yyvsp[(1) - (2)].declList))->Append((yyvsp[(2) - (2)].decl)); }
    break;

  case 4:

/* Line 1464 of yacc.c  */
#line 159 "parser.y"
    { ((yyval.declList) = new List<Decl*>)->Append((yyvsp[(1) - (1)].decl)); }
    break;

  case 5:

/* Line 1464 of yacc.c  */
#line 161 "parser.y"
    { (yyval.decl) = (yyvsp[(1) - (1)].varDecl); }
    break;

  case 6:

/* Line 1464 of yacc.c  */
#line 162 "parser.y"
    { (yyval.decl) = (yyvsp[(1) - (1)].fnDecl); }
    break;

  case 7:

/* Line 1464 of yacc.c  */
#line 163 "parser.y"
    { (yyval.decl) = (yyvsp[(1) - (1)].classDecl); }
    break;

  case 8:

/* Line 1464 of yacc.c  */
#line 164 "parser.y"
    { (yyval.decl) = (yyvsp[(1) - (1)].interDecl); }
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 166 "parser.y"
    { (yyval.classDecl) = new ClassDecl(new Identifier((yylsp[(2) - (7)]), (yyvsp[(2) - (7)].identifier)), (yyvsp[(3) - (7)].namedType), (yyvsp[(4) - (7)].implements), (yyvsp[(6) - (7)].declList)); }
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 167 "parser.y"
    { (yyval.classDecl) = new ClassDecl(new Identifier((yylsp[(2) - (6)]), (yyvsp[(2) - (6)].identifier)), (yyvsp[(3) - (6)].namedType), (yyvsp[(4) - (6)].implements), new List<Decl*>); }
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 169 "parser.y"
    { (yyval.namedType) = new NamedType(new Identifier((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].identifier))); }
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 170 "parser.y"
    { (yyval.namedType) = nullptr; }
    break;

  case 13:

/* Line 1464 of yacc.c  */
#line 172 "parser.y"
    { (yyval.implements) = (yyvsp[(2) - (2)].implements); }
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 173 "parser.y"
    { (yyval.implements) = new List<NamedType*>; }
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 175 "parser.y"
    { ((yyval.implements)=(yyvsp[(1) - (3)].implements))->Append(new NamedType(new Identifier((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].identifier)))); }
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 176 "parser.y"
    { ((yyval.implements) = new List<NamedType*>)->Append(new NamedType(new Identifier((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].identifier)))); }
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 178 "parser.y"
    { ((yyval.declList)=(yyvsp[(1) - (2)].declList))->Append((yyvsp[(2) - (2)].varDecl)); }
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 179 "parser.y"
    { ((yyval.declList)=(yyvsp[(1) - (2)].declList))->Append((yyvsp[(2) - (2)].fnDecl)); }
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 180 "parser.y"
    { ((yyval.declList)=new List<Decl*>)->Append((yyvsp[(1) - (1)].varDecl)); }
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 181 "parser.y"
    { ((yyval.declList)=new List<Decl*>)->Append((yyvsp[(1) - (1)].fnDecl)); }
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 183 "parser.y"
    { (yyval.interDecl) = new InterfaceDecl(new Identifier((yylsp[(2) - (5)]), (yyvsp[(2) - (5)].identifier)), (yyvsp[(4) - (5)].declList)); }
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 184 "parser.y"
    { (yyval.interDecl) = new InterfaceDecl(new Identifier((yylsp[(2) - (4)]), (yyvsp[(2) - (4)].identifier)), new List<Decl*>); }
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 186 "parser.y"
    { (yyval.varDecl) = (yyvsp[(1) - (2)].varDecl); }
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 188 "parser.y"
    { (yyval.varDecl) = new VarDecl(new Identifier((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].identifier)), (yyvsp[(1) - (2)].type)); }
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 190 "parser.y"
    { (yyval.type) = Type::intType; }
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 191 "parser.y"
    { (yyval.type) = Type::doubleType; }
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 192 "parser.y"
    { (yyval.type) = Type::boolType; }
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 193 "parser.y"
    { (yyval.type) = Type::stringType; }
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 194 "parser.y"
    { (yyval.type) = new NamedType(new Identifier((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].identifier))); }
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 195 "parser.y"
    { (yyval.type) = new ArrayType(Join((yylsp[(1) - (2)]), (yylsp[(2) - (2)])), (yyvsp[(1) - (2)].type)); }
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 197 "parser.y"
    { ((yyval.fnDecl) = new FnDecl(new Identifier((yylsp[(2) - (6)]), (yyvsp[(2) - (6)].identifier)), (yyvsp[(1) - (6)].type), (yyvsp[(4) - (6)].formals)))->SetFunctionBody((yyvsp[(6) - (6)].stmtBlock)); }
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 198 "parser.y"
    { ((yyval.fnDecl) = new FnDecl(new Identifier((yylsp[(2) - (6)]), (yyvsp[(2) - (6)].identifier)), Type::voidType, (yyvsp[(4) - (6)].formals)))->SetFunctionBody((yyvsp[(6) - (6)].stmtBlock)); }
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 200 "parser.y"
    { ((yyval.declList) = (yyvsp[(1) - (2)].declList))->Append((yyvsp[(2) - (2)].fnDecl));}
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 201 "parser.y"
    { ((yyval.declList) = new List<Decl*>)->Append((yyvsp[(1) - (1)].fnDecl));}
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 202 "parser.y"
    { (yyval.fnDecl) = new FnDecl(new Identifier((yylsp[(2) - (6)]), (yyvsp[(2) - (6)].identifier)), (yyvsp[(1) - (6)].type), (yyvsp[(4) - (6)].formals)); }
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 203 "parser.y"
    { (yyval.fnDecl) = new FnDecl(new Identifier((yylsp[(2) - (6)]), (yyvsp[(2) - (6)].identifier)), Type::voidType, (yyvsp[(4) - (6)].formals)); }
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 205 "parser.y"
    { (yyval.formals) = (yyvsp[(1) - (1)].formals);}
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 206 "parser.y"
    { (yyval.formals) = new List<VarDecl*>; }
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 208 "parser.y"
    { ((yyval.formals)=(yyvsp[(1) - (3)].formals))->Append((yyvsp[(3) - (3)].varDecl)); }
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 209 "parser.y"
    { ((yyval.formals) = new List<VarDecl*>)->Append((yyvsp[(1) - (1)].varDecl)); }
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 211 "parser.y"
    { (yyval.stmtBlock) = new StmtBlock((yyvsp[(2) - (4)].formals), (yyvsp[(3) - (4)].stmts)); }
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 212 "parser.y"
    { (yyval.stmtBlock) = new StmtBlock((yyvsp[(2) - (3)].formals), new List<Stmt*>); }
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 213 "parser.y"
    { (yyval.stmtBlock) = new StmtBlock(new List<VarDecl*>, (yyvsp[(2) - (3)].stmts)); }
    break;

  case 44:

/* Line 1464 of yacc.c  */
#line 214 "parser.y"
    { (yyval.stmtBlock) = new StmtBlock(new List<VarDecl*>, new List<Stmt*>); }
    break;

  case 45:

/* Line 1464 of yacc.c  */
#line 216 "parser.y"
    { ((yyval.formals) = (yyvsp[(1) - (2)].formals))->Append((yyvsp[(2) - (2)].varDecl)); }
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 217 "parser.y"
    { ((yyval.formals) = new List<VarDecl*>)->Append((yyvsp[(1) - (1)].varDecl)); }
    break;

  case 47:

/* Line 1464 of yacc.c  */
#line 219 "parser.y"
    { ((yyval.stmts) = (yyvsp[(1) - (2)].stmts))->Append((yyvsp[(2) - (2)].stmt)); }
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 220 "parser.y"
    { ((yyval.stmts) = new List<Stmt*>)->Append((yyvsp[(1) - (1)].stmt)); }
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 222 "parser.y"
    { (yyval.stmt) = (yyvsp[(1) - (2)].expr); }
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 223 "parser.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmtBlock); }
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 224 "parser.y"
    { (yyval.stmt) = new BreakStmt((yylsp[(1) - (2)])); }
    break;

  case 52:

/* Line 1464 of yacc.c  */
#line 225 "parser.y"
    { (yyval.stmt) = new ReturnStmt((yylsp[(1) - (2)]), new EmptyExpr); }
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 226 "parser.y"
    { (yyval.stmt) = new ReturnStmt((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].expr)); }
    break;

  case 54:

/* Line 1464 of yacc.c  */
#line 227 "parser.y"
    { (yyval.stmt) = new PrintStmt((yyvsp[(3) - (5)].exprs)); }
    break;

  case 55:

/* Line 1464 of yacc.c  */
#line 228 "parser.y"
    { (yyval.stmt) = new SwitchStmt((yyvsp[(3) - (8)].expr), (yyvsp[(6) - (8)].cases), (yyvsp[(7) - (8)].defaultStmt)); }
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 229 "parser.y"
    { (yyval.stmt) = new WhileStmt((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt)); }
    break;

  case 57:

/* Line 1464 of yacc.c  */
#line 230 "parser.y"
    { (yyval.stmt) = new ForStmt((yyvsp[(3) - (9)].expr), (yyvsp[(5) - (9)].expr), (yyvsp[(7) - (9)].expr), (yyvsp[(9) - (9)].stmt)); }
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 231 "parser.y"
    { (yyval.stmt) = new IfStmt((yyvsp[(3) - (5)].expr), (yyvsp[(5) - (5)].stmt), nullptr); }
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 232 "parser.y"
    { (yyval.stmt) = new IfStmt((yyvsp[(3) - (7)].expr), (yyvsp[(5) - (7)].stmt), (yyvsp[(7) - (7)].stmt)); }
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 234 "parser.y"
    { ((yyval.cases) = (yyvsp[(1) - (2)].cases))->Append((yyvsp[(2) - (2)].caseStmt)); }
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 235 "parser.y"
    { ((yyval.cases) = new List<CaseStmt*>)->Append((yyvsp[(1) - (1)].caseStmt)); }
    break;

  case 62:

/* Line 1464 of yacc.c  */
#line 237 "parser.y"
    { (yyval.caseStmt) = new CaseStmt(new IntConstant((yylsp[(2) - (4)]), (yyvsp[(2) - (4)].integerConstant)), (yyvsp[(4) - (4)].stmts)); }
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 238 "parser.y"
    { (yyval.caseStmt) = new CaseStmt(new IntConstant((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].integerConstant)), new List<Stmt*>); }
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 240 "parser.y"
    { (yyval.defaultStmt) = new DefaultStmt((yyvsp[(3) - (3)].stmts)); }
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 241 "parser.y"
    { (yyval.defaultStmt) = new DefaultStmt(new List<Stmt*>); }
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 243 "parser.y"
    { ((yyval.exprs) = (yyvsp[(1) - (3)].exprs))->Append((yyvsp[(3) - (3)].expr)); }
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 244 "parser.y"
    { ((yyval.exprs) = new List<Expr*>)->Append((yyvsp[(1) - (1)].expr)); }
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 246 "parser.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); }
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 247 "parser.y"
    { (yyval.expr) = new EmptyExpr(); }
    break;

  case 70:

/* Line 1464 of yacc.c  */
#line 249 "parser.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); }
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 250 "parser.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].lValue); }
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 251 "parser.y"
    { (yyval.expr) = new This((yylsp[(1) - (1)])); }
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 252 "parser.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].call); }
    break;

  case 74:

/* Line 1464 of yacc.c  */
#line 253 "parser.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); }
    break;

  case 75:

/* Line 1464 of yacc.c  */
#line 254 "parser.y"
    { (yyval.expr) = new AssignExpr((yyvsp[(1) - (3)].lValue), new Operator((yylsp[(2) - (3)]), "="), (yyvsp[(3) - (3)].expr)); }
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 255 "parser.y"
    { (yyval.expr) = new ArithmeticExpr((yyvsp[(1) - (3)].expr), new Operator((yylsp[(2) - (3)]), "+"), (yyvsp[(3) - (3)].expr)); }
    break;

  case 77:

/* Line 1464 of yacc.c  */
#line 256 "parser.y"
    { (yyval.expr) = new ArithmeticExpr((yyvsp[(1) - (3)].expr), new Operator((yylsp[(2) - (3)]), "-"), (yyvsp[(3) - (3)].expr)); }
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 257 "parser.y"
    { (yyval.expr) = new ArithmeticExpr((yyvsp[(1) - (3)].expr), new Operator((yylsp[(2) - (3)]), "*"), (yyvsp[(3) - (3)].expr)); }
    break;

  case 79:

/* Line 1464 of yacc.c  */
#line 258 "parser.y"
    { (yyval.expr) = new ArithmeticExpr((yyvsp[(1) - (3)].expr), new Operator((yylsp[(2) - (3)]), "/"), (yyvsp[(3) - (3)].expr)); }
    break;

  case 80:

/* Line 1464 of yacc.c  */
#line 259 "parser.y"
    { (yyval.expr) = new ArithmeticExpr((yyvsp[(1) - (3)].expr), new Operator((yylsp[(2) - (3)]), "%"), (yyvsp[(3) - (3)].expr)); }
    break;

  case 81:

/* Line 1464 of yacc.c  */
#line 260 "parser.y"
    { (yyval.expr) = new ArithmeticExpr(new Operator((yylsp[(1) - (2)]), "-"), (yyvsp[(2) - (2)].expr)); }
    break;

  case 82:

/* Line 1464 of yacc.c  */
#line 261 "parser.y"
    { (yyval.expr) = new RelationalExpr((yyvsp[(1) - (3)].expr), new Operator((yylsp[(2) - (3)]), "<"), (yyvsp[(3) - (3)].expr)); }
    break;

  case 83:

/* Line 1464 of yacc.c  */
#line 262 "parser.y"
    { (yyval.expr) = new RelationalExpr((yyvsp[(1) - (3)].expr), new Operator((yylsp[(2) - (3)]), ">"), (yyvsp[(3) - (3)].expr)); }
    break;

  case 84:

/* Line 1464 of yacc.c  */
#line 263 "parser.y"
    { (yyval.expr) = new RelationalExpr((yyvsp[(1) - (3)].expr), new Operator((yylsp[(2) - (3)]), "<="), (yyvsp[(3) - (3)].expr)); }
    break;

  case 85:

/* Line 1464 of yacc.c  */
#line 264 "parser.y"
    { (yyval.expr) = new RelationalExpr((yyvsp[(1) - (3)].expr), new Operator((yylsp[(2) - (3)]), ">="), (yyvsp[(3) - (3)].expr)); }
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 265 "parser.y"
    { (yyval.expr) = new EqualityExpr((yyvsp[(1) - (3)].expr), new Operator((yylsp[(2) - (3)]), "=="), (yyvsp[(3) - (3)].expr)); }
    break;

  case 87:

/* Line 1464 of yacc.c  */
#line 266 "parser.y"
    { (yyval.expr) = new EqualityExpr((yyvsp[(1) - (3)].expr), new Operator((yylsp[(2) - (3)]), "!="), (yyvsp[(3) - (3)].expr)); }
    break;

  case 88:

/* Line 1464 of yacc.c  */
#line 267 "parser.y"
    { (yyval.expr) = new LogicalExpr((yyvsp[(1) - (3)].expr), new Operator((yylsp[(2) - (3)]), "||"), (yyvsp[(3) - (3)].expr)); }
    break;

  case 89:

/* Line 1464 of yacc.c  */
#line 268 "parser.y"
    { (yyval.expr) = new LogicalExpr((yyvsp[(1) - (3)].expr), new Operator((yylsp[(2) - (3)]), "&&"), (yyvsp[(3) - (3)].expr)); }
    break;

  case 90:

/* Line 1464 of yacc.c  */
#line 269 "parser.y"
    { (yyval.expr) = new LogicalExpr(new Operator((yylsp[(1) - (2)]), "!"), (yyvsp[(2) - (2)].expr)); }
    break;

  case 91:

/* Line 1464 of yacc.c  */
#line 270 "parser.y"
    { (yyval.expr) = new ReadIntegerExpr(Join((yylsp[(1) - (3)]), (yylsp[(3) - (3)]))); }
    break;

  case 92:

/* Line 1464 of yacc.c  */
#line 271 "parser.y"
    { (yyval.expr) = new ReadLineExpr(Join((yylsp[(1) - (3)]), (yylsp[(3) - (3)]))); }
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 272 "parser.y"
    { (yyval.expr) = new NewExpr(Join((yylsp[(1) - (4)]), (yylsp[(4) - (4)])), new NamedType(new Identifier((yylsp[(3) - (4)]), (yyvsp[(3) - (4)].identifier)))); }
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 273 "parser.y"
    { (yyval.expr) = new NewArrayExpr(Join((yylsp[(1) - (6)]), (yylsp[(6) - (6)])), (yyvsp[(3) - (6)].expr), (yyvsp[(5) - (6)].type)); }
    break;

  case 95:

/* Line 1464 of yacc.c  */
#line 274 "parser.y"
    { (yyval.expr) = new PostFixExpr((yyvsp[(1) - (2)].lValue), new Operator((yylsp[(2) - (2)]), "++")); }
    break;

  case 96:

/* Line 1464 of yacc.c  */
#line 275 "parser.y"
    { (yyval.expr) = new PostFixExpr((yyvsp[(1) - (2)].lValue), new Operator((yylsp[(2) - (2)]), "--")); }
    break;

  case 97:

/* Line 1464 of yacc.c  */
#line 277 "parser.y"
    { (yyval.lValue) = new FieldAccess(nullptr, new Identifier((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].identifier))); }
    break;

  case 98:

/* Line 1464 of yacc.c  */
#line 278 "parser.y"
    { (yyval.lValue) = new FieldAccess((yyvsp[(1) - (3)].expr), new Identifier((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].identifier))); }
    break;

  case 99:

/* Line 1464 of yacc.c  */
#line 279 "parser.y"
    { (yyval.lValue) = new ArrayAccess(Join((yylsp[(1) - (4)]), (yylsp[(4) - (4)])), (yyvsp[(1) - (4)].expr), (yyvsp[(3) - (4)].expr));}
    break;

  case 100:

/* Line 1464 of yacc.c  */
#line 281 "parser.y"
    { (yyval.call) = new Call(Join((yylsp[(1) - (4)]), (yylsp[(4) - (4)])), nullptr, new Identifier((yylsp[(1) - (4)]), (yyvsp[(1) - (4)].identifier)), (yyvsp[(3) - (4)].exprs)); }
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 282 "parser.y"
    { (yyval.call) = new Call(Join((yylsp[(1) - (6)]), (yylsp[(6) - (6)])), (yyvsp[(1) - (6)].expr), new Identifier((yylsp[(3) - (6)]), (yyvsp[(3) - (6)].identifier)), (yyvsp[(5) - (6)].exprs)); }
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 284 "parser.y"
    { (yyval.exprs) = (yyvsp[(1) - (1)].exprs); }
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 285 "parser.y"
    { (yyval.exprs) = new List<Expr*>; }
    break;

  case 104:

/* Line 1464 of yacc.c  */
#line 287 "parser.y"
    { (yyval.expr) = new IntConstant((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].integerConstant)); }
    break;

  case 105:

/* Line 1464 of yacc.c  */
#line 288 "parser.y"
    { (yyval.expr) = new DoubleConstant((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].doubleConstant)); }
    break;

  case 106:

/* Line 1464 of yacc.c  */
#line 289 "parser.y"
    { (yyval.expr) = new BoolConstant((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].boolConstant)); }
    break;

  case 107:

/* Line 1464 of yacc.c  */
#line 290 "parser.y"
    { (yyval.expr) = new StringConstant((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].stringConstant)); }
    break;

  case 108:

/* Line 1464 of yacc.c  */
#line 291 "parser.y"
    { (yyval.expr) = new NullConstant((yylsp[(1) - (1)])); }
    break;



/* Line 1464 of yacc.c  */
#line 2569 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

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
		      yytoken, &yylval, &yylloc);
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

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1684 of yacc.c  */
#line 293 "parser.y"


/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated y.tab.c file.
 * This section is where you put definitions of helper functions.
 */

/* Function: InitParser
 * --------------------
 * This function will be called before any calls to yyparse().  It is designed
 * to give you an opportunity to do anything that must be done to initialize
 * the parser (set global variables, configure starting state, etc.). One
 * thing it already does for you is assign the value of the global variable
 * yydebug that controls whether yacc prints debugging information about
 * parser actions (shift/reduce) and contents of state stack during parser.
 * If set to false, no information is printed. Setting it to true will give
 * you a running trail that might be helpful when debugging your parser.
 * Please be sure the variable is set to false when submitting your final
 * version.
 */
void InitParser()
{
   PrintDebug("parser", "Initializing parser");
   yydebug = false;
}

