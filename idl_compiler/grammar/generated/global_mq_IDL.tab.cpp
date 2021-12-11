
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
#line 57 "global_mq_IDL.y"

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
     KW_DISCRIMINATED_UNION = 262,
     KW_CASE = 263,
     KW_NONEXTENDABLE = 264,
     KW_EXTENSION = 265,
     KW_HASH_LINE = 266,
     KW_DEFAULT = 267,
     KW_EMPTY = 268,
     KW_MAX_LENGTH = 269,
     KW_ENUM = 270,
     KW_BYTE_ARRAY = 271,
     IDENTIFIER = 272,
     KW_IDENTIFIER = 273,
     STRING_LITERAL = 274,
     INTEGER_LITERAL = 275,
     KW_INTEGER = 276,
     KW_UINTEGER = 277,
     KW_REAL = 278,
     KW_CHARACTER_STRING = 279,
     KW_BLOB = 280,
     KW_VECTOR = 281,
     KW_PROTO = 282,
     KW_SCOPE = 283
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
#line 149 "global_mq_IDL.tab.c"

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
#define YYLAST   511

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  165
/* YYNRULES -- Number of states.  */
#define YYNSTATES  430

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   283

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      37,    38,     2,    41,    35,    42,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    31,    29,
      39,    30,    40,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    34,     2,    36,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    32,     2,    33,     2,     2,     2,     2,
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
      25,    26,    27,    28
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    13,    16,    19,    22,
      25,    29,    34,    41,    52,    64,    72,    81,    86,    90,
      95,    99,   103,   107,   113,   120,   125,   129,   133,   137,
     142,   147,   151,   155,   162,   170,   175,   178,   182,   187,
     190,   194,   196,   198,   200,   202,   204,   206,   208,   210,
     212,   214,   219,   226,   233,   240,   247,   253,   259,   265,
     271,   277,   283,   289,   295,   305,   315,   325,   335,   344,
     353,   362,   371,   380,   389,   398,   407,   409,   414,   421,
     428,   435,   442,   448,   454,   460,   466,   472,   478,   484,
     490,   500,   510,   520,   530,   539,   548,   557,   566,   575,
     584,   593,   602,   604,   609,   616,   623,   630,   637,   643,
     649,   655,   661,   667,   673,   679,   685,   695,   705,   715,
     725,   734,   743,   752,   761,   770,   779,   788,   797,   799,
     804,   809,   817,   825,   830,   835,   840,   845,   850,   855,
     860,   866,   873,   881,   889,   897,   905,   913,   921,   930,
     940,   950,   953,   957,   961,   963,   969,   978,   982,   988,
     995,  1004,  1006,  1009,  1012,  1014
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      44,     0,    -1,    -1,    44,    45,    -1,    44,    50,    -1,
      44,    47,    -1,    44,    52,    -1,    44,    54,    -1,    44,
      58,    -1,    44,    46,    -1,    11,    20,    29,    -1,    11,
      20,    19,    29,    -1,    28,    17,    27,    30,    17,    29,
      -1,     4,    17,    31,    17,    30,    20,    30,     6,    17,
      29,    -1,     4,     9,    17,    31,    17,    30,    20,    30,
       6,    17,    29,    -1,     4,    17,    31,    17,    30,    20,
      32,    -1,     4,     9,    17,    31,    17,    30,    20,    32,
      -1,    48,    59,    17,    29,    -1,    48,    10,    31,    -1,
      49,    59,    17,    29,    -1,    49,    10,    31,    -1,    48,
      33,    29,    -1,    49,    33,    29,    -1,     5,    17,    30,
      20,    32,    -1,     5,     9,    17,    30,    20,    32,    -1,
      51,    59,    17,    29,    -1,    51,    10,    31,    -1,    51,
      33,    29,    -1,     6,    17,    32,    -1,     6,     9,    17,
      32,    -1,    53,    59,    17,    29,    -1,    53,    10,    31,
      -1,    53,    33,    29,    -1,     8,    17,    30,    20,    31,
      32,    -1,     8,     9,    17,    30,    20,    31,    32,    -1,
      57,    59,    17,    29,    -1,    55,    33,    -1,     7,    17,
      32,    -1,     7,     9,    17,    32,    -1,    57,    56,    -1,
      57,    33,    29,    -1,    60,    -1,    61,    -1,    62,    -1,
      63,    -1,    64,    -1,    68,    -1,    67,    -1,    65,    -1,
      66,    -1,    21,    -1,    21,    12,    30,    70,    -1,    21,
      34,    70,    35,    70,    36,    -1,    21,    37,    70,    35,
      70,    36,    -1,    21,    34,    70,    35,    70,    38,    -1,
      21,    37,    70,    35,    70,    38,    -1,    21,    34,    35,
      70,    36,    -1,    21,    37,    35,    70,    36,    -1,    21,
      34,    35,    70,    38,    -1,    21,    37,    35,    70,    38,
      -1,    21,    34,    70,    35,    36,    -1,    21,    37,    70,
      35,    36,    -1,    21,    34,    70,    35,    38,    -1,    21,
      37,    70,    35,    38,    -1,    21,    34,    70,    35,    70,
      36,    12,    30,    70,    -1,    21,    37,    70,    35,    70,
      36,    12,    30,    70,    -1,    21,    34,    70,    35,    70,
      38,    12,    30,    70,    -1,    21,    37,    70,    35,    70,
      38,    12,    30,    70,    -1,    21,    34,    35,    70,    36,
      12,    30,    70,    -1,    21,    37,    35,    70,    36,    12,
      30,    70,    -1,    21,    34,    35,    70,    38,    12,    30,
      70,    -1,    21,    37,    35,    70,    38,    12,    30,    70,
      -1,    21,    34,    70,    35,    36,    12,    30,    70,    -1,
      21,    37,    70,    35,    36,    12,    30,    70,    -1,    21,
      34,    70,    35,    38,    12,    30,    70,    -1,    21,    37,
      70,    35,    38,    12,    30,    70,    -1,    22,    -1,    22,
      12,    30,    70,    -1,    22,    34,    70,    35,    70,    36,
      -1,    22,    37,    70,    35,    70,    36,    -1,    22,    34,
      70,    35,    70,    38,    -1,    22,    37,    70,    35,    70,
      38,    -1,    22,    34,    35,    70,    36,    -1,    22,    37,
      35,    70,    36,    -1,    22,    34,    35,    70,    38,    -1,
      22,    37,    35,    70,    38,    -1,    22,    34,    70,    35,
      36,    -1,    22,    37,    70,    35,    36,    -1,    22,    34,
      70,    35,    38,    -1,    22,    37,    70,    35,    38,    -1,
      22,    34,    70,    35,    70,    36,    12,    30,    70,    -1,
      22,    37,    70,    35,    70,    36,    12,    30,    70,    -1,
      22,    34,    70,    35,    70,    38,    12,    30,    70,    -1,
      22,    37,    70,    35,    70,    38,    12,    30,    70,    -1,
      22,    34,    35,    70,    36,    12,    30,    70,    -1,    22,
      37,    35,    70,    36,    12,    30,    70,    -1,    22,    34,
      35,    70,    38,    12,    30,    70,    -1,    22,    37,    35,
      70,    38,    12,    30,    70,    -1,    22,    34,    70,    35,
      36,    12,    30,    70,    -1,    22,    37,    70,    35,    36,
      12,    30,    70,    -1,    22,    34,    70,    35,    38,    12,
      30,    70,    -1,    22,    37,    70,    35,    38,    12,    30,
      70,    -1,    23,    -1,    23,    12,    30,    70,    -1,    23,
      34,    70,    35,    70,    36,    -1,    23,    37,    70,    35,
      70,    36,    -1,    23,    34,    70,    35,    70,    38,    -1,
      23,    37,    70,    35,    70,    38,    -1,    23,    34,    35,
      70,    36,    -1,    23,    37,    35,    70,    36,    -1,    23,
      34,    35,    70,    38,    -1,    23,    37,    35,    70,    38,
      -1,    23,    34,    70,    35,    36,    -1,    23,    37,    70,
      35,    36,    -1,    23,    34,    70,    35,    38,    -1,    23,
      37,    70,    35,    38,    -1,    23,    34,    70,    35,    70,
      36,    12,    30,    70,    -1,    23,    37,    70,    35,    70,
      36,    12,    30,    70,    -1,    23,    34,    70,    35,    70,
      38,    12,    30,    70,    -1,    23,    37,    70,    35,    70,
      38,    12,    30,    70,    -1,    23,    34,    35,    70,    36,
      12,    30,    70,    -1,    23,    37,    35,    70,    36,    12,
      30,    70,    -1,    23,    34,    35,    70,    38,    12,    30,
      70,    -1,    23,    37,    35,    70,    38,    12,    30,    70,
      -1,    23,    34,    70,    35,    36,    12,    30,    70,    -1,
      23,    37,    70,    35,    36,    12,    30,    70,    -1,    23,
      34,    70,    35,    38,    12,    30,    70,    -1,    23,    37,
      70,    35,    38,    12,    30,    70,    -1,    24,    -1,    24,
      14,    30,    70,    -1,    24,    12,    30,    19,    -1,    24,
      14,    30,    70,    12,    30,    19,    -1,    24,    12,    30,
      19,    14,    30,    70,    -1,    16,    34,    70,    36,    -1,
      26,    39,    21,    40,    -1,    26,    39,    22,    40,    -1,
      26,    39,    23,    40,    -1,    26,    39,    24,    40,    -1,
      26,    39,    25,    40,    -1,    26,    39,    16,    40,    -1,
      26,    39,     6,    17,    40,    -1,    26,    39,     6,     9,
      17,    40,    -1,    26,    39,    21,    40,    12,    30,    13,
      -1,    26,    39,    22,    40,    12,    30,    13,    -1,    26,
      39,    23,    40,    12,    30,    13,    -1,    26,    39,    24,
      40,    12,    30,    13,    -1,    26,    39,    25,    40,    12,
      30,    13,    -1,    26,    39,    16,    40,    12,    30,    13,
      -1,    26,    39,     6,    17,    40,    12,    30,    13,    -1,
      26,    39,     6,     9,    17,    40,    12,    30,    13,    -1,
      26,    39,     9,     6,    17,    40,    12,    30,    13,    -1,
       6,    17,    -1,     6,     9,    17,    -1,     9,     6,    17,
      -1,    25,    -1,    15,    71,    32,    69,    33,    -1,    15,
      71,    32,    69,    33,    12,    30,    17,    -1,    17,    30,
      20,    -1,    69,    35,    17,    30,    20,    -1,    18,    37,
      19,    38,    30,    20,    -1,    69,    35,    18,    37,    19,
      38,    30,    20,    -1,    20,    -1,    41,    20,    -1,    42,
      20,    -1,    19,    -1,    17,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    64,    64,    65,    66,    67,    68,    69,    70,    71,
      75,    76,    81,    84,    85,    89,    90,    94,    95,    96,
      97,   101,   102,   106,   107,   108,   109,   113,   117,   118,
     119,   120,   124,   128,   129,   130,   134,   138,   139,   140,
     144,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   248,   249,
     250,   251,   252,   260,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   284,   285,   286,   289,   293,   294,   298,   299,   300,
     301,   305,   306,   307,   308,   312
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "KW_FILE", "KW_MESSAGE",
  "KW_PUBLISHABLE", "KW_STRUCT", "KW_DISCRIMINATED_UNION", "KW_CASE",
  "KW_NONEXTENDABLE", "KW_EXTENSION", "KW_HASH_LINE", "KW_DEFAULT",
  "KW_EMPTY", "KW_MAX_LENGTH", "KW_ENUM", "KW_BYTE_ARRAY", "IDENTIFIER",
  "KW_IDENTIFIER", "STRING_LITERAL", "INTEGER_LITERAL", "KW_INTEGER",
  "KW_UINTEGER", "KW_REAL", "KW_CHARACTER_STRING", "KW_BLOB", "KW_VECTOR",
  "KW_PROTO", "KW_SCOPE", "';'", "'='", "':'", "'{'", "'}'", "'['", "','",
  "']'", "'('", "')'", "'<'", "'>'", "'+'", "'-'", "$accept", "file",
  "line_directive", "scope", "message_alias", "message_begin",
  "message_and_body", "message", "publishable_begin", "publishable",
  "struct_begin", "struct", "discriminated_union_case_begin",
  "discriminated_union_case", "discriminated_union_begin",
  "discriminated_union", "data_type", "integer_type",
  "unsigned_integer_type", "real_type", "character_string_type",
  "byte_array_type", "vector_type", "struct_type", "blob_type",
  "inline_enum_type", "enum_values", "expr", "qname", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,    59,
      61,    58,   123,   125,    91,    44,    93,    40,    41,    60,
      62,    43,    45
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    43,    44,    44,    44,    44,    44,    44,    44,    44,
      45,    45,    46,    47,    47,    48,    48,    49,    49,    49,
      49,    50,    50,    51,    51,    51,    51,    52,    53,    53,
      53,    53,    54,    55,    55,    55,    56,    57,    57,    57,
      58,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    63,    63,
      63,    63,    63,    64,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    66,    66,    66,    67,    68,    68,    69,    69,    69,
      69,    70,    70,    70,    70,    71
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       3,     4,     6,    10,    11,     7,     8,     4,     3,     4,
       3,     3,     3,     5,     6,     4,     3,     3,     3,     4,
       4,     3,     3,     6,     7,     4,     2,     3,     4,     2,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     6,     6,     6,     6,     5,     5,     5,     5,
       5,     5,     5,     5,     9,     9,     9,     9,     8,     8,
       8,     8,     8,     8,     8,     8,     1,     4,     6,     6,
       6,     6,     5,     5,     5,     5,     5,     5,     5,     5,
       9,     9,     9,     9,     8,     8,     8,     8,     8,     8,
       8,     8,     1,     4,     6,     6,     6,     6,     5,     5,
       5,     5,     5,     5,     5,     5,     9,     9,     9,     9,
       8,     8,     8,     8,     8,     8,     8,     8,     1,     4,
       4,     7,     7,     4,     4,     4,     4,     4,     4,     4,
       5,     6,     7,     7,     7,     7,     7,     7,     8,     9,
       9,     2,     3,     3,     1,     5,     8,     3,     5,     6,
       8,     1,     2,     2,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     3,
       9,     5,     0,     0,     4,     0,     6,     0,     7,     0,
       8,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    76,   102,   128,
     154,     0,     0,     0,    41,    42,    43,    44,    45,    48,
      49,    47,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
       0,     0,    28,     0,    37,     0,    10,     0,     0,   151,
       0,    18,   165,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,    20,
      22,     0,    26,    27,     0,    31,    32,     0,     0,     0,
      40,    36,     0,     0,     0,     0,     0,    29,    38,    11,
       0,   152,   153,     0,   164,   161,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    17,    19,    25,    30,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,   162,
     163,   133,    51,     0,     0,     0,     0,    77,     0,     0,
       0,     0,   103,     0,     0,     0,     0,   130,   129,     0,
       0,     0,   139,   134,   135,   136,   137,   138,     0,     0,
      35,     0,     0,    24,    12,     0,     0,   155,     0,    56,
      58,    60,    62,     0,    57,    59,    61,    63,     0,    82,
      84,    86,    88,     0,    83,    85,    87,    89,     0,   108,
     110,   112,   114,     0,   109,   111,   113,   115,     0,     0,
       0,     0,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    15,   157,     0,     0,     0,     0,
       0,     0,     0,     0,    52,    54,     0,     0,     0,     0,
      53,    55,     0,     0,     0,     0,    78,    80,     0,     0,
       0,     0,    79,    81,     0,     0,     0,     0,   104,   106,
       0,     0,     0,     0,   105,   107,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
      16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   132,   131,     0,     0,     0,   147,   142,   143,
     144,   145,   146,    34,     0,     0,     0,   156,   158,     0,
      68,    70,    72,    74,     0,     0,    69,    71,    73,    75,
       0,     0,    94,    96,    98,   100,     0,     0,    95,    97,
      99,   101,     0,     0,   120,   122,   124,   126,     0,     0,
     121,   123,   125,   127,     0,     0,     0,   148,     0,     0,
      13,   159,     0,    64,    66,    65,    67,    90,    92,    91,
      93,   116,   118,   117,   119,   149,   150,    14,     0,   160
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    64,    65,    66,    20,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,   168,   128,    83
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -87
static const yytype_int16 yypact[] =
{
     -87,     9,   -87,    17,    21,    23,    41,   -15,     1,   -87,
     -87,   -87,    54,    93,   -87,   114,   -87,   135,   -87,    14,
     -87,    18,    -3,    24,    15,    35,    30,    38,    39,     0,
       4,    48,    66,    55,    84,    70,   -10,    -1,     5,    11,
     -87,    68,    83,    96,   -87,   -87,   -87,   -87,   -87,   -87,
     -87,   -87,   -87,    94,   102,   105,   103,   113,   126,   115,
     123,   138,    57,   145,   144,   -87,   308,   156,   191,   160,
     177,   168,   -87,   181,   -87,   188,   -87,   182,   203,   -87,
     206,   -87,   -87,   194,    91,   197,    86,   134,   204,   187,
     233,   205,   245,   250,   210,   211,   319,   -87,   217,   -87,
     -87,   218,   -87,   -87,   229,   -87,   -87,   230,   249,   237,
     -87,   -87,   255,   273,   263,   277,   267,   -87,   -87,   -87,
     285,   -87,   -87,    50,   -87,   -87,   283,   284,   269,    91,
      91,   274,    91,   275,    91,    91,   276,    91,   286,    91,
      91,   287,    91,   291,   289,    91,    64,   314,   296,   297,
     298,   299,   305,   306,   -87,   -87,   -87,   -87,   317,   307,
     320,   318,   330,   321,   -87,   322,   324,   315,    49,   -87,
     -87,   -87,   -87,    47,   129,    53,   143,   -87,    56,   153,
     128,   163,   -87,   142,   173,   150,   183,   341,   344,   340,
     323,   342,   346,   348,   349,   350,   352,   353,   347,   335,
     -87,   351,   166,   -87,   -87,   354,   356,   357,    79,   358,
     360,   361,   364,   180,   365,   366,   367,   368,   235,   369,
     370,   371,   372,   240,   373,   374,   375,   376,   241,   377,
     378,   379,   380,   246,   381,   382,   383,   384,   260,   338,
     385,   359,   386,   362,   387,   388,   389,   390,   391,   392,
     393,   394,   251,   395,   -87,   -87,   396,   397,   398,   363,
     399,   400,   401,   402,   404,   411,   403,   405,   406,   407,
     413,   426,   409,   410,   412,   414,   429,   431,   415,   416,
     417,   418,   437,   438,   421,   422,   423,   424,   443,   444,
     427,   428,   430,   432,   447,   449,    91,   445,   451,   435,
     454,   455,   456,   457,   458,   459,   460,   442,   -87,   408,
     -87,   450,   446,   461,   462,   464,    91,    91,    91,    91,
     463,   465,    91,    91,    91,    91,   466,   467,    91,    91,
      91,    91,   468,   469,    91,    91,    91,    91,   470,   471,
      91,    91,    91,    91,   472,   473,    91,    91,    91,    91,
     474,   475,   -87,   -87,   476,   477,   478,   -87,   -87,   -87,
     -87,   -87,   -87,   -87,   490,   448,   489,   -87,   -87,   441,
     -87,   -87,   -87,   -87,    91,    91,   -87,   -87,   -87,   -87,
      91,    91,   -87,   -87,   -87,   -87,    91,    91,   -87,   -87,
     -87,   -87,    91,    91,   -87,   -87,   -87,   -87,    91,    91,
     -87,   -87,   -87,   -87,    91,    91,   479,   -87,   481,   452,
     -87,   -87,   480,   -87,   -87,   -87,   -87,   -87,   -87,   -87,
     -87,   -87,   -87,   -87,   -87,   -87,   -87,   -87,   491,   -87
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -87,   -87,   -87,   -87,   -87,   -87,   -87,   -87,   -87,   -87,
     -87,   -87,   -87,   -87,   483,   -87,    -5,   -87,   -87,   -87,
     -87,   -87,   -87,   -87,   -87,   -87,   -87,   -86,   -87
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
     131,   133,    85,   136,   138,    29,   141,   143,    55,     2,
      58,    88,    61,     3,     4,     5,     6,    91,    30,    75,
       7,     6,    62,    94,    86,    95,    21,    87,    68,    76,
      23,    77,    25,    89,    22,    67,    90,     8,    24,    92,
      26,    69,    93,   172,   173,    70,   175,    63,   177,   178,
      27,   180,    71,   182,   183,    73,   185,    78,    28,   188,
      31,   112,    72,    32,    33,    79,   108,   166,   167,    34,
      35,    74,    80,   189,   109,    36,    37,    38,    39,    40,
      41,   190,   207,   209,   208,   210,    81,    42,   213,   214,
     218,   215,   219,   223,   220,   228,   258,   259,   233,    31,
     238,    82,    32,    53,    84,   124,   125,    96,    34,    35,
     124,   125,    97,    98,    36,    37,    38,    39,    40,    41,
      31,   130,   101,    32,    56,    99,    54,   126,   127,    34,
      35,   100,   126,   127,   102,    36,    37,    38,    39,    40,
      41,    31,   103,   104,    32,    59,   105,    57,   124,   125,
      34,    35,   106,   124,   125,   107,    36,    37,    38,    39,
      40,    41,   124,   125,   224,   211,   225,   212,    60,   132,
     126,   127,   124,   125,   110,   126,   127,   111,   229,   216,
     230,   217,   124,   125,   126,   127,   234,   113,   235,   221,
     115,   222,   124,   125,   126,   127,   253,   116,   254,   226,
     117,   227,   124,   125,   126,   127,   124,   125,   114,   231,
     352,   232,   120,   118,   126,   127,   264,   119,   265,   236,
     121,   237,   135,   122,   126,   127,   123,   129,   126,   127,
     370,   371,   372,   373,   134,   139,   376,   377,   378,   379,
     144,   145,   382,   383,   384,   385,   154,   155,   388,   389,
     390,   391,   124,   125,   394,   395,   396,   397,   156,   157,
     400,   401,   402,   403,   124,   125,   158,   159,   137,   124,
     125,   270,   160,   271,   126,   127,   276,   282,   277,   283,
     140,   309,   288,   310,   289,   142,   126,   127,   413,   414,
     161,   126,   127,   162,   415,   416,   294,   163,   295,   164,
     417,   418,   165,   169,   170,   171,   419,   420,   187,   174,
     176,   179,   421,   422,    31,     6,    62,    32,   423,   424,
     191,   181,   184,    34,    35,   146,   186,   199,   147,    36,
      37,    38,    39,    40,    41,   148,   192,   193,   194,   195,
     149,   150,   151,   152,   153,   196,   197,   198,   201,   200,
     202,   204,   206,   203,   205,   239,   240,   241,   244,   243,
     245,   246,   247,   242,   248,   249,   251,   250,   296,   257,
     260,   252,   261,   262,   255,   256,   263,   266,   267,   268,
     269,   272,   273,   274,   275,   278,   279,   280,   281,   284,
     285,   286,   287,   290,   291,   292,   293,     0,   299,   298,
     315,   311,   300,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   364,   297,   320,   301,   302,   303,
     304,   305,   306,   321,   307,   326,   308,   313,   314,   316,
     317,   318,   319,   322,   312,   323,   324,   325,   327,   328,
     329,   332,   330,   333,   331,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   350,
     348,   351,   349,   354,   353,   355,   356,   365,   357,   358,
     359,   360,   361,   362,   363,     0,   366,   410,   367,   412,
       0,   427,   368,   369,    19,     0,     0,     0,     0,     0,
     407,     0,   425,   374,   426,   375,   380,   381,   386,   387,
     392,   393,   398,   399,   404,   405,   406,   409,   408,   411,
     428,   429
};

