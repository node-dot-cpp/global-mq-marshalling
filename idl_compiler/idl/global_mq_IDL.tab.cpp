
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 54 "global_mq_IDL.y"

#include "global_mq_parser_helper.h"
extern int yylex();


/* Line 189 of yacc.c  */
#line 79 "global_mq_IDL.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
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
     KW_FILE = 258,
     KW_MESSAGE = 259,
     KW_PUBLISHABLE = 260,
     KW_STRUCT = 261,
     KW_NONEXTENDABLE = 262,
     KW_EXTENSION = 263,
     KW_HASH_LINE = 264,
     KW_DEFAULT = 265,
     KW_EMPTY = 266,
     KW_MAX_LENGTH = 267,
     KW_ENUM = 268,
     KW_BYTE_ARRAY = 269,
     IDENTIFIER = 270,
     KW_IDENTIFIER = 271,
     STRING_LITERAL = 272,
     INTEGER_LITERAL = 273,
     KW_INTEGER = 274,
     KW_UINTEGER = 275,
     KW_REAL = 276,
     KW_CHARACTER_STRING = 277,
     KW_BLOB = 278,
     KW_VECTOR = 279,
     KW_PROTO = 280
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 146 "global_mq_IDL.tab.c"

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
# if YYENABLE_NLS
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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   510

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  168
/* YYNRULES -- Number of states.  */
#define YYNSTATES  431

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   280

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      34,    35,     2,    38,    32,    39,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    29,    26,
      36,    27,    37,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    31,     2,    33,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    28,     2,    30,     2,     2,     2,     2,
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
      25
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    13,    16,    20,    25,
      32,    40,    48,    53,    57,    61,    68,    76,    84,    89,
      93,    97,   101,   106,   111,   115,   119,   121,   123,   125,
     127,   129,   131,   133,   135,   137,   139,   141,   143,   148,
     155,   162,   169,   176,   182,   188,   194,   200,   206,   212,
     218,   224,   234,   244,   254,   264,   273,   282,   291,   300,
     309,   318,   327,   336,   338,   343,   350,   357,   364,   371,
     377,   383,   389,   395,   401,   407,   413,   419,   429,   439,
     449,   459,   468,   477,   486,   495,   504,   513,   522,   531,
     533,   538,   545,   552,   559,   566,   572,   578,   584,   590,
     596,   602,   608,   614,   624,   634,   644,   654,   663,   672,
     681,   690,   699,   708,   717,   726,   728,   733,   738,   746,
     754,   759,   764,   769,   774,   779,   784,   790,   797,   803,
     810,   816,   823,   831,   839,   847,   855,   863,   872,   882,
     892,   901,   911,   921,   930,   940,   950,   953,   957,   961,
     964,   968,   972,   975,   979,   983,   985,   991,  1000,  1004,
    1010,  1017,  1026,  1028,  1032,  1034,  1037,  1040,  1042
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      41,     0,    -1,    -1,    41,    42,    -1,    41,    44,    -1,
      41,    46,    -1,    41,    48,    -1,     9,    18,    26,    -1,
       9,    18,    17,    26,    -1,     4,    25,    27,    62,    15,
      28,    -1,     4,     7,    25,    27,    62,    15,    28,    -1,
       4,    25,    27,    62,     7,    15,    28,    -1,    43,    49,
      15,    26,    -1,    43,     8,    29,    -1,    43,    30,    26,
      -1,     5,    25,    27,    62,    15,    28,    -1,     5,     7,
      25,    27,    62,    15,    28,    -1,     5,    25,    27,    62,
       7,    15,    28,    -1,    45,    49,    15,    26,    -1,    45,
       8,    29,    -1,    45,    30,    26,    -1,     6,    15,    28,
      -1,     6,     7,    15,    28,    -1,    47,    49,    15,    26,
      -1,    47,     8,    29,    -1,    47,    30,    26,    -1,    50,
      -1,    51,    -1,    52,    -1,    53,    -1,    54,    -1,    60,
      -1,    59,    -1,    55,    -1,    56,    -1,    57,    -1,    58,
      -1,    19,    -1,    19,    10,    27,    63,    -1,    19,    31,
      63,    32,    63,    33,    -1,    19,    34,    63,    32,    63,
      33,    -1,    19,    31,    63,    32,    63,    35,    -1,    19,
      34,    63,    32,    63,    35,    -1,    19,    31,    32,    63,
      33,    -1,    19,    34,    32,    63,    33,    -1,    19,    31,
      32,    63,    35,    -1,    19,    34,    32,    63,    35,    -1,
      19,    31,    63,    32,    33,    -1,    19,    34,    63,    32,
      33,    -1,    19,    31,    63,    32,    35,    -1,    19,    34,
      63,    32,    35,    -1,    19,    31,    63,    32,    63,    33,
      10,    27,    63,    -1,    19,    34,    63,    32,    63,    33,
      10,    27,    63,    -1,    19,    31,    63,    32,    63,    35,
      10,    27,    63,    -1,    19,    34,    63,    32,    63,    35,
      10,    27,    63,    -1,    19,    31,    32,    63,    33,    10,
      27,    63,    -1,    19,    34,    32,    63,    33,    10,    27,
      63,    -1,    19,    31,    32,    63,    35,    10,    27,    63,
      -1,    19,    34,    32,    63,    35,    10,    27,    63,    -1,
      19,    31,    63,    32,    33,    10,    27,    63,    -1,    19,
      34,    63,    32,    33,    10,    27,    63,    -1,    19,    31,
      63,    32,    35,    10,    27,    63,    -1,    19,    34,    63,
      32,    35,    10,    27,    63,    -1,    20,    -1,    20,    10,
      27,    63,    -1,    20,    31,    63,    32,    63,    33,    -1,
      20,    34,    63,    32,    63,    33,    -1,    20,    31,    63,
      32,    63,    35,    -1,    20,    34,    63,    32,    63,    35,
      -1,    20,    31,    32,    63,    33,    -1,    20,    34,    32,
      63,    33,    -1,    20,    31,    32,    63,    35,    -1,    20,
      34,    32,    63,    35,    -1,    20,    31,    63,    32,    33,
      -1,    20,    34,    63,    32,    33,    -1,    20,    31,    63,
      32,    35,    -1,    20,    34,    63,    32,    35,    -1,    20,
      31,    63,    32,    63,    33,    10,    27,    63,    -1,    20,
      34,    63,    32,    63,    33,    10,    27,    63,    -1,    20,
      31,    63,    32,    63,    35,    10,    27,    63,    -1,    20,
      34,    63,    32,    63,    35,    10,    27,    63,    -1,    20,
      31,    32,    63,    33,    10,    27,    63,    -1,    20,    34,
      32,    63,    33,    10,    27,    63,    -1,    20,    31,    32,
      63,    35,    10,    27,    63,    -1,    20,    34,    32,    63,
      35,    10,    27,    63,    -1,    20,    31,    63,    32,    33,
      10,    27,    63,    -1,    20,    34,    63,    32,    33,    10,
      27,    63,    -1,    20,    31,    63,    32,    35,    10,    27,
      63,    -1,    20,    34,    63,    32,    35,    10,    27,    63,
      -1,    21,    -1,    21,    10,    27,    63,    -1,    21,    31,
      63,    32,    63,    33,    -1,    21,    34,    63,    32,    63,
      33,    -1,    21,    31,    63,    32,    63,    35,    -1,    21,
      34,    63,    32,    63,    35,    -1,    21,    31,    32,    63,
      33,    -1,    21,    34,    32,    63,    33,    -1,    21,    31,
      32,    63,    35,    -1,    21,    34,    32,    63,    35,    -1,
      21,    31,    63,    32,    33,    -1,    21,    34,    63,    32,
      33,    -1,    21,    31,    63,    32,    35,    -1,    21,    34,
      63,    32,    35,    -1,    21,    31,    63,    32,    63,    33,
      10,    27,    63,    -1,    21,    34,    63,    32,    63,    33,
      10,    27,    63,    -1,    21,    31,    63,    32,    63,    35,
      10,    27,    63,    -1,    21,    34,    63,    32,    63,    35,
      10,    27,    63,    -1,    21,    31,    32,    63,    33,    10,
      27,    63,    -1,    21,    34,    32,    63,    33,    10,    27,
      63,    -1,    21,    31,    32,    63,    35,    10,    27,    63,
      -1,    21,    34,    32,    63,    35,    10,    27,    63,    -1,
      21,    31,    63,    32,    33,    10,    27,    63,    -1,    21,
      34,    63,    32,    33,    10,    27,    63,    -1,    21,    31,
      63,    32,    35,    10,    27,    63,    -1,    21,    34,    63,
      32,    35,    10,    27,    63,    -1,    22,    -1,    22,    12,
      27,    63,    -1,    22,    10,    27,    17,    -1,    22,    12,
      27,    63,    10,    27,    17,    -1,    22,    10,    27,    17,
      12,    27,    63,    -1,    14,    31,    63,    33,    -1,    24,
      36,    19,    37,    -1,    24,    36,    20,    37,    -1,    24,
      36,    22,    37,    -1,    24,    36,    23,    37,    -1,    24,
      36,    14,    37,    -1,    24,    36,     4,    15,    37,    -1,
      24,    36,     4,     7,    15,    37,    -1,    24,    36,     5,
      15,    37,    -1,    24,    36,     5,     7,    15,    37,    -1,
      24,    36,     6,    15,    37,    -1,    24,    36,     6,     7,
      15,    37,    -1,    24,    36,    19,    37,    10,    27,    11,
      -1,    24,    36,    20,    37,    10,    27,    11,    -1,    24,
      36,    22,    37,    10,    27,    11,    -1,    24,    36,    23,
      37,    10,    27,    11,    -1,    24,    36,    14,    37,    10,
      27,    11,    -1,    24,    36,     4,    15,    37,    10,    27,
      11,    -1,    24,    36,     4,     7,    15,    37,    10,    27,
      11,    -1,    24,    36,     7,     4,    15,    37,    10,    27,
      11,    -1,    24,    36,     5,    15,    37,    10,    27,    11,
      -1,    24,    36,     5,     7,    15,    37,    10,    27,    11,
      -1,    24,    36,     7,     5,    15,    37,    10,    27,    11,
      -1,    24,    36,     6,    15,    37,    10,    27,    11,    -1,
      24,    36,     6,     7,    15,    37,    10,    27,    11,    -1,
      24,    36,     7,     6,    15,    37,    10,    27,    11,    -1,
       4,    15,    -1,     4,     7,    15,    -1,     7,     4,    15,
      -1,     5,    15,    -1,     5,     7,    15,    -1,     7,     5,
      15,    -1,     6,    15,    -1,     6,     7,    15,    -1,     7,
       6,    15,    -1,    23,    -1,    13,    64,    28,    61,    30,
      -1,    13,    64,    28,    61,    30,    10,    27,    15,    -1,
      15,    27,    18,    -1,    61,    32,    15,    27,    18,    -1,
      16,    34,    17,    35,    27,    18,    -1,    61,    32,    16,
      34,    17,    35,    27,    18,    -1,    15,    -1,    62,    32,
      15,    -1,    18,    -1,    38,    18,    -1,    39,    18,    -1,
      17,    -1,    15,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    61,    61,    62,    63,    64,    65,    69,    70,    74,
      75,    76,    77,    78,    82,    86,    87,    88,    89,    90,
      94,    98,    99,   100,   101,   105,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   211,   212,   213,   214,   215,
     223,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   255,   256,   257,   260,
     261,   262,   265,   266,   267,   270,   274,   275,   279,   280,
     281,   282,   286,   287,   291,   292,   293,   294,   298
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "KW_FILE", "KW_MESSAGE",
  "KW_PUBLISHABLE", "KW_STRUCT", "KW_NONEXTENDABLE", "KW_EXTENSION",
  "KW_HASH_LINE", "KW_DEFAULT", "KW_EMPTY", "KW_MAX_LENGTH", "KW_ENUM",
  "KW_BYTE_ARRAY", "IDENTIFIER", "KW_IDENTIFIER", "STRING_LITERAL",
  "INTEGER_LITERAL", "KW_INTEGER", "KW_UINTEGER", "KW_REAL",
  "KW_CHARACTER_STRING", "KW_BLOB", "KW_VECTOR", "KW_PROTO", "';'", "'='",
  "'{'", "':'", "'}'", "'['", "','", "']'", "'('", "')'", "'<'", "'>'",
  "'+'", "'-'", "$accept", "file", "line_directive", "message_begin",
  "message", "publishable_begin", "publishable", "struct_begin", "struct",
  "data_type", "integer_type", "unsigned_integer_type", "real_type",
  "character_string_type", "byte_array_type", "vector_type",
  "message_type", "publishable_type", "struct_type", "blob_type",
  "inline_enum_type", "enum_values", "proto_values", "expr", "qname", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,    59,    61,   123,    58,
     125,    91,    44,    93,    40,    41,    60,    62,    43,    45
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    41,    41,    41,    41,    42,    42,    43,
      43,    43,    43,    43,    44,    45,    45,    45,    45,    45,
      46,    47,    47,    47,    47,    48,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    53,    53,    53,    53,    53,
      54,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    56,    56,    56,    57,
      57,    57,    58,    58,    58,    59,    60,    60,    61,    61,
      61,    61,    62,    62,    63,    63,    63,    63,    64
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     3,     4,     6,
       7,     7,     4,     3,     3,     6,     7,     7,     4,     3,
       3,     3,     4,     4,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     6,
       6,     6,     6,     5,     5,     5,     5,     5,     5,     5,
       5,     9,     9,     9,     9,     8,     8,     8,     8,     8,
       8,     8,     8,     1,     4,     6,     6,     6,     6,     5,
       5,     5,     5,     5,     5,     5,     5,     9,     9,     9,
       9,     8,     8,     8,     8,     8,     8,     8,     8,     1,
       4,     6,     6,     6,     6,     5,     5,     5,     5,     5,
       5,     5,     5,     9,     9,     9,     9,     8,     8,     8,
       8,     8,     8,     8,     8,     1,     4,     4,     7,     7,
       4,     4,     4,     4,     4,     4,     5,     6,     5,     6,
       5,     6,     7,     7,     7,     7,     7,     8,     9,     9,
       8,     9,     9,     8,     9,     9,     2,     3,     3,     2,
       3,     3,     2,     3,     3,     1,     5,     8,     3,     5,
       6,     8,     1,     3,     1,     2,     2,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     3,     0,     4,
       0,     5,     0,     6,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,    63,
      89,   115,   155,     0,     0,     0,    26,    27,    28,    29,
      30,    33,    34,    35,    36,    32,    31,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       7,     0,   146,     0,   149,     0,   152,     0,     0,     0,
      13,   168,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,     0,    19,    20,
       0,    24,    25,     0,     0,   162,     0,     0,     0,    22,
       8,   147,   150,   153,   148,   151,   154,     0,   167,   164,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    12,
      18,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   165,   166,   120,    38,     0,     0,     0,     0,
      64,     0,     0,     0,     0,    90,     0,     0,     0,     0,
     117,   116,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   125,   121,   122,   123,   124,     0,     0,     9,   163,
       0,     0,    15,     0,     0,   156,     0,    43,    45,    47,
      49,     0,    44,    46,    48,    50,     0,    69,    71,    73,
      75,     0,    70,    72,    74,    76,     0,    95,    97,    99,
     101,     0,    96,    98,   100,   102,     0,     0,     0,     0,
     126,     0,   128,     0,   130,     0,     0,     0,     0,     0,
       0,     0,     0,    10,    11,    16,    17,   158,     0,     0,
       0,     0,     0,     0,     0,     0,    39,    41,     0,     0,
       0,     0,    40,    42,     0,     0,     0,     0,    65,    67,
       0,     0,     0,     0,    66,    68,     0,     0,     0,     0,
      91,    93,     0,     0,     0,     0,    92,    94,     0,     0,
     127,     0,   129,     0,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   119,   118,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   136,   132,   133,   134,   135,
       0,   157,   159,     0,    55,    57,    59,    61,     0,     0,
      56,    58,    60,    62,     0,     0,    81,    83,    85,    87,
       0,     0,    82,    84,    86,    88,     0,     0,   107,   109,
     111,   113,     0,     0,   108,   110,   112,   114,     0,     0,
       0,   137,     0,   140,     0,   143,     0,     0,     0,   160,
       0,    51,    53,    52,    54,    77,    79,    78,    80,   103,
     105,   104,   106,   138,   141,   144,   139,   142,   145,     0,
     161
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     7,     8,     9,    10,    11,    12,    13,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,   151,    96,   112,    72
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -76
static const yytype_int16 yypact[] =
{
     -76,   212,   -76,    10,    53,    54,   -16,   -76,     6,   -76,
      51,   -76,    91,   -76,    -7,    -3,     9,    63,    22,    65,
      59,    93,   111,   113,   159,    77,    88,    85,    11,    52,
      57,    56,   -76,    94,   129,   162,   -76,   -76,   -76,   -76,
     -76,   -76,   -76,   -76,   -76,   -76,   -76,   119,   141,   172,
     145,   166,   184,   204,   196,   205,   196,   209,   -76,   193,
     -76,   223,   -76,   228,   -76,   229,   -76,   234,   235,   240,
     -76,   -76,   233,    14,   248,   144,   152,   253,   163,   168,
     257,   171,   176,   264,   265,   263,   -76,   230,   -76,   -76,
     236,   -76,   -76,   269,   196,   -76,     1,   196,     8,   -76,
     -76,   -76,   -76,   -76,   -76,   -76,   -76,   128,   -76,   -76,
     279,   280,   268,    14,    14,   270,    14,   271,    14,    14,
     272,    14,   275,    14,    14,   276,    14,   277,   293,    14,
     120,   122,   126,   192,   259,   282,   283,   284,   285,   -76,
     -76,   -76,   -10,   298,   286,   300,    48,   301,   297,   299,
     294,    47,   -76,   -76,   -76,   -76,    74,    84,   101,   107,
     -76,   187,   114,   190,   121,   -76,   191,   133,   194,   140,
     315,   319,   316,   295,   318,   302,   320,   303,   321,   322,
     323,   324,   331,   332,   333,   334,   317,   325,   -76,   -76,
     326,   327,   -76,   312,   329,   337,   189,   338,   339,   340,
     341,   195,   342,   346,   347,   348,   238,   349,   350,   351,
     352,   239,   353,   354,   355,   356,   243,   357,   358,   359,
     360,   246,   361,   362,   363,   364,   254,   365,   366,   343,
     367,   344,   368,   345,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   -76,   -76,   -76,   -76,   -76,   378,   379,
     383,   380,   384,   385,   388,   389,   381,   386,   390,   391,
     392,   393,   387,   395,   394,   396,   397,   398,   412,   416,
     400,   401,   402,   403,   421,   422,   406,   407,   408,   409,
     427,   428,   413,   414,   415,   417,   429,   433,    14,   382,
     435,   419,   437,   423,   438,   424,   439,   442,   443,   444,
     445,   446,   447,   448,   434,   449,   436,   450,    14,    14,
      14,    14,   441,   451,    14,    14,    14,    14,   452,   453,
      14,    14,    14,    14,   454,   455,    14,    14,    14,    14,
     456,   457,    14,    14,    14,    14,   458,   459,    14,    14,
      14,    14,   460,   461,   -76,   -76,   462,   463,   464,   465,
     466,   479,   467,   468,   469,   -76,   -76,   -76,   -76,   -76,
     474,   -76,   -76,   425,   -76,   -76,   -76,   -76,    14,    14,
     -76,   -76,   -76,   -76,    14,    14,   -76,   -76,   -76,   -76,
      14,    14,   -76,   -76,   -76,   -76,    14,    14,   -76,   -76,
     -76,   -76,    14,    14,   -76,   -76,   -76,   -76,    14,    14,
     486,   -76,   487,   -76,   488,   -76,   489,   490,   491,   -76,
     476,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,
     -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   492,
     -76
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   278,
     -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,
     -76,   -76,   -47,   -75,   -76
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
     115,   117,    20,   120,   122,   186,   125,   127,   143,    98,
      21,    22,    23,    24,    25,   147,   144,    14,    53,    26,
      27,    74,   145,   148,    54,    28,    29,    30,    31,    32,
      33,   108,   109,   145,    55,    15,    34,    57,   155,   156,
     145,   158,    75,   160,   161,    76,   163,   142,   165,   166,
     146,   168,   110,   111,   171,    21,    22,    23,    24,    47,
      16,    18,    77,   190,    26,    27,    83,    80,    84,    19,
      28,    29,    30,    31,    32,    33,    59,   195,    17,   196,
     145,    48,   201,    78,   206,    60,    79,   211,    81,   216,
      56,    82,   221,    58,   226,    21,    22,    23,    24,    50,
      61,   108,   109,    71,    26,    27,    70,   197,    62,   198,
      28,    29,    30,    31,    32,    33,    73,   199,    63,   200,
      65,    51,   110,   111,   108,   109,    64,   172,    66,   174,
      85,   108,   109,   176,   202,   173,   203,   175,   108,   109,
     204,   177,   205,   149,   150,   110,   111,   209,    88,   210,
     108,   109,   110,   111,   214,    86,   215,   108,   109,   110,
     111,   108,   109,    67,    68,    69,   219,    89,   220,   108,
     109,   110,   111,   224,    91,   225,   114,    87,   110,   111,
     108,   109,   110,   111,   116,   108,   109,    90,   108,   109,
     110,   111,    92,   108,   109,   119,   178,   179,   180,    93,
     121,   110,   111,   124,   250,   251,   110,   111,   126,   110,
     111,    95,     2,   344,   110,   111,     3,     4,     5,   100,
     207,     6,   208,   212,   217,   213,   218,   222,   256,   223,
     257,    94,    97,   364,   365,   366,   367,    99,   101,   370,
     371,   372,   373,   102,   103,   376,   377,   378,   379,   104,
     105,   382,   383,   384,   385,   106,   139,   388,   389,   390,
     391,   107,   140,   394,   395,   396,   397,   130,   131,   132,
     133,   262,   268,   263,   269,   113,   274,   134,   275,   280,
     118,   281,   135,   136,   123,   137,   138,   286,    49,   287,
      52,   128,   129,   411,   412,   141,   181,   152,   153,   413,
     414,   154,   157,   159,   162,   415,   416,   164,   167,   169,
     170,   417,   418,   187,   188,   189,   191,   419,   420,   182,
     183,   184,   185,   421,   422,   192,   193,   227,   194,   228,
     247,   229,   230,   231,   238,   233,   235,   236,   237,   232,
     234,   239,   240,   241,   242,   243,   248,   249,   252,   253,
     254,   255,   258,   244,   245,   246,   259,   260,   261,   264,
     265,   266,   267,   270,   271,   272,   273,   276,   277,   278,
     279,   282,   283,   284,   285,     0,     0,   291,   293,   295,
     290,   292,   294,     0,     0,     0,     0,     0,     0,     0,
       0,   312,   288,   289,     0,     0,   313,   318,     0,   345,
     299,   300,   301,   302,   303,   319,   305,   296,   297,   298,
     306,   308,   309,   304,   307,   310,   311,   314,   315,   316,
     317,   320,   324,   321,   322,   323,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   342,
     338,   339,   340,   343,   341,   346,   347,   348,   350,   352,
     349,   351,   353,   354,   362,   355,   356,   357,   358,   359,
     410,   360,     0,     0,   361,     0,     0,   363,   368,     0,
       0,     0,     0,     0,   401,     0,   403,     0,   369,   374,
     375,   380,   381,   386,   387,   392,   393,   398,   399,   400,
     405,   402,   409,   404,   406,   407,   408,   423,   424,   425,
     426,   427,   428,   429,     0,     0,     0,     0,     0,     0,
     430
};

