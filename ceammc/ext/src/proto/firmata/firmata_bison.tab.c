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
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 0

/* Substitute the type names.  */
#define YYSTYPE         FIRMATA_STYPE
/* Substitute the variable and function names.  */
#define yypush_parse    firmata_push_parse
#define yypstate_new    firmata_pstate_new
#define yypstate_delete firmata_pstate_delete
#define yypstate        firmata_pstate
#define yylex           firmata_lex
#define yyerror         firmata_error
#define yydebug         firmata_debug
#define yynerrs         firmata_nerrs


/* Copy the first part of user declarations.  */
#line 1 "firmata_bison.y" /* yacc.c:339  */

#include <stdio.h>

#include "firmata_proto.h"

void yyerror(FirmataMessage* msg, const char* s) {
    msg->state = STATE_ERROR;
}

#define YYMAXDEPTH 500

unsigned char decode_wchar(int ch, char* out)
{
    if (ch <= 0x7f) {
        if(out) out[0] = ch;
        return 1;
    }
    else if (ch <= 0x7ff)
    {
        if(out) {
            out[0] = (0xc0 | ((ch >> 6) & 0x1f));
            out[1] = (0x80 | (ch & 0x3f));
        }
        return 2;
    }

    return 0;
}


#line 107 "firmata_bison.tab.c" /* yacc.c:339  */

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
   by #include "firmata_bison.tab.h".  */
#ifndef YY_FIRMATA_FIRMATA_BISON_TAB_H_INCLUDED
# define YY_FIRMATA_FIRMATA_BISON_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef FIRMATA_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define FIRMATA_DEBUG 1
#  else
#   define FIRMATA_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define FIRMATA_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined FIRMATA_DEBUG */
#if FIRMATA_DEBUG
extern int firmata_debug;
#endif

/* Token type.  */
#ifndef FIRMATA_TOKENTYPE
# define FIRMATA_TOKENTYPE
  enum firmata_tokentype
  {
    FIRMATA_TOKEN_ANALOG_IO = 258,
    FIRMATA_TOKEN_DIGITAL_IO = 259,
    FIRMATA_TOKEN_PROTOCOL_VERSION = 260,
    FIRMATA_TOKEN_SET_PIN_MODE = 261,
    FIRMATA_TOKEN_SET_DIGITAL_PIN_VALUE = 262,
    FIRMATA_TOKEN_START_SYSEX = 263,
    FIRMATA_TOKEN_STOP_SYSEX = 264,
    FIRMATA_TOKEN_ERROR_RESET = 265,
    FIRMATA_TOKEN_BYTE7 = 266,
    FIRMATA_TOKEN_SYSEX_ID_EXTENDED = 267,
    FIRMATA_TOKEN_SYSEX_ID_RESERVED = 268,
    FIRMATA_TOKEN_SYSEX_ID_ANALOG_MAPPING_QUERY = 269,
    FIRMATA_TOKEN_SYSEX_ID_ANALOG_MAPPING_RESPONSE = 270,
    FIRMATA_TOKEN_SYSEX_ID_CAPABILITY_QUERY = 271,
    FIRMATA_TOKEN_SYSEX_ID_CAPABILITY_RESPONSE = 272,
    FIRMATA_TOKEN_SYSEX_ID_PIN_STATE_QUERY = 273,
    FIRMATA_TOKEN_SYSEX_ID_PIN_STATE_RESPONSE = 274,
    FIRMATA_TOKEN_SYSEX_ID_EXTENDED_ANALOG = 275,
    FIRMATA_TOKEN_SYSEX_ID_STRING_DATA = 276,
    FIRMATA_TOKEN_SYSEX_ID_REPORT_FIRMWARE = 277,
    FIRMATA_TOKEN_SYSEX_ID_SAMPLING_INTERVAL = 278,
    FIRMATA_TOKEN_SYSEX_ID_SYSEX_NON_REALTIME = 279,
    FIRMATA_TOKEN_SYSEX_ID_SYSEX_REALTIME = 280,
    FIRMATA_TOKEN_SYSEX_ID_PIN_CAPABILITY_END = 281
  };
#endif

/* Value type.  */
#if ! defined FIRMATA_STYPE && ! defined FIRMATA_STYPE_IS_DECLARED

union FIRMATA_STYPE
{
#line 32 "firmata_bison.y" /* yacc.c:355  */

    FirmataMessage* m;
    int integer;

#line 187 "firmata_bison.tab.c" /* yacc.c:355  */
};

