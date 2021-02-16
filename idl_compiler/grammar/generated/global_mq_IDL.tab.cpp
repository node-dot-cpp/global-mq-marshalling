
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
#line 55 "global_mq_IDL.y"

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
     KW_PROTO = 280,
     KW_SCOPE = 281
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
#line 147 "global_mq_IDL.tab.c"

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
#define YYLAST   474

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  156
/* YYNRULES -- Number of states.  */
#define YYNSTATES  397

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   281

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      35,    36,     2,    39,    33,    40,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    29,    27,
      37,    28,    38,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    32,     2,    34,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    30,     2,    31,     2,     2,     2,     2,
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
      25,    26
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    13,    16,    19,    22,
      26,    31,    38,    49,    61,    69,    78,    83,    87,    92,
      96,   100,   104,   108,   113,   118,   122,   126,   130,   135,
     140,   144,   148,   150,   152,   154,   156,   158,   160,   162,
     164,   166,   168,   173,   180,   187,   194,   201,   207,   213,
     219,   225,   231,   237,   243,   249,   259,   269,   279,   289,
     298,   307,   316,   325,   334,   343,   352,   361,   363,   368,
     375,   382,   389,   396,   402,   408,   414,   420,   426,   432,
     438,   444,   454,   464,   474,   484,   493,   502,   511,   520,
     529,   538,   547,   556,   558,   563,   570,   577,   584,   591,
     597,   603,   609,   615,   621,   627,   633,   639,   649,   659,
     669,   679,   688,   697,   706,   715,   724,   733,   742,   751,
     753,   758,   763,   771,   779,   784,   789,   794,   799,   804,
     809,   814,   820,   827,   835,   843,   851,   859,   867,   875,
     884,   894,   904,   907,   911,   915,   917,   923,   932,   936,
     942,   949,   958,   960,   963,   966,   968
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      42,     0,    -1,    -1,    42,    43,    -1,    42,    48,    -1,
      42,    45,    -1,    42,    50,    -1,    42,    52,    -1,    42,
      44,    -1,     9,    18,    27,    -1,     9,    18,    17,    27,
      -1,    26,    15,    25,    28,    15,    27,    -1,     4,    15,
      29,    15,    28,    18,    28,     6,    15,    27,    -1,     4,
       7,    15,    29,    15,    28,    18,    28,     6,    15,    27,
      -1,     4,    15,    29,    15,    28,    18,    30,    -1,     4,
       7,    15,    29,    15,    28,    18,    30,    -1,    46,    53,
      15,    27,    -1,    46,     8,    29,    -1,    47,    53,    15,
      27,    -1,    47,     8,    29,    -1,    46,    31,    27,    -1,
      47,    31,    27,    -1,     5,    15,    30,    -1,     5,     7,
      15,    30,    -1,    49,    53,    15,    27,    -1,    49,     8,
      29,    -1,    49,    31,    27,    -1,     6,    15,    30,    -1,
       6,     7,    15,    30,    -1,    51,    53,    15,    27,    -1,
      51,     8,    29,    -1,    51,    31,    27,    -1,    54,    -1,
      55,    -1,    56,    -1,    57,    -1,    58,    -1,    62,    -1,
      61,    -1,    59,    -1,    60,    -1,    19,    -1,    19,    10,
      28,    64,    -1,    19,    32,    64,    33,    64,    34,    -1,
      19,    35,    64,    33,    64,    34,    -1,    19,    32,    64,
      33,    64,    36,    -1,    19,    35,    64,    33,    64,    36,
      -1,    19,    32,    33,    64,    34,    -1,    19,    35,    33,
      64,    34,    -1,    19,    32,    33,    64,    36,    -1,    19,
      35,    33,    64,    36,    -1,    19,    32,    64,    33,    34,
      -1,    19,    35,    64,    33,    34,    -1,    19,    32,    64,
      33,    36,    -1,    19,    35,    64,    33,    36,    -1,    19,
      32,    64,    33,    64,    34,    10,    28,    64,    -1,    19,
      35,    64,    33,    64,    34,    10,    28,    64,    -1,    19,
      32,    64,    33,    64,    36,    10,    28,    64,    -1,    19,
      35,    64,    33,    64,    36,    10,    28,    64,    -1,    19,
      32,    33,    64,    34,    10,    28,    64,    -1,    19,    35,
      33,    64,    34,    10,    28,    64,    -1,    19,    32,    33,
      64,    36,    10,    28,    64,    -1,    19,    35,    33,    64,
      36,    10,    28,    64,    -1,    19,    32,    64,    33,    34,
      10,    28,    64,    -1,    19,    35,    64,    33,    34,    10,
      28,    64,    -1,    19,    32,    64,    33,    36,    10,    28,
      64,    -1,    19,    35,    64,    33,    36,    10,    28,    64,
      -1,    20,    -1,    20,    10,    28,    64,    -1,    20,    32,
      64,    33,    64,    34,    -1,    20,    35,    64,    33,    64,
      34,    -1,    20,    32,    64,    33,    64,    36,    -1,    20,
      35,    64,    33,    64,    36,    -1,    20,    32,    33,    64,
      34,    -1,    20,    35,    33,    64,    34,    -1,    20,    32,
      33,    64,    36,    -1,    20,    35,    33,    64,    36,    -1,
      20,    32,    64,    33,    34,    -1,    20,    35,    64,    33,
      34,    -1,    20,    32,    64,    33,    36,    -1,    20,    35,
      64,    33,    36,    -1,    20,    32,    64,    33,    64,    34,
      10,    28,    64,    -1,    20,    35,    64,    33,    64,    34,
      10,    28,    64,    -1,    20,    32,    64,    33,    64,    36,
      10,    28,    64,    -1,    20,    35,    64,    33,    64,    36,
      10,    28,    64,    -1,    20,    32,    33,    64,    34,    10,
      28,    64,    -1,    20,    35,    33,    64,    34,    10,    28,
      64,    -1,    20,    32,    33,    64,    36,    10,    28,    64,
      -1,    20,    35,    33,    64,    36,    10,    28,    64,    -1,
      20,    32,    64,    33,    34,    10,    28,    64,    -1,    20,
      35,    64,    33,    34,    10,    28,    64,    -1,    20,    32,
      64,    33,    36,    10,    28,    64,    -1,    20,    35,    64,
      33,    36,    10,    28,    64,    -1,    21,    -1,    21,    10,
      28,    64,    -1,    21,    32,    64,    33,    64,    34,    -1,
      21,    35,    64,    33,    64,    34,    -1,    21,    32,    64,
      33,    64,    36,    -1,    21,    35,    64,    33,    64,    36,
      -1,    21,    32,    33,    64,    34,    -1,    21,    35,    33,
      64,    34,    -1,    21,    32,    33,    64,    36,    -1,    21,
      35,    33,    64,    36,    -1,    21,    32,    64,    33,    34,
      -1,    21,    35,    64,    33,    34,    -1,    21,    32,    64,
      33,    36,    -1,    21,    35,    64,    33,    36,    -1,    21,
      32,    64,    33,    64,    34,    10,    28,    64,    -1,    21,
      35,    64,    33,    64,    34,    10,    28,    64,    -1,    21,
      32,    64,    33,    64,    36,    10,    28,    64,    -1,    21,
      35,    64,    33,    64,    36,    10,    28,    64,    -1,    21,
      32,    33,    64,    34,    10,    28,    64,    -1,    21,    35,
      33,    64,    34,    10,    28,    64,    -1,    21,    32,    33,
      64,    36,    10,    28,    64,    -1,    21,    35,    33,    64,
      36,    10,    28,    64,    -1,    21,    32,    64,    33,    34,
      10,    28,    64,    -1,    21,    35,    64,    33,    34,    10,
      28,    64,    -1,    21,    32,    64,    33,    36,    10,    28,
      64,    -1,    21,    35,    64,    33,    36,    10,    28,    64,
      -1,    22,    -1,    22,    12,    28,    64,    -1,    22,    10,
      28,    17,    -1,    22,    12,    28,    64,    10,    28,    17,
      -1,    22,    10,    28,    17,    12,    28,    64,    -1,    14,
      32,    64,    34,    -1,    24,    37,    19,    38,    -1,    24,
      37,    20,    38,    -1,    24,    37,    21,    38,    -1,    24,
      37,    22,    38,    -1,    24,    37,    23,    38,    -1,    24,
      37,    14,    38,    -1,    24,    37,     6,    15,    38,    -1,
      24,    37,     6,     7,    15,    38,    -1,    24,    37,    19,
      38,    10,    28,    11,    -1,    24,    37,    20,    38,    10,
      28,    11,    -1,    24,    37,    21,    38,    10,    28,    11,
      -1,    24,    37,    22,    38,    10,    28,    11,    -1,    24,
      37,    23,    38,    10,    28,    11,    -1,    24,    37,    14,
      38,    10,    28,    11,    -1,    24,    37,     6,    15,    38,
      10,    28,    11,    -1,    24,    37,     6,     7,    15,    38,
      10,    28,    11,    -1,    24,    37,     7,     6,    15,    38,
      10,    28,    11,    -1,     6,    15,    -1,     6,     7,    15,
      -1,     7,     6,    15,    -1,    23,    -1,    13,    65,    30,
      63,    31,    -1,    13,    65,    30,    63,    31,    10,    28,
      15,    -1,    15,    28,    18,    -1,    63,    33,    15,    28,
      18,    -1,    16,    35,    17,    36,    28,    18,    -1,    63,
      33,    16,    35,    17,    36,    28,    18,    -1,    18,    -1,
      39,    18,    -1,    40,    18,    -1,    17,    -1,    15,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    62,    62,    63,    64,    65,    66,    67,    68,    72,
      73,    78,    81,    82,    86,    87,    91,    92,    93,    94,
      98,    99,   103,   104,   105,   106,   110,   114,   115,   116,
     117,   121,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   225,
     226,   227,   228,   229,   237,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   261,   262,   263,   266,   270,   271,   275,   276,
     277,   278,   282,   283,   284,   285,   289
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
  "KW_CHARACTER_STRING", "KW_BLOB", "KW_VECTOR", "KW_PROTO", "KW_SCOPE",
  "';'", "'='", "':'", "'{'", "'}'", "'['", "','", "']'", "'('", "')'",
  "'<'", "'>'", "'+'", "'-'", "$accept", "file", "line_directive", "scope",
  "message_alias", "message_begin", "message_and_body", "message",
  "publishable_begin", "publishable", "struct_begin", "struct",
  "data_type", "integer_type", "unsigned_integer_type", "real_type",
  "character_string_type", "byte_array_type", "vector_type", "struct_type",
  "blob_type", "inline_enum_type", "enum_values", "expr", "qname", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,    59,    61,    58,
     123,   125,    91,    44,    93,    40,    41,    60,    62,    43,
      45
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    41,    42,    42,    42,    42,    42,    42,    42,    43,
      43,    44,    45,    45,    46,    46,    47,    47,    47,    47,
      48,    48,    49,    49,    49,    49,    50,    51,    51,    51,
      51,    52,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    57,
      57,    57,    57,    57,    58,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    60,    60,    60,    61,    62,    62,    63,    63,
      63,    63,    64,    64,    64,    64,    65
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     3,
       4,     6,    10,    11,     7,     8,     4,     3,     4,     3,
       3,     3,     3,     4,     4,     3,     3,     3,     4,     4,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     6,     6,     6,     6,     5,     5,     5,
       5,     5,     5,     5,     5,     9,     9,     9,     9,     8,
       8,     8,     8,     8,     8,     8,     8,     1,     4,     6,
       6,     6,     6,     5,     5,     5,     5,     5,     5,     5,
       5,     9,     9,     9,     9,     8,     8,     8,     8,     8,
       8,     8,     8,     1,     4,     6,     6,     6,     6,     5,
       5,     5,     5,     5,     5,     5,     5,     9,     9,     9,
       9,     8,     8,     8,     8,     8,     8,     8,     8,     1,
       4,     4,     7,     7,     4,     4,     4,     4,     4,     4,
       4,     5,     6,     7,     7,     7,     7,     7,     7,     8,
       9,     9,     2,     3,     3,     1,     5,     8,     3,     5,
       6,     8,     1,     2,     2,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     3,     8,
       5,     0,     0,     4,     0,     6,     0,     7,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    41,    67,    93,   119,   145,     0,     0,     0,    32,
      33,    34,    35,    36,    39,    40,    38,    37,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,    27,     0,     9,     0,     0,   142,     0,    17,
     156,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    20,     0,    19,    21,     0,
      25,    26,     0,    30,    31,     0,     0,     0,    23,    28,
      10,     0,   143,   144,     0,   155,   152,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    16,    18,    24,    29,     0,
       0,     0,     0,     0,     0,   153,   154,   124,    42,     0,
       0,     0,     0,    68,     0,     0,     0,     0,    94,     0,
       0,     0,     0,   121,   120,     0,     0,     0,   130,   125,
     126,   127,   128,   129,     0,     0,    11,     0,     0,   146,
       0,    47,    49,    51,    53,     0,    48,    50,    52,    54,
       0,    73,    75,    77,    79,     0,    74,    76,    78,    80,
       0,    99,   101,   103,   105,     0,   100,   102,   104,   106,
       0,     0,     0,     0,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    14,   148,     0,     0,     0,     0,
       0,     0,     0,     0,    43,    45,     0,     0,     0,     0,
      44,    46,     0,     0,     0,     0,    69,    71,     0,     0,
       0,     0,    70,    72,     0,     0,     0,     0,    95,    97,
       0,     0,     0,     0,    96,    98,     0,     0,   132,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    15,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     123,   122,     0,     0,     0,   138,   133,   134,   135,   136,
     137,     0,     0,     0,   147,   149,     0,    59,    61,    63,
      65,     0,     0,    60,    62,    64,    66,     0,     0,    85,
      87,    89,    91,     0,     0,    86,    88,    90,    92,     0,
       0,   111,   113,   115,   117,     0,     0,   112,   114,   116,
     118,     0,     0,     0,   139,     0,     0,    12,   150,     0,
      55,    57,    56,    58,    81,    83,    82,    84,   107,   109,
     108,   110,   140,   141,    13,     0,   151
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,   144,   109,    71
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -75
static const yytype_int16 yypact[] =
{
     -75,     5,   -75,     8,    10,    19,    56,    28,   -75,   -75,
     -75,    48,    83,   -75,   102,   -75,   121,   -75,    33,    55,
      62,    82,   104,    87,    -5,   105,    25,   130,   109,   143,
     125,    -8,    -2,     3,     9,   -75,   127,   157,   158,   -75,
     -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   147,   163,
     176,   164,   167,   180,   168,   172,   185,   175,   191,   177,
     -75,   184,   -75,   188,   -75,   192,   206,   -75,   211,   -75,
     -75,   197,    11,   210,   142,   215,   216,   232,   242,   217,
     244,   270,   218,   219,   293,   -75,   212,   -75,   -75,   224,
     -75,   -75,   225,   -75,   -75,   226,   241,   229,   -75,   -75,
     -75,   243,   -75,   -75,   146,   -75,   -75,   245,   246,   235,
      11,    11,   233,    11,   237,    11,    11,   256,    11,   257,
      11,    11,   260,    11,   261,   259,    11,    38,   272,   258,
     263,   264,   266,   267,   268,   -75,   -75,   -75,   -75,   280,
     277,   284,   289,   283,    26,   -75,   -75,   -75,   -75,    24,
     114,    29,   129,   -75,    30,   138,    39,   149,   -75,    46,
     162,    51,   169,   307,   310,   306,   285,   309,   312,   315,
     316,   317,   318,   319,   313,    64,   -75,   314,   320,   323,
     155,   324,   325,   326,   328,    59,   329,   330,   331,   332,
      65,   333,   334,   335,   336,    77,   337,   338,   339,   340,
      84,   341,   342,   343,   344,   103,   345,   346,   347,   348,
     115,   302,   349,   321,   350,   327,   351,   352,   353,   354,
     355,   356,    70,   357,   -75,   -75,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   368,   367,   369,   370,   371,
     375,   376,   372,   373,   374,   377,   379,   393,   378,   380,
     381,   382,   394,   397,   383,   384,   385,   386,   405,   406,
     389,   390,   391,   392,   411,   412,    11,   407,   413,   398,
     415,   416,   417,   418,   419,   420,   421,   427,   -75,   422,
     408,   423,   424,   426,    11,    11,    11,    11,   425,   428,
      11,    11,    11,    11,   429,   430,    11,    11,    11,    11,
     431,   432,    11,    11,    11,    11,   433,   434,    11,    11,
      11,    11,   435,   436,    11,    11,    11,    11,   437,   438,
     -75,   -75,   439,   440,   441,   -75,   -75,   -75,   -75,   -75,
     -75,   453,   414,   452,   -75,   -75,   399,   -75,   -75,   -75,
     -75,    11,    11,   -75,   -75,   -75,   -75,    11,    11,   -75,
     -75,   -75,   -75,    11,    11,   -75,   -75,   -75,   -75,    11,
      11,   -75,   -75,   -75,   -75,    11,    11,   -75,   -75,   -75,
     -75,    11,    11,   443,   -75,   444,   445,   -75,   -75,   446,
     -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,   -75,   -75,   -75,   -75,   455,   -75
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,   -75,     4,   -75,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,   -75,   -75,   -74,   -75
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
     112,   114,    73,   117,   119,     2,   122,   124,    76,     3,
       4,     5,    63,    79,     6,    18,    50,    20,    53,    82,
      56,    83,    64,    19,    74,    21,    22,    75,   105,   106,
      77,     7,    66,    78,    23,    80,   148,   149,    81,   151,
      67,   153,   154,    25,   156,   165,   158,   159,    57,   161,
     107,   108,   164,   166,    26,    27,    28,   179,   181,   180,
     182,    29,    30,   186,   191,   187,   192,    31,    32,    33,
      34,    35,    36,   196,    24,   197,   185,    59,   190,    37,
     201,   195,   202,   200,    58,   206,   205,   207,   210,    26,
      27,    48,   223,   234,   224,   235,    29,    30,   277,   240,
     278,   241,    31,    32,    33,    34,    35,    36,    26,    27,
      51,   246,    60,   247,    49,    29,    30,    62,   252,    61,
     253,    31,    32,    33,    34,    35,    36,    26,    27,    54,
      65,   105,   106,    52,    29,    30,    68,   258,    69,   259,
      31,    32,    33,    34,    35,    36,   105,   106,   183,   264,
     184,   265,    55,   107,   108,   105,   106,    72,    70,   105,
     106,   142,   143,   188,    84,   189,   105,   106,   107,   108,
     228,   229,   193,    86,   194,   111,    87,   107,   108,   105,
     106,   107,   108,   198,    85,   199,   105,   106,   107,   108,
      88,    89,   320,    90,    91,    92,   203,    93,   204,    94,
      95,   107,   108,   208,    96,   209,    97,    98,   107,   108,
     337,   338,   339,   340,    99,   100,   343,   344,   345,   346,
     101,   102,   349,   350,   351,   352,   103,   104,   355,   356,
     357,   358,   105,   106,   361,   362,   363,   364,   110,   135,
     367,   368,   369,   370,   115,   120,   125,   126,   113,   105,
     106,   136,   137,   138,   107,   108,   139,   140,   141,   105,
     106,   105,   106,   145,   146,   116,   150,   380,   381,   147,
     152,   107,   108,   382,   383,   118,   163,   121,   167,   384,
     385,   107,   108,   107,   108,   386,   387,   105,   106,   155,
     157,   388,   389,   160,   162,   175,   168,   390,   391,   127,
     128,   169,   170,   123,   171,   172,   173,   129,   174,   107,
     108,   176,   130,   131,   132,   133,   134,   177,   178,   211,
     212,   213,   216,   214,   215,   217,   218,   219,   220,   221,
     266,   222,   225,   227,   230,   231,   232,   226,   233,   236,
     237,   238,   239,   242,   243,   244,   245,   248,   249,   250,
     251,   254,   255,   256,   257,   260,   261,   262,   263,   268,
     269,     0,     0,   279,     0,   270,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   288,   267,   289,   271,
     272,   273,   274,   275,   276,   294,   295,   281,   282,   300,
     284,   285,   286,   287,   280,   290,   283,   291,   292,   293,
     296,   297,   298,   301,   306,   299,   302,   307,   303,   304,
     305,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   322,   321,   324,   323,   325,   326,   327,
     328,   329,   330,   331,     0,   379,   333,   332,   334,     0,
       0,   377,   335,   336,     0,     0,     0,     0,     0,     0,
       0,   374,     0,   341,   392,   393,   342,   347,   348,   353,
     354,   359,   360,   365,   366,   371,   372,   373,   376,   375,
     378,     0,   394,   396,   395
};