static const yytype_int16 yycheck[] =
{
      86,    87,    12,    89,    90,    20,    92,    93,    13,     0,
      15,    12,    17,     4,     5,     6,     7,    12,    17,    19,
      11,     7,     8,    12,    34,    14,     9,    37,    31,    29,
       9,    27,     9,    34,    17,    17,    37,    28,    17,    34,
      17,    17,    37,   129,   130,    30,   132,    33,   134,   135,
       9,   137,    17,   139,   140,    17,   142,     9,    17,   145,
       6,    66,    32,     9,    10,    17,     9,    17,    18,    15,
      16,    32,     6,     9,    17,    21,    22,    23,    24,    25,
      26,    17,    33,    36,    35,    38,    31,    33,   174,    36,
     176,    38,    36,   179,    38,   181,    17,    18,   184,     6,
     186,    17,     9,    10,    34,    19,    20,    39,    15,    16,
      19,    20,    29,    17,    21,    22,    23,    24,    25,    26,
       6,    35,    17,     9,    10,    31,    33,    41,    42,    15,
      16,    29,    41,    42,    31,    21,    22,    23,    24,    25,
      26,     6,    29,    17,     9,    10,    31,    33,    19,    20,
      15,    16,    29,    19,    20,    17,    21,    22,    23,    24,
      25,    26,    19,    20,    36,    36,    38,    38,    33,    35,
      41,    42,    19,    20,    29,    41,    42,    33,    36,    36,
      38,    38,    19,    20,    41,    42,    36,    31,    38,    36,
      30,    38,    19,    20,    41,    42,    30,    20,    32,    36,
      32,    38,    19,    20,    41,    42,    19,    20,    17,    36,
     296,    38,    30,    32,    41,    42,    36,    29,    38,    36,
      17,    38,    35,    17,    41,    42,    32,    30,    41,    42,
     316,   317,   318,   319,    30,    30,   322,   323,   324,   325,
      30,    30,   328,   329,   330,   331,    29,    29,   334,   335,
     336,   337,    19,    20,   340,   341,   342,   343,    29,    29,
     346,   347,   348,   349,    19,    20,    17,    30,    35,    19,
      20,    36,    17,    38,    41,    42,    36,    36,    38,    38,
      35,    30,    36,    32,    38,    35,    41,    42,   374,   375,
      17,    41,    42,    30,   380,   381,    36,    20,    38,    32,
     386,   387,    17,    20,    20,    36,   392,   393,    19,    35,
      35,    35,   398,   399,     6,     7,     8,     9,   404,   405,
       6,    35,    35,    15,    16,     6,    35,    20,     9,    21,
      22,    23,    24,    25,    26,    16,    40,    40,    40,    40,
      21,    22,    23,    24,    25,    40,    40,    30,    30,    29,
      20,    29,    37,    32,    30,    14,    12,    17,    12,    17,
      12,    12,    12,    40,    12,    12,    31,    20,    30,    12,
      12,    20,    12,    12,    20,    19,    12,    12,    12,    12,
      12,    12,    12,    12,    12,    12,    12,    12,    12,    12,
      12,    12,    12,    12,    12,    12,    12,    -1,    12,    40,
      37,     6,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     6,    30,    12,    30,    30,    30,
      30,    30,    30,    12,    31,    12,    32,    30,    30,    30,
      30,    30,    30,    30,    38,    30,    30,    30,    12,    30,
      30,    12,    30,    12,    30,    30,    30,    30,    30,    12,
      12,    30,    30,    30,    30,    12,    12,    30,    30,    12,
      30,    12,    30,    12,    19,    30,    12,    17,    13,    13,
      13,    13,    13,    13,    32,    -1,    30,    29,    17,    38,
      -1,    29,    20,    19,     1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    13,    30,    13,    30,    30,    30,    30,    30,
      30,    30,    30,    30,    30,    30,    30,    17,    30,    20,
      30,    20
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    44,     0,     4,     5,     6,     7,    11,    28,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    57,
      58,     9,    17,     9,    17,     9,    17,     9,    17,    20,
      17,     6,     9,    10,    15,    16,    21,    22,    23,    24,
      25,    26,    33,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    10,    33,    59,    10,    33,    59,    10,
      33,    59,     8,    33,    55,    56,    57,    17,    31,    17,
      30,    17,    32,    17,    32,    19,    29,    27,     9,    17,
       6,    31,    17,    71,    34,    12,    34,    37,    12,    34,
      37,    12,    34,    37,    12,    14,    39,    29,    17,    31,
      29,    17,    31,    29,    17,    31,    29,    17,     9,    17,
      29,    33,    59,    31,    17,    30,    20,    32,    32,    29,
      30,    17,    17,    32,    19,    20,    41,    42,    70,    30,
      35,    70,    35,    70,    30,    35,    70,    35,    70,    30,
      35,    70,    35,    70,    30,    30,     6,     9,    16,    21,
      22,    23,    24,    25,    29,    29,    29,    29,    17,    30,
      17,    17,    30,    20,    32,    17,    17,    18,    69,    20,
      20,    36,    70,    70,    35,    70,    35,    70,    70,    35,
      70,    35,    70,    70,    35,    70,    35,    19,    70,     9,
      17,     6,    40,    40,    40,    40,    40,    40,    30,    20,
      29,    30,    20,    32,    29,    30,    37,    33,    35,    36,
      38,    36,    38,    70,    36,    38,    36,    38,    70,    36,
      38,    36,    38,    70,    36,    38,    36,    38,    70,    36,
      38,    36,    38,    70,    36,    38,    36,    38,    70,    14,
      12,    17,    40,    17,    12,    12,    12,    12,    12,    12,
      20,    31,    20,    30,    32,    20,    19,    12,    17,    18,
      12,    12,    12,    12,    36,    38,    12,    12,    12,    12,
      36,    38,    12,    12,    12,    12,    36,    38,    12,    12,
      12,    12,    36,    38,    12,    12,    12,    12,    36,    38,
      12,    12,    12,    12,    36,    38,    30,    30,    40,    12,
      40,    30,    30,    30,    30,    30,    30,    31,    32,    30,
      32,     6,    38,    30,    30,    37,    30,    30,    30,    30,
      12,    12,    30,    30,    30,    30,    12,    12,    30,    30,
      30,    30,    12,    12,    30,    30,    30,    30,    12,    12,
      30,    30,    30,    30,    12,    12,    30,    30,    30,    30,
      12,    12,    70,    19,    12,    30,    12,    13,    13,    13,
      13,    13,    13,    32,     6,    17,    30,    17,    20,    19,
      70,    70,    70,    70,    30,    30,    70,    70,    70,    70,
      30,    30,    70,    70,    70,    70,    30,    30,    70,    70,
      70,    70,    30,    30,    70,    70,    70,    70,    30,    30,
      70,    70,    70,    70,    30,    30,    30,    13,    30,    17,
      29,    20,    38,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    13,    13,    29,    30,    20
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
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1448 "global_mq_IDL.tab.c"
	break;
      case 4: /* "KW_MESSAGE" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1457 "global_mq_IDL.tab.c"
	break;
      case 5: /* "KW_PUBLISHABLE" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1466 "global_mq_IDL.tab.c"
	break;
      case 6: /* "KW_STRUCT" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1475 "global_mq_IDL.tab.c"
	break;
      case 7: /* "KW_DISCRIMINATED_UNION" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1484 "global_mq_IDL.tab.c"
	break;
      case 8: /* "KW_CASE" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1493 "global_mq_IDL.tab.c"
	break;
      case 9: /* "KW_NONEXTENDABLE" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1502 "global_mq_IDL.tab.c"
	break;
      case 10: /* "KW_EXTENSION" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1511 "global_mq_IDL.tab.c"
	break;
      case 11: /* "KW_HASH_LINE" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1520 "global_mq_IDL.tab.c"
	break;
      case 12: /* "KW_DEFAULT" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1529 "global_mq_IDL.tab.c"
	break;
      case 13: /* "KW_EMPTY" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1538 "global_mq_IDL.tab.c"
	break;
      case 14: /* "KW_MAX_LENGTH" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1547 "global_mq_IDL.tab.c"
	break;
      case 15: /* "KW_ENUM" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1556 "global_mq_IDL.tab.c"
	break;
      case 16: /* "KW_BYTE_ARRAY" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1565 "global_mq_IDL.tab.c"
	break;
      case 17: /* "IDENTIFIER" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1574 "global_mq_IDL.tab.c"
	break;
      case 18: /* "KW_IDENTIFIER" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1583 "global_mq_IDL.tab.c"
	break;
      case 19: /* "STRING_LITERAL" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1592 "global_mq_IDL.tab.c"
	break;
      case 20: /* "INTEGER_LITERAL" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1601 "global_mq_IDL.tab.c"
	break;
      case 21: /* "KW_INTEGER" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1610 "global_mq_IDL.tab.c"
	break;
      case 22: /* "KW_UINTEGER" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1619 "global_mq_IDL.tab.c"
	break;
      case 23: /* "KW_REAL" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1628 "global_mq_IDL.tab.c"
	break;
      case 24: /* "KW_CHARACTER_STRING" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1637 "global_mq_IDL.tab.c"
	break;
      case 25: /* "KW_BLOB" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1646 "global_mq_IDL.tab.c"
	break;
      case 26: /* "KW_VECTOR" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1655 "global_mq_IDL.tab.c"
	break;
      case 27: /* "KW_PROTO" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1664 "global_mq_IDL.tab.c"
	break;
      case 28: /* "KW_SCOPE" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1673 "global_mq_IDL.tab.c"
	break;
      case 29: /* "';'" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1682 "global_mq_IDL.tab.c"
	break;
      case 30: /* "'='" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1691 "global_mq_IDL.tab.c"
	break;
      case 31: /* "':'" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1700 "global_mq_IDL.tab.c"
	break;
      case 32: /* "'{'" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1709 "global_mq_IDL.tab.c"
	break;
      case 33: /* "'}'" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1718 "global_mq_IDL.tab.c"
	break;
      case 34: /* "'['" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1727 "global_mq_IDL.tab.c"
	break;
      case 35: /* "','" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1736 "global_mq_IDL.tab.c"
	break;
      case 36: /* "']'" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1745 "global_mq_IDL.tab.c"
	break;
      case 37: /* "'('" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1754 "global_mq_IDL.tab.c"
	break;
      case 38: /* "')'" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1763 "global_mq_IDL.tab.c"
	break;
      case 39: /* "'<'" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1772 "global_mq_IDL.tab.c"
	break;
      case 40: /* "'>'" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1781 "global_mq_IDL.tab.c"
	break;
      case 41: /* "'+'" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1790 "global_mq_IDL.tab.c"
	break;
      case 42: /* "'-'" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1799 "global_mq_IDL.tab.c"
	break;
      case 44: /* "file" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1808 "global_mq_IDL.tab.c"
	break;
      case 45: /* "line_directive" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1817 "global_mq_IDL.tab.c"
	break;
      case 46: /* "scope" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1826 "global_mq_IDL.tab.c"
	break;
      case 47: /* "message_alias" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1835 "global_mq_IDL.tab.c"
	break;
      case 48: /* "message_begin" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1844 "global_mq_IDL.tab.c"
	break;
      case 49: /* "message_and_body" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1853 "global_mq_IDL.tab.c"
	break;
      case 50: /* "message" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1862 "global_mq_IDL.tab.c"
	break;
      case 51: /* "publishable_begin" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1871 "global_mq_IDL.tab.c"
	break;
      case 52: /* "publishable" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1880 "global_mq_IDL.tab.c"
	break;
      case 53: /* "struct_begin" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1889 "global_mq_IDL.tab.c"
	break;
      case 54: /* "struct" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1898 "global_mq_IDL.tab.c"
	break;
      case 55: /* "discriminated_union_case_begin" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1907 "global_mq_IDL.tab.c"
	break;
      case 56: /* "discriminated_union_case" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1916 "global_mq_IDL.tab.c"
	break;
      case 57: /* "discriminated_union_begin" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1925 "global_mq_IDL.tab.c"
	break;
      case 58: /* "discriminated_union" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1934 "global_mq_IDL.tab.c"
	break;
      case 59: /* "data_type" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1943 "global_mq_IDL.tab.c"
	break;
      case 60: /* "integer_type" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1952 "global_mq_IDL.tab.c"
	break;
      case 61: /* "unsigned_integer_type" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1961 "global_mq_IDL.tab.c"
	break;
      case 62: /* "real_type" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1970 "global_mq_IDL.tab.c"
	break;
      case 63: /* "character_string_type" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1979 "global_mq_IDL.tab.c"
	break;
      case 64: /* "byte_array_type" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1988 "global_mq_IDL.tab.c"
	break;
      case 65: /* "vector_type" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1997 "global_mq_IDL.tab.c"
	break;
      case 66: /* "struct_type" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 2006 "global_mq_IDL.tab.c"
	break;
      case 67: /* "blob_type" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 2015 "global_mq_IDL.tab.c"
	break;
      case 68: /* "inline_enum_type" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 2024 "global_mq_IDL.tab.c"
	break;
      case 69: /* "enum_values" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 2033 "global_mq_IDL.tab.c"
	break;
      case 70: /* "expr" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 2042 "global_mq_IDL.tab.c"
	break;
      case 71: /* "qname" */

/* Line 1000 of yacc.c  */
#line 55 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 2051 "global_mq_IDL.tab.c"
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
#line 64 "global_mq_IDL.y"
    { (yyval) = 0; ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 65 "global_mq_IDL.y"
    { (yyval) = 0; releaseYys2((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 66 "global_mq_IDL.y"
    { (yyval) = addMessageToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 67 "global_mq_IDL.y"
    { (yyval) = addMessageToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 68 "global_mq_IDL.y"
    { (yyval) = addPublishableToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 69 "global_mq_IDL.y"
    { (yyval) = addStructToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 70 "global_mq_IDL.y"
    { (yyval) = addDiscriminatedUnionToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 71 "global_mq_IDL.y"
    { (yyval) = addScopeToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 75 "global_mq_IDL.y"
    { (yyval) = 0; processLineDirective((yyvsp[(2) - (3)]), 0); releaseYys2((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 76 "global_mq_IDL.y"
    { (yyval) = 0; processLineDirective((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); releaseYys2((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 81 "global_mq_IDL.y"
    { (yyval) = createScope((yyvsp[(1) - (6)]), (yyvsp[(2) - (6)]), (yyvsp[(5) - (6)])); releaseYys3((yyvsp[(3) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 84 "global_mq_IDL.y"
    { (yyval) = createMessageAlias((yyvsp[(1) - (10)]), false, (yyvsp[(2) - (10)]), (yyvsp[(4) - (10)]), (yyvsp[(6) - (10)]), (yyvsp[(9) - (10)])); releaseYys5((yyvsp[(3) - (10)]), (yyvsp[(5) - (10)]), (yyvsp[(7) - (10)]), (yyvsp[(8) - (10)]), (yyvsp[(10) - (10)])); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 85 "global_mq_IDL.y"
    { (yyval) = createMessageAlias((yyvsp[(1) - (11)]), true, (yyvsp[(3) - (11)]), (yyvsp[(5) - (11)]), (yyvsp[(7) - (11)]), (yyvsp[(10) - (11)])); releaseYys6((yyvsp[(2) - (11)]), (yyvsp[(4) - (11)]), (yyvsp[(6) - (11)]), (yyvsp[(8) - (11)]), (yyvsp[(9) - (11)]), (yyvsp[(11) - (11)])); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 89 "global_mq_IDL.y"
    { (yyval) = createMessage((yyvsp[(1) - (7)]), false, (yyvsp[(2) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(6) - (7)])); releaseYys3((yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 90 "global_mq_IDL.y"
    { (yyval) = createMessage((yyvsp[(1) - (8)]), true, (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(7) - (8)])); releaseYys4((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(8) - (8)])); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 94 "global_mq_IDL.y"
    { (yyval) = addToMessage((yyvsp[(1) - (4)]), createAttribute((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))); releaseYys((yyvsp[(4) - (4)])); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 95 "global_mq_IDL.y"
    { (yyval) = insertExtensionMarkerToMessage((yyvsp[(1) - (3)])); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 96 "global_mq_IDL.y"
    { (yyval) = addToMessage((yyvsp[(1) - (4)]), createAttribute((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))); releaseYys((yyvsp[(4) - (4)])); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 97 "global_mq_IDL.y"
    { (yyval) = insertExtensionMarkerToMessage((yyvsp[(1) - (3)])); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 101 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (3)]); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 102 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (3)]); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 106 "global_mq_IDL.y"
    { (yyval) = createPublishable((yyvsp[(1) - (5)]), false, (yyvsp[(2) - (5)]), (yyvsp[(4) - (5)])); releaseYys2((yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 107 "global_mq_IDL.y"
    { (yyval) = createPublishable((yyvsp[(1) - (6)]), true, (yyvsp[(3) - (6)]), (yyvsp[(5) - (6)])); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 108 "global_mq_IDL.y"
    { (yyval) = addToPublishable((yyvsp[(1) - (4)]), createAttribute((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))); releaseYys((yyvsp[(4) - (4)])); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 109 "global_mq_IDL.y"
    { (yyval) = insertExtensionMarkerToPublishable((yyvsp[(1) - (3)])); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 113 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (3)]); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 117 "global_mq_IDL.y"
    { (yyval) = createStruct((yyvsp[(1) - (3)]), false, (yyvsp[(2) - (3)])); releaseYys((yyvsp[(3) - (3)])); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 118 "global_mq_IDL.y"
    { (yyval) = createStruct((yyvsp[(1) - (4)]), true, (yyvsp[(3) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 119 "global_mq_IDL.y"
    { (yyval) = addToStruct((yyvsp[(1) - (4)]), createAttribute((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))); releaseYys((yyvsp[(4) - (4)])); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 120 "global_mq_IDL.y"
    { (yyval) = insertExtensionMarkerToStruct((yyvsp[(1) - (3)])); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 124 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (3)]); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 128 "global_mq_IDL.y"
    { (yyval) = createDiscriminatedUnionCase((yyvsp[(1) - (6)]), false, (yyvsp[(2) - (6)]), (yyvsp[(4) - (6)])); releaseYys3((yyvsp[(3) - (6)]), (yyvsp[(5) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 129 "global_mq_IDL.y"
    { (yyval) = createDiscriminatedUnionCase((yyvsp[(1) - (7)]), true, (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 130 "global_mq_IDL.y"
    { (yyval) = addToDiscriminatedUnionCase((yyvsp[(1) - (4)]), createAttribute((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))); releaseYys((yyvsp[(4) - (4)])); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 134 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (2)]); releaseYys((yyvsp[(2) - (2)])); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 138 "global_mq_IDL.y"
    { (yyval) = createDiscriminatedUnion((yyvsp[(1) - (3)]), false, (yyvsp[(2) - (3)])); releaseYys((yyvsp[(3) - (3)])); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 139 "global_mq_IDL.y"
    { (yyval) = createDiscriminatedUnion((yyvsp[(1) - (4)]), true, (yyvsp[(3) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 140 "global_mq_IDL.y"
    { (yyval) = addToDiscriminatedUnion((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 144 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (3)]); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 160 "global_mq_IDL.y"
    { (yyval) = createIntegerType((yyvsp[(1) - (1)])); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 161 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefault((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 162 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 163 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 164 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 165 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 166 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 167 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 168 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 169 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 170 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 171 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 172 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 173 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 174 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 175 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 176 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 177 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 178 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 179 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 180 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 181 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 182 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 183 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 184 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 185 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 189 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerType((yyvsp[(1) - (1)])); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 190 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefault((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 191 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 192 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 193 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 194 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 195 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 196 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 197 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 198 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 199 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 200 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 201 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 202 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 203 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 204 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 205 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 206 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 207 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 208 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 209 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 210 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 211 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 212 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 213 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 214 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 218 "global_mq_IDL.y"
    { (yyval) = createRealType((yyvsp[(1) - (1)])); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 219 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefault((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 220 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 221 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 222 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 223 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 224 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 225 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 226 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 227 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 228 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 229 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 230 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 231 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 232 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 233 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 234 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 235 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 236 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 237 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 238 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 239 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 240 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 241 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 242 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 243 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 248 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (1)]), false, 0, false, 0); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 249 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (4)]), true, (yyvsp[(4) - (4)]), false, 0); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 250 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (4)]), false, 0, true, (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 251 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (7)]), true, (yyvsp[(4) - (7)]), true, (yyvsp[(7) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)])); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 252 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (7)]), true, (yyvsp[(7) - (7)]), true, (yyvsp[(4) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)])); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 260 "global_mq_IDL.y"
    { (yyval) = createByteArrayType((yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 264 "global_mq_IDL.y"
    { (yyval) = createVectorOfIntegerType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 265 "global_mq_IDL.y"
    { (yyval) = createVectorOfUintegerType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 266 "global_mq_IDL.y"
    { (yyval) = createVectorOfRealType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 267 "global_mq_IDL.y"
    { (yyval) = createVectorOfCharStringType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 268 "global_mq_IDL.y"
    { (yyval) = createVectorOfBLOBType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 269 "global_mq_IDL.y"
    { (yyval) = createVectorOfByteArrayType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 270 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (5)]), (yyvsp[(4) - (5)]), false, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 271 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (6)]), (yyvsp[(5) - (6)]), true, false); releaseYys4((yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 272 "global_mq_IDL.y"
    { (yyval) = createVectorOfIntegerType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 273 "global_mq_IDL.y"
    { (yyval) = createVectorOfUintegerType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 274 "global_mq_IDL.y"
    { (yyval) = createVectorOfRealType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 275 "global_mq_IDL.y"
    { (yyval) = createVectorOfCharStringType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 276 "global_mq_IDL.y"
    { (yyval) = createVectorOfBLOBType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 277 "global_mq_IDL.y"
    { (yyval) = createVectorOfByteArrayType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 278 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (8)]), (yyvsp[(4) - (8)]), false, true); releaseYys6((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(8) - (8)])); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 279 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (9)]), (yyvsp[(5) - (9)]), true, true); releaseYys7((yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)])); ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 280 "global_mq_IDL.y"
    { (yyval) = createVectorOfStructsType((yyvsp[(1) - (9)]), (yyvsp[(5) - (9)]), true, true); releaseYys7((yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)])); ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 284 "global_mq_IDL.y"
    { (yyval) = createStructType((yyvsp[(1) - (2)]), false, (yyvsp[(2) - (2)])); ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 285 "global_mq_IDL.y"
    { (yyval) = createStructType((yyvsp[(1) - (3)]), true, (yyvsp[(3) - (3)])); releaseYys( (yyvsp[(2) - (3)]) );;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 286 "global_mq_IDL.y"
    { (yyval) = createStructType((yyvsp[(2) - (3)]), true, (yyvsp[(3) - (3)])); releaseYys( (yyvsp[(1) - (3)]) ); ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 289 "global_mq_IDL.y"
    { (yyval) = createBlobType((yyvsp[(1) - (1)])); ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 293 "global_mq_IDL.y"
    { (yyval) = createInlineEnum((yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(4) - (5)])); releaseYys2((yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 294 "global_mq_IDL.y"
    { (yyval) = createInlineEnumWithDefault((yyvsp[(1) - (8)]), (yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(8) - (8)])); releaseYys4((yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 298 "global_mq_IDL.y"
    { (yyval) = addEnumValue(0, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); releaseYys((yyvsp[(2) - (3)])); ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 299 "global_mq_IDL.y"
    { (yyval) = addEnumValue((yyvsp[(1) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); releaseYys2((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)])); ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 300 "global_mq_IDL.y"
    { (yyval) = addEnumValue(0, (yyvsp[(3) - (6)]), (yyvsp[(6) - (6)])); releaseYys4((yyvsp[(1) - (6)]), (yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)])); ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 301 "global_mq_IDL.y"
    { (yyval) = addEnumValue((yyvsp[(1) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(8) - (8)])); releaseYys3((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(4) - (8)])); releaseYys2((yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 305 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 306 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(2) - (2)]); releaseYys((yyvsp[(1) - (2)])); ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 307 "global_mq_IDL.y"
    { (yyval) = makeMinusIntLit((yyvsp[(2) - (2)])); releaseYys((yyvsp[(1) - (2)])); ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 308 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 312 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;



/* Line 1455 of yacc.c  */
#line 3441 "global_mq_IDL.tab.c"
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