typedef union FIRMATA_STYPE FIRMATA_STYPE;
# define FIRMATA_STYPE_IS_TRIVIAL 1
# define FIRMATA_STYPE_IS_DECLARED 1
#endif



#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct firmata_pstate firmata_pstate;

int firmata_push_parse (firmata_pstate *ps, int pushed_char, FIRMATA_STYPE const *pushed_val, FirmataMessage *result);

firmata_pstate * firmata_pstate_new (void);
void firmata_pstate_delete (firmata_pstate *ps);

#endif /* !YY_FIRMATA_FIRMATA_BISON_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 213 "firmata_bison.tab.c" /* yacc.c:358  */

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
         || (defined FIRMATA_STYPE_IS_TRIVIAL && FIRMATA_STYPE_IS_TRIVIAL)))

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
#define YYFINAL  33
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   97

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  27
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  48
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  79

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   281

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
      25,    26
};

#if FIRMATA_DEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    88,    88,    90,    94,    99,   104,   109,   133,   138,
     139,   142,   143,   144,   146,   151,   152,   153,   154,   157,
     158,   161,   162,   165,   166,   169,   170,   171,   172,   175,
     176,   178,   184,   189,   196,   202,   203,   205,   208,   212,
     213,   215,   220,   221,   223,   226,   231,   237,   241
};
#endif

#if FIRMATA_DEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ANALOG_IO", "DIGITAL_IO",
  "PROTOCOL_VERSION", "SET_PIN_MODE", "SET_DIGITAL_PIN_VALUE",
  "START_SYSEX", "STOP_SYSEX", "ERROR_RESET", "BYTE7", "SYSEX_ID_EXTENDED",
  "SYSEX_ID_RESERVED", "SYSEX_ID_ANALOG_MAPPING_QUERY",
  "SYSEX_ID_ANALOG_MAPPING_RESPONSE", "SYSEX_ID_CAPABILITY_QUERY",
  "SYSEX_ID_CAPABILITY_RESPONSE", "SYSEX_ID_PIN_STATE_QUERY",
  "SYSEX_ID_PIN_STATE_RESPONSE", "SYSEX_ID_EXTENDED_ANALOG",
  "SYSEX_ID_STRING_DATA", "SYSEX_ID_REPORT_FIRMWARE",
  "SYSEX_ID_SAMPLING_INTERVAL", "SYSEX_ID_SYSEX_NON_REALTIME",
  "SYSEX_ID_SYSEX_REALTIME", "SYSEX_ID_PIN_CAPABILITY_END", "$accept",
  "program", "message", "pin", "value14", "byte7", "sysex", "sysex_body",
  "sysex_paylod", "sysex_query_firmware", "sysex_string",
  "sysex_sampling_interval", "capability_response", "pin_capability_list",
  "pin_capability_chain", "pin_capability", "pin_mode", "pin_resolution",
  "analog_mapping_response", "pin_state_respone", "pin_state_bytes7", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281
};
# endif

#define YYPACT_NINF -50

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-50)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      87,    -6,    -6,    -6,    -6,    -6,    64,   -50,    62,   -50,
     -50,   -50,    -6,   -50,    -6,    -6,    -6,    -6,    -6,    -6,
     -50,    -6,   -50,    -5,    -6,    -6,   -50,    -6,    -6,    -6,
     -50,   -50,    -2,   -50,   -50,   -50,    -6,   -50,   -50,   -50,
     -50,    -6,   -50,    -6,   -50,    -6,   -50,   -50,    -5,   -50,
      17,   -50,    -6,   -50,    -6,   -50,   -50,    -6,    -6,   -50,
     -50,   -50,   -50,   -50,    -6,   -50,   -50,   -50,   -50,   -50,
     -50,   -50,    -6,   -50,    -6,   -50,    -6,    -6,   -50
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,    10,     0,     2,
       9,    13,     0,    11,     0,     0,     0,     0,     0,     0,
      17,     0,    19,     0,     0,     0,    23,     0,     0,     0,
      27,    28,     0,     1,     3,     4,     0,     5,     6,     7,
       8,     0,    29,    16,    44,    18,    37,    42,    20,    35,
       0,    39,     0,    21,     0,    22,    32,    24,     0,    25,
      34,    26,    14,    12,    15,    30,    45,    36,    38,    40,
      43,    41,     0,    33,     0,    47,    46,    31,    48
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -50,   -50,     0,     8,    -3,    -1,   -50,   -50,   -22,   -50,
     -49,   -50,   -50,   -19,   -50,   -20,   -23,   -50,   -50,   -50,
     -50
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     8,     9,    12,    56,    36,    10,    32,    43,    59,
      57,    61,    48,    49,    50,    51,    52,    71,    45,    55,
      76
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      13,    13,    15,    13,    13,    11,    11,    62,    34,    35,
      14,    37,    16,    17,    38,    39,    40,    41,    42,    64,
      44,    46,    47,    13,    13,    77,    60,    58,    11,    67,
      69,    72,    53,    54,     0,    63,     0,     0,     0,     0,
      42,     0,    65,    68,    66,     0,     0,    47,     0,    47,
       0,    70,     0,    47,    73,     0,     0,    74,     0,     0,
       0,     0,    33,    65,     0,     1,     2,     3,     4,     5,
       6,    75,     7,     0,    73,    78,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
       1,     2,     3,     4,     5,     6,     0,     7
};