static const yytype_int16 yycheck[] =
{
      74,    75,    10,    77,    78,     0,    80,    81,    10,     4,
       5,     6,    17,    10,     9,     7,    12,     7,    14,    10,
      16,    12,    27,    15,    32,    15,     7,    35,    17,    18,
      32,    26,     7,    35,    15,    32,   110,   111,    35,   113,
      15,   115,   116,    15,   118,     7,   120,   121,    15,   123,
      39,    40,   126,    15,     6,     7,     8,    31,    34,    33,
      36,    13,    14,    34,    34,    36,    36,    19,    20,    21,
      22,    23,    24,    34,    18,    36,   150,    15,   152,    31,
      34,   155,    36,   157,    29,    34,   160,    36,   162,     6,
       7,     8,    28,    34,    30,    36,    13,    14,    28,    34,
      30,    36,    19,    20,    21,    22,    23,    24,     6,     7,
       8,    34,    30,    36,    31,    13,    14,    30,    34,    15,
      36,    19,    20,    21,    22,    23,    24,     6,     7,     8,
      25,    17,    18,    31,    13,    14,     6,    34,    29,    36,
      19,    20,    21,    22,    23,    24,    17,    18,    34,    34,
      36,    36,    31,    39,    40,    17,    18,    32,    15,    17,
      18,    15,    16,    34,    37,    36,    17,    18,    39,    40,
      15,    16,    34,    15,    36,    33,    29,    39,    40,    17,
      18,    39,    40,    34,    27,    36,    17,    18,    39,    40,
      27,    15,   266,    29,    27,    15,    34,    29,    36,    27,
      15,    39,    40,    34,    29,    36,    15,    30,    39,    40,
     284,   285,   286,   287,    30,    27,   290,   291,   292,   293,
      28,    15,   296,   297,   298,   299,    15,    30,   302,   303,
     304,   305,    17,    18,   308,   309,   310,   311,    28,    27,
     314,   315,   316,   317,    28,    28,    28,    28,    33,    17,
      18,    27,    27,    27,    39,    40,    15,    28,    15,    17,
      18,    17,    18,    18,    18,    33,    33,   341,   342,    34,
      33,    39,    40,   347,   348,    33,    17,    33,     6,   353,
     354,    39,    40,    39,    40,   359,   360,    17,    18,    33,
      33,   365,   366,    33,    33,    18,    38,   371,   372,     6,
       7,    38,    38,    33,    38,    38,    38,    14,    28,    39,
      40,    27,    19,    20,    21,    22,    23,    28,    35,    12,
      10,    15,    10,    38,    15,    10,    10,    10,    10,    10,
      28,    18,    18,    10,    10,    10,    10,    17,    10,    10,
      10,    10,    10,    10,    10,    10,    10,    10,    10,    10,
      10,    10,    10,    10,    10,    10,    10,    10,    10,    38,
      10,    -1,    -1,     6,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    28,    10,    28,
      28,    28,    28,    28,    28,    10,    10,    28,    28,    10,
      28,    28,    28,    28,    36,    28,    35,    28,    28,    28,
      28,    28,    28,    10,    10,    28,    28,    10,    28,    28,
      28,    28,    28,    28,    28,    10,    10,    28,    28,    28,
      28,    10,    10,    10,    17,    10,    28,    11,    11,    11,
      11,    11,    11,     6,    -1,    36,    28,    15,    15,    -1,
      -1,    27,    18,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    11,    -1,    28,    11,    11,    28,    28,    28,    28,
      28,    28,    28,    28,    28,    28,    28,    28,    15,    28,
      18,    -1,    27,    18,    28
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    42,     0,     4,     5,     6,     9,    26,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,     7,    15,
       7,    15,     7,    15,    18,    15,     6,     7,     8,    13,
      14,    19,    20,    21,    22,    23,    24,    31,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,     8,    31,
      53,     8,    31,    53,     8,    31,    53,    15,    29,    15,
      30,    15,    30,    17,    27,    25,     7,    15,     6,    29,
      15,    65,    32,    10,    32,    35,    10,    32,    35,    10,
      32,    35,    10,    12,    37,    27,    15,    29,    27,    15,
      29,    27,    15,    29,    27,    15,    29,    15,    30,    30,
      27,    28,    15,    15,    30,    17,    18,    39,    40,    64,
      28,    33,    64,    33,    64,    28,    33,    64,    33,    64,
      28,    33,    64,    33,    64,    28,    28,     6,     7,    14,
      19,    20,    21,    22,    23,    27,    27,    27,    27,    15,
      28,    15,    15,    16,    63,    18,    18,    34,    64,    64,
      33,    64,    33,    64,    64,    33,    64,    33,    64,    64,
      33,    64,    33,    17,    64,     7,    15,     6,    38,    38,
      38,    38,    38,    38,    28,    18,    27,    28,    35,    31,
      33,    34,    36,    34,    36,    64,    34,    36,    34,    36,
      64,    34,    36,    34,    36,    64,    34,    36,    34,    36,
      64,    34,    36,    34,    36,    64,    34,    36,    34,    36,
      64,    12,    10,    15,    38,    15,    10,    10,    10,    10,
      10,    10,    18,    28,    30,    18,    17,    10,    15,    16,
      10,    10,    10,    10,    34,    36,    10,    10,    10,    10,
      34,    36,    10,    10,    10,    10,    34,    36,    10,    10,
      10,    10,    34,    36,    10,    10,    10,    10,    34,    36,
      10,    10,    10,    10,    34,    36,    28,    28,    38,    10,
      38,    28,    28,    28,    28,    28,    28,    28,    30,     6,
      36,    28,    28,    35,    28,    28,    28,    28,    10,    10,
      28,    28,    28,    28,    10,    10,    28,    28,    28,    28,
      10,    10,    28,    28,    28,    28,    10,    10,    28,    28,
      28,    28,    10,    10,    28,    28,    28,    28,    10,    10,
      64,    17,    10,    28,    10,    11,    11,    11,    11,    11,
      11,     6,    15,    28,    15,    18,    17,    64,    64,    64,
      64,    28,    28,    64,    64,    64,    64,    28,    28,    64,
      64,    64,    64,    28,    28,    64,    64,    64,    64,    28,
      28,    64,    64,    64,    64,    28,    28,    64,    64,    64,
      64,    28,    28,    28,    11,    28,    15,    27,    18,    36,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    11,    11,    27,    28,    18
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
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1417 "global_mq_IDL.tab.c"
	break;
      case 4: /* "KW_MESSAGE" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1426 "global_mq_IDL.tab.c"
	break;
      case 5: /* "KW_PUBLISHABLE" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1435 "global_mq_IDL.tab.c"
	break;
      case 6: /* "KW_STRUCT" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1444 "global_mq_IDL.tab.c"
	break;
      case 7: /* "KW_NONEXTENDABLE" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1453 "global_mq_IDL.tab.c"
	break;
      case 8: /* "KW_EXTENSION" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1462 "global_mq_IDL.tab.c"
	break;
      case 9: /* "KW_HASH_LINE" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1471 "global_mq_IDL.tab.c"
	break;
      case 10: /* "KW_DEFAULT" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1480 "global_mq_IDL.tab.c"
	break;
      case 11: /* "KW_EMPTY" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1489 "global_mq_IDL.tab.c"
	break;
      case 12: /* "KW_MAX_LENGTH" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1498 "global_mq_IDL.tab.c"
	break;
      case 13: /* "KW_ENUM" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1507 "global_mq_IDL.tab.c"
	break;
      case 14: /* "KW_BYTE_ARRAY" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1516 "global_mq_IDL.tab.c"
	break;
      case 15: /* "IDENTIFIER" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1525 "global_mq_IDL.tab.c"
	break;
      case 16: /* "KW_IDENTIFIER" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1534 "global_mq_IDL.tab.c"
	break;
      case 17: /* "STRING_LITERAL" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1543 "global_mq_IDL.tab.c"
	break;
      case 18: /* "INTEGER_LITERAL" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1552 "global_mq_IDL.tab.c"
	break;
      case 19: /* "KW_INTEGER" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1561 "global_mq_IDL.tab.c"
	break;
      case 20: /* "KW_UINTEGER" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1570 "global_mq_IDL.tab.c"
	break;
      case 21: /* "KW_REAL" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1579 "global_mq_IDL.tab.c"
	break;
      case 22: /* "KW_CHARACTER_STRING" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1588 "global_mq_IDL.tab.c"
	break;
      case 23: /* "KW_BLOB" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1597 "global_mq_IDL.tab.c"
	break;
      case 24: /* "KW_VECTOR" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1606 "global_mq_IDL.tab.c"
	break;
      case 25: /* "KW_PROTO" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1615 "global_mq_IDL.tab.c"
	break;
      case 26: /* "KW_SCOPE" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1624 "global_mq_IDL.tab.c"
	break;
      case 27: /* "';'" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1633 "global_mq_IDL.tab.c"
	break;
      case 28: /* "'='" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1642 "global_mq_IDL.tab.c"
	break;
      case 29: /* "':'" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1651 "global_mq_IDL.tab.c"
	break;
      case 30: /* "'{'" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1660 "global_mq_IDL.tab.c"
	break;
      case 31: /* "'}'" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1669 "global_mq_IDL.tab.c"
	break;
      case 32: /* "'['" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1678 "global_mq_IDL.tab.c"
	break;
      case 33: /* "','" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1687 "global_mq_IDL.tab.c"
	break;
      case 34: /* "']'" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1696 "global_mq_IDL.tab.c"
	break;
      case 35: /* "'('" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1705 "global_mq_IDL.tab.c"
	break;
      case 36: /* "')'" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1714 "global_mq_IDL.tab.c"
	break;
      case 37: /* "'<'" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1723 "global_mq_IDL.tab.c"
	break;
      case 38: /* "'>'" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1732 "global_mq_IDL.tab.c"
	break;
      case 39: /* "'+'" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1741 "global_mq_IDL.tab.c"
	break;
      case 40: /* "'-'" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1750 "global_mq_IDL.tab.c"
	break;
      case 42: /* "file" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1759 "global_mq_IDL.tab.c"
	break;
      case 43: /* "line_directive" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1768 "global_mq_IDL.tab.c"
	break;
      case 44: /* "scope" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1777 "global_mq_IDL.tab.c"
	break;
      case 45: /* "message_alias" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1786 "global_mq_IDL.tab.c"
	break;
      case 46: /* "message_begin" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1795 "global_mq_IDL.tab.c"
	break;
      case 47: /* "message_and_body" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1804 "global_mq_IDL.tab.c"
	break;
      case 48: /* "message" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1813 "global_mq_IDL.tab.c"
	break;
      case 49: /* "publishable_begin" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1822 "global_mq_IDL.tab.c"
	break;
      case 50: /* "publishable" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1831 "global_mq_IDL.tab.c"
	break;
      case 51: /* "struct_begin" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1840 "global_mq_IDL.tab.c"
	break;
      case 52: /* "struct" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1849 "global_mq_IDL.tab.c"
	break;
      case 53: /* "data_type" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1858 "global_mq_IDL.tab.c"
	break;
      case 54: /* "integer_type" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1867 "global_mq_IDL.tab.c"
	break;
      case 55: /* "unsigned_integer_type" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1876 "global_mq_IDL.tab.c"
	break;
      case 56: /* "real_type" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1885 "global_mq_IDL.tab.c"
	break;
      case 57: /* "character_string_type" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1894 "global_mq_IDL.tab.c"
	break;
      case 58: /* "byte_array_type" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1903 "global_mq_IDL.tab.c"
	break;
      case 59: /* "vector_type" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1912 "global_mq_IDL.tab.c"
	break;
      case 60: /* "struct_type" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1921 "global_mq_IDL.tab.c"
	break;
      case 61: /* "blob_type" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1930 "global_mq_IDL.tab.c"
	break;
      case 62: /* "inline_enum_type" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1939 "global_mq_IDL.tab.c"
	break;
      case 63: /* "enum_values" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1948 "global_mq_IDL.tab.c"
	break;
      case 64: /* "expr" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1957 "global_mq_IDL.tab.c"
	break;
      case 65: /* "qname" */

/* Line 1000 of yacc.c  */
#line 53 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1966 "global_mq_IDL.tab.c"
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
#line 62 "global_mq_IDL.y"
    { (yyval) = 0; ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 63 "global_mq_IDL.y"
    { (yyval) = 0; releaseYys2((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 64 "global_mq_IDL.y"
    { (yyval) = addMessageToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 65 "global_mq_IDL.y"
    { (yyval) = addMessageToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 66 "global_mq_IDL.y"
    { (yyval) = addPublishableToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 67 "global_mq_IDL.y"
    { (yyval) = addStructToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 68 "global_mq_IDL.y"
    { (yyval) = addScopeToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 72 "global_mq_IDL.y"
    { (yyval) = 0; processLineDirective((yyvsp[(2) - (3)]), 0); releaseYys2((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 73 "global_mq_IDL.y"
    { (yyval) = 0; processLineDirective((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); releaseYys2((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 78 "global_mq_IDL.y"
    { (yyval) = createScope((yyvsp[(1) - (6)]), (yyvsp[(2) - (6)]), (yyvsp[(5) - (6)])); releaseYys3((yyvsp[(3) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 81 "global_mq_IDL.y"
    { (yyval) = createMessageAlias((yyvsp[(1) - (10)]), false, (yyvsp[(2) - (10)]), (yyvsp[(4) - (10)]), (yyvsp[(6) - (10)]), (yyvsp[(9) - (10)])); releaseYys5((yyvsp[(3) - (10)]), (yyvsp[(5) - (10)]), (yyvsp[(7) - (10)]), (yyvsp[(8) - (10)]), (yyvsp[(10) - (10)])); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 82 "global_mq_IDL.y"
    { (yyval) = createMessageAlias((yyvsp[(1) - (11)]), true, (yyvsp[(3) - (11)]), (yyvsp[(5) - (11)]), (yyvsp[(7) - (11)]), (yyvsp[(10) - (11)])); releaseYys6((yyvsp[(2) - (11)]), (yyvsp[(4) - (11)]), (yyvsp[(6) - (11)]), (yyvsp[(8) - (11)]), (yyvsp[(9) - (11)]), (yyvsp[(11) - (11)])); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 86 "global_mq_IDL.y"
    { (yyval) = createMessage((yyvsp[(1) - (7)]), false, (yyvsp[(2) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(6) - (7)])); releaseYys3((yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 87 "global_mq_IDL.y"
    { (yyval) = createMessage((yyvsp[(1) - (8)]), true, (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(7) - (8)])); releaseYys4((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(8) - (8)])); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 91 "global_mq_IDL.y"
    { (yyval) = addToMessage((yyvsp[(1) - (4)]), createAttribute((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))); releaseYys((yyvsp[(4) - (4)])); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 92 "global_mq_IDL.y"
    { (yyval) = insertExtensionMarkerToMessage((yyvsp[(1) - (3)])); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 93 "global_mq_IDL.y"
    { (yyval) = addToMessage((yyvsp[(1) - (4)]), createAttribute((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))); releaseYys((yyvsp[(4) - (4)])); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 94 "global_mq_IDL.y"
    { (yyval) = insertExtensionMarkerToMessage((yyvsp[(1) - (3)])); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 98 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (3)]); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 99 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (3)]); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 103 "global_mq_IDL.y"
    { (yyval) = createPublishable((yyvsp[(1) - (3)]), false, (yyvsp[(2) - (3)])); releaseYys((yyvsp[(3) - (3)])); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 104 "global_mq_IDL.y"
    { (yyval) = createPublishable((yyvsp[(1) - (4)]), true, (yyvsp[(3) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 105 "global_mq_IDL.y"
    { (yyval) = addToPublishable((yyvsp[(1) - (4)]), createAttribute((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))); releaseYys((yyvsp[(4) - (4)])); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 106 "global_mq_IDL.y"
    { (yyval) = insertExtensionMarkerToPublishable((yyvsp[(1) - (3)])); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 110 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (3)]); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 114 "global_mq_IDL.y"
    { (yyval) = createStruct((yyvsp[(1) - (3)]), false, (yyvsp[(2) - (3)])); releaseYys((yyvsp[(3) - (3)])); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 115 "global_mq_IDL.y"
    { (yyval) = createStruct((yyvsp[(1) - (4)]), true, (yyvsp[(3) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 116 "global_mq_IDL.y"
    { (yyval) = addToStruct((yyvsp[(1) - (4)]), createAttribute((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))); releaseYys((yyvsp[(4) - (4)])); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 117 "global_mq_IDL.y"
    { (yyval) = insertExtensionMarkerToStruct((yyvsp[(1) - (3)])); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 121 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (3)]); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 137 "global_mq_IDL.y"
    { (yyval) = createIntegerType((yyvsp[(1) - (1)])); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 138 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefault((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 139 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 140 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 141 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 142 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 143 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 144 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 145 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 146 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 147 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 148 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 149 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 150 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 151 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 152 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 153 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 154 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 155 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 156 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 157 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 158 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 159 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 160 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 161 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 162 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 166 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerType((yyvsp[(1) - (1)])); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 167 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefault((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 168 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 169 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 170 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 171 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 172 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 173 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 174 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 175 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 176 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 177 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 178 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 179 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 180 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 181 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 182 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 183 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 184 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 185 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 186 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 187 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 188 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 189 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 190 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 191 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 195 "global_mq_IDL.y"
    { (yyval) = createRealType((yyvsp[(1) - (1)])); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 196 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefault((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 197 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 198 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 199 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 200 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 201 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 202 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 203 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 204 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 205 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 206 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 207 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 208 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 209 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 210 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 211 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 212 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 213 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 214 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 215 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 216 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 217 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 218 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 219 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 220 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 225 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (1)]), false, 0, false, 0); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 226 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (4)]), true, (yyvsp[(4) - (4)]), false, 0); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 227 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (4)]), false, 0, true, (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 228 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (7)]), true, (yyvsp[(4) - (7)]), true, (yyvsp[(7) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)])); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 229 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (7)]), true, (yyvsp[(7) - (7)]), true, (yyvsp[(4) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)])); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 237 "global_mq_IDL.y"
    { (yyval) = createByteArrayType((yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 241 "global_mq_IDL.y"
    { (yyval) = createVectorOfIntegerType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 242 "global_mq_IDL.y"
    { (yyval) = createVectorOfUintegerType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 243 "global_mq_IDL.y"
    { (yyval) = createVectorOfRealType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 244 "global_mq_IDL.y"
    { (yyval) = createVectorOfCharStringType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 245 "global_mq_IDL.y"
    { (yyval) = createVectorOfBLOBType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 246 "global_mq_IDL.y"
    { (yyval) = createVectorOfByteArrayType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 247 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (5)]), (yyvsp[(4) - (5)]), false, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 248 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (6)]), (yyvsp[(5) - (6)]), true, false); releaseYys4((yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 249 "global_mq_IDL.y"
    { (yyval) = createVectorOfIntegerType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 250 "global_mq_IDL.y"
    { (yyval) = createVectorOfUintegerType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 251 "global_mq_IDL.y"
    { (yyval) = createVectorOfRealType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 252 "global_mq_IDL.y"
    { (yyval) = createVectorOfCharStringType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 253 "global_mq_IDL.y"
    { (yyval) = createVectorOfBLOBType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 254 "global_mq_IDL.y"
    { (yyval) = createVectorOfByteArrayType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 255 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (8)]), (yyvsp[(4) - (8)]), false, true); releaseYys6((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(8) - (8)])); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 256 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (9)]), (yyvsp[(5) - (9)]), true, true); releaseYys7((yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)])); ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 257 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (9)]), (yyvsp[(5) - (9)]), true, true); releaseYys7((yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)])); ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 261 "global_mq_IDL.y"
    { (yyval) = createStructType((yyvsp[(1) - (2)]), false, (yyvsp[(2) - (2)])); ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 262 "global_mq_IDL.y"
    { (yyval) = createStructType((yyvsp[(1) - (3)]), true, (yyvsp[(3) - (3)])); releaseYys( (yyvsp[(2) - (3)]) );;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 263 "global_mq_IDL.y"
    { (yyval) = createStructType((yyvsp[(2) - (3)]), true, (yyvsp[(3) - (3)])); releaseYys( (yyvsp[(1) - (3)]) ); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 266 "global_mq_IDL.y"
    { (yyval) = createBlobType((yyvsp[(1) - (1)])); ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 270 "global_mq_IDL.y"
    { (yyval) = createInlineEnum((yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(4) - (5)])); releaseYys2((yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 271 "global_mq_IDL.y"
    { (yyval) = createInlineEnumWithDefault((yyvsp[(1) - (8)]), (yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(8) - (8)])); releaseYys4((yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 275 "global_mq_IDL.y"
    { (yyval) = addEnumValue(0, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); releaseYys((yyvsp[(2) - (3)])); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 276 "global_mq_IDL.y"
    { (yyval) = addEnumValue((yyvsp[(1) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); releaseYys2((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)])); ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 277 "global_mq_IDL.y"
    { (yyval) = addEnumValue(0, (yyvsp[(3) - (6)]), (yyvsp[(6) - (6)])); releaseYys4((yyvsp[(1) - (6)]), (yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)])); ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 278 "global_mq_IDL.y"
    { (yyval) = addEnumValue((yyvsp[(1) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(8) - (8)])); releaseYys3((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(4) - (8)])); releaseYys2((yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 282 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 283 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(2) - (2)]); releaseYys((yyvsp[(1) - (2)])); ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 284 "global_mq_IDL.y"
    { (yyval) = makeMinusIntLit((yyvsp[(2) - (2)])); releaseYys((yyvsp[(1) - (2)])); ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 285 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 289 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;



/* Line 1455 of yacc.c  */
#line 3293 "global_mq_IDL.tab.c"
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