static const yytype_int16 yycheck[] =
{
      75,    76,    18,    78,    79,    15,    81,    82,     7,    56,
       4,     5,     6,     7,     8,     7,    15,     7,    25,    13,
      14,    10,    32,    15,    27,    19,    20,    21,    22,    23,
      24,    17,    18,    32,    25,    25,    30,    15,   113,   114,
      32,   116,    31,   118,   119,    34,   121,    94,   123,   124,
      97,   126,    38,    39,   129,     4,     5,     6,     7,     8,
       7,     7,    10,    15,    13,    14,    10,    10,    12,    15,
      19,    20,    21,    22,    23,    24,    17,    30,    25,    32,
      32,    30,   157,    31,   159,    26,    34,   162,    31,   164,
      27,    34,   167,    28,   169,     4,     5,     6,     7,     8,
       7,    17,    18,    15,    13,    14,    29,    33,    15,    35,
      19,    20,    21,    22,    23,    24,    31,    33,     7,    35,
       7,    30,    38,    39,    17,    18,    15,     7,    15,     7,
      36,    17,    18,     7,    33,    15,    35,    15,    17,    18,
      33,    15,    35,    15,    16,    38,    39,    33,    29,    35,
      17,    18,    38,    39,    33,    26,    35,    17,    18,    38,
      39,    17,    18,     4,     5,     6,    33,    26,    35,    17,
      18,    38,    39,    33,    29,    35,    32,    15,    38,    39,
      17,    18,    38,    39,    32,    17,    18,    15,    17,    18,
      38,    39,    26,    17,    18,    32,     4,     5,     6,    15,
      32,    38,    39,    32,    15,    16,    38,    39,    32,    38,
      39,    15,     0,   288,    38,    39,     4,     5,     6,    26,
      33,     9,    35,    33,    33,    35,    35,    33,    33,    35,
      35,    27,    27,   308,   309,   310,   311,    28,    15,   314,
     315,   316,   317,    15,    15,   320,   321,   322,   323,    15,
      15,   326,   327,   328,   329,    15,    26,   332,   333,   334,
     335,    28,    26,   338,   339,   340,   341,     4,     5,     6,
       7,    33,    33,    35,    35,    27,    33,    14,    35,    33,
      27,    35,    19,    20,    27,    22,    23,    33,    10,    35,
      12,    27,    27,   368,   369,    26,    37,    18,    18,   374,
     375,    33,    32,    32,    32,   380,   381,    32,    32,    32,
      17,   386,   387,    15,    28,    15,    15,   392,   393,    37,
      37,    37,    37,   398,   399,    28,    27,    12,    34,    10,
      18,    15,    37,    15,    10,    15,    15,    15,    15,    37,
      37,    10,    10,    10,    10,    28,    17,    10,    10,    10,
      10,    10,    10,    28,    28,    28,    10,    10,    10,    10,
      10,    10,    10,    10,    10,    10,    10,    10,    10,    10,
      10,    10,    10,    10,    10,    -1,    -1,    10,    10,    10,
      37,    37,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    27,    27,    -1,    -1,    10,    10,    -1,    17,
      27,    27,    27,    27,    27,    10,    27,    37,    37,    37,
      27,    27,    27,    35,    34,    27,    27,    27,    27,    27,
      27,    27,    10,    27,    27,    27,    10,    27,    27,    27,
      27,    10,    10,    27,    27,    27,    27,    10,    10,    10,
      27,    27,    27,    10,    27,    10,    27,    10,    10,    10,
      27,    27,    10,    10,    18,    11,    11,    11,    11,    11,
      35,    27,    -1,    -1,    15,    -1,    -1,    17,    27,    -1,
      -1,    -1,    -1,    -1,    11,    -1,    11,    -1,    27,    27,
      27,    27,    27,    27,    27,    27,    27,    27,    27,    27,
      11,    27,    18,    27,    27,    27,    27,    11,    11,    11,
      11,    11,    11,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      18
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    41,     0,     4,     5,     6,     9,    42,    43,    44,
      45,    46,    47,    48,     7,    25,     7,    25,     7,    15,
      18,     4,     5,     6,     7,     8,    13,    14,    19,    20,
      21,    22,    23,    24,    30,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,     8,    30,    49,
       8,    30,    49,    25,    27,    25,    27,    15,    28,    17,
      26,     7,    15,     7,    15,     7,    15,     4,     5,     6,
      29,    15,    64,    31,    10,    31,    34,    10,    31,    34,
      10,    31,    34,    10,    12,    36,    26,    15,    29,    26,
      15,    29,    26,    15,    27,    15,    62,    27,    62,    28,
      26,    15,    15,    15,    15,    15,    15,    28,    17,    18,
      38,    39,    63,    27,    32,    63,    32,    63,    27,    32,
      63,    32,    63,    27,    32,    63,    32,    63,    27,    27,
       4,     5,     6,     7,    14,    19,    20,    22,    23,    26,
      26,    26,    62,     7,    15,    32,    62,     7,    15,    15,
      16,    61,    18,    18,    33,    63,    63,    32,    63,    32,
      63,    63,    32,    63,    32,    63,    63,    32,    63,    32,
      17,    63,     7,    15,     7,    15,     7,    15,     4,     5,
       6,    37,    37,    37,    37,    37,    15,    15,    28,    15,
      15,    15,    28,    27,    34,    30,    32,    33,    35,    33,
      35,    63,    33,    35,    33,    35,    63,    33,    35,    33,
      35,    63,    33,    35,    33,    35,    63,    33,    35,    33,
      35,    63,    33,    35,    33,    35,    63,    12,    10,    15,
      37,    15,    37,    15,    37,    15,    15,    15,    10,    10,
      10,    10,    10,    28,    28,    28,    28,    18,    17,    10,
      15,    16,    10,    10,    10,    10,    33,    35,    10,    10,
      10,    10,    33,    35,    10,    10,    10,    10,    33,    35,
      10,    10,    10,    10,    33,    35,    10,    10,    10,    10,
      33,    35,    10,    10,    10,    10,    33,    35,    27,    27,
      37,    10,    37,    10,    37,    10,    37,    37,    37,    27,
      27,    27,    27,    27,    35,    27,    27,    34,    27,    27,
      27,    27,    10,    10,    27,    27,    27,    27,    10,    10,
      27,    27,    27,    27,    10,    10,    27,    27,    27,    27,
      10,    10,    27,    27,    27,    27,    10,    10,    27,    27,
      27,    27,    10,    10,    63,    17,    10,    27,    10,    27,
      10,    27,    10,    10,    10,    11,    11,    11,    11,    11,
      27,    15,    18,    17,    63,    63,    63,    63,    27,    27,
      63,    63,    63,    63,    27,    27,    63,    63,    63,    63,
      27,    27,    63,    63,    63,    63,    27,    27,    63,    63,
      63,    63,    27,    27,    63,    63,    63,    63,    27,    27,
      27,    11,    27,    11,    27,    11,    27,    27,    27,    18,
      35,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    11,    11,    11,    11,    11,    11,    27,
      18
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

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
# if YYLTYPE_IS_TRIVIAL
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
		  Type, Value); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
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
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 3: /* "KW_FILE" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1447 "global_mq_IDL.tab.c"
	break;
      case 4: /* "KW_MESSAGE" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1456 "global_mq_IDL.tab.c"
	break;
      case 5: /* "KW_PUBLISHABLE" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1465 "global_mq_IDL.tab.c"
	break;
      case 6: /* "KW_STRUCT" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1474 "global_mq_IDL.tab.c"
	break;
      case 7: /* "KW_NONEXTENDABLE" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1483 "global_mq_IDL.tab.c"
	break;
      case 8: /* "KW_EXTENSION" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1492 "global_mq_IDL.tab.c"
	break;
      case 9: /* "KW_HASH_LINE" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1501 "global_mq_IDL.tab.c"
	break;
      case 10: /* "KW_DEFAULT" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1510 "global_mq_IDL.tab.c"
	break;
      case 11: /* "KW_EMPTY" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1519 "global_mq_IDL.tab.c"
	break;
      case 12: /* "KW_MAX_LENGTH" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1528 "global_mq_IDL.tab.c"
	break;
      case 13: /* "KW_ENUM" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1537 "global_mq_IDL.tab.c"
	break;
      case 14: /* "KW_BYTE_ARRAY" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1546 "global_mq_IDL.tab.c"
	break;
      case 15: /* "IDENTIFIER" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1555 "global_mq_IDL.tab.c"
	break;
      case 16: /* "KW_IDENTIFIER" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1564 "global_mq_IDL.tab.c"
	break;
      case 17: /* "STRING_LITERAL" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1573 "global_mq_IDL.tab.c"
	break;
      case 18: /* "INTEGER_LITERAL" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1582 "global_mq_IDL.tab.c"
	break;
      case 19: /* "KW_INTEGER" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1591 "global_mq_IDL.tab.c"
	break;
      case 20: /* "KW_UINTEGER" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1600 "global_mq_IDL.tab.c"
	break;
      case 21: /* "KW_REAL" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1609 "global_mq_IDL.tab.c"
	break;
      case 22: /* "KW_CHARACTER_STRING" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1618 "global_mq_IDL.tab.c"
	break;
      case 23: /* "KW_BLOB" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1627 "global_mq_IDL.tab.c"
	break;
      case 24: /* "KW_VECTOR" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1636 "global_mq_IDL.tab.c"
	break;
      case 25: /* "KW_PROTO" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1645 "global_mq_IDL.tab.c"
	break;
      case 26: /* "';'" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1654 "global_mq_IDL.tab.c"
	break;
      case 27: /* "'='" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1663 "global_mq_IDL.tab.c"
	break;
      case 28: /* "'{'" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1672 "global_mq_IDL.tab.c"
	break;
      case 29: /* "':'" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1681 "global_mq_IDL.tab.c"
	break;
      case 30: /* "'}'" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1690 "global_mq_IDL.tab.c"
	break;
      case 31: /* "'['" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1699 "global_mq_IDL.tab.c"
	break;
      case 32: /* "','" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1708 "global_mq_IDL.tab.c"
	break;
      case 33: /* "']'" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1717 "global_mq_IDL.tab.c"
	break;
      case 34: /* "'('" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1726 "global_mq_IDL.tab.c"
	break;
      case 35: /* "')'" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1735 "global_mq_IDL.tab.c"
	break;
      case 36: /* "'<'" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1744 "global_mq_IDL.tab.c"
	break;
      case 37: /* "'>'" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1753 "global_mq_IDL.tab.c"
	break;
      case 38: /* "'+'" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1762 "global_mq_IDL.tab.c"
	break;
      case 39: /* "'-'" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1771 "global_mq_IDL.tab.c"
	break;
      case 41: /* "file" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1780 "global_mq_IDL.tab.c"
	break;
      case 42: /* "line_directive" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1789 "global_mq_IDL.tab.c"
	break;
      case 43: /* "message_begin" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1798 "global_mq_IDL.tab.c"
	break;
      case 44: /* "message" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1807 "global_mq_IDL.tab.c"
	break;
      case 45: /* "publishable_begin" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1816 "global_mq_IDL.tab.c"
	break;
      case 46: /* "publishable" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1825 "global_mq_IDL.tab.c"
	break;
      case 47: /* "struct_begin" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1834 "global_mq_IDL.tab.c"
	break;
      case 48: /* "struct" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1843 "global_mq_IDL.tab.c"
	break;
      case 49: /* "data_type" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1852 "global_mq_IDL.tab.c"
	break;
      case 50: /* "integer_type" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1861 "global_mq_IDL.tab.c"
	break;
      case 51: /* "unsigned_integer_type" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1870 "global_mq_IDL.tab.c"
	break;
      case 52: /* "real_type" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1879 "global_mq_IDL.tab.c"
	break;
      case 53: /* "character_string_type" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1888 "global_mq_IDL.tab.c"
	break;
      case 54: /* "byte_array_type" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1897 "global_mq_IDL.tab.c"
	break;
      case 55: /* "vector_type" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1906 "global_mq_IDL.tab.c"
	break;
      case 56: /* "message_type" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1915 "global_mq_IDL.tab.c"
	break;
      case 57: /* "publishable_type" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1924 "global_mq_IDL.tab.c"
	break;
      case 58: /* "struct_type" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1933 "global_mq_IDL.tab.c"
	break;
      case 59: /* "blob_type" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1942 "global_mq_IDL.tab.c"
	break;
      case 60: /* "inline_enum_type" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1951 "global_mq_IDL.tab.c"
	break;
      case 61: /* "enum_values" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1960 "global_mq_IDL.tab.c"
	break;
      case 62: /* "proto_values" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1969 "global_mq_IDL.tab.c"
	break;
      case 63: /* "expr" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1978 "global_mq_IDL.tab.c"
	break;
      case 64: /* "qname" */