static const yytype_int8 yycheck[] =
{
       1,     2,     3,     4,     5,    11,    11,     9,     8,    12,
       2,    14,     4,     5,    15,    16,    17,    18,    19,    41,
      21,    26,    23,    24,    25,    74,    29,    28,    11,    48,
      50,    54,    24,    25,    -1,    36,    -1,    -1,    -1,    -1,
      41,    -1,    43,    26,    45,    -1,    -1,    48,    -1,    50,
      -1,    52,    -1,    54,    57,    -1,    -1,    58,    -1,    -1,
      -1,    -1,     0,    64,    -1,     3,     4,     5,     6,     7,
       8,    72,    10,    -1,    77,    76,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
       3,     4,     5,     6,     7,     8,    -1,    10
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    10,    28,    29,
      33,    11,    30,    32,    30,    32,    30,    30,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    34,     0,    29,    31,    32,    31,    32,    32,
      32,    32,    32,    35,    32,    45,    26,    32,    39,    40,
      41,    42,    43,    30,    30,    46,    31,    37,    32,    36,
      31,    38,     9,    32,    35,    32,    32,    40,    26,    42,
      32,    44,    43,    31,    32,    32,    47,    37,    32
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    27,    28,    28,    29,    29,    29,    29,    29,    29,
      29,    30,    31,    32,    33,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    35,
      35,    36,    37,    37,    38,    39,    39,    40,    40,    41,
      41,    42,    43,    44,    45,    45,    46,    47,    47
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     3,     3,     3,     3,     3,     1,
       1,     1,     2,     1,     3,     3,     2,     1,     2,     1,
       2,     2,     2,     1,     2,     2,     2,     1,     1,     1,
       2,     3,     1,     2,     1,     1,     2,     1,     2,     1,
       2,     2,     1,     1,     1,     2,     3,     1,     2
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
      yyerror (result, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if FIRMATA_DEBUG

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
                  Type, Value, result); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, FirmataMessage *result)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (result);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, FirmataMessage *result)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, result);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, FirmataMessage *result)
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
                                              , result);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, result); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !FIRMATA_DEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !FIRMATA_DEBUG */


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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, FirmataMessage *result)
{
  YYUSE (yyvaluep);
  YYUSE (result);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}



struct yypstate
  {
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
    /* Used to determine if this is the first time this instance has
       been used.  */
    int yynew;
  };

/* Initialize the parser data structure.  */
yypstate *
yypstate_new (void)
{
  yypstate *yyps;
  yyps = (yypstate *) malloc (sizeof *yyps);
  if (!yyps)
    return YY_NULLPTR;
  yyps->yynew = 1;
  return yyps;
}

void
yypstate_delete (yypstate *yyps)
{
#ifndef yyoverflow
  /* If the stack was reallocated but the parse did not complete, then the
     stack still needs to be freed.  */
  if (!yyps->yynew && yyps->yyss != yyps->yyssa)
    YYSTACK_FREE (yyps->yyss);
#endif
  free (yyps);
}

#define firmata_nerrs yyps->firmata_nerrs
#define yystate yyps->yystate
#define yyerrstatus yyps->yyerrstatus
#define yyssa yyps->yyssa
#define yyss yyps->yyss
#define yyssp yyps->yyssp
#define yyvsa yyps->yyvsa
#define yyvs yyps->yyvs
#define yyvsp yyps->yyvsp
#define yystacksize yyps->yystacksize


/*---------------.
| yypush_parse.  |
`---------------*/

