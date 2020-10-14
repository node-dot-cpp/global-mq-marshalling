
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
#line 52 "global_mq_IDL.y"

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
     KW_NONEXTENDABLE = 260,
     KW_EXTENSION = 261,
     KW_HASH_LINE = 262,
     KW_DEFAULT = 263,
     KW_EMPTY = 264,
     KW_MAX_LENGTH = 265,
     KW_ENUM = 266,
     KW_BYTE_ARRAY = 267,
     IDENTIFIER = 268,
     KW_IDENTIFIER = 269,
     STRING_LITERAL = 270,
     INTEGER_LITERAL = 271,
     KW_INTEGER = 272,
     KW_UINTEGER = 273,
     KW_REAL = 274,
     KW_CHARACTER_STRING = 275,
     KW_BLOB = 276,
     KW_VECTOR = 277,
     KW_PROTO = 278
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
#line 144 "global_mq_IDL.tab.c"

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
#define YYLAST   380

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  133
/* YYNRULES -- Number of states.  */
#define YYNSTATES  334

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   278

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      32,    33,     2,    36,    30,    37,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    27,    24,
      34,    25,    35,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    29,     2,    31,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    26,     2,    28,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    14,    19,    26,    34,
      42,    47,    51,    55,    57,    59,    61,    63,    65,    67,
      69,    71,    73,    78,    85,    92,    99,   106,   112,   118,
     124,   130,   136,   142,   148,   154,   164,   174,   184,   194,
     203,   212,   221,   230,   239,   248,   257,   266,   268,   273,
     280,   287,   294,   301,   307,   313,   319,   325,   331,   337,
     343,   349,   359,   369,   379,   389,   398,   407,   416,   425,
     434,   443,   452,   461,   463,   468,   475,   482,   489,   496,
     502,   508,   514,   520,   526,   532,   538,   544,   554,   564,
     574,   584,   593,   602,   611,   620,   629,   638,   647,   656,
     658,   663,   668,   676,   684,   689,   694,   699,   704,   709,
     714,   720,   727,   735,   743,   751,   759,   767,   776,   786,
     796,   798,   804,   813,   817,   823,   830,   839,   841,   845,
     847,   850,   853,   855
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      39,     0,    -1,    -1,    39,    40,    -1,    39,    42,    -1,
       7,    16,    24,    -1,     7,    16,    15,    24,    -1,     4,
      23,    25,    53,    13,    26,    -1,     4,     5,    23,    25,
      53,    13,    26,    -1,     4,    23,    25,    53,     5,    13,
      26,    -1,    41,    43,    13,    24,    -1,    41,     6,    27,
      -1,    41,    28,    24,    -1,    44,    -1,    45,    -1,    46,
      -1,    47,    -1,    48,    -1,    51,    -1,    50,    -1,    49,
      -1,    17,    -1,    17,     8,    25,    54,    -1,    17,    29,
      54,    30,    54,    31,    -1,    17,    32,    54,    30,    54,
      31,    -1,    17,    29,    54,    30,    54,    33,    -1,    17,
      32,    54,    30,    54,    33,    -1,    17,    29,    30,    54,
      31,    -1,    17,    32,    30,    54,    31,    -1,    17,    29,
      30,    54,    33,    -1,    17,    32,    30,    54,    33,    -1,
      17,    29,    54,    30,    31,    -1,    17,    32,    54,    30,
      31,    -1,    17,    29,    54,    30,    33,    -1,    17,    32,
      54,    30,    33,    -1,    17,    29,    54,    30,    54,    31,
       8,    25,    54,    -1,    17,    32,    54,    30,    54,    31,
       8,    25,    54,    -1,    17,    29,    54,    30,    54,    33,
       8,    25,    54,    -1,    17,    32,    54,    30,    54,    33,
       8,    25,    54,    -1,    17,    29,    30,    54,    31,     8,
      25,    54,    -1,    17,    32,    30,    54,    31,     8,    25,
      54,    -1,    17,    29,    30,    54,    33,     8,    25,    54,
      -1,    17,    32,    30,    54,    33,     8,    25,    54,    -1,
      17,    29,    54,    30,    31,     8,    25,    54,    -1,    17,
      32,    54,    30,    31,     8,    25,    54,    -1,    17,    29,
      54,    30,    33,     8,    25,    54,    -1,    17,    32,    54,
      30,    33,     8,    25,    54,    -1,    18,    -1,    18,     8,
      25,    54,    -1,    18,    29,    54,    30,    54,    31,    -1,
      18,    32,    54,    30,    54,    31,    -1,    18,    29,    54,
      30,    54,    33,    -1,    18,    32,    54,    30,    54,    33,
      -1,    18,    29,    30,    54,    31,    -1,    18,    32,    30,
      54,    31,    -1,    18,    29,    30,    54,    33,    -1,    18,
      32,    30,    54,    33,    -1,    18,    29,    54,    30,    31,
      -1,    18,    32,    54,    30,    31,    -1,    18,    29,    54,
      30,    33,    -1,    18,    32,    54,    30,    33,    -1,    18,
      29,    54,    30,    54,    31,     8,    25,    54,    -1,    18,
      32,    54,    30,    54,    31,     8,    25,    54,    -1,    18,
      29,    54,    30,    54,    33,     8,    25,    54,    -1,    18,
      32,    54,    30,    54,    33,     8,    25,    54,    -1,    18,
      29,    30,    54,    31,     8,    25,    54,    -1,    18,    32,
      30,    54,    31,     8,    25,    54,    -1,    18,    29,    30,
      54,    33,     8,    25,    54,    -1,    18,    32,    30,    54,
      33,     8,    25,    54,    -1,    18,    29,    54,    30,    31,
       8,    25,    54,    -1,    18,    32,    54,    30,    31,     8,
      25,    54,    -1,    18,    29,    54,    30,    33,     8,    25,
      54,    -1,    18,    32,    54,    30,    33,     8,    25,    54,
      -1,    19,    -1,    19,     8,    25,    54,    -1,    19,    29,
      54,    30,    54,    31,    -1,    19,    32,    54,    30,    54,
      31,    -1,    19,    29,    54,    30,    54,    33,    -1,    19,
      32,    54,    30,    54,    33,    -1,    19,    29,    30,    54,
      31,    -1,    19,    32,    30,    54,    31,    -1,    19,    29,
      30,    54,    33,    -1,    19,    32,    30,    54,    33,    -1,
      19,    29,    54,    30,    31,    -1,    19,    32,    54,    30,
      31,    -1,    19,    29,    54,    30,    33,    -1,    19,    32,
      54,    30,    33,    -1,    19,    29,    54,    30,    54,    31,
       8,    25,    54,    -1,    19,    32,    54,    30,    54,    31,
       8,    25,    54,    -1,    19,    29,    54,    30,    54,    33,
       8,    25,    54,    -1,    19,    32,    54,    30,    54,    33,
       8,    25,    54,    -1,    19,    29,    30,    54,    31,     8,
      25,    54,    -1,    19,    32,    30,    54,    31,     8,    25,
      54,    -1,    19,    29,    30,    54,    33,     8,    25,    54,
      -1,    19,    32,    30,    54,    33,     8,    25,    54,    -1,
      19,    29,    54,    30,    31,     8,    25,    54,    -1,    19,
      32,    54,    30,    31,     8,    25,    54,    -1,    19,    29,
      54,    30,    33,     8,    25,    54,    -1,    19,    32,    54,
      30,    33,     8,    25,    54,    -1,    20,    -1,    20,    10,
      25,    54,    -1,    20,     8,    25,    15,    -1,    20,    10,
      25,    54,     8,    25,    15,    -1,    20,     8,    25,    15,
      10,    25,    54,    -1,    12,    29,    54,    31,    -1,    22,
      34,    17,    35,    -1,    22,    34,    18,    35,    -1,    22,
      34,    20,    35,    -1,    22,    34,    21,    35,    -1,    22,
      34,    12,    35,    -1,    22,    34,     4,    13,    35,    -1,
      22,    34,     4,     5,    13,    35,    -1,    22,    34,    17,
      35,     8,    25,     9,    -1,    22,    34,    18,    35,     8,
      25,     9,    -1,    22,    34,    20,    35,     8,    25,     9,
      -1,    22,    34,    21,    35,     8,    25,     9,    -1,    22,
      34,    12,    35,     8,    25,     9,    -1,    22,    34,     4,
      13,    35,     8,    25,     9,    -1,    22,    34,     4,     5,
      13,    35,     8,    25,     9,    -1,    22,    34,     5,     4,
      13,    35,     8,    25,     9,    -1,    21,    -1,    11,    55,
      26,    52,    28,    -1,    11,    55,    26,    52,    28,     8,
      25,    13,    -1,    13,    25,    16,    -1,    52,    30,    13,
      25,    16,    -1,    14,    32,    15,    33,    25,    16,    -1,
      52,    30,    14,    32,    15,    33,    25,    16,    -1,    13,
      -1,    53,    30,    13,    -1,    16,    -1,    36,    16,    -1,
      37,    16,    -1,    15,    -1,    13,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    59,    59,    60,    61,    65,    66,    70,    71,    72,
      73,    74,    78,    82,    83,    84,    85,    86,    87,    88,
      89,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   181,
     182,   183,   184,   185,   193,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     215,   219,   220,   224,   225,   226,   227,   231,   232,   236,
     237,   238,   239,   243
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "KW_FILE", "KW_MESSAGE",
  "KW_NONEXTENDABLE", "KW_EXTENSION", "KW_HASH_LINE", "KW_DEFAULT",
  "KW_EMPTY", "KW_MAX_LENGTH", "KW_ENUM", "KW_BYTE_ARRAY", "IDENTIFIER",
  "KW_IDENTIFIER", "STRING_LITERAL", "INTEGER_LITERAL", "KW_INTEGER",
  "KW_UINTEGER", "KW_REAL", "KW_CHARACTER_STRING", "KW_BLOB", "KW_VECTOR",
  "KW_PROTO", "';'", "'='", "'{'", "':'", "'}'", "'['", "','", "']'",
  "'('", "')'", "'<'", "'>'", "'+'", "'-'", "$accept", "file",
  "line_directive", "message_begin", "message", "data_type",
  "integer_type", "unsigned_integer_type", "real_type",
  "character_string_type", "byte_array_type", "vector_type", "blob_type",
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
     275,   276,   277,   278,    59,    61,   123,    58,   125,    91,
      44,    93,    40,    41,    60,    62,    43,    45
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    38,    39,    39,    39,    40,    40,    41,    41,    41,
      41,    41,    42,    43,    43,    43,    43,    43,    43,    43,
      43,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    45,    45,    45,
      45,    45,    45,    45,    45,    45,    45,    45,    45,    45,
      45,    45,    45,    45,    45,    45,    45,    45,    45,    45,
      45,    45,    45,    46,    46,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    46,    47,
      47,    47,    47,    47,    48,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    49,
      50,    51,    51,    52,    52,    52,    52,    53,    53,    54,
      54,    54,    54,    55
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     3,     4,     6,     7,     7,
       4,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     6,     6,     6,     6,     5,     5,     5,
       5,     5,     5,     5,     5,     9,     9,     9,     9,     8,
       8,     8,     8,     8,     8,     8,     8,     1,     4,     6,
       6,     6,     6,     5,     5,     5,     5,     5,     5,     5,
       5,     9,     9,     9,     9,     8,     8,     8,     8,     8,
       8,     8,     8,     1,     4,     6,     6,     6,     6,     5,
       5,     5,     5,     5,     5,     5,     5,     9,     9,     9,
       9,     8,     8,     8,     8,     8,     8,     8,     8,     1,
       4,     4,     7,     7,     4,     4,     4,     4,     4,     4,
       5,     6,     7,     7,     7,     7,     7,     8,     9,     9,
       1,     5,     8,     3,     5,     6,     8,     1,     3,     1,
       2,     2,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     3,     0,     4,     0,     0,
       0,     0,     0,     0,    21,    47,    73,    99,   120,     0,
       0,     0,    13,    14,    15,    16,    17,    20,    19,    18,
       0,     0,     0,     5,    11,   133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      12,     0,     0,   127,     0,     6,     0,   132,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    10,     0,     0,     0,
       0,     0,     0,     0,   130,   131,   104,    22,     0,     0,
       0,     0,    48,     0,     0,     0,     0,    74,     0,     0,
       0,     0,   101,   100,     0,     0,     0,   109,   105,   106,
     107,   108,     0,     0,     7,   128,     0,     0,   121,     0,
      27,    29,    31,    33,     0,    28,    30,    32,    34,     0,
      53,    55,    57,    59,     0,    54,    56,    58,    60,     0,
      79,    81,    83,    85,     0,    80,    82,    84,    86,     0,
       0,     0,     0,   110,     0,     0,     0,     0,     0,     0,
       8,     9,   123,     0,     0,     0,     0,     0,     0,     0,
       0,    23,    25,     0,     0,     0,     0,    24,    26,     0,
       0,     0,     0,    49,    51,     0,     0,     0,     0,    50,
      52,     0,     0,     0,     0,    75,    77,     0,     0,     0,
       0,    76,    78,     0,     0,   111,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   103,   102,     0,     0,     0,   116,   112,
     113,   114,   115,     0,   122,   124,     0,    39,    41,    43,
      45,     0,     0,    40,    42,    44,    46,     0,     0,    65,
      67,    69,    71,     0,     0,    66,    68,    70,    72,     0,
       0,    91,    93,    95,    97,     0,     0,    92,    94,    96,
      98,     0,     0,     0,   117,     0,   125,     0,    35,    37,
      36,    38,    61,    63,    62,    64,    87,    89,    88,    90,
     118,   119,     0,   126
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     5,     6,     7,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    93,    54,    61,    36
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -40
static const yytype_int16 yypact[] =
{
     -40,    45,   -40,     7,    -2,   -40,    36,   -40,   -15,    -5,
      -6,    23,    12,    34,     3,     8,     9,     5,   -40,    32,
      50,    68,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,
      63,    92,    75,   -40,   -40,   -40,    66,   126,    89,    91,
     116,   100,   118,   120,   124,   128,   145,   134,   142,   166,
     -40,   127,    92,   -40,    -3,   -40,   159,   -40,   -40,   153,
     164,   148,   126,   126,   155,   126,   162,   126,   126,   168,
     126,   169,   126,   126,   174,   126,   175,   178,   126,    38,
     206,   176,   181,   182,   187,   188,   -40,    -8,   211,   199,
     213,   202,   196,    -9,   -40,   -40,   -40,   -40,    13,    53,
      28,    60,   -40,    40,    67,    46,    79,   -40,    47,    86,
      54,    93,   219,   222,   218,   197,   220,   226,   227,   228,
     229,   230,   214,   215,   -40,   -40,   223,   231,   236,   163,
     237,   239,   242,   243,    80,   244,   245,   248,   249,    87,
     250,   251,   254,   255,   106,   256,   257,   260,   261,   107,
     262,   263,   266,   267,   114,   268,   269,   270,   271,   135,
     258,   259,   246,   272,   247,   264,   265,   273,   274,   275,
     -40,   -40,   -40,   252,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   286,   287,   288,   289,   285,   295,   290,
     291,   292,   293,   300,   301,   294,   296,   297,   298,   312,
     316,   302,   303,   304,   305,   317,   318,   306,   307,   308,
     309,   327,   328,   126,   322,   330,   314,   332,   333,   334,
     335,   336,   337,   323,   338,   325,   339,   126,   126,   126,
     126,   324,   331,   126,   126,   126,   126,   340,   341,   126,
     126,   126,   126,   342,   343,   126,   126,   126,   126,   344,
     345,   126,   126,   126,   126,   346,   347,   126,   126,   126,
     126,   348,   349,   -40,   -40,   350,   351,   352,   -40,   -40,
     -40,   -40,   -40,   360,   -40,   -40,   253,   -40,   -40,   -40,
     -40,   126,   126,   -40,   -40,   -40,   -40,   126,   126,   -40,
     -40,   -40,   -40,   126,   126,   -40,   -40,   -40,   -40,   126,
     126,   -40,   -40,   -40,   -40,   126,   126,   -40,   -40,   -40,
     -40,   126,   126,   353,   -40,   354,   -40,   355,   -40,   -40,
     -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,
     -40,   -40,   362,   -40
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,   -40,
     -40,   -40,   -40,   -40,   -40,   235,   -39,   -40
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      64,    66,    88,    69,    71,   122,    74,    76,    30,    32,
      89,    38,     8,    47,    10,    48,    41,    44,    33,   128,
      31,   129,    90,    97,    98,    35,   100,    90,   102,   103,
       9,   105,    39,   107,   108,    40,   110,    42,    45,   113,
      43,    46,    11,   114,   130,     2,   131,    12,    13,     3,
      34,   115,     4,    14,    15,    16,    17,    18,    19,   135,
     134,   136,   139,    37,    20,   144,    49,   149,    57,    58,
     154,   140,   159,   141,    50,    57,    58,   145,   150,   146,
     151,    51,    57,    58,   132,   155,   133,   156,    52,    59,
      60,   137,    56,   138,    57,    58,    59,    60,   142,    55,
     143,    57,    58,    59,    60,    53,    57,    58,    57,    58,
     147,   181,   148,   182,    62,    59,    60,   152,   187,   153,
     188,    63,    59,    60,   157,    67,   158,    59,    60,    59,
      60,    57,    58,    57,    58,    57,    58,   193,   199,   194,
     200,    57,    58,    57,    58,   205,    65,   206,    68,    72,
      70,    86,    59,    60,    59,    60,    59,    60,    73,    77,
      57,    58,    59,    60,    59,    60,   211,    78,   212,    94,
      79,    80,    91,    92,   263,    75,   175,   176,    81,    96,
      95,    59,    60,    82,    83,    99,    84,    85,   277,   278,
     279,   280,   101,   112,   283,   284,   285,   286,   104,   106,
     289,   290,   291,   292,   109,   111,   295,   296,   297,   298,
     116,   117,   301,   302,   303,   304,   118,   119,   307,   308,
     309,   310,   120,   121,   123,   124,   125,   126,   127,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   172,
     170,   171,   318,   319,   174,   177,   173,   178,   320,   321,
     179,   180,   183,   184,   322,   323,   185,   186,   189,   190,
     324,   325,   191,   192,   195,   196,   326,   327,   197,   198,
     201,   202,   328,   329,   203,   204,   207,   208,   209,   210,
     216,   215,   217,   213,   214,   223,   317,    87,     0,   218,
     219,   231,   232,   237,     0,     0,     0,     0,   220,   221,
     222,   224,   225,   238,   227,   228,   229,   230,   243,   244,
     226,   233,   234,   235,   236,   239,   240,   241,   242,   245,
     249,   246,   247,   248,   250,   255,   256,   251,   252,   253,
     254,   257,   258,   259,   260,   261,   262,   264,   265,   266,
     267,   275,   268,   269,   270,   271,   272,     0,   273,   281,
       0,   274,     0,     0,   276,     0,   282,     0,     0,     0,
     314,     0,   330,   331,     0,   287,   288,   293,   294,   299,
     300,   305,   306,   311,   312,   313,   316,   315,   333,     0,
     332
};