/* Line 1000 of yacc.c  */
#line 52 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1987 "global_mq_IDL.tab.c"
	break;

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

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 61 "global_mq_IDL.y"
    { (yyval) = 0; ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 62 "global_mq_IDL.y"
    { (yyval) = 0; releaseYys2((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 63 "global_mq_IDL.y"
    { (yyval) = addMessageToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 64 "global_mq_IDL.y"
    { (yyval) = addPublishableToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 65 "global_mq_IDL.y"
    { (yyval) = addStructToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 69 "global_mq_IDL.y"
    { (yyval) = 0; processLineDirective((yyvsp[(2) - (3)]), 0); releaseYys2((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 70 "global_mq_IDL.y"
    { (yyval) = 0; processLineDirective((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); releaseYys2((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 74 "global_mq_IDL.y"
    { (yyval) = createMessage((yyvsp[(1) - (6)]), false, (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)])); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 75 "global_mq_IDL.y"
    { (yyval) = createMessage((yyvsp[(1) - (7)]), true, (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 76 "global_mq_IDL.y"
    { (yyval) = createMessage((yyvsp[(1) - (7)]), true, (yyvsp[(4) - (7)]), (yyvsp[(6) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 77 "global_mq_IDL.y"
    { (yyval) = addToMessage((yyvsp[(1) - (4)]), createAttribute((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))); releaseYys((yyvsp[(4) - (4)])); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 78 "global_mq_IDL.y"
    { (yyval) = insertExtensionMarkerToMessage((yyvsp[(1) - (3)])); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 82 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (3)]); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 86 "global_mq_IDL.y"
    { (yyval) = createPublishable((yyvsp[(1) - (6)]), false, (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)])); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 87 "global_mq_IDL.y"
    { (yyval) = createPublishable((yyvsp[(1) - (7)]), true, (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 88 "global_mq_IDL.y"
    { (yyval) = createPublishable((yyvsp[(1) - (7)]), true, (yyvsp[(4) - (7)]), (yyvsp[(6) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 89 "global_mq_IDL.y"
    { (yyval) = addToPublishable((yyvsp[(1) - (4)]), createAttribute((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))); releaseYys((yyvsp[(4) - (4)])); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 90 "global_mq_IDL.y"
    { (yyval) = insertExtensionMarkerToPublishable((yyvsp[(1) - (3)])); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 94 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (3)]); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 98 "global_mq_IDL.y"
    { (yyval) = createStruct((yyvsp[(1) - (3)]), false, (yyvsp[(2) - (3)])); releaseYys((yyvsp[(3) - (3)])); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 99 "global_mq_IDL.y"
    { (yyval) = createStruct((yyvsp[(1) - (4)]), true, (yyvsp[(3) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 100 "global_mq_IDL.y"
    { (yyval) = addToStruct((yyvsp[(1) - (4)]), createAttribute((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))); releaseYys((yyvsp[(4) - (4)])); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 101 "global_mq_IDL.y"
    { (yyval) = insertExtensionMarkerToStruct((yyvsp[(1) - (3)])); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 105 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (3)]); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 123 "global_mq_IDL.y"
    { (yyval) = createIntegerType((yyvsp[(1) - (1)])); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 124 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefault((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 125 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 126 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 127 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 128 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 129 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 130 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 131 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 132 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 133 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 134 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 135 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 136 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 137 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 138 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 139 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 140 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 141 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 142 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 143 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 144 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 145 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 146 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 147 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 148 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 152 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerType((yyvsp[(1) - (1)])); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 153 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefault((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 154 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 155 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 156 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 157 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 158 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 159 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 160 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 161 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 162 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 163 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 164 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 165 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 166 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 167 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 168 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 169 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 170 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 171 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 172 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 173 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 174 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 175 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 176 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 177 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 181 "global_mq_IDL.y"
    { (yyval) = createRealType((yyvsp[(1) - (1)])); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 182 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefault((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 183 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 184 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 185 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 186 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 187 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 188 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 189 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 190 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 191 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 192 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 193 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 194 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 195 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 196 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 197 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 198 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 199 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 200 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 201 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 202 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 203 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 204 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 205 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 206 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 211 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (1)]), false, 0, false, 0); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 212 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (4)]), true, (yyvsp[(4) - (4)]), false, 0); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 213 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (4)]), false, 0, true, (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 214 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (7)]), true, (yyvsp[(4) - (7)]), true, (yyvsp[(7) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)])); ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 215 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (7)]), true, (yyvsp[(7) - (7)]), true, (yyvsp[(4) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)])); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 223 "global_mq_IDL.y"
    { (yyval) = createByteArrayType((yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 227 "global_mq_IDL.y"
    { (yyval) = createVectorOfIntegerType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 228 "global_mq_IDL.y"
    { (yyval) = createVectorOfUintegerType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 229 "global_mq_IDL.y"
    { (yyval) = createVectorOfCharStringType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 230 "global_mq_IDL.y"
    { (yyval) = createVectorOfBLOBType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 231 "global_mq_IDL.y"
    { (yyval) = createVectorOfByteArrayType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 232 "global_mq_IDL.y"
    { (yyval) = createVectorOfMassagesType((yyvsp[(1) - (5)]), (yyvsp[(4) - (5)]), false, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 233 "global_mq_IDL.y"
    { (yyval) = createVectorOfMassagesType((yyvsp[(1) - (6)]), (yyvsp[(5) - (6)]), true, false); releaseYys4((yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 234 "global_mq_IDL.y"
    { (yyval) = createVectorOfPublishablesType((yyvsp[(1) - (5)]), (yyvsp[(4) - (5)]), false, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 235 "global_mq_IDL.y"
    { (yyval) = createVectorOfPublishablesType((yyvsp[(1) - (6)]), (yyvsp[(5) - (6)]), true, false); releaseYys4((yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 236 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (5)]), (yyvsp[(4) - (5)]), false, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 237 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (6)]), (yyvsp[(5) - (6)]), true, false); releaseYys4((yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 238 "global_mq_IDL.y"
    { (yyval) = createVectorOfIntegerType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 239 "global_mq_IDL.y"
    { (yyval) = createVectorOfUintegerType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 240 "global_mq_IDL.y"
    { (yyval) = createVectorOfCharStringType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 241 "global_mq_IDL.y"
    { (yyval) = createVectorOfBLOBType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 242 "global_mq_IDL.y"
    { (yyval) = createVectorOfByteArrayType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 243 "global_mq_IDL.y"
    { (yyval) = createVectorOfMassagesType((yyvsp[(1) - (8)]), (yyvsp[(4) - (8)]), false, true); releaseYys6((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(8) - (8)])); ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 244 "global_mq_IDL.y"
    { (yyval) = createVectorOfMassagesType((yyvsp[(1) - (9)]), (yyvsp[(5) - (9)]), true, true); releaseYys7((yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)])); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 245 "global_mq_IDL.y"
    { (yyval) = createVectorOfMassagesType((yyvsp[(1) - (9)]), (yyvsp[(5) - (9)]), true, true); releaseYys7((yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)])); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 246 "global_mq_IDL.y"
    { (yyval) = createVectorOfPublishablesType((yyvsp[(1) - (8)]), (yyvsp[(4) - (8)]), false, true); releaseYys6((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(8) - (8)])); ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 247 "global_mq_IDL.y"
    { (yyval) = createVectorOfPublishablesType((yyvsp[(1) - (9)]), (yyvsp[(5) - (9)]), true, true); releaseYys7((yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)])); ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 248 "global_mq_IDL.y"
    { (yyval) = createVectorOfPublishablesType((yyvsp[(1) - (9)]), (yyvsp[(5) - (9)]), true, true); releaseYys7((yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)])); ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 249 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (8)]), (yyvsp[(4) - (8)]), false, true); releaseYys6((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(8) - (8)])); ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 250 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (9)]), (yyvsp[(5) - (9)]), true, true); releaseYys7((yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)])); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 251 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (9)]), (yyvsp[(5) - (9)]), true, true); releaseYys7((yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)])); ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 255 "global_mq_IDL.y"
    { (yyval) = createMessageType((yyvsp[(1) - (2)]), false, (yyvsp[(2) - (2)])); ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 256 "global_mq_IDL.y"
    { (yyval) = createMessageType((yyvsp[(1) - (3)]), true, (yyvsp[(3) - (3)])); releaseYys( (yyvsp[(2) - (3)]) );;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 257 "global_mq_IDL.y"
    { (yyval) = createMessageType((yyvsp[(2) - (3)]), true, (yyvsp[(3) - (3)])); releaseYys( (yyvsp[(1) - (3)]) ); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 260 "global_mq_IDL.y"
    { (yyval) = createPublishableType((yyvsp[(1) - (2)]), false, (yyvsp[(2) - (2)])); ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 261 "global_mq_IDL.y"
    { (yyval) = createPublishableType((yyvsp[(1) - (3)]), true, (yyvsp[(3) - (3)])); releaseYys( (yyvsp[(2) - (3)]) );;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 262 "global_mq_IDL.y"
    { (yyval) = createPublishableType((yyvsp[(2) - (3)]), true, (yyvsp[(3) - (3)])); releaseYys( (yyvsp[(1) - (3)]) ); ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 265 "global_mq_IDL.y"
    { (yyval) = createStructType((yyvsp[(1) - (2)]), false, (yyvsp[(2) - (2)])); ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 266 "global_mq_IDL.y"
    { (yyval) = createStructType((yyvsp[(1) - (3)]), true, (yyvsp[(3) - (3)])); releaseYys( (yyvsp[(2) - (3)]) );;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 267 "global_mq_IDL.y"
    { (yyval) = createStructType((yyvsp[(2) - (3)]), true, (yyvsp[(3) - (3)])); releaseYys( (yyvsp[(1) - (3)]) ); ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 270 "global_mq_IDL.y"
    { (yyval) = createBlobType((yyvsp[(1) - (1)])); ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 274 "global_mq_IDL.y"
    { (yyval) = createInlineEnum((yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(4) - (5)])); releaseYys2((yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 275 "global_mq_IDL.y"
    { (yyval) = createInlineEnumWithDefault((yyvsp[(1) - (8)]), (yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(8) - (8)])); releaseYys4((yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 279 "global_mq_IDL.y"
    { (yyval) = addEnumValue(0, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); releaseYys((yyvsp[(2) - (3)])); ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 280 "global_mq_IDL.y"
    { (yyval) = addEnumValue((yyvsp[(1) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); releaseYys2((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)])); ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 281 "global_mq_IDL.y"
    { (yyval) = addEnumValue(0, (yyvsp[(3) - (6)]), (yyvsp[(6) - (6)])); releaseYys4((yyvsp[(1) - (6)]), (yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)])); ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 282 "global_mq_IDL.y"
    { (yyval) = addEnumValue((yyvsp[(1) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(8) - (8)])); releaseYys3((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(4) - (8)])); releaseYys2((yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 286 "global_mq_IDL.y"
    { (yyval) = addProtoValue(0, (yyvsp[(1) - (1)])); ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 287 "global_mq_IDL.y"
    { (yyval) = addProtoValue((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); releaseYys((yyvsp[(2) - (3)])); ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 291 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 292 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(2) - (2)]); releaseYys((yyvsp[(1) - (2)])); ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 293 "global_mq_IDL.y"
    { (yyval) = makeMinusIntLit((yyvsp[(2) - (2)])); releaseYys((yyvsp[(1) - (2)])); ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 294 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 298 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;



/* Line 1455 of yacc.c  */
#line 3384 "global_mq_IDL.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

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
		      yytoken, &yylval);
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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


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
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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