int
yypush_parse (yypstate *yyps, int yypushed_char, YYSTYPE const *yypushed_val, FirmataMessage *result)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

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

  if (!yyps->yynew)
    {
      yyn = yypact[yystate];
      goto yyread_pushed_token;
    }

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
      if (!yyps->yynew)
        {
          YYDPRINTF ((stderr, "Return for a new token:\n"));
          yyresult = YYPUSH_MORE;
          goto yypushreturn;
        }
      yyps->yynew = 0;
yyread_pushed_token:
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yypushed_char;
      if (yypushed_val)
        yylval = *yypushed_val;
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
#line 88 "firmata_bison.y" /* yacc.c:1661  */
    {
                    result->state = STATE_DONE; }
#line 1391 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 3:
#line 90 "firmata_bison.y" /* yacc.c:1661  */
    {
                    result->state = STATE_DONE; }
#line 1398 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 4:
#line 94 "firmata_bison.y" /* yacc.c:1661  */
    {
                    result->command = PROTO_ANALOG_IO_MESSAGE;
                    result->pin = (yyvsp[-1].integer);
                    result->value = (yyvsp[0].integer);
                }
#line 1408 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 5:
#line 99 "firmata_bison.y" /* yacc.c:1661  */
    {
                    result->command = PROTO_DIGITAL_IO_MESSAGE;
                    result->pin = (yyvsp[-1].integer);
                    result->value = (yyvsp[0].integer);
                }
#line 1418 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 6:
#line 104 "firmata_bison.y" /* yacc.c:1661  */
    {
                    result->command = PROTO_PROTOCOL_VERSION;
                    result->proto_version.major = (yyvsp[-1].integer);
                    result->proto_version.minor = (yyvsp[0].integer);
                }
#line 1428 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 7:
#line 109 "firmata_bison.y" /* yacc.c:1661  */
    {
                    result->command = PROTO_SET_PIN_MODE;
                    result->pin = (yyvsp[-1].integer);

                    switch((yyvsp[0].integer)) {
                    case PROTO_PIN_MODE_INPUT:
                    case PROTO_PIN_MODE_OUTPUT:
                    case PROTO_PIN_MODE_ANALOG:
                    case PROTO_PIN_MODE_PWM:
                    case PROTO_PIN_MODE_SERVO:
                    case PROTO_PIN_MODE_I2C:
                    case PROTO_PIN_MODE_ONEWIRE:
                    case PROTO_PIN_MODE_STEPPER:
                    case PROTO_PIN_MODE_ENCODER:
                    case PROTO_PIN_MODE_SERIAL:
                    case PROTO_PIN_MODE_PULLUP:
                        result->pin_mode = (yyvsp[0].integer);
                        break;
                    default:
                        fprintf(stderr, "invalid pin mode value: %d\n", (yyvsp[0].integer));
                        result->pin_mode = PROTO_PIN_MODE_OUTPUT;
                        break;
                    }
                }
#line 1457 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 8:
#line 133 "firmata_bison.y" /* yacc.c:1661  */
    {
                    result->command = PROTO_SET_DIGITAL_PIN_VALUE;
                    result->pin = (yyvsp[-1].integer);
                    result->value = ((yyvsp[0].integer)) ? PROTO_PIN_HIGH : PROTO_PIN_LOW;
                }
#line 1467 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 10:
#line 139 "firmata_bison.y" /* yacc.c:1661  */
    { result->state = STATE_IN_PROCESS; }
#line 1473 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 11:
#line 142 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = (0xF & (yyvsp[0].integer)); }
#line 1479 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 12:
#line 143 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = ((yyvsp[-1].integer)) | ((yyvsp[0].integer) << 7); }
#line 1485 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 13:
#line 144 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = (0x7F & (yyvsp[0].integer)); }
#line 1491 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 14:
#line 146 "firmata_bison.y" /* yacc.c:1661  */
    {
                    result->command = PROTO_START_SYSEX;
                 }
#line 1499 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 15:
#line 151 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = 0; }
#line 1505 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 16:
#line 152 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = 0; }
#line 1511 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 17:
#line 153 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = 0; }
#line 1517 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 18:
#line 154 "firmata_bison.y" /* yacc.c:1661  */
    {
                    result->sysex_id = PROTO_SYSEX_ANALOG_MAPPING_RESPONSE;
                }
#line 1525 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 19:
#line 157 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = 0; }
#line 1531 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 20:
#line 158 "firmata_bison.y" /* yacc.c:1661  */
    {
                    result->sysex_id = PROTO_SYSEX_CAPABILITY_RESPONSE;
                }