static const yytype_int16 yycheck[] =
{
      39,    40,     5,    42,    43,    13,    45,    46,    23,    15,
      13,     8,     5,     8,    16,    10,     8,     8,    24,    28,
      25,    30,    30,    62,    63,    13,    65,    30,    67,    68,
      23,    70,    29,    72,    73,    32,    75,    29,    29,    78,
      32,    32,     6,     5,    31,     0,    33,    11,    12,     4,
      27,    13,     7,    17,    18,    19,    20,    21,    22,    31,
      99,    33,   101,    29,    28,   104,    34,   106,    15,    16,
     109,    31,   111,    33,    24,    15,    16,    31,    31,    33,
      33,    13,    15,    16,    31,    31,    33,    33,    25,    36,
      37,    31,    26,    33,    15,    16,    36,    37,    31,    24,
      33,    15,    16,    36,    37,    13,    15,    16,    15,    16,
      31,    31,    33,    33,    25,    36,    37,    31,    31,    33,
      33,    30,    36,    37,    31,    25,    33,    36,    37,    36,
      37,    15,    16,    15,    16,    15,    16,    31,    31,    33,
      33,    15,    16,    15,    16,    31,    30,    33,    30,    25,
      30,    24,    36,    37,    36,    37,    36,    37,    30,    25,
      15,    16,    36,    37,    36,    37,    31,    25,    33,    16,
       4,     5,    13,    14,   213,    30,    13,    14,    12,    31,
      16,    36,    37,    17,    18,    30,    20,    21,   227,   228,
     229,   230,    30,    15,   233,   234,   235,   236,    30,    30,
     239,   240,   241,   242,    30,    30,   245,   246,   247,   248,
       4,    35,   251,   252,   253,   254,    35,    35,   257,   258,
     259,   260,    35,    35,    13,    26,    13,    25,    32,    10,
       8,    13,    35,    13,     8,     8,     8,     8,     8,    16,
      26,    26,   281,   282,     8,     8,    15,     8,   287,   288,
       8,     8,     8,     8,   293,   294,     8,     8,     8,     8,
     299,   300,     8,     8,     8,     8,   305,   306,     8,     8,
       8,     8,   311,   312,     8,     8,     8,     8,     8,     8,
       8,    35,    35,    25,    25,    33,    33,    52,    -1,    25,
      25,     8,     8,     8,    -1,    -1,    -1,    -1,    25,    25,
      25,    25,    25,     8,    25,    25,    25,    25,     8,     8,
      32,    25,    25,    25,    25,    25,    25,    25,    25,    25,
       8,    25,    25,    25,     8,     8,     8,    25,    25,    25,
      25,    25,    25,    25,    25,     8,     8,    15,     8,    25,
       8,    16,     9,     9,     9,     9,     9,    -1,    25,    25,
      -1,    13,    -1,    -1,    15,    -1,    25,    -1,    -1,    -1,
       9,    -1,     9,     9,    -1,    25,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    16,    25,    16,    -1,
      25
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    39,     0,     4,     7,    40,    41,    42,     5,    23,
      16,     6,    11,    12,    17,    18,    19,    20,    21,    22,
      28,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      23,    25,    15,    24,    27,    13,    55,    29,     8,    29,
      32,     8,    29,    32,     8,    29,    32,     8,    10,    34,
      24,    13,    25,    13,    53,    24,    26,    15,    16,    36,
      37,    54,    25,    30,    54,    30,    54,    25,    30,    54,
      30,    54,    25,    30,    54,    30,    54,    25,    25,     4,
       5,    12,    17,    18,    20,    21,    24,    53,     5,    13,
      30,    13,    14,    52,    16,    16,    31,    54,    54,    30,
      54,    30,    54,    54,    30,    54,    30,    54,    54,    30,
      54,    30,    15,    54,     5,    13,     4,    35,    35,    35,
      35,    35,    13,    13,    26,    13,    25,    32,    28,    30,
      31,    33,    31,    33,    54,    31,    33,    31,    33,    54,
      31,    33,    31,    33,    54,    31,    33,    31,    33,    54,
      31,    33,    31,    33,    54,    31,    33,    31,    33,    54,
      10,     8,    13,    35,    13,     8,     8,     8,     8,     8,
      26,    26,    16,    15,     8,    13,    14,     8,     8,     8,
       8,    31,    33,     8,     8,     8,     8,    31,    33,     8,
       8,     8,     8,    31,    33,     8,     8,     8,     8,    31,
      33,     8,     8,     8,     8,    31,    33,     8,     8,     8,
       8,    31,    33,    25,    25,    35,     8,    35,    25,    25,
      25,    25,    25,    33,    25,    25,    32,    25,    25,    25,
      25,     8,     8,    25,    25,    25,    25,     8,     8,    25,
      25,    25,    25,     8,     8,    25,    25,    25,    25,     8,
       8,    25,    25,    25,    25,     8,     8,    25,    25,    25,
      25,     8,     8,    54,    15,     8,    25,     8,     9,     9,
       9,     9,     9,    25,    13,    16,    15,    54,    54,    54,
      54,    25,    25,    54,    54,    54,    54,    25,    25,    54,
      54,    54,    54,    25,    25,    54,    54,    54,    54,    25,
      25,    54,    54,    54,    54,    25,    25,    54,    54,    54,
      54,    25,    25,    25,     9,    25,    16,    33,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
       9,     9,    25,    16
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
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1353 "global_mq_IDL.tab.c"
	break;
      case 4: /* "KW_MESSAGE" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1362 "global_mq_IDL.tab.c"
	break;
      case 5: /* "KW_NONEXTENDABLE" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1371 "global_mq_IDL.tab.c"
	break;
      case 6: /* "KW_EXTENSION" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1380 "global_mq_IDL.tab.c"
	break;
      case 7: /* "KW_HASH_LINE" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1389 "global_mq_IDL.tab.c"
	break;
      case 8: /* "KW_DEFAULT" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1398 "global_mq_IDL.tab.c"
	break;
      case 9: /* "KW_EMPTY" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1407 "global_mq_IDL.tab.c"
	break;
      case 10: /* "KW_MAX_LENGTH" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1416 "global_mq_IDL.tab.c"
	break;
      case 11: /* "KW_ENUM" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1425 "global_mq_IDL.tab.c"
	break;
      case 12: /* "KW_BYTE_ARRAY" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1434 "global_mq_IDL.tab.c"
	break;
      case 13: /* "IDENTIFIER" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1443 "global_mq_IDL.tab.c"
	break;
      case 14: /* "KW_IDENTIFIER" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1452 "global_mq_IDL.tab.c"
	break;
      case 15: /* "STRING_LITERAL" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1461 "global_mq_IDL.tab.c"
	break;
      case 16: /* "INTEGER_LITERAL" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1470 "global_mq_IDL.tab.c"
	break;
      case 17: /* "KW_INTEGER" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1479 "global_mq_IDL.tab.c"
	break;
      case 18: /* "KW_UINTEGER" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1488 "global_mq_IDL.tab.c"
	break;
      case 19: /* "KW_REAL" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1497 "global_mq_IDL.tab.c"
	break;
      case 20: /* "KW_CHARACTER_STRING" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1506 "global_mq_IDL.tab.c"
	break;
      case 21: /* "KW_BLOB" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1515 "global_mq_IDL.tab.c"
	break;
      case 22: /* "KW_VECTOR" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1524 "global_mq_IDL.tab.c"
	break;
      case 23: /* "KW_PROTO" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1533 "global_mq_IDL.tab.c"
	break;
      case 24: /* "';'" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1542 "global_mq_IDL.tab.c"
	break;
      case 25: /* "'='" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1551 "global_mq_IDL.tab.c"
	break;
      case 26: /* "'{'" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1560 "global_mq_IDL.tab.c"
	break;
      case 27: /* "':'" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1569 "global_mq_IDL.tab.c"
	break;
      case 28: /* "'}'" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1578 "global_mq_IDL.tab.c"
	break;
      case 29: /* "'['" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1587 "global_mq_IDL.tab.c"
	break;
      case 30: /* "','" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1596 "global_mq_IDL.tab.c"
	break;
      case 31: /* "']'" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1605 "global_mq_IDL.tab.c"
	break;
      case 32: /* "'('" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1614 "global_mq_IDL.tab.c"
	break;
      case 33: /* "')'" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1623 "global_mq_IDL.tab.c"
	break;
      case 34: /* "'<'" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1632 "global_mq_IDL.tab.c"
	break;
      case 35: /* "'>'" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1641 "global_mq_IDL.tab.c"
	break;
      case 36: /* "'+'" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1650 "global_mq_IDL.tab.c"
	break;
      case 37: /* "'-'" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1659 "global_mq_IDL.tab.c"
	break;
      case 39: /* "file" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1668 "global_mq_IDL.tab.c"
	break;
      case 40: /* "line_directive" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1677 "global_mq_IDL.tab.c"
	break;
      case 41: /* "message_begin" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1686 "global_mq_IDL.tab.c"
	break;
      case 42: /* "message" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1695 "global_mq_IDL.tab.c"
	break;
      case 43: /* "data_type" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1704 "global_mq_IDL.tab.c"
	break;
      case 44: /* "integer_type" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1713 "global_mq_IDL.tab.c"
	break;
      case 45: /* "unsigned_integer_type" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1722 "global_mq_IDL.tab.c"
	break;
      case 46: /* "real_type" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1731 "global_mq_IDL.tab.c"
	break;
      case 47: /* "character_string_type" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1740 "global_mq_IDL.tab.c"
	break;
      case 48: /* "byte_array_type" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1749 "global_mq_IDL.tab.c"
	break;
      case 49: /* "vector_type" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1758 "global_mq_IDL.tab.c"
	break;
      case 50: /* "blob_type" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1767 "global_mq_IDL.tab.c"
	break;
      case 51: /* "inline_enum_type" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1776 "global_mq_IDL.tab.c"
	break;
      case 52: /* "enum_values" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1785 "global_mq_IDL.tab.c"
	break;
      case 53: /* "proto_values" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1794 "global_mq_IDL.tab.c"
	break;
      case 54: /* "expr" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1803 "global_mq_IDL.tab.c"
	break;
      case 55: /* "qname" */

/* Line 1000 of yacc.c  */
#line 50 "global_mq_IDL.y"
	{ releaseYys((*yyvaluep)); };

/* Line 1000 of yacc.c  */
#line 1812 "global_mq_IDL.tab.c"
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
#line 59 "global_mq_IDL.y"
    { (yyval) = 0; ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 60 "global_mq_IDL.y"
    { (yyval) = 0; releaseYys2((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 61 "global_mq_IDL.y"
    { (yyval) = addToFile((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 65 "global_mq_IDL.y"
    { (yyval) = 0; processLineDirective((yyvsp[(2) - (3)]), 0); releaseYys2((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 66 "global_mq_IDL.y"
    { (yyval) = 0; processLineDirective((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); releaseYys2((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 70 "global_mq_IDL.y"
    { (yyval) = createMessage((yyvsp[(1) - (6)]), false, (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)])); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 71 "global_mq_IDL.y"
    { (yyval) = createMessage((yyvsp[(1) - (7)]), true, (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 72 "global_mq_IDL.y"
    { (yyval) = createMessage((yyvsp[(1) - (7)]), true, (yyvsp[(4) - (7)]), (yyvsp[(6) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 73 "global_mq_IDL.y"
    { (yyval) = addToMessage((yyvsp[(1) - (4)]), createAttribute((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]))); releaseYys((yyvsp[(4) - (4)])); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 74 "global_mq_IDL.y"
    { (yyval) = insertExtensionMarker((yyvsp[(1) - (3)])); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 78 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (3)]); releaseYys2((yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 93 "global_mq_IDL.y"
    { (yyval) = createIntegerType((yyvsp[(1) - (1)])); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 94 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefault((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 95 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 96 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 97 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 98 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 99 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 100 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 101 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 102 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 103 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 104 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 105 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 106 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 107 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 108 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 109 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 110 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 111 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 112 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 113 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 114 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 115 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 116 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 117 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 118 "global_mq_IDL.y"
    { (yyval) = createIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 122 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerType((yyvsp[(1) - (1)])); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 123 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefault((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 124 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 125 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 126 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 127 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 128 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 129 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 130 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 131 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 132 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 133 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 134 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 135 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 136 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 137 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 138 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 139 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 140 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 141 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 142 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 143 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 144 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 145 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 146 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 147 "global_mq_IDL.y"
    { (yyval) = createUnsignedIntegerTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 151 "global_mq_IDL.y"
    { (yyval) = createRealType((yyvsp[(1) - (1)])); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 152 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefault((yyvsp[(1) - (4)]), (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 153 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 154 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), true); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 155 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, true, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 156 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (6)]), true, false, (yyvsp[(3) - (6)]), true, (yyvsp[(5) - (6)]), false); releaseYys3((yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 157 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 158 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 159 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, true, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 160 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), false, false, 0, true, (yyvsp[(4) - (5)]), false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 161 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 162 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, true); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 163 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, true, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 164 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithLimits((yyvsp[(1) - (5)]), true, false, (yyvsp[(3) - (5)]), false, 0, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 165 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 166 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), true, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 167 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, true, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 168 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (9)]), true, false, (yyvsp[(3) - (9)]), true, (yyvsp[(5) - (9)]), false, (yyvsp[(9) - (9)])); releaseYys5((yyvsp[(2) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)])); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 169 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 170 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 171 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, true, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 172 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), false, false, 0, true, (yyvsp[(4) - (8)]), false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 173 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 174 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, true, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 175 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, true, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 176 "global_mq_IDL.y"
    { (yyval) = createRealTypeWithDefaultAndLimits((yyvsp[(1) - (8)]), true, false, (yyvsp[(3) - (8)]), false, 0, false, (yyvsp[(8) - (8)])); releaseYys5((yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 181 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (1)]), false, 0, false, 0); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 182 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (4)]), true, (yyvsp[(4) - (4)]), false, 0); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 183 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (4)]), false, 0, true, (yyvsp[(4) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 184 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (7)]), true, (yyvsp[(4) - (7)]), true, (yyvsp[(7) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)])); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 185 "global_mq_IDL.y"
    { (yyval) = createCharacterStringType((yyvsp[(1) - (7)]), true, (yyvsp[(7) - (7)]), true, (yyvsp[(4) - (7)])); releaseYys4((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)])); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 193 "global_mq_IDL.y"
    { (yyval) = createByteArrayType((yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); releaseYys2((yyvsp[(2) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 197 "global_mq_IDL.y"
    { (yyval) = createVectorOfIntegerType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 198 "global_mq_IDL.y"
    { (yyval) = createVectorOfUintegerType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 199 "global_mq_IDL.y"
    { (yyval) = createVectorOfCharStringType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 200 "global_mq_IDL.y"
    { (yyval) = createVectorOfBLOBType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 201 "global_mq_IDL.y"
    { (yyval) = createVectorOfByteArrayType((yyvsp[(1) - (4)]), false); releaseYys3((yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 202 "global_mq_IDL.y"
    { (yyval) = createVectorOfMassagesType((yyvsp[(1) - (5)]), (yyvsp[(4) - (5)]), false, false); releaseYys3((yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 203 "global_mq_IDL.y"
    { (yyval) = createVectorOfMassagesType((yyvsp[(1) - (6)]), (yyvsp[(5) - (6)]), true, false); releaseYys4((yyvsp[(2) - (6)]), (yyvsp[(3) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 204 "global_mq_IDL.y"
    { (yyval) = createVectorOfIntegerType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 205 "global_mq_IDL.y"
    { (yyval) = createVectorOfUintegerType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 206 "global_mq_IDL.y"
    { (yyval) = createVectorOfCharStringType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 207 "global_mq_IDL.y"
    { (yyval) = createVectorOfBLOBType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 208 "global_mq_IDL.y"
    { (yyval) = createVectorOfByteArrayType((yyvsp[(1) - (7)]), true); releaseYys6((yyvsp[(2) - (7)]), (yyvsp[(3) - (7)]), (yyvsp[(4) - (7)]), (yyvsp[(5) - (7)]), (yyvsp[(6) - (7)]), (yyvsp[(7) - (7)])); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 209 "global_mq_IDL.y"
    { (yyval) = createVectorOfMassagesType((yyvsp[(1) - (8)]), (yyvsp[(4) - (8)]), false, true); releaseYys6((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(8) - (8)])); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 210 "global_mq_IDL.y"
    { (yyval) = createVectorOfMassagesType((yyvsp[(1) - (9)]), (yyvsp[(5) - (9)]), true, true); releaseYys7((yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)])); ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 211 "global_mq_IDL.y"
    { (yyval) = createVectorOfMassagesType((yyvsp[(1) - (9)]), (yyvsp[(5) - (9)]), true, true); releaseYys7((yyvsp[(2) - (9)]), (yyvsp[(3) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(8) - (9)]), (yyvsp[(9) - (9)])); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 215 "global_mq_IDL.y"
    { (yyval) = createBlobType((yyvsp[(1) - (1)])); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 219 "global_mq_IDL.y"
    { (yyval) = createInlineEnum((yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(4) - (5)])); releaseYys2((yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 220 "global_mq_IDL.y"
    { (yyval) = createInlineEnumWithDefault((yyvsp[(1) - (8)]), (yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(8) - (8)])); releaseYys4((yyvsp[(3) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 224 "global_mq_IDL.y"
    { (yyval) = addEnumValue(0, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); releaseYys((yyvsp[(2) - (3)])); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 225 "global_mq_IDL.y"
    { (yyval) = addEnumValue((yyvsp[(1) - (5)]), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])); releaseYys2((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)])); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 226 "global_mq_IDL.y"
    { (yyval) = addEnumValue(0, (yyvsp[(3) - (6)]), (yyvsp[(6) - (6)])); releaseYys4((yyvsp[(1) - (6)]), (yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(5) - (6)])); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 227 "global_mq_IDL.y"
    { (yyval) = addEnumValue((yyvsp[(1) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(8) - (8)])); releaseYys3((yyvsp[(2) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(4) - (8)])); releaseYys2((yyvsp[(6) - (8)]), (yyvsp[(7) - (8)])); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 231 "global_mq_IDL.y"
    { (yyval) = addProtoValue(0, (yyvsp[(1) - (1)])); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 232 "global_mq_IDL.y"
    { (yyval) = addProtoValue((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); releaseYys((yyvsp[(2) - (3)])); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 236 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 237 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(2) - (2)]); releaseYys((yyvsp[(1) - (2)])); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 238 "global_mq_IDL.y"
    { (yyval) = makeMinusIntLit((yyvsp[(2) - (2)])); releaseYys((yyvsp[(1) - (2)])); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 239 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 243 "global_mq_IDL.y"
    { (yyval) = (yyvsp[(1) - (1)]); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2985 "global_mq_IDL.tab.c"
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



