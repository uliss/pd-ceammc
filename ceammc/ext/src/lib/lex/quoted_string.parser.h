/* A Bison parser, made by GNU Bison 3.5.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_CEAMMC_QUOTED_STRING_QUOTED_STRING_PARSER_H_INCLUDED
# define YY_CEAMMC_QUOTED_STRING_QUOTED_STRING_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef CEAMMC_QUOTED_STRING_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define CEAMMC_QUOTED_STRING_DEBUG 1
#  else
#   define CEAMMC_QUOTED_STRING_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define CEAMMC_QUOTED_STRING_DEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined CEAMMC_QUOTED_STRING_DEBUG */
#if CEAMMC_QUOTED_STRING_DEBUG
extern int ceammc_quoted_string_debug;
#endif
/* "%code requires" blocks.  */
#line 17 "quoted_string.y"

    # include <stddef.h>
    # include <stdio.h>

    typedef struct interval {
        int start;
        int end;
        int compressed;
    } t_interval;

    typedef struct param {
        size_t idx;
        t_interval** isp; // interval stack pointer
        size_t n;
        const size_t max_n;
    } t_param;

    void push_range(t_param* p, t_interval rng);
    void yyerror(t_param* param, const char *s);

#line 77 "quoted_string.parser.h"

/* Token type.  */
#ifndef CEAMMC_QUOTED_STRING_TOKENTYPE
# define CEAMMC_QUOTED_STRING_TOKENTYPE
  enum ceammc_quoted_string_tokentype
  {
    TOK_STRING_END = 0,
    TOK_DOUBLE_QUOTE = 258,
    TOK_DOUBLE_QUOTE_BEGIN = 259,
    TOK_DOUBLE_QUOTE_END = 260,
    TOK_QUOTED_ATOM = 261,
    TOK_SIMPLE_ATOM = 262
  };
#endif

/* Value type.  */
#if ! defined CEAMMC_QUOTED_STRING_STYPE && ! defined CEAMMC_QUOTED_STRING_STYPE_IS_DECLARED
typedef t_interval CEAMMC_QUOTED_STRING_STYPE;
# define CEAMMC_QUOTED_STRING_STYPE_IS_TRIVIAL 1
# define CEAMMC_QUOTED_STRING_STYPE_IS_DECLARED 1
#endif



#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct ceammc_quoted_string_pstate ceammc_quoted_string_pstate;

int ceammc_quoted_string_push_parse (ceammc_quoted_string_pstate *ps, int pushed_char, CEAMMC_QUOTED_STRING_STYPE const *pushed_val, t_param* p);

ceammc_quoted_string_pstate * ceammc_quoted_string_pstate_new (void);
void ceammc_quoted_string_pstate_delete (ceammc_quoted_string_pstate *ps);

#endif /* !YY_CEAMMC_QUOTED_STRING_QUOTED_STRING_PARSER_H_INCLUDED  */
