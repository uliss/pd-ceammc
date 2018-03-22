/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
#line 32 "firmata_bison.y" /* yacc.c:1915  */

    FirmataMessage* m;
    int integer;

#line 94 "firmata_bison.tab.h" /* yacc.c:1915  */
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