#line 1539 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 21:
#line 161 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = 0; }
#line 1545 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 22:
#line 162 "firmata_bison.y" /* yacc.c:1661  */
    {
                    result->sysex_id = PROTO_SYSEX_PIN_STATE_RESPONSE;
                }
#line 1553 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 23:
#line 165 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = 0; }
#line 1559 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 24:
#line 166 "firmata_bison.y" /* yacc.c:1661  */
    {
                    result->sysex_id = PROTO_SYSEX_STRING_DATA;
                }
#line 1567 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 25:
#line 169 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = 0; }
#line 1573 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 26:
#line 170 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = 0; }
#line 1579 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 27:
#line 171 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = 0; }
#line 1585 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 28:
#line 172 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = 0; }
#line 1591 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 31:
#line 178 "firmata_bison.y" /* yacc.c:1661  */
    {
    result->sysex_id = PROTO_SYSEX_REPORT_FIRMWARE;
    result->proto_version.major = (yyvsp[-2].integer);
    result->proto_version.minor = (yyvsp[-1].integer);
}
#line 1601 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 32:
#line 184 "firmata_bison.y" /* yacc.c:1661  */
    {
                    char buf[2];
                    size_t n = decode_wchar((yyvsp[0].integer), buf);
                    firmata_message_append(result, buf, n);
                }
#line 1611 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 33:
#line 189 "firmata_bison.y" /* yacc.c:1661  */
    {
                    char buf[2];
                    size_t n = decode_wchar((yyvsp[0].integer), buf);
                    firmata_message_append(result, buf, n);
                }
#line 1621 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 34:
#line 196 "firmata_bison.y" /* yacc.c:1661  */
    {
    result->sysex_id = PROTO_SYSEX_SAMPLING_INTERVAL;
    result->value = (yyvsp[0].integer);
}
#line 1630 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 37:
#line 205 "firmata_bison.y" /* yacc.c:1661  */
    {
                        result->pin_cap_idx++;
                    }
#line 1638 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 38:
#line 208 "firmata_bison.y" /* yacc.c:1661  */
    {
                        result->pin_cap_idx++;
}
#line 1646 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 41:
#line 215 "firmata_bison.y" /* yacc.c:1661  */
    {
                        int n = result->capabilities[result->pin_cap_idx].num;
                        firmata_message_append_pin_capability(result, (yyvsp[-1].integer), (yyvsp[0].integer));
}
#line 1655 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 42:
#line 220 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = 0x0F & (yyvsp[0].integer); }
#line 1661 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 43:
#line 221 "firmata_bison.y" /* yacc.c:1661  */
    { (yyval.integer) = (yyvsp[0].integer); }
#line 1667 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 44:
#line 223 "firmata_bison.y" /* yacc.c:1661  */
    {
                            firmata_message_append_analog_mapping(result, (yyvsp[0].integer));
                        }
#line 1675 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 45:
#line 226 "firmata_bison.y" /* yacc.c:1661  */
    {
                            firmata_message_append_analog_mapping(result, (yyvsp[0].integer));
                        }
#line 1683 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 46:
#line 231 "firmata_bison.y" /* yacc.c:1661  */
    {
                            result->pin = (yyvsp[-2].integer);
                            result->pin_mode = (yyvsp[-1].integer);
                        }
#line 1692 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 47:
#line 237 "firmata_bison.y" /* yacc.c:1661  */
    {
                            result->pin_state.value = (0x7F & (yyvsp[0].integer));
                            result->pin_state.num_bytes = 1;
                        }
#line 1701 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;

  case 48:
#line 241 "firmata_bison.y" /* yacc.c:1661  */
    {
                            long v = (((long)(0x7F & (yyvsp[0].integer))) << (result->pin_state.num_bytes * 7));
                            result->pin_state.value += v;
                            result->pin_state.num_bytes++;
                        }
#line 1711 "firmata_bison.tab.c" /* yacc.c:1661  */
    break;


#line 1715 "firmata_bison.tab.c" /* yacc.c:1661  */
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
      yyerror (result, YY_("syntax error"));
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
        yyerror (result, yymsgp);
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
                      yytoken, &yylval, result);
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
                  yystos[yystate], yyvsp, result);
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
  yyerror (result, YY_("memory exhausted"));
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
                  yytoken, &yylval, result);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, result);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  yyps->yynew = 1;

yypushreturn:
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
